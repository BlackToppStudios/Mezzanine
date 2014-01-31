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
#ifndef _uihotkeyhandler_h
#define _uihotkeyhandler_h

#include "datatypes.h"
#include "Input/inputenumerations.h"
#include "Input/metacode.h"

namespace Mezzanine
{
    namespace UI
    {
        class Button;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class HotKeyHandler
        /// @headerfile hotkeyhandler.h
        /// @brief This class is responsible for the registration and handling of hotkeys in the UI system.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB HotKeyHandler
        {
        public:
            /// @brief A convenience type for handling hotkey pairs.
            typedef std::pair< Input::InputCode, UI::Button* > HotKeyPair;
            /// @brief Basic container type for hotkey binding storage by this class.
            typedef std::multimap< Input::InputCode, UI::Button* > HotKeyContainer;
            /// @brief Iterator type for hotkey bindings stored by this class.
            typedef HotKeyContainer::iterator                      HotKeyIterator;
            /// @brief Const Iterator type for hotkey bindings stored by this class.
            typedef HotKeyContainer::const_iterator                ConstHotKeyIterator;
        protected:
            /// @internal
            /// @brief A container storing all registered hotkey bindings.
            HotKeyContainer HotKeys;
        public:
            /// @brief Class constructor.
            HotKeyHandler();
            /// @brief Class destructor.
            ~HotKeyHandler();

            ///////////////////////////////////////////////////////////////////////////////
            // HotKey Management

            /// @brief Binds a key to a button.
            /// @details This function allows buttons to behave like they are pressed without mouse input.
            /// @param HotKey The key or button (on the input device) to activate the button.
            /// @param BoundButton The button being bound to the hotkey.
            void BindHotKey(const Input::MetaCode& HotKey, UI::Button* BoundButton);
            /// @brief Removes a previously set hotkey binding.
            /// @param HotKey The key or button (on the input device) to activate the button.
            /// @param BoundButton The button currently bound to the hotkey.
            void UnbindHotKey(const Input::MetaCode& HotKey, UI::Button* BoundButton);
            /// @brief Clears all registered hotkeys.
            void RemoveAllHotKeys();
        };//HotKeyHandler
    }//UI
}//Mezzanine

#endif
