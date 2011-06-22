//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _uispinner_h
#define _uispinner_h

#include "uiwidget.h"
#include "enumerations.h"

namespace phys
{
    namespace UI
    {
        class Button;
        class Caption;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Spinner
        /// @headerfile uispinner.h
        /// @brief This is a simple widget for a numeric variable in a box.
        /// @details Like the Scrollbar Widget, the size provided will be for the area of all the
        /// elements of this widget combined.  In the case of separate format, if horizontal, the
        /// width of the increment and decrement buttons will be the same as the height or if vertical
        /// the height will be the same as the width provided.  In either case making the two buttons
        /// perfectly square in size, with the remaining space going to the display caption.  @n
        /// In the case of the together format, then half of the height is taken and that is used for
        /// both the width and height, and then they are placed on the appropriate side.
        ///////////////////////////////////////
        class PHYS_LIB Spinner : public Widget
        {
            protected:
                Button* Increment;
                Button* Decrement;
                Caption* ValueDisplay;
                bool DecimalDisplay;
                bool Editable;
                bool IncrementLock;
                bool DecrementLock;
                int Value;
                int MaxValue;
                int MinValue;
                Whole DecimalPlaces;
                String EditCache;
                UI::SpinnerStyle SpinLayout;
                Vector2 IncrementOffset;
                Vector2 DecrementOffset;
                Vector2 ValueDisplayOffset;
                /// @brief Constructor helper function for creating a horizontally aligned spinner.
                virtual void CreateHorizontalSpinner(const Vector2& Position, const Vector2& Size, const Real& GlyphHeight);
                /// @brief Constructor helper function for creating a vertically aligned spinner.
                virtual void CreateVerticalSpinner(const Vector2& Position, const Vector2& Size, const Real& GlyphHeight);
                /// @brief Constructor helper function for creating a box shaped spinner with buttons together.
                virtual void CreateBoxSpinner(const Vector2& Position, const Vector2& Size, const Real& GlyphHeight);
                /// @brief Calculates the relative offsets for the UI elements that make up this widget.
                virtual void CalculateOffsets(const Vector2& Size);
                /// @brief Internal function for setting the location(position) of this widget.
                virtual void SetLocation(const Vector2& Position);
                /// @brief Internal function for setting the area(size) of this widget.
                virtual void SetArea(const Vector2& Size);
                /// @brief Checks to ensure the value is within it's limits.
                virtual void CheckValueLimits();
                /// @brief Gets the current value of this spinner as text, for use with updating the caption.
                virtual String GetValueAsText();
                /// @brief For use with widget update/automation.
                virtual void Update(bool Force = false);
                /// @brief Processes the captured inputs.
                virtual void ProcessCapturedInputs();
            public:
                /// @brief Class constructor.
                /// @param name The Name for the Widget.
                /// @param Position The position of the widget.
                /// @param Size The size of the widget.
                /// @param SStyle The layout of buttons this widget will have.  See SpinnerStyle enum or class description for more details.
                /// @param GlyphHeight The desired relative height of the text you want.
                /// @param parent The parent layer that created this widget.
                Spinner(const String& name, const Vector2& Position, const Vector2& Size, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight, Layer* parent);
                /// @brief Class destructor.
                ~Spinner();
                /// @brief Sets the visibility of this widget.
                /// @param visible Bool determining whether or not this widget should be visible.
                virtual void SetVisible(bool visible);
                /// @brief Gets the visibility of this widget.
                /// @return Returns a bool representing the visibility of this widget.
                virtual bool IsVisible();
                /// @brief Forces this widget to be shown.
                virtual void Show();
                /// @brief Forces this widget to hide.
                virtual void Hide();
                /// @brief Sets the current value of this widget.
                /// @param ValueToSet The value to be set.
                virtual void SetSpinnerValue(const int& ValueToSet);
                /// @brief Gets the current value stored in this spinner.
                /// @return Returns an int, which is the current value within this spinner.
                virtual int GetSpinnerValue();
                /// @brief Sets limits on what the value contained in this widget can be.
                /// @details Setting a limit of zero for both means no limits will be enforced.
                /// @param Lower The Minimum allowed limit.
                /// @param Upper The Maximum allowed limit.
                virtual void SetValueLimits(const int& Lower, const int& Upper);
                /// @brief Sets whether or not this widget should be allowed to be edited directly.
                /// @param Edit Whether or not this widget should be editable.
                virtual void SetEditable(bool Edit);
                /// @brief Gets whether or not this widget is currently editable.
                /// @return Return Returns a bool indicating whether or not this widget is editable.
                virtual bool GetEditable();
                /// @brief Enables the display of Reals for the value rather then Wholes.
                /// @details Internally it is still storing wholes, just they are converted before being displayed.
                /// @param Enable Whether or not to enable this feature.
                /// @param DecimalPlaces The number of places passed the decimal you wish to display.
                virtual void EnableDecimalDisplay(bool Enable, const Whole& Places = 1);
                /// @brief Checks to see if the current mouse position is over this widget.
                /// @return Returns a bool value, true if the mouse is over this widget, false if it's not.
                virtual bool CheckMouseHover();
                /// @brief Sets the relative position of this widget.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Position A vector2 representing the relative position of this widget.
                virtual void SetPosition(const Vector2& Position);
                /// @brief Gets the relative position of this widget.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative position of this widget.
                virtual Vector2 GetPosition();
                /// @brief Sets the pixel position of this widget.
                /// @param Position A vector2 representing the pixel position of this widget.
                virtual void SetActualPosition(const Vector2& Position);
                /// @brief Sets the pixel position of this widget.
                /// @return Returns a vector2 representing the pixel position of this widget.
                virtual Vector2 GetActualPosition();
                /// @brief Sets the relative size of this widget.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Size A vector2 representing the relative size of this widget.
                virtual void SetSize(const Vector2& Size);
                /// @brief Gets the relative size of this widget.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative size of this widget.
                virtual Vector2 GetSize();
                /// @brief Sets the pixel size of this widget.
                /// @param Size A vector2 representing the pixel size of this widget.
                virtual void SetActualSize(const Vector2& Size);
                /// @brief Sets the pixel size of this widget.
                /// @return Returns a vector2 representing the pixel size of this widget.
                virtual Vector2 GetActualSize();
                /// @brief Gets the increment button of this widget.
                /// @return Returns a pointer to the increment button.
                virtual Button* GetIncrement();
                /// @brief Gets the decrement button of this widget.
                /// @return Returns a pointer to the decrement button.
                virtual Button* GetDecrement();
                /// @brief Gets the value display caption of this widget.
                /// @return Returns a pointer to the value display caption.
                virtual Caption* GetValueDisplay();
        };//spinner
    }//ui
}//phys

#endif
