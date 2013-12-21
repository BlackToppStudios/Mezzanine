//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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

#ifndef _uipagedcontainer_h
#define _uipagedcontainer_h

#include "UI/widget.h"

namespace Mezzanine
{
    namespace UI
    {
        class PageProvider;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class WidgetEventArguments
        /// @headerfile widget.h
        /// @brief This is the base class for widget specific event arguments.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB ChildFocusEventArguments : public WidgetEventArguments
        {
        public:
            /// @brief The identification of the source firing this event.
            const String ChildName;

            /// @brief Class constructor.
            /// @param Name The name of the event being fired.
            /// @param Source The identification of the widget firing this event.
            /// @param Child The name of the child that has gained focus.
            ChildFocusEventArguments(const String& Name, const String& Source, const String& Child) :
                WidgetEventArguments(Name,Source), ChildName(Child) {  }
            /// @brief Class destructor.
            virtual ~ChildFocusEventArguments() {  }
        };//ChildFocusEventArguments

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the base class for containers that have a render area and work area of different sizes.
        /// @details The central premise of all PagedLayoutContainers is that their render dimensions does not
        /// accurately express their total dimensions, or "work area" where widgets can be placed.  The work area
        /// of a PagedLayoutContainer can be nagivated via a small selection of specific widgets.
        ///////////////////////////////////////
        class MEZZ_LIB PagedContainer : public Widget
        {
        public:
            /// @brief Basic container type for Visible @ref QuadRenderable storage by this class.
            typedef std::vector<QuadRenderable*>           VisibleChildContainer;
            /// @brief Iterator type for Visible @ref QuadRenderable instances stored by this class.
            typedef VisibleChildContainer::iterator        VisibleChildIterator;
            /// @brief Const Iterator type for Visible @ref QuadRenderable instances stored by this class.
            typedef VisibleChildContainer::const_iterator  ConstVisibleChildIterator;

            /// @enum ProviderMode
            /// @brief An enum describing how the providers for this container are configured and being used.
            /// @details Depending on the method returning the values, these values can mean slightly different things.
            /// The documentation provided below describes the meaning when querying the overall configuration first, and
            /// the second description describes what it means when an individual provider is queried on a given container.
            enum ProviderMode
            {
                PM_Error          = 0,   ///< The PageProvider configuration is invalid, or the queried PageProvider isn't in use by the container.
                PM_Single_X       = 1,   ///< There is only one PageProvider providing pages for the X axis of this container, or the queried provider is being used only for the X axis by the container.
                PM_Single_Y       = 2,   ///< There is only one PageProvider providing pages for the Y axis of this container, or the queried provider is being used only for the Y axis by the container.
                PM_Single_XY      = 3,   ///< There is only one PageProvider providing pages for both the X and Y axes of this container, or the queried provider is being used for both the X and Y axes be the container.
                PM_Dual_XY        = 4    ///< There are two different PageProviders each providing pages for their respective axis.  This value isn't returned by a provider query.
            };

            /// @brief String containing the type name for this class: "PagedContainer".
            static const String TypeName;
            /// @brief Event name for when a child of this widget gains focus.
            static const String EventChildFocusGained;
        protected:
            /// @internal
            /// @brief Unified rect storing the size alloted for children of this container.
            UnifiedVec2 WorkAreaSize;
            /// @internal
            /// @brief A container of children that meet the criteria for rendering in this container.
            VisibleChildContainer VisibleChildren;
            /// @internal
            /// @brief A pointer to the last child widget that was focused by this container.
            Widget* LastFocusedChild;
            /// @internal
            /// @brief A pointer to the X axis provider.
            PageProvider* XProvider;
            /// @internal
            /// @brief A pointer to the Y axis provider.
            PageProvider* YProvider;

