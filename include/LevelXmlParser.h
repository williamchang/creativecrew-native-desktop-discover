/**
@file
    LevelXmlParser.h
@brief
    This file provides some methods for the parsing of a .scene file, with support for userData.
@author
    William Chang
@version
    0.1
@date
    - Created: 2006-11-04
    - Modified: 2006-11-08
    .
*/

#ifndef __LevelXmlParser_H__
#define __LevelXmlParser_H__

#include <vector>
#include <string>
#include <Ogre.h>
#include <ObjectManager.h>

#include "tinyxml.h"

using namespace std;
using namespace Ogre;

/** @class nodeProperty */
class nodeProperty {
public:
    String nodeName;
    String propertyNm;
    String valueName;
    String typeName;

    nodeProperty(String node,String propertyName,String value,String type) {
        nodeName = node;
        propertyNm = propertyName;
        valueName = value;
        typeName = type;
    }
};

/** @class LevelXmlParser */
class LevelXmlParser {
public:
    LevelXmlParser() : mSceneMgr(0) {}
    virtual ~LevelXmlParser() {}

    void parseDotScene(const String &SceneName, const String& groupName, SceneManager *yourSceneMgr, OgreBulletDynamics::DynamicsWorld* mPhysicsScene, SceneNode *pAttachNode = NULL, String sPrependNode = "");
    String getProperty(String ndNm, String prop);

    vector<nodeProperty> nodeProperties;
protected:
    void processNode(TiXmlElement *XMLNode, SceneNode *pAttach, OgreBulletDynamics::DynamicsWorld* mPhysicsScene);
    SceneManager *mSceneMgr;
    String m_sPrependNode;
};

#endif // #ifndef __LevelXmlParser_H__