/**
@file
    LevelControl.h
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

#ifndef __LevelControl_H__
#define __LevelControl_H__

#include "EnginePrerequisites.h"
#include "LevelXmlParser.h"

namespace Discover {

/** @class LevelControl */
class LevelControl {
public:
    /// Default constructor.
    LevelControl() {}
    /// Default destructor.
    virtual ~LevelControl() {}
    /// Load level, parse XML file.
    void loadLevel(Ogre::String s) {
        try {
            Ogre::SceneNode* dotSceneNode = glbObject->renderSceneMgr->getRootSceneNode()->createChildSceneNode("DotSceneNode");
            Ogre::LogManager::getSingleton().logMessage("Loading the scene");
            mDotScene.parseDotScene(s, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, glbObject->renderSceneMgr, glbObject->physicsWorld, dotSceneNode);
            Ogre::LogManager::getSingleton().logMessage("Finished loading the scene!");
            
            // StaticGeometry. http://www.ogre3d.org/docs/api/html/classOgre_1_1StaticGeometry.html#_details
            /*
            dotSceneNode->setVisible(false);
            Ogre::StaticGeometry* dotStaticGeometry = glbObject->renderSceneMgr->createStaticGeometry("DotStaticGeometry");
            dotStaticGeometry->setRenderingDistance(1000);
            dotStaticGeometry->setRegionDimensions(Ogre::Vector3(500, 500, 500));
            dotStaticGeometry->setCastShadows(false);
            dotStaticGeometry->addSceneNode(dotSceneNode);
            dotStaticGeometry->build();
            glbObject->renderSceneMgr->destroySceneNode(dotSceneNode->getName());
            */
        } catch(Ogre::Exception& e) { 
            Ogre::LogManager::getSingleton().logMessage(Ogre::String("Unable to parse level: ") + e.getFullDescription());
        } 
    }
private:
    LevelXmlParser mDotScene;
};

} // END namespace Discover

#endif // #ifndef __LevelControl_H__