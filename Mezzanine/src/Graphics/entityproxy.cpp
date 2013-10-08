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
#ifndef _graphicsentityproxy_cpp
#define _graphicsentityproxy_cpp

/// @file
/// @brief This file contains the implementation for the World proxy wrapping basic entity(mesh) functionality.

#include "Graphics/entityproxy.h"
#include "Graphics/scenemanager.h"
#include "Graphics/meshmanager.h"
#include "Graphics/mesh.h"

#include "enumerations.h"
#include "exception.h"
#include "serialization.h"
#include "stringtool.h"

#include <Ogre.h>

namespace Mezzanine
{
    namespace Graphics
    {
        EntityProxy::EntityProxy(Mesh* TheMesh, SceneManager* Creator) :
            RenderableProxy(Creator),
            GraphicsEntity(NULL),
            ProxyMesh(NULL)
            { this->CreateEntity(TheMesh); }

        EntityProxy::EntityProxy(const String& MeshName, const String& GroupName, SceneManager* Creator) :
            RenderableProxy(Creator),
            GraphicsEntity(NULL),
            ProxyMesh(NULL)
            { this->CreateEntity(MeshName,GroupName); }

        EntityProxy::~EntityProxy()
            { this->DestroyEntity(); }

        void EntityProxy::CreateEntity(Mesh* ObjectMesh)
        {
            if( ObjectMesh != NULL ) {
                this->GraphicsEntity = this->Manager->_GetGraphicsWorldPointer()->createEntity( ObjectMesh->_GetInternalMesh() );
                this->GraphicsNode->attachObject( this->GraphicsEntity );
                this->GraphicsEntity->setUserAny( Ogre::Any( static_cast<RenderableProxy*>( this ) ) );
                this->GraphicsEntity->setVisibilityFlags(0);
                this->GraphicsEntity->setQueryFlags(0);

                this->ProxyMesh = ObjectMesh;
            }else{
                MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to create internal entity with NULL Mesh.  This may be caused by an invalid mesh name or resource group being provided.");
            }
        }

        void EntityProxy::CreateEntity(const String& MeshName, const String& GroupName)
        {
            Mesh* TheMesh = MeshManager::GetSingletonPtr()->LoadMesh(MeshName,GroupName);
            this->CreateEntity(TheMesh);
        }

