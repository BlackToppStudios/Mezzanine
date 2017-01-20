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

#ifndef _graphicsguillotinetexturepacker_cpp
#define _graphicsguillotinetexturepacker_cpp

#include "Graphics/guillotinetexturepacker.h"

namespace
{
    using namespace Mezzanine;
    using namespace Mezzanine::Graphics;

    /// @brief Determines the score of a free Rect based on the best overall area fit.
    /// @param RectSize The size of the Rect to be placed.
    /// @param FreeRect The free Rect to be scored.
    /// @return Returns the score of placing the new texture in the free Rect.  Lower is better.
    static Integer ScoreBestAreaFit(const TexturePacker::RectSizeType& RectSize, const TexturePacker::RectType& FreeRect)
        { return FreeRect.GetWidth() * FreeRect.GetHeight() - RectSize.Width * RectSize.Height; }
    /// @brief Determines the score of a free Rect based on closely matching the short side.
    /// @param RectSize The size of the Rect to be placed.
    /// @param FreeRect The free Rect to be scored.
    /// @return Returns the score of placing the new texture in the free Rect.  Lower is better.
    static Integer ScoreBestShortSideFit(const TexturePacker::RectSizeType& RectSize, const TexturePacker::RectType& FreeRect)
        { return std::min( std::abs( FreeRect.GetWidth() - RectSize.Width ), std::abs( FreeRect.GetHeight() - RectSize.Height ) ); }
    /// @brief Determines the score of a free Rect based on closely matching the long side.
    /// @param RectSize The size of the Rect to be placed.
    /// @param FreeRect The free Rect to be scored.
    /// @return Returns the score of placing the new texture in the free Rect.  Lower is better.
    static Integer ScoreBestLongSideFit(const TexturePacker::RectSizeType& RectSize, const TexturePacker::RectType& FreeRect)
        { return std::max( std::abs( FreeRect.GetWidth() - RectSize.Width ), std::abs( FreeRect.GetHeight() - RectSize.Height ) ); }
    /// @brief Determines the score of a free Rect based on the worst overall area fit.
    /// @param RectSize The size of the Rect to be placed.
    /// @param FreeRect The free Rect to be scored.
    /// @return Returns the score of placing the new texture in the free Rect.  Lower is better.
    static Integer ScoreWorstAreaFit(const TexturePacker::RectSizeType& RectSize, const TexturePacker::RectType& FreeRect)
        { return -( ScoreBestAreaFit(RectSize,FreeRect) ); }
    /// @brief Determines the score of a free Rect based on the most leftover space for the long side.
    /// @param RectSize The size of the Rect to be placed.
    /// @param FreeRect The free Rect to be scored.
    /// @return Returns the score of placing the new texture in the free Rect.  Lower is better.
    static Integer ScoreWorstShortSideFit(const TexturePacker::RectSizeType& RectSize, const TexturePacker::RectType& FreeRect)
        { return -( ScoreBestShortSideFit(RectSize,FreeRect) ); }
    /// @brief Determines the score of a free Rect based on the most leftover space for the short side.
    /// @param RectSize The size of the Rect to be placed.
    /// @param FreeRect The free Rect to be scored.
    /// @return Returns the score of placing the new texture in the free Rect.  Lower is better.
    static Integer ScoreWorstLongSideFit(const TexturePacker::RectSizeType& RectSize, const TexturePacker::RectType& FreeRect)
        { return -( ScoreBestLongSideFit(RectSize,FreeRect) ); }
    /// @brief Determines the score of a free Rect for placing a new texture.
    /// @param RectSize The size of the Rect to be placed.
    /// @param FreeRect The free Rect to be scored.
    /// @param RectChoice The heuristic to use for scoring which free Rect to use for new texture placement.
    /// @return Returns the score of placing the new texture in the free Rect.  Lower is better.
    static Integer ScoreByHeuristic(const TexturePacker::RectSizeType& RectSize, const TexturePacker::RectType& FreeRect, const GuillotinePlacement RectChoice)
    {
        switch( RectChoice )
        {
            case Graphics::GP_BestAreaFit:       return ScoreBestAreaFit(RectSize,FreeRect);       break;
            case Graphics::GP_BestShortSideFit:  return ScoreBestShortSideFit(RectSize,FreeRect);  break;
            case Graphics::GP_BestLongSideFit:   return ScoreBestLongSideFit(RectSize,FreeRect);   break;
            case Graphics::GP_WorstAreaFit:      return ScoreWorstAreaFit(RectSize,FreeRect);      break;
            case Graphics::GP_WorstShortSideFit: return ScoreWorstShortSideFit(RectSize,FreeRect); break;
            case Graphics::GP_WorstLongSideFit:  return ScoreWorstLongSideFit(RectSize,FreeRect);  break;
            default:                             return std::numeric_limits<TexturePacker::RectType::MemberType>::max();  break;
        }
    }
}

