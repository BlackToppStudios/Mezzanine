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

#ifndef _uipagedcontainer_h
#define _uipagedcontainer_h

#include "UI/widget.h"

namespace Mezzanine
{
    namespace UI
    {
        class PageProvider;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the EventArguments class for when a child of a paged container is selected.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB ChildSelectedArguments : public WidgetEventArguments
        {
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Public Data Members

            /// @brief The identification of the source firing this event.
            const String ChildName;
            /// @brief Boole storing whether or not the named child is gaining or losing focus.
            const Boole Selected;

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            /// @brief Class constructor.
            /// @param Name The name of the event being fired.
            /// @param Source The identification of the widget firing this event.
            /// @param Child The name of the child that has gained focus.
            /// @param Select True if the child is becoming the current selection in it's parent container, or false if it is losing the current selection in it's parent container.
            ChildSelectedArguments(const String& Name, const String& Source, const String& Child, const Boole Select) :
                WidgetEventArguments(Name,Source), ChildName(Child), Selected(Select) {  }
            /// @brief Class destructor.
            virtual ~ChildSelectedArguments() {  }

            ///////////////////////////////////////////////////////////////////////////////
            // CountedPtr Functionality

            /// @copydoc EventArguments::GetMostDerived()
            virtual ChildSelectedArguments* GetMostDerived()
                { return this; }
        };//ChildSelectedArguments
    }//UI

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a metaprogramming traits class used by ChildFocusEventArguments.
        /// @details This is need for an intrusive CountedPtr implementation.  Should a working external reference count be made this
        /// could be dropped in favor of a leaner implementation.
        ///////////////////////////////////////
        template <>
        class ReferenceCountTraits<UI::ChildSelectedArguments>
        {
        public:
            /// @brief Typedef communicating the reference count type to be used.
            typedef UI::ChildSelectedArguments RefCountType;

            /// @brief Method responsible for creating a reference count for a CountedPtr of the templated type.
            /// @param Target A pointer to the target class that is to be reference counted.
            /// @return Returns a pointer to a new reference counter for the templated type.
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief Enum used to decide the type of casting to be used by a reference counter of the templated type.
            enum { IsCastable = CastStatic };
        };//ReferenceCountTraits<ChildSelectedArguments>

    namespace UI
    {
        /// @brief Convenience typedef for passing around ChildSelectedArguments.
        typedef CountedPtr<ChildSelectedArguments> ChildSelectedArgumentsPtr;

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the base class for containers that have a render area and work area of different sizes.
        /// @details The central premise of all PagedLayoutContainers is that their render dimensions does not
        /// accurately express their total dimensions, or "work area" where widgets can be placed.  The work area
        /// of a PagedLayoutContainer can be nagivated via a small selection of specific widgets.
        ///////////////////////////////////////
        class MEZZ_LIB PagedContainer : public Widget
        {
        public:
            /// @brief Basic container type for Visible @ref Widget storage by this class.
            typedef std::vector<Widget*>                   VisibleChildContainer;
            /// @brief Iterator type for Visible @ref Widget instances stored by this class.
            typedef VisibleChildContainer::iterator        VisibleChildIterator;
            /// @brief Const Iterator type for Visible @ref Widget instances stored by this class.
            typedef VisibleChildContainer::const_iterator  ConstVisibleChildIterator;

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
            /// @brief Event name for when a child of this widget gets selected.
            static const String EventChildSelected;
        protected:
            /// @internal
            /// @brief A container of children that meet the criteria for rendering in this container.
            VisibleChildContainer VisibleChildren;
            /// @internal
            /// @brief Vector2 storing the size for all pages of this container.
            Vector2 WorkAreaSize;
            /// @internal
            /// @brief A pointer to the last child widget that was selected within this container.
            Widget* LastSelectedChild;
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
            /// @copydoc Widget::HandleChildStateChangeImpl(Widget*,const UInt32&,const UInt32&)
            virtual void HandleChildStateChangeImpl(Widget* Child, const UInt32& OldState, const UInt32& NewState);
            /// @internal
            /// @brief The container specific logic for updating it's dimensions.
            /// @param OldSelfRect The pre-update size of this widget.
            /// @param NewSelfRect The post-update size of this widget.
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

