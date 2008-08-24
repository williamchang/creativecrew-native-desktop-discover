/**
@file
    Discover.h
@brief
    Coming soon.
@author
    William Chang
@version
    0.1
@date
    - Created: 2006-11-04
    - Modified: 2007-03-28
    .
@note
    References:
    - C/C++ Building Reference:
        - http://msdn2.microsoft.com/en-us/library/91621w01.aspx
        .
    - Organizing Code Files in C and C++:
        - http://www.gamedev.net/reference/articles/article1798.asp
        .
    -  Managing C++ Objects:
        - http://billharlan.com/pub/papers/Managing_Cpp_Objects.html
        .
    .
*/

#ifndef __Discover_H__
#define __Discover_H__

#include "Global.h"

#include "CameraControl.h"
#include "LoadingControl.h"
#include "UserControl.h"
#include "LevelControl.h"
#include "CharacterControl.h"
#include "EffectControl.h"
#include "NarrationControl.h"

/**
@mainpage Discover Engine
@section intro_sec Introduction
    Based on C++ object-oriented programming (OPP). A game engine framework that
    integrates Ogre (Object-Oriented Graphics Rendering Engine) and
    OgreBullet (Bullet Collision Detection and Physics).

    - Creative Crew: http://www.creativecrew.org/
    - Google Code: http://creativecrew.googlecode.com/
    - Ogre: http://www.ogre3d.org/
    - OgreBullet: http://www.ogre3d.org/phpBB2addons/viewforum.php?f=12
    - Bullet Collision Detection and Physics: http://www.bulletphysics.com/
    .
@section new Features:

    <strong>Fundamental Features:</strong>
        - Updated and integrated code-base to OGRE 1.4.9 [Eihort]. <strong>[DONE]</strong>
        - Rewritten base class. <strong>[DONE]</strong>
        - Working narration module (basic slide show). <strong>[DONE]</strong>
            - Can play based on time and events. <strong>[DONE]</strong>
            - Display slides based on popular image formats (ex: jpg, gif, png, bmp). <strong>[DONE]</strong>
            - Can display caption with or without slides. <strong>[DONE]</strong>
            .
        - Integrated OgreBullet, the physics (Bullet) into code-base (solution). <strong>[DONE]</strong>
        - Loading world (scene). <strong>[DONE]</strong>
        - Apply collision to world (scene). <strong>[DONE]</strong>
        - Object-oriented ready (character.h class). <strong>[DONE]</strong>
        - Loading model to scene. (Credit to Nike and Jim.) <strong>[DONE]</strong>
        - Animating model basically functional (for instance, attack, idle, walk). (Credit to Nike and Jim.) <strong>[DONE]</strong>
        - Translate and rotate model with animation by player (control by keyboard). (Credit to Nike and Jim.) <strong>[DONE]</strong>
        - The "character.h" class is OgreNewt (physics) compatiable. <strong>[DONE]</strong>
        - Apply rigid body (using CollisionPrimitives::Box) and mass. <strong>[DONE]</strong>
        - Halt translation on collision detection against the environment. <strong>[DONE]</strong>
        - Limit translation to the x and y plane (for 2D experience). <strong>[DONE]</strong>
        - Camera lock-on perpendicular to model's "hip" (for 2D experience). <strong>[DONE]</strong>
        - Add the player ability to shoot from the model's line-of-sight. <strong>[DONE]</strong>
        - Smooth player 180 degree rotation when going between right and left. <strong>[DONE]</strong>
        .

    <strong>Upcoming Features:</strong>
        - Smooth out animation and blend animations.
        - Redesign 2D concept level (using Autodesk Maya for level designing) with better presentation and better test cases.
        .
    @section install Installation
        TODO
    @section compiling Compiling
        TODO
    @section license License
        
        <strong>Creative Crew</strong>

        "Discover Engine" is licensed under the <a target="_blank" href="http://www.gnu.org/copyleft/lgpl.html">GNU Lesser Public License (LGPL)</a>.

        Under the LGPL you may use "Discover Engine" for any purpose you wish, as long as you:
        -# Release any modifications to the "Discover Engine" source back to the community.
        -# Pass on the source to "Discover Engine" with all the copyrights intact.
        -# Make it clear where you have customised it. 
*/

#endif // #ifndef __Discover_H__