            /// @copydoc Renderable::ProtoSerializeImpl(XML::Node&) const
            virtual void ProtoSerializeImpl(XML::Node& SelfRoot) const;
            /// @copydoc Renderable::ProtoDeSerializeImpl(const XML::Node&)
            virtual void ProtoDeSerializeImpl(const XML::Node& SelfRoot);
            /// @internal
            /// @brief The container specific logic for updating it's dimensions.
            virtual void UpdateContainerDimensionsImpl(const Rect& OldSelfRect, const Rect& NewSelfRect) = 0;
        public:
            /// @brief Blank constructor.
            /// @param Parent The parent Screen that created this widget.
            PagedContainer(Screen* Parent);
            /// @brief Standard initialization constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param Parent The parent Screen that created this widget.
            PagedContainer(const String& RendName, Screen* Parent);
            /// @brief Rect constructor.
            /// @param RendName The name to be given to this renderable.
            /// @param RendRect The rect describing this widget's transform relative to it's parent.
            /// @param Parent The parent screen that created this renderable.
            PagedContainer(const String& RendName, const UnifiedRect& RendRect, Screen* Parent);
            /// @brief Class destructor.
            virtual ~PagedContainer();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Sets the size of this containers work area.
            /// @note The relative portion of the Unified dimensions passed in are relative to the containers parent, just like the
            /// containers own dimensions.  This can be larger than the containers dimensions, and in fact should be larger to justify
            /// using this class or more derived child classes.
            /// @param Area The new size for this containers work area.
            virtual void SetWorkAreaSize(const UnifiedVec2& Area);
            /// @brief Gets the size of this containers work area.
            /// @return Returns a const reference to this containers work area size.
            virtual const UnifiedVec2& GetWorkAreaSize() const;
            /// @brief Gets the size of this containers work area in pixels.
            /// @return Returns a Vector2 containing the actual pixel size of this containers work area.
            virtual Vector2 GetActualWorkAreaSize() const;

            /// @copydoc QuadRenderable::UpdateDimensions(const Rect&, const Rect&)
            virtual void UpdateDimensions(const Rect& OldSelfRect, const Rect& NewSelfRect);

            /// @brief Gets a pointer to the last focused child widget in this container.
            /// @return Returns a pointer to the child widget that last gained focus.
            virtual Widget* GetLastFocusedWidget() const;

            /// @brief Gets the current provider configuration of this container.
            /// @return Returns an enum describing the the provider configuration for this container.
            virtual ProviderMode GetProviderConfig() const;
            /// @brief Gets the role of the specified PageProvider in this container.
            /// @param Prov The PageProvider to check this container for.
            /// @return Returns a ProviderMode enum value representing how the specified PageProvider is providing for this container.
            virtual ProviderMode GetProviderConfig(const PageProvider* Prov) const;

            /// @brief Sets the page providers for both axes.
            /// @param XProv The PageProvider controlling the current horizontal page.
            /// @param YProv The PageProvider controlling the current vertical page.
            virtual void SetProviders(PageProvider* XProv, PageProvider* YProv) = 0;
            /// @brief Sets the PageProvider for the X axis.
            /// @param XProv The PageProvider controlling the current horizontal page.
            virtual void SetXProvider(PageProvider* XProv) = 0;
            /// @brief Sets the PageProvider for the Y axis.
            /// @param YProv The PageProvider controlling the current vertical page.
            virtual void SetYProvider(PageProvider* YProv) = 0;

            /// @brief Gets the PageProvider for the X axis.
            /// @return Returns a pointer to the PageProvider for the X axis of this container.
            virtual PageProvider* GetXProvider() const;
            /// @brief Gets the PageProvider for the Y axis.
            /// @return Returns a pointer to the PageProvider for the Y axis of this container.
            virtual PageProvider* GetYProvider() const;

            /// @brief Unbinds a provider being used by this container.
            /// @param Prov A pointer to the PageProvider to be unbound if it is used by this container.
            virtual void UnbindProvider(PageProvider* Prov);

            ///////////////////////////////////////////////////////////////////////////////
            // Child Management

            /// @copydoc QuadRenderable::AddChild(Widget*)
            virtual void AddChild(Widget* Child);
            /// @copydoc QuadRenderable::RemoveChild(Widget*)
            virtual void RemoveChild(Widget* Child);
            /// @copydoc QuadRenderable::RemoveAllChildren()
            virtual void RemoveAllChildren();

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert the PageProvider data of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializePageData(XML::Node& SelfRoot) const;
            /// @copydoc Renderable::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;

            /// @brief Take the data stored in an XML Node and overwrite the PageProvider data of this object with it.
            /// @param SelfRoo tAn XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerializePageData(const XML::Node& SelfRoot);
            /// @copydoc Renderable::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

            /// @copydoc Renderable::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Event Methods

            /// @brief Self logic to be executed when focus is given to a child of this widget.
            /// @param ChildName The name of the child that has gained focus.
            virtual void _OnChildFocusGained(const String& ChildName);

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @copydoc EventSubscriber::_NotifyEvent(const EventArguments& Args)
            virtual void _NotifyEvent(const EventArguments& Args);
            /// @copydoc QuadRenderable::_AppendRenderDataCascading(ScreenRenderData&)
            virtual void _AppendRenderDataCascading(ScreenRenderData& RenderData);
        };//PagedContainer
    }//UI
}//Mezzanine

#endif
