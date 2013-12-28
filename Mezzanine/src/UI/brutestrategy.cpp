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
#ifndef _uibrutestrategy_cpp
#define _uibrutestrategy_cpp

#include "UI/brutestrategy.h"
#include "UI/screen.h"
#include "UI/widget.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // BruteStrategy Functors

        ///////////////////////////////////////////////////////////////////////////////
        /// @class MouseHoverCheck
        /// @headerfile brutestategy.cpp
        /// @brief Simple functor for finding which renderable the mouse is hovered over.
        /// @details This is intended for reverse checks.
        ///////////////////////////////////////
        class MouseHoverCheck
        {
        public:
            /// @brief The current mouse position to be checked for hover.
            Vector2 MousePos;
            /// @brief A pointer to the result if one was found, or NULL if it wasn't.
            Widget* Result;

            /// @brief Class constructor.
            /// @param Pos The position on the screen being checked for overlap.
            MouseHoverCheck(const Vector2& Pos) :
                MousePos(Pos),
                Result(NULL)
                {  }
            /// @brief Class destructor.
            ~MouseHoverCheck()
                {  }

            /// @brief Interface needed for processing child widgets of screens and widgets.
            /// @param Wid A pointer to the Widget that will be processed.
            /// @return Returns true if a valid result is found.
            Bool operator()(Widget* Wid)
            {
                // Can it be seen?
                if( Wid->IsVisible() && Wid->GetNumVisibleRenderLayers() ) {
                    // Is it inside?
                    if( Wid->IsInside(this->MousePos) ) {
                        // Has mouse picking been disabled?
                        if( !Wid->GetMousePassthrough() ) {
                            this->Result = static_cast<Widget*>( Wid );
                        }
                    }
                }
                return ( Result != NULL );
            }
        };//MouseHoverCheck

        ///////////////////////////////////////////////////////////////////////////////
        // BruteStrategy Methods

        Widget* BruteStrategy::FindHoveredWidget(const Vector2& MousePos)
        {
            MouseHoverCheck Check(MousePos);
            this->ParentScreen->_ReverseProcessAllChildren(&Check);
            return Check.Result;
        }
    }//UI
}//Mezzanine

#endif
