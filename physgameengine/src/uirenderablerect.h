//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#ifndef _uirenderablerect_h
#define _uirenderablerect_h

#include "vector2.h"

namespace phys
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class RenderableRect
        /// @headerfile uirenderablerect.h
        /// @brief This class contains all the utilities necessary for capturing input.
        /// @details This is commonly used for Text-based widgets, such as TextBox's, Spinners, and InputBox's.
        ///////////////////////////////////////
        struct PHYS_LIB RenderableRect
        {
            Vector2 Position;
            Vector2 Size;
            bool Relative;

            /// @brief Detailed Constructor.
            /// @details Sets all the data of the struct.
            /// @param Position The position of the Renderable's Rect.
            /// @param Size The size of the Renderable's Rect.
            /// @param Relative Whether or not this Rect is using relative(0-1) or absolute units(Pixels).
            RenderableRect(const Vector2& Position, const Vector2& Size, bool Relative)
            {
                this->Position = Position;
                this->Size = Size;
                this->Relative = Relative;
            };
            /// @brief No Initialization Constructor.
            /// @details Sets all data members to zero's.
            RenderableRect()
            {
                this->Position = Vector2();
                this->Size = Vector2();
                this->Relative = false;
            };
            /// @brief Class destructor.
            virtual ~RenderableRect() {};
            /// @brief Assignment Operator.
            /// @details Copys the contents of a RenderableRect to another.
            /// @param Other The other RenderableRect to copy from.
            virtual RenderableRect& operator=(const RenderableRect& Other)
            {
                this->Position = Other.Position;
                this->Size = Other.Size;
                this->Relative = Other.Relative;
                return *this;
            };
            /// @brief Comparison Operator.
            /// @details Checks to see if the two RenderableRects are equal.
            /// @param Other The other RenderableRect to compare against.
            virtual bool operator==(const RenderableRect& Other)
            {
                if(this->Position != Other.Position) return false;
                if(this->Size != Other.Size) return false;
                if(this->Relative != Other.Relative) return false;
                return true;
            };
        };//renderablerect
    }//ui
}//phys

#endif
