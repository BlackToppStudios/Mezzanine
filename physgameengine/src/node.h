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

#include "datatypes.h"

namespace Ogre
{
    class SceneNode;
}

namespace phys
{
    class SceneManager;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class Node
    /// @headerfile node.h
    /// @brief This is a helper class which non-physicsl objects, such as lights, cameras, ribbon trails, and
    /// particle effects may be attached to for enhanced effects.
    /// @details This class simply contains a group of functions for easy manipulation of movement and orientation
    /// as well as attaching objects to them.  This class should only be created through the scene manager. @n
    /// There are 4 types of nodes: @n
    /// Center - Works in conjunction with the orbit node and serves as it's center point for the orbit node to
    /// rotate around. @n
    /// Orbit - Counterpart to the Center node, this type does the orbiting. @n
    /// Stand - Stationary node that points in one direction, that is it. @n
    /// Free - A node with no rules governing how it moves.
    ///////////////////////////////////////
    class Node
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
            String& GetName();
    };//node
}//phys

#endif
