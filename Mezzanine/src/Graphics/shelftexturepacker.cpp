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

#ifndef _graphicsshelftexturepacker_cpp
#define _graphicsshelftexturepacker_cpp

#include "Graphics/shelftexturepacker.h"

namespace Mezzanine
{
    namespace Graphics
    {
        ///////////////////////////////////////////////////////////////////////////////
        // TextureShelf Methods

        TextureShelf::TextureShelf(const Integer YPosition) :
            XPos(0),
            YPos(YPosition),
            ShelfHeight(0)
            {  }

        TextureShelf::TextureShelf(const Integer YPosition, const Integer Height) :
            XPos(0),
            YPos(YPosition),
            ShelfHeight(Height)
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Functionality

        Boole TextureShelf::FitsOnShelf(const RectSizeType& RectSize, const RectSizeType& AtlasSize, const Boole CanResize) const
        {
            const Integer ActualHeight = CanResize ? ( AtlasSize.Height - this->YPos ) : this->ShelfHeight;
            return ( this->XPos + RectSize.Width <= AtlasSize.Width && RectSize.Height <= ActualHeight );
        }

        TexturePacker::RectType TextureShelf::AddToShelf(const RectSizeType& RectSize)
        {
            RectType NewNode(this->XPos,this->YPos,RectSize);
            this->UsedRectangles.push_back(NewNode);

            this->XPos += RectSize.Width;
            this->ShelfHeight = std::max(this->ShelfHeight,RectSize.Height);
            return NewNode;
        }

