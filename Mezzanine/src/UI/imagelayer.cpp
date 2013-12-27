//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _uiimagelayer_cpp
#define _uiimagelayer_cpp

#include "UI/imagelayer.h"
#include "UI/quadrenderable.h"
#include "UI/screen.h"

namespace Mezzanine
{
    namespace UI
    {
        ImageLayer::ImageLayer(QuadRenderable* ParentRenderable) :
            RenderLayer(ParentRenderable),
            LayerSprite(NULL),
            BorderWidth(0)
        {
            this->FillColours[0] = ColourValue::White();
            this->FillColours[1] = ColourValue::White();
            this->FillColours[2] = ColourValue::White();
            this->FillColours[3] = ColourValue::White();
            this->BorderColours[0] = ColourValue::Black();
            this->BorderColours[1] = ColourValue::Black();
            this->BorderColours[2] = ColourValue::Black();
            this->BorderColours[3] = ColourValue::Black();
        }

        ImageLayer::~ImageLayer()
            {  }

        void ImageLayer::RedrawImpl(bool Force)
        {
            // Get the Texel Offsets
            Real TexelOffsetX = this->Parent->GetScreen()->GetTexelOffsetX();
            Real TexelOffsetY = this->Parent->GetScreen()->GetTexelOffsetY();

            // Get the parent rect and apply the scaling
            Rect ActDims = this->GetAreaRect();

            // Apply the texel offsets
            Vector2 TopLeft, TopRight, BottomLeft, BottomRight;
            TopLeft.X = ActDims.Position.X + TexelOffsetX;                        TopLeft.Y = ActDims.Position.Y + TexelOffsetY;
            TopRight.X = (ActDims.Position.X + ActDims.Size.X) + TexelOffsetX;    TopRight.Y = ActDims.Position.Y + TexelOffsetY;
            BottomLeft.X = ActDims.Position.X + TexelOffsetX;                     BottomLeft.Y = (ActDims.Position.Y + ActDims.Size.Y) + TexelOffsetY;
            BottomRight.X = (ActDims.Position.X + ActDims.Size.X) + TexelOffsetX; BottomRight.Y = (ActDims.Position.Y + ActDims.Size.Y) + TexelOffsetY;

            // Border and rotation
            if(0.0 != this->BorderWidth)
            {
                Vector2 OuterTopLeft = TopLeft, OuterTopRight = TopRight, OuterBottomLeft = BottomLeft, OuterBottomRight = BottomRight;
                OuterTopLeft.X -= BorderWidth;     OuterTopLeft.Y -= BorderWidth;
                OuterTopRight.X += BorderWidth;    OuterTopRight.Y -= BorderWidth;
                OuterBottomLeft.X -= BorderWidth;  OuterBottomLeft.Y += BorderWidth;
                OuterBottomRight.X += BorderWidth; OuterBottomRight.Y += BorderWidth;

                this->RotationTransform(TopLeft,TopRight,BottomLeft,BottomRight);
                this->RotationTransform(OuterTopLeft,OuterTopRight,OuterBottomLeft,OuterBottomRight);
                this->DrawBorder(TopLeft,TopRight,BottomLeft,BottomRight,
                                 OuterTopLeft,OuterTopRight,OuterBottomLeft,OuterBottomRight);
            }else{
                this->RotationTransform(TopLeft,TopRight,BottomLeft,BottomRight);
            }
            // Fill
            this->DrawFill(TopLeft,TopRight,BottomLeft,BottomRight);
        }

