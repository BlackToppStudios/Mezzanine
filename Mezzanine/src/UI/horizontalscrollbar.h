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
#ifndef _uihorizontalscrollbar_h
#define _uihorizontalscrollbar_h

#include "UI/scrollbar.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a scrollbar class aligned on the X axis.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB HorizontalScrollbar : public Scrollbar
        {
        public:
            /// @brief String containing the type name for this class: "HorizontalScrollbar".
            static const String TypeName;
        protected:
            friend class HorizontalScrollbarFactory;

            /// @copydoc QuadRenderable::CreateLayoutStrat()
            virtual void CreateLayoutStrat();
            /// @internal
            /// @brief Constructor helper function for creating a horizontally aligned scrollbar.
            /// @param ScrollStyle The button style to be applied to this scrollbar.
            virtual void ConstructHorizontalScrollbar(const UI::ScrollbarStyle& ScrollStyle);
            /// @copydoc Scrollbar::GetUpperScollLimit() const
            virtual Real GetUpperScrollLimit() const;
            /// @copydoc Scrollbar::GetLowerScrollLimit() const
            virtual Real GetLowerScrollLimit() const;
        //public:
            /// @brief Blank constructor.
            /// @param Parent The parent screen that created this scrollbar.
            HorizontalScrollbar(Screen* Parent);
            /// @brief Standard initialization constructor.
            /// @param RendName The name to be given to this scrollbar.
            /// @param Style An enum value representing how you want your scrollbar constructed.  See @ref UI::ScrollbarStyle enum for more info.
            /// @param Parent The parent screen that created this scrollbar.
            HorizontalScrollbar(const String& RendName, const UI::ScrollbarStyle& Style, Screen* Parent);
            /// @brief Rect constructor.
            /// @param RendName The name to be given to this scrollbar.
            /// @param RendRect The rect describing this widget's transform relative to it's parent.
            /// @param Style An enum value representing how you want your scrollbar constructed.  See @ref UI::ScrollbarStyle enum for more info.
            /// @param Parent The parent screen that created this scrollbar.
            HorizontalScrollbar(const String& RendName, const UnifiedRect& RendRect, const UI::ScrollbarStyle& Style, Screen* Parent);
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created HorizontalScrollbar will belong to.
            HorizontalScrollbar(const XML::Node& XMLNode, Screen* Parent);
            /// @brief Class destructor.
            virtual ~HorizontalScrollbar();
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @copydoc Widget::GetTypeName() const
            virtual const String& GetTypeName() const;

            /// @copydoc QuadRenderable::SetScrollerValue(const Real&)
            virtual void SetScrollerValue(const Real& Value);
            /// @copydoc QuadRenderable::GetScrollerValue() const
            virtual Real GetScrollerValue() const;
            /// @copydoc QuadRenderable::SetScrollerSize(const Real&)
            virtual void SetScrollerSize(const Real& Size);
            /// @copydoc QuadRenderable::GetScrollerSize() const
            virtual Real GetScrollerSize() const;
            /// @copydoc QuadRenderable::UpdateDimensions(const Rect&, const Rect&)
            virtual void UpdateDimensions(const Rect& OldSelfRect, const Rect& NewSelfRect);

            ///////////////////////////////////////////////////////////////////////////////
            // Visibility and Priority Methods

            /// @copydoc Renderable::SetVisible(Boolean)
            virtual void SetVisible(Boolean CanSee);
            /// @copydoc Renderable::Show()
            virtual void Show();
            /// @copydoc Renderable::Hide()
            virtual void Hide();

            ///////////////////////////////////////////////////////////////////////////////
            // PageProvider Methods

            /// @copydoc PageProvider::GetMaxXPages() const
            virtual Real GetMaxXPages() const;
            /// @copydoc PageProvider::GetMaxYPages() const
            virtual Real GetMaxYPages() const;

            /// @copydoc PageProvider::GetCurrentXPage() const
            virtual Real GetCurrentXPage() const;
            /// @copydoc PageProvider::GetCurrentYPage() const
            virtual Real GetCurrentYPage() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc QuadRenderable::ProtoSerializeChildQuads(XML::Node&) const
            virtual void ProtoSerializeChildQuads(XML::Node& SelfRoot) const;
            /// @copydoc QuadRenderable::ProtoDeSerializeChildQuads(const XML::Node&)
            virtual void ProtoDeSerializeChildQuads(const XML::Node& SelfRoot);

            /// @copydoc Renderable::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @copydoc PageProvider::_NotifyContainerUpdated()
            virtual void _NotifyContainerUpdated();
            /// @copydoc Scrollbar::_MouseScroll(const Vector2&)
            virtual Boolean _MouseScroll(const Vector2& MouseDelta);
            /// @copydoc Scrollbar::_MouseWheelScroll(const Integer)
            virtual Boolean _MouseWheelScroll(const Integer Direction);
            /// @copydoc Scrollbar::_ScrollBackScroll(const Vector2&)
            virtual Boolean _ScrollBackScroll(const Vector2& HitPosition);
            /// @copydoc Scrollbar::_ButtonScroll(Button*)
            virtual Boolean _ButtonScroll(Button* TheButton);
        };//HorizontalScrollbar

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the factory implementation for HorizontalScrollbar widgets.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB HorizontalScrollbarFactory : public WidgetFactory
        {
        public:
            /// @brief Class constructor.
            HorizontalScrollbarFactory() {  }
            /// @brief Class destructor.
            virtual ~HorizontalScrollbarFactory() {  }

            /// @copydoc WidgetFactory::GetWidgetTypeName() const
            virtual String GetWidgetTypeName() const;

            /// @brief Creates a new HorizontalScrollbar.
            /// @param RendName The name to be given to the created HorizontalScrollbar.
            /// @param Style An enum value representing how to position the button layout of the scrollbar.  See @ref UI::ScrollbarStyle enum for more info.
            /// @param Parent The screen the created HorizontalScrollbar will belong to.
            /// @return Returns a pointer to the created HorizontalScrollbar.
            virtual HorizontalScrollbar* CreateHorizontalScrollbar(const String& RendName, const UI::ScrollbarStyle& Style, Screen* Parent);
            /// @brief Creates a new HorizontalScrollbar.
            /// @param RendName The name to be given to the created HorizontalScrollbar.
            /// @param RendRect The dimensions that will be assigned to the created HorizontalScrollbar.
            /// @param Style An enum value representing how to position the button layout of the scrollbar.  See @ref UI::ScrollbarStyle enum for more info.
            /// @param Parent The screen the created HorizontalScrollbar will belong to.
            /// @return Returns a pointer to the created HorizontalScrollbar.
            virtual HorizontalScrollbar* CreateHorizontalScrollbar(const String& RendName, const UnifiedRect& RendRect, const UI::ScrollbarStyle& Style, Screen* Parent);
            /// @brief Creates a new HorizontalScrollbar.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created HorizontalScrollbar will belong to.
            /// @return Returns a pointer to the created HorizontalScrollbar.
            virtual HorizontalScrollbar* CreateHorizontalScrollbar(const XML::Node& XMLNode, Screen* Parent);

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
        };//HorizontalScrollbarFactory
    }//UI
}//Mezzanine

#endif