        void EntityProxy::DestroyEntity()
        {
            if( this->GraphicsEntity ) {
                this->GraphicsNode->detachObject( this->GraphicsEntity );
                this->Manager->_GetGraphicsWorldPointer()->destroyEntity( this->GraphicsEntity );
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Mezzanine::ProxyType EntityProxy::GetProxyType() const
        {
            return PT_Graphics_EntityProxy;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Mesh Management

        void EntityProxy::SetMesh(const String& MeshName, const String& Group)
        {
            if( this->GraphicsEntity ) {
                Bool Visible = this->GetVisible();
                Bool CastShadow = this->GetCastShadows();
                UInt32 LMask = this->GetLightMask();
                UInt32 VMask = this->GetVisibilityMask();
                UInt32 QMask = this->GetQueryMask();
                Real RenderDist = this->GetRenderDistance();

                this->DestroyEntity();
                this->CreateEntity(MeshName,Group);

                this->SetVisible( Visible );
                this->SetCastShadows( CastShadow );
                this->SetLightMask( LMask );
                this->SetVisibilityMask( VMask );
                this->SetQueryMask( QMask );
                this->SetRenderDistance( RenderDist );
            }else{
                this->CreateEntity(MeshName,Group);
            }
        }

        void EntityProxy::SetMesh(Mesh* ObjectMesh)
        {
            if( this->GraphicsEntity ) {
                Bool Visible = this->GetVisible();
                Bool CastShadow = this->GetCastShadows();
                UInt32 LMask = this->GetLightMask();
                UInt32 VMask = this->GetVisibilityMask();
                UInt32 QMask = this->GetQueryMask();
                Real RenderDist = this->GetRenderDistance();

                this->DestroyEntity();
                this->CreateEntity(ObjectMesh);

                this->SetVisible( Visible );
                this->SetCastShadows( CastShadow );
                this->SetLightMask( LMask );
                this->SetVisibilityMask( VMask );
                this->SetQueryMask( QMask );
                this->SetRenderDistance( RenderDist );
            }else{
                this->CreateEntity(ObjectMesh);
            }
        }

        Mesh* EntityProxy::GetMesh() const
        {
            return this->ProxyMesh;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Entity Properties

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void EntityProxy::ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfRoot = ParentNode.AppendChild(this->GetDerivedSerializableName());
            if( !SelfRoot.AppendAttribute("InWorld").SetValue( this->IsInWorld() ? "true" : "false" ) ) {
                SerializeError("Create XML Attribute Values",EntityProxy::GetSerializableName(),true);
            }

            this->ProtoSerializeProperties(SelfRoot);
            this->ProtoSerializeMesh(SelfRoot);
        }

        void EntityProxy::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->RenderableProxy::ProtoSerializeProperties(SelfRoot);
        }

        void EntityProxy::ProtoSerializeMesh(XML::Node& SelfRoot) const
        {
            XML::Node MeshNode = SelfRoot.AppendChild( EntityProxy::GetSerializableName() + "Mesh" );

            if( MeshNode.AppendAttribute("Version").SetValue("1") &&
                MeshNode.AppendAttribute("ProxyMeshName").SetValue( this->ProxyMesh->GetName() ) &&
                MeshNode.AppendAttribute("ProxyMeshGroup").SetValue( this->ProxyMesh->GetGroup() ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",EntityProxy::GetSerializableName() + "Mesh",true);
            }
        }

        void EntityProxy::ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            Bool WasInWorld = false;
            XML::Attribute InWorldAttrib = SelfRoot.GetAttribute("InWorld");
            if( !InWorldAttrib.Empty() ) {
                WasInWorld = StringTools::ConvertToBool( InWorldAttrib.AsString() );
            }

            this->ProtoDeSerializeMesh(SelfRoot);
            this->ProtoDeSerializeProperties(SelfRoot);

            if( WasInWorld ) {
                this->AddToWorld();
            }
        }

        void EntityProxy::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->RenderableProxy::ProtoDeSerializeProperties(SelfRoot);
        }

        void EntityProxy::ProtoDeSerializeMesh(const XML::Node& SelfRoot)
        {
            XML::Attribute CurrAttrib;
            XML::Node MeshNode = SelfRoot.GetChild( EntityProxy::GetSerializableName() + "Mesh" );

            if( !MeshNode.Empty() ) {
                if(MeshNode.GetAttribute("Version").AsInt() == 1) {
                    String MeshName, MeshGroup = Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME;

                    CurrAttrib = MeshNode.GetAttribute("ProxyMeshName");
                    if( !CurrAttrib.Empty() )
                        MeshName = CurrAttrib.AsString();

                    CurrAttrib = MeshNode.GetAttribute("ProxyMeshGroup");
                    if( !CurrAttrib.Empty() )
                        MeshGroup = CurrAttrib.AsString();

                    Mesh* NewMesh = MeshManager::GetSingletonPtr()->LoadMesh( MeshName, MeshGroup );
                    this->SetMesh( NewMesh );
                }
            }
        }

        String EntityProxy::GetDerivedSerializableName() const
            { return EntityProxy::GetSerializableName(); }

        String EntityProxy::GetSerializableName()
            { return "EntityProxy"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Ogre::Entity* EntityProxy::_GetGraphicsObject() const
            { return this->GraphicsEntity; }

        Ogre::MovableObject* EntityProxy::_GetBaseGraphicsObject() const
            { return this->GraphicsEntity; }
    }//Graphics
}//Mezzanine

#endif
