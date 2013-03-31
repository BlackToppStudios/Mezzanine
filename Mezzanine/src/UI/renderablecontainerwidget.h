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
#ifndef _uirenderablecontainerwidget_h
#define _uirenderablecontainerwidget_h

#include "UI/resizinginfo.h"
#include "UI/widget.h"

namespace Mezzanine
{
    namespace UI
    {
        //forward declares
        class BasicRenderable;
        class AreaRenderable;
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
        ///////////////////////////////////////////////////////////////////////////////
        /// @class RenderableContainerWidget
        /// @headerfile uirenderablecontainerwidget.h
        /// @brief This is an interface class for the two types of Renderable Container Widgets.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB RenderableContainerWidget : public Widget
        {
            protected:
                Whole RenderablesAdded;
                /// @copydoc Widget::UpdateImpl(bool Force)
                virtual void UpdateImpl(bool Force = false);
                /// @copydoc Widget::SetVisibleImpl(bool visible)
                virtual void SetVisibleImpl(bool visible) = 0;
                /// @copydoc Widget::CheckMouseHoverImpl()
                virtual bool CheckMouseHoverImpl() = 0;
            //public:
                /// @brief Class constructor.
                /// @param Parent The parent Screen that created this widget.
                /// @param name The Name for the Widget.
                RenderableContainerWidget(const String& name, Screen* Parent);
                /// @brief Class destructor.
                virtual ~RenderableContainerWidget();
            public:
                ///////////////////////////////////////////////////////////////////////////////
                // Creating BasicRenderables

                /// @brief Creates a button.
                /// @return Returns a pointer to the created button.
                /// @param Name The name of the button.
                /// @param Rect The Rect representing the position and size of the button.
                virtual Button* CreateButton(ConstString& Name, const RenderableRect& Rect) = 0;
                /// @brief Creates a text button.
                /// @return Returns a pointer to the created button.
                /// @param Name The name of the button.
                /// @param Rect The Rect representing the position and size of the button.
                /// @param Glyph One of the glyphs specified in your mta file.  Must be valid.
                /// @param Text Any text you want printed on the button.
                virtual Button* CreateButton(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, ConstString& Text) = 0;
                /// @brief Creates a text button.
                /// @return Returns a pointer to the created button.
                /// @param Name The name of the button.
                /// @param Rect The Rect representing the position and size of the button.
                /// @param LineHeight The lineheight you want the text to have in relative units.  This will automatically select the glyph and scale it for you.
                /// @param Text Any text you want printed on the button.
                virtual Button* CreateButton(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, ConstString& Text) = 0;
                /// @brief Creates a rectangle.
                /// @return Returns a pointer to the created rectangle.
                /// @param Rect The Rect representing the position and size of the rectangle.
                virtual Rectangle* CreateRectangle(const RenderableRect& Rect) = 0;
                /// @brief Creates a caption.
                /// @return Returns a pointer to the created caption.
                /// @param Name The name of this caption.
                /// @param Rect The Rect representing the position and size of the caption.
                /// @param Glyph One of the glyphs specified in your mta file.  Must be valid.
                /// @param Text Any text you want printed on the caption.
                virtual Caption* CreateCaption(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text) = 0;
                /// @brief Creates a caption.
                /// @return Returns a pointer to the created caption.
                /// @param Name The name of this caption.
                /// @param Rect The Rect representing the position and size of the caption.
                /// @param LineHeight The lineheight you want the text to have in relative units.  This will automatically select the glyph and scale it for you.
                /// @param Text Any text you want printed on the caption.
                virtual Caption* CreateCaption(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text) = 0;
                /// @brief Creates a markup text.
                /// @details The constructor will ignore the size portion of the Rect passed in, since Markup Texts don't have a default size.
                /// @return Returns a pointer to the created markup text.
                /// @param Name The name of this markup text.
                /// @param Rect The Rect representing the position and size of the markup text.
                /// @param Glyph One of the glyphs specified in your mta file.  Must be valid.
                /// @param Text Any text you want printed on the markup text.
                virtual MarkupText* CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text) = 0;
                /// @brief Creates a markup text.
                /// @details The constructor will ignore the size portion of the Rect passed in, since Markup Texts don't have a default size.
                /// @return Returns a pointer to the created markup text.
                /// @param Name The name of this markup text.
                /// @param Rect The Rect representing the position and size of the markup text.
                /// @param LineHeight The lineheight you want the text to have in relative units.  This will automatically select the glyph and scale it for you.
                /// @param Text Any text you want printed on the markup text.
                virtual MarkupText* CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text) = 0;

