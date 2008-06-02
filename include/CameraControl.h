/**
@file
    CameraControl.h
@brief
    Coming soon.
@author
    William Chang
@version
    0.1
@date
    - Created: 2006-11-09
    - Modified: 2008-06-01
    .
@note
    References:
    - Euler Angle Class:
        - http://www.ogre3d.org/wiki/index.php/Euler_Angle_Class
        .
    - First Person (Free-Look) Camera:
        - http://www.ogre3d.org/wiki/index.php/How_to_limit_the_pitch_of_a_camera
        - http://www.ogre3d.org/wiki/index.php/Creating_a_simple_first-person_camera_system
        .
    - Third Person Camera:
        - http://www.ogre3d.org/wiki/index.php/3rd_person_camera_system_tutorial
        .
    .
*/

#ifndef __CameraControl_H__
#define __CameraControl_H__

#include "EnginePrerequisites.h"

namespace Discover {

/** @class CameraControl */
class CameraControl {
public:
    /** Camera classification. */
    enum Perspective {
        CAM_DEFAULT = 0,
        CAM_STATIC,
        CAM_FREELOOK,
        CAM_FIRSTPERSON,
        CAM_THIRDPERSON,
        CAM_TOP,
        CAM_SIDE,
    };

    /// Default constructor.
    CameraControl();
    /// Ogre constructor.
    CameraControl(Ogre::RenderWindow* rw, Ogre::SceneManager* sm, Ogre::Camera* cam);
    /// Default destructor.
    virtual ~CameraControl();

    /// Instantiate this.
    bool instantiateThis();

    /// Create camera.
    void createCamera(Ogre::String uniquename, Ogre::Vector3 position = Ogre::Vector3::ZERO, Ogre::Real nearclipdistance = 0.1);
    /// Create default camera. Must be called once.
    void createCameraDefault();
    /// Point camera.
    void pointCamera(Ogre::Vector3 lookat);
    /// Set camera.
    void setCamera(CameraControl::Perspective mode, Ogre::String objectname);
    /// Move camera.
    void moveCamera(Ogre::Vector3 move, Ogre::Radian rotateX, Ogre::Radian rotateY);
    /// Get camera.
    Ogre::Camera* getCamera();
    /// Get camera position.
    Ogre::Vector3 getPosition();
    /// Get camera orientation.
    Ogre::Quaternion getOrientation();
    /// Switch camera.
    void switchCamera(Ogre::String name);
    /// Run one frame.
    virtual void runOneFrame(const Ogre::FrameEvent& evt);
protected:
    /// Create viewport on entire window. Must be called once.
    void createViewportFull();

    // Declare mode variable.
    CameraControl::Perspective mPerspective;

    // Declare target variables.
    Ogre::SceneNode* mNodeTarget;

    // Declare render objects.
    Ogre::RenderWindow* mWindow;
    Ogre::SceneManager* mSceneMgr;
    Ogre::SceneNode* mNode;
    Ogre::Viewport* mViewport;
    Ogre::Camera* mCamera;
    Ogre::Degree mCameraYaw, mCameraPitch;
    Ogre::Vector3 mTranslateVector;
    Ogre::Radian mRotateX, mRotateY;
};

} // END namespace Discover

#endif // #ifndef __CameraControl_H__