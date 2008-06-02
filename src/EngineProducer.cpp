#include "EngineHeaders.h"

using namespace Discover;

//---------------------------------------------------------------------
EngineProducer::EngineProducer() {}
//---------------------------------------------------------------------
EngineProducer::~EngineProducer() {}
//---------------------------------------------------------------------
void EngineProducer::start() {if(!instantiateThis()) return;}
//---------------------------------------------------------------------
bool EngineProducer::instantiateThis() {
    // Create a singleton. Must be called once.
    new Global();
    // Register the source of resources.
    registerResourceSource();
    // If Ogre initialization fails, abandon.
    if(!instantiateOgre()) return false;

    // Instantiate managers.
    glbObject->instantiateThis();
    glbInput->instantiateThis();
    glbConfig->instantiateThis();

    // Load resources.
    {
        StageListener::createCameraDefault();
        LoadingControl l;
        l.start(glbObject->renderWindow, 1, 1, 0.75);
        registerResourceListener();
        loadResources();
        l.finish();
    }

    // Instantiate render window.
    glbInput->instantiateRenderWindow(glbObject->renderWindow);

    // Register frame listeners.
    glbInput->registerFrameListener();
    glbObject->registerFrameListener();
    glbConfig->registerFrameListener();
    this->registerFrameListener();

    // Start main loop.
    startLoopMain();

    return true;
}
//---------------------------------------------------------------------
void EngineProducer::registerResourceSource() {
    // Load resource paths from config file.
    Ogre::ConfigFile cfg;
    cfg.load("resources.cfg");

    // Go through all sections & settings in the file.
    Ogre::ConfigFile::SectionIterator cfgSection = cfg.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while(cfgSection.hasMoreElements()) {
        secName = cfgSection.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = cfgSection.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for(i = settings->begin(); i != settings->end(); ++i) {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
}
//---------------------------------------------------------------------
bool EngineProducer::instantiateOgre() {
    // Show the configuration dialog and Instantiate the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(glbObject->renderRoot->showConfigDialog())	{
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        glbObject->renderWindow = glbObject->renderRoot->initialise(true);
        return true;
    }
    return false;
}
//---------------------------------------------------------------------
void EngineProducer::registerResourceListener() {}
//---------------------------------------------------------------------
void EngineProducer::loadResources(void) {
    // Initialise, parse scripts etc.
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//---------------------------------------------------------------------
void EngineProducer::registerFrameListener() {
    // Dynamically allocate memory for declared pointer of type typename.
    lstnDebug = new DebugListener();
    lstnStage = new StageListener();

    // Add frame listeners to system renderer.
    glbObject->renderRoot->addFrameListener(lstnDebug);
    glbObject->renderRoot->addFrameListener(lstnStage);
}
//---------------------------------------------------------------------
void EngineProducer::startLoopMain() {
    // Infinite loop, until broken out of by frame listeners
    // or break out by calling shutdownEngineRequest()
    mShutdownEngine = false;

    unsigned long mTimeLastFrame = 0;
    bool mPaused = false;
    bool mSingleStep = false;

    while(!mShutdownEngine) {
        if(glbObject->renderWindow->isClosed()) break;

        // Calculate time since last frame and remember current time for next frame.
        unsigned long mTimeCurrentFrame = glbObject->renderRoot->getTimer()->getMilliseconds();
        unsigned long mTimeSinceLastFrame = mTimeCurrentFrame - mTimeLastFrame;
        mTimeLastFrame = mTimeCurrentFrame;

        // Update inputManager.
        glbInput->capture();

        // Update physics.
        if(!mPaused || mSingleStep) {
            glbObject->physicsWorld->stepSimulation(mTimeSinceLastFrame);
        }
        mSingleStep = false;

        // Render next frame.
        if(!glbObject->renderRoot->renderOneFrame()) break;

        // Allow platform to pump/create/etc messages/events once per frame.
        Ogre::WindowEventUtilities::messagePump();
    }
}
//---------------------------------------------------------------------
void EngineProducer::shutdownEngine() {mShutdownEngine = true;}
//---------------------------------------------------------------------