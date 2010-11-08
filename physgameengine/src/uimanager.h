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
#ifndef _uimanager_h
#define _uimanager_h

#include "managerbase.h"
#include "datatypes.h"

#include <map>

namespace Gorilla
{
    class Silverback;
    class Layer;
    class Screen;
}

namespace phys
{
    class UIScreen;
    class UILayer;
    class UIButton;
    class Widget;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class UIManager
    /// @headerfile uimanager.h
    /// @brief This class is responsible for any and all user interactions with the User interface/HUD.
    /// @details Currently, you have to create the UI/HUD in code.  Font and sprite data is loaded through a premade Gorilla file(*.gorilla).
    ///////////////////////////////////////
    class UIManager : public ManagerBase
    {
        protected:
            /// @brief Pointer for the Gorilla core class, where this manager gets it's functionality.
            Gorilla::Silverback* Silver;
            std::vector< UIScreen* > Screens;
            UIButton* HoveredButton;
            Widget* HoveredWidget;
        public:
            /// @brief Class Constructor.
            /// @details Standard class initialization constructor.
            UIManager();
            /// @brief Class Destructor.
            /// @details The class destructor.
            ~UIManager();
            /// @brief Inherited from ManagerBase.
            void Initialize();
            /// @brief Inherited from ManagerBase.
            void DoMainLoopItems();
            /// @brief Loads a Gorilla file for use with this manager.
            /// @param Name The name of the file to be loaded, not including the extension.
            void LoadGorilla(const String& Name);
            /// @brief Gets the button the mouse is hovering over.
            /// @details This check will look through both standalone buttons and widget buttons.
            /// @return Returns a pointer to the button, or NULL if it's not over any visable buttons.
            UIButton* GetHoveredButton();
            /// @brief Gets the Widget the mouse is hovering over.
            /// @details If the button found during button checks belongs to a widget, this will get that widget.
            /// @return Returns a pointer to the button, or NULL if it's not over any visable buttons.
            Widget* GetHoveredWidget();
            /// @brief Creates an internal HUD screen.
            /// @details Screens are the base set of renderable UI you can use, allowing you to switch entire sets of UI's
            /// on the fly if needed.  For performance reasons you should always keep the number of screens you create to a minimum.
            /// @param Screen The name to be given to the screen.
            /// @param Atlas The name of a previously loaded Gorilla file to be used with this screen.
            /// @param Viewport The name of the viewport to create this screen in.
            UIScreen* CreateScreen(const String& Screen, const String& Atlas, const String& Viewport="DefaultViewport");
            /// @brief Gets an already created screen by name.
            /// @return Returns a pointer to the screen of the specified name.
            UIScreen* GetScreen(const String& Name);
            /// @brief Gets an already created screen by index.
            /// @return Returns a pointer to the screen at the specified index.
            UIScreen* GetScreen(Whole Index);
            /// @brief Gets the number of screens created and stored in this manager.
            /// @return Returns the number of screens this manager is storing.
            Whole GetNumScreens();
            /// @brief Deletes a screen and removes all trace of it from the manager.
            /// @details Destroying a screen will also destroy all of it's layers, and everything contained in those layers.
            /// @param Screen The screen to be destroyed.
            void DestroyScreen(UIScreen* Screen);
            /// @brief Deletes all screens stored in this manager.
            void DestroyAllScreens();
            /// @brief Searches all screens and gets the named Layer.
            /// @return Returns the named layer if found, NULL if not.
            UILayer* GetLayer(String& Name);
            /// @brief Searches all visable screens and layers to see if a button was clicked.
            /// @return Returns the button clicked if there is one, NULL if not.
            UIButton* GetButtonMouseIsOver();
            /// @brief Gets the type of manager that this manager is.
            /// @return Returns an enum value representing the type of manager that this manager is.
            ManagerBase::ManagerTypeName GetType() const;
            /// @internal
            /// @brief Gets the internal silverback pointer.
            /// @return Returns a pointer to the internal silverback class.
            Gorilla::Silverback* GetSilverbackPointer();
    };//uimanager
}//phys

#endif
