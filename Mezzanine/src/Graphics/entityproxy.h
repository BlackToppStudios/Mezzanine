// © Copyright 2010 - 2013 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
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
#ifndef _graphicsentityproxy_h
#define _graphicsentityproxy_h

/// @file
/// @brief This file contains the declaration for the World proxy wrapping basic entity(mesh) functionality.

#include "Graphics/graphicsproxy.h"

namespace Ogre
{
    class Entity;
    class SceneNode;
}

namespace Mezzanine
{
    namespace Graphics
    {
        class Mesh;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the proxy class for placing and manipulating a mesh in the scene.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB EntityProxy : public GraphicsProxy
        {
        protected:
            /// @internal
            /// @brief A pointer to the internal Entity this proxy is based on.
            Ogre::Entity* GraphicsEntity;
            /// @internal
            /// @brief A pointer to the internal object storing the entity transform.
            Ogre::SceneNode* GraphicsNode;

            /// @internal
            /// @brief Creates an internal entity based on the provided mesh.
            /// @param MeshName The name of the mesh to be loaded and applied to this proxy.
            /// @param GroupName The resource group name where the mesh can be found.
            virtual void CreateEntity(const String& MeshName, const String& GroupName);
            /// @internal
            /// @brief Creates an internal entity based on the provided mesh.
            /// @param TheMesh A pointer to the to be applied to this proxy.
            virtual void CreateEntity(Mesh* TheMesh);
            /// @internal
            /// @brief Destroys the internal entity in use by this proxy.
            virtual void DestroyEntity();
        public:
            /// @brief Standard constructor.
            /// @param MeshName The name of the mesh to be loaded and applied to this proxy.
            /// @param GroupName The resource group name where the mesh can be found.
            /// @param Creator A pointer to the manager that created this proxy.
            EntityProxy(const String& MeshName, const String& GroupName, SceneManager* Creator);
            /// @brief Loaded Mesh constructor.
            /// @brief Creates an internal entity based on the provided mesh.
            /// @param TheMesh A pointer to the to be applied to this proxy.
            EntityProxy(Mesh* TheMesh, SceneManager* Creator);
            /// @brief Class destructor.
            virtual ~EntityProxy();
        };//EntityProxy
    }//Graphics
}//Mezzanine

#endif