namespace Mezzanine
{
    namespace Graphics
    {
        GuillotineTexturePacker::GuillotineTexturePacker(const Whole TexWidth, const Whole TexHeight)
            { this->Initialize(TexWidth,TexHeight); }

        TexturePacker::PlacementResult GuillotineTexturePacker::FindPositionForNewNode(const RectSizeType& RectSize, const GuillotinePlacement RectChoice, Integer* NodeIndex) const
        {
            PlacementResult BestResult;
            Integer BestScore = std::numeric_limits<RectType::MemberType>::max();

            for( Whole CurrIndex = 0 ; CurrIndex < this->FreeRectangles.size() ; ++CurrIndex )
            {
                const RectType& CurrRect = this->FreeRectangles[CurrIndex];
                RectSizeType CurrRectSize = CurrRect.GetSize();
                if( RectSize.IsSameSize(CurrRectSize) ) {
                    // Fits exactly without rotating.
                    BestResult.first.SetValuesBySize(CurrRect.Left,CurrRect.Top,RectSize);
                    BestResult.second = false;
                    BestScore = std::numeric_limits<RectType::MemberType>::min();
                    *NodeIndex = CurrIndex;
                    break;
                }else if( this->AllowRotated && RectSize.IsSameSizeRotated(CurrRectSize) ) {
                    // Only fits exactly after we rotate.
                    BestResult.first.SetValuesBySize(CurrRect.Left,CurrRect.Top,RectSize);
                    BestResult.second = true;
                    BestScore = std::numeric_limits<RectType::MemberType>::min();
                    *NodeIndex = CurrIndex;
                    break;
                }else if( RectSize.FitsInside(CurrRectSize) ) {
                    // Fits upright within.
                    Integer Score = ScoreByHeuristic(RectSize,CurrRect,RectChoice);
                    if( Score < BestScore ) {
                        BestResult.first.SetValuesBySize(CurrRect.Left,CurrRect.Top,RectSize);
                        BestResult.second = false;
                        BestScore = Score;
                        *NodeIndex = CurrIndex;
                    }
                }else if( this->AllowRotated && RectSize.FitsInsideRotated(CurrRectSize) ) {
                    // Fits sideways within.
                    Integer Score = ScoreByHeuristic(RectSize,CurrRect,RectChoice);
                    if( Score < BestScore ) {
                        BestResult.first.SetValuesBySize(CurrRect.Left,CurrRect.Top,RectSize.GetRotated());
                        BestResult.second = true;
                        BestScore = Score;
                        *NodeIndex = CurrIndex;
                    }
                }
            }
            return BestResult;
        }

        void GuillotineTexturePacker::SplitFreeRectByHeuristic(const RectType& FreeRect, const RectType& PlacedRect, const GuillotineSplit Method)
        {
            const Integer Width = FreeRect.GetWidth() - PlacedRect.GetWidth();
            const Integer Height = FreeRect.GetHeight() - PlacedRect.GetHeight();

            Boole SplitHorizontal = true;
            switch( Method )
            {
                case Graphics::GS_ShorterLeftoverAxis:  SplitHorizontal = ( Width <= Height );  break;
                case Graphics::GS_LongerLeftoverAxis:  SplitHorizontal = ( Width > Height );  break;
                case Graphics::GS_MinimizeArea:  SplitHorizontal = ( PlacedRect.GetWidth() * Height > PlacedRect.GetHeight() * Width );  break;
                case Graphics::GS_MaximizeArea:  SplitHorizontal = ( PlacedRect.GetWidth() * Height <= PlacedRect.GetHeight() * Width );  break;
                case Graphics::GS_ShorterAxis:  SplitHorizontal = ( FreeRect.GetWidth() <= FreeRect.GetHeight() );  break;
                case Graphics::GS_LongerAxis:  SplitHorizontal = ( FreeRect.GetWidth() > FreeRect.GetHeight() );  break;
            }
            SplitFreeRectAlongAxis(FreeRect,PlacedRect,SplitHorizontal);
        }

