// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef _uicheckbox_h
#define _uicheckbox_h

#include "datatypes.h"
#include "UI/button.h"

namespace Mezzanine
{
    namespace UI
    {
        class Button;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a simple widget for storing a bool value.
        /// @details This class does not use a LayoutStrategy.
        ///////////////////////////////////////
        class MEZZ_LIB CheckBox : public Button
        {
        public:
            /// @brief Enum describing extended widget states for the CheckBox widget.
            enum WidgetStateExt
            {
                WS_Selected = Widget::WS_Mezzanine_Reserved_1
            };
            /// @brief Enum describing additional pre-initialized RenderLayerGroups created by the Checkbox widget.
            enum WidgetGroupIDExt
            {
                WG_SelectedNormal = Widget::WG_Mezzanine_Reserved_1,
                WG_SelectedHovered = Widget::WG_Mezzanine_Reserved_2
            };

            /// @brief String containing the type name for this class: "CheckBox".
            static const String TypeName;
            /// @brief Event name for when this checkbox is Selected.
            static const String EventSelected;
            /// @brief Event name for when this checkbox is Deselected.
            static const String EventDeselected;
        protected:
            /// @internal
            /// @brief Stores whether or not the current state of this CheckBox is locked.
            Boole SelectLock;

            /// @internal
            /// @brief Contains all the common necessary startup initializations for this class.
            void ConstructCheckbox();
        public:
            /// @brief Blank constructor.
            /// @param Parent The parent Screen that created this widget.
            CheckBox(Screen* Parent);
            /// @brief Standard initialization constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param Parent The parent Screen that created this widget.
            CheckBox(const String& RendName, Screen* Parent);
            /// @brief Rect constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param RendRect The rect describing this widget's transform relative to it's parent.
            /// @param Parent The parent screen that created this renderable.
            CheckBox(const String& RendName, const UnifiedRect& RendRect, Screen* Parent);
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created CheckBox will belong to.
            CheckBox(const XML::Node& XMLNode, Screen* Parent);
            /// @brief Class destructor.
            virtual ~CheckBox();
        //public:
            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @brief Gets whether this checkbox is selected or not.
            /// @return Returns a bool representing whether or not this checkbox is selected.
            virtual Boole IsSelected();
            /// @brief Gets wether this checkbox is locked into it's current state.
            /// @return Returns true if this checkbox can't change it's state, false otherwise.
            virtual Boole IsLocked();
            /// @brief Manually select or deselect this checkbox.
            /// @param Select The value to set the status of this checkbox.
            virtual void ManualSelect(Boole Select);
            /// @brief Locks (or unlocks) the current state of this checkbox.
            /// @param Lock Whether or not to lock the current state of this checkbox.
            virtual void SetSelectLock(Boole Lock);
            /// @copydoc Widget::GetTypeName() const
            virtual const String& GetTypeName() const;

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

            /// @copydoc Button::_OnActivate()
            virtual void _OnActivate();
            /// @copydoc Button::_OnDeactivate()
            virtual void _OnDeactivate();
            /// @brief Self logic to be executed when this checkbox is selected.
            virtual void _OnSelected();
            /// @brief Self logic to be executed when this checkbox is deselected.
            virtual void _OnDeselected();
        };//CheckBox

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the factory implementation for CheckBox widgets.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB CheckBoxFactory : public WidgetFactory
        {
        public:
            /// @brief Class constructor.
            CheckBoxFactory() {  }
            /// @brief Class destructor.
            virtual ~CheckBoxFactory() {  }

            /// @copydoc WidgetFactory::GetWidgetTypeName() const
            virtual String GetWidgetTypeName() const;

            /// @brief Creates a new CheckBox.
            /// @param RendName The name to be given to the created CheckBox.
            /// @param Parent The screen the created CheckBox will belong to.
            /// @return Returns a pointer to the created CheckBox.
            virtual CheckBox* CreateCheckBox(const String& RendName, Screen* Parent);
            /// @brief Creates a new CheckBox.
            /// @param RendName The name to be given to the created CheckBox.
            /// @param RendRect The dimensions that will be assigned to the created CheckBox.
            /// @param Parent The screen the created CheckBox will belong to.
            /// @return Returns a pointer to the created CheckBox.
            virtual CheckBox* CreateCheckBox(const String& RendName, const UnifiedRect& RendRect, Screen* Parent);
            /// @brief Creates a new CheckBox.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created CheckBox will belong to.
            /// @return Returns a pointer to the created CheckBox.
            virtual CheckBox* CreateCheckBox(const XML::Node& XMLNode, Screen* Parent);

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
        };//CheckBoxFactory
    }//UI
}//Mezzanine

#endif
