//© Copyright 2010 BlackTopp Studios Inc.
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
#ifndef _uiscrollbar_h
#define _uiscrollbar_h

#include "datatypes.h"
#include "vector2.h"
#include "uiwidget.h"

namespace phys
{
    class UIButton;
    class UIRectangle;
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Scrollbar
        /// @headerfile scrollbar.h
        /// @brief This class is a widget class, to be used alongside any scrollable widget.
        /// @details If you want to have buttons to accompany your scrollbar they'll automatically
        /// have their height match the width of the scrollbar if it's vertical, or their width
        /// match their height of the scrollbar if it's horizontal, based on the dimensions provided. @n
        /// Scrollbars can come in a few styles.  Separate is the typical way to present them with the Up
        /// or Left button being located at the top of left side of the scroller respectively.  Together
        /// is where both scroll buttons are next to each other instead of on opposite sides of the
        /// scroller.
        ///////////////////////////////////////
        class Scrollbar : public Widget
        {
            public:
                enum BarStyle
                {
                    NoButtons,
                    Separate,
                    TogetherUpLeft,
                    TogetherDownRight
                };
            protected:
                String Name;
                UIButton* Scroller;
                UIButton* UpLeftButton;
                UIButton* DownRightButton;
                Real ScrollUpperLimit;
                Real ScrollLowerLimit;
                Real ScrollMinHeight;
                void CreateHorizontalScrollbar(Vector2 Position, Vector2 Size, Scrollbar::BarStyle Style);
                void CreateVerticalScrollbar(Vector2 Position, Vector2 Size, Scrollbar::BarStyle Style);
            public:
                Scrollbar(String& Name, Vector2 Position, Vector2 Size, Scrollbar::BarStyle Style);
                ~Scrollbar();
        };//scrollbar
    }//UI
}//phys

#endif