        void ImageLayer::DrawBorder(const Vector2& TopLeft, const Vector2& TopRight, const Vector2& BottomLeft, const Vector2& BottomRight,
                                    const Vector2& OuterTopLeft, const Vector2& OuterTopRight, const Vector2& OuterBottomLeft, const Vector2& OuterBottomRight)
        {
            Vector2 WP = this->Parent->GetScreen()->GetWhitePixel(PriAtlas);

            // North
            if( 0 != this->BorderColours[UI::Border_North].AlphaChannel )
            {
                this->PushTriangle(TopLeft, OuterTopRight, OuterTopLeft, WP, this->BorderColours[UI::Border_North], this->PriAtlas);
                this->PushTriangle(TopLeft, TopRight, OuterTopRight, WP, this->BorderColours[UI::Border_North], this->PriAtlas);
            }

            // East
            if( 0 != this->BorderColours[UI::Border_East].AlphaChannel )
            {
                this->PushTriangle(BottomRight, OuterTopRight, TopRight, WP, this->BorderColours[UI::Border_East], this->PriAtlas);
                this->PushTriangle(BottomRight, OuterBottomRight, OuterTopRight, WP, this->BorderColours[UI::Border_East], this->PriAtlas);
            }

            // South
            if( 0 != this->BorderColours[UI::Border_South].AlphaChannel )
            {
                this->PushTriangle(OuterBottomLeft, BottomRight, BottomLeft, WP, this->BorderColours[UI::Border_South], this->PriAtlas);
                this->PushTriangle(OuterBottomLeft, OuterBottomRight, BottomRight, WP, this->BorderColours[UI::Border_South], this->PriAtlas);
            }

            // West
            if( 0 != this->BorderColours[UI::Border_West].AlphaChannel )
            {
                this->PushTriangle(OuterBottomLeft, TopLeft, OuterTopLeft, WP, BorderColours[UI::Border_West], this->PriAtlas);
                this->PushTriangle(OuterBottomLeft, BottomLeft, TopLeft, WP, BorderColours[UI::Border_West], this->PriAtlas);
            }
        }

