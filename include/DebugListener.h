/**
@file
    DebugListener.h
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
    .
*/

#ifndef __DebugListener_H__
#define __DebugListener_H__

#include <OgreBulletDynamicsRigidBody.h>
#include <Constraints/OgreBulletDynamicsPoint2pointConstraint.h>
#include "EnginePrerequisites.h"

namespace Discover {

/** @class DebugListener */
class DebugListener: public Ogre::FrameListener {
public:
    /** Physics Option. */
    enum PhysicsOption {
        PHY_WIREFRAME = 0,
        PHY_AABB,
        PHY_TEXT_FEATURES,
        PHY_CONTACT,
        PHY_DEACTIVATION,
        PHY_TEXT_NOHELP,
        PHY_TEXT_DRAW,
        PHY_PROFILETIMINGS,
        PHY_SAT,
        PHY_LCP,
        PHY_CCD
    };

    /// Default constructor.
    DebugListener();
    /// Default destructor.
    virtual ~DebugListener();

    /// Instantiate this.
    bool instantiateThis();
    /// Show debug statistics.
    void showDebugOverlay(bool show);
    /// Update debug statistics.
    void updateStats();

    /// Set physics debug options.
    static void setPhysicsDebug(DebugListener::PhysicsOption option);
    /// Pick object with cursor.
    void pickObject(bool isPick);
    /// Drag object with cursor.
    Ogre::Vector3 dragObject(Ogre::Vector3& cursor);
    /// Remove 3D lines.
    void remove3DLine();
    /// Get physics body using ray casting.
    OgreBulletDynamics::RigidBody* getPhysicsBody(Ogre::Ray r);
    /// Get entity (movable) using ray casting.
    Ogre::Entity* getEntity(Ogre::Ray r);
    /// Shoot projectiles from camera.
    void Shoot();
protected:
    // Override Ogre::FrameListener.
    virtual bool frameStarted(const Ogre::FrameEvent& evt);
    virtual bool frameEnded(const Ogre::FrameEvent& evt);

    // Declare data types.
    std::string mDebugText1;
    std::string mDebugText2;

    // Declare control objects.
    CameraControl mCtrlCamera;
    UserControl mCtrlUser;

    // Declare pick variables for debugging physics.
    Ogre::Overlay* mCursorOverlay;
    Ogre::OverlayContainer* mCursor;
    Ogre::SceneNode* targetNode;
    bool hasTargetBody;
    Ogre::Entity *mDragPoint1, *mDragPoint2;
    Ogre::SceneNode *mDragPoint1Node, *mDragPoint2Node;
    Ogre::Vector3 mHitPoint;
    Ogre::Vector3 mCursorIntersectPoint;
    Ogre::Vector3 mLocalPoint;
    OgreBulletDynamics::RigidBody* targetBody;
    OgreBulletDynamics::TypedConstraint* mPickConstraint;

    // Declare 3D line variables.
    Ogre::SceneNode* mDragLineNode;
    Ogre::ManualObject* mDragLine;

    // Declare shoot variables for debugging physics.
    int count;

    // Declare time variables.
    Ogre::Real mTimeUntilNextToggle; 

    // Declare debugging variables.
    Ogre::Overlay* mDebugOverlay;
};

} // END namespace Discover

#endif // #ifndef __DebugListener_H__