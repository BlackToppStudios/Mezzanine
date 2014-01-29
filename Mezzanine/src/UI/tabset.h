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
#ifndef _uitabset_h
#define _uitabset_h

#include "UI/stackedcontainer.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a widget that stores sets of renderables but only displays one at a time.
        /// @details This widget is useful for tabbed option displays.
        ///////////////////////////////////////
        class MEZZ_LIB TabSet : public StackedContainer
        {
        public:
            /// @brief Convenience typedef for objects that will be used to represent tabs in this TabSet.
            typedef Widget                                    TabbedSubSet;
            /// @brief Container class for storing @ref TabbedSubSet instances stored by this class.
            typedef std::map<TabbedSubSet*,UInt16>            TabbedSubSetContainer;
            /// @brief Iterator type for @ref TabbedSubSet instances stored by this class.
            typedef TabbedSubSetContainer::iterator           TabbedSubSetIterator;
            /// @brief Const Iterator type for @ref TabbedSubSet instances stored stored by this class.
            typedef TabbedSubSetContainer::const_iterator     ConstTabbedSubSetIterator;

            /// @brief String containing the type name for this class: "TabSet".
            static const String TypeName;
        protected:
            friend class TabSetFactory;
            /// @internal
            /// @brief A container storing all the bindings to
            TabbedSubSetContainer SubSetBindings;
            /// @internal
            /// @brief A pointer to the only visible child of this TabSet.
            TabbedSubSet* VisibleChild;

            /// @copydoc Renderable::ProtoSerializeImpl(XML::Node&) const
            virtual void ProtoSerializeImpl(XML::Node& SelfRoot) const;
            /// @copydoc Renderable::ProtoDeSerializeImpl(const XML::Node&)
            virtual void ProtoDeSerializeImpl(const XML::Node& SelfRoot);
        //public:
            /// @brief Blank constructor.
            /// @param Parent The parent Screen that created this widget.
            TabSet(Screen* Parent);
            /// @brief Standard initialization constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param Parent The parent Screen that created this widget.
            TabSet(const String& RendName, Screen* Parent);
            /// @brief Rect constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param RendRect The rect describing this widget's transform relative to it's parent.
            /// @param Parent The parent screen that created this renderable.
            TabSet(const String& RendName, const UnifiedRect& RendRect, Screen* Parent);
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created TabSet will belong to.
            TabSet(const XML::Node& XMLNode, Screen* Parent);
            /// @brief Class destructor.
            virtual ~TabSet();
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @copydoc Widget::GetTypeName() const
            virtual const String& GetTypeName() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Visibility and Priority Methods

            /// @copydoc Renderable::SetVisible(Boolean)
            virtual void SetVisible(Boolean CanSee);
            /// @copydoc Renderable::Show()
            virtual void Show();
            /// @copydoc Renderable::Hide()
            virtual void Hide();

            ///////////////////////////////////////////////////////////////////////////////
            // TabSet Properties

            ///////////////////////////////////////////////////////////////////////////////
            // TabSet Configuration

            /// @copydoc StackedContainer::SetButtonCongig(const UInt16, StackButton*)
            /// @note The UInt16 passed into this method should represent the ZOrder of the TabbedSubSet it will make visible.
            virtual void SetButtonConfig(const UInt16 Config, StackButton* ConfigButton);
            /// @copydoc StackedContainer::GetButtonConfig(const StackButton*) const
            /// @note This method will return the ZOrder of the TabbedSubSet it will make visible.  This will return 0 if the button is invalid.
            virtual UInt16 GetButtonConfig(const StackButton* ConfigButton) const;

            /// @brief Creates a new TabbedSubSet for this TabSet.
            /// @note The ZOrder given to the created TabbedSubSet will be used as an ID for setting the visible child of this TabSet.
            /// Ideally ZOrder "0" should not be used since it is returned in the event of an error condition when querying for a button configuration.
            /// @param Name The name to be given to the TabbedSubSet.
            /// @param ChildZOrder The ZOrder to be assigned to the TabbedSubSet.
            /// @return Returns a pointer to the created TabbedSubSet.
            virtual TabbedSubSet* CreateTabbedSubSet(const String& Name, const UInt16 ChildZOrder);

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc Renderable::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
            /// @brief Convert the Button Bindings of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeButtonBindings(XML::Node& SelfRoot) const;

            /// @copydoc Renderable::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);
            /// @brief Take the data stored in an XML Node and overwrite the Button Bindings of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerializeButtonBindings(const XML::Node& SelfRoot);

            /// @copydoc Renderable::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Event Methods

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @copydoc StackedContainer::_NotifyButtonSelected(StackedButton*)
            virtual void _NotifyButtonSelected(StackButton* Selected);
            /// @copydoc QuadRenderable::_AppendRenderDataCascading(ScreenRenderData&)
            virtual void _AppendRenderDataCascading(ScreenRenderData& RenderData);
        };//TabSet

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the factory implementation for TabSet widgets.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB TabSetFactory : public WidgetFactory
        {
        public:
            /// @brief Class constructor.
            TabSetFactory() {  }
            /// @brief Class destructor.
            virtual ~TabSetFactory() {  }

            /// @copydoc WidgetFactory::GetWidgetTypeName() const
            virtual String GetWidgetTypeName() const;

            /// @brief Creates a new TabSet.
            /// @param RendName The name to be given to the created TabSet.
            /// @param Parent The screen the created TabSet will belong to.
            /// @return Returns a pointer to the created TabSet.
            virtual TabSet* CreateTabSet(const String& RendName, Screen* Parent);
            /// @brief Creates a new TabSet.
            /// @param RendName The name to be given to the created TabSet.
            /// @param RendRect The dimensions that will be assigned to the created TabSet.
            /// @param Parent The screen the created TabSet will belong to.
            /// @return Returns a pointer to the created TabSet.
            virtual TabSet* CreateTabSet(const String& RendName, const UnifiedRect& RendRect, Screen* Parent);
            /// @brief Creates a new TabSet.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created TabSet will belong to.
            /// @return Returns a pointer to the created TabSet.
            virtual TabSet* CreateTabSet(const XML::Node& XMLNode, Screen* Parent);

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
        };//TabSetFactory
    }//UI
}//Mezzanine

#endif
