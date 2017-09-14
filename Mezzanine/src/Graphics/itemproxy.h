// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _graphicsitemproxy_h
#define _graphicsitemproxy_h

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
        ///////////////////////////////////////
        class MEZZ_LIB ItemProxy : public RenderableProxy
        {
        protected:
            // We want to be able to preserve all our settings and set a mesh seemlessly, but the underlying implementation doesn't support that.
            // So we need a cache of all the settings we can access.  Our base class already handles two such settings, so we can skip those.

            /// @brief A pointer to the internal Entity this proxy is based on.
            Ogre::Entity* GraphicsEntity;
            /// @brief A pointer to the mesh being used by this proxy.
            Mesh* ProxyMesh;

            /// @brief Stores the maximum distance this object can be from the camera before it is clipped/culled.
            Real RenderDist;
            /// @brief A bitmask which will be compared against to determine which lights apply to this object.
            UInt32 LightMask;
            /// @brief Stores whether or not this object should be rendered, regardless of other criteria.
            Boole SceneVisible;
            /// @brief Stores whether or not this object can cast shadows on other objects.
            Boole CanCastShadows;

            /// @brief Creates an internal entity based on the provided mesh.
            /// @param ObjectMesh A pointer to the mesh to be applied to this proxy.
            virtual void CreateEntity(Mesh* ObjectMesh);
            /// @brief Creates an internal entity based on the provided mesh.
            /// @param MeshName The name of the mesh to be loaded and applied to this proxy.
            /// @param GroupName The resource group name where the mesh can be found.
            virtual void CreateEntity(const String& MeshName, const String& GroupName);
            /// @brief Destroys the internal entity in use by this proxy.
            virtual void DestroyEntity();
        public:
            /// @brief Blank constructor.
            /// @param ID The unique ID assigned to this ItemProxy.
            /// @param Creator A pointer to the manager that created this proxy.
            ItemProxy(const UInt32 ID, SceneManager* Creator);
            /// @brief Loaded Mesh constructor.
            /// @param ID The unique ID assigned to this ItemProxy.
            /// @param TheMesh A pointer to the mesh to be applied to this proxy.
            /// @param Creator A pointer to the manager that created this proxy.
            ItemProxy(const UInt32 ID, Mesh* TheMesh, SceneManager* Creator);
            /// @brief Standard constructor.
            /// @param ID The unique ID assigned to this ItemProxy.
            /// @param MeshName The name of the mesh to be loaded and applied to this proxy.
            /// @param GroupName The resource group name where the mesh can be found.
            /// @param Creator A pointer to the manager that created this proxy.
            ItemProxy(const UInt32 ID, const String& MeshName, const String& GroupName, SceneManager* Creator);
            /// @brief XML constructor.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            /// @param Creator A pointer to the manager that created this proxy.
            ItemProxy(const XML::Node& SelfRoot, SceneManager* Creator);
            /// @brief Class destructor.
            virtual ~ItemProxy();

            /// @todo Create sub-item access methods on this class.
            /// @todo Create minor material manipulation methods on this class.
            /// @todo Create Animation state and Skeleton access methods on this class.

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc EntityProxy::GetComponentType() const
            virtual Mezzanine::ComponentType GetComponentType() const override;
            /// @copydoc EntityProxy::IsStatic() const
            virtual Boole IsStatic() const override;

            /// @copydoc EntityProxy::Activate()
            virtual void Activate() override;
            /// @copydoc EntityProxy::Deactivate()
            virtual void Deactivate() override;

            ///////////////////////////////////////////////////////////////////////////////
            // Mesh Management

            /// @brief Sets the mesh to be used by this proxy.
            /// @param MeshName The name of the mesh to apply to this proxy.
            /// @param Group The resource group to which the mesh belongs.
            virtual void SetMesh(const String& MeshName, const String& Group);
            /// @brief Sets the mesh to be used by this proxy.
            /// @param ObjectMesh The mesh to apply to this proxy.
            virtual void SetMesh(Mesh* ObjectMesh);
            /// @brief Gets the mesh currently being used by this proxy.
            /// @return Returns a pointer to the mesh currently being used by this proxy.
            virtual Mesh* GetMesh() const;

            ///////////////////////////////////////////////////////////////////////////////
            // RenderableProxy Properties

            /// @copydoc RenderableProxy::SetVisible(const Boole)
            virtual void SetVisible(const Boole Visible) override;
            /// @copydoc RenderableProxy::GetVisible() const
            virtual Boole GetVisible() const override;
            /// @copydoc RenderableProxy::SetCastShadows(const Boole)
            virtual void SetCastShadows(const Boole CastShadows) override;
            /// @copydoc RenderableProxy::GetCastShadows() const
            virtual Boole GetCastShadows() const override;
            /// @copydoc RenderableProxy::GetReceiveShadows() const
            virtual Boole GetReceiveShadows() const override;

            /// @copydoc RenderableProxy::SetLightMask(const UInt32)
            virtual void SetLightMask(const UInt32 Mask) override;
            /// @copydoc RenderableProxy::GetLightMask() const
            virtual UInt32 GetLightMask() const override;
            /// @copydoc RenderableProxy::SetVisibilityMask(const UInt32)
            virtual void SetVisibilityMask(const UInt32 Mask) override;
            /// @copydoc RenderableProxy::GetVisibilityMask() const
            virtual UInt32 GetVisibilityMask() const override;
            /// @copydoc RenderableProxy::SetQueryMask(const UInt32)
            virtual void SetQueryMask(const UInt32 Mask) override;
            /// @copydoc RenderableProxy::GetQueryMask() const
            virtual UInt32 GetQueryMask() const override;

            /// @copydoc RenderableProxy::SetRenderDistance(const Real)
            virtual void SetRenderDistance(const Real Distance) override;
            /// @copydoc RenderableProxy::GetRenderDistance() const
            virtual Real GetRenderDistance() const override;

            ///////////////////////////////////////////////////////////////////////////////
            // Entity Properties

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc EntityProxy::ProtoSerialize(XML::Node&) const
            virtual void ProtoSerialize(XML::Node& ParentNode) const override;
            /// @copydoc EntityProxy::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const override;
            /// @brief Convert the mesh of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeMesh(XML::Node& SelfRoot) const;

            /// @copydoc EntityProxy::ProtoDeSerialize(const XML::Node)
            virtual void ProtoDeSerialize(const XML::Node& SelfRoot) override;
            /// @copydoc EntityProxy::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot) override;
            /// @brief Take the data stored in an XML Node and overwrite the mesh of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            virtual void ProtoDeSerializeMesh(const XML::Node& SelfRoot);

            /// @copydoc EntityProxy::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const override;
            /// @copydoc EntityProxy::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Accessor for the internal entity.
            /// @return Returns a pointer to the internal entity this proxy is based on.
            virtual Ogre::Entity* _GetGraphicsObject() const;
            /// @copydoc RenderableProxy::_GetBaseGraphicsObject() const
            virtual Ogre::MovableObject* _GetBaseGraphicsObject() const override;
        };//ItemProxy
    }//Graphics
}//Mezzanine

#endif
