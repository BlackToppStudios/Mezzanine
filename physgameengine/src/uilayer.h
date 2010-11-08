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
#ifndef _uilayer_h
#define _uilayer_h

#include "datatypes.h"

#include <map>

namespace Gorilla
{
    class Screen;
    class Layer;
    class Rectangle;
}

namespace phys
{
    class UIButton;
    class UIManager;
    class UIRectangle;
    class UIItemList;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class UIButton
    /// @headerfile uibutton.h
    /// @brief This class is a helper class, specifically for use with widgets.
    /// @details A layer is a container for widgets and other UI elements that are visable.
    ///////////////////////////////////////
    class UILayer
    {
        protected:
            Gorilla::Layer* GorillaLayer;
            Gorilla::Screen* Parent;
            UIManager* Manager;
            String Name;
            std::vector<UIButton*> Buttons;
            std::vector<UIRectangle*> Rectangles;
        public:
            /// @brief Internal constructor
            /// @param GScreen The Gorilla Layer this Layer is based on.
            /// @param manager Pointer to the manager this layer belongs to..
            UILayer(const String& name, Gorilla::Layer* GLayer, Gorilla::Screen* GScreen, UIManager* manager);
            /// @brief Class destructor.
            ~UILayer();
            /// @brief Gets the name of this layer.
            /// @return Returns a string containing the name of this layer.
            String& GetName();
            /// @brief Sets the layers' visability.
            /// @param Visable A bool representing the visability of the layer.
            void SetVisible(bool Visible);
            /// @brief Gets the layers' visability.
            /// @return Returns a bool representing the visability of the layer.
            bool GetVisible();
            /// @brief Forces the layer to be shown.
            void Show();
            /// @brief Forces the layer to hide.
            void Hide();
            /// @brief Creates a button within this layer.
            /// @return Returns a pointer to the created button.
            /// @param X The position on screen in pixels for the left side of the button.
            /// @param Y The position on screen in pixels for the top side of the button.
            /// @param Height The height of the button in pixels.
            /// @param Width The width of the button in pixels.
            /// @param Glyph One of the glyphs specified in your gorilla file.  Must be valid.
            /// @param Text Any text you want printed on the button.
            UIButton* CreateButton(String& Name, Real X, Real Y, Real Width, Real Height, Whole Glyph, String Text);
            /// @brief Gets an already created button by name.
            /// @return Returns a pointer to the button of the specified name.
            UIButton* GetButton(String& Name);
            /// @brief Gets an already created button by index.
            /// @return Returns a pointer to the button at the specified index.
            UIButton* GetButton(Whole Index);
            /// @brief Gets the number of buttons created and stored in this class.
            /// @return Returns the number of buttons this class is storing.
            Whole GetNumButtons();
            /// @brief Creates a rectangle within this layer.
            /// @details Rectangles are innately put behind all captions, so z-order is not necessary.
            /// @param X The position on screen in pixels for the left side of the rectangle.
            /// @param Y The position on screen in pixels for the top side of the rectangle.
            /// @param Height The height of the rectangle in pixels.
            /// @param Width The width of the rectangle in pixels.
            UIRectangle* CreateRectangle(Real X, Real Y, Real Width, Real Height);
            /// @brief Gets the button the mouse is over if any.
            /// @details This function searches only the buttons contained in this layer.
            /// @return Returns the button the mouse is over, or NULL if there are none.
            UIButton* GetButtonMouseIsOver();
            /// @internal
            /// @brief Gets the internal gorilla layer pointer.
            Gorilla::Layer* GetGorillaLayer();
    };//uilayer
}//phys

#endif
