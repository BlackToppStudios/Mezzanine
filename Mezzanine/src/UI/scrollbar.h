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
#ifndef _uiscrollbar_h
#define _uiscrollbar_h

#include "uienumerations.h"
#include "UI/pageprovider.h"
#include "UI/widgetfactory.h"

namespace Mezzanine
{
    namespace UI
    {
        class Button;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the EventArguments class for when the scrollvalue of a scrollbar is updated.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB ScrollbarValueChangedArguments : public WidgetEventArguments
        {
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Public Data Members

            /// @brief The pre-update value of the calling scrollbar.
            const Real OldScrollerValue;
            /// @brief The post-update value of the calling scrollbar.
            const Real NewScrollerValue;

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            /// @brief Class constructor.
            /// @param Name The name of the event being fired.
            /// @param Source The identification of the widget firing this event.
            /// @param OldValue The pre-update value of the calling scrollbar.
            /// @param NewValue The post-update value of the calling scrollbar.
            ScrollbarValueChangedArguments(const String& Name, const String& Source, const Real& OldValue, const Real& NewValue) :
                WidgetEventArguments(Name,Source), OldScrollerValue(OldValue), NewScrollerValue(NewValue) {  }
            /// @brief Class destructor.
            virtual ~ScrollbarValueChangedArguments() {  }

            ///////////////////////////////////////////////////////////////////////////////
            // CountedPtr Functionality

            /// @copydoc EventArguments::GetMostDerived()
            virtual ScrollbarValueChangedArguments* GetMostDerived()
                { return this; }
        };//ScrollbarValueChangedArguments
    }//UI

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a metaprogramming traits class used by ScrollbarValueChangedArguments.
        /// @details This is need for an intrusive CountedPtr implementation.  Should a working external reference count be made this
        /// could be dropped in favor of a leaner implementation.
        ///////////////////////////////////////
        template <>
        class ReferenceCountTraits<UI::ScrollbarValueChangedArguments>
        {
        public:
            /// @brief Typedef communicating the reference count type to be used.
            typedef UI::ScrollbarValueChangedArguments RefCountType;

            /// @brief Method responsible for creating a reference count for a CountedPtr of the templated type.
            /// @param Target A pointer to the target class that is to be reference counted.
            /// @return Returns a pointer to a new reference counter for the templated type.
            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            /// @brief Enum used to decide the type of casting to be used by a reference counter of the templated type.
            enum { IsCastable = CastStatic };
        };//ReferenceCountTraits<ScrollbarValueChangedArguments>

    namespace UI
    {
        /// @brief Convenience typedef for passing around ScrollbarValueChangedArguments.
        typedef CountedPtr<ScrollbarValueChangedArguments> ScrollbarValueChangedArgumentsPtr;

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the scrollbar base class.
        /// @details If you want to have buttons to accompany your scrollbar they'll automatically
        /// have their height match the width of the scrollbar if it's vertical, or their width
        /// match their height of the scrollbar if it's horizontal, based on the dimensions provided. @n @n
        /// Scrollbars can come in a few styles.  Separate is the typical way to present them with the Up
        /// or Left button being located at the top of left side of the scroller respectively.  Together
        /// is where both scroll buttons are next to each other instead of on opposite sides of the
        /// scroller.
        ///////////////////////////////////////
        class MEZZ_LIB Scrollbar : public PageProvider
        {
        public:
            /// @brief String containing the type name for this class: "Scrollbar".
            static const String TypeName;
            /// @brief Event name for when this scrollbar has his scrollvalue updated.
            static const String EventScrollValueChanged;
        protected:
            /// @internal
            /// @brief The manipulatable widget that represents the current position on the scrollbar/slider.
            Button* Scroller;
            /// @internal
            /// @brief The background around that can be clicked on and represents the valid bounds for the scroller.
            Button* ScrollBack;
            /// @internal
            /// @brief The Up/Left button that when clicked will move the scroller in that respective direction.
            /// @remarks Depending on the button configuration this can be NULL for the lifetime of the widget.
            Button* UpLeftButton;
            /// @internal
            /// @brief The Down/Right button that when clicked will move the scroller in that respective direction.
            /// @remarks Depending on the button configuration this can be NULL for the lifetime of the widget.
            Button* DownRightButton;
            /// @internal
            /// @brief This is a pointer to the specific child that is locked and being manipulated.
            Button* ChildLock;
            /// @internal
            /// @brief The distance the scroller is to be moved when the up, left, down, or right buttons are clicked.
            Real IncrementDistance;
            /// @internal
            /// @brief The size of the scroller on the appropriate axis relative to the ScrollBack.
            Real ScrollerSize;
            /// @internal
            /// @brief Stores whether or not this scrollbar should hide when there is 1 page or less of list items.  Only used when this is set as a PageProvider.
            Boole AutoHideScroll;

