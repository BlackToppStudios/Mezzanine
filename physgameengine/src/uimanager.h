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
    ///////////////////////////////////////////////////////////////////////////////
    /// @class UIManager
    /// @headerfile uimanager.h
    /// @brief This class is responsible for any and all user interactions with the User interface/HUD.
    /// @details Optionally, you can create the UI/HUD in code or load a premade one using a Gorilla file(*.gorilla).
    ///////////////////////////////////////
    class UIManager : public ManagerBase
    {
        protected:
            /// @brief Pointer for the Gorilla core class, where this manager gets it's functionality.
            Gorilla::Silverback* Silver;
            std::map< String, Gorilla::Screen* > Screens;
        public:
            /// @brief Class Constructor.
            /// @details Standard class initialization constructor.
            /// @param Name The name of the .gorilla file to load with this manager.
            UIManager(World* World_);
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
            /// @brief Creates an internal HUD screen.
            /// @details Screens are the base set of renderable UI you can use, allowing you to switch entire sets of UI's
            /// on the fly if needed.  For performance reasons you should always keep the number of screens you create to a minimum.
            /// @param Screen The name to be given to the screen.
            /// @param Atlas The name of a previously loaded Gorilla file to be used with this screen.
            /// @param Viewport The name of the viewport to create this screen in.
            void CreateScreen(const String& Screen, const String& Atlas, const String& Viewport="DefaultViewport");
            /// @brief Gets the type of manager that this manager is.
            /// @return Returns an enum value representing the type of manager that this manager is.
            ManagerBase::ManagerTypeName GetType() const;
    };//uimanager
}//phys

#endif
