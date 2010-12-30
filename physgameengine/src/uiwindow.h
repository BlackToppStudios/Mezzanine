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
#ifndef _uiwindow_h
#define _uiwindow_h

#include "uiwidget.h"
#include "uiresizinginfo.h"

namespace phys
{
    namespace UI
    {
        //forward declares
        class Button;
        class TextButton;
        class Rectangle;
        class Caption;
        class MarkupText;
        class LineList;
        class Scrollbar;
        class CheckBox;
        class ButtonListBox;
        class ListBox;
        //typedefs
        typedef ResizingInfo<Button*> OffsetButtonInfo;
        typedef ResizingInfo<Rectangle*> OffsetRectangleInfo;
        typedef ResizingInfo<Caption*> OffsetCaptionInfo;
        typedef ResizingInfo<MarkupText*> OffsetMarkupTextInfo;
        typedef ResizingInfo<Widget*> OffsetWidgetInfo;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Window
        /// @headerfile uiwindow.h
        /// @brief This is a container widget capable of holding any other widget.
        /// @details
        ///////////////////////////////////////
        class PHYS_LIB Window : public Widget
        {
            protected:
                Rectangle* WindowBack;
                //Caption* WindowTitle;
                std::vector<OffsetButtonInfo> Buttons;
                std::vector<OffsetRectangleInfo> Rectangles;
                std::vector<OffsetCaptionInfo> Captions;
                std::vector<OffsetMarkupTextInfo> MarkupTexts;
                std::vector<OffsetWidgetInfo> Widgets;
                /// @brief For use with widget update/automation.
                void Update(bool Force = false);
                /// @brief Internal function for setting the location(position) of this widget.
                void SetLocation(Vector2 &Position);
                /// @brief Internal function for setting the area(size) of this widget.
                void SetArea(Vector2 &Size);
                /// @brief Calculates a new offset for an individual element.
                Vector2 CalculateOffset(Vector2 NewSize, Vector2 OldSize, Vector2 EleOffset, UI::ResizeableAnchor Anchor);
                /// @brief Calculates a new size for an individual element.
                Vector2 CalculateSize(Vector2 NewSize, Vector2 OldSize, Vector2 EleSize, UI::ResizeableTether Tether);
            public:
                /// @brief Standard initialization constructor.
                /// @param name The name of the window.
                /// @param Position The position of the window.
                /// @param Size The size of the window.
                /// @param Layer The parent layer this window belongs to.
                Window(ConstString& name, Vector2 Position, Vector2 Size, UILayer* Layer);
                /// @brief Standard destructor.
                virtual ~Window();
                /// @brief Sets the visibility of this window.
                /// @param visible Bool determining whether or not this window should be visible.
                virtual void SetVisible(bool visible);
                /// @brief Gets the visibility of this window.
                /// @return Returns a bool representing the visibility of this window.
                virtual bool IsVisible();
                /// @brief Forces this window to be shown.
                virtual void Show();
                /// @brief Forces this window to hide.
                virtual void Hide();
                /// @brief Checks to see if the current mouse position is over this window.
                /// @return Returns a bool value, true if the mouse is over this window, false if it's not.
                virtual bool CheckMouseHover();
                /// @brief Sets the relative position of this window.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Position A vector2 representing the relative position of this window.
                virtual void SetPosition(Vector2 Position);
                /// @brief Gets the relative position of this window.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative position of this window.
                virtual Vector2 GetPosition();
                /// @brief Sets the pixel position of this window.
                /// @param Position A vector2 representing the pixel position of this window.
                virtual void SetActualPosition(Vector2 Position);
                /// @brief Sets the pixel position of this window.
                /// @return Returns a vector2 representing the pixel position of this window.
                virtual Vector2 GetActualPosition();
                /// @brief Sets the relative size of this window.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Size A vector2 representing the relative size of this window.
                virtual void SetSize(Vector2 Size);
                /// @brief Gets the relative size of this window.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative size of this window.
                virtual Vector2 GetSize();
                /// @brief Sets the pixel size of this window.
                /// @param Size A vector2 representing the pixel size of this window.
                virtual void SetActualSize(Vector2 Size);
                /// @brief Sets the pixel size of this window.
                /// @return Returns a vector2 representing the pixel size of this window.
                virtual Vector2 GetActualSize();
                ///////////////////////////////////////////////////////////////////////////////
                // Creating and working with All Basic UI Elements
                ///////////////////////////////////////
                /// @brief Creates a button within this Window.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
                /// @return Returns a pointer to the created button.
                /// @param Name The name of the button.
                /// @param Position The top left position of the button.
                /// @param Size The size of the Button.
                Button* CreateButton(ConstString& Name, Vector2 Position, Vector2 Size);
                /// @brief Creates a text button within this Window.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
                /// @return Returns a pointer to the created button.
                /// @param Name The name of the button.
                /// @param Position The top left position of the button.
                /// @param Size The size of the Button.
                /// @param Glyph One of the glyphs specified in your gorilla file.  Must be valid.
                /// @param Text Any text you want printed on the button.
                TextButton* CreateTextButton(ConstString& Name, Vector2 Position, Vector2 Size, Whole Glyph, ConstString Text);
                /// @brief Gets an already created button by name.
                /// @return Returns a pointer to the button of the specified name.
                Button* GetButton(ConstString& Name);
                /// @brief Gets an already created button by index.
                /// @return Returns a pointer to the button at the specified index.
                Button* GetButton(Whole Index);
                /// @brief Gets the OffsetButtonInfo of an already created button by name.
                /// @return Returns a reference to the button of the specified name.
                OffsetButtonInfo* GetOffsetButtonInfo(ConstString& Name);
                /// @brief Gets the OffsetButtonInfo of an already created button by index.
                /// @return Returns a reference to the button at the specified index.
                OffsetButtonInfo* GetOffsetButtonInfo(Whole Index);
                /// @brief Gets the number of buttons created and stored in this class.
                /// @return Returns the number of buttons this class is storing.
                Whole GetNumButtons();
                /// @brief Destroys a button.
                /// @param ToBeDestroyed Pointer to the button you want destroyed.
                void DestroyButton(UI::Button* ToBeDestroyed);
                /// @brief Creates a rectangle within this Window.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0). @n
                /// Rectangles are innately put behind all captions, so z-order is not necessary.
                /// @param Position The top left position of the button.
                /// @param Size The size of the Button.
                Rectangle* CreateRectangle(Vector2 Position, Vector2 Size);
                /// @brief Gets an already created rectangle by index.
                /// @return Returns a pointer to the rectangle at the specified index.
                Rectangle* GetRectangle(Whole Index);
                /// @brief Gets the OffsetRectangleInfo of an already created rectangle by index.
                /// @return Returns a reference to the rectangle at the specified index.
                OffsetRectangleInfo* GetOffsetRectangleInfo(Whole Index);
                /// @brief Gets the number of rectangles created and stored in this class.
                /// @return Returns the number of rectangles this class is storing.
                Whole GetNumRectangles();
                /// @brief Destroys a rectangle.
                /// @param ToBeDestroyed Pointer to the rectangle you want destroyed.
                void DestroyRectangle(UI::Rectangle* ToBeDestroyed);
                /// @brief Creates a caption within this Window.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
                /// @return Returns a pointer to the created caption.
                /// @param Name The name of this caption.
                /// @param Position The top left position of the caption.
                /// @param Size The size of the Button.
                /// @param Glyph One of the glyphs specified in your gorilla file.  Must be valid.
                /// @param Text Any text you want printed on the caption.
                Caption* CreateCaption(ConstString& Name, Vector2 Position, Vector2 Size, Whole Glyph, String Text);
                /// @brief Gets an already created caption by name.
                /// @return Returns a pointer to the caption of the specified name.
                Caption* GetCaption(ConstString& Name);
                /// @brief Gets an already created caption by index.
                /// @return Returns a pointer to the caption at the specified index.
                Caption* GetCaption(Whole Index);
                /// @brief Gets the OffsetCaptionInfo of an already created caption by name.
                /// @return Returns a reference to the caption of the specified name.
                OffsetCaptionInfo* GetOffsetCaptionInfo(ConstString& Name);
                /// @brief Gets the OffsetCaptionInfo of an already created caption by index.
                /// @return Returns a reference to the caption at the specified index.
                OffsetCaptionInfo* GetOffsetCaptionInfo(Whole Index);
                /// @brief Gets the number of captions created and stored in this class.
                /// @return Returns the number of captions this class is storing.
                Whole GetNumCaptions();
                /// @brief Destroys a caption.
                /// @param ToBeDestroyed Pointer to the caption you want destroyed.
                void DestroyCaption(UI::Caption* ToBeDestroyed);
                /// @brief Creates a markup text within this Window.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
                /// @return Returns a pointer to the created markup text.
                /// @param Name The name of this markup text.
                /// @param Position The top left position of the markup text.
                /// @param Glyph One of the glyphs specified in your gorilla file.  Must be valid.
                /// @param Text Any text you want printed on the markup text.
                MarkupText* CreateMarkupText(ConstString& Name, Vector2 Position, Whole Glyph, String Text);
                /// @brief Gets an already created markup text by name.
                /// @return Returns a pointer to the markup text of the specified name.
                MarkupText* GetMarkupText(ConstString& Name);
                /// @brief Gets an already created markup text by index.
                /// @return Returns a pointer to the markup text at the specified index.
                MarkupText* GetMarkupText(Whole Index);
                /// @brief Gets the OffsetMarkupTextInfo of an already created markup text by name.
                /// @return Returns a reference to the markup text of the specified name.
                OffsetMarkupTextInfo* GetOffsetMarkupTextInfo(ConstString& Name);
                /// @brief Gets the OffsetMarkupTextInfo of an already created markup text by index.
                /// @return Returns a reference to the markup text at the specified index.
                OffsetMarkupTextInfo* GetOffsetMarkupTextInfo(Whole Index);
                /// @brief Gets the number of markup texts created and stored in this class.
                /// @return Returns the number of markup texts this class is storing.
                Whole GetNumMarkupTexts();
                /// @brief Destroys a markup text.
                /// @param ToBeDestroyed Pointer to the markup text you want destroyed.
                void DestroyMarkupText(UI::MarkupText* ToBeDestroyed);
                ///////////////////////////////////////////////////////////////////////////////
                // Working with all Widgets
                ///////////////////////////////////////
                /// @brief Gets an already created widget by name.
                /// @return Returns a pointer to the widget of the specified name.
                Widget* GetWidget(ConstString& Name);
                /// @brief Gets an already created widget by index.
                /// @return Returns a pointer to the widget at the specified index.
                Widget* GetWidget(Whole Index);
                /// @brief Gets the OffsetWidgetInfo of an already created widget by name.
                /// @return Returns a reference to the widget of the specified name.
                OffsetWidgetInfo* GetOffsetWidgetInfo(ConstString& Name);
                /// @brief Gets the OffsetWidgetInfo of an already created widget by index.
                /// @return Returns a reference to the widget at the specified index.
                OffsetWidgetInfo* GetOffsetWidgetInfo(Whole Index);
                /// @brief Gets the number of widgets created and stored in this class.
                /// @return Returns the number of widgets this class is storing.
                Whole GetNumWidgets();
                /// @brief Destroys a widget.
                /// @param ToBeDestroyed Pointer to the widget you want destroyed.
                void DestroyWidget(UI::Widget* ToBeDestroyed);
                ///////////////////////////////////////////////////////////////////////////////
                // Creating Widgets
                ///////////////////////////////////////
                /// @brief Creates a Scrollbar within this Window.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
                /// @return Returns a pointer to the created Scrollbar.
                /// @param Name The name of the Scrollbar.
                /// @param Position The top left position of the Scrollbar.
                /// @param Size The size of the Scrollbar.
                /// @param Style The style of scrollbar you want to create, see Scrollbar documentation for more details.
                UI::Scrollbar* CreateScrollbar(ConstString& Name, Vector2 Position, Vector2 Size, UI::ScrollbarStyle Style);
                /// @brief Creates a CheckBox within this Window.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
                /// @return Returns a pointer to the created CheckBox.
                /// @param Name The name of the CheckBox.
                /// @param Position The top left position of the CheckBox.
                /// @param Size The size of the CheckBox.
                /// @param Glyph The glyphs to use by default for use with the label.  Glyphs are defined in your .gorilla file.
                /// @param LabelText The text to display with the label.  The label uses the Markup Text class, and thus it's
                /// light markup text language.  You can also pass in a blank string if you don't wish to have a label, you can
                /// create a label after construction.
                UI::CheckBox* CreateCheckBox(ConstString& Name, Vector2 Position, Vector2 Size, Whole Glyph, String &LabelText);
                /// @brief Creates a Button List Box within this Window.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
                /// @return Returns a pointer to the created Button List Box.
                /// @param Name The name of the Button List Box.
                /// @param Position The top left position of the Button List Box.
                /// @param Size The size of the Button List Box.
                /// @param ScrollbarWidth The relative width of the scrollbar thats created with this widget.
                /// @param ScrollbarStyle The style of scrollbar you want to create, see Scrollbar documentation for more details.
                UI::ButtonListBox* CreateButtonListBox(ConstString& Name, Vector2 Position, Vector2 Size, Real ScrollbarWidth, UI::ScrollbarStyle ScrollbarStyle);
                /// @brief Creates a List Box within this Window.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
                /// @return Returns a pointer to the created List Box.
                /// @param Name The name of the List Box.
                /// @param Position The top left position of the List Box.
                /// @param Size The size of the List Box.
                /// @param ScrollbarWidth The relative width of the scrollbar thats created with this widget.
                /// @param ScrollbarStyle The style of scrollbar you want to create, see Scrollbar documentation for more details.
                UI::ListBox* CreateListBox(ConstString& Name, Vector2 Position, Vector2 Size, Real ScrollbarWidth, UI::ScrollbarStyle ScrollbarStyle);
        };//window
    }//UI
}//phys

#endif
