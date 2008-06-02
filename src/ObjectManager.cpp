#include "ObjectManager.h"

using namespace Discover;

// Definition with an explicit initializer.
// Reference: http://www.informit.com/guides/content.asp?g=cplusplus&seqNum=298&rl=1
int ObjectManager::intObjectTotal = 0;

//---------------------------------------------------------------------
ObjectManager::ObjectManager() {
    // Instantiate render.
    renderRoot = new Ogre::Root();
}
//---------------------------------------------------------------------
ObjectManager::~ObjectManager() {
    // Delete the physics world.
    if(physicsWorld) {delete physicsWorld;physicsWorld = NULL;}

    // Delete the render root.
    if(renderRoot) {renderRoot->shutdown();delete renderRoot;renderRoot = NULL;}
}
//---------------------------------------------------------------------
bool ObjectManager::instantiateThis() {
    intObjectTotal = 0;

    // Create the SceneManager, in renderSceneMgr case a generic one.
    renderSceneMgr = renderRoot->createSceneManager(Ogre::ST_GENERIC);

    // Set default mipmap level.
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    //renderOverlayMgr = new Ogre::OverlayManager();
    renderOverlayMgr = Ogre::OverlayManager::getSingletonPtr();

    // Create earth gravity force of -9.80665 m/(s^2) on y-coordinate.
    Ogre::Vector3 gravity(0, -9.81, 0);
    // Create bounding box.
    Ogre::AxisAlignedBox bounds(Ogre::Vector3(-1000, -100, -1000), Ogre::Vector3(1000,  1000,  1000));
    // Create the physics world.
    physicsWorld = new OgreBulletDynamics::DynamicsWorld(renderSceneMgr, bounds, gravity);

    return true;
}
//---------------------------------------------------------------------
void ObjectManager::registerFrameListener() {}
//---------------------------------------------------------------------
void ObjectManager::createMeshStatic(const Ogre::String& uniquename, const Ogre::String& filename, Ogre::Vector3 position, Ogre::Quaternion orientation) {
    Ogre::Entity* ent = renderSceneMgr->createEntity(uniquename, filename);
    ent->setQueryFlags(ObjectManager::GEOMETRY_QUERY_MASK);

    OgreBulletCollisions::MeshToShapeConverter* mesh = new OgreBulletCollisions::MeshToShapeConverter(ent);
    OgreBulletDynamics::RigidBody* b = new OgreBulletDynamics::RigidBody("rigidMesh_" + uniquename, physicsWorld);
    
    Ogre::SceneNode* node = renderSceneMgr->getRootSceneNode()->createChildSceneNode("nodeScene_" + uniquename);
    node->attachObject(ent);

    b->setStaticShape(node, mesh->createConvex(), getRestitution(ObjectManager::MAT_CONCRETE), getFriction(ObjectManager::MAT_GENERIC), position, orientation);
    delete mesh;
}
//---------------------------------------------------------------------
OgreBulletDynamics::RigidBody* ObjectManager::createSceneStatic(Ogre::SceneManager* mSceneMgr, OgreBulletDynamics::DynamicsWorld* mPhysicsWorld, const Ogre::String& uniquename, const Ogre::String& filename, const Ogre::Vector3 position, const Ogre::Quaternion orientation, const float friction, bool shadow) {
    Ogre::Entity* ent = mSceneMgr->createEntity(uniquename, filename);
    ent->setCastShadows(shadow);

    OgreBulletCollisions::MeshToShapeConverter* mesh = new OgreBulletCollisions::MeshToShapeConverter(ent);
    OgreBulletDynamics::RigidBody* b = new OgreBulletDynamics::RigidBody("rigidMesh_" + uniquename, mPhysicsWorld);
    
    Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode("nodeScene_" + uniquename);
    node->attachObject(ent);

    b->setStaticShape(node, mesh->createTrimesh(), getRestitution(ObjectManager::MAT_CONCRETE), friction, position, orientation);
    delete mesh;
    return b;
}
//---------------------------------------------------------------------
void ObjectManager::createMeshDynamic(const Ogre::String& uniquename, const Ogre::String& filename, Ogre::Real mass, Ogre::Vector3 position, Ogre::Quaternion orientation) {
    Ogre::Entity* ent = renderSceneMgr->createEntity(uniquename, filename);
    ent->setQueryFlags(ObjectManager::GEOMETRY_QUERY_MASK);

    OgreBulletCollisions::BoxCollisionShape* cube = new OgreBulletCollisions::BoxCollisionShape(getBoundingBox(ent));
    OgreBulletDynamics::RigidBody* b = new OgreBulletDynamics::RigidBody("rigidCube_" + uniquename, physicsWorld);
    
    Ogre::SceneNode* node = renderSceneMgr->getRootSceneNode()->createChildSceneNode("nodeScene_" + uniquename);
    node->attachObject(ent);

    b->setShape(node, cube, getRestitution(ObjectManager::MAT_GENERIC), getFriction(ObjectManager::MAT_GENERIC), mass, position, orientation);
}
//---------------------------------------------------------------------
void ObjectManager::createScaleMeshDynamic(const Ogre::String& uniquename, const Ogre::String& filename, Ogre::Real mass, Ogre::Vector3 scalerender, Ogre::Vector3 scalecollision, Ogre::Vector3 position, Ogre::Quaternion orientation) {
    Ogre::Entity* ent = renderSceneMgr->createEntity(uniquename, filename);
    ent->setQueryFlags(ObjectManager::GEOMETRY_QUERY_MASK);

    OgreBulletCollisions::BoxCollisionShape* cube = new OgreBulletCollisions::BoxCollisionShape(scalecollision);
    OgreBulletDynamics::RigidBody* b = new OgreBulletDynamics::RigidBody("rigidCube_" + uniquename, physicsWorld);
    
    Ogre::SceneNode* node = renderSceneMgr->getRootSceneNode()->createChildSceneNode("nodeScene_" + uniquename);
    node->setScale(scalerender);
    node->attachObject(ent);

    b->setShape(node, cube, getRestitution(ObjectManager::MAT_GENERIC), getFriction(ObjectManager::MAT_GENERIC), mass, position, orientation);
}
//---------------------------------------------------------------------
float ObjectManager::getFriction(ObjectManager::MaterialCommon physical) {
    switch(physical) {
        case ObjectManager::MAT_GENERIC:
            return 0.5;
        case ObjectManager::MAT_ZERO:
            return 0;
        case ObjectManager::MAT_CONCRETE:
            return 1;
        case ObjectManager::MAT_WOOD:
            return 0.4;
        case ObjectManager::MAT_METAL:
            return 0.2;
        default:
            return 0;
    }
}
//---------------------------------------------------------------------
float ObjectManager::getRestitution(ObjectManager::MaterialCommon physical) {
    switch(physical) {
        case ObjectManager::MAT_GENERIC:
            return 0.1;
        case ObjectManager::MAT_ZERO:
            return 0;
        case ObjectManager::MAT_CONCRETE:
            return 0;
        case ObjectManager::MAT_WOOD:
            return 0.6;
        case ObjectManager::MAT_METAL:
            return 0.3;
        default:
            return 0;
    }
}
//---------------------------------------------------------------------
void ObjectManager::setMaterialByName(const Ogre::String& name, const Ogre::String& materialname, MaterialCommon physical) {
    Ogre::Entity* ent = renderSceneMgr->getEntity(name);
    ent->setMaterialName(materialname);
    ent->setNormaliseNormals(true);
}
//---------------------------------------------------------------------
Ogre::String ObjectManager::toString(ObjectManager::MaterialCommon physical) {
    switch(physical) {
        case ObjectManager::MAT_GENERIC:
            return "MaterialCommon_Generic";
        case ObjectManager::MAT_ZERO:
            return "MaterialCommon_Zero";
        case ObjectManager::MAT_WOOD:
            return "MaterialCommon_Wood";
        case ObjectManager::MAT_METAL:
            return "MaterialCommon_Metal";
        default:
            return "";
    }
}
//---------------------------------------------------------------------
void ObjectManager::addForceByName(const Ogre::String& name, Ogre::Vector3 direction) {
    OgreBulletDynamics::RigidBody* b = static_cast<OgreBulletDynamics::RigidBody*>(physicsWorld->findObject(renderSceneMgr->getEntity(name)->getParentSceneNode()));
    b->enableActiveState();
    b->setLinearVelocity(direction);
}
//---------------------------------------------------------------------
 OgreBulletDynamics::RigidBody* ObjectManager::getPhysicsBodyByName(const Ogre::String& name) {
    OgreBulletDynamics::RigidBody* b = static_cast<OgreBulletDynamics::RigidBody*>(physicsWorld->findObject(renderSceneMgr->getEntity(name)->getParentSceneNode()));
    return b;
}
 //---------------------------------------------------------------------
