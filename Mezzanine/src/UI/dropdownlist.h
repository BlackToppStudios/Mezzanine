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
#ifndef _uidropdownlist_h
#define _uidropdownlist_h

#include "UI/widget.h"
//#include "UI/listbox.h"

namespace Mezzanine
{
    namespace UI
    {
        class ListBox;
        class CheckBox;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a widget that displays one selection from a list that can have it's visibility toggled.
        /// @details
        /// @remarks It is not uncommon to have a DropDownList in close proximity to other renderables that could overlap with
        /// the ListBox child of a DropDownList.  When this happens be mindful of the ZOrders of sibling renderables.  If the
        /// DropDownList has the highest ZOrder among it's surrounding renderables then the ListBox will behave as expected.
        ///////////////////////////////////////
        class MEZZ_LIB DropDownList : public Widget
        {
        public:
            /// @brief String containing the type name for this class: "DropDownList".
            static const String TypeName;
        protected:
            friend class DropDownListFactory;
            /// @internal
            /// @brief A pointer to a copy of the ListItem that is the current selection.
            Widget* SelectionDisplay;
            /// @internal
            /// @brief A pointer to the CheckBox that will toggle the visibility of the Selection List.
            CheckBox* ListToggle;
            /// @internal
            /// @brief A pointer to the ListBox holding all possible selections.
            ListBox* SelectionList;

            /// @internal
            /// @brief Convenience method for the construction of a DropDownList.
            /// @param Style An enum value representing how the scrollbar child of thie ListBox will be constructed.  See @ref UI::ScrollbarStyle enum for more info.
            virtual void ConstructDropDownList(const UI::ScrollbarStyle& Style);
            /// @internal
            /// @brief Configures the selection display of this DropDownList to match the new selection.
            /// @param NewSelection A pointer to the item that was selected in the ListBox.
            virtual void UpdateCurrentSelection(Widget* NewSelection);
        //public:
            /// @brief Blank constructor.
            /// @param Parent The parent Screen that created this widget.
            DropDownList(Screen* Parent);
            /// @brief Standard initialization constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param Style An enum value representing how the scrollbar child of the ListBox will be constructed.  See @ref UI::ScrollbarStyle enum for more info.
            /// @param Parent The parent Screen that created this widget.
            DropDownList(const String& RendName, const UI::ScrollbarStyle& Style, Screen* Parent);
            /// @brief Rect constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param RendRect The rect describing this widget's transform relative to it's parent.
            /// @param Style An enum value representing how the scrollbar child of the ListBox will be constructed.  See @ref UI::ScrollbarStyle enum for more info.
            /// @param Parent The parent screen that created this renderable.
            DropDownList(const String& RendName, const UnifiedRect& RendRect, const UI::ScrollbarStyle& Style, Screen* Parent);
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created ListBox will belong to.
            DropDownList(const XML::Node& XMLNode, Screen* Parent);
            /// @brief Class destructor.
            virtual ~DropDownList();
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @copydoc QuadRenderable::UpdateDimensions(const Rect&, const Rect&)
            virtual void UpdateDimensions(const Rect& OldSelfRect, const Rect& NewSelfRect);

            /// @copydoc Widget::GetTypeName() const
            virtual const String& GetTypeName() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Visibility and Priority Methods

            /// @copydoc Renderable::SetVisible(Boole)
            virtual void SetVisible(Boole CanSee);
            /// @copydoc Renderable::Show()
            virtual void Show();
            /// @copydoc Renderable::Hide()
            virtual void Hide();

            ///////////////////////////////////////////////////////////////////////////////
            // DropDownList Properties

            ///////////////////////////////////////////////////////////////////////////////
            // DropDownList Configuration

            /// @brief Gets the Widget displaying the current selection.
            /// @return Returns a pointer to the Widget responsible for the display of the selected list item.
            virtual Widget* GetSelectionDisplay() const;
            /// @brief Gets the CheckBox that toggles the selection list visibility.
            /// @return Returns a pointer to the CheckBox responsible for toggling the selection list visibility.
            virtual CheckBox* GetListToggle() const;
            /// @brief Gets the ListBox containing all the list options.
            /// @return Returns a pointer to the ListBox containing all selectable items.
            virtual ListBox* GetSelectionList() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc Renderable::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
            /// @copydoc QuadRenderable::ProtoSerializeChildQuads(XML::Node&) const
            virtual void ProtoSerializeChildQuads(XML::Node& SelfRoot) const;

            /// @copydoc Renderable::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);
            /// @copydoc QuadRenderable::ProtoDeSerializeChildQuads(const XML::Node&)
            virtual void ProtoDeSerializeChildQuads(const XML::Node& SelfRoot);

            /// @copydoc Renderable::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Event Methods

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @copydoc EventSubscriber::_NotifyEvent(EventArgumentsPtr)
            virtual void _NotifyEvent(EventArgumentsPtr Args);
        };//DropDownList

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the factory implementation for DropDownList widgets.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB DropDownListFactory : public WidgetFactory
        {
        public:
            /// @brief Class constructor.
            DropDownListFactory() {  }
            /// @brief Class destructor.
            virtual ~DropDownListFactory() {  }

            /// @copydoc WidgetFactory::GetWidgetTypeName() const
            virtual String GetWidgetTypeName() const;

            /// @brief Creates a new DropDownList.
            /// @param RendName The name to be given to the created DropDownList.
            /// @param Style An enum value representing how the scrollbar child of the ListBox will be constructed.  See @ref UI::ScrollbarStyle enum for more info.
            /// @param Parent The screen the created DropDownList will belong to.
            /// @return Returns a pointer to the created DropDownList.
            virtual DropDownList* CreateDropDownList(const String& RendName, const UI::ScrollbarStyle& Style, Screen* Parent);
            /// @brief Creates a new DropDownList.
            /// @param RendName The name to be given to the created DropDownList.
            /// @param RendRect The dimensions that will be assigned to the created DropDownList.
            /// @param Style An enum value representing how the scrollbar child of the ListBox will be constructed.  See @ref UI::ScrollbarStyle enum for more info.
            /// @param Parent The screen the created DropDownList will belong to.
            /// @return Returns a pointer to the created DropDownList.
            virtual DropDownList* CreateDropDownList(const String& RendName, const UnifiedRect& RendRect, const UI::ScrollbarStyle& Style, Screen* Parent);
            /// @brief Creates a new DropDownList.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created DropDownList will belong to.
            /// @return Returns a pointer to the created DropDownList.
            virtual DropDownList* CreateDropDownList(const XML::Node& XMLNode, Screen* Parent);

            /// @copydoc WidgetFactory::CreateWidget(Screen*)
            virtual Widget* CreateWidget(Screen* Parent);
            /// @copydoc WidgetFactory::CreateWidget(const String&, const NameValuePairMap&, Screen*)
            virtual Widget* CreateWidget(const String& RendName, const NameValuePairMap& Params, Screen* Parent);
            /// @copydoc WidgetFactory::CreateWidget(const String&, const UnifiedRect&, const NameValuePairMap&, Screen*)
            virtual Widget* CreateWidget(const String& RendName, const UnifiedRect& RendRect, const NameValuePairMap& Params, Screen* Parent);
            /// @copydoc WidgetFactory::CreateWidget(const XML::Node&, Screen*)
            virtual Widget* CreateWidget(const XML::Node& XMLNode, Screen* Parent);
            /// @copydoc WidgetFactory::DestroyWidget(Widget*)
            virtual void DestroyWidget(Widget* ToBeDestroyed);
        };//DropDownListFactory
    }//UI
}//Mezzanine

#endif
