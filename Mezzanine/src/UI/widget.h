//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _uiwidget_h
#define _uiwidget_h

#include "UI/quadrenderable.h"
#include "UI/widgetfactory.h"
#include "Input/metacode.h"
#include "eventpublisher.h"
#include "eventsubscriber.h"

namespace Mezzanine
{
    namespace UI
    {
        class Button;
        class RenderableContainer;
        class GenericWidgetFactory;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class WidgetEventArguments
        /// @headerfile widget.h
        /// @brief This is the base class for widget specific event arguments.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB WidgetEventArguments : public EventArguments
        {
        public:
            /// @brief The identification of the source firing this event.
            const String WidgetName;

            /// @brief Class constructor.
            /// @param Name The name of the event being fired.
            /// @param Source The identification of the widget firing this event.
            WidgetEventArguments(const String& Name, const String& Source) :
                EventArguments(Name), WidgetName(Source) {  }
            /// @brief Class destructor.
            virtual ~WidgetEventArguments() {  }
        };//WidgetEventArguments

        ///////////////////////////////////////////////////////////////////////////////
        /// @class Widget
        /// @headerfile widget.h
        /// @brief This is the base class for all widgets.
        /// @details A widget is really a mechanism for implementing not-so-generic UI behaviors.  They
        /// are control stuctures allowing UI elements to interact with each other and potentially classes
        /// outside the UI system(through the event system). @n @n
        /// This class automatically creates a "Normal" and "Hovered" RenderLayerGroup that are used with
        /// the events of this class, these groups should be used and additional groups only made for special
        /// cases.
        ///////////////////////////////////////
        class MEZZ_LIB Widget : public QuadRenderable, public EventPublisher, public EventSubscriber
        {
        public:
            /// @enum WidgetState
            /// @brief Enum describing the current state of the widget.
            enum WidgetState
            {
                WS_Untouched = 0,
                WS_Hovered   = 1,
                WS_Focused   = 2,
                WS_Dragged   = 4
            };

            /// @brief Container class for storing @ref RenderLayerGroup instances in relation to widget states.
            typedef std::map<UInt32,RenderLayerGroup*>       StateLayerGroupMap;
            /// @brief Iterator type for @ref RenderLayerGroup instances stored in relation to widget states.
            typedef StateLayerGroupMap::iterator             StateLayerGroupIterator;
            /// @brief Const Iterator type for @ref RenderLayerGroup instances stored in relation to widget states.
            typedef StateLayerGroupMap::const_iterator       ConstStateLayerGroupIterator;

            /// @brief String containing the type name for this class: "GenericWidget".
            static const String TypeName;
            /// @brief Event name for when the mouse enters this widget.
            static const String EventMouseEnter;
            /// @brief Event name for when the mouse leaves this widget.
            static const String EventMouseExit;
            /// @brief Event name for when the mouse starts dragging this widget.
            static const String EventMouseDragStart;
            /// @brief Event name for when a mouse activation button is pressed, and held while moving.
            static const String EventMouseDragging;
            /// @brief Event name for when the mouse stops dragging this widget.
            static const String EventMouseDragEnd;
            /// @brief Event name for when this widget gains focus.
            static const String EventFocusGained;
            /// @brief Event name for when this widget loses focus.
            static const String EventFocusLost;
            /// @brief Event name for when the system locks focus on this widget.
            static const String EventFocusLocked;
            /// @brief Event name fow when the system removes the focus lock from this widget.
            static const String EventFocusUnlocked;
            /// @brief Event name for when this widget is switched from being hidden to being shown.
            static const String EventVisibilityShown;
            /// @brief Event name for when this widget is switched from being shown to being hidden.
            static const String EventVisibilityHidden;
        protected:
            friend class GenericWidgetFactory;
            /// @internal
            /// @brief Map containing all the RenderLayerGroups bound to specific widget states.
            StateLayerGroupMap StateGroupBindings;
            /// @internal
            /// @brief The child widget of this widget the mouse is over, if any.
            Widget* HoveredSubWidget;
            /// @internal
            /// @brief UInt32 describing the current state of this widget.
            UInt32 State;