        Boole TextureShelf::ShouldRotateToFit(const RectSizeType& RectSize, const RectSizeType& AtlasSize) const
        {
            Boole NoXSpace = ( RectSize.Width > RectSize.Height &&
                               RectSize.Width > AtlasSize.Width - this->XPos );
            Boole ShelfTallEnough = ( RectSize.Width > RectSize.Height &&
                                      RectSize.Width < this->ShelfHeight );
            Boole RectTooTall = ( RectSize.Width < RectSize.Height &&
                                  RectSize.Height > this->ShelfHeight &&
                                  RectSize.Height <= AtlasSize.Width - this->XPos );
            return ( NoXSpace || ShelfTallEnough || RectTooTall );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // ShelfTexturePacker Methods

        ShelfTexturePacker::ShelfTexturePacker() :
            CurrentY(0)
            {  }

        ShelfTexturePacker::ShelfTexturePacker(const Whole TexWidth, const Whole TexHeight) :
            CurrentY(0)
            { this->Initialize(TexWidth,TexHeight); }

        Boole ShelfTexturePacker::CanStartNewShelf(const Integer Height) const
        {
            const TextureShelf& Last = this->Shelves.back();
            return ( Last.YPos + Last.ShelfHeight + Height <= static_cast<Integer>( this->AtlasImage.GetHeight() ) );
        }

        void ShelfTexturePacker::StartNewShelf(const Integer InitialHeight)
        {
            if( !this->Shelves.empty() ) {
                this->CurrentY += this->Shelves.back().ShelfHeight;
            }

            TextureShelf Shelf(this->CurrentY,InitialHeight);
            this->Shelves.push_back(Shelf);
        }

        TexturePacker::PlacementResult ShelfTexturePacker::HandleNextFit(const RectSizeType& RectSize)
        {
            PlacementResult RetRect(RectType(),false);
            TextureShelf& LastShelf = this->Shelves.back();
            RectSizeType AtlasSize(this->AtlasImage.GetWidth(),this->AtlasImage.GetHeight());
            if( LastShelf.FitsOnShelf(RectSize,AtlasSize,true) ) {
                RetRect.first = LastShelf.AddToShelf(RectSize);
            }
            return RetRect;
        }

        TexturePacker::PlacementResult ShelfTexturePacker::HandleFirstFit(const RectSizeType& RectSize)
        {
            PlacementResult RetRect(RectType(),false);
            RectSizeType AtlasSize(this->AtlasImage.GetWidth(),this->AtlasImage.GetHeight());

            const ShelfIterator EndShelf = this->Shelves.end();
            const ShelfIterator LastShelf = --this->Shelves.end();
            for( ShelfIterator ShelfIt = this->Shelves.begin() ; ShelfIt != EndShelf ; ++ShelfIt )
            {
                if( (*ShelfIt).FitsOnShelf(RectSize,AtlasSize,ShelfIt == LastShelf) ) {
                    RetRect.first = (*ShelfIt).AddToShelf(RectSize);
                    return RetRect;
                }
            }
            return RetRect;
        }

        TexturePacker::PlacementResult ShelfTexturePacker::HandleBestAreaFit(const RectSizeType& RectSize)
        {
            PlacementResult RetRect(RectType(),false);
            RectSizeType AtlasSize(this->AtlasImage.GetWidth(),this->AtlasImage.GetHeight());
            Whole BestShelfArea = std::numeric_limits<Whole>::max();
            Boole BestRotated = false;

            ShelfIterator BestShelf = this->Shelves.end();
            const ShelfIterator EndShelf = this->Shelves.end();
            const ShelfIterator LastShelf = --this->Shelves.end();
            for( ShelfIterator ShelfIt = this->Shelves.begin() ; ShelfIt != EndShelf ; ++ShelfIt )
            {
                Boole IsRotated = this->AllowRotated && (*ShelfIt).ShouldRotateToFit(RectSize,AtlasSize);
                RectSizeType TestSize = ( IsRotated ? RectSize.GetRotated() : RectSize );

                if( (*ShelfIt).FitsOnShelf(TestSize,AtlasSize,ShelfIt == LastShelf) ) {
                    Whole ShelfArea = ( AtlasSize.Width - (*ShelfIt).XPos ) * (*ShelfIt).ShelfHeight;
                    if( ShelfArea < BestShelfArea ) {
                        BestShelf = ShelfIt;
                        BestShelfArea = ShelfArea;
                        BestRotated = IsRotated;
                    }
                }
            }

            if( BestShelf != EndShelf ) {
                RetRect.first = (*BestShelf).AddToShelf( BestRotated ? RectSize.GetRotated() : RectSize );
                RetRect.second = BestRotated;
            }
            return RetRect;
        }

        TexturePacker::PlacementResult ShelfTexturePacker::HandleWorstAreaFit(const RectSizeType& RectSize)
        {
            PlacementResult RetRect(RectType(),false);
            RectSizeType AtlasSize(this->AtlasImage.GetWidth(),this->AtlasImage.GetHeight());
            Integer BestShelfArea = -1;
            Boole BestRotated = false;

            ShelfIterator BestShelf = this->Shelves.end();
            const ShelfIterator EndShelf = this->Shelves.end();
            const ShelfIterator LastShelf = --this->Shelves.end();
            for( ShelfIterator ShelfIt = this->Shelves.begin() ; ShelfIt != EndShelf ; ++ShelfIt )
            {
                Boole IsRotated = this->AllowRotated && (*ShelfIt).ShouldRotateToFit(RectSize,AtlasSize);
                RectSizeType TestSize = ( IsRotated ? RectSize.GetRotated() : RectSize );

                if( (*ShelfIt).FitsOnShelf(TestSize,AtlasSize,ShelfIt == LastShelf) ) {
                    Integer ShelfArea = ( AtlasSize.Width - (*ShelfIt).XPos ) * (*ShelfIt).ShelfHeight;
                    if( ShelfArea > BestShelfArea ) {
                        BestShelf = ShelfIt;
                        BestShelfArea = ShelfArea;
                        BestRotated = IsRotated;
                    }
                }
            }

            if( BestShelf != EndShelf ) {
                RetRect.first = (*BestShelf).AddToShelf( BestRotated ? RectSize.GetRotated() : RectSize );
                RetRect.second = BestRotated;
            }
            return RetRect;
        }

        TexturePacker::PlacementResult ShelfTexturePacker::HandleBestHeightFit(const RectSizeType& RectSize)
        {
            PlacementResult RetRect(RectType(),false);
            RectSizeType AtlasSize(this->AtlasImage.GetWidth(),this->AtlasImage.GetHeight());
            Integer BestHeightDiff = std::numeric_limits<Integer>::max();
            Boole BestRotated = false;

            ShelfIterator BestShelf = this->Shelves.end();
            const ShelfIterator EndShelf = this->Shelves.end();
            const ShelfIterator LastShelf = --this->Shelves.end();
            for( ShelfIterator ShelfIt = this->Shelves.begin() ; ShelfIt != EndShelf ; ++ShelfIt )
            {
                Boole IsRotated = this->AllowRotated && (*ShelfIt).ShouldRotateToFit(RectSize,AtlasSize);
                RectSizeType TestSize = ( IsRotated ? RectSize.GetRotated() : RectSize );

                if( (*ShelfIt).FitsOnShelf(TestSize,AtlasSize,ShelfIt == LastShelf) ) {
                    Integer HeightDiff = std::max( (*ShelfIt).ShelfHeight - TestSize.Height, 0 );
                    if( HeightDiff < BestHeightDiff ) {
                        BestShelf = ShelfIt;
                        BestHeightDiff = HeightDiff;
                        BestRotated = IsRotated;
                    }
                }
            }

            if( BestShelf != EndShelf ) {
                RetRect.first = (*BestShelf).AddToShelf( BestRotated ? RectSize.GetRotated() : RectSize );
                RetRect.second = BestRotated;
            }
            return RetRect;
        }

        TexturePacker::PlacementResult ShelfTexturePacker::HandleBestWidthFit(const RectSizeType& RectSize)
        {
            PlacementResult RetRect(RectType(),false);
            RectSizeType AtlasSize(this->AtlasImage.GetWidth(),this->AtlasImage.GetHeight());
            Integer BestWidthDiff = std::numeric_limits<Integer>::max();
            Boole BestRotated = false;

            ShelfIterator BestShelf = this->Shelves.end();
            const ShelfIterator EndShelf = this->Shelves.end();
            const ShelfIterator LastShelf = --this->Shelves.end();
            for( ShelfIterator ShelfIt = this->Shelves.begin() ; ShelfIt != EndShelf ; ++ShelfIt )
            {
                Boole IsRotated = this->AllowRotated && (*ShelfIt).ShouldRotateToFit(RectSize,AtlasSize);
                RectSizeType TestSize = ( IsRotated ? RectSize.GetRotated() : RectSize );

                if( (*ShelfIt).FitsOnShelf(TestSize,AtlasSize,ShelfIt == LastShelf) ) {
                    Integer WidthDiff = AtlasSize.Width - (*ShelfIt).XPos - TestSize.Width;
                    if( WidthDiff >= 0 && WidthDiff < BestWidthDiff ) {
                        BestShelf = ShelfIt;
                        BestWidthDiff = WidthDiff;
                        BestRotated = IsRotated;
                    }
                }
            }

            if( BestShelf != EndShelf ) {
                RetRect.first = (*BestShelf).AddToShelf( BestRotated ? RectSize.GetRotated() : RectSize );
                RetRect.second = BestRotated;
            }
            return RetRect;

        }

        TexturePacker::PlacementResult ShelfTexturePacker::HandleWorstWidthFit(const RectSizeType& RectSize)
        {
            PlacementResult Result(RectType(),false);
            RectSizeType AtlasSize(this->AtlasImage.GetWidth(),this->AtlasImage.GetHeight());
            Integer BestWidthDiff = -1;
            Boole BestRotated = false;

            ShelfIterator BestShelf = this->Shelves.end();
            const ShelfIterator EndShelf = this->Shelves.end();
            const ShelfIterator LastShelf = --this->Shelves.end();
            for( ShelfIterator ShelfIt = this->Shelves.begin() ; ShelfIt != EndShelf ; ++ShelfIt )
            {
                Boole IsRotated = this->AllowRotated && (*ShelfIt).ShouldRotateToFit(RectSize,AtlasSize);
                RectSizeType TestSize = ( IsRotated ? RectSize.GetRotated() : RectSize );

                if( (*ShelfIt).FitsOnShelf(TestSize,AtlasSize,ShelfIt == LastShelf) ) {
                    Integer WidthDiff = AtlasSize.Width - (*ShelfIt).XPos - TestSize.Width;
                    if( WidthDiff > 0 && WidthDiff > BestWidthDiff ) {
                        BestShelf = ShelfIt;
                        BestWidthDiff = WidthDiff;
                        BestRotated = IsRotated;
                    }
                }
            }

            if( BestShelf != EndShelf ) {
                Result.first = (*BestShelf).AddToShelf( BestRotated ? RectSize.GetRotated() : RectSize );
                Result.second = BestRotated;
            }
            return Result;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Initialization

        void ShelfTexturePacker::Initialize(const Whole TexWidth, const Whole TexHeight)
        {
            this->InitImage(TexWidth,TexHeight);

            this->CurrentY = 0;
            this->Shelves.clear();

            this->StartNewShelf(0);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Insertion

        TexturePacker::PlacementResult ShelfTexturePacker::Insert(const Image* ToInsert)
            { return this->Insert(ToInsert,Graphics::SP_BestAreaFit); }

        TexturePacker::ImageContainer ShelfTexturePacker::Insert(const ImageContainer& ToPack, PackResultHandler* Handler)
            { return this->Insert(ToPack,Handler,Graphics::SP_BestAreaFit); }

        TexturePacker::PlacementResult ShelfTexturePacker::Insert(const Image* ToInsert, const ShelfPlacement RectChoice)
        {
            PlacementResult Result;
            RectSizeType ImageSize = this->GetPaddedRectSize(ToInsert);

            switch( RectChoice )
            {
                case Graphics::SP_NextFit:        Result = this->HandleNextFit(ImageSize);        break;
                case Graphics::SP_FirstFit:       Result = this->HandleFirstFit(ImageSize);       break;
                case Graphics::SP_BestAreaFit:    Result = this->HandleBestAreaFit(ImageSize);    break;
                case Graphics::SP_WorstAreaFit:   Result = this->HandleWorstAreaFit(ImageSize);   break;
                case Graphics::SP_BestHeightFit:  Result = this->HandleBestHeightFit(ImageSize);  break;
                case Graphics::SP_BestWidthFit:   Result = this->HandleBestWidthFit(ImageSize);   break;
                case Graphics::SP_WorstWidthFit:  Result = this->HandleWorstWidthFit(ImageSize);  break;
                default:                          return Result;// Should never get to this unless RectChoice is corrupted.
            }

            if( Result.first.GetArea() <= 0 ) {
                if( this->CanStartNewShelf(ImageSize.Height) ) {
                    this->StartNewShelf(ImageSize.Height);
                    Result.first = this->Shelves.back().AddToShelf(ImageSize);
                    Result.second = false;
                }else{
                    return Result;
                }
            }

            if( Result.second ) {
                this->AddRotatedToAtlas(ToInsert,Result.first);
            }else{
                this->AddUnrotatedToAtlas(ToInsert,Result.first);
            }

            return Result;
        }

        TexturePacker::ImageContainer ShelfTexturePacker::Insert(const ImageContainer& ToPack, PackResultHandler* Handler, const ShelfPlacement RectChoice)
        {
            ConstImageIterator ImageIt = ToPack.begin();
            const ConstImageIterator ImageEnd = ToPack.end();
            while( ImageIt != ImageEnd )
            {
                PlacementResult PlacedRect = this->Insert( (*ImageIt).Img, RectChoice );
                if( PlacedRect.first.GetArea() > 0 ) {
                    Handler->ProcessTexturePlacement( (*ImageIt), PlacedRect );
                    ++ImageIt;
                }else{
                    return ImageContainer(ImageIt,ImageEnd);
                }
            }

            return ImageContainer(ImageIt,ImageEnd);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Query

        const ShelfTexturePacker::ShelfContainer& ShelfTexturePacker::GetShelves() const
            { return this->Shelves; }

        Real ShelfTexturePacker::GetCoverage() const
        {
            Whole UsedSurfaceArea = 0;
            for( ConstShelfIterator ShelfIt = this->Shelves.begin() ; ShelfIt < this->Shelves.end() ; ++ShelfIt )
            {
                for( ConstRectIterator RectIt = (*ShelfIt).UsedRectangles.begin() ; RectIt != (*ShelfIt).UsedRectangles.end() ; ++RectIt )
                    { UsedSurfaceArea += (*RectIt).GetWidth() * (*RectIt).GetHeight(); }
            }
            return static_cast<Real>( UsedSurfaceArea / ( this->AtlasImage.GetWidth() * this->AtlasImage.GetHeight() ) );
        }
    }//Graphics
}//Mezzanine

#endif
