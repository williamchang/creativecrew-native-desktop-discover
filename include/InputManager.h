/**
@file
    InputManager.h
@brief
    Coming soon.
@author
    William Chang
@version
    0.1
@date
    - Created: 2006-11-08
    - Modified: 2008-06-02
    .
@note
    References:
    - http://www.ogre3d.org/wiki/index.php/Using_OIS
    - http://www.wreckedgames.com/
    .
*/

//#include <nxOgre.h> // Irregularity fix. Order of placement is mandatory.

#ifndef __InputManager_H__
#define __InputManager_H__

#include <OgreRenderWindow.h>

// Use this define to signify OIS will be used as a DLL
//(so that dll import/export macros are in effect)
#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

namespace Discover {

/** @class InputManager */
class InputManager: public OIS::KeyListener, OIS::MouseListener, OIS::JoyStickListener {
public:
    /// Default constructor.
    InputManager();
    /// Default destructor.
    virtual ~InputManager();

    /// Instantiate manager.
    bool instantiateThis();
    /// Register manager's frame listeners.
    void registerFrameListener();

    /// Instantiate render window.
    void instantiateRenderWindow(Ogre::RenderWindow *renderWindow);
    /// Capture all inputs.
    void capture();

    /// Add a keyboard listener.
    void addKeyListener(OIS::KeyListener *keyListener, const std::string& instanceName);
    /// Add a mouse listener.
    void addMouseListener(OIS::MouseListener *mouseListener, const std::string& instanceName);
    /// Add a joystick listener.
    void addJoystickListener(OIS::JoyStickListener *joystickListener, const std::string& instanceName);

    /// Remove a keyboard listener by name.
    void removeKeyListener(const std::string& instanceName);
    /// Remove a mouse listener by name.
    void removeMouseListener(const std::string& instanceName);
    /// Remove a joystick listener by name.
    void removeJoystickListener(const std::string& instanceName);

    /// Remove a keyboard listener by pointer.
    void removeKeyListener(OIS::KeyListener* keyListener);
    /// Remove a mouse listener by pointer.
    void removeMouseListener(OIS::MouseListener* mouseListener);
    /// Remove a joystick listener by pointer.
    void removeJoystickListener(OIS::JoyStickListener* joystickListener);

    /// Remove all input listeners.
    void removeAllListeners();
    /// Remove all keyboard listeners.
    void removeAllKeyListeners();
    /// Remove all mouse listeners.
    void removeAllMouseListeners();
    /// Remove all joystick listeners.
    void removeAllJoystickListeners();

    /// Set mouse region.
    void setWindowExtents(int width, int height);

    /// Get mouse.
    OIS::Mouse* getMouse();
    /// Get keyboard.
    OIS::Keyboard* getKeyboard();
    /// Get joystick.
    OIS::JoyStick* getJoystick(unsigned int index);
    /// Get number of joysticks.
    int getNumOfJoysticks();
protected:
    /// Keyboard pressed.
    bool keyPressed(const OIS::KeyEvent &e);
    /// Keyboard released.
    bool keyReleased(const OIS::KeyEvent &e);

    /// Mouse moved.
    bool mouseMoved(const OIS::MouseEvent &e);
    /// Mouse pressed.
    bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    /// Mouse released.
    bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

    // Joystick.
    bool povMoved(const OIS::JoyStickEvent &e, int pov);
    bool axisMoved(const OIS::JoyStickEvent &e, int axis);
    bool sliderMoved(const OIS::JoyStickEvent &e, int sliderID);
    bool buttonPressed(const OIS::JoyStickEvent &e, int button);
    bool buttonReleased(const OIS::JoyStickEvent &e, int button);

    // Declare input objects.
    OIS::InputManager* mInputSystem;
    OIS::Keyboard* mKeyboard;
    OIS::Mouse* mMouse;

    std::vector<OIS::JoyStick*> mJoysticks;
    std::vector<OIS::JoyStick*>::iterator itJoystick;
    std::vector<OIS::JoyStick*>::iterator itJoystickEnd;

    std::map<std::string, OIS::KeyListener*> mKeyListeners;
    std::map<std::string, OIS::MouseListener*> mMouseListeners;
    std::map<std::string, OIS::JoyStickListener*> mJoystickListeners;

    std::map<std::string, OIS::KeyListener*>::iterator itKeyListener;
    std::map<std::string, OIS::MouseListener*>::iterator itMouseListener;
    std::map<std::string, OIS::JoyStickListener*>::iterator itJoystickListener;

    std::map<std::string, OIS::KeyListener*>::iterator itKeyListenerEnd;
    std::map<std::string, OIS::MouseListener*>::iterator itMouseListenerEnd;
    std::map<std::string, OIS::JoyStickListener*>::iterator itJoystickListenerEnd;
};

} // END namespace Discover

#endif // #ifndef __InputManager_H__