// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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
#ifndef _uilistbox_h
#define _uilistbox_h

#include "colourvalue.h"
#include "UI/uienumerations.h"
#include "UI/widget.h"
#include "UI/textlayer.h"

namespace Mezzanine
{
    namespace UI
    {
        class FontData;
        class VerticalScrollbar;
        class VerticalContainer;

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a widget for displaying a list of captions in a box.
        /// @details This class uses the base LayoutStrategy.
        /// @remarks ListItems owned by a ListBox are not direct children of the ListBox.  Instead they are direct children of
        /// the VerticalContainer that is the child of the ListBox.  There are convenience methods that allow light management
        /// of ListItems, but they are really just wrappers around similiar methods on the VerticalContainer.
        ///////////////////////////////////////
        class MEZZ_LIB ListBox : public Widget
        {
        public:
            /// @brief Convenience typedef for objects that will be used to represent list items in this ListBox.
            typedef Widget ListItem;

            /// @brief This enum is used to describe how new ListItems are inserted into the list.
            enum ListItemOrdering
            {
                LIO_TopInsert      = 1,  ///< New entries will be inserted at the top of the list.
                LIO_BottomInsert   = 2   ///< New entries will be inserted at the bottom of the list.
            };

            /// @brief String containing the type name for this class: "ListBox".
            static const String TypeName;
        protected:
            /// @internal
            /// @brief Stores the default font that will be used by all ListItems generated by this ListBox.
            FontData* ListItemFont;
            /// @internal
            /// @brief A pointer to the vertical scrollbar responsible for the visible list items.
            VerticalScrollbar* ListScroll;
            /// @internal
            /// @brief A pointer to the vertical container storing all the list items.
            VerticalContainer* ListContainer;
            /// @internal
            /// @brief The amount of auto-scaling to be applied to all created list items.
            Real ListItemCharScaling;
            /// @internal
            /// @brief The auto-scaling mode that is to be applied to all created list items.
            TextLayer::ScalingMode ListItemCharScalingMode;
            /// @internal
            /// @brief Stores how new entries will be inserted with other elements in this ListBox.
            ListItemOrdering Ordering;

            /// @copydoc Widget::HandleInputImpl(const Input::MetaCode&)
            virtual Boole HandleInputImpl(const Input::MetaCode& Code);
            /// @internal
            /// @brief Convenience method for the construction of a ListBox.
            /// @param Style An enum value representing how the scrollbar child of thie ListBox will be constructed.  See @ref UI::ScrollbarStyle enum for more info.
            virtual void ConstructListBox(const UI::ScrollbarStyle& Style);
            /// @internal
            /// @brief Helper method that creates a basic ListItem with no TextLayers.
            /// @param ItemName The name to be given to the new ListItem.
            /// @return Returns a pointer to the newly created ListItem.
            virtual ListItem* CreateListItem(const String& ItemName);
        public:
            /// @brief Blank constructor.
            /// @param Parent The parent Screen that created this widget.
            ListBox(Screen* Parent);
            /// @brief Standard initialization constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param Style An enum value representing how the scrollbar child of thie ListBox will be constructed.  See @ref UI::ScrollbarStyle enum for more info.
            /// @param Parent The parent Screen that created this widget.
            ListBox(const String& RendName, const UI::ScrollbarStyle& Style, Screen* Parent);
            /// @brief Rect constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param RendRect The rect describing this widget's transform relative to it's parent.
            /// @param Style An enum value representing how the scrollbar child of thie ListBox will be constructed.  See @ref UI::ScrollbarStyle enum for more info.
            /// @param Parent The parent screen that created this renderable.
            ListBox(const String& RendName, const UnifiedRect& RendRect, const UI::ScrollbarStyle& Style, Screen* Parent);
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created ListBox will belong to.
            ListBox(const XML::Node& XMLNode, Screen* Parent);
            /// @brief Class destructor.
            virtual ~ListBox();
        //public:
            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @brief Sets both the size and enforcement rules for forced list item sizing in this container.
            /// @note The sizing provided here works just like any other normal list item sizing does, with the relative parts being based on the the parent container (this).
            /// @param ForcedSize A SizingInfo describing how all children should size themselves on dimension updates.
            /// @param Enforcement A bitmask containing when the list item sizing will be enforced by this container.  See SizeEnforcement enum for more details.
            virtual void SetListItemSizing(const SizingInfo& ForcedSize, const Whole Enforcement);
            /// @brief Sets the size to be given to children processed by this container if forced sizing is enabled.
            /// @note The sizing provided here works just like any other normal list item sizing does, with the relative parts being based on the the parent container (this).
            /// @param ForcedSize A SizingInfo describing how all children should size themselves on dimension updates.
            virtual void SetListItemSize(const SizingInfo& ForcedSize);
            /// @brief Gets the size to be given to children processed by this container if forced sizing is enabled.
            /// @return Returns a const SizeInfo reference to the sizing given to children processed by this container if forced sizing is enabled.
            virtual const SizingInfo& GetListItemSize() const;
            /// @brief Sets when the set list item sizing will be applied to any given list item.
            /// @param Enforcement A bitmask containing when the list item sizing will be enforced by this container.  See SizeEnforcement enum for more details.
            virtual void SetListItemSizeEnforcement(const Whole Enforcement);
            /// @brief Gets when the set list item sizing will be applied to any given list item.
            /// @return Returns a bitmask describing when the list item sizing will be enforced by this container.  See SizeEnforcement enum for more details.
            virtual Whole GetListItemSizeEnforcement() const;

