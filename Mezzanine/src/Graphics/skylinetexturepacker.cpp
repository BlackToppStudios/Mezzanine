// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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

#ifndef _graphicsskylinetexturepacker_cpp
#define _graphicsskylinetexturepacker_cpp

#include "Graphics/skylinetexturepacker.h"

namespace Mezzanine
{
    namespace Graphics
    {
        ///////////////////////////////////////////////////////////////////////////////
        // TextureSkylineNode Methods

        TextureSkylineNode::TextureSkylineNode(const Integer Width) :
            XPos(0),
            YPos(0),
            LineWidth(Width)
            {  }

        TextureSkylineNode::TextureSkylineNode(const Integer XPosition, const Integer YPosition, const Integer Width) :
            XPos(XPosition),
            YPos(YPosition),
            LineWidth(Width)
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Functionality

        Integer TextureSkylineNode::GetEndPoint() const
            { return this->XPos + this->LineWidth; }

        Boole TextureSkylineNode::PointIsWithin(const Integer Point) const
            { return ( Point >= this->XPos && Point <= this->GetEndPoint() ); }

        Boole TextureSkylineNode::Overlaps(const TextureSkylineNode& Other) const
            { return ( Other.PointIsWithin(this->XPos) || Other.PointIsWithin(this->GetEndPoint()) ); }

        ///////////////////////////////////////////////////////////////////////////////
        // ShelfTexturePacker Methods

        SkylineTexturePacker::SkylineTexturePacker(const Whole TexWidth, const Whole TexHeight)
            { this->Initialize(TexWidth,TexHeight); }

        void SkylineTexturePacker::AddSkylineLevel(const Whole SkylineIndex, const RectType& Placed)
        {
            /*TextureSkylineNode NewNode(Placed.Left,Placed.Bottom,Placed.GetWidth());

            SkylineIterator CurrIt = this->Skyline.begin() + SkylineIndex;
            const SkylineIterator EndIt = this->Skyline.end();
            while( CurrIt != EndIt )
            {

                ++CurrIt;
            }//*/
            TextureSkylineNode NewNode(Placed.Left,Placed.Bottom,Placed.GetWidth());
            this->Skyline.insert( this->Skyline.begin() + SkylineIndex, NewNode );

            for( Whole CurrIndex = SkylineIndex + 1 ; CurrIndex < this->Skyline.size() ; ++CurrIndex )
            {
                TextureSkylineNode& CurrNode = this->Skyline[CurrIndex];
                if( CurrNode.XPos < this->Skyline[CurrIndex - 1].GetEndPoint() ) {
                    Integer Shrink = this->Skyline[CurrIndex - 1].GetEndPoint() - CurrNode.XPos;

                    CurrNode.XPos += Shrink;
                    CurrNode.LineWidth -= Shrink;

                    if( CurrNode.LineWidth <= 0 ) {
                        this->Skyline.erase( this->Skyline.begin() + CurrIndex );
                        --CurrIndex;
                    }else{
                        break;
                    }
                }else{
                    break;
                }
            }
            this->MergeSkylines();//*/
        }

        Integer SkylineTexturePacker::RectangleFits(const Whole SkylineIndex, const Integer Width, const Integer Height) const
        {
            Integer XPos = this->Skyline[SkylineIndex].XPos;
            if( XPos + Width > static_cast<Integer>( this->AtlasImage.GetWidth() ) ) {
                return -1;
            }

            Integer RemainingWidth = Width;
            Integer YPos = this->Skyline[SkylineIndex].YPos;
            for( Whole CurrIndex = SkylineIndex ; RemainingWidth > 0 ; ++CurrIndex )
            {
                YPos = std::max(YPos,this->Skyline[CurrIndex].YPos);
                if( YPos + Height > static_cast<Integer>( this->AtlasImage.GetHeight() ) ) {
                    return -1;
                }
                RemainingWidth -= this->Skyline[CurrIndex].LineWidth;
            }
            return YPos;
        }

        Integer SkylineTexturePacker::ComputeWastedArea(const Whole SkylineIndex, const Integer YPos, const RectSizeType& RectSize) const
        {
            Integer WastedArea = 0;
            const Integer RectLeft = this->Skyline[SkylineIndex].XPos;
            const Integer RectRight = RectLeft + RectSize.Width;
            for( Whole CurrIndex = SkylineIndex ; CurrIndex < this->Skyline.size() ; ++CurrIndex )
            {
                const TextureSkylineNode& CurrNode = this->Skyline[CurrIndex];
                if( CurrNode.XPos >= RectRight || CurrNode.GetEndPoint() <= RectLeft ) {
                    break;
                }

                if( YPos < CurrNode.YPos ) {
                    return -1;
                }

                Integer LeftSide = CurrNode.XPos;
                Integer RightSide = std::min( RectRight, LeftSide + CurrNode.LineWidth );
                WastedArea += ( RightSide - LeftSide ) * ( YPos - CurrNode.YPos );
            }
            return WastedArea;
        }

