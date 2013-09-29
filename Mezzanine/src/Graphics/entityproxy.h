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

#include "Graphics/renderableproxy.h"

namespace Ogre
{
    class Entity;
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
        class MEZZ_LIB EntityProxy : public RenderableProxy
        {
        protected:
            /// @internal
            /// @brief A pointer to the internal Entity this proxy is based on.
            Ogre::Entity* GraphicsEntity;
            /// @internal
            /// @brief A pointer to the mesh being used by this proxy.
            Mesh* ProxyMesh;

            /// @internal
            /// @brief Creates an internal entity based on the provided mesh.
            /// @param MeshName The name of the mesh to be loaded and applied to this proxy.
            /// @param GroupName The resource group name where the mesh can be found.
            virtual void CreateEntity(const String& MeshName, const String& GroupName);
            /// @internal
            /// @brief Creates an internal entity based on the provided mesh.
            /// @param ProxyMesh A pointer to the to be applied to this proxy.
            virtual void CreateEntity(Mesh* ProxyMesh);
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

            /// @todo Create sub-entity access methods on this class.
            /// @todo Create minor material manipulation methods on this class.
            /// @todo Create Animation state and Skeleton access methods on this class.

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc WorldProxy::GetProxyType() const
            virtual Mezzanine::ProxyType GetProxyType() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Mesh Management

            /// @brief Sets the mesh to be used by this proxy.
            /// @param MeshName The name of the mesh to apply to this proxy.
            /// @param Group The resource group to which the mesh belongs.
            virtual void SetMesh(const String& MeshName, const String& Group);
            /// @brief Sets the mesh to be used by this proxy.
            /// @param Mesh The mesh to apply to this proxy.
            virtual void SetMesh(Mesh* ObjectMesh);
            /// @brief Gets the mesh currently being used by this proxy.
            /// @return Returns a pointer to the mesh currently being used by this proxy.
            virtual Mesh* GetMesh() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Entity Properties

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc WorldProxy::ProtoSerialize(XML::Node&) const
            virtual void ProtoSerialize(XML::Node& ParentNode) const;
            /// @copydoc RenderableProxy::ProtoSerializeProperties(XML::Node& SelfRoot) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
            /// @brief Convert the mesh of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeMesh(XML::Node& SelfRoot) const;

            /// @copydoc WorldProxy::ProtoDeSerialize(const XML::Node&)
            virtual void ProtoDeSerialize(const XML::Node& SelfRoot);
            /// @copydoc RenderableProxy::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);
            /// @brief Take the data stored in an XML Node and overwrite the mesh of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerializeMesh(const XML::Node& SelfRoot);

            /// @copydoc WorldProxy::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const;
            /// @copydoc WorldProxy::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Accessor for the internal entity.
            /// @return Returns a pointer to the internal entity this proxy is based on.
            virtual Ogre::Entity* _GetGraphicsObject() const;
            /// @copydoc RenderableProxy::_GetBaseGraphicsObject() const
            virtual Ogre::MovableObject* _GetBaseGraphicsObject() const;
        };//EntityProxy
    }//Graphics
}//Mezzanine

#endif
