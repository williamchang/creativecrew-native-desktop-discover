#include "CameraControl.h"

using namespace Discover;

//---------------------------------------------------------------------
CameraControl::CameraControl():
    mWindow(0), 
    mSceneMgr(0), 
    mNode(0),
    mViewport(0),
    mCamera(0),
    mTranslateVector(Ogre::Vector3::ZERO),
    mRotateX(0),
    mRotateY(0)
    {}
//---------------------------------------------------------------------
CameraControl::CameraControl(Ogre::RenderWindow* rw, Ogre::SceneManager* sm, Ogre::Camera* cam):
    mWindow(rw),
    mSceneMgr(sm),
    mCamera(cam),
    mViewport(0),
    mNode(0),
    mTranslateVector(Ogre::Vector3::ZERO),
    mRotateX(0),
    mRotateY(0)
    {}
//---------------------------------------------------------------------
CameraControl::~CameraControl() {}
//---------------------------------------------------------------------
bool CameraControl::instantiateThis() {
    mSceneMgr = glbObject->renderSceneMgr;
    mWindow = glbObject->renderWindow;
    mCamera = glbObject->renderCamera;
    mPerspective = CameraControl::CAM_FREELOOK;

    return true;
}
//---------------------------------------------------------------------
void CameraControl::createCamera(Ogre::String uniquename, Ogre::Vector3 position, Ogre::Real nearclipdistance) {
    Ogre::String strParentNode = "CameraAllNode_" + uniquename;
    Ogre::String strChildNode = "CameraNode_" + uniquename;

    // Create a camera.
    mCamera = mSceneMgr->createCamera(uniquename);
    // Set camera clip distance.
    mCamera->setNearClipDistance(nearclipdistance);

    // Create a node
    mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(strChildNode, position);
    // Attach camera to node.
    mNode->attachObject(mCamera);
}
//---------------------------------------------------------------------
void CameraControl::createCameraDefault() {
    mWindow = glbObject->renderWindow;
    mSceneMgr = glbObject->renderSceneMgr;
    mCamera = glbObject->renderCamera;
    mPerspective = CameraControl::CAM_DEFAULT;

    this->createCamera("DefaultCamera");
    this->createViewportFull();
}
//---------------------------------------------------------------------
void CameraControl::createViewportFull() {
    // Create one viewport on entire window.
    mViewport = mWindow->addViewport(mCamera);
    mViewport->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
    // Alter the camera aspect ratio to match the viewport.
    mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));
}
//---------------------------------------------------------------------
void CameraControl::pointCamera(Ogre::Vector3 lookat) {
    mNode->lookAt(lookat, Ogre::Node::TS_WORLD);
}
//---------------------------------------------------------------------
void CameraControl::setCamera(Perspective mode, Ogre::String targetname) {
    mPerspective = mode;
    //mNodeTarget = glbObject->renderSceneMgr->getEntity(targetname)->getParentNode();

    switch(mode) {
        case CameraControl::CAM_THIRDPERSON:
            mNode->setAutoTracking(true, mNodeTarget);
            mNode->setFixedYawAxis(true);
            break;
        case CameraControl::CAM_TOP:
            mNode->setAutoTracking(true, mNodeTarget);
            mNode->setFixedYawAxis(true);
            break;
        case CameraControl::CAM_SIDE:
            mNode->setAutoTracking(true, mNodeTarget);
            mNode->setFixedYawAxis(true);
            break;
    }
}
//---------------------------------------------------------------------
void CameraControl::moveCamera(Ogre::Vector3 move, Ogre::Radian rotateX, Ogre::Radian rotateY) {
    // Note that yaw direction is around a fixed axis (freelook style) rather than a natural yaw (e.g. airplane)
    mNode->translate(move, Ogre::Node::TS_LOCAL);
    mNode->yaw(rotateX, Ogre::Node::TS_WORLD);

    // Setup for first person (free-look) camera.
    mCameraYaw -= Ogre::Degree(rotateX);
    mCameraPitch += Ogre::Degree(rotateY);
    if(mCameraPitch < Ogre::Degree(-90)) {mCameraPitch = Ogre::Degree(-90);}
    if(mCameraPitch > Ogre::Degree(90)) {mCameraPitch = Ogre::Degree(90);}
    mNode->setOrientation(Ogre::Quaternion(mCameraYaw, Ogre::Vector3::NEGATIVE_UNIT_Y) * Ogre::Quaternion(mCameraPitch, Ogre::Vector3::UNIT_X));
}
//---------------------------------------------------------------------
void CameraControl::runOneFrame(const Ogre::FrameEvent& evt) {}
//---------------------------------------------------------------------
Ogre::Camera* CameraControl::getCamera() {
    return mViewport->getCamera();
}
//---------------------------------------------------------------------
Ogre::Vector3 CameraControl::getPosition() {
    return mNode->getPosition();
}
//---------------------------------------------------------------------
Ogre::Quaternion CameraControl::getOrientation() {
    return mNode->getOrientation();
}
//---------------------------------------------------------------------
void CameraControl::switchCamera(Ogre::String name) {
    Ogre::String strParentNode = "CameraAllNode_" + name;
    Ogre::String strChildNode = "CameraNode_" + name;

    mViewport = mWindow->getViewport(0);
    mViewport->setCamera(mSceneMgr->getCamera(name));
}
//---------------------------------------------------------------------
