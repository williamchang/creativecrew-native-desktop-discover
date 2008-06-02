#include "EngineHeaders.h"

using namespace Discover;

//---------------------------------------------------------------------
CharacterControl::CharacterControl() {if(!instantiateThis()) return;}
//---------------------------------------------------------------------
CharacterControl::~CharacterControl() {}
//---------------------------------------------------------------------
bool CharacterControl::instantiateThis() {
    return true;
}
//---------------------------------------------------------------------
void CharacterControl::createCharacter(const Ogre::String& name, Ogre::Vector3 position) {
    /*
    // Create physics body.
    nxOgre::blueprint<nxOgre::character> bpCharacter;
    bpCharacter.setToDefault();
    bpCharacter.setGravity(true);
    bpCharacter.setSlopeLimit(45);
    bpCharacter.setStep(0.2);
    bpCharacter.setShapeAsCapsule(0.35, 0.75);
    bpCharacter.setMesh("");
    mCharacter = bpCharacter.create(name, position, glbObject->physicsScene);

    // Render character body.
    mEnt = glbObject->renderSceneMgr->createEntity(name + "ninja", "ninja.mesh");
    mEnt->setCastShadows(true);
    mNode = mCharacter->mNode->createChildSceneNode();
    mNode->attachObject(mEnt);

    // Initial position, scale, direction.
    vecPosition = Ogre::Vector3(0, -0.7, 0); // Offset from collision mesh.
    vecSize = Ogre::Vector3(0.008, 0.008, 0.008);
    vecLookAt = Ogre::Vector3(-10, 0, 0);

    // Set position, scale, direction.
    mNode->setPosition(vecPosition);
    mNode->setScale(vecSize);
    this->setDirection(vecLookAt);

    // Set initial animation.
    mAnimationState = mEnt->getAnimationState("Idle1");
    mAnimationState->setLoop(true);
    mAnimationState->setEnabled(true);

    // Set initial locomotion.
    realWalkSpeed = 0.8;
    realJumpThrust = 1;
    // Set initial yaw.
    realYawSpeed = 2000;
    strYawOrientation = "right";

    // Instantiate others.
    keyCount = 0;
    isKeyHold = false;
    isKeyReleased = false;
    isJump = false;
    */
}
//---------------------------------------------------------------------
void CharacterControl::setAnimation(CharacterControl::AnimationMode mode) {
    switch(mode) {
        case CharacterControl::AM_WALK:
            mAnimationState = mEnt->getAnimationState("Walk");
            mAnimationState->setEnabled(true);
            isKeyHold = true;
            isKeyReleased = false;
            break;
        case CharacterControl::AM_RUN:
            mAnimationState = mEnt->getAnimationState("Run");
            mAnimationState->setEnabled(true);
            isKeyHold = true;
            isKeyReleased = false;
            break;
        case CharacterControl::AM_ATTACK:
            mAnimationState = mEnt->getAnimationState("Attack1");
            mAnimationState->setEnabled(true);
            mAnimationState->setTimePosition(0);
            mAnimationState->setLoop(false);
            isKeyHold = false;
            isKeyReleased = true;
            break;
    }
}
//---------------------------------------------------------------------
void CharacterControl::runOneFrame(const Ogre::FrameEvent& evt) {
    /*
    // Key state and character state.
    if(isKeyHold)
        mAnimationState->setLoop(true);
    if(!isKeyReleased && (bool)mCharacter->mController->getActor()->getLinearVelocity().isZero()) {
        mAnimationState->setLoop(false);
        isKeyHold = false;
        isKeyReleased = true;
    }

    // Set animation state.
    if(mAnimationState) {
        // Set speed of animation for each frame.
        mAnimationState->addTime(evt.timeSinceLastFrame * 1.5);
        
        if(mAnimationState->getTimePosition() >= mAnimationState->getLength()) {
            mAnimationState = mEnt->getAnimationState("Idle1");
            mAnimationState->setLoop(true);
            mAnimationState->setEnabled(true);
        }
    }
    */
}
//---------------------------------------------------------------------
void CharacterControl::setDirection(Ogre::Vector3 lookAt) {
    lookAt.normalise();
    Ogre::Quaternion q = mNode->getOrientation().xAxis().getRotationTo(lookAt) * mNode->getOrientation().zAxis().getRotationTo(lookAt);
    //mCharacter->turn(q);
}
//---------------------------------------------------------------------
void CharacterControl::setLocomotion(CharacterControl::LocomotionMode mode) {
    switch(mode) {
        case CharacterControl::LM_FORWARD:
            //mCharacter->mNextMovement.z = -1 * realWalkSpeed;
            this->setAnimation(CharacterControl::AM_WALK);
            break;
        case CharacterControl::LM_JUMP:
            //mCharacter->move(nxOgre::character::JUMP);
            this->setAnimation(CharacterControl::AM_JUMP);
            break;
        case CharacterControl::LM_ATTACK:
            this->setAnimation(CharacterControl::AM_ATTACK);
            break;
    }
}
//---------------------------------------------------------------------
void CharacterControl::setUser(UserControl& user) {
    // Instantiate user control.
    mCtrlUser = &user;
    mCtrlUser->instantiateThis();
    mCtrlUser->setMode(UserControl::UM_PLAYER);
    mCtrlUser->setCharacter(*this);
}
//---------------------------------------------------------------------
OgreBulletDynamics::RigidBody* CharacterControl::getCharacter() {
    return mCharacter;
}
//---------------------------------------------------------------------