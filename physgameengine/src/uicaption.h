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
#ifndef _uicaption_h
#define _uicaption_h

#include "crossplatformexport.h"
#include "colourvalue.h"
#include "datatypes.h"
#include "enumerations.h"
#include "vector2.h"

namespace Gorilla
{
    class Caption;
    class Rectangle;
}

namespace phys
{
    class UIManager;
    class UILayer;
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Caption
        /// @headerfile uicaption.h
        /// @brief This class is a helper class, specifically for use with text.
        /// @details Unlike a button, this class cannot be interacted with by clicking.
        /// It is important to understand what you want your space to do when selecting the class to use.
        ///////////////////////////////////////
        class PHYS_LIB Caption
        {
            protected:
                Gorilla::Caption* GorillaCaption;
                Gorilla::Rectangle* GorillaRectangle;
                UILayer* Parent;
                UIManager* Manager;
                Vector2 RelPosition;
                Vector2 RelSize;
                String Name;
            public:
                /// @brief Internal constructor
                /// @param Name The name of this caption.
                /// @param Position The top left position of the button.
                /// @param Size The size of the Button.
                /// @param Glyph One of the glyphs specified in your gorilla file.  Must be valid.
                /// @param Text Any text you want printed on the caption.
                /// @param Layer Pointer to the layer that created this caption.
                Caption(String& name, Vector2 Position, Vector2 Size, Whole Glyph, String Text, UILayer* Layer);
                /// @brief Class destructor.
                ~Caption();
                /// @brief Gets the name of this caption.
                /// @return Returns a string containing the name of this caption.
                String& GetName();
                /// @brief Sets the text displayed within the caption.
                /// @param Text The text to be displayed.
                void SetText(String& Text);
                /// @brief Gets the text displayed within the caption.
                /// @return Returns the text being displayed.
                String GetText();
                /// @brief Sets the background colour of the caption.
                /// @param Colour A colour value representing the colour to be set.
                void SetBackgroundColour(ColourValue& Colour);
                /// @brief Sets the background image(if provided in the atlas) of the caption.
                /// @param Name The name of the sprite to set as the background.
                void SetBackgroundSprite(const String& Name);
                /// @brief Aligns the text of the caption.
                /// @param Align The enum value representing the horizontal alignment to be set.
                void HorizontallyAlign(UI::TextHorizontalAlign Align);
                /// @brief Aligns the text of the caption.
                /// @param Align The enum value representing the vertical alignment to be set.
                void VerticallyAlign(UI::TextVerticalAlign Align);
                /// @brief Sets the relative top left position of this caption.
                /// @param Position A Vector2 representing the location of this caption.
                void SetPosition(Vector2 Position);
                /// @brief Gets the relative top left position of this caption.
                /// @return Returns a Vector2 representing the location of this caption.
                Vector2 GetPosition();
                /// @brief Sets the top left position of this caption in pixels.
                /// @param Position A Vector2 representing the location of this caption.
                void SetActualPosition(Vector2 Position);
                /// @brief Gets the top left position of this caption in pixels.
                /// @return Returns a Vector2 representing the location of this caption.
                Vector2 GetActualPosition();
                /// @brief Sets the relative size of this caption.
                /// @param Size A vector2 representing the size of this caption.
                void SetSize(Vector2 Size);
                /// @brief Gets the relative size of this caption.
                /// @return Returns a vector2 representing the size of this caption.
                Vector2 GetSize();
                /// @brief Sets the size of this caption in pixels.
                /// @param Size A vector2 representing the size of this caption.
                void SetActualSize(Vector2 Size);
                /// @brief Gets the size of this caption in pixels.
                /// @return Returns a vector2 representing the size of this caption.
                Vector2 GetActualSize();
                /// @brief Sets the priority this button should be rendered with.
                /// @details The default value for this is Medium.
                /// @param Priority The priority level to be used when rendering this button.
                void SetRenderPriority(UI::RenderPriority Priority);
                /// @brief Gets the priority this button should be rendered with.
                /// @return Returns an enum value representing this button's priority level.
                UI::RenderPriority GetRenderPriority();
        };//caption
    }//UI
}//phys

#endif
