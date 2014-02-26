// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _graphicsrenderableproxy_cpp
#define _graphicsrenderableproxy_cpp

/// @file
/// @brief This file contains the implementation for the base class from which graphics proxies inherit.

#include "Graphics/renderableproxy.h"
#include "Graphics/scenemanager.h"

#include "enumerations.h"
#include "exception.h"
#include "serialization.h"
#include "stringtool.h"

#include <Ogre.h>

namespace Mezzanine
{
    namespace Graphics
    {
        RenderableProxy::RenderableProxy(SceneManager* Creator) :
            GraphicsNode(NULL),
            Manager(Creator),
            VisibilityMask(Ogre::MovableObject::getDefaultVisibilityFlags()),
            QueryMask(Ogre::MovableObject::getDefaultQueryFlags()),
            InWorld(false)
        {
            this->GraphicsNode = this->Manager->_GetGraphicsWorldPointer()->getRootSceneNode()->createChildSceneNode();
        }

        RenderableProxy::~RenderableProxy()
        {
            this->GraphicsNode->getParentSceneNode()->removeChild(this->GraphicsNode);
            this->Manager->_GetGraphicsWorldPointer()->destroySceneNode(this->GraphicsNode);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        AxisAlignedBox RenderableProxy::GetAABB() const
            { return AxisAlignedBox( this->_GetBaseGraphicsObject()->getBoundingBox() ); }

        void RenderableProxy::AddToWorld()
        {
            if( !this->InWorld ) {
                this->_GetBaseGraphicsObject()->setVisibilityFlags( this->VisibilityMask );
                this->_GetBaseGraphicsObject()->setQueryFlags( this->QueryMask );
                this->InWorld = true;
            }
        }

        void RenderableProxy::RemoveFromWorld()
        {
            if( this->InWorld ) {
                this->_GetBaseGraphicsObject()->setVisibilityFlags(0);
                this->_GetBaseGraphicsObject()->setQueryFlags(0);
                this->InWorld = false;
            }
        }

        Boole RenderableProxy::IsInWorld() const
            { return this->InWorld; }

        WorldManager* RenderableProxy::GetCreator() const
            { return this->Manager; }

        ///////////////////////////////////////////////////////////////////////////////
        // RenderableProxy Properties

        void RenderableProxy::SetVisible(const Boole Visible)
            { this->_GetBaseGraphicsObject()->setVisible(Visible); }

        Boole RenderableProxy::GetVisible() const
            { return this->_GetBaseGraphicsObject()->getVisible(); }

        void RenderableProxy::SetCastShadows(const Boole CastShadows)
            { this->_GetBaseGraphicsObject()->setCastShadows(CastShadows); }

        Boole RenderableProxy::GetCastShadows() const
            { return this->_GetBaseGraphicsObject()->getCastShadows(); }

        Boole RenderableProxy::GetReceiveShadows() const
            { return this->_GetBaseGraphicsObject()->getReceivesShadows(); }

        void RenderableProxy::SetLightMask(const UInt32 Mask)
            { this->_GetBaseGraphicsObject()->setLightMask(Mask); }

        UInt32 RenderableProxy::GetLightMask() const
            { return this->_GetBaseGraphicsObject()->getLightMask(); }

        void RenderableProxy::SetVisibilityMask(const UInt32 Mask)
        {
            this->VisibilityMask = Mask;
            if( this->InWorld ) {
                this->_GetBaseGraphicsObject()->setVisibilityFlags( this->VisibilityMask );
            }
        }

        UInt32 RenderableProxy::GetVisibilityMask() const
            { return this->VisibilityMask; }

        void RenderableProxy::SetQueryMask(const UInt32 Mask)
        {
            this->QueryMask = Mask;
            if( this->InWorld ) {
                this->_GetBaseGraphicsObject()->setQueryFlags( this->QueryMask );
            }
        }

        UInt32 RenderableProxy::GetQueryMask() const
            { return this->QueryMask; }

        void RenderableProxy::SetRenderDistance(const Real Distance)
            { this->_GetBaseGraphicsObject()->setRenderingDistance(Distance); }

        Real RenderableProxy::GetRenderDistance() const
            { return this->_GetBaseGraphicsObject()->getRenderingDistance(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Transform Methods

        void RenderableProxy::SetLocation(const Vector3& Loc)
            { this->GraphicsNode->setPosition( Loc.GetOgreVector3() ); }

        void RenderableProxy::SetLocation(const Real X, const Real Y, const Real Z)
            { this->GraphicsNode->setPosition(X,Y,Z); }

        Vector3 RenderableProxy::GetLocation() const
            { return Vector3( this->GraphicsNode->getPosition() ); }

        void RenderableProxy::SetOrientation(const Quaternion& Ori)
            { this->GraphicsNode->setOrientation( Ori.GetOgreQuaternion() ); }

        void RenderableProxy::SetOrientation(const Real X, const Real Y, const Real Z, const Real W)
            { this->GraphicsNode->setOrientation(W,X,Y,Z); }

        Quaternion RenderableProxy::GetOrientation() const
            { return Quaternion( this->GraphicsNode->getOrientation() ); }

        void RenderableProxy::SetScale(const Vector3& Sc)
            { this->GraphicsNode->setScale( Sc.GetOgreVector3() ); }

        void RenderableProxy::SetScale(const Real X, const Real Y, const Real Z)
            { this->GraphicsNode->setScale(X,Y,Z); }

        Vector3 RenderableProxy::GetScale() const
            { return Vector3( this->GraphicsNode->getScale() ); }

        void RenderableProxy::Translate(const Vector3& Trans)
            { this->SetLocation( this->GetLocation() + Trans ); }

        void RenderableProxy::Translate(const Real X, const Real Y, const Real Z)
            { this->SetLocation( this->GetLocation() + Vector3(X,Y,Z) ); }

        void RenderableProxy::Yaw(const Real Angle)
            { this->SetOrientation( this->GetOrientation() * Quaternion(Angle,Vector3::Unit_Y()) ); }

        void RenderableProxy::Pitch(const Real Angle)
            { this->SetOrientation( this->GetOrientation() * Quaternion(Angle,Vector3::Unit_X()) ); }

        void RenderableProxy::Roll(const Real Angle)
            { this->SetOrientation( this->GetOrientation() * Quaternion(Angle,Vector3::Unit_Z()) ); }

        void RenderableProxy::Rotate(const Vector3& Axis, const Real Angle)
            { this->SetOrientation( this->GetOrientation() * Quaternion(Angle,Axis) ); }

        void RenderableProxy::Rotate(const Quaternion& Rotation)
            { this->SetOrientation( this->GetOrientation() * Rotation ); }

        void RenderableProxy::Scale(const Vector3& Scale)
            { this->SetScale( this->GetScale() * Scale ); }

        void RenderableProxy::Scale(const Real X, const Real Y, const Real Z)
            { this->SetScale( this->GetScale() * Vector3(X,Y,Z) ); }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void RenderableProxy::ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfRoot = ParentNode.AppendChild(this->GetDerivedSerializableName());
            if( !SelfRoot.AppendAttribute("InWorld").SetValue( this->IsInWorld() ? "true" : "false" ) ) {
                SerializeError("Create XML Attribute Values",WorldProxy::GetSerializableName(),true);
            }

            this->ProtoSerializeProperties(SelfRoot);
        }

        void RenderableProxy::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->WorldProxy::ProtoSerializeProperties(SelfRoot);

            XML::Node PropertiesNode = SelfRoot.AppendChild( RenderableProxy::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("Visible").SetValue( this->GetVisible() ? "true" : "false" ) &&
                PropertiesNode.AppendAttribute("CastShadows").SetValue( this->GetCastShadows() ? "true" : "false" ) &&
                PropertiesNode.AppendAttribute("LightMask").SetValue( this->GetLightMask() ) &&
                PropertiesNode.AppendAttribute("VisibilityMask").SetValue( this->GetVisibilityMask() ) &&
                PropertiesNode.AppendAttribute("QueryMask").SetValue( this->GetQueryMask() ) &&
                PropertiesNode.AppendAttribute("RenderDistance").SetValue( this->GetRenderDistance() ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",RenderableProxy::GetSerializableName() + "Properties",true);
            }
        }

        void RenderableProxy::ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            Boole WasInWorld = false;
            XML::Attribute InWorldAttrib = SelfRoot.GetAttribute("InWorld");
            if( !InWorldAttrib.Empty() ) {
                WasInWorld = StringTools::ConvertToBool( InWorldAttrib.AsString() );
            }

            this->ProtoDeSerializeProperties(SelfRoot);

            if( WasInWorld ) {
                this->AddToWorld();
            }
        }

        void RenderableProxy::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->WorldProxy::ProtoDeSerializeProperties(SelfRoot);

            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( RenderableProxy::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = PropertiesNode.GetAttribute("Visible");
                    if( !CurrAttrib.Empty() )
                        this->SetVisible( StringTools::ConvertToBool( CurrAttrib.AsString() ) );

                    CurrAttrib = PropertiesNode.GetAttribute("CastShadows");
                    if( !CurrAttrib.Empty() )
                        this->SetCastShadows( StringTools::ConvertToBool( CurrAttrib.AsString() ) );

                    CurrAttrib = PropertiesNode.GetAttribute("LightMask");
                    if( !CurrAttrib.Empty() )
                        this->SetLightMask( CurrAttrib.AsWhole() );

                    CurrAttrib = PropertiesNode.GetAttribute("VisibilityMask");
                    if( !CurrAttrib.Empty() )
                        this->SetVisibilityMask( CurrAttrib.AsWhole() );

                    CurrAttrib = PropertiesNode.GetAttribute("QueryMask");
                    if( !CurrAttrib.Empty() )
                        this->SetQueryMask( CurrAttrib.AsWhole() );

                    CurrAttrib = PropertiesNode.GetAttribute("RenderDistance");
                    if( !CurrAttrib.Empty() )
                        this->SetRenderDistance( CurrAttrib.AsReal() );
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (RenderableProxy::GetSerializableName() + "Properties" ) + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,RenderableProxy::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String RenderableProxy::GetDerivedSerializableName() const
            { return RenderableProxy::GetSerializableName(); }

        String RenderableProxy::GetSerializableName()
            { return "RenderableProxy"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Ogre::SceneNode* RenderableProxy::_GetGraphicsNode() const
            { return this->GraphicsNode; }
    }//Graphics
}//Mezzanine

#endif
