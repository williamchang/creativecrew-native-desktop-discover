/**
@file
    StageListener.h
@brief
    Coming soon.
@author
    William Chang
@version
    0.1
@date
    - Created: 2006-11-04
    - Modified: 2007-01-29
    .
@note
    References:
    - Ogre:
        - http://www.ogre3d.org/docs/manual/
        - http://www.ogre3d.org/docs/api/html/
        - http://www.ogre3d.org/wiki/index.php/Main_Page
        .
    .
    - OgreNewt:
        - http://www.ogre3d.org/wiki/index.php/Newton_Game_Dynamics
        - http://www.ogre3d.org/phpBB2addons/viewforum.php?f=4
        .
    .
*/

#ifndef __StageListener_H__
#define __StageListener_H__

#include "EnginePrerequisites.h"

namespace Discover {

/** @class StageListener */
class StageListener: public Ogre::FrameListener {
public:
    /// Default constructor.
    StageListener();
    /// Default destructor.
    virtual ~StageListener();

    /// Instantiate this.
    virtual bool instantiateThis();

    /// Create default camera for Engine setup.
    static void createCameraDefault();
    /// Create scene.
    virtual void createScene();
    /// Create physics scene.
    virtual void createPhysicsScene();

    // Setup this abstract/adapter class.
    //virtual void InstantiateScene() = 0; ///< Pure virtual functions, must be overridden.
    //virtual void createScene() = 0; ///< Pure virtual functions, must be overridden.
    //virtual void deleteScene();
protected:
    // Override Ogre::FrameListener.
    virtual bool frameStarted(const Ogre::FrameEvent& evt);
    virtual bool frameEnded(const Ogre::FrameEvent& evt);

    // Declare control objects.
    CameraControl* mCtrlCamera;
    LevelControl mCtrlLevel;
    //CharacterControl* mCtrlCharacter;
    UserControl* mCtrlUser;

    // Declare data types.
    Ogre::Real mTimeUntilNextToggle;
};

} // END namespace Discover

#endif // #ifndef __StageListener_H__