// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _uistackbutton_h
#define _uistackbutton_h

#include "UI/button.h"

namespace Mezzanine
{
    namespace UI
    {
        class StackedContainer;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a button with additional data used to track the binding to a StackedContainer which can be serialized.
        /// @details This class does not use a LayoutStrategy.
        ///////////////////////////////////////
        class StackButton : public Button
        {
        public:
            /// @brief String containing the type name for this class: "StackButton".
            static const String TypeName;
        protected:
            /// @internal
            /// @brief A pointer storing the StackedContainer this button is bound to.
            StackedContainer* BoundContainer;
        public:
            /// @brief Blank constructor.
            /// @param Parent The parent Screen that created this widget.
            StackButton(Screen* Parent);
            /// @brief Standard initialization constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param Parent The parent Screen that created this widget.
            StackButton(const String& RendName, Screen* Parent);
            /// @brief Rect constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param RendRect The rect describing this widget's transform relative to it's parent.
            /// @param Parent The parent screen that created this renderable.
            StackButton(const String& RendName, const UnifiedRect& RendRect, Screen* Parent);
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created Button will belong to.
            StackButton(const XML::Node& XMLNode, Screen* Parent);
            /// @brief Standard destructor.
            virtual ~StackButton();
        //public:
            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @brief Gets the container that is currently bound to this button.
            /// @return Returns a pointer to the StackedContainer currently bound to this button.
            virtual StackedContainer* GetBoundContainer() const;

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

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Notifies this StackButton that a StackedContainer has been bound to it.
            /// @param ToBeBound A pointer to the StackedContainer that this StackButton will be bound to.
            virtual void _SetBoundContainer(StackedContainer* ToBeBound);
        };//StackButton

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the factory implementation for StackButton widgets.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB StackButtonFactory : public WidgetFactory
        {
        public:
            /// @brief Class constructor.
            StackButtonFactory() {  }
            /// @brief Class destructor.
            virtual ~StackButtonFactory() {  }

            /// @copydoc WidgetFactory::GetWidgetTypeName() const
            virtual String GetWidgetTypeName() const;

            /// @brief Creates a new StackButton.
            /// @param RendName The name to be given to the created StackButton.
            /// @param Parent The screen the created StackButton will belong to.
            /// @return Returns a pointer to the created StackButton.
            virtual StackButton* CreateStackButton(const String& RendName, Screen* Parent);
            /// @brief Creates a new StackButton.
            /// @param RendName The name to be given to the created StackButton.
            /// @param RendRect The dimensions that will be assigned to the created StackButton.
            /// @param Parent The screen the created StackButton will belong to.
            /// @return Returns a pointer to the created StackButton.
            virtual StackButton* CreateStackButton(const String& RendName, const UnifiedRect& RendRect, Screen* Parent);
            /// @brief Creates a new StackButton.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created StackButton will belong to.
            /// @return Returns a pointer to the created StackButton.
            virtual StackButton* CreateStackButton(const XML::Node& XMLNode, Screen* Parent);

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
        };//StackButtonFactory
    }//UI
}//Mezzanine

#endif