            /// @brief Sets the Unified width of the child scrollbar in this ListBox.
            /// @note This defaults to UnifiedDim(0.08,0.0).
            /// @param ScrollWidth The relative (to this) and absolute components of the width to be given the child scrollbar.
            virtual void SetScrollbarWidth(const UnifiedDim& ScrollWidth);
            /// @brief Gets the Unified width of the child scrollbar in this ListBox.
            /// @return Returns a const reference to the Unified dimension containing the width of the child scrollbar.
            virtual const UnifiedDim& GetScrollbarWidth() const;

            /// @copydoc QuadRenderable::UpdateDimensions(const Rect&, const Rect&)
            virtual void UpdateDimensions(const Rect& OldSelfRect, const Rect& NewSelfRect);

            /// @copydoc Widget::GetTypeName() const
            virtual const String& GetTypeName() const;

            ///////////////////////////////////////////////////////////////////////////////
            // ListBox Properties

            /// @brief Sets the font that will be used by ListItems in this ListBox.
            /// @note This method is not retroactive.  ListItems that have been already created won't be altered, but new
            /// ListItems will have the specified font as their default font (which can be overridden via Markup).
            /// @param Font A pointer to the font that will be used by ListItems in this ListBox.
            virtual void SetListItemFont(FontData* Font);
            /// @brief Sets the font that will be used by ListItems in this ListBox.
            /// @note This method is not retroactive.  ListItems that have been already created won't be altered, but new
            /// ListItems will have the specified font as their default font (which can be overridden via Markup).
            /// @param FontName The name of the Font to use when creating ListItems in this ListBox.
            virtual void SetListItemFont(const String& FontName);
            /// @brief Gets the font that will be used by ListItems in this ListBox.
            /// @return Returns a pointer to the Font being used by all ListItems generated by this ListBox.
            virtual FontData* GetListItemFont() const;

            /// @brief Sets the mode and scaler of auto-scaling applied to ListItems created by this ListBox.
            /// @remarks There are a lot of considerations to be made with auto-scaling.  Auto-scaling will adjust the size of the provided font
            /// automatically based on the scaling mode that is set, which really decides what the relative scalar provided for auto-scaling is
            /// relative to.  If auto-scaling is relative to the screen, then the text will only change in size when the screen does.  Parent and
            /// layer relative scaling are very similiar since layers get their dimensions from the parent widget they belong to.  The difference
            /// comes from scaling applied to the layer itself.  Parent relative scaling ignores the layer scaling, while layer relative factors
            /// it in.  @n @n
            /// In addition to the relative component to the scaling, manual scaling will also be applied after the auto-scaling is.  Auto-scaling
            /// will be completely ignored if the custom size on a character is set.
            /// @param Mode A ScalingMode enum value describing what the provided scaler will be relative to.
            /// @param Scalar The relative value that will be used to determine the needed scaling to be applied to text in this layer.
            virtual void SetListItemTextScale(const TextLayer::ScalingMode Mode, const Real Scalar);
            /// @brief Gets the automatic scaling mode being used by ListItems created by this ListBox.
            /// @return Returns a ScalingMode enum value describing what the relative scaler is relative to, if auto-scaling is enabled at all.
            virtual TextLayer::ScalingMode GetListItemTextScalingMode() const;
            /// @brief Gets the relative scalar being used to automatically scale text in ListItems created by this ListBox.
            /// @return Returns a Real representing the relative Y size to use when detmining how to uniformly scale text in ListItems.
            virtual Real GetListItemTextScalar() const;

            /// @brief Sets how new entries will be inserted into the list.
            /// @param Order A ListItemOrdering enum value expressing how this ListBox is to insert new entries.
            virtual void SetListItemOrdering(ListBox::ListItemOrdering Order);
            /// @brief Gets how new entries will be inserted into the list.
            /// @return Returns a ListItemOrdering enum value representing how new ListItems are being inserted into this ListBox.
            virtual ListBox::ListItemOrdering GetListItemOrdering() const;

