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
#include "uirenderablerect.h"
#include "uienumerations.h"

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
        class ListBox;
        class LineList;
        class Window;
        class Menu;
        class Spinner;
        class ScrolledCellGrid;
        class PagedCellGrid;
        class DropDownList;
        class TabSet;
        class Screen;
        ///////////////////////////////////////////////////////////////////////////////
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
                virtual ~Layer();
                ///////////////////////////////////////////////////////////////////////////////
                // Non-check related Utility Functions
                ///////////////////////////////////////
                /// @brief Gets the name of this layer.
                /// @return Returns a string containing the name of this layer.
                virtual String& GetName();
                /// @brief Gets the parent screen of this layer.
                /// @return Returns a pointer to the screen that created this layer.
                virtual Screen* GetParent();
                /// @brief Sets the layers' visability.
                /// @param Visable A bool representing the visability of the layer.
                virtual void SetVisible(bool Visible);
                /// @brief Gets the visibility setting of this layer.
                /// @return Returns a bool that is the current visibility setting of this layer.
                virtual bool GetVisible() const;
                /// @brief Gets the layers' visability.
                /// @details This function will check the visibility of all parent objects to see if it is being
                /// drawn.  This will not tell you whether or not this layer has it's own visibility setting
                /// enabled.  For that see: GetVisible().
                /// @return Returns a bool representing the visability of the layer.
                virtual bool IsVisible() const;
                /// @brief Forces the layer to be shown.
                virtual void Show();
                /// @brief Forces the layer to hide.
                virtual void Hide();
                ///////////////////////////////////////////////////////////////////////////////
                // Creating and working with All Basic UI Elements
                ///////////////////////////////////////
                /// @brief Creates a button within this layer.
                /// @return Returns a pointer to the created button.
                /// @param Name The name of the button.
                /// @param Rect The Rect representing the position and size of the button.
                virtual Button* CreateButton(ConstString& Name, const RenderableRect& Rect);
                /// @brief Creates a text button within this layer.
                /// @return Returns a pointer to the created button.
                /// @param Name The name of the button.
                /// @param Rect The Rect representing the position and size of the button.
                /// @param Glyph One of the glyphs specified in your gorilla file.  Must be valid.
                /// @param Text Any text you want printed on the button.
                virtual TextButton* CreateTextButton(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, ConstString& Text);
                /// @brief Creates a text button within this layer.
                /// @return Returns a pointer to the created button.
                /// @param Name The name of the button.
                /// @param Rect The Rect representing the position and size of the button.
                /// @param LineHeight The lineheight you want the text to have in relative units.  This will automatically select the glyph and scale it for you.
                /// @param Text Any text you want printed on the button.
                virtual TextButton* CreateTextButton(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, ConstString& Text);
                /// @brief Gets an already created button by name.
                /// @return Returns a pointer to the button of the specified name.
                virtual Button* GetButton(ConstString& Name);
                /// @brief Gets an already created button by index.
                /// @return Returns a pointer to the button at the specified index.
                virtual Button* GetButton(const Whole& Index);
                /// @brief Gets the number of buttons created and stored in this class.
                /// @return Returns the number of buttons this class is storing.
                virtual Whole GetNumButtons();
                /// @brief Destroys a button.
                /// @param ToBeDestroyed Pointer to the button you want destroyed.
                virtual void DestroyButton(Button* ToBeDestroyed);
                /// @brief Creates a rectangle within this layer.
                /// @return Returns a pointer to the created rectangle.
                /// @param Rect The Rect representing the position and size of the rectangle.
                virtual Rectangle* CreateRectangle(const RenderableRect& Rect);
                /// @brief Gets an already created rectangle by index.
                /// @return Returns a pointer to the rectangle at the specified index.
                virtual Rectangle* GetRectangle(const Whole& Index);
                /// @brief Gets the number of rectangles created and stored in this class.
                /// @return Returns the number of rectangles this class is storing.
                virtual Whole GetNumRectangles();
                /// @brief Destroys a rectangle.
                /// @param ToBeDestroyed Pointer to the rectangle you want destroyed.
                virtual void DestroyRectangle(Rectangle* ToBeDestroyed);
                /// @brief Creates a caption within this layer.
                /// @return Returns a pointer to the created caption.
                /// @param Name The name of this caption.
                /// @param Rect The Rect representing the position and size of the caption.
                /// @param Glyph One of the glyphs specified in your gorilla file.  Must be valid.
                /// @param Text Any text you want printed on the caption.
                virtual Caption* CreateCaption(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text);
                /// @brief Creates a caption within this layer.
                /// @return Returns a pointer to the created caption.
                /// @param Name The name of this caption.
                /// @param Rect The Rect representing the position and size of the caption.
                /// @param LineHeight The lineheight you want the text to have in relative units.  This will automatically select the glyph and scale it for you.
                /// @param Text Any text you want printed on the caption.
                virtual Caption* CreateCaption(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text);
                /// @brief Gets an already created caption by name.
                /// @return Returns a pointer to the caption of the specified name.
                virtual Caption* GetCaption(ConstString& Name);
                /// @brief Gets an already created caption by index.
                /// @return Returns a pointer to the caption at the specified index.
                virtual Caption* GetCaption(const Whole& Index);
                /// @brief Gets the number of captions created and stored in this class.
                /// @return Returns the number of captions this class is storing.
                virtual Whole GetNumCaptions();
                /// @brief Destroys a caption.
                /// @param ToBeDestroyed Pointer to the caption you want destroyed.
                virtual void DestroyCaption(Caption* ToBeDestroyed);
                /// @brief Creates a markup text within this layer.
                /// @details The constructor will ignore the size portion of the Rect passed in, since Markup Texts don't have a default size.
                /// @return Returns a pointer to the created markup text.
                /// @param Name The name of this markup text.
                /// @param Rect The Rect representing the position and size of the markup text.
                /// @param Glyph One of the glyphs specified in your gorilla file.  Must be valid.
                /// @param Text Any text you want printed on the markup text.
                virtual MarkupText* CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text);
                /// @brief Creates a markup text within this layer.
                /// @details The constructor will ignore the size portion of the Rect passed in, since Markup Texts don't have a default size.
                /// @return Returns a pointer to the created markup text.
                /// @param Name The name of this markup text.
                /// @param Rect The Rect representing the position and size of the markup text.
                /// @param LineHeight The lineheight you want the text to have in relative units.  This will automatically select the glyph and scale it for you.
                /// @param Text Any text you want printed on the markup text.
                virtual MarkupText* CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text);
                /// @brief Gets an already created markup text by name.
                /// @return Returns a pointer to the markup text of the specified name.
                virtual MarkupText* GetMarkupText(ConstString& Name);
                /// @brief Gets an already created markup text by index.
                /// @return Returns a pointer to the markup text at the specified index.
                virtual MarkupText* GetMarkupText(const Whole& Index);
                /// @brief Gets the number of markup texts created and stored in this class.
                /// @return Returns the number of markup texts this class is storing.
                virtual Whole GetNumMarkupTexts();
                /// @brief Destroys a markup text.
                /// @param ToBeDestroyed Pointer to the markup text you want destroyed.
                virtual void DestroyMarkupText(MarkupText* ToBeDestroyed);
                /// @brief Creates a line list within this layer.
                virtual LineList* CreateLineList();
                /// @brief Gets an already created line list by index.
                /// @return Returns a pointer to the line list at the specified index.
                virtual LineList* GetLineList(const Whole& Index);
                /// @brief Gets the number of line lists created and stored in this class.
                /// @return Returns the number of line lists this class is storing.
                virtual Whole GetNumLineLists();
                /// @brief Destroys a line list.
                /// @param ToBeDestroyed Pointer to the line list you want destroyed.
                virtual void DestroyLineList(LineList* ToBeDestroyed);
                ///////////////////////////////////////////////////////////////////////////////
                // Working with all Widgets
                ///////////////////////////////////////
                /// @brief Adds a widget to this layer.
                /// @details This function is necessary when creating custom widgets in game logic.
                /// @param ToAdd The widget to be added.
                virtual void AddWidget(Widget* ToAdd);
                /// @brief Gets an already created widget by name.
                /// @return Returns a pointer to the widget of the specified name.
                virtual Widget* GetWidget(ConstString& Name);
                /// @brief Gets an already created widget by index.
                /// @return Returns a pointer to the widget at the specified index.
                virtual Widget* GetWidget(Whole Index);
                /// @brief Gets the number of widgets created and stored in this class.
                /// @return Returns the number of widgets this class is storing.
                virtual Whole GetNumWidgets();
                /// @brief Destroys a widget.
                /// @param ToBeDestroyed Pointer to the widget you want destroyed.
                virtual void DestroyWidget(Widget* ToBeDestroyed);
                ///////////////////////////////////////////////////////////////////////////////
                // Creating Widgets
                ///////////////////////////////////////
                /// @brief Creates a Scrollbar within this layer.
                /// @return Returns a pointer to the created Scrollbar.
                /// @param Name The name of the Scrollbar.
                /// @param Rect The Rect representing the position and size of the Scrollbar.
                /// @param Style The style of scrollbar you want to create, see Scrollbar documentation for more details.
                virtual Scrollbar* CreateScrollbar(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& Style);
                /// @brief Creates a CheckBox within this layer.
                /// @details The label uses the Markup Text class, and thus it's light markup text language.  You can also
                /// pass in a blank string if you don't wish to have a label, you can create a label after construction.
                /// @return Returns a pointer to the created CheckBox.
                /// @param Name The name of the CheckBox.
                /// @param Rect The Rect representing the position and size of the CheckBox.
                /// @param LineHeight The lineheight you want the text to have. If the Rect passed in is relative, this will expect LineHeight to be relative as well.
                /// @param LabelText The text to display with the label.
                virtual CheckBox* CreateCheckBox(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String &LabelText);
                /// @brief Creates a List Box within this layer.
                /// @return Returns a pointer to the created List Box.
                /// @param Name The name of the List Box.
                /// @param Rect The Rect representing the position and size of the List Box.
                /// @param ScrollbarStyle The style of scrollbar you want to create, see Scrollbar documentation for more details.
                virtual ListBox* CreateListBox(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& ScrollbarStyle);
                /// @brief Creates a Window within this layer.
                /// @return Returns a pointer to the created Window.
                /// @param Name The name of the Window.
                /// @param Rect The Rect representing the position and size of the Window.
                virtual Window* CreateWidgetWindow(ConstString& Name, const RenderableRect& Rect);
                /// @brief Creates a Menu within this layer.
                /// @return Returns a pointer to the created Menu.
                /// @param Name The name of the Menu.
                /// @param Rect The Rect representing the position and size of the Menu.
                virtual Menu* CreateMenu(ConstString& Name, const RenderableRect& Rect);
                /// @brief Creates a Spinner within this layer.
                /// @return Returns a pointer to the created Spinner.
                /// @param Name The Name for the Widget.
                /// @param Rect The Rect representing the position and size of the Spinner.
                /// @param SStyle The layout of buttons this widget will have.  See SpinnerStyle enum or class description for more details.
                /// @param GlyphHeight The desired relative height of the text you want.
                virtual Spinner* CreateSpinner(ConstString& Name, const RenderableRect& Rect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight);
                /// @brief Creates a scrolled cell grid within this layer.
                /// @return Returns a pointer to the created ScrolledCellGrid.
                /// @param Name The name of the widget.
                /// @param Rect The Rect representing the position and size of the ScrolledCellGrid.
                /// @param Thickness The width of the vertical scrollbar in relative units.  The same amount of actual pixels is used
                /// to determine the height of the horizontal scrollbar.
                /// @param Style An enum value representing how you want your scrollbar constructed.  See class details for more info.
                virtual ScrolledCellGrid* CreateScrolledCellGrid(ConstString& Name, const RenderableRect& Rect, const Real& Thickness, const UI::ScrollbarStyle& Style);
                /// @brief Creates a paged cell grid within this layer.
                /// @return Returns a pointer to the created PagedCellGrid.
                /// @param Name The name of the widget.
                /// @param Rect The Rect representing the position and size of the PagedCellGrid.
                /// @param SpnRect The Rect representing the position and size of the Spinner.
                /// @param SStyle The style of spinner to create.
                /// @param GlyphHeight The desired lineheight of the glyphs to be used with the spinner.
                virtual PagedCellGrid* CreatePagedCellGrid(ConstString& Name, const RenderableRect& Rect, const RenderableRect& SpnRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight);
                /// @brief Creates a drop down list within this layer.
                /// @return Returns a pointer to the created DropDownList.
                /// @param Name The Name for the Widget.
                /// @param Rect The renderable rect representing the position and size of this widget.
                /// @param LineHeight The lineheight you want the text to have. If the Rect passed in is relative, this will expect LineHeight to be relative as well.
                /// @param ScrollStyle The style of the scrollbar you want for this List Box.  See Scrollbar class for more information.
                virtual DropDownList* CreateDropDownList(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const UI::ScrollbarStyle& ScrollStyle);
                /// @brief Creates a tabset within this layer.
                /// @return Returns a pointer to the created tabset.
                /// @param Name The Name for the Widget.
                /// @param SetRect The Rect representing the position and size of all the Renderable Sets generated by the tabset.
                virtual TabSet* CreateTabSet(const String& Name, const RenderableRect& SetRect);
                ///////////////////////////////////////////////////////////////////////////////
                // UI Element and Widget Checks
                ///////////////////////////////////////
                /// @brief Gets the button the mouse is over if any.
                /// @details This function searches only the buttons contained in this layer.
                /// @return Returns the button the mouse is over, or NULL if there are none.
                virtual Button* CheckButtonMouseIsOver();
                /// @brief Gets the widget the mouse is over if any.
                /// @details This function searches only the widgets contained in this layer.
                /// @return Returns the widget the mouse is over, or NULL if there are none.
                virtual Widget* CheckWidgetMouseIsOver();
                /// @brief Updates UI elements positions and sizes after a change in the viewport size.
                /// @details This function is called automatically, and shouldn't need to be called manually.
                /// @param OldViewportSize The updated size of the viewport this layer belongs to.
                virtual void ViewportUpdate(const Vector2& OldViewportSize);
                ///////////////////////////////////////////////////////////////////////////////
                // Internal Functions
                ///////////////////////////////////////
                /// @internal
                /// @brief Gets the internal gorilla layer pointer.
                virtual Gorilla::Layer* GetGorillaLayer();
        };//layer
    }//ui
}//phys

#endif
