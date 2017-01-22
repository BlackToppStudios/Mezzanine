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
#ifndef _rect_h
#define _rect_h

#include "datatypes.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A convenience class used with RectBase for operations dealing with the rects size.
    ///////////////////////////////////////
    template<typename NumType>
    struct RectSize
    {
        ///////////////////////////////////////////////////////////////////////////////
        // Public Types

        /// @brief Convenience typedef for describing the type you are working with.
        typedef RectSize<NumType> SelfType;
        /// @brief Convenience typedef for the member type in this Rect;
        typedef NumType MemberType;

        ///////////////////////////////////////////////////////////////////////////////
        // Public Data Members

        /// @brief The Width of the Rect.
        NumType Width;
        /// @brief The Height of the Rect.
        NumType Height;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Blank constructor.
        RectSize() :
            Width(0),
            Height(0)
            {  }
        /// @brief Copy constructor.
        /// @param Other The other RectSize to copy from.
        RectSize(const SelfType& Other) = default;
        /// @brief Move constructor.
        /// @param Other The other RectSize to be moved.
        RectSize(SelfType&& Other) = default;
        /// @brief Descriptive constructor.
        /// @param RectWidth The Width of the Rect.
        /// @param RectHeight The Height of the Rect.
        RectSize(const NumType RectWidth, const NumType RectHeight) :
            Width(RectWidth),
            Height(RectHeight)
            {  }
        /// @brief Class destructor.
        ~RectSize() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Sets all values of this RectSize to zero.
        /// @return Returns a reference to this.
        SelfType& SetZero()
        {
            this->Width = 0;
            this->Height = 0;
            return *this;
        }
        /// @brief Sets the value of each member of this RectSize.
        /// @param RectWidth The Width of the Rect.
        /// @param RectHeight The Height of the Rect.
        /// @return Returns a reference to this.
        SelfType& SetValues(const NumType RectWidth, const NumType RectHeight)
        {
            this->Width = RectWidth;
            this->Height = RectHeight;
            return *this;
        }

        /// @brief Swaps the width and height of this RectSize.
        /// @return Returns a reference to this.
        SelfType& Rotate()
        {
            std::swap(this->Width,this->Height);
            return *this;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Query

        /// @brief Gets the total area of this RectSize.
        /// @return Returns the width times height of this Rect.
        NumType GetArea() const
            { return ( this->Width * this->Height ); }
        /// @brief Gets a rotated copy of this RectSize.
        /// @return Returns a new RectSize that has this RectSizes dimensions but rotated.
        SelfType GetRotated() const
            { return SelfType(this->Height,this->Width); }

        /// @brief Checks to see if the two RectSizes are the same size.
        /// @param Other The other RectSize to compare to.
        /// @return Returns true of the two RectSizes are the same size.
        Boole IsSameSize(const SelfType& Other) const
            { return ( this->Width == Other.Width && this->Height == Other.Height ); }
        /// @brief Checks to see if the two RectSizes would be the same size if one were flipped.
        /// @param Other The other RectSize to compare to.
        /// @return Returns true of the two RectSizes are the same size after flipping one.
        Boole IsSameSizeRotated(const SelfType& Other) const
            { return ( this->Width == Other.Height && this->Height == Other.Width ); }
        /// @brief Checks to see if this RectSize is small enough to fit inside another RectSize.
        /// @param Other The other RectSize to compare to.
        /// @return Returns true if this RectSize would fit inside the other RectSize, false otherwise.
        Boole FitsInside(const SelfType& Other) const
            { return ( this->Width <= Other.Width && this->Height <= Other.Height ); }
        /// @brief Checks to see if this RectSize is small enough to fit inside another RectSize if it were flipped on it's side.
        /// @param Other The other RectSize to compare to.
        /// @return Returns true if this RectSize would fit inside the other RectSize after being flipped, false otherwise.
        Boole FitsInsideRotated(const SelfType& Other) const
            { return ( this->Width <= Other.Height && this->Height <= Other.Width ); }

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment Operator.
        /// @param Other The other RectSize to copy data from.
        SelfType& operator=(const SelfType& Other) = default;

        /// @brief Equality Operator.
        /// @param Other The other RectSize to compare with.
        /// @return Returns true if both RectSizes express the same size.
        Boole operator==(const SelfType& Other)
            { return ( this->Width == Other.Width && this->Height == Other.Height ); }
        /// @brief Inequality Operator.
        /// @param Other The other RectSize to compare with.
        /// @return Returns true if the two RectSizes express different sizes.
        Boole operator!=(const SelfType& Other)
            { return ( this->Width != Other.Width || this->Height != Other.Height ); }
    };//RectSize

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A class for tracking a rectangle by tracking the four edges, rather than a corner and Width/height.
    ///////////////////////////////////////
    template<typename NumType>
    class RectBase
    {
    public:
        ///////////////////////////////////////////////////////////////////////////////
        // Public Types

        /// @brief Convenience typedef for describing the type you are working with.
        typedef RectBase<NumType> SelfType;
        /// @brief Convenience typedef for the RectSize type used in this Rect.
        typedef RectSize<NumType> SizeType;
        /// @brief Convenience typedef for the member type in this Rect;
        typedef NumType MemberType;

        ///////////////////////////////////////////////////////////////////////////////
        // Public Data Members

        /// @brief The Left edge of the rect.
        NumType Left;
        /// @brief The Top edge of the rect.
        NumType Top;
        /// @brief The Right edge of the rect.
        NumType Right;
        /// @brief The Bottom edge of the rect.
        NumType Bottom;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Blank constructor.
        RectBase() :
            Left(0),
            Top(0),
            Right(0),
            Bottom(0)
            {  }
        /// @brief Copy constructor.
        /// @param Other The other RectBase to copy from.
        RectBase(const SelfType& Other) = default;
        /// @brief Move constructor.
        /// @param Other The other RectBase to be moved.
        RectBase(SelfType&& Other) = default;
        /// @brief Descriptive constructor.
        /// @param RectLeft The Left edge of the Rect.
        /// @param RectTop The Top edge of the Rect.
        /// @param RectRight The Right edge of the Rect.
        /// @param RectBottom The Bottom edge of the Rect.
        RectBase(const NumType RectLeft, const NumType RectTop, const NumType RectRight, const NumType RectBottom) :
            Left(RectLeft),
            Top(RectTop),
            Right(RectRight),
            Bottom(RectBottom)
            {  }
        /// @brief RectSize constructor.
        /// @param RectLeft The Left edge of the Rect.
        /// @param RectTop The Top edge of the Rect.
        /// @param Size The RectSize containing the width and height of the Rect.
        RectBase(const NumType RectLeft, const NumType RectTop, const SizeType Size) :
            Left(RectLeft),
            Top(RectTop),
            Right(RectLeft + Size.Width),
            Bottom(RectTop + Size.Height)
            {  }
        /// @brief Class destructor.
        ~RectBase() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Sets all values of this Rect to zero.
        /// @return Returns a reference to this.
        SelfType& SetZero()
        {
            this->Left = 0;
            this->Top = 0;
            this->Right = 0;
            this->Bottom = 0;
            return *this;
        }
        /// @brief Sets the value of each member of this Rect.
        /// @param RectLeft The Left edge of the Rect.
        /// @param RectTop The Top edge of the Rect.
        /// @param RectRight The Right edge of the Rect.
        /// @param RectBottom The Bottom edge of the Rect.
        /// @return Returns a reference to this.
        SelfType& SetValues(const NumType RectLeft, const NumType RectTop, const NumType RectRight, const NumType RectBottom)
        {
            this->Left = RectLeft;
            this->Top = RectTop;
            this->Right = RectRight;
            this->Bottom = RectBottom;
            return *this;
        }
        /// @brief Sets the value of each member of this Rect.
        /// @param RectLeft The Left edge of the Rect.
        /// @param RectTop The Top edge of the Rect.
        /// @param Size The RectSize containing the width and height of the Rect.
        /// @return Returns a reference to this.
        SelfType& SetValuesBySize(const NumType RectLeft, const NumType RectTop, const SizeType Size)
        {
            this->Left = RectLeft;
            this->Top = RectTop;
            this->Right = RectLeft + Size.Width;
            this->Bottom = RectTop + Size.Height;
            return *this;
        }
        /// @brief Sets the value of each member of this Rect.
        /// @param RectLeft The Left edge of the Rect.
        /// @param RectTop The Top edge of the Rect.
        /// @param RectWidth The Width of the Rect.
        /// @param RectHeight The Height of the Rect.
        /// @return Returns a reference to this.
        SelfType& SetValuesBySize(const NumType RectLeft, const NumType RectTop, const NumType RectWidth, const NumType RectHeight)
        {
            this->Left = RectLeft;
            this->Top = RectTop;
            this->Right = RectLeft + RectWidth;
            this->Bottom = RectTop + RectHeight;
            return *this;
        }

        /// @brief Clamps the position and size of this Rect to the provided size.
        /// @param Width The width of the area this Rect will be placed in.
        /// @param Height The height of the area this Rect will be placed in.
        /// @return Returns a reference to this.
        SelfType& ClampToSize(const NumType Width, const NumType Height)
        {
            this->Left = std::max(NumType(0),this->Left);
            this->Top = std::max(NumType(0),this->Top);
            this->Right = std::min(Width,this->Right);
            this->Bottom = std::min(Height,this->Bottom);
            return *this;
        }
        /// @brief Rotates this Rect while preserving it's position.
        /// @return Returns a reference to this.
        SelfType& Rotate()
        {
            NumType Width = this->GetWidth();
            NumType Height = this->GetHeight();
            this->Right = this->Left + Height;
            this->Bottom = this->Top + Width;
            return *this;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Query

        /// @brief Gets the width of this Rect.
        /// @return Returns the width of this Rect.
        NumType GetWidth() const
            { return this->Right - this->Left; }
        /// @brief Gets the height of this Rect.
        /// @return Returns the height of this Rect.
        NumType GetHeight() const
            { return this->Bottom - this->Top; }

        /// @brief Gets the size of this Rect.
        /// @return Returns a RectSize containing the width and height of this Rect.
        SizeType GetSize() const
            { return SizeType(this->GetWidth(),this->GetHeight()); }

        /// @brief Gets the total area of this Rect.
        /// @return Returns the width times height of this Rect.
        NumType GetArea() const
            { return ( this->GetWidth() * this->GetHeight() ); }

        /// @brief Checks to see if another Rect overlaps with this Rect.
        /// @param Other The other Rect to be compared with.
        /// @return Returns true if the two Rects overlap, false otherwise.
        Boole Overlaps(const SelfType& Other) const
        {
            Boole XOverlap = ( this->Left >= Other.Left && this->Left <= Other.Right ) ||
                             ( Other.Left >= this->Left && Other.Left <= this->Right );
            Boole YOverlap = ( this->Top >= Other.Top && this->Top <= Other.Bottom ) ||
                             ( Other.Top >= this->Top && Other.Top <= this->Bottom );
            return ( XOverlap && YOverlap );
        }
        /// @brief Checks to see if this Rects bounds are entirely within another Rect.
        /// @param Other The other Rect to be compared with.
        /// @return Returns true if this Rect is completely engulfed by the specified Rect, false otherwise.
        Boole IsContainedIn(const SelfType& Other) const
        {
            return ( this->Left >= Other.Left && this->Right <= Other.Right &&
                     this->Top >= Other.Top && this->Bottom <= Other.Bottom );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment Operator.
        /// @param Other The other Rect to copy data from.
        SelfType& operator=(const SelfType& Other) = default;

        /// @brief Equality Operator.
        /// @param Other The other Rect to compare with.
        /// @return Returns true if both Rects express the same bounds.
        Boole operator==(const SelfType& Other)
            { return ( this->Left == Other.Left && this->Top == Other.Top && this->Right == Other.Right && this->Bottom == Other.Bottom ); }
        /// @brief Inequality Operator.
        /// @param Other The other Rect to compare with.
        /// @return Returns true if the two Rects express different bounds.
        Boole operator!=(const SelfType& Other)
            { return ( this->Left != Other.Left || this->Top != Other.Top || this->Right != Other.Right || this->Bottom != Other.Bottom ); }
    };//RectBase

    /// @brief Convenience type for a Rect using Whole numbers.
    typedef RectBase<Whole> WholeRect;
    /// @brief Convenience type for a Rect using Integer numbers.
    typedef RectBase<Integer> IntegerRect;
    /// @brief Convenience type for a Rect using Real numbers.
    typedef RectBase<Real> RealRect;
}//Mezzanine

#endif
