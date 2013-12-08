//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _uimenuentry_h
#define _uimenuentry_h

#include "UI/stackedcontainer.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class is a control mechanism for multiple windows in a heirarchy.
        /// @details This class controls the presentation and order of different windows, useful
        /// for creating menu systems, be it a game main menu, or in-game menu. @n @n
        /// Also it should be noted that since this is just a control system for other classes, it
        /// doesn't have a position or size like other classes.  Instead when you call those functions
        /// to set or get the position or size, you'll be working with the current top level window.
        ///////////////////////////////////////
        class MEZZ_LIB MenuEntry : public StackedContainer
        {
        public:
            /// @brief Basic container type for child MenuEntry storage by this class.
            typedef std::vector<MenuEntry*>               MenuEntryContainer;
            /// @brief Iterator type for child MenuEntry instances stored by this class.
            typedef MenuEntryContainer::iterator          MenuEntryIterator;
            /// @brief Const Iterator type for child MenuEntry instances stored by this class.
            typedef MenuEntryContainer::const_iterator    ConstMenuEntryIterator;

            /// @brief String containing the type name for this class: "MenuEntry".
            static const String TypeName;
        protected:
            /// @internal
            /// @brief A pointer to the active stack of MenuEntries.
            MenuEntryContainer* MenuStack;
            /// @internal
            /// @brief A pointer to the button that will push this entry on the menu stack.
            Button* PushButton;
            /// @internal
            /// @brief A pointer to the button that will pop this entry from the menu stack.
            Button* PopButton;
        public:
            /// @brief Blank constructor.
            /// @param Parent The parent Screen that created this widget.
            MenuEntry(Screen* Parent);
            /// @brief Standard initialization constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param Parent The parent Screen that created this widget.
            MenuEntry(const String& RendName, Screen* Parent);
            /// @brief Rect constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param RendRect The rect describing this widget's transform relative to it's parent.
            /// @param Parent The parent screen that created this renderable.
            MenuEntry(const String& RendName, const UnifiedRect& RendRect, Screen* Parent);
            /// @brief Class destructor.
            virtual ~MenuEntry();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @brief Gets whether or not this is the Root of the MenuEntry hierarchy.
            /// @return Returns true if this MenuEntry has no parent entry, false otherwise.
            virtual Bool IsRootEntry() const;

            ///////////////////////////////////////////////////////////////////////////////
            // MenuEntry Properties



            ///////////////////////////////////////////////////////////////////////////////
            // Menu Configuration

            /// @brief Sets the button that will push(add) this entry on the menu stack, making it visible.
            /// @param Push A pointer to the button that will make this entry visible.
            virtual void SetEntryPushButton(Button* Push);
            /// @brief Sets the button that will pop(remove) this entry from the menu stack, hiding it.
            /// @param Pop A pointer to the button that will make this entry hide.
            virtual void SetEntryPopButton(Button* Pop);

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization



            ///////////////////////////////////////////////////////////////////////////////
            // Internal Event Methods



            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @copydoc EventSubscriber::_NotifyEvent(const EventArguments& Args)
            virtual void _NotifyEvent(const EventArguments& Args);
        };//MenuEntry
    }//UI
}//Mezzanine

#endif
