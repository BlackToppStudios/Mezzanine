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

#ifndef _graphicsmaxrectstexturepacker_cpp
#define _graphicsmaxrectstexturepacker_cpp

#include "Graphics/maxrectstexturepacker.h"

namespace Mezzanine
{
    namespace Graphics
    {
        /// @brief Returns the length of shared between two segments.
        /// @param Int1Start The start point of the first segment.
        /// @param Int1End The end point of the first segment.
        /// @param Int2Start The start point of the second segment.
        /// @param Int2End The end point of the second segment.
        /// @return Returns the length of the overlap, or zero if the two segments are disjoint otherwise.
        Integer CommonIntervalLength(const Integer Int1Start, const Integer Int1End, const Integer Int2Start, const Integer Int2End)
        {
            if( Int1End < Int2Start || Int2End < Int1Start ) {
                return 0;
            }
            return std::min(Int1End,Int2End) - std::max(Int1Start,Int2Start);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // MaxRectsTexturePacker Methods

        MaxRectsTexturePacker::MaxRectsTexturePacker()
            {  }

        MaxRectsTexturePacker::MaxRectsTexturePacker(const Whole TexWidth, const Whole TexHeight)
            { this->Initialize(TexWidth,TexHeight); }

        MaxRectsTexturePacker::~MaxRectsTexturePacker()
            {  }

        RectScoreResult MaxRectsTexturePacker::ScoreRect(const RectSizeType& RectSize, const MaxRectsPlacement Method) const
        {
            RectScoreResult Ret;

            switch( Method )
            {
                case Graphics::MRP_BestShortSideFit:  Ret = FindPositionForBestShortSideFit(RectSize);  break;
                case Graphics::MRP_BestLongSideFit:   Ret = FindPositionForBestLongSideFit(RectSize);   break;
                case Graphics::MRP_BestAreaFit:       Ret = FindPositionForBestAreaFit(RectSize);       break;
                case Graphics::MRP_TopLeftRule:       Ret = FindPositionForTopLeft(RectSize);           break;
                case Graphics::MRP_ContactPointRule:
                {
                    Ret = FindPositionForContactPoint(RectSize);
                    Ret.Score1 = -Ret.Score1;
                    break;
                }
            }

            if( Ret.NewRect.GetArea() <= 0 ) {
                Ret.ResetScores();
            }

            return Ret;
        }

        void MaxRectsTexturePacker::PlaceRect(const RectType& Node)
        {
            Whole NumRectangles = this->FreeRectangles.size();
            for( Whole CurrIndex = 0 ; CurrIndex < NumRectangles ; ++CurrIndex )
            {
                RectType FreeRect = this->FreeRectangles[CurrIndex];
                if( this->SplitFreeNode(FreeRect,Node) ) {
                    this->FreeRectangles.erase( this->FreeRectangles.begin() + CurrIndex );
                    --CurrIndex;
                    --NumRectangles;
                }
            }

            this->PruneFreeList();
            this->UsedRectangles.push_back(Node);
        }

        Integer MaxRectsTexturePacker::ContactPointScoreNode(const Integer XPos, const Integer YPos, const RectSizeType& RectSize) const
        {
            Integer Score = 0;

            if( XPos == 0 || XPos + RectSize.Width == static_cast<Integer>( this->AtlasImage.GetWidth() ) ) {
                Score += RectSize.Height;
            }
            if( YPos == 0 || YPos + RectSize.Height == static_cast<Integer>( this->AtlasImage.GetHeight() ) ) {
                Score += RectSize.Width;
            }

            for( Whole CurrIndex = 0 ; CurrIndex < this->UsedRectangles.size() ; ++CurrIndex )
            {
                const RectType& CurrRect = this->UsedRectangles[CurrIndex];
                if( CurrRect.Left == XPos + RectSize.Width || CurrRect.Right == XPos ) {
                    Score += CommonIntervalLength( CurrRect.Top, CurrRect.Bottom, YPos, YPos + RectSize.Height );
                }
                if( CurrRect.Top == YPos + RectSize.Height || CurrRect.Bottom == YPos ) {
                    Score += CommonIntervalLength( CurrRect.Left, CurrRect.Right, XPos, XPos + RectSize.Width );
                }
            }

            return Score;
        }

        Boole MaxRectsTexturePacker::SplitFreeNode(const RectType& FreeNode, const RectType& UsedNode)
        {
            if( UsedNode.Left >= FreeNode.Right || UsedNode.Right <= FreeNode.Left ||
                UsedNode.Top >= FreeNode.Bottom || UsedNode.Bottom <= FreeNode.Top )
                { return false; }

            if( UsedNode.Left < FreeNode.Right && UsedNode.Right > FreeNode.Left ) {
                // New node at top side of the used node
                if( UsedNode.Top > FreeNode.Top && UsedNode.Top < FreeNode.Bottom ) {
                    RectType NewNode = FreeNode;
                    NewNode.Bottom = UsedNode.Top;
                    this->FreeRectangles.push_back(NewNode);
                }
                // New node at bottom side of the used node
                if( UsedNode.Bottom < FreeNode.Bottom ) {
                    RectType NewNode = FreeNode;
                    NewNode.Top = UsedNode.Bottom;
                    this->FreeRectangles.push_back(NewNode);
                }
            }

            if( UsedNode.Top < FreeNode.Bottom && UsedNode.Bottom > FreeNode.Top ) {
                // New node at the left side of the used node
                if( UsedNode.Left > FreeNode.Left && UsedNode.Left < FreeNode.Right ) {
                    RectType NewNode = FreeNode;
                    NewNode.Right = UsedNode.Left;
                    this->FreeRectangles.push_back(NewNode);
                }
                // New node at the right side of the used node
                if( UsedNode.Right < FreeNode.Right ) {
                    RectType NewNode = FreeNode;
                    NewNode.Left = UsedNode.Right;
                    this->FreeRectangles.push_back(NewNode);
                }
            }

            return true;
        }

        RectScoreResult MaxRectsTexturePacker::FindPositionForTopLeft(const RectSizeType& RectSize) const
        {
            RectScoreResult BestNode;
            Integer BestY = std::numeric_limits<RectType::MemberType>::max();
            Integer BestX = std::numeric_limits<RectType::MemberType>::max();

            for( Whole CurrIndex = 0 ; CurrIndex < this->FreeRectangles.size() ; ++CurrIndex )
            {
                const RectType& CurrRect = this->FreeRectangles[CurrIndex];
                const RectSizeType CurrRectSize = CurrRect.GetSize();
                if( RectSize.FitsInside(CurrRectSize) ) {
                    Integer TopSideY = CurrRect.Top + RectSize.Height;
                    if( TopSideY < BestY || ( TopSideY == BestY && CurrRect.Left < BestX ) ) {
                        BestNode.NewRect.SetValuesBySize(CurrRect.Left,CurrRect.Top,RectSize);
                        BestNode.Rotated = false;
                        BestY = TopSideY;
                        BestX = CurrRect.Left;
                    }
                }
                if( this->AllowRotated && RectSize.FitsInsideRotated(CurrRectSize) ) {
                    Integer TopSideY = CurrRect.Top + RectSize.Width;
                    if( TopSideY < BestY || ( TopSideY == BestY && CurrRect.Left < BestX ) ) {
                        BestNode.NewRect.SetValuesBySize(CurrRect.Left,CurrRect.Top,RectSize.GetRotated());
                        BestNode.Rotated = true;
                        BestY = TopSideY;
                        BestX = CurrRect.Left;
                    }
                }
            }

            BestNode.Score1 = BestY;
            BestNode.Score2 = BestX;
            return BestNode;
        }

        RectScoreResult MaxRectsTexturePacker::FindPositionForBestShortSideFit(const RectSizeType& RectSize) const
        {
            RectScoreResult BestNode;
            Integer BestLongSideFit = std::numeric_limits<RectType::MemberType>::max();
            Integer BestShortSideFit = std::numeric_limits<RectType::MemberType>::max();

            for( Whole CurrIndex = 0 ; CurrIndex < this->FreeRectangles.size() ; ++CurrIndex )
            {
                const RectType& CurrRect = this->FreeRectangles[CurrIndex];
                const RectSizeType CurrRectSize = CurrRect.GetSize();
                if( RectSize.FitsInside(CurrRectSize) ) {
                    Integer LeftOverHori = std::abs( CurrRectSize.Width - RectSize.Width );
                    Integer LeftOverVert = std::abs( CurrRectSize.Height - RectSize.Height );
                    Integer ShortSideFit = std::min( LeftOverHori, LeftOverVert );
                    Integer LongSideFit = std::max( LeftOverHori, LeftOverVert );

                    if( ShortSideFit < BestShortSideFit || ( ShortSideFit == BestShortSideFit && LongSideFit < BestLongSideFit ) ) {
                        BestNode.NewRect.SetValuesBySize(CurrRect.Left,CurrRect.Top,RectSize);
                        BestNode.Rotated = false;
                        BestLongSideFit = LongSideFit;
                        BestShortSideFit = ShortSideFit;
                    }
                }
                if( this->AllowRotated && RectSize.FitsInsideRotated(CurrRectSize) ) {
                    Integer LeftOverHori = std::abs( CurrRectSize.Width - RectSize.Height );
                    Integer LeftOverVert = std::abs( CurrRectSize.Height - RectSize.Width );
                    Integer ShortSideFit = std::min( LeftOverHori, LeftOverVert );
                    Integer LongSideFit = std::max( LeftOverHori, LeftOverVert );

                    if( ShortSideFit < BestShortSideFit || ( ShortSideFit == BestShortSideFit && LongSideFit < BestLongSideFit ) ) {
                        BestNode.NewRect.SetValuesBySize(CurrRect.Left,CurrRect.Top,RectSize.GetRotated());
                        BestNode.Rotated = true;
                        BestLongSideFit = LongSideFit;
                        BestShortSideFit = ShortSideFit;
                    }
                }
            }

            BestNode.Score1 = BestShortSideFit;
            BestNode.Score2 = BestLongSideFit;
            return BestNode;
        }

        RectScoreResult MaxRectsTexturePacker::FindPositionForBestLongSideFit(const RectSizeType& RectSize) const
        {
            RectScoreResult BestNode;
            Integer BestLongSideFit = std::numeric_limits<RectType::MemberType>::max();
            Integer BestShortSideFit = std::numeric_limits<RectType::MemberType>::max();

            for( Whole CurrIndex = 0 ; CurrIndex < this->FreeRectangles.size() ; ++CurrIndex )
            {
                const RectType& CurrRect = this->FreeRectangles[CurrIndex];
                const RectSizeType CurrRectSize = CurrRect.GetSize();
                if( RectSize.FitsInside(CurrRectSize) ) {
                    Integer LeftOverHori = std::abs( CurrRectSize.Width - RectSize.Width );
                    Integer LeftOverVert = std::abs( CurrRectSize.Height - RectSize.Height );
                    Integer ShortSideFit = std::min( LeftOverHori, LeftOverVert );
                    Integer LongSideFit = std::max( LeftOverHori, LeftOverVert );

                    if( LongSideFit < BestLongSideFit || ( LongSideFit == BestLongSideFit && ShortSideFit < BestShortSideFit ) ) {
                        BestNode.NewRect.SetValuesBySize(CurrRect.Left,CurrRect.Top,RectSize);
                        BestNode.Rotated = false;
                        BestLongSideFit = LongSideFit;
                        BestShortSideFit = ShortSideFit;
                    }
                }
                if( this->AllowRotated && RectSize.FitsInsideRotated(CurrRectSize) ) {
                    Integer LeftOverHori = std::abs( CurrRectSize.Width - RectSize.Height );
                    Integer LeftOverVert = std::abs( CurrRectSize.Height - RectSize.Width );
                    Integer ShortSideFit = std::min( LeftOverHori, LeftOverVert );
                    Integer LongSideFit = std::max( LeftOverHori, LeftOverVert );

                    if( LongSideFit < BestLongSideFit || ( LongSideFit == BestLongSideFit && ShortSideFit < BestShortSideFit ) ) {
                        BestNode.NewRect.SetValuesBySize(CurrRect.Left,CurrRect.Top,RectSize.GetRotated());
                        BestNode.Rotated = true;
                        BestLongSideFit = LongSideFit;
                        BestShortSideFit = ShortSideFit;
                    }
                }
            }

            BestNode.Score1 = BestShortSideFit;
            BestNode.Score2 = BestLongSideFit;
            return BestNode;
        }

        RectScoreResult MaxRectsTexturePacker::FindPositionForBestAreaFit(const RectSizeType& RectSize) const
        {
            RectScoreResult BestNode;
            Integer BestAreaFit = std::numeric_limits<RectType::MemberType>::max();
            Integer BestShortSideFit = std::numeric_limits<RectType::MemberType>::max();

            for( Whole CurrIndex = 0 ; CurrIndex < this->FreeRectangles.size() ; ++CurrIndex )
            {
                const RectType& CurrRect = this->FreeRectangles[CurrIndex];
                const RectSizeType CurrRectSize = CurrRect.GetSize();
                Integer AreaFit = CurrRectSize.GetArea() - RectSize.GetArea();

                if( RectSize.FitsInside(CurrRectSize) ) {
                    Integer LeftOverHori = std::abs( CurrRectSize.Width - RectSize.Width );
                    Integer LeftOverVert = std::abs( CurrRectSize.Height - RectSize.Height );
                    Integer ShortSideFit = std::min( LeftOverHori, LeftOverVert );

                    if( AreaFit < BestAreaFit || ( AreaFit == BestAreaFit && ShortSideFit < BestShortSideFit ) ) {
                        BestNode.NewRect.SetValuesBySize(CurrRect.Left,CurrRect.Top,RectSize);
                        BestNode.Rotated = false;
                        BestShortSideFit = ShortSideFit;
                        BestAreaFit = AreaFit;
                    }
                }
                if( this->AllowRotated && RectSize.FitsInsideRotated(CurrRectSize) ) {
                    Integer LeftOverHori = std::abs( CurrRectSize.Width - RectSize.Height );
                    Integer LeftOverVert = std::abs( CurrRectSize.Height - RectSize.Width );
                    Integer ShortSideFit = std::min( LeftOverHori, LeftOverVert );

                    if( AreaFit < BestAreaFit || ( AreaFit == BestAreaFit && ShortSideFit < BestShortSideFit ) ) {
                        BestNode.NewRect.SetValuesBySize(CurrRect.Left,CurrRect.Top,RectSize.GetRotated());
                        BestNode.Rotated = true;
                        BestShortSideFit = ShortSideFit;
                        BestAreaFit = AreaFit;
                    }
                }
            }

            BestNode.Score1 = BestAreaFit;
            BestNode.Score2 = BestShortSideFit;
            return BestNode;
        }

        RectScoreResult MaxRectsTexturePacker::FindPositionForContactPoint(const RectSizeType& RectSize) const
        {
            RectScoreResult BestNode;
            Integer BestContactScore = -1;

            for( Whole CurrIndex = 0 ; CurrIndex < this->FreeRectangles.size() ; ++CurrIndex )
            {
                const RectType& CurrRect = this->FreeRectangles[CurrIndex];
                const RectSizeType CurrRectSize = CurrRect.GetSize();
                if( RectSize.FitsInside(CurrRectSize) ) {
                    Integer Score = this->ContactPointScoreNode(CurrRect.Left,CurrRect.Top,RectSize);
                    if( Score > BestContactScore ) {
                        BestNode.NewRect.SetValuesBySize(CurrRect.Left,CurrRect.Top,RectSize);
                        BestNode.Rotated = false;
                        BestContactScore = Score;
                    }
                }
                if( this->AllowRotated && RectSize.FitsInsideRotated(CurrRectSize) ) {
                    Integer Score = this->ContactPointScoreNode(CurrRect.Left,CurrRect.Top,RectSize.GetRotated());
                    if( Score > BestContactScore ) {
                        BestNode.NewRect.SetValuesBySize(CurrRect.Left,CurrRect.Top,RectSize.GetRotated());
                        BestNode.Rotated = true;
                        BestContactScore = Score;
                    }
                }
            }

            BestNode.Score1 = BestContactScore;
            return BestNode;
        }

        void MaxRectsTexturePacker::PruneFreeList()
        {
            for( Whole CurrIndex = 0 ; CurrIndex < this->FreeRectangles.size() ; ++CurrIndex )
            {
                const RectType& CurrRect = this->FreeRectangles[CurrIndex];
                for( Whole CompIndex = CurrIndex + 1 ; CompIndex < this->FreeRectangles.size() ; ++CompIndex )
                {
                    const RectType& CompRect = this->FreeRectangles[CompIndex];
                    if( CurrRect.IsContainedIn(CompRect) ) {
                        this->FreeRectangles.erase( this->FreeRectangles.begin() + CurrIndex );
                        --CurrIndex;
                    }else if( CompRect.IsContainedIn(CurrRect) ) {
                        this->FreeRectangles.erase( this->FreeRectangles.begin() + CompIndex );
                        --CompIndex;
                    }
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Initialization

        void MaxRectsTexturePacker::Initialize(const Whole TexWidth, const Whole TexHeight)
        {
            this->InitImage(TexWidth,TexHeight);

            this->UsedRectangles.clear();
            this->FreeRectangles.clear();

            RectType StartRect(0,0,TexWidth,TexHeight);
            this->FreeRectangles.push_back(StartRect);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Insertion

        TexturePacker::PlacementResult MaxRectsTexturePacker::Insert(const Image* ToInsert)
            { return this->Insert(ToInsert,true,Graphics::MRP_TopLeftRule); }

        TexturePacker::ImageContainer MaxRectsTexturePacker::Insert(const ImageContainer& ToPack, PackResultHandler* Handler)
            { return this->Insert(ToPack,Handler,true,Graphics::MRP_TopLeftRule); }

        TexturePacker::PlacementResult MaxRectsTexturePacker::Insert(const Image* ToInsert, const Boole Merge, const MaxRectsPlacement RectChoice)
        {
            RectScoreResult Ret;
            RectSizeType PackSize = this->GetPaddedRectSize(ToInsert);
            switch( RectChoice )
            {
                case MRP_BestShortSideFit:  Ret = this->FindPositionForBestShortSideFit(PackSize);
                case MRP_TopLeftRule:       Ret = this->FindPositionForTopLeft(PackSize);
                case MRP_ContactPointRule:  Ret = this->FindPositionForContactPoint(PackSize);
                case MRP_BestLongSideFit:   Ret = this->FindPositionForBestLongSideFit(PackSize);
                case MRP_BestAreaFit:       Ret = this->FindPositionForBestAreaFit(PackSize);
            }

            if( Ret.NewRect.GetArea() <= 0 ) {
                return PlacementResult(Ret.NewRect,Ret.Rotated);
            }

            if( Ret.Rotated ) {
                this->AddRotatedToAtlas(ToInsert,Ret.NewRect);
            }else{
                this->AddUnrotatedToAtlas(ToInsert,Ret.NewRect);
            }

            this->PlaceRect(Ret.NewRect);
            return PlacementResult(Ret.NewRect,Ret.Rotated);
        }

        TexturePacker::ImageContainer MaxRectsTexturePacker::Insert(const ImageContainer& ToPack, PackResultHandler* Handler, const Boole Merge, const MaxRectsPlacement RectChoice)
        {
            ImageContainer Unpacked(ToPack);
            while( Unpacked.size() > 0 )
            {
                RectScoreResult BestScore;

                for( Whole UnpackedIndex = 0 ; UnpackedIndex < Unpacked.size() ; ++UnpackedIndex )
                {
                    RectScoreResult ScoredRect;
                    RectSizeType UnpackedRectSize = this->GetPaddedRectSize(Unpacked[UnpackedIndex].Img);
                    ScoredRect = this->ScoreRect(UnpackedRectSize,RectChoice);

                    if( ScoredRect < BestScore ) {
                        BestScore = ScoredRect;
                        BestScore.Index = UnpackedIndex;
                    }
                }

                if( BestScore.Index == -1 ) {
                    return Unpacked;
                }

                if( BestScore.Rotated ) {
                    this->AddRotatedToAtlas(Unpacked[BestScore.Index].Img,BestScore.NewRect);
                }else{
                    this->AddUnrotatedToAtlas(Unpacked[BestScore.Index].Img,BestScore.NewRect);
                }

                Handler->ProcessTexturePlacement(Unpacked[BestScore.Index],PlacementResult(BestScore.NewRect,BestScore.Rotated));
                this->PlaceRect(BestScore.NewRect);
                Unpacked.erase( Unpacked.begin() + BestScore.Index );
            }
            return Unpacked;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Query

        const TexturePacker::RectVector& MaxRectsTexturePacker::GetFreeRectangles() const
            { return this->FreeRectangles; }

        const TexturePacker::RectVector& MaxRectsTexturePacker::GetUsedRectangles() const
            { return this->UsedRectangles; }

        Real MaxRectsTexturePacker::GetCoverage() const
        {
            Whole UsedSurfaceArea = 0;
            for( ConstRectIterator RectIt = this->UsedRectangles.begin() ; RectIt != this->UsedRectangles.end() ; ++RectIt )
                { UsedSurfaceArea += (*RectIt).GetArea(); }
            return static_cast<Real>( UsedSurfaceArea / ( this->AtlasImage.GetWidth() * this->AtlasImage.GetHeight() ) );
        }
    }//Graphics
}//Mezzanine

#endif