                ///////////////////////////////////////////////////////////////////////////////
                // Working with All AreaRenderables

                /// @brief Gets an already created AreaRenderable by name.
                /// @return Returns a pointer to the AreaRenderable of the specified name.
                virtual AreaRenderable* GetAreaRenderable(ConstString& Name) = 0;
                /// @brief Gets an already created AreaRenderable by index.
                /// @return Returns a pointer to the AreaRenderable at the specified index.
                virtual AreaRenderable* GetAreaRenderable(const Whole& Index) = 0;
                /// @brief Gets the number of widgets created and stored in this class.
                /// @return Returns the number of widgets this class is storing.
                virtual Whole GetNumAreaRenderables() = 0;
                /// @brief Destroys an AreaRenderable.
                /// @param ToBeDestroyed Pointer to the AreaRenderable you want destroyed.
                virtual void DestroyAreaRenderable(AreaRenderable* ToBeDestroyed) = 0;
                /// @brief Destroys all AreaRenderables in this widget.
                virtual void DestroyAllAreaRenderables() = 0;

                ///////////////////////////////////////////////////////////////////////////////
                // Creating Widgets

                /// @brief Creates a Scrollbar.
                /// @return Returns a pointer to the created Scrollbar.
                /// @param Name The name of the Scrollbar.
                /// @param Rect The Rect representing the position and size of the Scrollbar.
                /// @param Style The style of scrollbar you want to create, see Scrollbar documentation for more details.
                virtual Scrollbar* CreateScrollbar(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& Style) = 0;
                /// @brief Creates a CheckBox.
                /// @details The label uses the Markup Text class, and thus it's light markup text language.  You can also
                /// pass in a blank string if you don't wish to have a label, you can create a label after construction.
                /// @return Returns a pointer to the created CheckBox.
                /// @param Name The name of the CheckBox.
                /// @param Rect The Rect representing the position and size of the CheckBox.
                /// @param LineHeight The lineheight you want the text to have. If the Rect passed in is relative, this will expect LineHeight to be relative as well.
                /// @param LabelText The text to display with the label.
                virtual CheckBox* CreateCheckBox(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String &LabelText) = 0;
                /// @brief Creates a List Box.
                /// @return Returns a pointer to the created List Box.
                /// @param Name The name of the List Box.
                /// @param Rect The Rect representing the position and size of the List Box.
                /// @param ScrollStyle The style of scrollbar you want to create, see Scrollbar documentation for more details.
                virtual ListBox* CreateListBox(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& ScrollStyle) = 0;
                /// @brief Creates a Spinner.
                /// @return Returns a pointer to the created Spinner.
                /// @param Name The Name for the Widget.
                /// @param Rect The Rect representing the position and size of the Spinner.
                /// @param SStyle The layout of buttons this widget will have.  See SpinnerStyle enum or class description for more details.
                /// @param GlyphHeight The desired relative height of the text you want.
                virtual Spinner* CreateSpinner(ConstString& Name, const RenderableRect& Rect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight) = 0;
                /// @brief Creates a scrolled cell grid.
                /// @return Returns a pointer to the created ScrolledCellGrid.
                /// @param Name The name of the widget.
                /// @param Rect The Rect representing the position and size of the ScrolledCellGrid.
                /// @param Thickness The width of the vertical scrollbar in relative units.  The same amount of actual pixels is used
                /// to determine the height of the horizontal scrollbar.
                /// @param Style An enum value representing how you want your scrollbar constructed.  See class details for more info.
                virtual ScrolledCellGrid* CreateScrolledCellGrid(ConstString& Name, const RenderableRect& Rect, const Real& Thickness, const UI::ScrollbarStyle& Style) = 0;
                /// @brief Creates a paged cell grid.
                /// @return Returns a pointer to the created PagedCellGrid.
                /// @param Name The name of the widget.
                /// @param Rect The Rect representing the position and size of the PagedCellGrid.
                /// @param SpnRect The Rect representing the position and size of the Spinner.
                /// @param SStyle The style of spinner to create.
                /// @param GlyphHeight The desired lineheight of the glyphs to be used with the spinner.
                virtual PagedCellGrid* CreatePagedCellGrid(ConstString& Name, const RenderableRect& Rect, const RenderableRect& SpnRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight) = 0;
                /// @brief Creates a drop down list.
                /// @return Returns a pointer to the created DropDownList.
                /// @param Name The Name for the Widget.
                /// @param Rect The renderable rect representing the position and size of this widget.
                /// @param LineHeight The lineheight you want the text to have. If the Rect passed in is relative, this will expect LineHeight to be relative as well.
                /// @param ScrollStyle The style of the scrollbar you want for this List Box.  See Scrollbar class for more information.
                virtual DropDownList* CreateDropDownList(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const UI::ScrollbarStyle& ScrollStyle) = 0;
                /// @brief Creates a tabset.
                /// @return Returns a pointer to the created tabset.
                /// @param Name The Name for the Widget.
                /// @param SetRect The Rect representing the position and size of all the Renderable Sets generated by the tabset.
                virtual TabSet* CreateTabSet(ConstString& Name, const RenderableRect& SetRect) = 0;

