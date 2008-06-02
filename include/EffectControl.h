/**
@file
    EffectControl.h
@brief
    Coming soon.
@author
    William Chang
@version
    0.1
@date
    - Created: 2006-11-04
    - Modified: 2007-01-27
    .
*/

#ifndef __EffectControl_H__
#define __EffectControl_H__

#include "EnginePrerequisites.h"

namespace Discover {

/** @class EffectControl */
    class EffectControl: public Ogre::FrameListener {
public:
    /// Default constructor.
    EffectControl();
    /// Default destructor.
    virtual ~EffectControl();

    /// Instantiate this.
    bool instantiateThis();

    /// Create overlay.
    static Ogre::Overlay* createOverlay(Ogre::String name);
    /// Create overlay container.
    static Ogre::OverlayContainer* createOverlayContainer(Ogre::String name, Ogre::Overlay* overlay, Ogre::Vector2 position, Ogre::Vector2 dimensions, Ogre::String material = "", Ogre::String caption = "");
protected:
    // Override Ogre::FrameListener.
    virtual bool frameStarted(const Ogre::FrameEvent& evt);
    virtual bool frameEnded(const Ogre::FrameEvent& evt);
};

} // END namespace Discover

#endif // #ifndef __EffectControl_H__