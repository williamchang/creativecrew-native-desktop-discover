/**
@file
    FindingHaven.h
@brief
    Coming soon.
@author
    William Chang
@version
    0.1
@date
    - Created: 2006-11-04
    - Modified: 2006-11-08
    .
*/

#ifndef __FindingHaven_H__
#define __FindingHaven_H__

#include "EngineProducer.h"

using namespace Discover;

/** @class FindingHaven */
class FindingHaven {
public:
    FindingHaven() {}
    virtual ~FindingHaven() {}
    void go() {
        EngineProducer app;
        app.start();
    }
};

#endif // #ifndef __FindingHaven_H__