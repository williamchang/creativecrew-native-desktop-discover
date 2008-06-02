/**
@file
    EnginePrerequisitess.h
@brief
    Coming soon.
@author
    William Chang
@version
    0.1
@date
    - Created: 2007-03-15
    - Modified: 2007-03-17
    .
@note
    References:
    - http://osdab.sourceforge.net/tips/predeclare.php?mode=advanced
    .
*/

#ifndef __EnginePrerequisites_H__
#define __EnginePrerequisites_H__

#include <Ogre.h>

#include "Global.h"

namespace Discover {

    // Define engine version
    #define ENGINE_VERSION_MAJOR 0
    #define ENGINE_VERSION_MINOR 1
    #define ENGINE_VERSION_PATCH 0
    #define ENGINE_VERSION_SUFFIX "Alpha"
    #define ENGINE_VERSION_NAME "Stone Age"

    #define ENGINE_VERSION ((ENGINE_VERSION_MAJOR << 16) | (ENGINE_VERSION_MINOR << 8) | ENGINE_VERSION_PATCH)

    // Pre-declare classes
    // Allows use of pointers in header files without including individual .h
    // so decreases dependencies between files
    class EngineProducer;

    class Utilities;

    class CameraControl;
    class LoadingControl;
    class UserControl;
    class LevelControl;
    class CharacterControl;
    class EffectControl;
    class NarrationControl;

    class DebugListener;
    class StageListener;

} // END namespace Discover

#endif // #ifndef __EnginePrerequisites_H__