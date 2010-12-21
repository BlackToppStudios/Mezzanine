//© Copyright 2010 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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

#include "crossplatformexport.h"
#include "datatypes.h"
#include "vector2.h"
#include "uiwidget.h"
#include "enumerations.h"

namespace phys
{
    class UILayer;
    namespace UI
    {
        class Button;
        class Rectangle;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Scrollbar
        /// @headerfile uiscrollbar.h
        /// @brief This class is a widget class, to be used alongside any scrollable widget.
        /// @details If you want to have buttons to accompany your scrollbar they'll automatically
        /// have their height match the width of the scrollbar if it's vertical, or their width
        /// match their height of the scrollbar if it's horizontal, based on the dimensions provided. @n
        /// Scrollbars can come in a few styles.  Separate is the typical way to present them with the Up
        /// or Left button being located at the top of left side of the scroller respectively.  Together
        /// is where both scroll buttons are next to each other instead of on opposite sides of the
        /// scroller.
        ///////////////////////////////////////
        class PHYS_LIB Scrollbar : public Widget
        {
            protected:
                friend class phys::UIManager;
                UI::Button* Scroller;
                UI::Rectangle* ScrollBack;
                UI::Button* UpLeftButton;
                UI::Button* DownRightButton;
                UI::Button* HoveredButton;
                UI::Rectangle* HoveredBack;
                Vector2 ScrollBackOffset;
                Vector2 UpLeftButtonOffset;
                Vector2 DownRightButtonOffset;
                Real ScrollerUpperLimit;
                Real ScrollerLowerLimit;
                Real ScrollerValue;
                Real IncrementDistance;
                UI::ScrollbarStyle ScrollStyle;
                bool Horizontal;
                bool ScrollerLock;
                bool ScrollBackLock;
                bool UpLeftLock;
                bool DownRightLock;
                /// @internal
                /// @brief Constructor helper function for creating a horizontally aligned scrollbar.
                void CreateHorizontalScrollbar(Vector2 Position, Vector2 Size);
                /// @internal
                /// @brief Constructor helper function for creating a vertically aligned scrollbar.
                void CreateVerticalScrollbar(Vector2 Position, Vector2 Size);
                /// @internal
                /// @brief Determines if the dimensions passed into this object are valid and can be used.
                bool IsValidDimensions(Vector2 &Size);
                /// @internal
                /// @brief Sets the horizontal perameter as necessary based on the size provided for the widget.
                void SetHorizontal(Vector2 &Size);
                /// @internal
                /// @brief Calculates the relative offsets for the UI elements that make up this widget.
                void CalculateOffsets(Vector2 &Size);
                /// @internal
                /// @brief Calculates the limits by which the scroller is allowed to move.
                void CalculateScrollLimits();
                /// @internal
                /// @brief Calculates the scrollbar value based on the scrollers current position.
                void CalculateScrollValue();
                /// @internal
                /// @brief Checks if a provided coordinate is within the defined limits, and adjusts it if not.
                void SetToWithinLimits(Real &Coord);
                /// @internal
                /// @brief Internal function for setting the location(position) of this widget.
                void SetLocation(Vector2 &Position);
                /// @internal
                /// @brief Internal function for setting the area(size) of this widget.
                void SetArea(Vector2 &Size);
                /// @internal
                /// @brief Scrolls the scroller either up/left or down/right, based on the parameter.  For use with buttons.
                void ButtonScroll(bool UpLeft);
                /// @internal
                /// @brief Scrolls the scroller either up/left or down/right.  For use with the mouse.
                void MouseScroll(Vector2 Scroll);
                /// @internal
                /// @brief Scrolls the scroller either up/left or down/right.  For use with the scrollback.
                void ScrollBackScroll();
                /// @internal
                /// @brief Performs all the necessary update and automation processes for this widget.
                virtual void Update(bool Force = false);
            public:
                /// @brief Standard initialization constructor.
                /// @param Name The name of this scrollbar.
                /// @param Position A vector2 representing the top left position for this widget to be placed on screen.
                /// @param Size A vector2 representing the size of this widget.
                /// @param Style An enum value representing how you want your scrollbar constructed.  See class details for more info.
                /// @param parent The Layer that created this scrollbar.
                Scrollbar(ConstString& Name, Vector2 Position, Vector2 Size, UI::ScrollbarStyle Style, UILayer* parent);
                /// @brief Standard class destructor.
                virtual ~Scrollbar();
                /// @brief Sets the visibility of this scrollbar.
                /// @param visible Bool determining whether or not this scrollbar should be visible.
                virtual void SetVisible(bool visible);
                /// @brief Gets the visibility of this scrollbar.
                /// @return Returns a bool representing the visibility of this scrollbar.
                virtual bool IsVisible();
                /// @brief Forces this scrollbar to be shown.
                virtual void Show();
                /// @brief Forces this scrollbar to hide.
                virtual void Hide();
                /// @brief Checks to see if the current mouse position is over this widget.
                /// @return Returns a bool value, true if the mouse is over this widget, false if it's not.
                virtual bool CheckMouseHover();
                /// @brief Get the currnent scroll position represented by a value between 0 and 1.
                /// @details For example, if the scroller is halfway down the limits it's allowed, this will return 0.5. @n
                /// Like other values, the top and left represent origin(0) values.
                /// @return Returns the stored scroll position.
                virtual Real GetScrollerValue();
                /// @brief Sets the relative distance the scrollbar will move when the up/left or down/right buttons are pressed.
                /// @param IncDist A real representing the amount to increment.  Can be negative.
                virtual void SetIncrementDistance(Real IncDist);
                /// @brief Sets the length(or height) of the scroller based on the relative size of it's background.
                /// @param Size The relative size you with to set the scroller to.  Values range from 0.0 to 1.0.
                virtual void SetScrollerSize(Real Size);
                /// @brief Sets the relative position of this widget in pixels.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0. @n
                /// The top and the left are considered the origin, thus values of 0 represent one of these points.
                /// @param Position A vector2 representing the relative position of this widget.
                virtual void SetPosition(Vector2 Position);
                /// @brief Gets the relative position of this widget.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative position of this widget.
                virtual Vector2 GetPosition();
                /// @brief Sets the pixel position of this widget.
                /// @param Position A vector2 representing the pixel position of this widget.
                virtual void SetActualPosition(Vector2 Position);
                /// @brief Sets the pixel position of this widget.
                /// @return Returns a vector2 representing the pixel position of this widget.
                virtual Vector2 GetActualPosition();
                /// @brief Sets the relative size of this widget.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Size A vector2 representing the relative size of this widget.
                virtual void SetSize(Vector2 Size);
                /// @brief Gets the relative size of this widget.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative size of this widget.
                virtual Vector2 GetSize();
                /// @brief Sets the pixel size of this widget.
                /// @param Size A vector2 representing the pixel size of this widget.
                virtual void SetActualSize(Vector2 Size);
                /// @brief Sets the pixel size of this widget.
                /// @return Returns a vector2 representing the pixel size of this widget.
                virtual Vector2 GetActualSize();
                /// @brief Gets the Scroller button within this widget.
                /// @return Returns a pointer to the Scroller button within this widget.
                virtual Button* GetScroller();
                /// @brief Gets the UpLeft button within this widget, if it was initialized.
                /// @return Returns a pointer to the UpLeft button within this widget, or NULL if none.
                virtual Button* GetUpLeftButton();
                /// @brief Gets the DownRight button within this widget, if it was initialized.
                /// @return Returns a pointer to the DownRight button within this widget, or NULL if none.
                virtual Button* GetDownRightButton();
                /// @brief Gets the Scroller background within this widget.
                /// @return Returns a pointer to the Scroller background within this widget.
                virtual Rectangle* GetScrollBack();
        };//scrollbar
    }//UI
}//phys

#endif

