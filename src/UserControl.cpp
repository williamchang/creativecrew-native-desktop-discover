#include "EngineHeaders.h"

using namespace Discover;

//---------------------------------------------------------------------
UserControl::UserControl():
    mTranslateVector(Ogre::Vector3::ZERO),
    mRotateX(0),
    mRotateY(0),
    mMoveSpeed(0),
    mRotateSpeed(0),
    mMoveScale(0),
    mRotateScale(0)
    {if(!instantiateThis()) return;}
//---------------------------------------------------------------------
UserControl::~UserControl() {}
//---------------------------------------------------------------------
bool UserControl::instantiateThis() {
    mUserMode = UserControl::UM_DEFAULT;
    mPointerX = 0;
    mPointerY = 0;
    mMouseSensitivity = 0.13;

    // Instantiate debugging.
    mNumScreenShots = 0;
    mSceneDetailIndex = 0;
    mAniso = 1;
    mFiltering = Ogre::TFO_BILINEAR;

    return true;
}
//---------------------------------------------------------------------
void UserControl::setMode(UserControl::UserMode mode) {mUserMode = mode;}
//---------------------------------------------------------------------
void UserControl::setCharacter(CharacterControl& character) {mCtrlCharacter = &character;}
//---------------------------------------------------------------------
void UserControl::setMoveSpeed(Ogre::Real speed) {mMoveSpeed = speed;}
//---------------------------------------------------------------------
void UserControl::setRotateSpeed(Ogre::Real speed) {mRotateSpeed = speed;}
//---------------------------------------------------------------------
UserControl::UserMode UserControl::getMode() {return mUserMode;}
//---------------------------------------------------------------------
Ogre::Vector3 UserControl::getTranslate() {return mTranslateVector;}
//---------------------------------------------------------------------
Ogre::Radian UserControl::getRotationX() {return mRotateX;}
//---------------------------------------------------------------------
Ogre::Radian UserControl::getRotationY() {return mRotateY;}
//---------------------------------------------------------------------
int UserControl::getPointerX() {return mPointerX;}
//---------------------------------------------------------------------
int UserControl::getPointerY() {return mPointerY;}
//---------------------------------------------------------------------
std::string UserControl::getDebugText() {return mDebugText;}
//---------------------------------------------------------------------
bool UserControl::processUnbufferedKeyInput() {
    mKeyboard = glbInput->getKeyboard();
    switch(mUserMode) {
        case UserControl::UM_PLAYER:
            // Strafe left by keypress.
            if(glbConfig->isKeyDown(mKeyboard, "left")) {
                mCtrlCharacter->setDirection(Ogre::Vector3(10, 0, 0));
                mCtrlCharacter->setLocomotion(CharacterControl::LM_FORWARD);
            }
            // Strafe right by keypress.
            if(glbConfig->isKeyDown(mKeyboard, "right")) {
                mCtrlCharacter->setDirection(Ogre::Vector3(-10, 0, 0));
                mCtrlCharacter->setLocomotion(CharacterControl::LM_FORWARD);
            }
            // Jump by keypress.
            if(glbConfig->isKeyDown(mKeyboard, "jump")) {
                mCtrlCharacter->setLocomotion(CharacterControl::LM_JUMP);
            }
            // Attack by keypress.
            if(glbConfig->isKeyDown(mKeyboard, "attack2")) {
                mCtrlCharacter->setLocomotion(CharacterControl::LM_ATTACK);
            }
            break;
        default:
            // Strafe left by keypress.
            if(glbConfig->isKeyDown(mKeyboard, "moveLeft")) {
                mTranslateVector.x = -mMoveScale;
            }
            // Strafe right by keypress.
            if(glbConfig->isKeyDown(mKeyboard, "moveRight")) {
                mTranslateVector.x = mMoveScale;
            }
            // Move forward by keypress.
            if(glbConfig->isKeyDown(mKeyboard, "forward")) {
                mTranslateVector.z = -mMoveScale;
            }
            // Move backward by keypress.
            if(glbConfig->isKeyDown(mKeyboard, "backward")) {
                mTranslateVector.z = mMoveScale;
            }
            // Move up by keypress.
            if(glbConfig->isKeyDown(mKeyboard, "moveUp")) {
                mTranslateVector.y = mMoveScale;
            }
            // Move down by keypress.
            if(glbConfig->isKeyDown(mKeyboard, "moveDown"))	{
                mTranslateVector.y = -mMoveScale;
            }
            // Turn left by keypress.
            if(glbConfig->isKeyDown(mKeyboard, "left")) {
                mRotateX = mRotateScale;
            }
            // Turn right by keypress.
            if(glbConfig->isKeyDown(mKeyboard, "right")) {
                mRotateX = -mRotateScale;
            }
            break;
    }
    // Quit by keypress.
    if(glbConfig->isKeyDown(mKeyboard, "quit")) {
        return false;
    }

    return true;
}
//---------------------------------------------------------------------
bool UserControl::isKeyInputAction() {
    mKeyboard = glbInput->getKeyboard();
    if(glbConfig->isKeyDown(mKeyboard, "attack") && mTimeUntilNextToggle <= 0) {
        mTimeUntilNextToggle = 1;
        return true;
    }
    return false;
}
//---------------------------------------------------------------------
bool UserControl::processUnbufferedKeyInputDebug(CameraControl& ctrlCam) {
    Ogre::Camera* cam = ctrlCam.getCamera();
    mKeyboard = glbInput->getKeyboard();

    if(glbConfig->isKeyDown(mKeyboard, "impulse1") && mTimeUntilNextToggle <= 0) {
        glbCfgVar.debugStats = !glbCfgVar.debugStats;
        mTimeUntilNextToggle = 1;
    }

    if(glbConfig->isKeyDown(mKeyboard, "impulse2") && mTimeUntilNextToggle <= 0) {
        switch(mFiltering) {
        case Ogre::TFO_BILINEAR:
            mFiltering = Ogre::TFO_TRILINEAR;
            mAniso = 1;
            break;
        case Ogre::TFO_TRILINEAR:
            mFiltering = Ogre::TFO_ANISOTROPIC;
            mAniso = 8;
            break;
        case Ogre::TFO_ANISOTROPIC:
            mFiltering = Ogre::TFO_BILINEAR;
            mAniso = 1;
            break;
        default:
            break;
        }
        Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(mFiltering);
        Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(mAniso);

        mTimeUntilNextToggle = 1;
    }

    if(glbConfig->isKeyDown(mKeyboard, "screenshot") && mTimeUntilNextToggle <= 0) {
        char tmp[20];
        sprintf(tmp, "screenshot_%d.png", ++mNumScreenShots);
        glbObject->renderWindow->writeContentsToFile(tmp);
        mTimeUntilNextToggle = 0.5;
        mDebugText = Ogre::String("Wrote ") + tmp;
    }

    if(glbConfig->isKeyDown(mKeyboard, "impulse3") && mTimeUntilNextToggle <=0)	{
        mSceneDetailIndex = (mSceneDetailIndex+1)%3 ;
        switch(mSceneDetailIndex) {
                case 0: cam->setPolygonMode(Ogre::PM_SOLID); break;
                case 1: cam->setPolygonMode(Ogre::PM_WIREFRAME); break;
                case 2: cam->setPolygonMode(Ogre::PM_POINTS); break;
        }
        mTimeUntilNextToggle = 0.5;
    }

    static bool displayCameraDetails = false;
    if(glbConfig->isKeyDown(mKeyboard, "impulse4") && mTimeUntilNextToggle <= 0) {
        displayCameraDetails = !displayCameraDetails;
        mTimeUntilNextToggle = 0.5;
        if(!displayCameraDetails)
            mDebugText = "";
    }
    if(displayCameraDetails) {
        // Print camera details
        mDebugText = "Name: " + cam->getName() + " " +
            "P: " + Ogre::StringConverter::toString(cam->getDerivedPosition()) + " " +
            "O: " + Ogre::StringConverter::toString(cam->getDerivedOrientation());
    }

    static bool displayBoundingBoxes = false;
    if(glbConfig->isKeyDown(mKeyboard, "impulse5") && mTimeUntilNextToggle <= 0) {
        displayBoundingBoxes = !displayBoundingBoxes;
        mTimeUntilNextToggle = 0.5;
        if(!displayBoundingBoxes)
            glbObject->renderSceneMgr->showBoundingBoxes(false);
    }
    if(displayBoundingBoxes) {
        glbObject->renderSceneMgr->showBoundingBoxes(true);
    }

    if (glbConfig->isKeyDown(mKeyboard, "f1") && mTimeUntilNextToggle <= 0) {
        DebugListener::setPhysicsDebug(DebugListener::PHY_WIREFRAME);
        mTimeUntilNextToggle = 1;
    }

    if (glbConfig->isKeyDown(mKeyboard, "f2") && mTimeUntilNextToggle <= 0) {
        DebugListener::setPhysicsDebug(DebugListener::PHY_AABB);
        mTimeUntilNextToggle = 1;
    }

    if (glbConfig->isKeyDown(mKeyboard, "f3") && mTimeUntilNextToggle <= 0) {
        DebugListener::setPhysicsDebug(DebugListener::PHY_TEXT_FEATURES);
        mTimeUntilNextToggle = 1;
    }

    if (glbConfig->isKeyDown(mKeyboard, "f4") && mTimeUntilNextToggle <= 0) {
        DebugListener::setPhysicsDebug(DebugListener::PHY_CONTACT);
        mTimeUntilNextToggle = 1;
    }

    if (glbConfig->isKeyDown(mKeyboard, "f5") && mTimeUntilNextToggle <= 0) {
        DebugListener::setPhysicsDebug(DebugListener::PHY_DEACTIVATION);
        mTimeUntilNextToggle = 1;
    }

    if (glbConfig->isKeyDown(mKeyboard, "f6") && mTimeUntilNextToggle <= 0) {
        DebugListener::setPhysicsDebug(DebugListener::PHY_TEXT_NOHELP);
        mTimeUntilNextToggle = 1;
    }

    if (glbConfig->isKeyDown(mKeyboard, "f7") && mTimeUntilNextToggle <= 0) {
        DebugListener::setPhysicsDebug(DebugListener::PHY_TEXT_DRAW);
        mTimeUntilNextToggle = 1;
    }

    if (glbConfig->isKeyDown(mKeyboard, "f8") && mTimeUntilNextToggle <= 0) {
        DebugListener::setPhysicsDebug(DebugListener::PHY_PROFILETIMINGS);
        mTimeUntilNextToggle = 1;
    }

    if (glbConfig->isKeyDown(mKeyboard, "f9") && mTimeUntilNextToggle <= 0) {
        DebugListener::setPhysicsDebug(DebugListener::PHY_SAT);
        mTimeUntilNextToggle = 1;
    }

    if (glbConfig->isKeyDown(mKeyboard, "f10") && mTimeUntilNextToggle <= 0) {
        DebugListener::setPhysicsDebug(DebugListener::PHY_LCP);
        mTimeUntilNextToggle = 1;
    }

    if (glbConfig->isKeyDown(mKeyboard, "f11") && mTimeUntilNextToggle <= 0) {
        DebugListener::setPhysicsDebug(DebugListener::PHY_CCD);
        mTimeUntilNextToggle = 1;
    }

    static bool pausePhysics = false;
    if(glbConfig->isKeyDown(mKeyboard, "pause") && mTimeUntilNextToggle <= 0) {
        pausePhysics = !pausePhysics;
        mTimeUntilNextToggle = 0.5;
    }
    if(pausePhysics) {
        // TODO Physics pause.
    }
    static bool isPick = false;
    if(glbConfig->isKeyDown(mKeyboard, "impulse9") && mTimeUntilNextToggle <= 0) {
        isPick = !isPick;
        mTimeUntilNextToggle = 0.5;
        if(!isPick) {
            mUserMode = UserControl::UM_DEFAULT;
        }
    }
    if(isPick) {
        mUserMode = UserControl::UM_DEBUGPICK;
    }

    if(glbConfig->isKeyDown(mKeyboard, "impulse0") && mTimeUntilNextToggle <= 0) {
        if(cam->getName() == "FreelookCamera")
            ctrlCam.switchCamera("DefaultCamera");
        if(cam->getName() == "DefaultCamera")
            ctrlCam.switchCamera("FreelookCamera");
        mTimeUntilNextToggle = 1;
    }

    return true;
}
//---------------------------------------------------------------------
bool UserControl::processUnbufferedMouseInput() {
    const OIS::MouseState& mMouse = glbInput->getMouse()->getMouseState();
    switch(mUserMode) {
        case UserControl::UM_PICK:
        case UserControl::UM_DEBUGPICK:
            if(mMouse.buttonDown(OIS::MB_Right)) {
                mRotateX = Ogre::Degree(-mMouse.X.rel * mMouseSensitivity);
                mRotateY = Ogre::Degree(-mMouse.Y.rel * mMouseSensitivity);
                mPointerX = 0;
                mPointerY = 0;
            }
            else {
                mPointerX = mMouse.X.rel;
                mPointerY = mMouse.Y.rel;
            }
            break;
        default:
            // Rotation factors, may not be used if the second mouse button is pressed.
            // If the second mouse button is pressed, then the mouse movement results in
            // sliding the camera, otherwise we rotate.
            if(mMouse.buttonDown(OIS::MB_Right)) {
                mTranslateVector.x += mMouse.X.rel * mMouseSensitivity * 0.2;
                mTranslateVector.y -= mMouse.Y.rel * mMouseSensitivity * 0.2;
            }
            else {
                mRotateX = Ogre::Degree(-mMouse.X.rel * mMouseSensitivity);
                mRotateY = Ogre::Degree(-mMouse.Y.rel * mMouseSensitivity);
            }
            break;
    }

    return true;
}
//---------------------------------------------------------------------
Ogre::String UserControl::getMouseInputAction() {
    const OIS::MouseState& mMouse = glbInput->getMouse()->getMouseState();
    switch(mUserMode) {
        case UserControl::UM_DEBUGPICK:
            if(mMouse.buttonDown(OIS::MB_Left))
                return "move";
            else if(mMouse.buttonDown(OIS::MB_Middle))
                return "grab";
            break;
    }

    return "";
}
//---------------------------------------------------------------------
bool UserControl::getUserInputs(const Ogre::FrameEvent& evt) {
    if(mTimeUntilNextToggle >= 0)
        mTimeUntilNextToggle -= evt.timeSinceLastFrame;

    if(!processUnbufferedKeyInput()) return false;
    if(!processUnbufferedMouseInput()) return false;

    mCtrlCharacter->runOneFrame(evt);

    return true;
}
//---------------------------------------------------------------------
bool UserControl::getUserInputs(CameraControl& ctrlCam, const Ogre::FrameEvent& evt) {
    if(mTimeUntilNextToggle >= 0)
        mTimeUntilNextToggle -= evt.timeSinceLastFrame;

    // If this is the first frame, pick a speed
    if(evt.timeSinceLastFrame == 0) {
        mMoveScale = 1;
        mRotateScale = 0.1;
    }
    // Otherwise scale movement units by time passed since last frame
    else {
        // Move about 100 units per second,
        mMoveScale = mMoveSpeed * evt.timeSinceLastFrame;
        // Take about 10 seconds for full rotation
        mRotateScale = mRotateSpeed * evt.timeSinceLastFrame;
    }
    mRotateX = 0;
    mRotateY = 0;
    mTranslateVector = Ogre::Vector3::ZERO;

    if(!processUnbufferedKeyInput()) return false;
    if(!processUnbufferedMouseInput()) return false;
    if(!processUnbufferedKeyInputDebug(ctrlCam)) return false;

    return true;
}
//---------------------------------------------------------------------