        void ImageLayer::DrawFill(const Vector2& TopLeft, const Vector2& TopRight, const Vector2& BottomLeft, const Vector2& BottomRight)
        {
            if(!this->IsCompletelyTransparent())
            {
                Vector2 UVs[4];
                if( this->LayerSprite == NULL ) {
                    UVs[UI::QC_TopLeft] = UVs[UI::QC_TopRight] = UVs[UI::QC_BottomRight] = UVs[UI::QC_BottomLeft] = this->Parent->GetScreen()->GetWhitePixel(this->PriAtlas);
                }else{
                    UVs[UI::QC_TopLeft] = this->LayerSprite->GetRelativeAtlasCoords(UI::QC_TopLeft);
                    UVs[UI::QC_TopRight] = this->LayerSprite->GetRelativeAtlasCoords(UI::QC_TopRight);
                    UVs[UI::QC_BottomRight] = this->LayerSprite->GetRelativeAtlasCoords(UI::QC_BottomRight);
                    UVs[UI::QC_BottomLeft] = this->LayerSprite->GetRelativeAtlasCoords(UI::QC_BottomLeft);
                }

                // Triangle A
                this->PushVertex(BottomLeft.X, BottomLeft.Y, UVs[UI::QC_BottomLeft], this->FillColours[UI::QC_BottomLeft], this->PriAtlas);      // Left/Bottom  3
                this->PushVertex(TopRight.X, TopRight.Y, UVs[UI::QC_TopRight], this->FillColours[UI::QC_TopRight], this->PriAtlas);              // Right/Top    1
                this->PushVertex(TopLeft.X, TopLeft.Y, UVs[UI::QC_TopLeft], this->FillColours[UI::QC_TopLeft], this->PriAtlas);                  // Left/Top     0

                // Triangle B
                this->PushVertex(BottomLeft.X, BottomLeft.Y, UVs[UI::QC_BottomLeft], this->FillColours[UI::QC_BottomLeft], this->PriAtlas);      // Left/Bottom   3
                this->PushVertex(BottomRight.X, BottomRight.Y, UVs[UI::QC_BottomRight], this->FillColours[UI::QC_BottomRight], this->PriAtlas);  // Right/Bottom  2
                this->PushVertex(TopRight.X, TopRight.Y, UVs[UI::QC_TopRight], this->FillColours[UI::QC_TopRight], this->PriAtlas);              // Right/Top     1
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        RenderLayer::RenderLayerType ImageLayer::GetLayerType() const
        {
            return RenderLayer::RLT_Image;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Fill Methods

        void ImageLayer::SetColour(const ColourValue& Colour)
        {
            this->FillColours[0] = Colour;
            this->FillColours[1] = Colour;
            this->FillColours[2] = Colour;
            this->FillColours[3] = Colour;
            this->_MarkDirty();
        }

        void ImageLayer::SetColour(const UI::QuadCorner Corner, const ColourValue& Colour)
        {
            this->FillColours[Corner] = Colour;
            this->_MarkDirty();
        }

        void ImageLayer::SetSprite(Sprite* PSprite)
        {
            /*if(PSprite == NULL)
            {
                this->PriAtlas = this->Parent->GetScreen()->GetPrimaryAtlas();
                UVs[0] = UVs[1] = UVs[2] = UVs[3] = Parent->GetScreen()->GetWhitePixel(PriAtlas);
                this->_MarkDirty();
            }else{
                this->PriAtlas = PSprite->GetAtlasName();
                Real TexelOffsetX = Parent->GetScreen()->GetTexelOffsetX();
                Real TexelOffsetY = Parent->GetScreen()->GetTexelOffsetY();
                if(TexelOffsetX) TexelOffsetX /= PSprite->Atlas->GetTextureSize();
                if(TexelOffsetY) TexelOffsetY /= PSprite->Atlas->GetTextureSize();
                UVs[0].X = UVs[3].X = PSprite->GetUVLeft();// - TexelOffsetX;
                UVs[0].Y = UVs[1].Y = PSprite->GetUVTop();// + TexelOffsetY;
                UVs[1].X = UVs[2].X = PSprite->GetUVRight();// - TexelOffsetX;
                UVs[2].Y = UVs[3].Y = PSprite->GetUVBottom();// + TexelOffsetY;
                this->_MarkDirty();
            }//*/
            this->PriAtlas = ( PSprite == NULL ? this->Parent->GetScreen()->GetPrimaryAtlas() : PSprite->GetAtlasName() );
            this->LayerSprite = PSprite;
            this->_MarkDirty();
        }

        void ImageLayer::SetSprite(const String& SpriteName)
        {
            Sprite* PSprite = this->Parent->GetScreen()->GetSprite(SpriteName,PriAtlas);
            this->SetSprite(PSprite);
        }

        void ImageLayer::SetSprite(const String& SpriteName, const String& Atlas)
        {
            Sprite* PSprite = this->Parent->GetScreen()->GetSprite(SpriteName,Atlas);
            this->SetSprite(PSprite);
        }

        void ImageLayer::SetGradient(const UI::Gradient Grad, const ColourValue& ColourA, const ColourValue& ColourB)
        {
            switch(Grad)
            {
                case UI::Gradient_NorthSouth:
                {
                    this->FillColours[0] = ColourA;
                    this->FillColours[1] = ColourA;
                    this->FillColours[2] = ColourB;
                    this->FillColours[3] = ColourB;
                }
                case UI::Gradient_WestEast:
                {
                    this->FillColours[0] = ColourA;
                    this->FillColours[3] = ColourA;
                    this->FillColours[1] = ColourB;
                    this->FillColours[2] = ColourB;
                }
                case UI::Gradient_Diagonal_1:
                {
                    ColourValue Average;
                    Average.RedChannel = (ColourA.RedChannel + ColourB.RedChannel) * 0.5f;
                    Average.GreenChannel = (ColourA.GreenChannel + ColourB.GreenChannel) * 0.5f;
                    Average.BlueChannel = (ColourA.BlueChannel + ColourB.BlueChannel) * 0.5f;
                    Average.AlphaChannel = (ColourA.AlphaChannel + ColourB.AlphaChannel) * 0.5f;
                    this->FillColours[0] = ColourA;
                    this->FillColours[1] = Average;
                    this->FillColours[2] = ColourB;
                    this->FillColours[3] = Average;
                }
                case UI::Gradient_Diagonal_2:
                {
                    ColourValue Average;
                    Average.RedChannel = (ColourA.RedChannel + ColourB.RedChannel) * 0.5f;
                    Average.GreenChannel = (ColourA.GreenChannel + ColourB.GreenChannel) * 0.5f;
                    Average.BlueChannel = (ColourA.BlueChannel + ColourB.BlueChannel) * 0.5f;
                    Average.AlphaChannel = (ColourA.AlphaChannel + ColourB.AlphaChannel) * 0.5f;
                    this->FillColours[0] = Average;
                    this->FillColours[1] = ColourA;
                    this->FillColours[2] = Average;
                    this->FillColours[3] = ColourB;
                }
            }
            this->_MarkDirty();
        }

        const ColourValue& ImageLayer::GetColour(const UI::QuadCorner Corner) const
        {
            return this->FillColours[Corner];
        }

        bool ImageLayer::IsCompletelyTransparent() const
        {
            return ( 0 == this->FillColours[UI::QC_TopLeft].AlphaChannel &&
                     0 == this->FillColours[UI::QC_TopRight].AlphaChannel &&
                     0 == this->FillColours[UI::QC_BottomLeft].AlphaChannel &&
                     0 == this->FillColours[UI::QC_BottomRight].AlphaChannel );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Border Methods

        void ImageLayer::SetBorderWidth(const Real Width)
        {
            this->BorderWidth = Width;
            this->_MarkDirty();
        }

        void ImageLayer::SetBorderColour(const ColourValue& Colour)
        {
            this->BorderColours[0] = Colour;
            this->BorderColours[1] = Colour;
            this->BorderColours[2] = Colour;
            this->BorderColours[3] = Colour;
            this->_MarkDirty();
        }

        void ImageLayer::SetBorderColour(const UI::Border Side, const ColourValue& Colour)
        {
            this->BorderColours[Side] = Colour;
            this->_MarkDirty();
        }

        void ImageLayer::SetBorder(const Real Width, const ColourValue& Colour)
        {
            this->SetBorderWidth(Width);
            this->SetBorderColour(Colour);
        }

        void ImageLayer::SetBorder(const Real Width, const ColourValue& North, const ColourValue& South, const ColourValue& East, const ColourValue& West)
        {
            this->SetBorderWidth(Width);
            this->BorderColours[UI::Border_North] = North;
            this->BorderColours[UI::Border_South] = South;
            this->BorderColours[UI::Border_East] = East;
            this->BorderColours[UI::Border_West] = West;
        }

        void ImageLayer::NoBorder()
        {
            this->SetBorderWidth(0.0);
            this->SetBorderColour(ColourValue::Black());
        }

        Real ImageLayer::GetBorderWidth() const
        {
            return this->BorderWidth;
        }

        const ColourValue& ImageLayer::GetBorderColour(const UI::Border Side) const
        {
            return this->BorderColours[Side];
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void ImageLayer::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->RenderLayer::ProtoSerializeProperties(SelfRoot);
            XML::Node PropertiesNode = SelfRoot.AppendChild( ImageLayer::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                PropertiesNode.AppendAttribute("BorderWidth").SetValue(this->BorderWidth) &&
                PropertiesNode.AppendAttribute("SpriteName").SetValue( this->LayerSprite ? this->LayerSprite->Name : "" ) )
            {
                XML::Node ScaleNode = PropertiesNode.AppendChild("Scale");
                this->Scale.ProtoSerialize(ScaleNode);

                XML::Node FillColoursNode = PropertiesNode.AppendChild("FillColours");

                XML::Node TopLeftFillNode = FillColoursNode.AppendChild("TopLeft");
                this->FillColours[UI::QC_TopLeft].ProtoSerialize(TopLeftFillNode);

                XML::Node TopRightFillNode = FillColoursNode.AppendChild("TopRight");
                this->FillColours[UI::QC_TopRight].ProtoSerialize(TopRightFillNode);

                XML::Node BottomLeftFillNode = FillColoursNode.AppendChild("BottomLeft");
                this->FillColours[UI::QC_BottomLeft].ProtoSerialize(BottomLeftFillNode);

                XML::Node BottomRightFillNode = FillColoursNode.AppendChild("BottomRight");
                this->FillColours[UI::QC_BottomRight].ProtoSerialize(BottomRightFillNode);

                XML::Node BorderColoursNode = PropertiesNode.AppendChild("BorderColours");

                XML::Node TopLeftBorderNode = BorderColoursNode.AppendChild("TopLeft");
                this->BorderColours[UI::QC_TopLeft].ProtoSerialize(TopLeftBorderNode);

                XML::Node TopRightBorderNode = FillColoursNode.AppendChild("TopRight");
                this->BorderColours[UI::QC_TopRight].ProtoSerialize(TopRightBorderNode);

                XML::Node BottomLeftBorderNode = FillColoursNode.AppendChild("BottomLeft");
                this->BorderColours[UI::QC_BottomLeft].ProtoSerialize(BottomLeftBorderNode);

                XML::Node BottomRightBorderNode = FillColoursNode.AppendChild("BottomRight");
                this->BorderColours[UI::QC_BottomRight].ProtoSerialize(BottomRightBorderNode);

                return;
            }else{
                SerializeError("Create XML Attribute Values",ImageLayer::GetSerializableName() + "Properties",true);
            }
        }

        void ImageLayer::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->RenderLayer::ProtoDeSerializeProperties(SelfRoot);
            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( ImageLayer::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = PropertiesNode.GetAttribute("BorderWidth");
                    if( !CurrAttrib.Empty() )
                        this->BorderWidth = CurrAttrib.AsReal();

                    CurrAttrib = PropertiesNode.GetAttribute("SpriteName");
                    if( !CurrAttrib.Empty() )
                        this->SetSprite( CurrAttrib.AsString() );

                    XML::Node ScaleNode = PropertiesNode.GetChild("Scale").GetFirstChild();
                    if( !ScaleNode.Empty() )
                        this->Scale.ProtoDeSerialize(ScaleNode);

                    XML::Node FillColoursNode = PropertiesNode.GetChild("FillColours");
                    XML::Node TopLeftFillNode = PropertiesNode.GetChild("TopLeft").GetFirstChild();
                    if( !TopLeftFillNode.Empty() )
                        this->FillColours[UI::QC_TopLeft].ProtoDeSerialize(TopLeftFillNode);

                    XML::Node TopRightFillNode = PropertiesNode.GetChild("TopRight").GetFirstChild();
                    if( !TopRightFillNode.Empty() )
                        this->FillColours[UI::QC_TopRight].ProtoDeSerialize(TopRightFillNode);

                    XML::Node BottomLeftFillNode = PropertiesNode.GetChild("BottomLeft").GetFirstChild();
                    if( !BottomLeftFillNode.Empty() )
                        this->FillColours[UI::QC_BottomLeft].ProtoDeSerialize(BottomLeftFillNode);

                    XML::Node BottomRightFillNode = PropertiesNode.GetChild("BottomRight").GetFirstChild();
                    if( !BottomRightFillNode.Empty() )
                        this->FillColours[UI::QC_BottomRight].ProtoDeSerialize(BottomRightFillNode);

                    XML::Node BorderColoursNode = PropertiesNode.GetChild("BorderColours");
                    XML::Node TopLeftBorderNode = PropertiesNode.GetChild("TopLeft").GetFirstChild();
                    if( !TopLeftBorderNode.Empty() )
                        this->BorderColours[UI::QC_TopLeft].ProtoDeSerialize(TopLeftBorderNode);

                    XML::Node TopRightBorderNode = PropertiesNode.GetChild("TopRight").GetFirstChild();
                    if( !TopRightBorderNode.Empty() )
                        this->BorderColours[UI::QC_TopRight].ProtoDeSerialize(TopRightBorderNode);

                    XML::Node BottomLeftBorderNode = PropertiesNode.GetChild("BottomLeft").GetFirstChild();
                    if( !BottomLeftBorderNode.Empty() )
                        this->BorderColours[UI::QC_BottomLeft].ProtoDeSerialize(BottomLeftBorderNode);

                    XML::Node BottomRightBorderNode = PropertiesNode.GetChild("BottomRight").GetFirstChild();
                    if( !BottomRightBorderNode.Empty() )
                        this->BorderColours[UI::QC_BottomRight].ProtoDeSerialize(BottomRightBorderNode);
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (ImageLayer::GetSerializableName() + "Properties") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,ImageLayer::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String ImageLayer::GetDerivedSerializableName() const
        {
            return ImageLayer::GetSerializableName();
        }

        String ImageLayer::GetSerializableName()
        {
            return "ImageLayer";
        }
    }//UI
}//Mezzanine

#endif
