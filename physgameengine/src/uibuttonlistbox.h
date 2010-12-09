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
#ifndef _uibuttonlistbox_h
#define _uibuttonlistbox_h

#include "uiwidget.h"
namespace phys
{
    namespace UI
    {
        class Rectangle;
        class Scrollbar;
        class Button;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class ButtonListBox
        /// @headerfile uibuttonlistbox.h
        /// @brief This is a widget for displaying a list of buttons in a box.
        /// @details
        ///////////////////////////////////////
        class PHYS_LIB ButtonListBox : public Widget
        {
            protected:
                Rectangle* BoxBack;
                //Scrollbar* VertScroll;
                std::vector<Button*> Selections;
                std::vector<Button*> VisableSelections;
                Real SelectionDist;
                /// @brief For use with widget update/automation.
                virtual void Update(bool Force = false);
            public:
                /// @brief Standard initialization constructor.
                ButtonListBox(String& name, Vector2 Position, Vector2 Size, UILayer* Layer);
                /// @brief Standard destructor.
                ~ButtonListBox();
                /// @brief Checks to see if the current mouse position is over this Button List Box.
                /// @return Returns a bool value, true if the mouse is over this Button List Box, false if it's not.
                virtual bool CheckMouseHover();
                /// @brief Adds a selectable button to the list to be displayed.
                /// @param Size The size of the button.
                /// @param BackgroundSprite Optional, name of the sprite to set as it's background.  Ignored if the string is empty.
                /// @param TextLabel Optional, will create a text button instead of a regular button and set it's text.  Ignored if
                /// the string is empty.  @n @n Note: If a Text button is created, you can't revert it to a regular button and vice
                /// versa.  You'll have to destroy the selection and make a new one.  In the case of having a text button, you can
                /// however still set it's text to an empty string.
                virtual void AddSelection(Vector2 Size, String& BackgroundSprite = "", String &TextLabel = "");
                /// @brief Sets the distance apart(and from the sides of box) the Selections will be from each other.
                /// @details This function expects a relative value to the screen size(0.0 to 1.0).  This value defaults to 0.025.
                /// @param Dist A relative value for the distance to be used when determining the position of Selections.
                virtual void SetSelectionDistance(Real Dist);
                /// @brief Sets the relative position of this Button List Box.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Position A vector2 representing the relative position of this Button List Box.
                virtual void SetPosition(Vector2 Position);
                /// @brief Gets the relative position of this Button List Box.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative position of this Button List Box.
                virtual Vector2 GetPosition();
                /// @brief Sets the pixel position of this Button List Box.
                /// @param Position A vector2 representing the pixel position of this Button List Box.
                virtual void SetActualPosition(Vector2 Position);
                /// @brief Sets the pixel position of this Button List Box.
                /// @return Returns a vector2 representing the pixel position of this Button List Box.
                virtual Vector2 GetActualPosition();
                /// @brief Sets the relative size of this Button List Box.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Size A vector2 representing the relative size of this Button List Box.
                virtual void SetSize(Vector2 Size);
                /// @brief Gets the relative size of this Button List Box.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative size of this Button List Box.
                virtual Vector2 GetSize();
                /// @brief Sets the pixel size of this Button List Box.
                /// @param Size A vector2 representing the pixel size of this Button List Box.
                virtual void SetActualSize(Vector2 Size);
                /// @brief Sets the pixel size of this Button List Box.
                /// @return Returns a vector2 representing the pixel size of this Button List Box.
                virtual Vector2 GetActualSize();
                /// @brief Gets the background of this Button List Box.
                /// @return Returns a pointer to the background of this Button List Box
                virtual Rectangle* GetBoxBack();
        };
    }//UI
}//phys

#endif
