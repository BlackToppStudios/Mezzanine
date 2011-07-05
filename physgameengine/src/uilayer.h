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
#ifndef _uilayer_h
#define _uilayer_h

#include "crossplatformexport.h"
#include "datatypes.h"
#include "vector2.h"
#include "enumerations.h"

#include <map>

namespace Gorilla
{
    class Screen;
    class Layer;
    class Rectangle;
}

namespace phys
{
    class UIManager;
    namespace UI
    {
        class Widget;
        class Rectangle;
        class Caption;
        class Button;
        class TextButton;
        class MarkupText;
        class Scrollbar;
        class CheckBox;
        class ButtonListBox;
        class ListBox;
        class LineList;
        class Window;
        class Menu;
        class Spinner;
        class ScrolledCellGrid;
        class PagedCellGrid;
        class Screen;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class UILayer
        /// @headerfile uilayer.h
        /// @brief This class is the basic container class for UI elements.
        /// @details A layer is a container for widgets and other UI elements that are visable.
        ///////////////////////////////////////
        class PHYS_LIB Layer
        {
            protected:
                Gorilla::Layer* GorillaLayer;
                Screen* Parent;
                UIManager* Manager;
                String Name;
                std::vector<Button*> Buttons;
                std::vector<Rectangle*> Rectangles;
                std::vector<Caption*> Captions;
                std::vector<MarkupText*> MarkupTexts;
                std::vector<LineList*> LineLists;
                std::vector<Widget*> Widgets;
            public:
                /// @brief Internal constructor.
                /// @param name The name of this layer.
                /// @param GLayer The Gorilla Layer this Layer is based on.
                /// @param PScreen The screen that created this layer.
                Layer(const String& name, Gorilla::Layer* GLayer, Screen* PScreen);
                /// @brief Class destructor.
                ~Layer();
                ///////////////////////////////////////////////////////////////////////////////
                // Non-check related Utility Functions
                ///////////////////////////////////////
                /// @brief Gets the name of this layer.
                /// @return Returns a string containing the name of this layer.
                String& GetName();
                /// @brief Gets the parent screen of this layer.
                /// @return Returns a pointer to the screen that created this layer.
                Screen* GetParent();
                /// @brief Sets the layers' visability.
                /// @param Visable A bool representing the visability of the layer.
                void SetVisible(bool Visible);
                /// @brief Gets the layers' visability.
                /// @return Returns a bool representing the visability of the layer.
                bool IsVisible();
                /// @brief Forces the layer to be shown.
                void Show();
                /// @brief Forces the layer to hide.
                void Hide();
                ///////////////////////////////////////////////////////////////////////////////
                // Creating and working with All Basic UI Elements
                ///////////////////////////////////////
                /// @brief Creates a button within this layer.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
                /// @return Returns a pointer to the created button.
                /// @param Name The name of the button.
                /// @param Position The top left position of the button.
                /// @param Size The size of the Button.
                Button* CreateButton(ConstString& Name, const Vector2& Position, const Vector2& Size);
                /// @brief Creates a text button within this layer.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
                /// @return Returns a pointer to the created button.
                /// @param Name The name of the button.
                /// @param Position The top left position of the button.
                /// @param Size The size of the Button.
                /// @param Glyph One of the glyphs specified in your gorilla file.  Must be valid.
                /// @param Text Any text you want printed on the button.
                TextButton* CreateTextButton(ConstString& Name, const Vector2& Position, const Vector2& Size, const Whole& Glyph, ConstString& Text);
                /// @brief Creates a text button within this layer.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
                /// @return Returns a pointer to the created button.
                /// @param Name The name of the button.
                /// @param Position The top left position of the button.
                /// @param Size The size of the Button.
                /// @param LineHeight The lineheight you want the text to have in relative units.  This will automatically select the glyph and scale it for you.
                /// @param Text Any text you want printed on the button.
                TextButton* CreateTextButton(ConstString& Name, const Vector2& Position, const Vector2& Size, const Real& LineHeight, ConstString& Text);
                /// @brief Gets an already created button by name.
                /// @return Returns a pointer to the button of the specified name.
                Button* GetButton(ConstString& Name);
                /// @brief Gets an already created button by index.
                /// @return Returns a pointer to the button at the specified index.
                Button* GetButton(const Whole& Index);
                /// @brief Gets the number of buttons created and stored in this class.
                /// @return Returns the number of buttons this class is storing.
                Whole GetNumButtons();
                /// @brief Destroys a button.
                /// @param ToBeDestroyed Pointer to the button you want destroyed.
                void DestroyButton(Button* ToBeDestroyed);
                /// @brief Creates a rectangle within this layer.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0). @n
                /// Rectangles are innately put behind all captions, so z-order is not necessary.
                /// @param Position The top left position of the button.
                /// @param Size The size of the Button.
                Rectangle* CreateRectangle(const Vector2& Position, const Vector2& Size);
                /// @brief Gets an already created rectangle by index.
                /// @return Returns a pointer to the rectangle at the specified index.
                Rectangle* GetRectangle(const Whole& Index);
                /// @brief Gets the number of rectangles created and stored in this class.
                /// @return Returns the number of rectangles this class is storing.
                Whole GetNumRectangles();
                /// @brief Destroys a rectangle.
                /// @param ToBeDestroyed Pointer to the rectangle you want destroyed.
                void DestroyRectangle(Rectangle* ToBeDestroyed);
                /// @brief Creates a caption within this layer.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
                /// @return Returns a pointer to the created caption.
                /// @param Name The name of this caption.
                /// @param Position The top left position of the caption.
                /// @param Size The size of the Button.
                /// @param Glyph One of the glyphs specified in your gorilla file.  Must be valid.
                /// @param Text Any text you want printed on the caption.
                Caption* CreateCaption(ConstString& Name, const Vector2& Position, const Vector2& Size, const Whole& Glyph, const String& Text);
                /// @brief Creates a caption within this layer.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
                /// @return Returns a pointer to the created caption.
                /// @param Name The name of this caption.
                /// @param Position The top left position of the caption.
                /// @param Size The size of the Button.
                /// @param LineHeight The lineheight you want the text to have in relative units.  This will automatically select the glyph and scale it for you.
                /// @param Text Any text you want printed on the caption.
                Caption* CreateCaption(ConstString& Name, const Vector2& Position, const Vector2& Size, const Real& LineHeight, const String& Text);
                /// @brief Gets an already created caption by name.
                /// @return Returns a pointer to the caption of the specified name.
                Caption* GetCaption(ConstString& Name);
                /// @brief Gets an already created caption by index.
                /// @return Returns a pointer to the caption at the specified index.
                Caption* GetCaption(const Whole& Index);
                /// @brief Gets the number of captions created and stored in this class.
                /// @return Returns the number of captions this class is storing.
                Whole GetNumCaptions();
                /// @brief Destroys a caption.
                /// @param ToBeDestroyed Pointer to the caption you want destroyed.
                void DestroyCaption(Caption* ToBeDestroyed);
                /// @brief Creates a markup text within this layer.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
                /// @return Returns a pointer to the created markup text.
                /// @param Name The name of this markup text.
                /// @param Position The top left position of the markup text.
                /// @param Glyph One of the glyphs specified in your gorilla file.  Must be valid.
                /// @param Text Any text you want printed on the markup text.
                MarkupText* CreateMarkupText(ConstString& Name, const Vector2& Position, const Whole& Glyph, const String& Text);
                /// @brief Creates a markup text within this layer.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
                /// @return Returns a pointer to the created markup text.
                /// @param Name The name of this markup text.
                /// @param Position The top left position of the markup text.
                /// @param LineHeight The lineheight you want the text to have in relative units.  This will automatically select the glyph and scale it for you.
                /// @param Text Any text you want printed on the markup text.
                MarkupText* CreateMarkupText(ConstString& Name, const Vector2& Position, const Real& LineHeight, const String& Text);
                /// @brief Gets an already created markup text by name.
                /// @return Returns a pointer to the markup text of the specified name.
                MarkupText* GetMarkupText(ConstString& Name);
                /// @brief Gets an already created markup text by index.
                /// @return Returns a pointer to the markup text at the specified index.
                MarkupText* GetMarkupText(const Whole& Index);
                /// @brief Gets the number of markup texts created and stored in this class.
                /// @return Returns the number of markup texts this class is storing.
                Whole GetNumMarkupTexts();
                /// @brief Destroys a markup text.
                /// @param ToBeDestroyed Pointer to the markup text you want destroyed.
                void DestroyMarkupText(MarkupText* ToBeDestroyed);
                /// @brief Creates a line list within this layer.
                LineList* CreateLineList();
                /// @brief Gets an already created line list by index.
                /// @return Returns a pointer to the line list at the specified index.
                LineList* GetLineList(const Whole& Index);
                /// @brief Gets the number of line lists created and stored in this class.
                /// @return Returns the number of line lists this class is storing.
                Whole GetNumLineLists();
                /// @brief Destroys a line list.
                /// @param ToBeDestroyed Pointer to the line list you want destroyed.
                void DestroyLineList(LineList* ToBeDestroyed);
                ///////////////////////////////////////////////////////////////////////////////
                // Working with all Widgets
                ///////////////////////////////////////
                /// @brief Adds a widget to this layer.
                /// @details This function is necessary when creating custom widgets in game logic.
                /// @param ToAdd The widget to be added.
                void AddWidget(Widget* ToAdd);
                /// @brief Gets an already created widget by name.
                /// @return Returns a pointer to the widget of the specified name.
                Widget* GetWidget(ConstString& Name);
                /// @brief Gets an already created widget by index.
                /// @return Returns a pointer to the widget at the specified index.
                Widget* GetWidget(Whole Index);
                /// @brief Gets the number of widgets created and stored in this class.
                /// @return Returns the number of widgets this class is storing.
                Whole GetNumWidgets();
                /// @brief Destroys a widget.
                /// @param ToBeDestroyed Pointer to the widget you want destroyed.
                void DestroyWidget(Widget* ToBeDestroyed);
                ///////////////////////////////////////////////////////////////////////////////
                // Creating Widgets
                ///////////////////////////////////////
                /// @brief Creates a Scrollbar within this layer.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
                /// @return Returns a pointer to the created Scrollbar.
                /// @param Name The name of the Scrollbar.
                /// @param Position The top left position of the Scrollbar.
                /// @param Size The size of the Scrollbar.
                /// @param Style The style of scrollbar you want to create, see Scrollbar documentation for more details.
                Scrollbar* CreateScrollbar(ConstString& Name, const Vector2& Position, const Vector2& Size, const UI::ScrollbarStyle& Style);
                /// @brief Creates a CheckBox within this layer.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
                /// @return Returns a pointer to the created CheckBox.
                /// @param Name The name of the CheckBox.
                /// @param Position The top left position of the CheckBox.
                /// @param Size The size of the CheckBox.
                /// @param Glyph The glyphs to use by default for use with the label.  Glyphs are defined in your .gorilla file.
                /// @param LabelText The text to display with the label.  The label uses the Markup Text class, and thus it's
                /// light markup text language.  You can also pass in a blank string if you don't wish to have a label, you can
                /// create a label after construction.
                CheckBox* CreateCheckBox(ConstString& Name, const Vector2& Position, const Vector2& Size, const Whole& Glyph, const String &LabelText);
                /// @brief Creates a Button List Box within this layer.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
                /// @return Returns a pointer to the created Button List Box.
                /// @param Name The name of the Button List Box.
                /// @param Position The top left position of the Button List Box.
                /// @param Size The size of the Button List Box.
                /// @param ScrollbarWidth The relative width of the scrollbar thats created with this widget.
                /// @param ScrollbarStyle The style of scrollbar you want to create, see Scrollbar documentation for more details.
                ButtonListBox* CreateButtonListBox(ConstString& Name, const Vector2& Position, const Vector2& Size, const Real& ScrollbarWidth, const UI::ScrollbarStyle& ScrollbarStyle);
                /// @brief Creates a List Box within this layer.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
                /// @return Returns a pointer to the created List Box.
                /// @param Name The name of the List Box.
                /// @param Position The top left position of the List Box.
                /// @param Size The size of the List Box.
                /// @param ScrollbarWidth The relative width of the scrollbar thats created with this widget.
                /// @param ScrollbarStyle The style of scrollbar you want to create, see Scrollbar documentation for more details.
                ListBox* CreateListBox(ConstString& Name, const Vector2& Position, const Vector2& Size, const Real& ScrollbarWidth, const UI::ScrollbarStyle& ScrollbarStyle);
                /// @brief Creates a Window within this layer.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
                /// @return Returns a pointer to the created Window.
                /// @param Name The name of the Window.
                /// @param Position The top left position of the Window.
                /// @param Size The size of the Window.
                Window* CreateWidgetWindow(ConstString& Name, const Vector2& Position, const Vector2& Size);
                /// @brief Creates a Menu within this layer.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
                /// @return Returns a pointer to the created Menu.
                /// @param Name The name of the Menu.
                /// @param Position The top left position of the Menu.
                /// @param Size The size of the Menu.
                Menu* CreateMenu(ConstString& Name, const Vector2& Position, const Vector2& Size);
                /// @brief Creates a Spinner within this layer.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
                /// @return Returns a pointer to the created Spinner.
                /// @param Name The Name for the Widget.
                /// @param Position The position of the widget.
                /// @param Size The size of the widget.
                /// @param SStyle The layout of buttons this widget will have.  See SpinnerStyle enum or class description for more details.
                /// @param GlyphHeight The desired relative height of the text you want.
                Spinner* CreateSpinner(ConstString& Name, const Vector2& Position, const Vector2& Size, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight);
                /// @brief Creates a scrolled cell grid within this layer.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
                /// @return Returns a pointer to the created ScrolledCellGrid.
                /// @param Name The name of the widget.
                /// @param Position The position of the widget.
                /// @param Size The size of the widget.
                /// @param Thickness The width of the vertical scrollbar in relative units.  The same amount of actual pixels is used
                /// to determine the height of the horizontal scrollbar.
                /// @param Style An enum value representing how you want your scrollbar constructed.  See class details for more info.
                ScrolledCellGrid* CreateScrolledCellGrid(ConstString& Name, const Vector2& Position, const Vector2& Size, const Real& Thickness, const UI::ScrollbarStyle& Style);
                /// @brief Creates a paged cell grid within this layer.
                /// @details This constructor expects relative values for position and size(values from 0.0 to 1.0).
                /// @return Returns a pointer to the created PagedCellGrid.
                /// @param Name The name of the widget.
                /// @param Position The position of the widget.
                /// @param Size The size of the widget.
                /// @param SpnPosition The position of the spinner created alongside this widget.
                /// @param SpnSize The size of the spinner created alongside this widget.
                /// @param SStyle The style of spinner to create.
                /// @param GlyphHeight The desired lineheight of the glyphs to be used with the spinner.
                PagedCellGrid* CreatePagedCellGrid(ConstString& Name, const Vector2& Position, const Vector2& Size, const Vector2& SpnPosition, const Vector2& SpnSize, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight);
                ///////////////////////////////////////////////////////////////////////////////
                // UI Element and Widget Checks
                ///////////////////////////////////////
                /// @brief Gets the button the mouse is over if any.
                /// @details This function searches only the buttons contained in this layer.
                /// @return Returns the button the mouse is over, or NULL if there are none.
                Button* CheckButtonMouseIsOver();
                /// @brief Gets the widget the mouse is over if any.
                /// @details This function searches only the widgets contained in this layer.
                /// @return Returns the widget the mouse is over, or NULL if there are none.
                Widget* CheckWidgetMouseIsOver();
                /// @brief Updates UI elements positions and sizes after a change in the viewport size.
                /// @details This function is called automatically, and shouldn't need to be called manually.
                /// @param OldViewportSize The updated size of the viewport this layer belongs to.
                void ViewportUpdate(const Vector2& OldViewportSize);
                ///////////////////////////////////////////////////////////////////////////////
                // Internal Functions
                ///////////////////////////////////////
                /// @internal
                /// @brief Gets the internal gorilla layer pointer.
                Gorilla::Layer* GetGorillaLayer();
        };//layer
    }//ui
}//phys

#endif
