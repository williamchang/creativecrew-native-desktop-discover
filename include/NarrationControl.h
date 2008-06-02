/**
@file
    NarrationControl.h
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
@note
    References:
    - C++ Syntax:
        - http://www-numi.fnal.gov/offline_software/srt_public_context/WebDocs/Companion/cxx_crib/class.html
        - http://www.fredosaurus.com/notes-cpp/expressions/exprsum.html
        - http://www.eggheadcafe.com/articles/20050511.asp
        .
    .
*/

#ifndef __NarrationControl_H__
#define __NarrationControl_H__

#include "EnginePrerequisites.h"
#include <OgreTextAreaOverlayElement.h>

using namespace Ogre;

/** @class NarrationControl */
class NarrationControl: public FrameListener {
public:
    // Constructor takes a RenderWindow because it uses that to determine input context
    // Standard constructor.
    NarrationControl(Ogre::RenderWindow* rw, Ogre::Camera* cam):
        mWindow(rw),
        mCamera(cam)
        {
            mTimer = new Timer();
            isOverlayCreated = false;
        }
    // Standard destructor.
    virtual ~NarrationControl() {}

    void setSlideShow(int set) {
        switch(set) {
            case 1:
                mTimer->reset();
                selectSlideSet = set;
                nextSlide = 1;
                showSlideShow(true);
                break;
            default:
                mDebugText = "SlideShow set does not exist.";
                showSlideShow(false);
                break;
        }
    }
    void showSlideShow(bool show) {
        if(mOverlay) {
            if(show) {
                mOverlay->show();
            }
            else {
                mOverlay->hide();
            }
        }
    }
    // Override frameStarted event to process that (don't care about frameEnded)
    bool frameStarted(const FrameEvent& evt) {
        switch(selectSlideSet) {
            case 1:
                switch(nextSlide) {
                    case 1:
                        if(recentTimerSeconds == 8.0) {
                            createOverlay("Introduction/Slide1", "Introduction Slide 1.");
                            nextSlide = 2;
                        }
                        break;
                    case 2:
                        if(recentTimerSeconds == 12.0) {
                            createOverlay("Introduction/Slide2", "Introduction Slide 2.");
                            nextSlide = 3;
                        }
                        break;
                    case 3:
                        if(recentTimerSeconds == 16.0) {
                            showSlideShow(false);
                        }
                }
                break;
        }
        if(mWindow->isClosed())
            return false;

        return true;
    }
    bool frameEnded(const FrameEvent& evt) {
        recentTimerSeconds = mTimer->getMilliseconds()*0.001;
        // Debug timer.
        mDebugText = "Timer: " + StringConverter::toString(recentTimerSeconds);
        return true;
    }

protected:
    // Declare standard objects.
    std::string mDebugText;

    Camera* mCamera;
    RenderWindow* mWindow;
    Overlay* mOverlay;
    Timer* mTimer;
    String lastSlideOverlay;
    long recentTimerSeconds;
    bool isOverlayCreated;
    int selectSlideSet;
    int lastSlide;
    int nextSlide;

    void transitionSlide(String s) {}
    void createOverlay(String s, String c) {
        if(isOverlayCreated == true) {
            OverlayManager::getSingleton().destroy("SlideOverlay");
            OverlayManager::getSingleton().destroyOverlayElement("SlidePanel");
            OverlayManager::getSingleton().destroyOverlayElement("SlideTextArea");
        }

        //OverlayManager& newOverlayManager = OverlayManager::getSingleton();

        // Create a container Panel.
        OverlayContainer* containerPanel = static_cast<OverlayContainer*>(
            OverlayManager::getSingleton().createOverlayElementFromTemplate("Introduction/TemplateSlidePanel", "Panel", "SlidePanel"));
        containerPanel->setMaterialName(s);

        // Create a element TextArea.
        TextAreaOverlayElement* elementTextArea = static_cast<TextAreaOverlayElement*>(
            OverlayManager::getSingleton().createOverlayElementFromTemplate("Introduction/TemplateSlideTextArea", "TextArea", "SlideTextArea"));
        elementTextArea->setCaption(c);

        // Create an Overlay, and add the container Panel.
        Overlay* newOverlay = OverlayManager::getSingleton().create("SlideOverlay");
        newOverlay->add2D(containerPanel);

        // Add the element TextArea to the container Panel.
        containerPanel->addChild(elementTextArea);

        // Show the Overlay.
        newOverlay->show();

        // Final check.
        mOverlay = newOverlay;
        isOverlayCreated = true;
    }
};

#endif // #ifndef __NarrationControl_H__