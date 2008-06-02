#include "Global.h"

/// @cond SPECIAL: extend and instantiate the Ogre::Singleton template.
namespace Ogre {
template<> Discover::Global* Ogre::Singleton<Discover::Global>::ms_Singleton = NULL;
} // END namespace Ogre
/// @endcond
namespace Discover {
//-------------------------------------------------------------------------------------
Global* Global::getSingletonPtr() {return ms_Singleton;}
Global& Global::getSingleton() {assert(ms_Singleton);return (*ms_Singleton);}
//-------------------------------------------------------------------------------------
Global::Global() {
    mgrObject = new ObjectManager();
    mgrInput = new InputManager();
    mgrConfig = new ConfigurationManager();
}
//-------------------------------------------------------------------------------------
Global::~Global() {
    if(mgrConfig) {delete mgrConfig;mgrConfig = NULL;}
    if(mgrInput) {delete mgrInput;mgrInput = NULL;}
    if(mgrObject) {delete mgrObject;mgrObject = NULL;}
}
//-------------------------------------------------------------------------------------
} // END namespace Discover