            /// @copydoc Widget::HandleInputImpl(const Input::MetaCode&)
            virtual Boole HandleInputImpl(const Input::MetaCode& Code);
            /// @internal
            /// @brief Subscribes to all the events of this scrollbars children we care about.  Used only on construction.
            virtual void SubscribeToChildEvents();
            /// @internal
            /// @brief Gets the range on which the scroller can be placed.
            /// @return Returns the UpperScrollLimit minus the LowerScrollLimit.
            virtual Real GetScrollRange() const;
            /// @internal
            /// @brief Gets the pixel position of the upper limit the scroller can be placed on.
            /// @return Returns a Real representing the scrollers upper limit.
            virtual Real GetUpperScrollLimit() const = 0;
            /// @internal
            /// @brief Gets the pixel position of the lower limit the scroller can be placed on.
            /// @return Returns a Real representing the scrollers limit limit.
            virtual Real GetLowerScrollLimit() const = 0;
        public:
            /// @brief Blank constructor.
            /// @param Parent The parent screen that created this scrollbar.
            Scrollbar(Screen* Parent);
            /// @brief Standard initialization constructor.
            /// @param RendName The name to be given to this scrollbar.
            /// @param Parent The parent screen that created this scrollbar.
            Scrollbar(const String& RendName, Screen* Parent);
            /// @brief Rect constructor.
            /// @param RendName The name to be given to this scrollbar.
            /// @param RendRect The rect describing this widget's transform relative to it's parent.
            /// @param Parent The parent screen that created this scrollbar.
            Scrollbar(const String& RendName, const UnifiedRect& RendRect, Screen* Parent);
            /// @brief Standard class destructor.
            virtual ~Scrollbar();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @brief Sets the value of this scrollbar and warps the scroller to that respective position.
            /// @param Value The value to be set for this scrollbar.  Range: 0.0 to 1.0.
            virtual void SetScrollerValue(const Real& Value) = 0;
            /// @brief Get the currnent scroll position represented by a value between 0 and 1.
            /// @details For example, if the scroller is halfway down the limits it's allowed, this will return 0.5. @n
            /// Like other values, the top and left represent origin(0) values.
            /// @return Returns the stored scroll position.
            virtual Real GetScrollerValue() const = 0;

            /// @brief Sets the length(or height) of the scroller based on the relative size of it's background.
            /// @param Size The relative size you with to set the scroller to.  Range: 0.0 to 1.0
            virtual void SetScrollerSize(const Real& Size) = 0;
            /// @brief Gets the size of the scroller relative to the ScrollBack.
            /// @return Returns a Real representing size of the scroller on the relevant axis relative to the ScrollBack.
            virtual Real GetScrollerSize() const = 0;

            /// @brief Sets the relative distance the scroller will move when the up/left or down/right buttons are pressed.
            /// @remarks Default: 0.1.
            /// @param IncDist A real representing the amount to increment.  Can be negative.
            virtual void SetIncrementDistance(const Real& IncDist);
            /// @brief Gets the relative distance the scroller will move on a button press.
            /// @return Returns a Real representing the relative distance to be moved on a button press.
            virtual Real GetIncrementDistance() const;

            /// @brief Sets whether or not this scrollbar will hide when it's set page container has less than one page to display.
            /// @param AutoHide True to make this hide when this scrollbars page container doesn't have more than one page to display, false to make this always visible.
            virtual void SetAutoHide(Boole AutoHide);
            /// @brief Gets whether or not this scrollbar will hide when it's set page container has less than one page to display.
            /// @return Returns true if this scrollbar is hiding when it's page container has less then one page to display, false if it's always visible.
            virtual Boole GetAutoHide() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Fetch Methods

            /// @brief Gets the Scroller button within this widget.
            /// @return Returns a pointer to the Scroller button within this widget.
            virtual Button* GetScroller() const;
            /// @brief Gets the UpLeft button within this widget, if it was initialized.
            /// @return Returns a pointer to the UpLeft button within this widget, or NULL if none.
            virtual Button* GetUpLeftButton() const;
            /// @brief Gets the DownRight button within this widget, if it was initialized.
            /// @return Returns a pointer to the DownRight button within this widget, or NULL if none.
            virtual Button* GetDownRightButton() const;
            /// @brief Gets the Scroller background within this widget.
            /// @return Returns a pointer to the Scroller background within this widget.
            virtual Button* GetScrollBack() const;

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

            /// @brief Self logic to be executed when this scrollbar changes it's scrollvalue.
            /// @param OldValue The previous value of this scrollbar.
            /// @param NewValue The updated value of this scrollbar.
            virtual void _OnScrollValueChanged(const Real OldValue, const Real NewValue);

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @copydoc EventSubscriber::_NotifyEvent(EventArgumentsPtr Args)
            virtual void _NotifyEvent(EventArgumentsPtr Args);
            /// @internal
            /// @brief Performs the operations for when the scroller is directly manipulated by the mouse.
            /// @param MouseDelta The amount in pixels the mouse has moved since the last frame.
            /// @return Returns true if the scroller was successfully moved, false otherwise.
            virtual Boole _MouseScroll(const Vector2& MouseDelta) = 0;
            /// @internal
            /// @brief Performs the operations for when the scroller is manipulated by the mouse wheel.
            /// @param Direction The direction of the mouse wheel spin.  Usually this will be a casted Input::DirectionalMotionState value.
            /// @return Returns true if the scroller was successfully moved, false otherwise.
            virtual Boole _MouseWheelScroll(const Integer Direction) = 0;
            /// @internal
            /// @brief Performs the operations for when the scrollback is clicked on to manipulate the scroller.
            /// @param HitPosition The location on the scroll back where the mouse was clicked (in screen coordinates).
            /// @return Returns true if the scroller was successfully moved, false otherwise.
            virtual Boole _ScrollBackScroll(const Vector2& HitPosition) = 0;
            /// @internal
            /// @brief Performs the operations for when one of the buttons is pressed to manipulate the scroller.
            /// @param TheButton The button that was activated (or to simulate an activation for).
            /// @return Returns true if the scroller was successfully moved, false otherwise.
            virtual Boole _ButtonScroll(Button* TheButton) = 0;
        };//Scrollbar
    }//UI
}//Mezzanine

#endif

