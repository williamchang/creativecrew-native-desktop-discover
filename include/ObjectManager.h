/**
@file
    ObjectManager.h
@brief
    Coming soon.
@author
    William Chang
@version
    0.1
@date
    - Created: 2006-11-04
    - Modified: 2008-06-02
    .
@note
    References:
    - Ogre:
        - http://www.ogre3d.org/wiki/index.php/Documentation_Architecture
        - http://www.ogre3d.org/docs/api/html/
        .
    - NxOgre scale use SI base units:
        - Length is in meters (m).
        - Density (Mass) is in kilogram^3 (kg^3)
        - Time is in seconds (s).
        .
    - NxOgre material: 
        - Static Friction: How difficult it is to start the object moving from rest with the high static friction, but once you get it moving, it slides fairly easily.
        - Dynamic Friction: The box glides very easily over the plane with the low dynamic friction. Dynamic friction is a stopping force against the object while it is moving along the ground or other surface.
        .
    .
*/

#ifndef __ObjectManager_H__
#define __ObjectManager_H__

#include <OgreBulletCollisionsShape.h>
#include <Shapes/OgreBulletCollisionsBoxShape.h>
#include <Shapes/OgreBulletCollisionsConvexHullShape.h>
#include <Shapes/OgreBulletCollisionsTrimeshShape.h>
#include <Utils/OgreBulletCollisionsMeshToShapeConverter.h>
#include <OgreBulletCollisionsRay.h>
#include <Debug/OgreBulletCollisionsDebugLines.h>
#include <OgreBulletDynamicsWorld.h>
#include <OgreBulletDynamicsRigidBody.h>

namespace Discover {

/** @class ObjectManager */
class ObjectManager {
public:
    /** Query Flags. */
    enum QueryFlags {
        ANY_QUERY_MASK = 1<<0,
        RAGDOLL_QUERY_MASK = 1<<1,
        GEOMETRY_QUERY_MASK = 1<<2,
        VEHICLE_QUERY_MASK = 1<<3,
        STATIC_GEOMETRY_QUERY_MASK = 1<<4
    };
    /** Material Common. */
    enum MaterialCommon {
        MAT_GENERIC = 0,
        MAT_ZERO,
        MAT_STICKY,
        MAT_BOUNCY,
        MAT_CONCRETE,
        MAT_WOOD,
        MAT_METAL,
        MAT_RUBBER,
        MAT_PLASTIC
    };

    /// Default constructor.
    ObjectManager();
    /// Default destructor.
    virtual ~ObjectManager();

    /// Instantiate manager.
    bool instantiateThis();
    /// Register manager's frame listeners.
    void registerFrameListener();

    /// Create physics static mesh.
    void createMeshStatic(const Ogre::String& uniquename, const Ogre::String& filename, Ogre::Vector3 position = Ogre::Vector3::ZERO, Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY);
    /// Create physics static mesh for scene.
    static OgreBulletDynamics::RigidBody* createSceneStatic(Ogre::SceneManager* mSceneMgr, OgreBulletDynamics::DynamicsWorld* mPhysicsWorld, const Ogre::String& uniquename, const Ogre::String& filename, const Ogre::Vector3 position, const Ogre::Quaternion orientation, const float friction, bool shadow = true);
    /// Create physics dynamic mesh.
    void createMeshDynamic(const Ogre::String& uniquename, const Ogre::String& filename, Ogre::Real mass, Ogre::Vector3 position = Ogre::Vector3::ZERO, Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY);
    /// Create physics dynamic mesh.
    void createScaleMeshDynamic(const Ogre::String& uniquename, const Ogre::String& filename, Ogre::Real mass, Ogre::Vector3 scalerender = Ogre::Vector3(1, 1, 1), Ogre::Vector3 scalecollision = Ogre::Vector3(1, 1, 1), Ogre::Vector3 position = Ogre::Vector3::ZERO, Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY);

    /// Set render material and physical material, by name to mesh.
    void setMaterialByName(const Ogre::String& name, const Ogre::String& materialname, MaterialCommon physical = ObjectManager::MAT_GENERIC);
    /// To string.
    Ogre::String toString(ObjectManager::MaterialCommon physical);

    /// Get friction.
    static float getFriction(ObjectManager::MaterialCommon physical);
    /// Get restitution (0 = no bounce, 1 = perfect bounce with no energy lost).
    static float getRestitution(ObjectManager::MaterialCommon physical);
    /// Add force by name to dynamic mesh.
    void addForceByName(const Ogre::String& name, Ogre::Vector3 direction);

    /// Get physics body by name.
    OgreBulletDynamics::RigidBody* getPhysicsBodyByName(const Ogre::String& name);
    /// Get object's bounding box.
    Ogre::Vector3 getBoundingBox(Ogre::Entity* ent);
    /// Get object's bounding box by file.
    Ogre::Vector3 getBoundingBoxByFile(const Ogre::String& filename);
    /// Get object's bounding box by name.
    Ogre::Vector3 getBoundingBoxByName(const Ogre::String& name);

    // Declare render objects.
    Ogre::Root* renderRoot; ///< Root class of the Ogre system.
    Ogre::RenderWindow* renderWindow; ///< Manages the target rendering window.
    Ogre::OverlayManager* renderOverlayMgr; ///< Manages overlay.
    Ogre::SceneManager* renderSceneMgr;  ///< Manages the organization and rendering of a scene.
    Ogre::Camera* renderCamera; ///< A viewpoint from which the scene will be rendered.

    // Declare physics objects.
    OgreBulletDynamics::DynamicsWorld* physicsWorld;
protected:
    /// Update tracker.
    void updateTracker(const Ogre::String& name, const Ogre::String& description);

    // Declare tracker.
    static int intObjectTotal;
};

} // END namespace Discover

#endif // #ifndef __ObjectManager_H__