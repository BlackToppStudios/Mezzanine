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
#ifndef _uimousehoverstrategy_h
#define _uimousehoverstrategy_h

#include "vector2.h"

namespace Mezzanine
{
    namespace UI
    {
        class Screen;
        class Widget;
        class QuadRenderable;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class MouseHoverStrategy
        /// @headerfile mousehoverstrategy.h
        /// @brief This is the base class for a method of finding which quad the mouse is hovered over.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB MouseHoverStrategy
        {
        protected:
            /// @internal
            /// @brief A pointer to the screen this strategy is being used by.
            Screen* ParentScreen;
            /// @internal
            /// @brief Stores whether or not the parent screen has been updated since the last time this strategys cache was updated.
            bool ScreenDirty;
        public:
            /// @brief Class constructor.
            MouseHoverStrategy() :
                ParentScreen(NULL),
                ScreenDirty(true)
                {  }
            /// @brief Class destructor.
            virtual ~MouseHoverStrategy()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Finds the hovered quad for the parent screen.
            /// @param MousePos The current mouse position.
            /// @return Returns a pointer to the hovered quad, or NULL if none.
            virtual Widget* FindHoveredWidget(const Vector2& MousePos) = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Sets the parent screen.
            /// @param Parent The parent screen this strategy is finding the hovered quad for.
            void _SetScreen(Screen* Parent)
                { this->ParentScreen = Parent; }
            /// @internal
            /// @brief Notifies this that it's parent screen was altered.
            void _NotifyScreenDirty()
                { this->ScreenDirty = true; }
        };//MouseHoverStrategy
    }//UI
}//Mezzanine

#endif
