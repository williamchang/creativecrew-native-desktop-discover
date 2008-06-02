#include "EngineHeaders.h"

using namespace Discover;

//---------------------------------------------------------------------
StageListener::StageListener() {if(!instantiateThis()) return;}
//---------------------------------------------------------------------
StageListener::~StageListener() {}
//---------------------------------------------------------------------
bool StageListener::instantiateThis() {
    //OIS::KeyListener* mKeyboardListener;
    //OIS::MouseListener* mMouseListener;
    //glbInput->addKeyListener(mKeyboardListener, "Player1_Keyboard");
    //glbInput->addMouseListener(mMouseListener, "Player1_Mouse");

    //mCtrlCharacter = new CharacterControl();

    createScene();
    createPhysicsScene();

    return true;
}
//---------------------------------------------------------------------
void StageListener::createCameraDefault() {CameraControl c;c.createCameraDefault();}
//---------------------------------------------------------------------
void StageListener::createScene() {
    // Set shadow. Alternative: Ogre::SHADOWTYPE_STENCIL_ADDITIVE, Ogre::SHADOWTYPE_STENCIL_MODULATIVE
    glbObject->renderSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);

    // Set sky.
    glbObject->renderSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

    // Set ambient light.
    glbObject->renderSceneMgr->setAmbientLight(Ogre::ColourValue(0.6, 0.6, 0.6));

    // Create sun.
    Ogre::Light* lightSun = glbObject->renderSceneMgr->createLight("Sun");
    lightSun->setType(Ogre::Light::LT_DIRECTIONAL);
    lightSun->setDirection(-2, -4, 2); 
    // Create spotlight.
    //Ogre::Light* lightLamp = glbObject->renderSceneMgr->createLight("Lamp");
    //lightLamp->setType(Ogre::Light::LT_POINT);
    //lightLamp->setPosition(0, 20, 0);
    //lightLamp->setDirection(10, 10, 0);

    // Load level.
    mCtrlLevel.loadLevel("scene6export.scene");

    // Create base units (real-world scale). 
    glbObject->createMeshDynamic("nxogrecube1m", "cube1m.mesh", 10, Ogre::Vector3(3, 10, 3));
    glbObject->createMeshDynamic("standard1cm", "scale1cm.mesh", 10, Ogre::Vector3(3, 11, -3));
    glbObject->createMeshDynamic("standard1m", "scale1cm.mesh", 10, Ogre::Vector3(20, 12, 8));

    // Create character.
    //mCtrlCharacter->createCharacter("Player1", Ogre::Vector3(1, 2, 0));
    //mCtrlUser = new UserControl();
    //mCtrlCharacter->setUser(*mCtrlUser);
    //mCtrlCharacter->createCharacter("Computer1", Ogre::Vector3(40, 30, 0));

    //mCtrlCamera = new CameraControl();
    //mCtrlCamera->instantiateThis();
    //mCtrlCamera->createCamera("ThirdPersonSide", Ogre::Vector3(0, 1, 5));
    //mCtrlCamera->setCamera(CameraControl::CAM_SIDE, "Player1");
}
//---------------------------------------------------------------------
void StageListener::createPhysicsScene() {
    //if(!glbObject->physicsWorld->isPaused()) glbObject->physicsWorld->pause();

    // Create multiples boxes stacked 10x10 with a top row of metal boxes.
    int count = 0;
    Ogre::Real offsetX = 25;
    Ogre::Real offsetZ = -5;
    for(int i=0;i<10;i++) {
        for(int j=0;j<9;j++) {
            Ogre::String strName = "PhysicsDemo1_" + Ogre::StringConverter::toString(count++);
            glbObject->createMeshDynamic(strName, "scale1cm.mesh", 20, Ogre::Vector3(offsetX, j * 1 + 0.5, i * 1 + offsetZ));
            glbObject->setMaterialByName(strName, "Material/woodbox", ObjectManager::MAT_WOOD);
        }
    }
    for(int i=0;i<10;i++) {
        Ogre::String strName = "PhysicsDemo1_" + Ogre::StringConverter::toString(count++);
        glbObject->createScaleMeshDynamic(strName, "scale1cm.mesh", 40, Ogre::Vector3(1, 2, 1), Ogre::Vector3(0.5, 1, 0.5), Ogre::Vector3(offsetX, 9 * 1 + 2, i * 1 + offsetZ));
        glbObject->setMaterialByName(strName, "Material/metalbox", ObjectManager::MAT_METAL);
    }

    //if(glbObject->physicsWorld->isPaused()) glbObject->physicsWorld->resume();
}
//---------------------------------------------------------------------
bool StageListener::frameStarted(const Ogre::FrameEvent& evt) {
    //if(!mCtrlUser->getUserInputs(evt)) return false;

    if(mTimeUntilNextToggle >= 0)
        mTimeUntilNextToggle -= evt.timeSinceLastFrame;
    
    return true;
}
//---------------------------------------------------------------------
bool StageListener::frameEnded(const Ogre::FrameEvent& evt) {
    return true;
}
//---------------------------------------------------------------------