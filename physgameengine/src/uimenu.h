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
#ifndef _uimenu_h
#define _uimenu_h

#include "uiwidget.h"

#include <stack>
#include <vector>

namespace phys
{
    namespace UI
    {
        class MenuWindow;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Menu
        /// @headerfile uimenu.h
        /// @brief This class is a control mechanism for multiple windows in a heirarchy.
        /// @details This class controls the presentation and order of different windows, useful
        /// for creating menu systems, be it a game main menu, or in-game menu. @n @n
        /// Also it should be noted that since this is just a control system for other classes, it
        /// doesn't have a position or size like other classes.  Instead when you call those functions
        /// to set or get the position or size, you'll be working with the current top level window.
        ///////////////////////////////////////
        class PHYS_LIB Menu : public Widget
        {
            protected:
                MenuWindow* RootWindow;
                std::vector<MenuWindow*> MenuStack;
                bool HideSubWindows;
                /// @brief For use with widget update/automation.
                virtual void Update(bool Force = false);
            public:
                /// @brief Standard initialization constructor.
                /// @param name The name of the window.
                /// @param Position The position of the window.
                /// @param Size The size of the window.
                /// @param Layer The parent layer this window belongs to.
                Menu(ConstString name, const Vector2 Position, const Vector2 Size, Layer* PLayer);
                /// @brief Standard destructor.
                ~Menu();
                /// @brief Sets the visibility of this menu.
                /// @param visible Bool determining whether or not this menu should be visible.
                virtual void SetVisible(bool visible);
                /// @brief Gets the visibility of this menu.
                /// @return Returns a bool representing the visibility of this menu.
                virtual bool IsVisible();
                /// @brief Forces this menu to be shown.
                virtual void Show();
                /// @brief Forces this menu to hide.
                virtual void Hide();
                /// @brief Sets autohiding of sub-windows.
                /// @param AutoHide Whether or not sub-windows should auto-hide when a new window is added to the stack.
                virtual void SetAutoHideSubWindows(bool AutoHide);
                /// @brief Checks to see if the current mouse position is over this menu.
                /// @return Returns a bool value, true if the mouse is over this menu, false if it's not.
                virtual bool CheckMouseHover();
                /// @brief Sets the relative position of this menu.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Position A vector2 representing the relative position of this menu.
                virtual void SetPosition(const Vector2 Position);
                /// @brief Gets the relative position of this menu.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative position of this menu.
                virtual Vector2 GetPosition();
                /// @brief Sets the pixel position of this menu.
                /// @param Position A vector2 representing the pixel position of this menu.
                virtual void SetActualPosition(const Vector2 Position);
                /// @brief Sets the pixel position of this menu.
                /// @return Returns a vector2 representing the pixel position of this menu.
                virtual Vector2 GetActualPosition();
                /// @brief Sets the relative size of this menu.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Size A vector2 representing the relative size of this menu.
                virtual void SetSize(const Vector2 Size);
                /// @brief Gets the relative size of this menu.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative size of this menu.
                virtual Vector2 GetSize();
                /// @brief Sets the pixel size of this menu.
                /// @param Size A vector2 representing the pixel size of this menu.
                virtual void SetActualSize(const Vector2 Size);
                /// @brief Sets the pixel size of this menu.
                /// @return Returns a vector2 representing the pixel size of this menu.
                virtual Vector2 GetActualSize();
                /// @brief Gets the Root window of this menu.
                /// @return Returns a pointer to the Root window of this menu.
                virtual MenuWindow* GetRootWindow();
                /// @brief Gets the current window at the top of the stack.
                /// @return Returns a pointer to the window currently at the top of the stack.
                virtual MenuWindow* GetTopWindow();
        };//Menu
    }//UI
}//phys

#endif
