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

#ifndef _uihorizontalcontainer_h
#define _uihorizontalcontainer_h

#include "UI/linearcontainer.h"

namespace Mezzanine
{
    namespace UI
    {
        class HorizontalScrollbar;
        class Spinner;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A layout container that aligns it's children along a common X axis.
        /// @details This class uses a HorizontalLayoutStrategy.
        ///////////////////////////////////////
        class MEZZ_LIB HorizontalContainer : public LinearContainer
        {
        public:
            /// @brief String containing the type name for this class: "HorizontalContainer".
            static const String TypeName;
        protected:
            /// @copydoc LayoutContainer::UpdateContainerDimensionsImpl(const Rect&, const Rect&)
            virtual void UpdateContainerDimensionsImpl(const Rect& OldSelfRect, const Rect& NewSelfRect);
        public:
            /// @brief Blank constructor.
            /// @param Parent The parent Screen that created this widget.
            HorizontalContainer(Screen* Parent);
            /// @brief Standard initialization constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param Parent The parent Screen that created this widget.
            HorizontalContainer(const String& RendName, Screen* Parent);
            /// @brief Rect constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param RendRect The rect describing this widget's transform relative to it's parent.
            /// @param Parent The parent screen that created this renderable.
            HorizontalContainer(const String& RendName, const UnifiedRect& RendRect, Screen* Parent);
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created HorizontalContainer will belong to.
            HorizontalContainer(const XML::Node& XMLNode, Screen* Parent);
            /// @brief Class destructor.
            virtual ~HorizontalContainer();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc PagedContainer::UpdateWorkAreaSize()
            virtual void UpdateWorkAreaSize();
            /// @copydoc PagedContainer::QuickUpdateWorkAreaSize(const UnifiedVec2&, Boole)
            virtual void QuickUpdateWorkAreaSize(const UnifiedVec2& ChildSize, Boole Adding);

            /// @copydoc Widget::GetTypeName() const
            virtual const String& GetTypeName() const;

            /// @copydoc PagedContainer::SetProviders(PageProvider*, PageProvider*)
            virtual void SetProviders(PageProvider* XProv, PageProvider* YProv);
            /// @copydoc PagedContainer::SetXProvider(PageProvider*)
            virtual void SetXProvider(PageProvider* XProv);
            /// @copydoc PagedContainer::SetYProvider(PageProvider*)
            virtual void SetYProvider(PageProvider* YProv);

            /// @brief Convenience method for configuring a HorizontalScrollbar as the page provider for this container.
            /// @param Prov A pointer to the HorizontalScrollbar which will determine which page this container is rendering.
            virtual void SetScrollbarProvider(HorizontalScrollbar* Prov);
            /// @brief Convenience method for configuring a Spinner as the page provider for this container.
            /// @param Prov A pointer to the Spinner which will determine which page this container is rendering.
            virtual void SetSpinnerProvider(Spinner* Prov);

            ///////////////////////////////////////////////////////////////////////////////
            // Child Management

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc Renderable::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
            /// @copydoc Renderable::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

            /// @copydoc Renderable::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods
        };//HorizontalContainer

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the factory implementation for HorizontalContainer widgets.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB HorizontalContainerFactory : public WidgetFactory
        {
        public:
            /// @brief Class constructor.
            HorizontalContainerFactory() {  }
            /// @brief Class destructor.
            virtual ~HorizontalContainerFactory() {  }

            /// @copydoc WidgetFactory::GetWidgetTypeName() const
            virtual String GetWidgetTypeName() const;

            /// @brief Creates a new HorizontalContainer.
            /// @param RendName The name to be given to the created HorizontalContainer.
            /// @param Parent The screen the created HorizontalContainer will belong to.
            /// @return Returns a pointer to the created HorizontalContainer.
            virtual HorizontalContainer* CreateHorizontalContainer(const String& RendName, Screen* Parent);
            /// @brief Creates a new HorizontalContainer.
            /// @param RendName The name to be given to the created HorizontalContainer.
            /// @param RendRect The dimensions that will be assigned to the created HorizontalContainer.
            /// @param Parent The screen the created HorizontalContainer will belong to.
            /// @return Returns a pointer to the created HorizontalContainer.
            virtual HorizontalContainer* CreateHorizontalContainer(const String& RendName, const UnifiedRect& RendRect, Screen* Parent);
            /// @brief Creates a new HorizontalContainer.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created HorizontalContainer will belong to.
            /// @return Returns a pointer to the created HorizontalContainer.
            virtual HorizontalContainer* CreateHorizontalContainer(const XML::Node& XMLNode, Screen* Parent);

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
        };//HorizontalContainerFactory
    }//UI
}//Mezzanine

#endif