                ///////////////////////////////////////////////////////////////////////////////
                // Working with all Widgets

                /// @brief Gets an already created widget by name.
                /// @return Returns a pointer to the widget of the specified name.
                virtual Widget* GetWidget(ConstString& Name) = 0;
                /// @brief Gets an already created widget by index.
                /// @return Returns a pointer to the widget at the specified index.
                virtual Widget* GetWidget(const Whole& Index) = 0;
                /// @brief Gets the number of widgets created and stored in this class.
                /// @return Returns the number of widgets this class is storing.
                virtual Whole GetNumWidgets() = 0;
                /// @brief Destroys a widget.
                /// @param ToBeDestroyed Pointer to the widget you want destroyed.
                virtual void DestroyWidget(Widget* ToBeDestroyed) = 0;
                /// @brief Destroys all Widgets in this widget.
                virtual void DestroyAllWidgets() = 0;

                ///////////////////////////////////////////////////////////////////////////////
                // Inherited from Widget

                /// @copydoc Widget::SetPosition(const Vector2& Position)
                virtual void SetPosition(const Vector2& Position) = 0;
                /// @copydoc Widget::SetActualPosition(const Vector2& Position)
                virtual void SetActualPosition(const Vector2& Position) = 0;
                /// @copydoc Widget::SetSize(const Vector2& Size)
                virtual void SetSize(const Vector2& Size) = 0;
                /// @copydoc Widget::SetActualSize(const Vector2& Size)
                virtual void SetActualSize(const Vector2& Size) = 0;
                /// @copydoc Widget::UpdateDimensions()
                virtual void UpdateDimensions() = 0;
        };//RenderableContainerWidget

        ///////////////////////////////////////////////////////////////////////////////
        /// @class OpenRenderableContainerWidget
        /// @headerfile uirenderablecontainerwidget.h
        /// @brief
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB OpenRenderableContainerWidget : public RenderableContainerWidget
        {
            public:
                typedef std::vector<AreaRenderable*>             AreaRenderableContainer;
                typedef AreaRenderableContainer::iterator        AreaRenderableIterator;
                typedef AreaRenderableContainer::const_iterator  ConstAreaRenderableIterator;
                typedef std::vector<Widget*>                     WidgetContainer;
                typedef WidgetContainer::iterator                WidgetIterator;
                typedef WidgetContainer::const_iterator          ConstWidgetIterator;
            protected:
                friend class ExtendedRenderableFactory;
                AreaRenderableContainer AreaRenderables;
                WidgetContainer Widgets;
                /// @internal
                /// @brief Child specific visibility method.
                virtual void SetVisibleImpl(bool visible);
                /// @internal
                /// @brief Child specific mouse hover method.
                virtual bool CheckMouseHoverImpl();
            //public:
                /// @brief Class constructor.
                /// @param Parent The parent Screen that created this widget.
                /// @param name The Name for the Widget.
                OpenRenderableContainerWidget(const String& name, Screen* Parent);
                /// @brief Class destructor.
                virtual ~OpenRenderableContainerWidget();
            public:
                ///////////////////////////////////////////////////////////////////////////////
                // Creating BasicRenderables

                /// @copydoc RenderableContainerWidget::CreateRectangle(const RenderableRect& Rect)
                virtual Rectangle* CreateRectangle(const RenderableRect& Rect);
                /// @copydoc RenderableContainerWidget::CreateCaption(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text)
                virtual Caption* CreateCaption(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text);
                /// @copydoc RenderableContainerWidget::CreateCaption(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text)
                virtual Caption* CreateCaption(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text);
                /// @copydoc RenderableContainerWidget::CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text)
                virtual MarkupText* CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text);
                /// @copydoc RenderableContainerWidget::CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text)
                virtual MarkupText* CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text);

                ///////////////////////////////////////////////////////////////////////////////
                // Working with All AreaRenderables

                /// @copydoc RenderableContainerWidget::GetAreaRenderable(ConstString& Name)
                virtual AreaRenderable* GetAreaRenderable(ConstString& Name);
                /// @copydoc RenderableContainerWidget::GetAreaRenderable(const Whole& Index)
                virtual AreaRenderable* GetAreaRenderable(const Whole& Index);
                /// @copydoc RenderableContainerWidget::GetNumAreaRenderables()
                virtual Whole GetNumAreaRenderables();
                /// @copydoc RenderableContainerWidget::DestroyAreaRenderable(AreaRenderable* ToBeDestroyed)
                virtual void DestroyAreaRenderable(AreaRenderable* ToBeDestroyed);
                /// @copydoc RenderableContainerWidget::DestroyAllAreaRenderables()
                virtual void DestroyAllAreaRenderables();

                ///////////////////////////////////////////////////////////////////////////////
                // Creating Widgets

                /// @copydoc RenderableContainerWidget::CreateButton(ConstString& Name, const RenderableRect& Rect)
                virtual Button* CreateButton(ConstString& Name, const RenderableRect& Rect);
                /// @copydoc RenderableContainerWidget::CreateButton(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, ConstString& Text)
                virtual Button* CreateButton(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, ConstString& Text);
                /// @copydoc RenderableContainerWidget::CreateButton(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, ConstString& Text)
                virtual Button* CreateButton(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, ConstString& Text);
                /// @copydoc RenderableContainerWidget::CreateScrollbar(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& Style)
                virtual Scrollbar* CreateScrollbar(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& Style);
                /// @copydoc RenderableContainerWidget::CreateCheckBox(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String &LabelText)
                virtual CheckBox* CreateCheckBox(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String &LabelText);
                /// @copydoc RenderableContainerWidget::CreateListBox(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& ScrollStyle)
                virtual ListBox* CreateListBox(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& ScrollStyle);
                /// @copydoc RenderableContainerWidget::CreateSpinner(ConstString& Name, const RenderableRect& Rect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
                virtual Spinner* CreateSpinner(ConstString& Name, const RenderableRect& Rect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight);
                /// @copydoc RenderableContainerWidget::CreateScrolledCellGrid(ConstString& Name, const RenderableRect& Rect, const Real& Thickness, const UI::ScrollbarStyle& Style)
                virtual ScrolledCellGrid* CreateScrolledCellGrid(ConstString& Name, const RenderableRect& Rect, const Real& Thickness, const UI::ScrollbarStyle& Style);
                /// @copydoc RenderableContainerWidget::CreatePagedCellGrid(ConstString& Name, const RenderableRect& Rect, const RenderableRect& SpnRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
                virtual PagedCellGrid* CreatePagedCellGrid(ConstString& Name, const RenderableRect& Rect, const RenderableRect& SpnRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight);
                /// @copydoc RenderableContainerWidget::CreateDropDownList(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const UI::ScrollbarStyle& ScrollStyle)
                virtual DropDownList* CreateDropDownList(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const UI::ScrollbarStyle& ScrollStyle);
                /// @copydoc RenderableContainerWidget::CreateTabSet(ConstString& Name, const RenderableRect& SetRect)
                virtual TabSet* CreateTabSet(ConstString& Name, const RenderableRect& SetRect);

                ///////////////////////////////////////////////////////////////////////////////
                // Working with all Widgets

                /// @copydoc RenderableContainerWidget::GetWidget(ConstString& Name)
                virtual Widget* GetWidget(ConstString& Name);
                /// @copydoc RenderableContainerWidget::GetWidget(const Whole& Index)
                virtual Widget* GetWidget(const Whole& Index);
                /// @copydoc RenderableContainerWidget::GetNumWidgets()
                virtual Whole GetNumWidgets();
                /// @copydoc RenderableContainerWidget::DestroyWidget(Widget* ToBeDestroyed)
                virtual void DestroyWidget(Widget* ToBeDestroyed);
                /// @copydoc RenderableContainerWidget::DestroyAllWidgets()
                virtual void DestroyAllWidgets();

                ///////////////////////////////////////////////////////////////////////////////
                // Inherited from Widget

                /// @copydoc Widget::SetPosition(const Vector2& Position)
                virtual void SetPosition(const Vector2& Position);
                /// @copydoc Widget::SetActualPosition(const Vector2& Position)
                virtual void SetActualPosition(const Vector2& Position);
                /// @copydoc Widget::SetSize(const Vector2& Size)
                virtual void SetSize(const Vector2& Size);
                /// @copydoc Widget::SetActualSize(const Vector2& Size)
                virtual void SetActualSize(const Vector2& Size);
                /// @copydoc Widget::UpdateDimensions()
                virtual void UpdateDimensions();
        };//OpenRenderableContainerWidget

        ///////////////////////////////////////////////////////////////////////////////
        /// @class EnclosedRenderableContainerWidget
        /// @headerfile uirenderablecontainerwidget.h
        /// @brief This is an abstract class for the creation and storage of widgets to be used by other classes.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB EnclosedRenderableContainerWidget : public RenderableContainerWidget
        {
            public:
                typedef ResizingInfo<AreaRenderable*> OffsetAreaRenderableInfo;
                typedef ResizingInfo<Widget*> OffsetWidgetInfo;
                typedef std::vector<OffsetAreaRenderableInfo>        AreaRenderableInfoContainer;
                typedef AreaRenderableInfoContainer::iterator        AreaRenderableInfoIterator;
                typedef AreaRenderableInfoContainer::const_iterator  ConstAreaRenderableInfoIterator;
                typedef std::vector<OffsetWidgetInfo>                WidgetInfoContainer;
                typedef WidgetInfoContainer::iterator                WidgetInfoIterator;
                typedef WidgetInfoContainer::const_iterator          ConstWidgetInfoIterator;
            protected:
                friend class RenderableFactory;
                Rectangle* WidgetBack;
                AreaRenderableInfoContainer AreaRenderables;
                WidgetInfoContainer Widgets;
                /// @internal
                /// @brief Child specific visibility method.
                virtual void SetVisibleImpl(bool visible);
                /// @internal
                /// @brief Child specific mouse hover method.
                virtual bool CheckMouseHoverImpl();
                /// @internal
                /// @brief Internal function for setting the location(position) of this widget.
                virtual void SetLocation(const Vector2& Position);
                /// @internal
                /// @brief Internal function for setting the area(size) of this widget.
                virtual void SetArea(const Vector2& Size);
                /// @internal
                /// @brief Calculates a new offset for an individual element.
                virtual Vector2 CalculateOffset(const Vector2& NewSize, const Vector2& OldSize, const Vector2& EleOffset, const UI::ResizeableAnchor& Anchor);
                /// @internal
                /// @brief Calculates a new size for an individual element.
                virtual Vector2 CalculateSize(const Vector2& NewSize, const Vector2& OldSize, const Vector2& EleSize, const UI::ResizeableTether& Tether);
            //public:
                /// @brief Class constructor.
                /// @param name The name of the window.
                /// @param Rect The Rect representing the position and size of the window.
                /// @param Parent The parent Screen this window belongs to.
                EnclosedRenderableContainerWidget(ConstString& name, const RenderableRect& Rect, Screen* Parent);
                /// @brief Class destructor.
                virtual ~EnclosedRenderableContainerWidget();
            public:
                ///////////////////////////////////////////////////////////////////////////////
                // Creating AreaRenderables

                /// @copydoc RenderableContainerWidget::CreateRectangle(const RenderableRect& Rect)
                virtual Rectangle* CreateRectangle(const RenderableRect& Rect);
                /// @copydoc RenderableContainerWidget::CreateCaption(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text)
                virtual Caption* CreateCaption(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text);
                /// @copydoc RenderableContainerWidget::CreateCaption(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text)
                virtual Caption* CreateCaption(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text);
                /// @copydoc RenderableContainerWidget::CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text)
                virtual MarkupText* CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text);
                /// @copydoc RenderableContainerWidget::CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text)
                virtual MarkupText* CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text);

                ///////////////////////////////////////////////////////////////////////////////
                // Working with All AreaRenderables

                /// @copydoc RenderableContainerWidget::GetAreaRenderable(ConstString& Name)
                virtual AreaRenderable* GetAreaRenderable(ConstString& Name);
                /// @copydoc RenderableContainerWidget::GetAreaRenderable(const Whole& Index)
                virtual AreaRenderable* GetAreaRenderable(const Whole& Index);
                /// @brief Gets the offset info struct for a AreaRenderable by name.
                /// @param Name The name of the AreaRenderable to get the offset info for.
                /// @return Returns a const reference to the offset info for the AreaRenderable of the specified name.
                virtual OffsetAreaRenderableInfo& GetOffsetAreaRenderableInfo(ConstString& Name);
                /// @brief Gets the offset info struct for a AreaRenderable by index.
                /// @param Index The index of the requested AreaRenderable to get the offset info for.
                /// @return Returns a const reference to the offset info for the AreaRenderable at the specified index.
                virtual OffsetAreaRenderableInfo& GetOffsetAreaRenderableInfo(const Whole& Index);
                /// @copydoc RenderableContainerWidget::GetNumAreaRenderables()
                virtual Whole GetNumAreaRenderables();
                /// @copydoc RenderableContainerWidget::DestroyAreaRenderable(AreaRenderable* ToBeDestroyed)
                virtual void DestroyAreaRenderable(AreaRenderable* ToBeDestroyed);
                /// @copydoc RenderableContainerWidget::DestroyAllAreaRenderables()
                virtual void DestroyAllAreaRenderables();

                ///////////////////////////////////////////////////////////////////////////////
                // Creating Widgets

                /// @copydoc RenderableContainerWidget::CreateButton(ConstString& Name, const RenderableRect& Rect)
                virtual Button* CreateButton(ConstString& Name, const RenderableRect& Rect);
                /// @copydoc RenderableContainerWidget::CreateButton(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, ConstString& Text)
                virtual Button* CreateButton(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, ConstString& Text);
                /// @copydoc RenderableContainerWidget::CreateButton(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, ConstString& Text)
                virtual Button* CreateButton(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, ConstString& Text);
                /// @copydoc RenderableContainerWidget::CreateScrollbar(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& Style)
                virtual Scrollbar* CreateScrollbar(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& Style);
                /// @copydoc RenderableContainerWidget::CreateCheckBox(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String &LabelText)
                virtual CheckBox* CreateCheckBox(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String &LabelText);
                /// @copydoc RenderableContainerWidget::CreateListBox(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& ScrollbarStyle)
                virtual ListBox* CreateListBox(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& ScrollbarStyle);
                /// @copydoc RenderableContainerWidget::CreateSpinner(ConstString& Name, const RenderableRect& Rect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
                virtual Spinner* CreateSpinner(ConstString& Name, const RenderableRect& Rect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight);
                /// @copydoc RenderableContainerWidget::CreateScrolledCellGrid(ConstString& Name, const RenderableRect& Rect, const Real& Thickness, const UI::ScrollbarStyle& Style)
                virtual ScrolledCellGrid* CreateScrolledCellGrid(ConstString& Name, const RenderableRect& Rect, const Real& Thickness, const UI::ScrollbarStyle& Style);
                /// @copydoc RenderableContainerWidget::CreatePagedCellGrid(ConstString& Name, const RenderableRect& Rect, const RenderableRect& SpnRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
                virtual PagedCellGrid* CreatePagedCellGrid(ConstString& Name, const RenderableRect& Rect, const RenderableRect& SpnRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight);
                /// @copydoc RenderableContainerWidget::CreateDropDownList(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const UI::ScrollbarStyle& ScrollStyle)
                virtual DropDownList* CreateDropDownList(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const UI::ScrollbarStyle& ScrollStyle);
                /// @copydoc RenderableContainerWidget::CreateTabSet(ConstString& Name, const RenderableRect& SetRect)
                virtual TabSet* CreateTabSet(ConstString& Name, const RenderableRect& SetRect);

                ///////////////////////////////////////////////////////////////////////////////
                // Working with all Widgets

                /// @copydoc RenderableContainerWidget::GetWidget(ConstString& Name)
                virtual Widget* GetWidget(ConstString& Name);
                /// @copydoc RenderableContainerWidget::GetWidget(const Whole& Index)
                virtual Widget* GetWidget(const Whole& Index);
                /// @brief Gets the OffsetWidgetInfo of an already created widget by name.
                /// @param Name The name of the widget to get the offset info for.
                /// @return Returns a reference to the widget of the specified name.
                virtual OffsetWidgetInfo& GetOffsetWidgetInfo(ConstString& Name);
                /// @brief Gets the OffsetWidgetInfo of an already created widget by index.
                /// @param Index The index of the requested widget to get the offset info for.
                /// @return Returns a reference to the widget at the specified index.
                virtual OffsetWidgetInfo& GetOffsetWidgetInfo(const Whole& Index);
                /// @copydoc RenderableContainerWidget::GetNumWidgets()
                virtual Whole GetNumWidgets();
                /// @copydoc RenderableContainerWidget::DestroyWidget(Widget* ToBeDestroyed)
                virtual void DestroyWidget(Widget* ToBeDestroyed);
                /// @copydoc RenderableContainerWidget::DestroyAllWidgets()
                virtual void DestroyAllWidgets();

                ///////////////////////////////////////////////////////////////////////////////
                // Inherited from Widget

                /// @copydoc Widget::SetPosition(const Vector2& Position)
                virtual void SetPosition(const Vector2& Position);
                /// @copydoc Widget::SetActualPosition(const Vector2& Position)
                virtual void SetActualPosition(const Vector2& Position);
                /// @copydoc Widget::SetSize(const Vector2& Size)
                virtual void SetSize(const Vector2& Size);
                /// @copydoc Widget::SetActualSize(const Vector2& Size)
                virtual void SetActualSize(const Vector2& Size);
                /// @copydoc Widget::UpdateDimensions()
                virtual void UpdateDimensions();

                ///////////////////////////////////////////////////////////////////////////////
                // Fetch Methods

                /// @brief Gets the background object of this widget.
                /// @return Returns a pointer to the rectangle that is the background for this widget.
                virtual Rectangle* GetWidgetBack();
        };//EnclosedRenderableContainerWidget
    }//ui
}//Mezzanine

#endif
