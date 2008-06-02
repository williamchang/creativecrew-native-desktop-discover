/**
@file
    UserControl.h
@brief
    Coming soon.
@author
    William Chang
@version
    0.1
@date
    - Created: 2006-11-09
    - Modified: 2008-06-02
    .
*/

#ifndef __UserControl_H__
#define __UserControl_H__

#include "EnginePrerequisites.h"

namespace Discover {

/** @class UserControl */
class UserControl {
public:
    /** User modes. */
    enum UserMode {
        UM_DEFAULT = 0,
        UM_PLAYER,
        UM_PICK,
        UM_DEBUG,
        UM_DEBUGPICK
    };

    /// Default constructor.
    UserControl();
    /// Default destructor.
    virtual ~UserControl();

    /// Instantiate this.
    bool instantiateThis();

    /// Set mode.
    virtual void setMode(UserMode mode);
    /// Set character.
    virtual void setCharacter(CharacterControl& character);
    /// Set move speed.
    virtual void setMoveSpeed(Ogre::Real speed);
    /// Set rotate speed.
    virtual void setRotateSpeed(Ogre::Real speed);
    /// Get mode.
    virtual UserControl::UserMode getMode();
    /// Get translate.
    virtual Ogre::Vector3 getTranslate();
    /// Get rotation of x-axis.
    virtual Ogre::Radian getRotationX();
    /// Get rotation of y-axis.
    virtual Ogre::Radian getRotationY();
    /// Get pointer of x-axis.
    virtual int getPointerX();
    /// Get pointer of y-axis.
    virtual int getPointerY();
    /// Get pointer of y-axis.
    virtual std::string getDebugText();

    /// Is keyboard acted.
    virtual bool isKeyInputAction();
    /// Get mouse action.
    virtual Ogre::String getMouseInputAction();
    /// Get user inputs.
    virtual bool getUserInputs(const Ogre::FrameEvent& evt);
    /// Get user inputs and update camera.
    virtual bool getUserInputs(CameraControl& ctrlCam, const Ogre::FrameEvent& evt);
protected:
    // Process input.
    virtual bool processUnbufferedKeyInput();
    virtual bool processUnbufferedMouseInput();
    virtual bool processUnbufferedKeyInputDebug(CameraControl& ctrlCam);

    // Declare standard objects.
    std::string mDebugText;

    // Declare mode variable.
    UserControl::UserMode mUserMode;

    // Declare controls objects.
    CharacterControl* mCtrlCharacter;

    // Declare locomotion variables.
    Ogre::Vector3 mTranslateVector;
    Ogre::Radian mRotateX, mRotateY;
    int mPointerX, mPointerY;

    // Declare speed variables.
    Ogre::Real mMouseSensitivity;
    Ogre::Real mMoveSpeed;
    Ogre::Degree mRotateSpeed;

    // Declare scale variables.
    Ogre::Real mMoveScale;
    Ogre::Degree mRotateScale;

    // Declare input variables.
    OIS::Keyboard* mKeyboard;

    // Declare time variables.
    Ogre::Real mTimeUntilNextToggle;

    // Declare debugging variables.
    Ogre::TextureFilterOptions mFiltering;
    int mSceneDetailIndex;
    unsigned int mNumScreenShots;
    int mAniso;
};

} // END namespace Discover

#endif // #ifndef __UserControl_H__