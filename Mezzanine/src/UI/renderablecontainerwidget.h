//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _uirenderablecontainerwidget_h
#define _uirenderablecontainerwidget_h

#include "UI/resizinginfo.h"
#include "UI/widget.h"

namespace Mezzanine
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
        class ListBox;
        class Spinner;
        class ScrolledCellGrid;
        class PagedCellGrid;
        class DropDownList;
        class TabSet;
        //typedefs
        typedef ResizingInfo<Button*> OffsetButtonInfo;
        typedef ResizingInfo<Rectangle*> OffsetRectangleInfo;
        typedef ResizingInfo<Caption*> OffsetCaptionInfo;
        typedef ResizingInfo<MarkupText*> OffsetMarkupTextInfo;
        typedef ResizingInfo<Widget*> OffsetWidgetInfo;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class RenderableContainerWidget
        /// @headerfile uirenderablecontainerwidget.h
        /// @brief This is an abstract class for the creation and storage of widgets to be used by other classes.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB RenderableContainerWidget : public Widget
        {
            protected:
                Rectangle* WidgetBack;
                Whole RenderablesAdded;
                std::vector<OffsetButtonInfo> Buttons;
                std::vector<OffsetRectangleInfo> Rectangles;
                std::vector<OffsetCaptionInfo> Captions;
                std::vector<OffsetMarkupTextInfo> MarkupTexts;
                std::vector<OffsetWidgetInfo> Widgets;
                /// @brief Child specific update method.
                virtual void UpdateImpl(bool Force = false);
                /// @brief Child specific visibility method.
                virtual void SetVisibleImpl(bool visible);
                /// @brief Child specific mouse hover method.
                virtual bool CheckMouseHoverImpl();
                /// @brief Internal function for setting the location(position) of this widget.
                virtual void SetLocation(const Vector2& Position);
                /// @brief Internal function for setting the area(size) of this widget.
                virtual void SetArea(const Vector2& Size);
                /// @brief Calculates a new offset for an individual element.
                virtual Vector2 CalculateOffset(const Vector2& NewSize, const Vector2& OldSize, const Vector2& EleOffset, const UI::ResizeableAnchor& Anchor);
                /// @brief Calculates a new size for an individual element.
                virtual Vector2 CalculateSize(const Vector2& NewSize, const Vector2& OldSize, const Vector2& EleSize, const UI::ResizeableTether& Tether);
            public:
                /// @brief Class constructor.
                /// @param name The name of the window.
                /// @param Rect The Rect representing the position and size of the window.
                /// @param PLayer The parent layer this window belongs to.
                RenderableContainerWidget(ConstString& name, const RenderableRect& Rect, Layer* PLayer);
                /// @brief Class destructor.
                virtual ~RenderableContainerWidget();
                /// @brief Sets the relative position of this widget.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Position A vector2 representing the relative position of this widget.
                virtual void SetPosition(const Vector2& Position);
                /// @brief Sets the pixel position of this widget.
                /// @param Position A vector2 representing the pixel position of this widget.
                virtual void SetActualPosition(const Vector2& Position);
                /// @brief Sets the relative size of this widget.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Size A vector2 representing the relative size of this widget.
                virtual void SetSize(const Vector2& Size);
                /// @brief Sets the pixel size of this widget.
                /// @param Size A vector2 representing the pixel size of this widget.
                virtual void SetActualSize(const Vector2& Size);
                /// @brief Updates the dimensions of this widget to match those of the new screen size.
                /// @details This function is called automatically when a viewport changes in size, and shouldn't need to be called manually.
                virtual void UpdateDimensions();
                /// @brief Gets the background object of this widget.
                /// @return Returns a pointer to the rectangle that is the background for this widget.
                virtual Rectangle* GetWidgetBack();
                ///////////////////////////////////////////////////////////////////////////////
                // Creating and working with All Basic UI Elements
                ///////////////////////////////////////
                /// @brief Creates a button within this widget.
                /// @return Returns a pointer to the created button.
                /// @param Name The name of the button.
                /// @param Rect The Rect representing the position and size of the button.
                virtual Button* CreateButton(ConstString& Name, const RenderableRect& Rect);
                /// @brief Creates a text button within this widget.
                /// @return Returns a pointer to the created button.
                /// @param Name The name of the button.
                /// @param Rect The Rect representing the position and size of the button.
                /// @param Glyph One of the glyphs specified in your mta file.  Must be valid.
                /// @param Text Any text you want printed on the button.
                virtual TextButton* CreateTextButton(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, ConstString& Text);
                /// @brief Creates a text button within this widget.
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
                /// @brief Gets the OffsetButtonInfo of an already created button by name.
                /// @return Returns a reference to the button of the specified name.
                virtual OffsetButtonInfo* GetOffsetButtonInfo(ConstString& Name);
                /// @brief Gets the OffsetButtonInfo of an already created button by index.
                /// @return Returns a reference to the button at the specified index.
                virtual OffsetButtonInfo* GetOffsetButtonInfo(const Whole& Index);
                /// @brief Gets the number of buttons created and stored in this class.
                /// @return Returns the number of buttons this class is storing.
                virtual Whole GetNumButtons();
                /// @brief Destroys a button.
                /// @param ToBeDestroyed Pointer to the button you want destroyed.
                virtual void DestroyButton(UI::Button* ToBeDestroyed);
                /// @brief Creates a rectangle within this widget.
                /// @return Returns a pointer to the created rectangle.
                /// @param Rect The Rect representing the position and size of the rectangle.
                virtual Rectangle* CreateRectangle(const RenderableRect& Rect);
                /// @brief Gets an already created rectangle by index.
                /// @return Returns a pointer to the rectangle at the specified index.
                virtual Rectangle* GetRectangle(const Whole& Index);
                /// @brief Gets the OffsetRectangleInfo of an already created rectangle by index.
                /// @return Returns a reference to the rectangle at the specified index.
                virtual OffsetRectangleInfo* GetOffsetRectangleInfo(const Whole& Index);
                /// @brief Gets the number of rectangles created and stored in this class.
                /// @return Returns the number of rectangles this class is storing.
                virtual Whole GetNumRectangles();
                /// @brief Destroys a rectangle.
                /// @param ToBeDestroyed Pointer to the rectangle you want destroyed.
                virtual void DestroyRectangle(UI::Rectangle* ToBeDestroyed);
                /// @brief Creates a caption within this widget.
                /// @return Returns a pointer to the created caption.
                /// @param Name The name of this caption.
                /// @param Rect The Rect representing the position and size of the caption.
                /// @param Glyph One of the glyphs specified in your mta file.  Must be valid.
                /// @param Text Any text you want printed on the caption.
                virtual Caption* CreateCaption(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text);
                /// @brief Creates a caption within this widget.
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
                /// @brief Gets the OffsetCaptionInfo of an already created caption by name.
                /// @return Returns a reference to the caption of the specified name.
                virtual OffsetCaptionInfo* GetOffsetCaptionInfo(ConstString& Name);
                /// @brief Gets the OffsetCaptionInfo of an already created caption by index.
                /// @return Returns a reference to the caption at the specified index.
                virtual OffsetCaptionInfo* GetOffsetCaptionInfo(const Whole& Index);
                /// @brief Gets the number of captions created and stored in this class.
                /// @return Returns the number of captions this class is storing.
                virtual Whole GetNumCaptions();
                /// @brief Destroys a caption.
                /// @param ToBeDestroyed Pointer to the caption you want destroyed.
                virtual void DestroyCaption(UI::Caption* ToBeDestroyed);
                /// @brief Creates a markup text within this widget.
                /// @details The constructor will ignore the size portion of the Rect passed in, since Markup Texts don't have a default size.
                /// @return Returns a pointer to the created markup text.
                /// @param Name The name of this markup text.
                /// @param Rect The Rect representing the position and size of the markup text.
                /// @param Glyph One of the glyphs specified in your mta file.  Must be valid.
                /// @param Text Any text you want printed on the markup text.
                virtual MarkupText* CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text);
                /// @brief Creates a markup text within this widget.
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
                /// @brief Gets the OffsetMarkupTextInfo of an already created markup text by name.
                /// @return Returns a reference to the markup text of the specified name.
                virtual OffsetMarkupTextInfo* GetOffsetMarkupTextInfo(ConstString& Name);
                /// @brief Gets the OffsetMarkupTextInfo of an already created markup text by index.
                /// @return Returns a reference to the markup text at the specified index.
                virtual OffsetMarkupTextInfo* GetOffsetMarkupTextInfo(const Whole& Index);
                /// @brief Gets the number of markup texts created and stored in this class.
                /// @return Returns the number of markup texts this class is storing.
                virtual Whole GetNumMarkupTexts();
                /// @brief Destroys a markup text.
                /// @param ToBeDestroyed Pointer to the markup text you want destroyed.
                virtual void DestroyMarkupText(UI::MarkupText* ToBeDestroyed);
                ///////////////////////////////////////////////////////////////////////////////
                // Working with all Widgets
                ///////////////////////////////////////
                /// @brief Gets an already created widget by name.
                /// @return Returns a pointer to the widget of the specified name.
                virtual Widget* GetWidget(ConstString& Name);
                /// @brief Gets an already created widget by index.
                /// @return Returns a pointer to the widget at the specified index.
                virtual Widget* GetWidget(const Whole& Index);
                /// @brief Gets the OffsetWidgetInfo of an already created widget by name.
                /// @return Returns a reference to the widget of the specified name.
                virtual OffsetWidgetInfo* GetOffsetWidgetInfo(ConstString& Name);
                /// @brief Gets the OffsetWidgetInfo of an already created widget by index.
                /// @return Returns a reference to the widget at the specified index.
                virtual OffsetWidgetInfo* GetOffsetWidgetInfo(const Whole& Index);
                /// @brief Gets the number of widgets created and stored in this class.
                /// @return Returns the number of widgets this class is storing.
                virtual Whole GetNumWidgets();
                /// @brief Destroys a widget.
                /// @param ToBeDestroyed Pointer to the widget you want destroyed.
                virtual void DestroyWidget(UI::Widget* ToBeDestroyed);
                ///////////////////////////////////////////////////////////////////////////////
                // Creating Widgets
                ///////////////////////////////////////
                /// @brief Creates a Scrollbar within this widget.
                /// @return Returns a pointer to the created Scrollbar.
                /// @param Name The name of the Scrollbar.
                /// @param Rect The Rect representing the position and size of the widget.
                /// @param Style The style of scrollbar you want to create, see Scrollbar documentation for more details.
                virtual Scrollbar* CreateScrollbar(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& Style);
                /// @brief Creates a CheckBox within this widget.
                /// @details The label uses the Markup Text class, and thus it's light markup text language.  You can also
                /// pass in a blank string if you don't wish to have a label, you can create a label after construction.
                /// @return Returns a pointer to the created CheckBox.
                /// @param Name The name of the CheckBox.
                /// @param Rect The Rect representing the position and size of the widget.
                /// @param LineHeight The lineheight you want the text to have. If the Rect passed in is relative, this will expect LineHeight to be relative as well.
                /// @param LabelText The text to display with the label.
                virtual CheckBox* CreateCheckBox(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String &LabelText);
                /// @brief Creates a List Box within this widget.
                /// @return Returns a pointer to the created List Box.
                /// @param Name The name of the List Box.
                /// @param Rect The Rect representing the position and size of the widget.
                /// @param ScrollbarStyle The style of scrollbar you want to create, see Scrollbar documentation for more details.
                virtual ListBox* CreateListBox(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& ScrollbarStyle);
                /// @brief Creates a Spinner within this widget.
                /// @return Returns a pointer to the created Spinner.
                /// @param Name The Name for the Widget.
                /// @param Rect The Rect representing the position and size of the widget.
                /// @param SStyle The layout of buttons this widget will have.  See SpinnerStyle enum or class description for more details.
                /// @param GlyphHeight The desired relative height of the text you want.
                virtual Spinner* CreateSpinner(ConstString& Name, const RenderableRect& Rect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight);
                /// @brief Creates a scrolled cell grid within this widget.
                /// @return Returns a pointer to the created ScrolledCellGrid.
                /// @param Name The name of the widget.
                /// @param Rect The Rect representing the position and size of the widget.
                /// @param Thickness The width of the vertical scrollbar in relative units.  The same amount of actual pixels is used
                /// to determine the height of the horizontal scrollbar.
                /// @param Style An enum value representing how you want your scrollbar constructed.  See class details for more info.
                virtual ScrolledCellGrid* CreateScrolledCellGrid(ConstString& Name, const RenderableRect& Rect, const Real& Thickness, const UI::ScrollbarStyle& Style);
                /// @brief Creates a paged cell grid within this widget.
                /// @return Returns a pointer to the created PagedCellGrid.
                /// @param Name The name of the widget.
                /// @param Rect The Rect representing the position and size of the CellGrid.
                /// @param SpnRect The Rect representing the position and size of the Spinner.
                /// @param SStyle The style of spinner to create.
                /// @param GlyphHeight The desired lineheight of the glyphs to be used with the spinner.
                virtual PagedCellGrid* CreatePagedCellGrid(ConstString& Name, const RenderableRect& Rect, const RenderableRect& SpnRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight);
                /// @brief Creates a drop down list within this widget.
                /// @return Returns a pointer to the created DropDownList.
                /// @param Name The Name for the Widget.
                /// @param Rect The renderable rect representing the position and size of this widget.
                /// @param LineHeight The lineheight you want the text to have. If the Rect passed in is relative, this will expect LineHeight to be relative as well.
                /// @param ScrollStyle The style of the scrollbar you want for this List Box.  See Scrollbar class for more information.
                virtual DropDownList* CreateDropDownList(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const UI::ScrollbarStyle& ScrollStyle);
                /// @brief Creates a tabset within this widget.
                /// @return Returns a pointer to the created tabset.
                /// @param Name The Name for the Widget.
                /// @param SetRect The Rect representing the position and size of all the Renderable Sets generated by the tabset.
                virtual TabSet* CreateTabSet(ConstString& Name, const RenderableRect& SetRect);
        };//WidgetContainer
    }//ui
}//Mezzanine

#endif
