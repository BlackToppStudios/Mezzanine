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
#ifndef _uimenuwindow_h
#define _uimenuwindow_h

#include "uiwindow.h"

#include <vector>

namespace phys
{
    namespace UI
    {
        class UIlayer;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class MenuWindow
        /// @headerfile uimenuwindow.h
        /// @brief This class is an extended version of the window class for use exclusively with the menu widget.
        /// @details
        ///////////////////////////////////////
        class MenuWindow : public Window
        {
            protected:
                MenuWindow* ParentWindow;
                std::vector<MenuWindow*> ChildWindows;
            public:
                /// @brief Standard initialization constructor.
                /// @param Name The name of the window.
                /// @param Position The position of the window.
                /// @param Size The size of the window.
                /// @param Layer The parent layer this window belongs to.
                MenuWindow(ConstString& Name, Vector2 Position, Vector2 Size, UILayer* Layer);
                /// @brief Standard destructor.
                ~MenuWindow();
                /// @brief Gets the parent window of this window.
                /// @return Returns a pointer to the parent window of this window.
                MenuWindow* GetParentWindow();
                /// @brief Adds a child window to this window.
                /// @param Name The name of the window.
                /// @param Position The position of the window.
                /// @param Size The size of the window.
                MenuWindow* CreateChildMenuWindow(ConstString& Name, Vector2 Position, Vector2 Size);
                /// @brief Gets an already created menu window by name.
                /// @return Returns a pointer to the menu window of the specified name.
                MenuWindow* GetMenuWindow(ConstString& Name);
                /// @brief Gets an already created menu window by index.
                /// @return Returns a pointer to the menu window at the specified index.
                MenuWindow* GetMenuWindow(Whole Index);
                /// @brief Gets the number of menu windows created and stored in this class.
                /// @return Returns the number of menu windows this class is storing.
                Whole GetNumMenuWindows();
                /// @brief Destroys a menu window.
                /// @param ToBeDestroyed Pointer to the menu window you want destroyed.
                void DestroyChildMenuWindow(MenuWindow* ToBeDestroyed);
        };
    }
}

#endif
