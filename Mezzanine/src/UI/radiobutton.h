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
#ifndef _uiradiobutton_h
#define _uiradiobutton_h

#include "UI/checkbox.h"

namespace Mezzanine
{
    namespace UI
    {
        class RadioButton;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a class designed to facilitate operations across an entire group of RadioButtons.
        /// @details
        ///////////////////////////////////////
        class RadioButtonGroup
        {
        public:
            /// @brief Basic container type for RadioButton storage by this class.
            typedef std::vector<RadioButton*>                       RadioButtonContainer;
            /// @brief Iterator type for RadioButton instances stored by this class.
            typedef RadioButtonContainer::iterator                  RadioButtonIterator;
            /// @brief Const Iterator type for RadioButton instances stored by this class.
            typedef RadioButtonContainer::const_iterator            ConstRadioButtonIterator;
        protected:
            /// @internal
            /// @brief A container storing all the RadioButtons belonging to this group.
            RadioButtonContainer GroupButtons;
            /// @internal
            /// @brief A pointer to the RadioButton that is the current selection, or NULL if none are selected.
            RadioButton* CurrentSelection;
        public:
            /// @brief Class constructor.
            RadioButtonGroup();
            /// @brief Class destructor.
            ~RadioButtonGroup();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @brief Adds a RadioButton to this group.
            /// @param ToAdd A pointer to the RadioButton to be added to this group.
            void AddButtonToGroup(RadioButton* ToAdd);
            /// @brief Gets the number of buttons in this group.
            /// @return Returns a Whole representing the number of buttons that belong to this group.
            Whole GetNumButtons() const;
            /// @brief Removes a RadioButton from this group.
            /// @param ToRemove A pointer to the RadioButton to be removed from this group.
            void RemoveButtonFromGroup(RadioButton* ToRemove);

            /// @brief Makes a RadioButton in this group the selection, deselecting all other RadioButtons in the group.
            /// @note You can pass in NULL to ensure none of the RadioButtons in this group are selected.
            /// @param ToSelect A pointer to the RadioButton in this group that will be selected.
            void SelectButton(RadioButton* ToSelect);
            /// @brief Deselects all but one (or just all) button(s) in this group
            /// @note You can pass in NULL to deselect all buttons in this group.
            /// @param Exclude The Button that will be excluded from forced deselection.
            void DeselectOtherButtons(RadioButton* Exclude);
            /// @brief Gets the button that is currently selected among this group of buttons.
            /// @return Returns a pointer to the currently selected button in this group.
            RadioButton* GetCurrentSelection() const;

            /// @brief Gets an iterator to the first RadioButton.
            /// @return Returns an iterator to the first RadioButton being stored by this renderable.
            RadioButtonIterator RadioButtonBegin();
            /// @brief Gets an iterator to one passed the last RadioButton.
            /// @return Returns an iterator to one passed the last RadioButton being stored by this renderable.
            RadioButtonIterator RadioButtonEnd();
            /// @brief Gets a const iterator to the first RadioButton.
            /// @return Returns a const iterator to the first RadioButton being stored by this renderable.
            ConstRadioButtonIterator RadioButtonBegin() const;
            /// @brief Gets an iterator to one passed the last RadioButton.
            /// @return Returns an iterator to one passed the last RadioButton being stored by this renderable.
            ConstRadioButtonIterator RadioButtonEnd() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Notifies this group a button has been selected.
            /// @param Selected The radio button that was selected.
            void _NotifyButtonSelected(RadioButton* Selected);
        };//RadioButtonGroup

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a simple widget where only one of it's selections can be selected at a time.
        /// @details
        ///////////////////////////////////////
        class RadioButton : public CheckBox
        {
        public:
            /// @brief String containing the type name for this class: "RadioButton".
            static const String TypeName;
        protected:
            friend class RadioButtonFactory;
            /// @internal
            /// @brief A pointer to the group this RadioButton belongs to.
            RadioButtonGroup* ButtonGroup;

