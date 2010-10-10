//© Copyright 2010 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef _internalmotionstate_h_cpp
#define _internalmotionstate_h_cpp

// Keeps this file form being documented by doxygen
/// @cond 0

#include <Ogre.h>
#include "btBulletDynamicsCommon.h"

#include "vector3.h"
#include "quaternion.h"

namespace phys{
    namespace internal{
        ///////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @class PhysMotionState
        /// @headerfile internalmotionstate.h.cpp
        /// @brief This class is used by the actor class to sync between the physics world and the graphical world.
        /// @details This class provides the link for position and orientation between the two worlds in the engine.
        /// This is called on every step(frame) of the world to sync the actor if it has moved.
        /// @attention This filename ends in .h.cpp which means it is a header and should not be compiled with the regular cpp files, just included by them, but it is also a
        /// source file and should not shipped with the DLL when the SDK is released. This is used for engine internals that need to be used by multiple classes.
        ///////////////////////////////////////
        class PhysMotionState : public btMotionState {
            private:
                Ogre::SceneNode* snode;
                btTransform worldtrans;

            public:
                /// @internal
                /// @brief Default Constructor.
                /// @details Basic no-initialization constructor.
                PhysMotionState();

                /// @brief Constructor.
                /// @details The class constructor.
                /// @param scenenode The scenenode belonging to the actor.
                PhysMotionState(Ogre::SceneNode* scenenode);

                /// @brief Destructor.
                /// @details The class destructor.
                virtual ~PhysMotionState();

                /// @brief Sets the scenenode.
                /// @details Sets the scenenode to be sync'd every step.
                /// @param scenenode The scenenode belonging to the actor.
                void SetNode(Ogre::SceneNode* scenenode);

                /// @brief Sets the initial position.
                /// @details Sets the position the actor will be placed in when it is added to the world.
                /// This function is called on by the ActorBase function SetInitPosition().
                /// @param position The vector3 representing the location to be used.
                void SetPosition(Vector3 position);

                /// @brief Sets the initial orientation.
                /// @details Sets the orientation the actor will have when it is added to the world.
                /// This function is called on by the ActorBase function SetInitOrientation().
                /// @param orientation The vector3 representing the orientation to be used.
                void SetOrientation(Quaternion orientation);

                /// @brief Sets the initial position.
                /// @details This function is called on by the physics world upon adding the actor to the world.
                /// This function uses the previous set vector3 that was set with SetInitPosition(). @n
                /// Default position is (0,0,0).
                /// @param worldTrans The location and orientation data.
                virtual void getWorldTransform(btTransform &worldTrans) const;

                /// @brief Updates the position and orientation.
                /// @details This function is called each step(frame) by the physics world to sync the physics and graphical worlds.
                /// @param worldTrans The location and orientation data.
                virtual void setWorldTransform(const btTransform &worldTrans);
        };
    }// /internal
}// /phys

/// @endcond

#endif

