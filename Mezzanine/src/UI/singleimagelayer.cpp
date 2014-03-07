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

#ifndef _uisingleimagelayer_cpp
#define _uisingleimagelayer_cpp

#include "UI/singleimagelayer.h"
#include "UI/quadrenderable.h"
#include "UI/screen.h"

namespace Mezzanine
{
    namespace UI
    {
        SingleImageLayer::SingleImageLayer(QuadRenderable* ParentRenderable) :
            ImageLayer(ParentRenderable)
        {
            this->FillColours[UI::QC_TopLeft] = ColourValue::White();
            this->FillColours[UI::QC_TopRight] = ColourValue::White();
            this->FillColours[UI::QC_BottomLeft] = ColourValue::White();
            this->FillColours[UI::QC_BottomRight] = ColourValue::White();
        }

        SingleImageLayer::~SingleImageLayer()
            {  }

        void SingleImageLayer::RedrawImpl(Boole Force)
        {
            // Get the Texel Offsets
            Real TexelOffsetX = this->Parent->GetScreen()->GetTexelOffsetX();
            Real TexelOffsetY = this->Parent->GetScreen()->GetTexelOffsetY();

            // Get the parent rect and apply the scaling
            Rect ActDims = this->GetAreaRect();

            // Apply the texel offsets
            Vector2 RectCorners[4];
            RectCorners[UI::QC_TopLeft].X = ActDims.Position.X + TexelOffsetX;                         RectCorners[UI::QC_TopLeft].Y = ActDims.Position.Y + TexelOffsetY;
            RectCorners[UI::QC_TopRight].X = (ActDims.Position.X + ActDims.Size.X) + TexelOffsetX;     RectCorners[UI::QC_TopRight].Y = ActDims.Position.Y + TexelOffsetY;
            RectCorners[UI::QC_BottomLeft].X = ActDims.Position.X + TexelOffsetX;                      RectCorners[UI::QC_BottomLeft].Y = (ActDims.Position.Y + ActDims.Size.Y) + TexelOffsetY;
            RectCorners[UI::QC_BottomRight].X = (ActDims.Position.X + ActDims.Size.X) + TexelOffsetX;  RectCorners[UI::QC_BottomRight].Y = (ActDims.Position.Y + ActDims.Size.Y) + TexelOffsetY;

            // Border and rotation
            if( 0.0 != this->BorderWidth ) {
                Vector2 OuterBorder[4];
                OuterBorder[UI::QC_TopLeft].X = RectCorners[UI::QC_TopLeft].X - BorderWidth;           OuterBorder[UI::QC_TopLeft].Y = RectCorners[UI::QC_TopLeft].Y - BorderWidth;
                OuterBorder[UI::QC_TopRight].X = RectCorners[UI::QC_TopRight].X + BorderWidth;         OuterBorder[UI::QC_TopRight].Y = RectCorners[UI::QC_TopRight].Y - BorderWidth;
                OuterBorder[UI::QC_BottomLeft].X = RectCorners[UI::QC_BottomLeft].X - BorderWidth;     OuterBorder[UI::QC_BottomLeft].Y = RectCorners[UI::QC_BottomLeft].Y + BorderWidth;
                OuterBorder[UI::QC_BottomRight].X = RectCorners[UI::QC_BottomRight].X + BorderWidth;   OuterBorder[UI::QC_BottomRight].Y = RectCorners[UI::QC_BottomRight].Y + BorderWidth;

                this->RotationTransform(RectCorners);
                this->RotationTransform(OuterBorder);
                this->DrawBorder(RectCorners,OuterBorder);
            }else{
                this->RotationTransform(RectCorners);
            }
            // Fill
            if( !this->IsCompletelyTransparent() ) {
                this->DrawFill(RectCorners,LayerSprite,FillColours);
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        UI::RenderLayerType SingleImageLayer::GetLayerType() const
            { return UI::RLT_SingleImage; }

        ///////////////////////////////////////////////////////////////////////////////
        // Fill Methods

        void SingleImageLayer::SetColour(const ColourValue& Colour)
        {
            this->FillColours[UI::QC_TopLeft] = Colour;
            this->FillColours[UI::QC_TopRight] = Colour;
            this->FillColours[UI::QC_BottomLeft] = Colour;
            this->FillColours[UI::QC_BottomRight] = Colour;
            this->_MarkDirty();
        }

        void SingleImageLayer::SetColour(const UI::QuadCorner Corner, const ColourValue& Colour)
        {
            this->FillColours[Corner] = Colour;
            this->_MarkDirty();
        }

        const ColourValue& SingleImageLayer::GetColour(const UI::QuadCorner Corner) const
        {
            return this->FillColours[Corner];
        }

        Boole SingleImageLayer::IsCompletelyTransparent() const
        {
            return ( 0 == this->FillColours[UI::QC_TopLeft].AlphaChannel &&
                     0 == this->FillColours[UI::QC_TopRight].AlphaChannel &&
                     0 == this->FillColours[UI::QC_BottomLeft].AlphaChannel &&
                     0 == this->FillColours[UI::QC_BottomRight].AlphaChannel );
        }

        void SingleImageLayer::SetSprite(Sprite* NewSprite)
        {
            if( this->LayerSprite != NewSprite ) {
                this->PriAtlas = ( NewSprite == NULL ? this->Parent->GetScreen()->GetPrimaryAtlas() : NewSprite->GetAtlasName() );
                this->LayerSprite = NewSprite;
                this->_MarkDirty();
            }
        }

        void SingleImageLayer::SetSprite(const String& SpriteName)
        {
            Sprite* NewSprite = this->Parent->GetScreen()->GetSprite(SpriteName,PriAtlas);
            this->SetSprite(NewSprite);
        }

        void SingleImageLayer::SetSprite(const String& SpriteName, const String& Atlas)
        {
            Sprite* NewSprite = this->Parent->GetScreen()->GetSprite(SpriteName,Atlas);
            this->SetSprite(NewSprite);
        }

        void SingleImageLayer::SetGradient(const UI::Gradient Grad, const ColourValue& ColourA, const ColourValue& ColourB)
        {
            switch(Grad)
            {
                case UI::Gradient_NorthSouth:
                {
                    this->FillColours[UI::QC_TopLeft] = ColourA;
                    this->FillColours[UI::QC_TopRight] = ColourA;
                    this->FillColours[UI::QC_BottomRight] = ColourB;
                    this->FillColours[UI::QC_BottomLeft] = ColourB;
                    break;
                }
                case UI::Gradient_WestEast:
                {
                    this->FillColours[UI::QC_TopLeft] = ColourA;
                    this->FillColours[UI::QC_BottomLeft] = ColourA;
                    this->FillColours[UI::QC_TopRight] = ColourB;
                    this->FillColours[UI::QC_BottomRight] = ColourB;
                    break;
                }
                case UI::Gradient_Diagonal_1:
                {
                    ColourValue Average;
                    Average.RedChannel = (ColourA.RedChannel + ColourB.RedChannel) * 0.5f;
                    Average.GreenChannel = (ColourA.GreenChannel + ColourB.GreenChannel) * 0.5f;
                    Average.BlueChannel = (ColourA.BlueChannel + ColourB.BlueChannel) * 0.5f;
                    Average.AlphaChannel = (ColourA.AlphaChannel + ColourB.AlphaChannel) * 0.5f;
                    this->FillColours[UI::QC_TopLeft] = ColourA;
                    this->FillColours[UI::QC_TopRight] = Average;
                    this->FillColours[UI::QC_BottomRight] = ColourB;
                    this->FillColours[UI::QC_BottomLeft] = Average;
                    break;
                }
                case UI::Gradient_Diagonal_2:
                {
                    ColourValue Average;
                    Average.RedChannel = (ColourA.RedChannel + ColourB.RedChannel) * 0.5f;
                    Average.GreenChannel = (ColourA.GreenChannel + ColourB.GreenChannel) * 0.5f;
                    Average.BlueChannel = (ColourA.BlueChannel + ColourB.BlueChannel) * 0.5f;
                    Average.AlphaChannel = (ColourA.AlphaChannel + ColourB.AlphaChannel) * 0.5f;
                    this->FillColours[UI::QC_TopLeft] = Average;
                    this->FillColours[UI::QC_TopRight] = ColourA;
                    this->FillColours[UI::QC_BottomRight] = Average;
                    this->FillColours[UI::QC_BottomLeft] = ColourB;
                    break;
                }
            }
            this->_MarkDirty();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void SingleImageLayer::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->ImageLayer::ProtoSerializeProperties(SelfRoot);
            XML::Node PropertiesNode = SelfRoot.AppendChild( SingleImageLayer::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("SpriteName").SetValue( this->LayerSprite ? this->LayerSprite->Name : "" ) &&
                PropertiesNode.AppendAttribute("SpriteAtlas").SetValue( this->LayerSprite ? this->LayerSprite->GetAtlasName() : "" ) )
            {
                XML::Node FillColoursNode = PropertiesNode.AppendChild("FillColours");
                XML::Node TopLeftFillNode = FillColoursNode.AppendChild("TopLeft");
                this->FillColours[UI::QC_TopLeft].ProtoSerialize(TopLeftFillNode);

                XML::Node TopRightFillNode = FillColoursNode.AppendChild("TopRight");
                this->FillColours[UI::QC_TopRight].ProtoSerialize(TopRightFillNode);

                XML::Node BottomLeftFillNode = FillColoursNode.AppendChild("BottomLeft");
                this->FillColours[UI::QC_BottomLeft].ProtoSerialize(BottomLeftFillNode);

                XML::Node BottomRightFillNode = FillColoursNode.AppendChild("BottomRight");
                this->FillColours[UI::QC_BottomRight].ProtoSerialize(BottomRightFillNode);

                return;
            }else{
                SerializeError("Create XML Attribute Values",SingleImageLayer::GetSerializableName() + "Properties",true);
            }
        }

        void SingleImageLayer::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->ImageLayer::ProtoDeSerializeProperties(SelfRoot);
            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( SingleImageLayer::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    String SpriteName, SpriteAtlas;

                    CurrAttrib = PropertiesNode.GetAttribute("SpriteName");
                    if( !CurrAttrib.Empty() )
                        SpriteName = CurrAttrib.AsString();

                    CurrAttrib = PropertiesNode.GetAttribute("SpriteAtlas");
                    if( !CurrAttrib.Empty() )
                        SpriteAtlas = CurrAttrib.AsString();

                    this->SetSprite(SpriteName,SpriteAtlas);

                    XML::Node FillColoursNode = PropertiesNode.GetChild("FillColours");
                    XML::Node TopLeftFillNode = FillColoursNode.GetChild("TopLeft").GetFirstChild();
                    if( !TopLeftFillNode.Empty() )
                        this->FillColours[UI::QC_TopLeft].ProtoDeSerialize(TopLeftFillNode);

                    XML::Node TopRightFillNode = FillColoursNode.GetChild("TopRight").GetFirstChild();
                    if( !TopRightFillNode.Empty() )
                        this->FillColours[UI::QC_TopRight].ProtoDeSerialize(TopRightFillNode);

                    XML::Node BottomLeftFillNode = FillColoursNode.GetChild("BottomLeft").GetFirstChild();
                    if( !BottomLeftFillNode.Empty() )
                        this->FillColours[UI::QC_BottomLeft].ProtoDeSerialize(BottomLeftFillNode);

                    XML::Node BottomRightFillNode = FillColoursNode.GetChild("BottomRight").GetFirstChild();
                    if( !BottomRightFillNode.Empty() )
                        this->FillColours[UI::QC_BottomRight].ProtoDeSerialize(BottomRightFillNode);
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (SingleImageLayer::GetSerializableName() + "Properties") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,SingleImageLayer::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String SingleImageLayer::GetDerivedSerializableName() const
            { return SingleImageLayer::GetSerializableName(); }

        String SingleImageLayer::GetSerializableName()
            { return "SingleImageLayer"; }
    }//UI
}//Mezzanine

#endif
