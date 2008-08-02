#include "EngineHeaders.h"

using namespace Discover;

//---------------------------------------------------------------------
DebugListener::DebugListener() {if(!instantiateThis()) return;}
//---------------------------------------------------------------------
DebugListener::~DebugListener() {
    delete glbObject->physicsWorld->getDebugDrawer();
    glbObject->physicsWorld->setDebugDrawer(0);
}
//---------------------------------------------------------------------
bool DebugListener::instantiateThis() {
    // Instantiate debugging.
    mDebugOverlay = glbObject->renderOverlayMgr->getByName("Core/DebugOverlay");
    mTimeUntilNextToggle = 0;

    // Instantiate camera control. Default: Position it at 500 in Z direction. Default: Look back along -Z.
    mCtrlCamera.instantiateThis();
    mCtrlCamera.createCamera("FreelookCamera", Ogre::Vector3(0, 1, 5));
    mCtrlCamera.switchCamera("FreelookCamera");
    mCtrlCamera.pointCamera(Ogre::Vector3(0, 0, -100));

    // Instantiate user control.
    mCtrlUser.instantiateThis();
    mCtrlUser.setMode(UserControl::UM_DEFAULT);
    mCtrlUser.setRotateSpeed(0.13);
    mCtrlUser.setMoveSpeed(6);

    // Instantiate pointer.
    mCursorOverlay = EffectControl::createOverlay("PointerOverlay");
    mCursorOverlay->show();
    mCursor = EffectControl::createOverlayContainer("Pointer", mCursorOverlay, Ogre::Vector2((glbObject->renderWindow->getWidth() / 2) - 16, (glbObject->renderWindow->getHeight() / 2) - 16), Ogre::Vector2(32, 32), "nx.arrow");
    mCursor->hide();

    // Instantiate physics debug.
    OgreBulletCollisions::DebugDrawer *debugDrawer = new OgreBulletCollisions::DebugDrawer();
    glbObject->physicsWorld->setDebugDrawer(debugDrawer);
    Ogre::SceneNode *debugNode = glbObject->renderSceneMgr->getRootSceneNode()->createChildSceneNode("debugDrawer", Ogre::Vector3::ZERO);
    debugNode->attachObject(static_cast<SimpleRenderable*>(debugDrawer));

    // Instantiate pick.
    mPickConstraint = NULL;
    targetBody = NULL;
    hasTargetBody = false;
    targetNode = glbObject->renderSceneMgr->getRootSceneNode()->createChildSceneNode("PickTarget");
    Ogre::Entity* mEnt =  glbObject->renderSceneMgr->createEntity("nx.bodyguide.ent", "nx.bodyguide.mesh");
    mEnt->setCastShadows(false);
    targetNode->attachObject(mEnt);
    targetNode->scale(1.2, 0.2, 1.2);
    targetNode->setVisible(false);

    // Instantiate drag line for pick.
    mDragLineNode = glbObject->renderSceneMgr->getRootSceneNode()->createChildSceneNode();
    mDragLine = new Ogre::ManualObject("PointerDragLine");

    // Instantiate drag points for pick.
    mDragPoint1 = glbObject->renderSceneMgr->createEntity("PickBall_1", "ball.mesh");
    mDragPoint2 = glbObject->renderSceneMgr->createEntity("PickBall_2", "ball.mesh");
    mDragPoint1->setMaterialName("Material/basicRed");
    mDragPoint2->setMaterialName("Material/basicGreen");
    mDragPoint1->setCastShadows(false);
    mDragPoint2->setCastShadows(false);
    mDragPoint1->setVisible(false);
    mDragPoint2->setVisible(false);
    mDragPoint1Node = glbObject->renderSceneMgr->getRootSceneNode()->createChildSceneNode();
    mDragPoint2Node = glbObject->renderSceneMgr->getRootSceneNode()->createChildSceneNode();
    mDragPoint1Node->attachObject(mDragPoint1);
    mDragPoint2Node->attachObject(mDragPoint2);
    mDragPoint1Node->setScale(0.1, 0.1, 0.1);
    mDragPoint2Node->setScale(0.1, 0.1, 0.1);

    return true;
}
//---------------------------------------------------------------------
void DebugListener::setPhysicsDebug(DebugListener::PhysicsOption option) {
    OgreBulletCollisions::DebugDrawer *debugDrawer = glbObject->physicsWorld->getDebugDrawer();
    bool wasWireframeShapes;

    switch(option) {
        case DebugListener::PHY_WIREFRAME:
            wasWireframeShapes = debugDrawer->doesDrawWireframe();
            debugDrawer->setDrawWireframe(!wasWireframeShapes);
            glbObject->physicsWorld->setShowDebugShapes(!wasWireframeShapes);
            break;
        case DebugListener::PHY_AABB:
            debugDrawer->setDrawAabb(!debugDrawer->doesDrawAabb());
            break;
        case DebugListener::PHY_TEXT_FEATURES:
            debugDrawer->setDrawFeaturesText(!debugDrawer->doesDrawFeaturesText());
            break;
        case DebugListener::PHY_CONTACT:
            debugDrawer->setDrawContactPoints(!debugDrawer->doesDrawContactPoints());
            break;
        case DebugListener::PHY_DEACTIVATION:
            debugDrawer->setNoDeactivation(!debugDrawer->doesNoDeactivation());
            break;
        case DebugListener::PHY_TEXT_NOHELP:
            debugDrawer->setNoHelpText(!debugDrawer->doesNoHelpText());
            break;
        case DebugListener::PHY_TEXT_DRAW:
            debugDrawer->setDrawText(!debugDrawer->doesDrawText());
            break;
        case DebugListener::PHY_PROFILETIMINGS:
            debugDrawer->setProfileTimings(!debugDrawer->doesProfileTimings());
            break;
        case DebugListener::PHY_SAT:
            debugDrawer->setEnableSatComparison(!debugDrawer->doesEnableSatComparison());
            break;
        case DebugListener::PHY_LCP:
            debugDrawer->setDisableBulletLCP(!debugDrawer->doesDisableBulletLCP());
            break;
        case DebugListener::PHY_CCD:
            debugDrawer->setEnableCCD(!debugDrawer->doesEnableCCD());
            break;
    }
}
//---------------------------------------------------------------------
void DebugListener::pickObject(bool isPick) {
    Ogre::Real x = mCursor->getLeft();
    Ogre::Real y = mCursor->getTop();
    Ogre::Ray r = mCtrlCamera.getCamera()->getCameraToViewportRay(x / glbObject->renderWindow->getWidth(), y / glbObject->renderWindow->getHeight());
    
    // Move object.
    if(isPick) {
        if(hasTargetBody) {
            // Calculate the drag plane.
            // The normal is the ray direction (alternative is to use camera direction).
            Ogre::Plane dragPlane;
            dragPlane.normal = -r.getDirection();
            dragPlane.d = -mCursorIntersectPoint.dotProduct(dragPlane.normal);
            // Intersect cursor ray with plane.
            std::pair<bool, Real> res;
            res = r.intersects(dragPlane);
            Ogre::Vector3 objPoint;
            if(res.first) {
                mCursorIntersectPoint = r.getPoint(res.second);
                objPoint = this->dragObject(mCursorIntersectPoint);
            }
            // Set drag line, draw 3D line and position.
            this->remove3DLine();
            mDragLine->begin("Material/debugGraphics", Ogre::RenderOperation::OT_LINE_LIST);
            mDragLine->position(mCursorIntersectPoint); // From.
            mDragLine->position(objPoint); // To.
            mDragLine->end();
            mDragLineNode->attachObject(mDragLine);
            // Set drag points, visible and position.
            mDragPoint1Node->setPosition(mCursorIntersectPoint);
            mDragPoint2Node->setPosition(objPoint);
            mDragPoint1->setVisible(true);
            mDragPoint2->setVisible(true);
        }
        return;
    }
    // Grab object.
    if(!isPick) {
        this->remove3DLine();
        if(mPickConstraint != NULL) {
            glbObject->physicsWorld->removeConstraint(mPickConstraint);
            delete mPickConstraint;mPickConstraint = NULL;
        }
        OgreBulletDynamics::RigidBody* t = this->getPhysicsBody(r);
        // Cast ray.
        if(t == NULL) {
            hasTargetBody = false;
            return;
        } else {
            // Can't add forces to static objects.
            if(t->isStaticObject()) {
                targetBody = NULL;
                hasTargetBody = false;
                return;
            }
            // Can't add forces to kinematics objects.
            if(t->isKinematicObject()) {
                return;
            }
            targetBody = t;
            hasTargetBody = true;
            mCursorIntersectPoint = mHitPoint;

            // Get local hit point from world hit point.
            mLocalPoint = mHitPoint - targetBody->getCenterOfMassPosition();

            /*OgreBulletDynamics::PointToPointConstraint* p2p = new OgreBulletDynamics::PointToPointConstraint(targetBody, mLocalPoint);
            glbObject->physicsWorld->addConstraint(p2p);
            p2p->setTau(0.1);
            mPickConstraint = p2p;*/
        }
    }
}
//---------------------------------------------------------------------
Ogre::Vector3 DebugListener::dragObject(Ogre::Vector3& cursor) {
    targetBody->enableActiveState();
    Ogre::Vector3 objPoint = mLocalPoint;

    /*if(mPickConstraint != NULL) {
        OgreBulletDynamics::PointToPointConstraint* p2p = static_cast<OgreBulletDynamics::PointToPointConstraint*>(mPickConstraint);
        p2p->setPivotA(mLocalPoint);
        p2p->setPivotB(cursor - Ogre::Vector3(0, 1, 0));
    }*/

    mDebugText2 = "Local: " + Ogre::StringConverter::toString(mLocalPoint);
    mDebugText2 += " | Intersect: " + Ogre::StringConverter::toString(mCursorIntersectPoint);

    objPoint += targetBody->getWorldPosition();
    
    // Add drag force.
    Ogre::Vector3 force = cursor - objPoint;
    force *= 4;
    //force -= OgreBulletCollisions::BtOgreConverter::to(targetBody->getBulletRigidBody()->getLinearVelocity());
    targetBody->setLinearVelocity(force);

    return objPoint;
}
//---------------------------------------------------------------------
void DebugListener::remove3DLine() {
    mDragPoint1->setVisible(false);
    mDragPoint2->setVisible(false);
    mDragLineNode->detachAllObjects();
    mDragLine->clear();
}
//---------------------------------------------------------------------
OgreBulletDynamics::RigidBody* DebugListener::getPhysicsBody(Ogre::Ray r) {
    // Use physics to cast ray.
    btVector3 btRayFrom = OgreBulletCollisions::OgreBtConverter::to(r.getOrigin());
    btVector3 btRayTo = OgreBulletCollisions::OgreBtConverter::to(r.getPoint(10000));
    btCollisionWorld::ClosestRayResultCallback mClosestRayResultCallback(btRayFrom, btRayTo);
    glbObject->physicsWorld->getBulletCollisionWorld()->rayTest(btRayFrom, btRayTo, mClosestRayResultCallback);
    if(mClosestRayResultCallback.hasHit()) {
        OgreBulletDynamics::RigidBody* b = static_cast<OgreBulletDynamics::RigidBody*>(glbObject->physicsWorld->findObject(mClosestRayResultCallback.m_collisionObject));
        mHitPoint = OgreBulletCollisions::BtOgreConverter::to(mClosestRayResultCallback.m_hitPointWorld);
        mDebugText1 = "Hit: " + Ogre::StringConverter::toString(mHitPoint);
        mDebugText1 += " | Name: " + b->getName();
        return b;
    }
    return NULL;
}
//---------------------------------------------------------------------
Ogre::Entity* DebugListener::getEntity(Ogre::Ray r) {
    Ogre::RaySceneQuery* mRayQuery = glbObject->renderSceneMgr->createRayQuery(r);
    mRayQuery->setQueryMask(ObjectManager::GEOMETRY_QUERY_MASK);
    mRayQuery->setQueryTypeMask(Ogre::SceneManager::ENTITY_TYPE_MASK);
    mRayQuery->setSortByDistance(true); // Sort by distance and get first hit.
    Ogre::RaySceneQueryResult& result = mRayQuery->execute();
    for(Ogre::RaySceneQueryResult::iterator itr = result.begin();itr != result.end();itr++) {
        if(itr->movable != NULL && itr->distance > 0) {
            size_t vertex_count, index_count;
            Ogre::Vector3* vertices;
            unsigned long* indices;
            Ogre::Entity* pentity = static_cast<Ogre::Entity*>(itr->movable);
            Utilities::getMeshInformation(pentity->getMesh(), vertex_count, vertices, index_count, indices, pentity->getParentNode()->getWorldPosition(), pentity->getParentNode()->getWorldOrientation(), pentity->getParentNode()->getScale());
            // Iterate through all the vertices for intersection
            for(int i = 0;i < static_cast<int>(index_count);i += 3) {
                std::pair<bool, float> hit = Ogre::Math::intersects(Ogre::Ray(r), vertices[i], vertices[i+1], vertices[i+2], true, false);
                mHitPoint = r.getPoint(Ogre::Real(hit.second));
            }
            delete[] vertices;
            delete[] indices;
        }
    }
    mRayQuery->clearResults();
    return NULL;
}
//---------------------------------------------------------------------
void DebugListener::Shoot() {
    // Declaration.
    Ogre::Vector3 vecDirection, vecDirectionPreset;
    Ogre::Quaternion quatCameraOrientation;
    Ogre::Vector3 vecCurPosition;
    Ogre::String strName = "Shoot_" + Ogre::StringConverter::toString(count++);

    // Get camera position and orientation.
    quatCameraOrientation = mCtrlCamera.getOrientation();
    vecDirectionPreset = Ogre::Vector3(0, 0, -10);
    vecDirection = quatCameraOrientation * vecDirectionPreset;
    vecCurPosition = mCtrlCamera.getPosition();
    
    glbObject->createScaleMeshDynamic(strName, "scale1cm.mesh", 4, Ogre::Vector3(0.4, 0.4, 0.4), Ogre::Vector3(0.2, 0.2, 0.2), vecCurPosition, quatCameraOrientation);
    glbObject->setMaterialByName(strName, "Examples/10PointBlock", ObjectManager::MAT_ZERO);
    glbObject->addForceByName(strName, vecDirection * 3);
}
//---------------------------------------------------------------------
void DebugListener::updateStats() {
    static Ogre::String currFps = "Current FPS: ";
    static Ogre::String avgFps = "Average FPS: ";
    static Ogre::String bestFps = "Best FPS: ";
    static Ogre::String worstFps = "Worst FPS: ";
    static Ogre::String tris = "Triangle Count: ";
    static Ogre::String batches = "Batch Count: ";

    // Update stats when necessary.
    try {
        Ogre::OverlayElement* guiAvg = glbObject->renderOverlayMgr->getOverlayElement("Core/AverageFps");
        Ogre::OverlayElement* guiCurr = glbObject->renderOverlayMgr->getOverlayElement("Core/CurrFps");
        Ogre::OverlayElement* guiBest = glbObject->renderOverlayMgr->getOverlayElement("Core/BestFps");
        Ogre::OverlayElement* guiWorst = glbObject->renderOverlayMgr->getOverlayElement("Core/WorstFps");

        const Ogre::RenderTarget::FrameStats& stats = glbObject->renderWindow->getStatistics();

        guiAvg->setCaption(avgFps + Ogre::StringConverter::toString(stats.avgFPS));
        guiCurr->setCaption(currFps + Ogre::StringConverter::toString(stats.lastFPS));
        guiBest->setCaption(bestFps + Ogre::StringConverter::toString(stats.bestFPS) + " " + Ogre::StringConverter::toString(stats.bestFrameTime) + " ms");
        guiWorst->setCaption(worstFps + Ogre::StringConverter::toString(stats.worstFPS) + " " + Ogre::StringConverter::toString(stats.worstFrameTime) + " ms");

        Ogre::OverlayElement* guiTris = glbObject->renderOverlayMgr->getOverlayElement("Core/NumTris");
        guiTris->setCaption(tris + Ogre::StringConverter::toString(stats.triangleCount));

        Ogre::OverlayElement* guiBatches = glbObject->renderOverlayMgr->getOverlayElement("Core/NumBatches");
        guiBatches->setCaption(batches + Ogre::StringConverter::toString(stats.batchCount));

        Ogre::OverlayElement* guiDbg = glbObject->renderOverlayMgr->getOverlayElement("Core/DebugText");
        guiDbg->setCaption("Debug Caption: " + mCtrlUser.getDebugText() + " | " + mDebugText1 + "\n" + mDebugText2);
    } catch(...) { /* ignore */ }
}
//---------------------------------------------------------------------
void DebugListener::showDebugOverlay(bool show) {
    if(mDebugOverlay) {
        if(show) {
            mDebugOverlay->show();
        } else {
            mDebugOverlay->hide();
        }
    }
}
//---------------------------------------------------------------------
bool DebugListener::frameStarted(const Ogre::FrameEvent& evt) {
    this->showDebugOverlay(glbCfgVar.debugStats);
    if(!mCtrlUser.getUserInputs(mCtrlCamera, evt)) return false;
    mCtrlCamera.moveCamera(mCtrlUser.getTranslate(), mCtrlUser.getRotationX(), mCtrlUser.getRotationY());

    switch(mCtrlUser.getMode()) {
        case UserControl::UM_DEBUGPICK:
            mCursor->show();
            mCursor->setPosition(mCursor->getLeft() + mCtrlUser.getPointerX(), mCursor->getTop() + mCtrlUser.getPointerY());

            if(mCtrlUser.getMouseInputAction() == "move") {
                this->pickObject(true);
            } else if(mCtrlUser.getMouseInputAction() == "grab") {
                this->pickObject(false);
            } else {
                this->remove3DLine();
            }

            // Is target picked.
            if(hasTargetBody == false) {
                targetNode->setVisible(false);
            } else {
                Ogre::Vector3 p = targetBody->getWorldPosition();
                p.y = 0.01;
                targetNode->setPosition(p);	
                targetNode->setVisible(true);
            }
            break;
        default:
            mCursor->hide();
            if(mCtrlUser.isKeyInputAction())
                this->Shoot();
            break;
    }

    return true;
}
//---------------------------------------------------------------------
bool DebugListener::frameEnded(const Ogre::FrameEvent& evt) {
    updateStats();

    return true;
}
//---------------------------------------------------------------------