Ogre::Vector3 ObjectManager::getBoundingBox(Ogre::Entity* ent) {
    Ogre::AxisAlignedBox ebb = ent->getBoundingBox();
    ebb.scale(Ogre::Vector3(0.49, 0.49, 0.49));

    return ebb.getMaximum() - ebb.getMinimum();
}
//---------------------------------------------------------------------
Ogre::Vector3 ObjectManager::getBoundingBoxByFile(const Ogre::String& filename) {
    Ogre::Entity* ent = renderSceneMgr->createEntity("getBoundingBoxByFile", filename);
    Ogre::AxisAlignedBox ebb = ent->getBoundingBox();
    renderSceneMgr->destroyEntity(ent);
    ebb.scale(Ogre::Vector3(0.49, 0.49, 0.49));

    return ebb.getMaximum() - ebb.getMinimum();
}
//---------------------------------------------------------------------
Ogre::Vector3 ObjectManager::getBoundingBoxByName(const Ogre::String& name) {
    Ogre::Entity* ent = renderSceneMgr->getEntity(name);
    Ogre::AxisAlignedBox ebb = ent->getBoundingBox();
    renderSceneMgr->destroyEntity(ent);
    ebb.scale(Ogre::Vector3(0.49, 0.49, 0.49));

    return ebb.getMaximum() - ebb.getMinimum();
}
//---------------------------------------------------------------------
void ObjectManager::updateTracker(const Ogre::String& name, const Ogre::String& description) {
    intObjectTotal++;
    Ogre::LogManager::getSingleton().logMessage("ObjectManager - " + description + ": " + name); 
}
//---------------------------------------------------------------------