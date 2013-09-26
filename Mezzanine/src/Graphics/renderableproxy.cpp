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
            Manager(Creator)
            {  }

        RenderableProxy::~RenderableProxy()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        AxisAlignedBox RenderableProxy::GetAABB() const
            { return AxisAlignedBox( this->_GetBaseGraphicsObject()->getBoundingBox() ); }

        WorldManager* RenderableProxy::GetCreator() const
            { return this->Manager; }

        ///////////////////////////////////////////////////////////////////////////////
        // Graphics Properties

        void RenderableProxy::SetVisible(const Bool Visible)
            { this->_GetBaseGraphicsObject()->setVisible(Visible); }

        Bool RenderableProxy::GetVisible() const
            { return this->_GetBaseGraphicsObject()->getVisible(); }

        void RenderableProxy::SetCastShadows(const Bool CastShadows)
            { this->_GetBaseGraphicsObject()->setCastShadows(CastShadows); }

        Bool RenderableProxy::GetCastShadows() const
            { return this->_GetBaseGraphicsObject()->getCastShadows(); }

        Bool RenderableProxy::GetReceiveShadows() const
            { return this->_GetBaseGraphicsObject()->getReceivesShadows(); }

        void RenderableProxy::SetLightMask(const UInt32 Mask)
            { this->_GetBaseGraphicsObject()->setLightMask(Mask); }

        UInt32 RenderableProxy::GetLightMask() const
            { return this->_GetBaseGraphicsObject()->getLightMask(); }

        void RenderableProxy::SetRenderDistance(const Real Distance)
            { this->_GetBaseGraphicsObject()->setRenderingDistance(Distance); }

        Real RenderableProxy::GetRenderDistance() const
            { return this->_GetBaseGraphicsObject()->getRenderingDistance(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void RenderableProxy::ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfRoot = ParentNode.AppendChild(this->GetDerivedSerializableName());

            this->ProtoSerializeProperties(SelfRoot);
        }

        void RenderableProxy::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            // We're at the base implementation, so no calling of child implementations
            XML::Node PropertiesNode = SelfRoot.AppendChild( RenderableProxy::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("Visible").SetValue( this->GetVisible() ? "true" : "false" ) &&
                PropertiesNode.AppendAttribute("CastShadows").SetValue( this->GetCastShadows() ? "true" : "false" ) &&
                PropertiesNode.AppendAttribute("LightMask").SetValue( this->GetLightMask() ) &&
                PropertiesNode.AppendAttribute("RenderDistance").SetValue( this->GetRenderDistance() ) )
            {
                XML::Node LocationNode = PropertiesNode.AppendChild("Location");
                this->GetLocation().ProtoSerialize( LocationNode );
                XML::Node OrientationNode = PropertiesNode.AppendChild("Orientation");
                this->GetOrientation().ProtoSerialize( OrientationNode );
                XML::Node ScaleNode = PropertiesNode.AppendChild("Scale");
                this->GetScale().ProtoSerialize( ScaleNode );

                return;
            }else{
                SerializeError("Create XML Attribute Values",RenderableProxy::GetSerializableName() + "Properties",true);
            }
        }

        void RenderableProxy::ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            this->ProtoDeSerializeProperties(SelfRoot);
        }

        void RenderableProxy::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            // We're at the base implementation, so no calling of child implementations
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

                    CurrAttrib = PropertiesNode.GetAttribute("RenderDistance");
                    if( !CurrAttrib.Empty() )
                        this->SetRenderDistance( CurrAttrib.AsReal() );

                    // Get the properties that need their own nodes
                    XML::Node PositionNode = PropertiesNode.GetChild("Location").GetFirstChild();
                    if( !PositionNode.Empty() ) {
                        Vector3 Loc(PositionNode);
                        this->SetLocation(Loc);
                    }

                    XML::Node OrientationNode = PropertiesNode.GetChild("Orientation").GetFirstChild();
                    if( !PositionNode.Empty() ) {
                        Quaternion Rot(OrientationNode);
                        this->SetOrientation(Rot);
                    }

                    XML::Node ScaleNode = PropertiesNode.GetChild("Scale").GetFirstChild();
                    if( !PositionNode.Empty() ) {
                        Vector3 Scale(ScaleNode);
                        this->SetScale(Scale);
                    }
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
    }//Graphics
}//Mezzanine

#endif
