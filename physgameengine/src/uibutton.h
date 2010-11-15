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
#ifndef _uibutton_h
#define _uibutton_h

#include "datatypes.h"
#include "enumerations.h"
#include "colourvalue.h"
#include "vector2.h"

namespace Gorilla
{
    class Caption;
    class Layer;
    class Rectangle;
    class Sprite;
}

namespace phys
{
    class UIManager;
    class UILayer;
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Button
        /// @headerfile uibutton.h
        /// @brief This class is a helper class, specifically for use as a button.
        /// @details Unlike rectangles and captions, this class can be interacted with by clicking.
        /// It is important to understand what you want your space to do when selecting the class to use.
        ///////////////////////////////////////
        class Button
        {
            protected:
                Gorilla::Caption* GorillaButton;
                Gorilla::Rectangle* GorillaRectangle;
                UILayer* Parent;
                UIManager* Manager;
                Gorilla::Sprite* NormalSprite;
                Gorilla::Sprite* HoveredSprite;
                bool MouseHover;
                Vector2 RelPosition;
                Vector2 RelSize;
                String Name;
            public:
                /// @brief Internal constructor
                /// @param GButton The Gorilla Caption this button is based on.
                /// @param manager Pointer to the manager that created this button.
                Button(String& name, Gorilla::Caption* GButton, UILayer* Layer);
                /// @brief Class destructor.
                ~Button();
                /// @brief Gets the name of this button.
                /// @return Returns a string containing the name of this button.
                String& GetName();
                /// @brief Sets the text displayed within the button.
                /// @param Text The text to be displayed.
                void SetText(String& Text);
                /// @brief Gets the text displayed within the button.
                /// @return Returns the text being displayed.
                String GetText();
                /// @brief Determines whether the mouse is over this button.
                /// @return Returns a bool indicating whether the mouse is over this button.
                bool CheckMouseHover();
                /// @brief Gets the stored value of whether or not the mouse is over the button.
                /// @details This function does not perform any checks.  If you want to do a manual check, call CheckMouseHover().
                /// @return Returns the stored value of whether or not the mouse is over the button.
                bool GetMouseHover();
                /// @brief Sets the background colour of the button.
                /// @param Colour A colour value representing the colour to be set.
                void SetBackgroundColour(ColourValue& Colour);
                /// @brief Sets the background image(if provided in the atlas) of the button.
                /// @param Name The name of the sprite to set as the background.
                void SetBackgroundSprite(const String& Name);
                /// @brief Sets an alternate background image that will be applied when the mouse is over this button.
                /// @param Name The name of the sprite to set as the alternate background.
                void SetHoveredSprite(const String& Name);
                /// @brief Enables a border and sets it's colour.
                /// @param Colour A colour value representing the colour to be set.
                void SetBorder(Real Width, ColourValue& Colour);
                /// @brief Aligns the text of the button.
                /// @param Align The enum value representing the horizontal alignment to be set.
                void HorizontallyAlign(UI::TextHorizontalAlign Align);
                /// @brief Aligns the text of the button.
                /// @param Align The enum value representing the vertical alignment to be set.
                void VerticallyAlign(UI::TextVerticalAlign Align);
                /// @brief Sets the relative top left position of this button.
                /// @param Position A Vector2 representing the location of this button.
                void SetPosition(Vector2 Position);
                /// @brief Gets the relative top left position of this button.
                /// @return Returns a Vector2 representing the location of this button.
                Vector2 GetPosition();
                /// @brief Sets the top left position of this button in pixels.
                /// @param Position A Vector2 representing the location of this button.
                void SetActualPosition(Vector2 Position);
                /// @brief Gets the top left position of this button in pixels.
                /// @return Returns a Vector2 representing the location of this button.
                Vector2 GetActualPosition();
                /// @brief Sets the relative size of this button.
                /// @param Size A vector2 representing the size of this button.
                void SetSize(Vector2 Size);
                /// @brief Gets the relative size of this button.
                /// @return Returns a vector2 representing the size of this button.
                Vector2 GetSize();
                /// @brief Sets the size of this button in pixels.
                /// @param Size A vector2 representing the size of this button.
                void SetActualSize(Vector2 Size);
                /// @brief Gets the size of this button in pixels.
                /// @return Returns a vector2 representing the size of this button.
                Vector2 GetActualSize();
        };//button
    }//UI
}//phys

#endif
