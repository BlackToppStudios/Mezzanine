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

#ifndef _uimultiimagelayer_cpp
#define _uimultiimagelayer_cpp

#include "UI/multiimagelayer.h"
#include "UI/quadrenderable.h"
#include "UI/screen.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // MultiImageData Methods

        MultiImageData::MultiImageData() :
            LayerSprite(NULL)
        {
            this->FillColours[UI::QC_TopLeft] = ColourValue::White();
            this->FillColours[UI::QC_TopRight] = ColourValue::White();
            this->FillColours[UI::QC_BottomLeft] = ColourValue::White();
            this->FillColours[UI::QC_BottomRight] = ColourValue::White();
        }

        MultiImageData::~MultiImageData()
            {  }

        /*///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void MultiImageData::ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfRoot = ParentNode.AppendChild( MultiImageData::GetSerializableName() );

            if( SelfRoot.AppendAttribute("Version").SetValue("1") &&
                SelfRoot.AppendAttribute("SpriteName").SetValue( this->LayerSprite ? this->LayerSprite->Name : "" ) )
            {
                XML::Node FillRectNode = SelfRoot.AppendChild("FillRect");
                this->FillRect.ProtoSerialize( FillRectNode );

                XML::Node FillColoursNode = SelfRoot.AppendChild("FillColours");
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
                SerializeError("Create XML Attribute Values",MultiImageData::GetSerializableName(),true);
            }
        }

        void MultiImageData::ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            XML::Attribute CurrAttrib;
            if( SelfRoot.Name() == MultiImageData::GetSerializableName() ) {
                if(SelfRoot.GetAttribute("Version").AsInt() == 1) {
                    CurrAttrib = SelfRoot.GetAttribute("SpriteName");
                    if( !CurrAttrib.Empty() )
                        this->SetSprite( CurrAttrib.AsString() );

                    XML::Node FillRectNode = SelfRoot.GetChild("FillRect").GetFirstChild();
                    if( !FillRectNode.Empty() )
                        this->FillRect.ProtoDeSerialize(FillRectNode);

                    XML::Node FillColoursNode = SelfRoot.GetChild("FillColours");
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
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + MultiImageData::GetSerializableName() + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,MultiImageData::GetSerializableName() + " was not found in the provided XML node, which was expected.");
            }
        }

        String MultiImageData::GetSerializableName()
            { return "MultiImageData"; }//*/

        ///////////////////////////////////////////////////////////////////////////////
        // MultiImageLayer Methods

        MultiImageLayer::MultiImageLayer(QuadRenderable* ParentRenderable) :
            ImageLayer(ParentRenderable)
            {  }

        MultiImageLayer::~MultiImageLayer()
            {  }

        void MultiImageLayer::RedrawImpl(Boole Force)
        {
            // Get the Texel Offsets
            Real TexelOffsetX = this->Parent->GetScreen()->GetTexelOffsetX();
            Real TexelOffsetY = this->Parent->GetScreen()->GetTexelOffsetY();

            // Get the parent rect and apply the scaling
            Rect ActDims = this->GetAreaRect();
            Vector2 RectCenter = ActDims.GetRectCenter();

            for( ImageDataIterator ImageIt = this->LayerImages.begin() ; ImageIt != this->LayerImages.end() ; ++ImageIt )
            {
                Rect ImageDims = (*ImageIt).FillRect.CalculateActualDimensions( ActDims );
                // Apply the texel offsets
                Vector2 RectCorners[4];
                RectCorners[UI::QC_TopLeft].X = ImageDims.Position.X + TexelOffsetX;                           RectCorners[UI::QC_TopLeft].Y = ImageDims.Position.Y + TexelOffsetY;
                RectCorners[UI::QC_TopRight].X = (ImageDims.Position.X + ImageDims.Size.X) + TexelOffsetX;     RectCorners[UI::QC_TopRight].Y = ImageDims.Position.Y + TexelOffsetY;
                RectCorners[UI::QC_BottomLeft].X = ImageDims.Position.X + TexelOffsetX;                        RectCorners[UI::QC_BottomLeft].Y = (ImageDims.Position.Y + ImageDims.Size.Y) + TexelOffsetY;
                RectCorners[UI::QC_BottomRight].X = (ImageDims.Position.X + ImageDims.Size.X) + TexelOffsetX;  RectCorners[UI::QC_BottomRight].Y = (ImageDims.Position.Y + ImageDims.Size.Y) + TexelOffsetY;

                // Border and rotation
                if( 0.0 != this->BorderWidth ) {
                    Vector2 OuterBorder[4];
                    OuterBorder[UI::QC_TopLeft].X = RectCorners[UI::QC_TopLeft].X - BorderWidth;               OuterBorder[UI::QC_TopLeft].Y = RectCorners[UI::QC_TopLeft].Y - BorderWidth;
                    OuterBorder[UI::QC_TopRight].X = RectCorners[UI::QC_TopRight].X + BorderWidth;             OuterBorder[UI::QC_TopRight].Y = RectCorners[UI::QC_TopRight].Y - BorderWidth;
                    OuterBorder[UI::QC_BottomLeft].X = RectCorners[UI::QC_BottomLeft].X - BorderWidth;         OuterBorder[UI::QC_BottomLeft].Y = RectCorners[UI::QC_BottomLeft].Y + BorderWidth;
                    OuterBorder[UI::QC_BottomRight].X = RectCorners[UI::QC_BottomRight].X + BorderWidth;       OuterBorder[UI::QC_BottomRight].Y = RectCorners[UI::QC_BottomRight].Y + BorderWidth;

                    this->RotationTransform(RectCorners,RectCenter);
                    this->RotationTransform(OuterBorder,RectCenter);
                    this->DrawBorder(RectCorners,OuterBorder);
                }else{
                    this->RotationTransform(RectCorners,RectCenter);
                }

                Boole TransparencyCheck = ( 0 == (*ImageIt).FillColours[UI::QC_TopLeft].AlphaChannel && 0 == (*ImageIt).FillColours[UI::QC_TopRight].AlphaChannel &&
                                            0 == (*ImageIt).FillColours[UI::QC_BottomLeft].AlphaChannel && 0 == (*ImageIt).FillColours[UI::QC_BottomRight].AlphaChannel );
                // Fill
                if( !TransparencyCheck ) {
                    this->DrawFill(RectCorners,(*ImageIt).LayerSprite,(*ImageIt).FillColours);
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        UI::RenderLayerType MultiImageLayer::GetLayerType() const
            { return UI::RLT_MultiImage; }

        ///////////////////////////////////////////////////////////////////////////////
        // MultiImageData Configuration

        void MultiImageLayer::ReserveMultiImageData(const Whole NumImages)
            { this->LayerImages.resize(NumImages); }

        MultiImageData& MultiImageLayer::GetMultiImageData(const Whole Index)
            { return this->LayerImages.at(Index); }

        const MultiImageData& MultiImageLayer::GetMultiImageData(const Whole Index) const
            { return this->LayerImages.at(Index); }

        Whole MultiImageLayer::GetNumMultiImageData() const
            { return this->LayerImages.size(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Fill Methods

        void MultiImageLayer::SetImageRect(const Whole Index, const UnifiedRect& Trans)
        {
            this->GetMultiImageData(Index).FillRect = Trans;
            this->_MarkDirty();
        }

        const UnifiedRect& MultiImageLayer::GetImageRect(const Whole Index) const
        {
            return this->GetMultiImageData(Index).FillRect;
        }

        void MultiImageLayer::SetImagePosition(const Whole Index, const UnifiedVec2& Position)
        {
            this->GetMultiImageData(Index).FillRect.Position = Position;
            this->_MarkDirty();
        }

        const UnifiedVec2& MultiImageLayer::GetImagePosition(const Whole Index) const
        {
            return this->GetMultiImageData(Index).FillRect.Position;
        }

        void MultiImageLayer::SetImageSize(const Whole Index, const UnifiedVec2& Size)
        {
            this->GetMultiImageData(Index).FillRect.Size = Size;
            this->_MarkDirty();
        }

        const UnifiedVec2& MultiImageLayer::GetImageSize(const Whole Index) const
        {
            return this->GetMultiImageData(Index).FillRect.Size;
        }

        void MultiImageLayer::SetColour(const Whole Index, const ColourValue& Colour)
        {
            MultiImageData& Data = this->GetMultiImageData(Index);
            Data.FillColours[UI::QC_TopLeft] = Colour;
            Data.FillColours[UI::QC_TopRight] = Colour;
            Data.FillColours[UI::QC_BottomLeft] = Colour;
            Data.FillColours[UI::QC_BottomRight] = Colour;
            this->_MarkDirty();
        }

        void MultiImageLayer::SetColour(const Whole Index, const UI::QuadCorner Corner, const ColourValue& Colour)
        {
            this->GetMultiImageData(Index).FillColours[Corner] = Colour;
            this->_MarkDirty();
        }

        const ColourValue& MultiImageLayer::GetColour(const Whole Index, const UI::QuadCorner Corner) const
        {
            return this->GetMultiImageData(Index).FillColours[Corner];
        }

        Boole MultiImageLayer::IsCompletelyTransparent(const Whole Index) const
        {
            const MultiImageData& Data = this->GetMultiImageData(Index);
            return ( 0 == Data.FillColours[UI::QC_TopLeft].AlphaChannel &&
                     0 == Data.FillColours[UI::QC_TopRight].AlphaChannel &&
                     0 == Data.FillColours[UI::QC_BottomLeft].AlphaChannel &&
                     0 == Data.FillColours[UI::QC_BottomRight].AlphaChannel );
        }

        void MultiImageLayer::SetSprite(const Whole Index, Sprite* NewSprite)
        {
            MultiImageData& Data = this->GetMultiImageData(Index);
            if( Data.LayerSprite != NewSprite ) {
                Data.LayerSprite = NewSprite;
                this->_MarkDirty();
            }
        }

        void MultiImageLayer::SetSprite(const Whole Index, const String& SpriteName)
        {
            Sprite* NewSprite = this->Parent->GetScreen()->GetSprite(SpriteName,PriAtlas);
            this->SetSprite(Index,NewSprite);
        }

        void MultiImageLayer::SetSprite(const Whole Index, const String& SpriteName, const String& Atlas)
        {
            Sprite* NewSprite = this->Parent->GetScreen()->GetSprite(SpriteName,Atlas);
            this->SetSprite(Index,NewSprite);
        }

        void MultiImageLayer::SetGradient(const Whole Index, const UI::Gradient Grad, const ColourValue& ColourA, const ColourValue& ColourB)
        {
            MultiImageData& Data = this->GetMultiImageData(Index);
            switch(Grad)
            {
                case UI::Gradient_NorthSouth:
                {
                    Data.FillColours[UI::QC_TopLeft] = ColourA;
                    Data.FillColours[UI::QC_TopRight] = ColourA;
                    Data.FillColours[UI::QC_BottomRight] = ColourB;
                    Data.FillColours[UI::QC_BottomLeft] = ColourB;
                    break;
                }
                case UI::Gradient_WestEast:
                {
                    Data.FillColours[UI::QC_TopLeft] = ColourA;
                    Data.FillColours[UI::QC_BottomLeft] = ColourA;
                    Data.FillColours[UI::QC_TopRight] = ColourB;
                    Data.FillColours[UI::QC_BottomRight] = ColourB;
                    break;
                }
                case UI::Gradient_Diagonal_1:
                {
                    ColourValue Average;
                    Average.RedChannel = (ColourA.RedChannel + ColourB.RedChannel) * 0.5f;
                    Average.GreenChannel = (ColourA.GreenChannel + ColourB.GreenChannel) * 0.5f;
                    Average.BlueChannel = (ColourA.BlueChannel + ColourB.BlueChannel) * 0.5f;
                    Average.AlphaChannel = (ColourA.AlphaChannel + ColourB.AlphaChannel) * 0.5f;
                    Data.FillColours[UI::QC_TopLeft] = ColourA;
                    Data.FillColours[UI::QC_TopRight] = Average;
                    Data.FillColours[UI::QC_BottomRight] = ColourB;
                    Data.FillColours[UI::QC_BottomLeft] = Average;
                    break;
                }
                case UI::Gradient_Diagonal_2:
                {
                    ColourValue Average;
                    Average.RedChannel = (ColourA.RedChannel + ColourB.RedChannel) * 0.5f;
                    Average.GreenChannel = (ColourA.GreenChannel + ColourB.GreenChannel) * 0.5f;
                    Average.BlueChannel = (ColourA.BlueChannel + ColourB.BlueChannel) * 0.5f;
                    Average.AlphaChannel = (ColourA.AlphaChannel + ColourB.AlphaChannel) * 0.5f;
                    Data.FillColours[UI::QC_TopLeft] = Average;
                    Data.FillColours[UI::QC_TopRight] = ColourA;
                    Data.FillColours[UI::QC_BottomRight] = Average;
                    Data.FillColours[UI::QC_BottomLeft] = ColourB;
                    break;
                }
            }
            this->_MarkDirty();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void MultiImageLayer::ProtoSerializeProperties(XML::Node& SelfRoot) const
        {
            this->ImageLayer::ProtoSerializeProperties(SelfRoot);
            XML::Node PropertiesNode = SelfRoot.AppendChild( MultiImageLayer::GetSerializableName() + "Properties" );

            if( PropertiesNode.AppendAttribute("Version").SetValue("1") )
            {
                XML::Node LayerImagesNode = PropertiesNode.AppendChild( "LayerImages" );
                LayerImagesNode.AppendAttribute("NumImages").SetValue( this->LayerImages.size() );

                for( ConstImageDataIterator ImageIt = this->LayerImages.begin() ; ImageIt != this->LayerImages.end() ; ++ImageIt )
                {
                    XML::Node ImageDataRoot = LayerImagesNode.AppendChild( "MultiImageData" );

                    if( ImageDataRoot.AppendAttribute("Version").SetValue("1") &&
                        ImageDataRoot.AppendAttribute("SpriteName").SetValue( (*ImageIt).LayerSprite ? (*ImageIt).LayerSprite->Name : "" ) &&
                        ImageDataRoot.AppendAttribute("SpriteAtlas").SetValue( (*ImageIt).LayerSprite ? (*ImageIt).LayerSprite->GetAtlasName() : "" ) )
                    {
                        XML::Node FillRectNode = ImageDataRoot.AppendChild("FillRect");
                        (*ImageIt).FillRect.ProtoSerialize( FillRectNode );

                        XML::Node FillColoursNode = ImageDataRoot.AppendChild("FillColours");
                        XML::Node TopLeftFillNode = FillColoursNode.AppendChild("TopLeft");
                        (*ImageIt).FillColours[UI::QC_TopLeft].ProtoSerialize(TopLeftFillNode);

                        XML::Node TopRightFillNode = FillColoursNode.AppendChild("TopRight");
                        (*ImageIt).FillColours[UI::QC_TopRight].ProtoSerialize(TopRightFillNode);

                        XML::Node BottomLeftFillNode = FillColoursNode.AppendChild("BottomLeft");
                        (*ImageIt).FillColours[UI::QC_BottomLeft].ProtoSerialize(BottomLeftFillNode);

                        XML::Node BottomRightFillNode = FillColoursNode.AppendChild("BottomRight");
                        (*ImageIt).FillColours[UI::QC_BottomRight].ProtoSerialize(BottomRightFillNode);

                        return;
                    }else{
                        SerializeError("Create XML Attribute Values","MultiImageData",true);
                    }
                }

                return;
            }else{
                SerializeError("Create XML Attribute Values",MultiImageLayer::GetSerializableName() + "Properties",true);
            }
        }

        void MultiImageLayer::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
        {
            this->ImageLayer::ProtoDeSerializeProperties(SelfRoot);
            XML::Attribute CurrAttrib;
            XML::Node PropertiesNode = SelfRoot.GetChild( MultiImageLayer::GetSerializableName() + "Properties" );

            if( !PropertiesNode.Empty() ) {
                if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                    XML::Node LayerImagesNode = PropertiesNode.GetChild( MultiImageLayer::GetSerializableName() + "Properties" );

                    CurrAttrib = LayerImagesNode.GetAttribute("NumImages");
                    if( !CurrAttrib.Empty() )
                        this->ReserveMultiImageData( CurrAttrib.AsWhole() );

                    /// @todo This loop expects the listed order in the XML to match the order in which they were serialized.  Within PugiXML at least this
                    /// shouldn't be a problem, however if Images start appearing out of order it may be worthwhile to investigate or add some ordering redundancy.
                    ImageDataIterator ImageIt = this->LayerImages.begin();
                    for( XML::NodeIterator ImageNodeIt = LayerImagesNode.begin() ; ImageNodeIt != LayerImagesNode.end() ; ++ImageNodeIt )
                    {
                        String SpriteName, SpriteAtlas;

                        if( ImageIt == this->LayerImages.end() ) {
                            MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION,"Seriailzed value for \"NumImages\" is smaller than number of nodes to deserialize.  Verify serialized XML.");
                        }

                        CurrAttrib = (*ImageNodeIt).GetAttribute("SpriteName");
                        if( !CurrAttrib.Empty() )
                            SpriteName = CurrAttrib.AsString();

                        CurrAttrib = (*ImageNodeIt).GetAttribute("SpriteAtlas");
                        if( !CurrAttrib.Empty() )
                            SpriteAtlas = CurrAttrib.AsString();

                        (*ImageIt).LayerSprite = this->Parent->GetScreen()->GetSprite(SpriteName,SpriteAtlas);

                        XML::Node FillRectNode = (*ImageNodeIt).GetChild("FillRect").GetFirstChild();
                        if( !FillRectNode.Empty() )
                            (*ImageIt).FillRect.ProtoDeSerialize(FillRectNode);

                        XML::Node FillColoursNode = (*ImageNodeIt).GetChild("FillColours");
                        XML::Node TopLeftFillNode = FillColoursNode.GetChild("TopLeft").GetFirstChild();
                        if( !TopLeftFillNode.Empty() )
                            (*ImageIt).FillColours[UI::QC_TopLeft].ProtoDeSerialize(TopLeftFillNode);

                        XML::Node TopRightFillNode = FillColoursNode.GetChild("TopRight").GetFirstChild();
                        if( !TopRightFillNode.Empty() )
                            (*ImageIt).FillColours[UI::QC_TopRight].ProtoDeSerialize(TopRightFillNode);

                        XML::Node BottomLeftFillNode = FillColoursNode.GetChild("BottomLeft").GetFirstChild();
                        if( !BottomLeftFillNode.Empty() )
                            (*ImageIt).FillColours[UI::QC_BottomLeft].ProtoDeSerialize(BottomLeftFillNode);

                        XML::Node BottomRightFillNode = FillColoursNode.GetChild("BottomRight").GetFirstChild();
                        if( !BottomRightFillNode.Empty() )
                            (*ImageIt).FillColours[UI::QC_BottomRight].ProtoDeSerialize(BottomRightFillNode);

                        ++ImageIt;
                    }
                }else{
                    MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (String("MultiImageData") + "Properties") + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,String("MultiImageData") + "Properties" + " was not found in the provided XML node, which was expected.");
            }
        }

        String MultiImageLayer::GetDerivedSerializableName() const
            { return MultiImageLayer::GetSerializableName(); }

        String MultiImageLayer::GetSerializableName()
            { return "MultiImageLayer"; }
    }//UI
}//Mezzanine

#endif
