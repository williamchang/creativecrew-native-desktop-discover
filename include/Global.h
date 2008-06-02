/**
@file
    Global.h
@brief
    Coming soon.
@author
    William Chang
@version
    0.1
@date
    - Created: 2006-11-04
    - Modified: 2008-06-02
    .
@note
    References:
    - Singleton:
        - http://www.ogre3d.org/wiki/index.php/Singleton
        - http://www.drizzle.com/~scottb/publish/gpgems1_singleton.htm
        - http://www.ogre3d.org/phpBB2/viewtopic.php?p=183076
        - http://www.ogre3d.org/phpBB2/viewtopic.php?t=24278
        - http://www.ogre3d.org/phpBB2/viewtopic.php?p=164392
        .
    - Pointers and Dynamic Allocation:
        - http://www.codeproject.com/cpp/pointers.asp
        - http://www.cplusplus.com/doc/tutorial/pointers.html
        - http://www.codersource.net/c++_pointers.html
        - http://richardbowles.tripod.com/cpp/cpp18.htm
        .
    - Pointers and References:
        - http://eckenrodehouse.net/thinkCScpp_html/chap16.htm
        - http://www.ogre3d.org/phpBB2/viewtopic.php?p=210086#210086
        .
    - Constant Variables:
        - http://www.possibility.com/Cpp/const.html
        - http://www.linuxjournal.com/article/7629
        .
    - Function Pointers:
        - http://www.newty.de/fpt/index.html
        .
    - Return Multiple Values:
        - http://answers.yahoo.com/question/index?qid=20061012215313AAYk5BA
        .
    .
@n
@code
    Using a singleton:
    /// Creating the global. This may only be called once!
    new Global();
    /// Obtaining a reference to the global instance.
    Global& rGlobal = Global::getSingleton();
    /// Obtaining a pointer to the global instance.
    Global* pGlobal = Global::getSingletonPtr();
@endcode
@note
    You may call the Singleton constructor only once. Calling it more often will
    result in a runtime exception. There is another semantical difference between
    getSingleton() and getSingletonPtr(): If the constructor is not called beforehand
    getSingletonPtr() will return a NULL-Pointer and getSingleton() will throw a
    runtime exception. So it is probably better to use getSingleton() most of the
    time, even though it is slightly slower. But you get a clearer response when
    you set something up wrongly.
*/

#ifndef __Global_H__
#define __Global_H__

#include <OgreSingleton.h>

#include "ConfigurationManager.h"
#include "ObjectManager.h"
#include "InputManager.h"

namespace Discover {

// Defines a preprocessor macro: #define search_string substitution_string
#define glbObject Global::getSingleton().mgrObject
#define glbInput Global::getSingleton().mgrInput
#define glbConfig Global::getSingleton().mgrConfig
#define glbCfgVar Global::getSingleton().mgrConfig->cfgVar
#define glbVar Global::getSingleton()

/** @class Global */
class Global: public Ogre::Singleton<Global> {
public:
    /// Default constructor.
    Global();
    /// Default destructor.
    ~Global();

    /// Get reference of singleton object.
    static Global& getSingleton();
    /// Get pointer of singleton object.
    static Global* getSingletonPtr();
    
    // Declare managers objects.
    ObjectManager* mgrObject;
    InputManager* mgrInput;
    ConfigurationManager* mgrConfig;

    // Declare time variables.
    /** Elapsed time in seconds since the last event.
        This gives you time between frame start & frame end,
        and between frame end and next frame start.
        @remarks
            This may not be the elapsed time but the average
            elapsed time between recently fired events.
    */
    Ogre::Real timeSinceLastEvent;
    /** Elapsed time in seconds since the last event of the same type,
        i.e. time for a complete frame.
        @remarks
            This may not be the elapsed time but the average
            elapsed time between recently fired events of the same type.
    */
    Ogre::Real timeSinceLastFrame;
};

} // END namespace Discover

#endif // #ifndef __Global_H__