            /// @copydoc Renderable::ProtoSerializeImpl(XML::Node&) const
            virtual void ProtoSerializeImpl(XML::Node& SelfRoot) const;
            /// @copydoc Renderable::ProtoDeSerializeImpl(const XML::Node&)
            virtual void ProtoDeSerializeImpl(const XML::Node& SelfRoot);
            /// @internal
            /// @brief Consumes input for this widget's use.
            /// @param Code The input to be processed.
            /// @return Returns true if the input was handled, false otherwise.
            virtual Bool HandleInputImpl(const Input::MetaCode& Code);
            /// @internal
            /// @brief Contains all the common necessary startup initializations for this class.
            void ConstructWidget();
        //public:
            /// @brief Blank constructor.
            /// @param Parent The parent Screen that created this widget.
            Widget(Screen* Parent);
            /// @brief Standard initialization constructor.
            /// @param RendName The name to be given to this widget.
            /// @param Parent The parent Screen that created this widget.
            Widget(const String& RendName, Screen* Parent);
            /// @brief Rect constructor.
            /// @param RendName The name to be given to this widget.
            /// @param RendRect The rect describing this widget's transform relative to it's parent.
            /// @param Parent The parent screen that created this widget.
            Widget(const String& RendName, const UnifiedRect& RendRect, Screen* Parent);
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            /// @param Parent The screen the created Widget will belong to.
            Widget(const XML::Node& XMLNode, Screen* Parent);
            /// @brief Standard destructor.
            virtual ~Widget();
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @copydoc Renderable::GetRenderableType() const
            RenderableType GetRenderableType() const;
            /// @brief Gets the type of widget this is.
            /// @return Returns a const String reference representing the type of widget this is.
            virtual const String& GetTypeName() const;
            /// @brief Gets the result of the last mouse hover check.
            /// @return Returns whether or not the mouse was hovering over this widget during the last check.
            virtual Bool IsHovered() const;
            /// @brief Gets whether or not this widget currently has focus.
            /// @return True if this widget has focus, false otherwise.
            virtual Bool HasFocus() const;
            /// @brief Gets whether or not this widget is being dragged.
            /// @return Returns true if this widget is being dragged, false otherwise.
            virtual Bool IsBeingDragged() const;

            ///////////////////////////////////////////////////////////////////////////////
            // State-LayerGroup Binding Methods

            /// @brief Binds a RenderLayerGroup to a WidgetState.
            /// @param BindState The WidgetState to be bound to.
            /// @param ToBind The RenderLayerGroup to be bound to the specified state.
            virtual void BindGroupToState(const UInt32 BindState, RenderLayerGroup* ToBind);
            /// @brief Gets the current RenderLayerGroup bound to a specified WidgetState.
            /// @param BindState The WidgetState bound to the RenderLayerGroup being retrieved.
            /// @return Returns a pointer to the RenderLayerGroup bound to the specified WidgetState, or NULL if none are bound.
            virtual RenderLayerGroup* GetGroupBoundToState(const UInt32 BindState) const;
            /// @brief Sets the group bound to the specified WidgetState as active.
            /// @param BindState The WidgetState bound to the RenderLayerGroup being set.
            /// @return Returns true if the active RenderLayerGroup has changed, false otherwise.
            virtual bool SetGroupFromState(const UInt32 BindState);

            ///////////////////////////////////////////////////////////////////////////////
            // Fetch Methods

            /// @brief Gets the hovered sub-widget within this widget, if any.
            /// @return Returns a pointer to the sub-widget within this widget the mouse is hovering over, or NULL if none.
            Widget* GetHoveredSubWidget() const;
            /// @brief Gets a pointer to the Widget at the bottom of the hovered SubWidget chain.
            /// @return Returns a pointer to the hovered sub-widget of the hovered sub-widget, etc., until you reach the end of the chain.
            Widget* GetBottomMostHoveredWidget();

            ///////////////////////////////////////////////////////////////////////////////
            // Visibility and Priority Methods

            /// @copydoc Renderable::SetVisible(Bool)
            virtual void SetVisible(Bool CanSee);
            /// @copydoc Renderable::GetVisible() const
            virtual Bool GetVisible() const;
            /// @copydoc Renderable::IsVisible() const
            virtual Bool IsVisible() const;
            /// @copydoc Renderable::Show()
            virtual void Show();
            /// @copydoc Renderable::Hide()
            virtual void Hide();

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @copydoc Renderable::ProtoSerializeProperties(XML::Node&) const
            virtual void ProtoSerializeProperties(XML::Node& SelfRoot) const;
            /// @brief Convert the state-group bindings of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeStateGroupBindings(XML::Node& SelfRoot) const;
            /// @brief Convert the Events of this class to an XML::Node ready for serialization.
            /// @param SelfRoot The root node containing all the serialized data for this instance.
            virtual void ProtoSerializeEvents(XML::Node& SelfRoot) const;