            /// @brief Checks the size of every child in this container and updates the work area to match the size needed.
            /// @note The work area size is automatically updated when a child is added or removed from a container.  You should
            /// only need to call this method manually if a childs size isn't determined yet when it is added to the container.
            virtual void UpdateWorkAreaSize() = 0;
            /// @brief Quickly updates the work area size based on a single childs' dimensions.
            /// @note The work area size is automatically updated when a child is added or removed from a container.  You should
            /// only need to call this method manually if a childs size isn't determined yet when it is added to the container.
            /// @param ChildSize The Unified dimensions describing the childs size.
            /// @param Adding Should be true if the child in question is being added to the container, false if it is being removed.
            virtual void QuickUpdateWorkAreaSize(const UnifiedVec2& ChildSize, Boole Adding) = 0;
            /// @brief Gets the size of this containers work area.
            /// @return Returns a const reference to a Vector2 containing this containers work area size in pixels.
            virtual const Vector2& GetWorkAreaSize() const;

            /// @copydoc QuadRenderable::UpdateDimensions(const Rect&, const Rect&)
            virtual void UpdateDimensions(const Rect& OldSelfRect, const Rect& NewSelfRect);
            /// @brief Forces an update of the visible children in this container.
            virtual void UpdateVisibleChildren();

            /// @brief Gets a pointer to the last selected child widget in this container.
            /// @return Returns a pointer to the child widget that was selected last.
            virtual Widget* GetLastSelectedChild() const;
            /// @brief Forces the currently selected child to become deselected.
            virtual void ClearSelectedChild();

            /// @brief Gets the current provider configuration of this container.
            /// @return Returns an enum describing the the provider configuration for this container.
            virtual ProviderMode GetProviderConfig() const;
            /// @brief Gets the role of the specified PageProvider in this container.
            /// @param Prov The PageProvider to check this container for.
            /// @return Returns a ProviderMode enum value representing how the specified PageProvider is providing for this container.
            virtual ProviderMode GetProviderConfig(const PageProvider* Prov) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Visibility and Priority Methods

            /// @copydoc Renderable::SetVisible(Boole)
            virtual void SetVisible(Boole CanSee);
            /// @copydoc Renderable::GetVisible() const
            virtual Boole GetVisible() const;
            /// @copydoc Renderable::IsVisible() const
            virtual Boole IsVisible() const;
            /// @copydoc Renderable::Show()
            virtual void Show();
            /// @copydoc Renderable::Hide()
            virtual void Hide();

            ///////////////////////////////////////////////////////////////////////////////
            // PagedContainer Configuration

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
            /// @brief Sets a single PageProvider as the provider for both the X and Y axis.
            /// @param Prov The PageProvider to be set for the X and Y axes.
            virtual void SetXYProvider(PageProvider* Prov);

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
            /// @copydoc QuadRenderable::AddChild(Widget*,const UInt16)
            virtual void AddChild(Widget* Child, const UInt16 ZOrder);
            /// @copydoc QuadRenderable::RemoveChild(Widget*)
            virtual void RemoveChild(Widget* ToBeRemoved);
            /// @copydoc QuadRenderable::RemoveAllChildren()
            virtual void RemoveAllChildren();
            /// @copydoc QuadRenderable::DestroyChild(Widget*)
            virtual void DestroyChild(Widget* ToBeDestroyed);
            /// @copydoc QuadRenderable::DestroyAllChildren()
            virtual void DestroyAllChildren();

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert the PageProvider data of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializePageData(XML::Node& SelfRoot) const;
            /// @copydoc Renderable::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;

            /// @brief Take the data stored in an XML Node and overwrite the PageProvider data of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            virtual void ProtoDeSerializePageData(const XML::Node& SelfRoot);
            /// @copydoc Renderable::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);

            /// @copydoc Renderable::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Event Methods

            /// @brief Self logic to be executed when focus is given to a child of this widget.
            /// @param ChildName The name of the child that has gained focus.
            /// @param Selected A Boole indicating whether the child is being selected or deselected.
            virtual void _OnChildSelected(const String& ChildName, const Boole Selected);

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @copydoc QuadRenderable::_AppendRenderDataCascading(ScreenRenderData&)
            virtual void _AppendRenderDataCascading(ScreenRenderData& RenderData);
        };//PagedContainer
    }//UI
}//Mezzanine

#endif