            ///////////////////////////////////////////////////////////////////////////////
            // ListBox Configuration

            /// @brief Convenience method that will create and add a ListItem with a single line text layer to this ListBox.
            /// @note The auto-created text layers ZOrder is 5.  TextLine(s) in the ListItem are set to a centered vertical
            /// alignment.  ListItems by default are sized to automatically fill the full X axis of it's container, and X+2 pixels in
            /// height, where the height is the space needed for all it's text lines.  New ListItems are also automatically added as
            /// a child of this ListBox, so you don't need to add them to the ListBox yourself.
            /// @param ItemName The name to be given to the new ListItem.
            /// @param Text The text that is to be created and assigned to the new list item.
            /// @return Returns a pointer to the newly created ListItem.
            virtual ListItem* CreateSingleLineListItem(const String& ItemName, const String& Text);
            /// @brief Convenience method that will create and add a ListItem with a multi-line text layer to this ListBox.
            /// @note The auto-created text layers ZOrder is 5.  TextLine(s) in the ListItem are set to a centered vertical
            /// alignment.  ListItems by default are sized to automatically fill the full X axis of it's container, and X+2 pixels in
            /// height, where the height is the space needed for all it's text lines.  New ListItems are also automatically added as
            /// a child of this ListBox, so you don't need to add them to the ListBox yourself.
            /// @param ItemName The name to be given to the new ListItem.
            /// @param Text The text that is to be created and assigned to the new list item.
            /// @return Returns a pointer to the newly created ListItem.
            virtual ListItem* CreateMultiLineListItem(const String& ItemName, const String& Text);
            /// @brief Gets a ListItem in this ListBox by name.
            /// @param Name The name of the ListItem to retrieve.
            /// @return Returns a pointer to the specified ListItem.
            virtual ListItem* GetListItem(const String& Name) const;
            /// @brief Gets the number of ListItems in this ListBox.
            /// @return Returns a Whole representing the number of ListItems owned by this ListBox.
            virtual Whole GetNumListItems() const;
            /// @brief Destroys a ListItem owned by this ListBox.
            /// @param ToBeDestroyed A pointer to the ListItem to be destroyed.
            virtual void DestroyListItem(ListItem* ToBeDestroyed);
            /// @brief Destroys all the ListItems currently owned by this ListBox.
            virtual void DestroyAllListItems();

            /// @brief Gets the Scrollbar within this widget.
            /// @return Returns a pointer to the VerticalScrollbar used to scroll through the ListItems of this widget.
            virtual VerticalScrollbar* GetListScroll() const;
            /// @brief Gets the ListItem container within this widget.
            /// @return Returns a pointer to the VerticalContainer used to store the ListItems of this widget.
            virtual VerticalContainer* GetListContainer() const;
            /// @brief Gets the last ListItem that was selected/focused.
            /// @return Returns a pointer to the last ListItem that was selected/focused.
            virtual ListItem* GetLastSelectedListItem() const;

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

        };//ListBox

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the factory implementation for ListBox widgets.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB ListBoxFactory : public WidgetFactory
        {
        public:
            /// @brief Class constructor.
            ListBoxFactory() {  }
            /// @brief Class destructor.
            virtual ~ListBoxFactory() {  }

            /// @copydoc WidgetFactory::GetWidgetTypeName() const
            virtual String GetWidgetTypeName() const;

            /// @brief Creates a new ListBox.
            /// @param RendName The name to be given to the created ListBox.
            /// @param Style An enum value representing how the scrollbar child of thie ListBox will be constructed.  See @ref UI::ScrollbarStyle enum for more info.
            /// @param Parent The screen the created ListBox will belong to.
            /// @return Returns a pointer to the created ListBox.
            virtual ListBox* CreateListBox(const String& RendName, const UI::ScrollbarStyle& Style, Screen* Parent);
            /// @brief Creates a new ListBox.
            /// @param RendName The name to be given to the created ListBox.
            /// @param RendRect The dimensions that will be assigned to the created ListBox.
            /// @param Style An enum value representing how the scrollbar child of thie ListBox will be constructed.  See @ref UI::ScrollbarStyle enum for more info.
            /// @param Parent The screen the created ListBox will belong to.
            /// @return Returns a pointer to the created ListBox.
            virtual ListBox* CreateListBox(const String& RendName, const UnifiedRect& RendRect, const UI::ScrollbarStyle& Style, Screen* Parent);
            /// @brief Creates a new ListBox.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created ListBox will belong to.
            /// @return Returns a pointer to the created ListBox.
            virtual ListBox* CreateListBox(const XML::Node& XMLNode, Screen* Parent);

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
        };//ListBoxFactory
    }//UI
}//Mezzanine

#endif
