#include "ConfigurationManager.h"

using namespace Discover;

//---------------------------------------------------------------------
ConfigurationManager::ConfigurationManager() {}
//---------------------------------------------------------------------
ConfigurationManager::~ConfigurationManager() {}
//---------------------------------------------------------------------
bool ConfigurationManager::instantiateThis() {
    // Instantiate cfgVariables.
    cfgVar.debugStats = false;

    return true;
}
//---------------------------------------------------------------------
void ConfigurationManager::registerFrameListener() {}
//---------------------------------------------------------------------
bool ConfigurationManager::isKeyDown(OIS::Keyboard* device, const Ogre::String& action) {
    // Locomotion.
    if(action == "forward")
        if(device->isKeyDown(OIS::KC_E)) return true;
    if(action == "backward")
        if(device->isKeyDown(OIS::KC_D)) return true;
    if(action == "moveLeft")
        if(device->isKeyDown(OIS::KC_S)) return true;
    if(action == "moveRight")
        if(device->isKeyDown(OIS::KC_F)) return true;
    if(action == "moveUp")
        if(device->isKeyDown(OIS::KC_SPACE)) return true;
    if(action == "moveDown")
        if(device->isKeyDown(OIS::KC_LSHIFT)) return true;
    if(action == "left")
        if(device->isKeyDown(OIS::KC_LEFT)) return true;
    if(action == "right")
        if(device->isKeyDown(OIS::KC_RIGHT)) return true;

    // Effects.
    if(action == "attack")
        if(device->isKeyDown(OIS::KC_R)) return true;
    if(action == "attack2")
        if(device->isKeyDown(OIS::KC_C)) return true;
    if(action == "block")
        if(device->isKeyDown(OIS::KC_X)) return true;
    if(action == "jump")
        if(device->isKeyDown(OIS::KC_Z)) return true;

    // Commands.
    if(action == "impulse1")
        if(device->isKeyDown(OIS::KC_1)) return true;
    if(action == "impulse2")
        if(device->isKeyDown(OIS::KC_2)) return true;
    if(action == "impulse3")
        if(device->isKeyDown(OIS::KC_3)) return true;
    if(action == "impulse4")
        if(device->isKeyDown(OIS::KC_4)) return true;
    if(action == "impulse5")
        if(device->isKeyDown(OIS::KC_5)) return true;
    if(action == "impulse6")
        if(device->isKeyDown(OIS::KC_6)) return true;
    if(action == "impulse9")
        if(device->isKeyDown(OIS::KC_9)) return true;
    if(action == "impulse0")
        if(device->isKeyDown(OIS::KC_0)) return true;
    
    // Functions.
    if(action == "f1")
        if(device->isKeyDown(OIS::KC_F1)) return true;
    if(action == "f2")
        if(device->isKeyDown(OIS::KC_F2)) return true;
    if(action == "f3")
        if(device->isKeyDown(OIS::KC_F3)) return true;
    if(action == "f4")
        if(device->isKeyDown(OIS::KC_F4)) return true;
    if(action == "f5")
        if(device->isKeyDown(OIS::KC_F5)) return true;
    if(action == "f6")
        if(device->isKeyDown(OIS::KC_F6)) return true;
    if(action == "f7")
        if(device->isKeyDown(OIS::KC_F7)) return true;
    if(action == "f8")
        if(device->isKeyDown(OIS::KC_F8)) return true;
    if(action == "f9")
        if(device->isKeyDown(OIS::KC_F9)) return true;
    if(action == "f10")
        if(device->isKeyDown(OIS::KC_F10)) return true;
    if(action == "f11")
        if(device->isKeyDown(OIS::KC_F11)) return true;
    if(action == "f12")
        if(device->isKeyDown(OIS::KC_F12)) return true;

    // Engine Commands.
    if(action == "pause")
        if(device->isKeyDown(OIS::KC_PAUSE)) return true;
    if(action == "screenshot")
        if(device->isKeyDown(OIS::KC_SYSRQ)) return true;
    if(action == "quit")
        if(device->isKeyDown(OIS::KC_ESCAPE)) return true;
    
    return false;
}
//---------------------------------------------------------------------