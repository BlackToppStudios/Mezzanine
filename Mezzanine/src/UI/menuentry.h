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
        class MenuButton;
        class MenuEntryStack;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a class designed to help MenuEntrys keep track of the Menu tree they belong to.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB MenuStack
        {
        protected:
        public:
            MenuStack();
            ~MenuStack();


        };//MenuStack

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This class is an entry for a single window/widget in a menu.
        /// @details This class can be used to represent a single window/widget in a heirarchy
        /// that will change visibility as one would expect in a game menu.
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

            /// @enum ButtonConfig
            /// @brief An enum describing how the MenuButton for this MenuEntry is configured and being used.
            enum ButtonConfig
            {
                BC_Error         = 0,    ///< Error condition or queried button isn't bound to this MenuEntry.
                BC_PushButton    = 1,    ///< Queried button is being used as the entry push button.
                BC_PopButton     = 2,    ///< Queried button is being used as the entry pop button.
                BC_ToggleButton  = 3     ///< Queried button is being used as both the push and pop button, aka toggle button.
            };

            /// @brief String containing the type name for this class: "MenuEntry".
            static const String TypeName;
        protected:
            /// @internal
            /// @brief A pointer to the active stack of MenuEntries.
            MenuEntryContainer* MenuStack;
            /// @internal
            /// @brief A pointer to the button that will push this entry on the menu stack.
            MenuButton* PushButton;
            /// @internal
            /// @brief A pointer to the button that will pop this entry from the menu stack.
            MenuButton* PopButton;
            /// @internal
            /// @brief Stores whether or not this Entry will automatically be hidden when another entry is pushed onto the stack after it.
            Bool AutoHideEntry;

            /// @internal
            /// @brief Pushes this MenuEntry onto the stack if one is available.
            /// @return Returns true if this MenuEntry was successfully pushed onto the stack, false otherwise.
            virtual Bool PushOntoStack();
            /// @internal
            /// @brief Pops this MenuEntry from the stack if one is available.
            /// @return Returns true if this MenuEntry was successfully popped from the stack, false otherwise.
            virtual Bool PopFromStack();
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
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created MenuEntry will belong to.
            MenuEntry(const XML::Node& XMLNode, Screen* Parent);
            /// @brief Class destructor.
            virtual ~MenuEntry();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @brief Gets whether or not this is the Root of the MenuEntry hierarchy.
            /// @return Returns true if this MenuEntry has no parent entry, false otherwise.
            virtual Bool IsRootEntry() const;
            /// @brief Gets the role of the specified MenuButton for this MenuEntry.
            /// @param EntryButton The button to check this MenuEntry for.
            /// @return Returns a ButtonConfig enum value representing how the specified MenuButton is being used by this MenuEntry.
            virtual ButtonConfig GetButtonConfig(const MenuButton* EntryButton) const;

            /// @brief Finds a MenuEntry in the menu stack and hides all Entries above it in the menu stack.
            /// @note This can return zero if the provided entry isn't on the stack or if it is the last entry on the stack.
            /// @param RollBackTo A pointer to the MenuEntry to roll the menu stack back to.
            /// @return Returns the number of MenuEntrys that were hidden by this method.
            virtual Whole RollBackToEntry(MenuEntry* RollBackTo);

            ///////////////////////////////////////////////////////////////////////////////
            // Visibility and Priority Methods

            /// @copydoc Renderable::SetVisible(Bool)
            virtual void SetVisible(Bool CanSee);
            /// @copydoc Renderable::Show()
            virtual void Show();
            /// @copydoc Renderable::Hide()
            virtual void Hide();

            ///////////////////////////////////////////////////////////////////////////////
            // MenuEntry Properties

            /// @brief Sets whether or not thie window should auto hide when another window is added to the menu stack.
            /// @param AutoHide whether or not to enable auto hiding for this menu window.
            virtual void SetAutoHide(Bool AutoHide);
            /// @brief Gets wether or not this window is set to auto hide when another window is added to the menu stack.
            /// @return Returns a bool indicating whether or not AutoHide is enabled on this menu window.
            virtual Bool GetAutoHide() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Menu Configuration

            /// @brief Sets the button that will push(add) this entry on the menu stack, making it visible.
            /// @param Push A pointer to the button that will make this entry visible.
            virtual void SetPushButton(MenuButton* Push);
            /// @brief Gets a pointer to the button that will add this entry to the menu stack.
            /// @return Returns a pointer to the button that will make this entry visible.
            virtual MenuButton* GetPushButton() const;
            /// @brief Sets the button that will pop(remove) this entry from the menu stack, hiding it.
            /// @param Pop A pointer to the button that will make this entry hide.
            virtual void SetPopButton(MenuButton* Pop);
            /// @brief Gets a pointer to the button that will remove this entry from the menu stack.
            /// @return Returns a pointer to the button that will make this entry hide.
            virtual MenuButton* GetPopButton() const;
            /// @brief Sets the button that will both push(add) and pop(remove) the entry from the menu stack, based on the current state of the entry.
            /// @param Toggle A pointer to the button that will toggle this MenuEntrys visibility.
            virtual void SetToggleButton(MenuButton* Toggle);

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc Renderable::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
            /// @copydoc Renderable::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

            /// @copydoc Renderable::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Event Methods



            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Gets the MenuStack this Entry belongs to.
            /// @return Returns a pointer to the MenuEntryContainer being used as the menu stack.
            MenuEntryContainer* _GetMenuStack() const;
            /// @internal
            /// @brief Notifies this MenuEntry and all if it's Entry children a new MenuStack is being applied to the menu tree.
            /// @param NewStack the new stack to be applied.  Can be NULL to remove the stack from all children.
            virtual void _NotifyStack(MenuEntryContainer* NewStack);
            /// @copydoc EventSubscriber::_NotifyEvent(const EventArguments&)
            virtual void _NotifyEvent(const EventArguments& Args);
            /// @copydoc QuadRenderable::_NotifyParenthood(QuadRenderable*)
            virtual void _NotifyParenthood(QuadRenderable* NewParent);
            /// @copydoc QuadRenderable::_HasAvailableRenderData() const
            virtual Bool _HasAvailableRenderData() const;
        };//MenuEntry
    }//UI
}//Mezzanine

#endif