        void GuillotineTexturePacker::SplitFreeRectAlongAxis(const RectType& FreeRect, const RectType& PlacedRect, const Boole SplitHorizontal)
        {
            RectType Lower;
            RectType RSide;
            Lower.SetValuesBySize(FreeRect.Left,FreeRect.Top + PlacedRect.GetHeight(),0,FreeRect.GetHeight() - PlacedRect.GetHeight());
            RSide.SetValuesBySize(FreeRect.Left + PlacedRect.GetWidth(),FreeRect.Top,FreeRect.GetWidth() - PlacedRect.GetWidth(),0);

            if( SplitHorizontal ) {
                Lower.Right = Lower.Left + FreeRect.GetWidth();
                RSide.Bottom = RSide.Top + PlacedRect.GetHeight();
            }else{
                Lower.Right = Lower.Left + PlacedRect.GetWidth();
                RSide.Bottom = RSide.Top + FreeRect.GetHeight();
            }

            if( Lower.GetWidth() > 0 && Lower.GetHeight() > 0 ) {
                this->FreeRectangles.push_back(Lower);
            }
            if( RSide.GetWidth() > 0 && RSide.GetHeight() > 0 ) {
                this->FreeRectangles.push_back(RSide);
            }
        }

        void GuillotineTexturePacker::MergeFreeList()
        {
            for( Whole CurrIndex = 0 ; CurrIndex < this->FreeRectangles.size() ; ++CurrIndex )
            {
                RectType& RectA = this->FreeRectangles[CurrIndex];
                for( Whole AdvIndex = CurrIndex + 1 ; AdvIndex < this->FreeRectangles.size() ; ++AdvIndex )
                {
                    RectType& RectB = this->FreeRectangles[AdvIndex];
                    if( RectA.GetWidth() == RectB.GetWidth() && RectA.Left == RectB.Left ) {
                        if( RectA.Top == RectB.Bottom ) {
                            RectA.Top -= RectB.GetHeight();
                            this->FreeRectangles.erase( this->FreeRectangles.begin() + AdvIndex );
                            --AdvIndex;
                        }else if( RectA.Bottom == RectB.Top ) {
                            RectA.Bottom += RectB.GetHeight();
                            this->FreeRectangles.erase( this->FreeRectangles.begin() + AdvIndex );
                            --AdvIndex;
                        }
                    }else if( RectA.GetHeight() == RectB.GetHeight() && RectA.Top == RectB.Top ) {
                        if( RectA.Left == RectB.Right ) {
                            RectA.Left -= RectB.GetWidth();
                            this->FreeRectangles.erase( this->FreeRectangles.begin() + AdvIndex );
                            --AdvIndex;
                        }else if( RectA.Right == RectB.Left ) {
                            RectA.Right += RectB.GetWidth();
                            this->FreeRectangles.erase( this->FreeRectangles.begin() + AdvIndex );
                            --AdvIndex;
                        }
                    }
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Initialization

        void GuillotineTexturePacker::Initialize(const Whole TexWidth, const Whole TexHeight)
        {
            this->InitImage(TexWidth,TexHeight);

            this->UsedRectangles.clear();
            this->FreeRectangles.clear();

            RectType StartRect(0,0,TexWidth,TexHeight);
            this->FreeRectangles.push_back(StartRect);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Insertion

        TexturePacker::PlacementResult GuillotineTexturePacker::Insert(const Image* ToInsert)
            { return this->Insert(ToInsert,true,Graphics::GP_BestAreaFit,Graphics::GS_MinimizeArea); }

        TexturePacker::ImageContainer GuillotineTexturePacker::Insert(const ImageContainer& ToPack, PackResultHandler* Handler)
            { return this->Insert(ToPack,Handler,true,Graphics::GP_BestAreaFit,Graphics::GS_MinimizeArea); }

        TexturePacker::PlacementResult GuillotineTexturePacker::Insert(const Image* ToInsert, const Boole Merge,
                                                                const GuillotinePlacement RectChoice, const GuillotineSplit SplitMethod)
        {
            Integer FreeNodeIndex = 0;
            RectSizeType ImageSize = this->GetPaddedRectSize(ToInsert);
            PlacementResult Result = this->FindPositionForNewNode(ImageSize,RectChoice,&FreeNodeIndex);

            if( Result.first.GetArea() <= 0 ) {
                return Result;
            }

            if( Result.second ) {
                this->AddRotatedToAtlas(ToInsert,Result.first);
            }else{
                this->AddUnrotatedToAtlas(ToInsert,Result.first);
            }

            RectType FreeRect = this->FreeRectangles[FreeNodeIndex];
            SplitFreeRectByHeuristic(FreeRect,Result.first,SplitMethod);
            this->FreeRectangles.erase( this->FreeRectangles.begin() + FreeNodeIndex );
            this->UsedRectangles.push_back(Result.first);

            if( Merge ) {
                this->MergeFreeList();
            }

            return Result;
        }

        TexturePacker::ImageContainer GuillotineTexturePacker::Insert(const TexturePacker::ImageContainer& ToPack, PackResultHandler* Handler, const Boole Merge,
                                                                      const GuillotinePlacement RectChoice, const GuillotineSplit SplitMethod)
        {
            Integer BestFreeRect = 0;
            Integer BestRect = 0;
            Boole BestRotated = false;
            ImageContainer Unpacked(ToPack);

            while( Unpacked.size() > 0 )
            {
                Integer BestScore = std::numeric_limits<RectType::MemberType>::max();

                for( Whole FreeIndex = 0 ; FreeIndex < this->FreeRectangles.size() ; ++FreeIndex )
                {
                    RectType& FreeRect = this->FreeRectangles[FreeIndex];
                    RectSizeType FreeRectSize = FreeRect.GetSize();
                    for( Whole UnpackedIndex = 0 ; UnpackedIndex < Unpacked.size() ; ++UnpackedIndex )
                    {
                        RectSizeType UnpackedRectSize = this->GetPaddedRectSize(Unpacked[UnpackedIndex].Img);
                        if( UnpackedRectSize.IsSameSize(FreeRectSize) ) {
                            BestFreeRect = FreeIndex;
                            BestRect = UnpackedIndex;
                            BestRotated = false;
                            BestScore = std::numeric_limits<RectType::MemberType>::min();
                            FreeIndex = this->FreeRectangles.size();
                            break;
                        }else if( this->AllowRotated && UnpackedRectSize.IsSameSizeRotated(FreeRectSize) ) {
                            BestFreeRect = FreeIndex;
                            BestRect = UnpackedIndex;
                            BestRotated = true;
                            BestScore = std::numeric_limits<RectType::MemberType>::min();
                            FreeIndex = this->FreeRectangles.size();
                            break;
                        }else if( UnpackedRectSize.FitsInside(FreeRectSize) ) {
                            Integer Score = ScoreByHeuristic(UnpackedRectSize,FreeRect,RectChoice);
                            if( Score < BestScore ) {
                                BestFreeRect = FreeIndex;
                                BestRect = UnpackedIndex;
                                BestRotated = false;
                                BestScore = Score;
                            }
                        }else if( this->AllowRotated && UnpackedRectSize.FitsInsideRotated(FreeRectSize) ) {
                            Integer Score = ScoreByHeuristic(UnpackedRectSize.GetRotated(),FreeRect,RectChoice);
                            if( Score < BestScore ) {
                                BestFreeRect = FreeIndex;
                                BestRect = UnpackedIndex;
                                BestRotated = true;
                                BestScore = Score;
                            }
                        }
                    }
                }

                if( BestScore == std::numeric_limits<RectType::MemberType>::max() ) {
                    return Unpacked;
                }

                RectType NewNode( this->FreeRectangles[BestFreeRect].Left,
                                  this->FreeRectangles[BestFreeRect].Top,
                                  this->GetPaddedRectSize(Unpacked[BestRect].Img) );

                if( BestRotated ) {
                    NewNode.Rotate();
                    this->AddRotatedToAtlas(Unpacked[BestRect].Img,NewNode);
                }else{
                    this->AddUnrotatedToAtlas(Unpacked[BestRect].Img,NewNode);
                }

                Handler->ProcessTexturePlacement(Unpacked[BestRect],PlacementResult(NewNode,BestRotated));
                this->UsedRectangles.push_back(NewNode);
                SplitFreeRectByHeuristic(this->FreeRectangles[BestFreeRect],NewNode,SplitMethod);
                this->FreeRectangles.erase( this->FreeRectangles.begin() + BestFreeRect );
                Unpacked.erase( Unpacked.begin() + BestRect );

                if( Merge ) {
                    this->MergeFreeList();
                }
            }

            return Unpacked;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Query

        const TexturePacker::RectVector& GuillotineTexturePacker::GetFreeRectangles() const
            { return this->FreeRectangles; }

        const TexturePacker::RectVector& GuillotineTexturePacker::GetUsedRectangles() const
            { return this->UsedRectangles; }

        Real GuillotineTexturePacker::GetCoverage() const
        {
            Whole UsedSurfaceArea = 0;
            for( ConstRectIterator RectIt = this->UsedRectangles.begin() ; RectIt != this->UsedRectangles.end() ; ++RectIt )
                { UsedSurfaceArea += (*RectIt).GetArea(); }
            return static_cast<Real>( UsedSurfaceArea / ( this->AtlasImage.GetWidth() * this->AtlasImage.GetHeight() ) );
        }
    }//Graphics
}//Mezzanine

#endif
