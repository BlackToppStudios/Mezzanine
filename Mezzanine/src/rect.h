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
#ifndef _rect_h
#define _rect_h

#include "datatypes.h"

namespace Mezzanine
{
    /// @brief A class for tracking a rectangle by tracking two corners Top, Left and
    /// Right, Bottom.
    template<typename NumType>
    class RectBase
    {
    public:
        ///////////////////////////////////////////////////////////////////////////////
        // Public Types

        /// @brief Convenience typedef for describing the type you are working with.
        typedef RectBase<NumType> SelfType;

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
        /// @brief Class destructor.
        ~RectBase()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Sets all values of this Rect to zero.
        void SetZero()
        {
            this->Left = 0;
            this->Top = 0;
            this->Right = 0;
            this->Bottom = 0;
        }
        /// @brief Sets the value of each member of this Rect.
        /// @param RectLeft The Left edge of the Rect.
        /// @param RectTop The Top edge of the Rect.
        /// @param RectRight The Right edge of the Rect.
        /// @param RectBottom The Bottom edge of the Rect.
        void SetValues(const NumType RectLeft, const NumType RectTop, const NumType RectRight, const NumType RectBottom)
        {
            this->Left = RectLeft;
            this->Top = RectTop;
            this->Right = RectRight;
            this->Bottom = RectBottom;
        }

        /// @brief Clamps the position and size of this Rect to the provided size.
        /// @param Width The width of the area this Rect will be placed in.
        /// @param Height The height of the area this Rect will be placed in.
        void ClampToSize(const NumType Width, const NumType Height)
        {
            this->Left = std::max(NumType(0),this->Left);
            this->Top = std::max(NumType(0),this->Top);
            this->Right = std::min(Width,this->Right);
            this->Bottom = std::min(Height,this->Bottom);
        }

        /// @brief Gets the width of this Rect.
        /// @return Returns the width of this Rect.
        NumType GetWidth() const
            { return this->Right - this->Left; }
        /// @brief Gets the height of this Rect.
        /// @return Returns the height of this Rect.
        NumType GetHeight() const
            { return this->Bottom - this->Top; }

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment Operator.
        /// @param Other The other Rect to copy data from.
        void operator=(const SelfType& Other)
        {
            this->Left = Other.Left;
            this->Top = Other.Top;
            this->Right = Other.Right;
            this->Bottom = Other.Bottom;
        }

        /// @brief Equality Operator.
        /// @param Other The other Rect to compare with.
        /// @return Returns true if both Rects express the same area.
        Boole operator==(const SelfType& Other)
            { return ( this->Left == Other.Left && this->Top == Other.Top && this->Right == Other.Right && this->Bottom == Other.Bottom ); }
        /// @brief Inequality Operator.
        /// @param Other The other Rect to compare with.
        /// @return Returns true if the two Rects express different areas.
        Boole operator!=(const SelfType& Other)
            { return ( this->Left != Other.Left || this->Top != Other.Top || this->Right != Other.Right || this->Bottom != Other.Bottom ); }
    };//RectBase

    /// @brief Convenience type for a Rect using Whole numbers.
    typedef RectBase<Whole> WholeRect;
    /// @brief Convenience type for a Rect using Real numbers.
    typedef RectBase<Real> RealRect;
}//Mezzanine

#endif
