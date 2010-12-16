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
#ifndef _node_h
#define _node_h

#include "attachable.h"
#include "crossplatformexport.h"
#include "datatypes.h"
#include "vector3.h"
#include "quaternion.h"

namespace Ogre
{
    class SceneNode;
}

namespace phys
{
    class SceneManager;
    class Camera;
    class Light;
    class ParticleEffect;
    class ActorBase;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class Node
    /// @headerfile node.h
    /// @brief This is a helper class which non-physics objects, such as lights, cameras, ribbon trails, and
    /// particle effects may be attached to for enhanced effects.
    /// @details This class simply contains a group of functions for easy manipulation of movement and orientation
    /// as well as attaching elements to them.  This class should only be created through the scene manager. @n
    /// There are 4 types of nodes: @n
    /// Center - Works in conjunction with the orbit node and serves as it's center point for the orbit node to
    /// rotate around. @n
    /// Orbit - Counterpart to the Center node, this type does the orbiting. @n
    /// Stand - Stationary node that points in one direction, that is it. @n
    /// Free - A node with no rules governing how it moves. @n
    /// Note:  There are no rules restricting the use of nodes anyway, this enum is here simply to help
    /// indicate the intended use upon creation.
    ///////////////////////////////////////
    class PHYS_LIB Node
    {
        public:
            enum NodeType
            {
                Center,
                Orbit,
                Stand,
                Free
            };
        protected:
            /// @brief Pointer to the ogre scenenode from which this class gets it's functionality.
            Ogre::SceneNode* OgreNode;
            /// @brief Pointer to the manager that created this class.
            SceneManager* Manager;
            /// @brief Enum value storing the type of node this class is.
            Node::NodeType Type;
            /// @brief Vector storing all attached cameras.
            std::vector< Attachable* > Elements;
        public:
            /// @brief Standard initialization constructor.
            /// @param Name The name of this node.
            /// @param manager Pointer to the manager that this node is to be used in.
            Node(const String& Name, SceneManager* manager);
            /// @brief Internal constructor.
            /// @details This constructor should not be called on manually.
            /// @param snode Pointer to the Ogre SceneNode this class is based on.
            /// @param manager Pointer to the manager that this node is to be used in.
            Node(Ogre::SceneNode* snode, SceneManager* manager);
            /// @brief Class destructor.
            ~Node();
            /// @brief Gets the name of this node.
            /// @return Returns a string containing the name given to this node.
            ConstString& GetName();
            /// @brief Attaches an attachable element to this Node.
            /// @param Element The Element to be attached.
            void AttachElement(Attachable* Element);
            /// @brief Detaches an attachable element from this Node.
            /// @param Element The Element to be detached.
            void DetachElement(Attachable* Element);
            /// @brief Detaches all attached cameras, lights, and particle effects.
            void DetachAllElements();
            /// @brief Gets the number of elements attached to this node.
            /// @return Returns the number of elements attached to this node.
            Whole GetNumAttachedElements();
            /// @brief Sets the position of this node.
            /// @param Position A vector3 representing the location of this node.
            void SetPosition(Vector3 Position);
            /// @brief Gets the position of this node.
            /// @return Returns a vector3 representing the location of this node.
            Vector3 GetPosition();
            /// @brief Sets the orientation of this node.
            /// @param Position A Quaternion representing the orientation of this node.
            void SetOrientation(Quaternion Orientation);
            /// @brief Gets the orientation of this node.
            /// @return Returns a quaternion representing the orientation of this node.
            Quaternion GetOrientation();
            /// @brief Automatically sets the orientation needed to point this node at a location in the world.
            /// @param LookAt The location in world space to point at.
            void LookAt(Vector3 LookAt);
            /// @brief Makes this node autotrack another node.
            /// @details This function will make this node update it's orientation every frame automatically so
            /// that it's always facing the target node.
            /// @param node The node to be tracked.
            /// @param Offset A vector3 representing the offset from the nodes location to be tracked.
            void SetAutoTracking(Node* node, Vector3 Offset=Vector3());
            /// @brief Makes this node autotrack an actor.
            /// @details This function will make this node update it's orientation every frame automatically so
            /// that it's always facing the target actor.
            /// @param Actor The actor to be tracked.
            /// @param Offset A vector3 representing the offset from the actor's location to be tracked.
            void SetAutoTracking(ActorBase* Actor, Vector3 Offset=Vector3());
            /// @brief Disables any active autotracking for this node.
            void DisableAutoTracking();
            /// @brief Increments the orbit of this node if this is an orbiting node.
            /// @details This function has no effect if this isn't an orbiting node.
            /// @param Radians A real reprsenting the amount of orbit to increment in radians.
            void IncrementOrbit(Real Radians);
            /// @brief Sets the type of node that this is.
            /// @details This is intended for internal/expert use only.  Manually calling this could disrupt normal function.
            /// @param type The type of node this is to be set as.
            void SetType(Node::NodeType type);
            /// @brief Gets the type of node that this is.
            /// @return Returns the type of node this is set as.
            Node::NodeType GetType();
    };//node
}//phys

#endif