        RectScoreResult SkylineTexturePacker::FindPositionForTopLeft(const RectSizeType& RectSize) const
        {
            RectScoreResult Ret;
            Integer BestWidth = std::numeric_limits<RectType::MemberType>::max();
            Integer BestHeight = std::numeric_limits<RectType::MemberType>::max();

            for( Whole CurrIndex = 0 ; CurrIndex < this->Skyline.size() ; ++CurrIndex )
            {
                Integer SkyPos = this->RectangleFits(CurrIndex,RectSize.Width,RectSize.Height);
                if( SkyPos >= 0 ) {
                    if( SkyPos + RectSize.Height < BestHeight ||
                        ( SkyPos + RectSize.Height == BestHeight && this->Skyline[CurrIndex].LineWidth < BestWidth ) )
                    {
                        Ret.NewRect.SetValuesBySize(this->Skyline[CurrIndex].XPos,SkyPos,RectSize);
                        Ret.Index = CurrIndex;
                        Ret.Rotated = false;
                        BestWidth = this->Skyline[CurrIndex].LineWidth;
                        BestHeight = SkyPos + RectSize.Height;
                    }
                }
                if( this->AllowRotated ) {
                    SkyPos = this->RectangleFits(CurrIndex,RectSize.Height,RectSize.Width);
                    if( SkyPos >= 0 ) {
                        if( SkyPos + RectSize.Width < BestHeight ||
                            ( SkyPos + RectSize.Width == BestHeight && this->Skyline[CurrIndex].LineWidth < BestWidth ) )
                        {
                            Ret.NewRect.SetValuesBySize(this->Skyline[CurrIndex].XPos,SkyPos,RectSize.GetRotated());
                            Ret.Index = CurrIndex;
                            Ret.Rotated = true;
                            BestWidth = this->Skyline[CurrIndex].LineWidth;
                            BestHeight = SkyPos + RectSize.Width;
                        }
                    }
                }
            }
            Ret.Score1 = BestHeight;
            Ret.Score2 = BestWidth;
            return Ret;
        }

        RectScoreResult SkylineTexturePacker::FindPositionForBestArea(const RectSizeType& RectSize) const
        {
            RectScoreResult Ret;
            Integer BestArea = std::numeric_limits<RectType::MemberType>::max();
            Integer BestHeight = std::numeric_limits<RectType::MemberType>::max();

            for( Whole CurrIndex = 0 ; CurrIndex < this->Skyline.size() ; ++CurrIndex )
            {
                Integer SkyPos = this->RectangleFits(CurrIndex,RectSize.Width,RectSize.Height);
                if( SkyPos >= 0 ) {
                    Integer WastedArea = this->ComputeWastedArea(CurrIndex,SkyPos,RectSize);
                    if( WastedArea < BestArea || ( WastedArea == BestArea && SkyPos + RectSize.Height < BestHeight ) ) {
                        Ret.NewRect.SetValuesBySize(this->Skyline[CurrIndex].XPos,SkyPos,RectSize);
                        Ret.Index = CurrIndex;
                        Ret.Rotated = false;
                        BestArea = WastedArea;
                        BestHeight = SkyPos + RectSize.Height;
                    }
                }
                if( this->AllowRotated ) {
                    SkyPos = this->RectangleFits(CurrIndex,RectSize.Height,RectSize.Width);
                    if( SkyPos >= 0 ) {
                        Integer WastedArea = this->ComputeWastedArea(CurrIndex,SkyPos,RectSize);
                        if( WastedArea < BestArea || ( WastedArea == BestArea && SkyPos + RectSize.Width < BestHeight ) ) {
                            Ret.NewRect.SetValuesBySize(this->Skyline[CurrIndex].XPos,SkyPos,RectSize.GetRotated());
                            Ret.Index = CurrIndex;
                            Ret.Rotated = true;
                            BestArea = WastedArea;
                            BestHeight = SkyPos + RectSize.Width;
                        }
                    }
                }
            }
            Ret.Score1 = BestArea;
            Ret.Score2 = BestHeight;
            return Ret;
        }