            /// @copydoc Renderable::ProtoSerializeImpl(XML::Node&) const
            virtual void ProtoSerializeImpl(XML::Node& SelfRoot) const;
            /// @copydoc Renderable::ProtoDeSerializeImpl(const XML::Node&)
            virtual void ProtoDeSerializeImpl(const XML::Node& SelfRoot);
        //public:
            /// @brief Blank constructor.
            /// @param Parent The parent Screen that created this widget.
            RadioButton(Screen* Parent);
            /// @brief Standard initialization constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param Parent The parent Screen that created this widget.
            RadioButton(const String& RendName, Screen* Parent);
            /// @brief Rect constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param RendRect The rect describing this widget's transform relative to it's parent.
            /// @param Parent The parent screen that created this renderable.
            RadioButton(const String& RendName, const UnifiedRect& RendRect, Screen* Parent);
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created RadioButton will belong to.
            RadioButton(const XML::Node& XMLNode, Screen* Parent);
            /// @brief Class destructor.
            virtual ~RadioButton();
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            ///////////////////////////////////////////////////////////////////////////////
            // RadioButton Properties

            ///////////////////////////////////////////////////////////////////////////////
            // RadioButton Configuration

            /// @brief Adds this RadioButton to the same RadioButtonGroup as the specified RadioButton.
            /// @param GroupButton The RadioButton to get the button group from and add this button to.
            virtual void AddToButtonGroup(RadioButton* GroupButton);
            /// @brief Removes this RadioButton from the RadioButtonGroup it currently belongs to.
            virtual void RemoveFromButtonGroup();

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert the Group Buttons of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeGroupButtons(XML::Node& SelfRoot) const;
            /// @copydoc Renderable::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;

            /// @brief Take the data stored in an XML Node and overwrite the Group Buttons of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerializeGroupButtons(const XML::Node& SelfRoot);
            /// @copydoc Renderable::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

            /// @copydoc Renderable::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Event Methods

            /// @copydoc Checkbox::_OnSelected()
            virtual void _OnSelected();
            /// @copydoc Checkbox::_OnDeselected()
            virtual void _OnDeselected();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Gets the RadioButtonGroup this button belongs to.
            /// @return Returns a pointer to the RadioButtonGroup that this button belongs to, or NULL if it doesn't have a group.
            virtual RadioButtonGroup* _GetButtonGroup() const;
        };//RadioButton

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the factory implementation for RadioButton widgets.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB RadioButtonFactory : public WidgetFactory
        {
        public:
            /// @brief Class constructor.
            RadioButtonFactory() {  }
            /// @brief Class destructor.
            virtual ~RadioButtonFactory() {  }

            /// @copydoc WidgetFactory::GetWidgetTypeName() const
            virtual String GetWidgetTypeName() const;

            /// @brief Creates a new RadioButton.
            /// @param RendName The name to be given to the created RadioButton.
            /// @param Parent The screen the created RadioButton will belong to.
            /// @return Returns a pointer to the created RadioButton.
            virtual RadioButton* CreateRadioButton(const String& RendName, Screen* Parent);
            /// @brief Creates a new RadioButton.
            /// @param RendName The name to be given to the created RadioButton.
            /// @param RendRect The dimensions that will be assigned to the created RadioButton.
            /// @param Parent The screen the created RadioButton will belong to.
            /// @return Returns a pointer to the created RadioButton.
            virtual RadioButton* CreateRadioButton(const String& RendName, const UnifiedRect& RendRect, Screen* Parent);
            /// @brief Creates a new RadioButton.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created RadioButton will belong to.
            /// @return Returns a pointer to the created RadioButton.
            virtual RadioButton* CreateRadioButton(const XML::Node& XMLNode, Screen* Parent);

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
        };//RadioButtonFactory
    }//UI
}//Mezzanine

#endif
