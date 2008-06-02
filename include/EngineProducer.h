/**
@file
    EngineProducer.h
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
*/

#ifndef __EngineProducer_H__
#define __EngineProducer_H__

#include "EnginePrerequisites.h"

namespace Discover {

/** @class EngineProducer */
class EngineProducer {
public:
    /// Default constructor.
    EngineProducer();
    /// Default destructor.
    virtual ~EngineProducer();

    /// Start engine.
    void start();
    /// Shutdown engine.
    void shutdownEngine();
protected:
    /// Instantiate engine.
    virtual bool instantiateThis();
    /// Instantiate Ogre.
    bool instantiateOgre();

    /// Register the source of resources.
    void registerResourceSource();
    /// Register resource listeners.
    virtual void registerResourceListener();
    /// Load resource group. Must at least do ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    virtual void loadResources();
    
    /// Register frame listeners.
    virtual void registerFrameListener();
    /// Start engine main loop.
    void startLoopMain();

    // Declare control objects.
    StageListener* lstnStage;
    DebugListener* lstnDebug;
private:
    // Declare common variables.
    bool mShutdownEngine;
};

} // END namespace Discover

#endif // #ifndef __EngineProducer_H__