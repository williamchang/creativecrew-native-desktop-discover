/**
@file
    CharacterControl.h
@brief
    Coming soon.
@author
    William Chang
@version
    0.1
@date
    - Created: 2006-11-04
    - Modified: 2007-03-26
    .
@note
    References:
    - OgreNewt Movement:
        - http://www.ogre3d.org/phpBB2addons/viewtopic.php?t=2110
        - http://www.ogre3d.org/phpBB2addons/viewtopic.php?t=1261
        - http://www.ogre3d.org/phpBB2addons/viewtopic.php?t=633
        .
    - Ogre Animation:
        - http://www.ogre3d.org/phpBB2/viewtopic.php?t=8485
        - http://www.ogre3d.org/phpBB2/viewtopic.php?p=66883
        - http://www.ogre3d.org/phpBB2/viewtopic.php?t=11088
        .
    - NxOgre:
        - Sinking Bug Fix: nxOgre_character.cpp, Line:51, desc.skinWidth = 0.0125f;
        .
    .
*/

#ifndef __CharacterControl_H__
#define __CharacterControl_H__

#include "EnginePrerequisites.h"

namespace Discover {

/** @class CharacterControl */
class CharacterControl {
public:
    /** Animation Mode. */
    enum AnimationMode {
        AM_IDLE = 0,
        AM_WALK,
        AM_RUN,
        AM_JUMP,
        AM_ATTACK
    };
    /** Locomotion Mode. */
    enum LocomotionMode {
        LM_FORWARD = 0,
        LM_JUMP,
        LM_ATTACK
    };

    /// Default constructor.
    CharacterControl();
    /// Default destructor.
    virtual ~CharacterControl();

    /// Instantiate this.
    virtual bool instantiateThis();

    ///  Create CharacterControl.
    virtual void createCharacter(const Ogre::String&, Ogre::Vector3 position = Ogre::Vector3(1, 1, 1));
    /// Set animation.
    virtual void setAnimation(CharacterControl::AnimationMode mode);
    /// Set direction.
    virtual void setDirection(Ogre::Vector3 lookAt);
    /// Set locomotion.
    virtual void setLocomotion(CharacterControl::LocomotionMode mode);
    /// Set user.
    virtual void setUser(UserControl& user);
    /// Set character.
    virtual OgreBulletDynamics::RigidBody* getCharacter();
    /// Run one frame.
    virtual void runOneFrame(const Ogre::FrameEvent& evt);
protected:
    // Declare control objects.
    UserControl* mCtrlUser;
    
    // Declare scene.
    Ogre::Entity* mEnt;
    Ogre::SceneNode* mNode;
    // Declare physics.
    OgreBulletDynamics::RigidBody* mCharacter;

    // Declare profile.
    Ogre::Vector3 vecPosition;
    Ogre::Vector3 vecSize;
    Ogre::Vector3 vecLookAt;
    
    // Declare debug.
    int keyCount;
    // Declare timing variables.
    Ogre::Real mTimeUntilNextToggle;
    // Declare shoot variables.
    int count;
    
    // Declare locomotion.
    Ogre::Real realWalkSpeed;
    Ogre::Real realJumpThrust;
    bool isKeyHold;
    bool isKeyReleased;
    bool isJump;

    // Declare animation.
    Ogre::AnimationState* mAnimationState;
    Ogre::AnimationStateSet* mAnimationStateSet;

    // Declara yaw.
    Ogre::Real realYawSpeed;
    Ogre::String strYawOrientation;

    /** Get body position.
    @remarks
        This method needs OgreNewt libraries.
    @returns
        Returns body position.
    */
    //Ogre::Vector3 getBodyPosition(OgreNewt::Body* b);

    /** Get body orientation.
    @remarks
        This method needs OgreNewt libraries.
    @returns
        Returns body orientation.
    */
    //Ogre::Quaternion getBodyOrientation(OgreNewt::Body* b);
};

} // END namespace Discover

#endif // #ifndef __CharacterControl_H__