        void SkylineTexturePacker::MergeSkylines()
        {
            for( Whole SkyIndex = 0 ; SkyIndex < this->Skyline.size() - 1 ; ++SkyIndex )
            {
                if( this->Skyline[SkyIndex].YPos == this->Skyline[SkyIndex + 1].YPos ) {
                    this->Skyline[SkyIndex].LineWidth += this->Skyline[SkyIndex + 1].LineWidth;
                    this->Skyline.erase( this->Skyline.begin() + ( SkyIndex + 1 ) );
                    --SkyIndex;
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Initialization

        void SkylineTexturePacker::Initialize(const Whole TexWidth, const Whole TexHeight)
        {
            this->InitImage(TexWidth,TexHeight);

            this->Skyline.clear();
            this->UsedRectangles.clear();

            TextureSkylineNode FreshNode(this->AtlasImage.GetWidth());
            this->Skyline.push_back(FreshNode);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Insertion

        TexturePacker::PlacementResult SkylineTexturePacker::Insert(const Image* ToInsert)
            { return this->Insert(ToInsert,Graphics::SLP_BestAreaFit); }

        TexturePacker::ImageContainer SkylineTexturePacker::Insert(const ImageContainer& ToPack, PackResultHandler* Handler)
            { return this->Insert(ToPack,Handler,Graphics::SLP_BestAreaFit); }

        TexturePacker::PlacementResult SkylineTexturePacker::Insert(const Image* ToInsert, const SkylinePlacement RectChoice)
        {
            RectScoreResult RectScore;
            RectSizeType ImageSize = this->GetPaddedRectSize(ToInsert);

            switch( RectChoice )
            {
                case Graphics::SLP_TopLeft:      RectScore = this->FindPositionForTopLeft(ImageSize);   break;
                case Graphics::SLP_BestAreaFit:  RectScore = this->FindPositionForBestArea(ImageSize);  break;
                default:                         return PlacementResult(RectScore.NewRect,RectScore.Rotated);
            }

            if( RectScore.NewRect.GetArea() <= 0 ) {
                return PlacementResult(RectScore.NewRect,RectScore.Rotated);
            }

            if( RectScore.Rotated ) {
                this->AddRotatedToAtlas(ToInsert,RectScore.NewRect);
            }else{
                this->AddUnrotatedToAtlas(ToInsert,RectScore.NewRect);
            }
            this->AddSkylineLevel(RectScore.Index,RectScore.NewRect);

            return PlacementResult(RectScore.NewRect,RectScore.Rotated);
        }

        TexturePacker::ImageContainer SkylineTexturePacker::Insert(const ImageContainer& ToPack, PackResultHandler* Handler, const SkylinePlacement RectChoice)
        {
            ImageContainer Unpacked(ToPack);
            while( Unpacked.size() > 0 )
            {
                RectScoreResult BestScore;
                Integer BestIndex = -1;

                for( Whole UnpackedIndex = 0 ; UnpackedIndex < Unpacked.size() ; ++UnpackedIndex )
                {
                    RectScoreResult ScoredRect;
                    RectSizeType ImageSize = this->GetPaddedRectSize(Unpacked[UnpackedIndex].Img);

                    switch( RectChoice )
                    {
                        case Graphics::SLP_TopLeft:      ScoredRect = this->FindPositionForTopLeft(ImageSize);   break;
                        case Graphics::SLP_BestAreaFit:  ScoredRect = this->FindPositionForBestArea(ImageSize);  break;
                        default:                         return Unpacked;
                    }

                    if( ScoredRect.NewRect.GetArea() <= 0 ) {
                        continue;
                    }

                    if( ScoredRect < BestScore ) {
                        BestScore = ScoredRect;
                        BestIndex = UnpackedIndex;
                    }
                }

                if( BestIndex < 0 || BestScore.Index < 0 ) {
                    return Unpacked;
                }

                if( BestScore.Rotated ) {
                    this->AddRotatedToAtlas(Unpacked[BestIndex].Img,BestScore.NewRect);
                }else{
                    this->AddUnrotatedToAtlas(Unpacked[BestIndex].Img,BestScore.NewRect);
                }

                Handler->ProcessTexturePlacement(Unpacked[BestIndex],PlacementResult(BestScore.NewRect,BestScore.Rotated));
                this->AddSkylineLevel(BestScore.Index,BestScore.NewRect);
                Unpacked.erase( Unpacked.begin() + BestIndex );
            }
            return Unpacked;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Query

        const TexturePacker::RectVector& SkylineTexturePacker::GetUsedRectangles() const
            { return this->UsedRectangles; }

        Real SkylineTexturePacker::GetCoverage() const
        {
            Whole UsedSurfaceArea = 0;
            for( ConstRectIterator RectIt = this->UsedRectangles.begin() ; RectIt != this->UsedRectangles.end() ; ++RectIt )
                { UsedSurfaceArea += (*RectIt).GetArea(); }
            return static_cast<Real>( UsedSurfaceArea / ( this->AtlasImage.GetWidth() * this->AtlasImage.GetHeight() ) );
        }
    }//Graphics
}//Mezzanine

#endif
