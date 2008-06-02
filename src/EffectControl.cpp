#include "EffectControl.h"

using namespace Discover;

//---------------------------------------------------------------------
EffectControl::EffectControl() {if(!instantiateThis()) return;}
//---------------------------------------------------------------------
EffectControl::~EffectControl() {}
//---------------------------------------------------------------------
bool EffectControl::instantiateThis() {
    return true;
}
//---------------------------------------------------------------------
Ogre::Overlay* EffectControl::createOverlay(Ogre::String name) {
    return glbObject->renderOverlayMgr->create(name);
}
//---------------------------------------------------------------------
Ogre::OverlayContainer* EffectControl::createOverlayContainer(Ogre::String name, Ogre::Overlay* overlay, Ogre::Vector2 position, Ogre::Vector2 dimensions, Ogre::String material, Ogre::String caption) {
    Ogre::String type = "Panel";
    if(caption != "")
        type = "TextArea";

    Ogre::OverlayContainer* c = static_cast<Ogre::OverlayContainer*>(glbObject->renderOverlayMgr->createOverlayElement(type, name));

    c->setMetricsMode(Ogre::GMM_PIXELS);
    c->setLeft(position.x);
    c->setTop(position.y);
    c->setWidth(dimensions.x);
    c->setHeight(dimensions.y);

    if(material != "")
        c->setMaterialName(material);

    if(caption != "") {
        c->setCaption(caption);
        c->setParameter("font_name", "nxogrefont");
        c->setParameter("char_height", "16");
        c->setParameter("horz_align", "left");
    }

    overlay->add2D(c);
    c->show();

    return c;
}
//---------------------------------------------------------------------
bool EffectControl::frameStarted(const Ogre::FrameEvent& evt) {
    return true;
}
//---------------------------------------------------------------------
bool EffectControl::frameEnded(const Ogre::FrameEvent& evt) {
    return true;
}
//---------------------------------------------------------------------