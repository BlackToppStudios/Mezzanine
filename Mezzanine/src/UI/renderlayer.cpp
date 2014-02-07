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
#ifndef _uirenderlayer_cpp
#define _uirenderlayer_cpp

#include "UI/renderlayer.h"
#include "UI/quadrenderable.h"
#include "UI/screen.h"
#include "mathtool.h"

namespace Mezzanine
{
    namespace UI
    {
        RenderLayer::RenderLayer(QuadRenderable* ParentRenderable) :
            Parent(ParentRenderable),
            IndexID(0),
            RotAngle(0)
        {
            this->PriAtlas = this->Parent->GetScreen()->GetPrimaryAtlas();
            this->Scale.SetValues(1,1);
        }

        RenderLayer::~RenderLayer()
            {  }

        void RenderLayer::RotationTransform(Vector2& Point, const Vector2& RotationCenter)
        {
            if(0 != this->RotAngle)
            {
                Real RotCos = MathTools::Cos(RotAngle);
                Real RotSin = MathTools::Sin(RotAngle);

                Point -= RotationCenter;

                Vector2 Copy = Point;
                Point.X = Copy.X * RotCos - Copy.Y * RotSin;
                Point.Y = Copy.X * RotSin + Copy.Y * RotCos;

                Point += RotationCenter;
            }
        }

        void RenderLayer::RotationTransform(Vector2& TopLeft, Vector2& TopRight, Vector2& BottomLeft, Vector2& BottomRight)
        {
            if( 0 == this->RotAngle )
                return;

            Vector2 RotCenter( TopLeft.X + ((BottomRight.X - TopLeft.X) * 0.5), TopLeft.Y + ((BottomRight.Y - TopLeft.Y) * 0.5) );
            this->RotationTransform(TopLeft,RotCenter);
            this->RotationTransform(TopRight,RotCenter);
            this->RotationTransform(BottomLeft,RotCenter);
            this->RotationTransform(BottomRight,RotCenter);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Whole RenderLayer::GetIndex() const
        {
            return this->IndexID;
        }

        void RenderLayer::NotifyActive()
        {
            /// @todo As settings are added that need may need to be modified, this method should be updated.
            /// Prime candidate for this is Animations.
        }

        void RenderLayer::NotifyInactive()
        {
            /// @todo As settings are added that need may need to be modified, this method should be updated.
            /// Prime candidate for this is Animations.
        }

        Rect RenderLayer::GetAreaRect() const
        {
            Rect Area(this->Parent->GetActualPosition(),this->Parent->GetActualSize());
            Area.ApplyScaling(this->Scale);
            return Area;
        }

        void RenderLayer::SetScale(const Vector2& Scaling)
        {
            if( this->Scale == Scaling )
                return;

            this->Scale = Scaling;
            this->_MarkDirty();
        }

        Vector2 RenderLayer::GetScale() const
        {
            return this->Scale;
        }

        Boolean RenderLayer::IsImageLayer() const
            { return ( this->GetLayerType() == UI::RLT_Image ); }

        Boolean RenderLayer::IsTextLayer() const
            { return ( this->IsSingleLineTextLayer() || this->IsMultiLineTextLayer() ); }

        Boolean RenderLayer::IsSingleLineTextLayer() const
            { return ( this->GetLayerType() == UI::RLT_SingleLineText ); }

        Boolean RenderLayer::IsMultiLineTextLayer() const
            { return ( this->GetLayerType() == UI::RLT_MultiLineText ); }

        ///////////////////////////////////////////////////////////////////////////////
        // Rotation Methods

        void RenderLayer::SetRotationDegrees(const Real& Degrees)
        {
            this->RotAngle = MathTools::DegreesToRadians(Degrees);
            this->_MarkDirty();
        }

        void RenderLayer::SetRotationRadians(const Real& Radians)
        {
            this->RotAngle = Radians;
            this->_MarkDirty();
        }

        Real RenderLayer::GetRotationDegrees() const
        {
            return MathTools::RadiansToDegrees(RotAngle);
        }

        Real RenderLayer::GetRotationRadians() const
        {
            return this->RotAngle;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Accessor Methods

        QuadRenderable* RenderLayer::GetParent() const
        {
            return this->Parent;
        }

        Screen* RenderLayer::GetScreen() const
        {
            return this->Parent->GetScreen();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void RenderLayer::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            QuadRenderer::ProtoSerializeProperties(SelfRoot);
            XML::Node PropertiesNode = SelfRoot.AppendChild( RenderLayer::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("RotAngle").SetValue(this->RotAngle) )
            {
                XML::Node ScaleNode = PropertiesNode.AppendChild("Scale");
                this->Scale.ProtoSerialize( ScaleNode );

                return;
            }else{
                SerializeError("Create XML Attribute Values",RenderLayer::GetSerializableName() + "Properties",true);
            }
        }

        void RenderLayer::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            QuadRenderer::ProtoDeSerializeProperties(SelfRoot);
            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( RenderLayer::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = PropertiesNode.GetAttribute("RotAngle");
                    if( !CurrAttrib.Empty() )
                        this->RotAngle = CurrAttrib.AsReal();

                    XML::Node ScaleNode = PropertiesNode.GetChild("Scale").GetFirstChild();
                    if( !ScaleNode.Empty() )
                        this->Scale.ProtoDeSerialize(ScaleNode);
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (RenderLayer::GetSerializableName() + "Properties") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,RenderLayer::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String RenderLayer::GetDerivedSerializableName() const
        {
            return RenderLayer::GetSerializableName();
        }

        String RenderLayer::GetSerializableName()
        {
            return "RenderLayer";
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        void RenderLayer::_MarkDirty()
        {
            this->Dirty = true;
            this->Parent->_MarkDirty();
        }

        void RenderLayer::_UpdateIndex(const Whole Index)
        {
            this->IndexID = Index;
        }
    }//UI
}//Mezzanine

#endif
