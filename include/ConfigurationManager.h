/**
@file
    ConfigurationManager.h
@brief
    Coming soon.
@author
    William Chang
@version
    0.1
@date
    - Created: 2006-11-11
    - Modified: 2008-06-02
    .
@note
    - Classification of settings:
        - cl_ : Client settings.
        - cg_ : Client game settings.
        - r_ : Renderer settings.
        - s_ : Sound engine settings.
        - m_ : Mouse settings.
        - in_ : Input Settings.
        - sv_ : Server settings.
        - g_ : Game settings.
        .
    .
*/

#ifndef __ConfigurationManager_H__
#define __ConfigurationManager_H__

#include <Ogre.h>

// Use this define to signify OIS will be used as a DLL
//(so that dll import/export macros are in effect)
#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

namespace Discover {

/** @class ConfigurationManager */
class ConfigurationManager {
public:
    /** @struct Variable */
    struct Variable {
        bool debugStats;
    };

    /// Default constructor.
    ConfigurationManager();
    /// Default destructor.
    virtual ~ConfigurationManager();

    /// Instantiate this.
    bool instantiateThis();
    /// Register manager's frame listeners.
    void registerFrameListener();

    /// Is key pressed.
    bool isKeyDown(OIS::Keyboard* device, const Ogre::String& action);

    Variable cfgVar;
};

} // END namespace Discover

#endif // #ifndef __ConfigurationManager_H__