            /// @copydoc Renderable::ProtoDeSerializeProperties(const XML::Node&)
            virtual void ProtoDeSerializeProperties(const XML::Node& SelfRoot);
            /// @brief Take the data stored in an XML Node and overwrite the state-group bindings of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerializeStateGroupBindings(const XML::Node& SelfRoot);
            /// @brief Take the data stored in an XML Node and overwrite the Events of this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            virtual void ProtoDeSerializeEvents(const XML::Node& SelfRoot);

            /// @copydoc Renderable::GetDerivedSerializableName() const
            virtual String GetDerivedSerializableName() const;
            /// @copydoc Renderable::GetSerializableName()
            static String GetSerializableName();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Event Methods

            /// @brief Self logic to be executed when the mouse cursor enters the bounds of this widget.
            /// @details This method should be exclusively called by the UIManager.
            virtual void _OnMouseEnter();
            /// @brief Self logic to be executed when the mouse cursor leaves the bounds of thiw widget.
            /// @details This method should be exclusively called by the UIManager.
            virtual void _OnMouseExit();
            /// @brief Self logic to be executed when the mouse cursor starts dragging across the bounds of this widget.
            /// @details This method should be exclusively called by the UIManager.
            virtual void _OnMouseDragStart();
            /// @brief Self logic to be executed when the mouse cursor is dragging across the bounds of this widget.
            /// @details This method should be exclusively called by the UIManager.
            virtual void _OnMouseDragging();
            /// @brief Self logic to be executed when the mouse cursor stops dragging across the bounds of this widget.
            /// @details This method should be exclusively called by the UIManager.
            virtual void _OnMouseDragEnd();
            /// @brief Self logic to be executed when focus is given to this widget.
            /// @details This method should be exclusively called by the UIManager.
            virtual void _OnFocusGained();
            /// @brief Self logic to be executed when focus is removed from this widget.
            /// @details This method should be exclusively called by the UIManager.
            virtual void _OnFocusLost();
            /// @brief Self logic to be executed when focus is locked to this widget.
            /// @details This method should be exclusively called by the UIManager.
            virtual void _OnFocusLocked();
            /// @brief Self logic to be executed when focus is no longer locked to this widget.
            /// @details This method should be exclusively called by the UIManager.
            virtual void _OnFocusUnlocked();
            /// @brief Self logic to be executed when this widget becomes visible.
            virtual void _OnVisibilityShown();
            /// @brief Self logic to be executed when this widget becomes invisible.
            virtual void _OnVisibilityHidden();

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Handles input passed to this widget.
            /// @note This input will be passed up if not consumed by this widget.
            /// @param Code The MetaCode to be processed.
            /// @return Returns true if this input was consumed/handled, false otherwise.
            virtual bool _HandleInput(const Input::MetaCode& Code);
            /// @copydoc EventSubscriber::_NotifyEvent(const EventArguments& Args)
            virtual void _NotifyEvent(const EventArguments& Args);
        };//Widget

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the factory implementation for generic widgets.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB GenericWidgetFactory : public WidgetFactory
        {
        public:
            /// @brief Class constructor.
            GenericWidgetFactory() {  }
            /// @brief Class destructor.
            virtual ~GenericWidgetFactory() {  }

            /// @copydoc WidgetFactory::GetWidgetTypeName() const
            virtual String GetWidgetTypeName() const;

            /// @brief Creates a new Widget.
            /// @param RendName The name to be given to the created Widget.
            /// @param Parent The screen the created Widget will belong to.
            /// @return Returns a pointer to the created Widget.
            virtual Widget* CreateWidget(const String& RendName, Screen* Parent);
            /// @brief Creates a new Widget.
            /// @param RendName The name to be given to the created Widget.
            /// @param RendRect The dimensions that will be assigned to the created Widget.
            /// @param Parent The screen the created Widget will belong to.
            /// @return Returns a pointer to the created Widget.
            virtual Widget* CreateWidget(const String& RendName, const UnifiedRect& RendRect, Screen* Parent);

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
        };//GenericWidgetFactory
    }//UI
}//Mezzanine

#endif
