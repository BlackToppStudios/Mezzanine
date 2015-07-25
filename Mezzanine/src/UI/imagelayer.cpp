// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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
            BorderWidth(0)
        {
            this->BorderColours[0] = ColourValue::Black();
            this->BorderColours[1] = ColourValue::Black();
            this->BorderColours[2] = ColourValue::Black();
            this->BorderColours[3] = ColourValue::Black();
        }

        ImageLayer::~ImageLayer()
            {  }

        void ImageLayer::DrawBorder(const Vector2* InnerRect, const Vector2* OuterRect)
        {
            Vector2 WP = this->Parent->GetScreen()->GetWhitePixel(PriAtlas);

            // North
            if( 0 != this->BorderColours[UI::Border_North].AlphaChannel ) {
                this->PushTriangle( InnerRect[UI::QC_TopLeft], OuterRect[UI::QC_TopRight], OuterRect[UI::QC_TopLeft], WP, this->BorderColours[UI::Border_North], this->PriAtlas );
                this->PushTriangle( InnerRect[UI::QC_TopLeft], InnerRect[UI::QC_TopRight], OuterRect[UI::QC_TopRight], WP, this->BorderColours[UI::Border_North], this->PriAtlas );
            }
            // East
            if( 0 != this->BorderColours[UI::Border_East].AlphaChannel ) {
                this->PushTriangle( InnerRect[UI::QC_BottomRight], OuterRect[UI::QC_TopRight], InnerRect[UI::QC_TopRight], WP, this->BorderColours[UI::Border_East], this->PriAtlas );
                this->PushTriangle( InnerRect[UI::QC_BottomRight], OuterRect[UI::QC_BottomRight], OuterRect[UI::QC_TopRight], WP, this->BorderColours[UI::Border_East], this->PriAtlas );
            }
            // South
            if( 0 != this->BorderColours[UI::Border_South].AlphaChannel ) {
                this->PushTriangle( OuterRect[UI::QC_BottomLeft], InnerRect[UI::QC_BottomRight], InnerRect[UI::QC_BottomLeft], WP, this->BorderColours[UI::Border_South], this->PriAtlas );
                this->PushTriangle( OuterRect[UI::QC_BottomLeft], OuterRect[UI::QC_BottomRight], InnerRect[UI::QC_BottomRight], WP, this->BorderColours[UI::Border_South], this->PriAtlas );
            }
            // West
            if( 0 != this->BorderColours[UI::Border_West].AlphaChannel ) {
                this->PushTriangle( OuterRect[UI::QC_BottomLeft], InnerRect[UI::QC_TopLeft], OuterRect[UI::QC_TopLeft], WP, BorderColours[UI::Border_West], this->PriAtlas );
                this->PushTriangle( OuterRect[UI::QC_BottomLeft], InnerRect[UI::QC_BottomLeft], InnerRect[UI::QC_TopLeft], WP, BorderColours[UI::Border_West], this->PriAtlas );
            }
        }

        void ImageLayer::DrawFill(const Vector2* FillRect, const Sprite* FillSprite, const ColourValue* FillColours)
        {
            Vector2 UVs[4];
            if( FillSprite == NULL ) {
                UVs[UI::QC_TopLeft] = UVs[UI::QC_TopRight] = UVs[UI::QC_BottomRight] = UVs[UI::QC_BottomLeft] = this->Parent->GetScreen()->GetWhitePixel(this->PriAtlas);
            }else{
                UVs[UI::QC_TopLeft] = FillSprite->GetRelativeAtlasCoords(UI::QC_TopLeft);
                UVs[UI::QC_TopRight] = FillSprite->GetRelativeAtlasCoords(UI::QC_TopRight);
                UVs[UI::QC_BottomRight] = FillSprite->GetRelativeAtlasCoords(UI::QC_BottomRight);
                UVs[UI::QC_BottomLeft] = FillSprite->GetRelativeAtlasCoords(UI::QC_BottomLeft);
            }

            // Triangle A
            this->PushVertex( FillRect[UI::QC_BottomLeft].X, FillRect[UI::QC_BottomLeft].Y, UVs[UI::QC_BottomLeft], FillColours[UI::QC_BottomLeft], this->PriAtlas );      // Left/Bottom  3
            this->PushVertex( FillRect[UI::QC_TopRight].X, FillRect[UI::QC_TopRight].Y, UVs[UI::QC_TopRight], FillColours[UI::QC_TopRight], this->PriAtlas );              // Right/Top    1
            this->PushVertex( FillRect[UI::QC_TopLeft].X, FillRect[UI::QC_TopLeft].Y, UVs[UI::QC_TopLeft], FillColours[UI::QC_TopLeft], this->PriAtlas );                  // Left/Top     0

            // Triangle B
            this->PushVertex( FillRect[UI::QC_BottomLeft].X, FillRect[UI::QC_BottomLeft].Y, UVs[UI::QC_BottomLeft], FillColours[UI::QC_BottomLeft], this->PriAtlas );      // Left/Bottom   3
            this->PushVertex( FillRect[UI::QC_BottomRight].X, FillRect[UI::QC_BottomRight].Y, UVs[UI::QC_BottomRight], FillColours[UI::QC_BottomRight], this->PriAtlas );  // Right/Bottom  2
            this->PushVertex( FillRect[UI::QC_TopRight].X, FillRect[UI::QC_TopRight].Y, UVs[UI::QC_TopRight], FillColours[UI::QC_TopRight], this->PriAtlas );              // Right/Top     1
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
                PropertiesNode.AppendAttribute("BorderWidth").SetValue(this->BorderWidth) )
            {
                XML::Node BorderColoursNode = PropertiesNode.AppendChild("BorderColours");

                XML::Node TopLeftBorderNode = BorderColoursNode.AppendChild("TopLeft");
                this->BorderColours[UI::QC_TopLeft].ProtoSerialize(TopLeftBorderNode);

                XML::Node TopRightBorderNode = BorderColoursNode.AppendChild("TopRight");
                this->BorderColours[UI::QC_TopRight].ProtoSerialize(TopRightBorderNode);

                XML::Node BottomLeftBorderNode = BorderColoursNode.AppendChild("BottomLeft");
                this->BorderColours[UI::QC_BottomLeft].ProtoSerialize(BottomLeftBorderNode);

                XML::Node BottomRightBorderNode = BorderColoursNode.AppendChild("BottomRight");
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
            { return ImageLayer::GetSerializableName(); }

        String ImageLayer::GetSerializableName()
            { return "ImageLayer"; }
    }//UI
}//Mezzanine

#endif
