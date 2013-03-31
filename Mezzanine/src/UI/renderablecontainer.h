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
#ifndef _uirenderablecontainer_h
#define _uirenderablecontainer_h

#include "colourvalue.h"
#include "UI/resizehandler.h"
#include "UI/renderablerect.h"
#include "UI/uienumerations.h"

namespace Mezzanine
{
    class UIManager;
    namespace UI
    {
        class Widget;
        class BasicRenderable;
        class Rectangle;
        class Caption;
        class Button;
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
        class OpenRenderableContainerWidget;
        class EnclosedRenderableContainerWidget;
        class Screen;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class RenderableFactory
        /// @headerfile renderablecontainer.h
        /// @brief This class is responsible for the creation and destruction of all UI renderables.
        /// @details This class only contains a basic set of renderables.  For more renderables see the ExtendedRenderableFactory class.
        ///////////////////////////////////////
        class MEZZ_LIB RenderableFactory
        {
            protected:
                UIManager* Manager;
                /// @internal
                /// @brief Sets all the activation keys for the provided button after it is created.
                void ButtonActivationSetup(Button* ToSet);
                /// @internal
                /// @brief Gets the screen that will possess ownership of the created renderables.
                virtual Screen* GetScreen() = 0;
            public:
                /// @brief Class constructor.
                RenderableFactory();
                /// @brief Class destructor.
                virtual ~RenderableFactory();

                ///////////////////////////////////////////////////////////////////////////////
                // Creating BasicRenderables

                /// @brief Creates a rectangle.
                /// @return Returns a pointer to the created rectangle.
                /// @param Rect The Rect representing the position and size of the rectangle.
                virtual Rectangle* CreateRectangle(const RenderableRect& Rect);
                /// @brief Creates a caption.
                /// @return Returns a pointer to the created caption.
                /// @param Name The name of this caption.
                /// @param Rect The Rect representing the position and size of the caption.
                /// @param Glyph One of the glyphs specified in your mta file.  Must be valid.
                /// @param Text Any text you want printed on the caption.
                virtual Caption* CreateCaption(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text);
                /// @brief Creates a caption.
                /// @return Returns a pointer to the created caption.
                /// @param Name The name of this caption.
                /// @param Rect The Rect representing the position and size of the caption.
                /// @param LineHeight The lineheight you want the text to have in relative units.  This will automatically select the glyph and scale it for you.
                /// @param Text Any text you want printed on the caption.
                virtual Caption* CreateCaption(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text);
                /// @brief Creates a markup text.
                /// @details The constructor will ignore the size portion of the Rect passed in, since Markup Texts don't have a default size.
                /// @return Returns a pointer to the created markup text.
                /// @param Name The name of this markup text.
                /// @param Rect The Rect representing the position and size of the markup text.
                /// @param Glyph One of the glyphs specified in your mta file.  Must be valid.
                /// @param Text Any text you want printed on the markup text.
                virtual MarkupText* CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text);
                /// @brief Creates a markup text.
                /// @details The constructor will ignore the size portion of the Rect passed in, since Markup Texts don't have a default size.
                /// @return Returns a pointer to the created markup text.
                /// @param Name The name of this markup text.
                /// @param Rect The Rect representing the position and size of the markup text.
                /// @param LineHeight The lineheight you want the text to have in relative units.  This will automatically select the glyph and scale it for you.
                /// @param Text Any text you want printed on the markup text.
                virtual MarkupText* CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text);

                ///////////////////////////////////////////////////////////////////////////////
                // Destroying BasicRenderables

                /// @brief Destroys a BasicRenderable.
                /// @param ToBeDestroyed Pointer to the BasicRenderable you want destroyed.
                virtual void DestroyBasicRenderable(BasicRenderable* ToBeDestroyed);

                ///////////////////////////////////////////////////////////////////////////////
                // Creating Widgets

                /// @brief Creates a button.
                /// @return Returns a pointer to the created button.
                /// @param Name The name of the button.
                /// @param Rect The Rect representing the position and size of the button.
                virtual Button* CreateButton(ConstString& Name, const RenderableRect& Rect);
                /// @brief Creates a text button.
                /// @return Returns a pointer to the created button.
                /// @param Name The name of the button.
                /// @param Rect The Rect representing the position and size of the button.
                /// @param Glyph One of the glyphs specified in your mta file.  Must be valid.
                /// @param Text Any text you want printed on the button.
                virtual Button* CreateButton(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, ConstString& Text);
                /// @brief Creates a text button.
                /// @return Returns a pointer to the created button.
                /// @param Name The name of the button.
                /// @param Rect The Rect representing the position and size of the button.
                /// @param LineHeight The lineheight you want the text to have in relative units.  This will automatically select the glyph and scale it for you.
                /// @param Text Any text you want printed on the button.
                virtual Button* CreateButton(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, ConstString& Text);
                /// @brief Creates a Scrollbar.
                /// @return Returns a pointer to the created Scrollbar.
                /// @param Name The name of the Scrollbar.
                /// @param Rect The Rect representing the position and size of the Scrollbar.
                /// @param Style The style of scrollbar you want to create, see Scrollbar documentation for more details.
                virtual Scrollbar* CreateScrollbar(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& Style);
                /// @brief Creates a CheckBox.
                /// @details The label uses the Markup Text class, and thus it's light markup text language.  You can also
                /// pass in a blank string if you don't wish to have a label, you can create a label after construction.
                /// @return Returns a pointer to the created CheckBox.
                /// @param Name The name of the CheckBox.
                /// @param Rect The Rect representing the position and size of the CheckBox.
                /// @param LineHeight The lineheight you want the text to have. If the Rect passed in is relative, this will expect LineHeight to be relative as well.
                /// @param LabelText The text to display with the label.
                virtual CheckBox* CreateCheckBox(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String &LabelText);
                /// @brief Creates a List Box.
                /// @return Returns a pointer to the created List Box.
                /// @param Name The name of the List Box.
                /// @param Rect The Rect representing the position and size of the List Box.
                /// @param ScrollStyle The style of scrollbar you want to create, see Scrollbar documentation for more details.
                virtual ListBox* CreateListBox(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& ScrollStyle);
                /// @brief Creates a Spinner.
                /// @return Returns a pointer to the created Spinner.
                /// @param Name The Name for the Widget.
                /// @param Rect The Rect representing the position and size of the Spinner.
                /// @param SStyle The layout of buttons this widget will have.  See SpinnerStyle enum or class description for more details.
                /// @param GlyphHeight The desired relative height of the text you want.
                virtual Spinner* CreateSpinner(ConstString& Name, const RenderableRect& Rect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight);
                /// @brief Creates a scrolled cell grid.
                /// @return Returns a pointer to the created ScrolledCellGrid.
                /// @param Name The name of the widget.
                /// @param Rect The Rect representing the position and size of the ScrolledCellGrid.
                /// @param Thickness The width of the vertical scrollbar in relative units.  The same amount of actual pixels is used
                /// to determine the height of the horizontal scrollbar.
                /// @param Style An enum value representing how you want your scrollbar constructed.  See class details for more info.
                virtual ScrolledCellGrid* CreateScrolledCellGrid(ConstString& Name, const RenderableRect& Rect, const Real& Thickness, const UI::ScrollbarStyle& Style);
                /// @brief Creates a paged cell grid.
                /// @return Returns a pointer to the created PagedCellGrid.
                /// @param Name The name of the widget.
                /// @param Rect The Rect representing the position and size of the PagedCellGrid.
                /// @param SpnRect The Rect representing the position and size of the Spinner.
                /// @param SStyle The style of spinner to create.
                /// @param GlyphHeight The desired lineheight of the glyphs to be used with the spinner.
                virtual PagedCellGrid* CreatePagedCellGrid(ConstString& Name, const RenderableRect& Rect, const RenderableRect& SpnRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight);
                /// @brief Creates a drop down list.
                /// @return Returns a pointer to the created DropDownList.
                /// @param Name The Name for the Widget.
                /// @param Rect The renderable rect representing the position and size of this widget.
                /// @param LineHeight The lineheight you want the text to have. If the Rect passed in is relative, this will expect LineHeight to be relative as well.
                /// @param ScrollStyle The style of the scrollbar you want for this List Box.  See Scrollbar class for more information.
                virtual DropDownList* CreateDropDownList(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const UI::ScrollbarStyle& ScrollStyle);
                /// @brief Creates a tabset.
                /// @return Returns a pointer to the created tabset.
                /// @param Name The Name for the Widget.
                /// @param SetRect The Rect representing the position and size of all the Renderable Sets generated by the tabset.
                virtual TabSet* CreateTabSet(ConstString& Name, const RenderableRect& SetRect);
                /// @brief Creates an EnclosedRenderableContainerWidget.
                /// @return Returns a pointer to the created EnclosedRenderableContainerWidget.
                /// @param Name The name of the window.
                /// @param Rect The Rect representing the position and size of the window.
                virtual EnclosedRenderableContainerWidget* CreateEnclosedRenderableContainerWidget(ConstString& Name, const RenderableRect& Rect);

                ///////////////////////////////////////////////////////////////////////////////
                // Destroying Widgets

                /// @brief Destroys a widget.
                /// @param ToBeDestroyed Pointer to the widget you want destroyed.
                virtual void DestroyWidget(Widget* ToBeDestroyed);
        };//RenderableFactory

        ///////////////////////////////////////////////////////////////////////////////
        /// @class ExtendedRenderableFactory
        /// @headerfile renderablecontainer.h
        /// @brief This class is responsible for the creation and destruction of all UI renderables.
        /// @details This class contains all the renderables of the standard RenderableFactory, plus a few extra exotic renderables.
        ///////////////////////////////////////
        class MEZZ_LIB ExtendedRenderableFactory : public RenderableFactory
        {
            public:
                /// @brief Class constructor.
                ExtendedRenderableFactory();
                /// @brief Class destructor.
                virtual ~ExtendedRenderableFactory();

                ///////////////////////////////////////////////////////////////////////////////
                // Creating BasicRenderables

                /// @brief Creates a linelist.
                /// @return Returns a pointer to the created linelist.
                /// @param Name The name to give to this Linelist.
                virtual LineList* CreateLineList(ConstString& Name);

                ///////////////////////////////////////////////////////////////////////////////
                // Creating Widgets

                /// @brief Creates a Window.
                /// @return Returns a pointer to the created Window.
                /// @param Name The name of the Window.
                /// @param Rect The Rect representing the position and size of the Window.
                virtual Window* CreateWidgetWindow(ConstString& Name, const RenderableRect& Rect);
                /// @brief Creates a Menu.
                /// @return Returns a pointer to the created Menu.
                /// @param Name The name of the Menu.
                /// @param Rect The Rect representing the position and size of the Menu.
                virtual Menu* CreateMenu(ConstString& Name, const RenderableRect& Rect);
                /// @brief Creates an OpenRenderableContainerWidget.
                /// @return Returns a pointer to the creates OpenRenderableContainerWidget.
                /// @param Name The name of the OpenRenderableContainerWidget.
                virtual OpenRenderableContainerWidget* CreateOpenRenderableContainerWidget(ConstString& Name);
        };//ExtendedRenderableFactory

        ///////////////////////////////////////////////////////////////////////////////
        /// @class RenderableContainer
        /// @headerfile renderablecontainer.h
        /// @brief
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB RenderableContainer : public ExtendedRenderableFactory
        {
            public:
                typedef std::vector<BasicRenderable*>             BasicRenderableContainer;
                typedef BasicRenderableContainer::iterator        BasicRenderableIterator;
                typedef BasicRenderableContainer::const_iterator  ConstBasicRenderableIterator;
                typedef std::vector<Widget*>                      WidgetContainer;
                typedef WidgetContainer::iterator                 WidgetIterator;
                typedef WidgetContainer::const_iterator           ConstWidgetIterator;
            protected:
                ResizeHandler* Resize;
                BasicRenderableContainer BasicRenderables;
                WidgetContainer Widgets;
                /// @copydoc RenderableFactory::GetScreen()
                virtual Screen* GetScreen() = 0;
            public:
                /// @brief Class constructor.
                RenderableContainer();
                /// @brief Class destructor.
                virtual ~RenderableContainer();

                ///////////////////////////////////////////////////////////////////////////////
                // Creating BasicRenderables

                /// @copydoc RenderableFactory::CreateRectangle(const RenderableRect& Rect)
                virtual Rectangle* CreateRectangle(const RenderableRect& Rect);
                /// @copydoc RenderableFactory::CreateCaption(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text)
                virtual Caption* CreateCaption(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text);
                /// @copydoc RenderableFactory::CreateCaption(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text)
                virtual Caption* CreateCaption(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text);
                /// @copydoc RenderableFactory::CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text)
                virtual MarkupText* CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text);
                /// @copydoc RenderableFactory::CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text)
                virtual MarkupText* CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text);
                /// @copydoc ExtendedRenderableFactory::CreateLineList(ConstString& Name)
                virtual LineList* CreateLineList(ConstString& Name);

                ///////////////////////////////////////////////////////////////////////////////
                // Working with all BasicRenderables

                /// @brief Adds a BasicRenderable to this container.
                /// @details This function is necessary when creating custom widgets in game logic.
                /// @param ToAdd The BasicRenderable to be added.
                virtual void AddBasicRenderable(BasicRenderable* ToAdd);
                /// @brief Gets an already created BasicRenderable by name.
                /// @return Returns a pointer to the BasicRenderable of the specified name.
                virtual BasicRenderable* GetBasicRenderable(ConstString& Name);
                /// @brief Gets an already created BasicRenderable by index.
                /// @return Returns a pointer to the BasicRenderable at the specified index.
                virtual BasicRenderable* GetBasicRenderable(const Whole& Index);
                /// @brief Gets the number of widgets created and stored in this class.
                /// @return Returns the number of widgets this class is storing.
                virtual Whole GetNumBasicRenderables();
                /// @copydoc RenderableFactory::DestroyBasicRenderable(BasicRenderable* ToBeDestroyed)
                virtual void DestroyBasicRenderable(BasicRenderable* ToBeDestroyed);
                /// @brief Destroys all BasicRenderables being stored in this container.
                virtual void DestroyAllBasicRenderables();

                ///////////////////////////////////////////////////////////////////////////////
                // Creating Widgets

                /// @copydoc RenderableFactory::CreateButton(ConstString& Name, const RenderableRect& Rect)
                virtual Button* CreateButton(ConstString& Name, const RenderableRect& Rect);
                /// @copydoc RenderableFactory::CreateButton(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, ConstString& Text)
                virtual Button* CreateButton(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, ConstString& Text);
                /// @copydoc RenderableFactory::CreateButton(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, ConstString& Text)
                virtual Button* CreateButton(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, ConstString& Text);
                /// @copydoc RenderableFactory::CreateScrollbar(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& Style)
                virtual Scrollbar* CreateScrollbar(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& Style);
                /// @copydoc RenderableFactory::CreateCheckBox(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String &LabelText)
                virtual CheckBox* CreateCheckBox(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& LabelText);
                /// @copydoc RenderableFactory::CreateListBox(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& ScrollbarStyle)
                virtual ListBox* CreateListBox(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& ScrollStyle);
                /// @copydoc RenderableFactory::CreateSpinner(ConstString& Name, const RenderableRect& Rect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
                virtual Spinner* CreateSpinner(ConstString& Name, const RenderableRect& Rect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight);
                /// @copydoc RenderableFactory::CreateScrolledCellGrid(ConstString& Name, const RenderableRect& Rect, const Real& Thickness, const UI::ScrollbarStyle& Style)
                virtual ScrolledCellGrid* CreateScrolledCellGrid(ConstString& Name, const RenderableRect& Rect, const Real& Thickness, const UI::ScrollbarStyle& Style);
                /// @copydoc RenderableFactory::CreatePagedCellGrid(ConstString& Name, const RenderableRect& Rect, const RenderableRect& SpnRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
                virtual PagedCellGrid* CreatePagedCellGrid(ConstString& Name, const RenderableRect& Rect, const RenderableRect& SpnRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight);
                /// @copydoc RenderableFactory::CreateDropDownList(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const UI::ScrollbarStyle& ScrollStyle)
                virtual DropDownList* CreateDropDownList(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const UI::ScrollbarStyle& ScrollStyle);
                /// @copydoc RenderableFactory::CreateTabSet(ConstString& Name, const RenderableRect& SetRect)
                virtual TabSet* CreateTabSet(ConstString& Name, const RenderableRect& SetRect);
                /// @copydoc RenderableFactory::CreateEnclosedRenderableContainerWidget(ConstString& Name, const RenderableRect& Rect)
                virtual EnclosedRenderableContainerWidget* CreateEnclosedRenderableContainerWidget(ConstString& Name, const RenderableRect& Rect);
                /// @copydoc ExtendedRenderableFactory::CreateWidgetWindow(ConstString& Name, const RenderableRect& Rect)
                virtual Window* CreateWidgetWindow(ConstString& Name, const RenderableRect& Rect);
                /// @copydoc ExtendedRenderableFactory::CreateMenu(ConstString& Name, const RenderableRect& Rect)
                virtual Menu* CreateMenu(ConstString& Name, const RenderableRect& Rect);
                /// @copydoc ExtendedRenderableFactory::CreateOpenRenderableContainerWidget(ConstString& Name)
                virtual OpenRenderableContainerWidget* CreateOpenRenderableContainerWidget(ConstString& Name);

                ///////////////////////////////////////////////////////////////////////////////
                // Working with all Widgets

                /// @brief Adds a widget to this layer.
                /// @details This function is necessary when creating custom widgets in game logic.
                /// @param ToAdd The widget to be added.
                virtual void AddWidget(Widget* ToAdd);
                /// @brief Gets an already created widget by name.
                /// @return Returns a pointer to the widget of the specified name.
                virtual Widget* GetWidget(ConstString& Name);
                /// @brief Gets an already created widget by index.
                /// @return Returns a pointer to the widget at the specified index.
                virtual Widget* GetWidget(const Whole& Index);
                /// @brief Gets the number of widgets created and stored in this class.
                /// @return Returns the number of widgets this class is storing.
                virtual Whole GetNumWidgets();
                /// @copydoc RenderableFactory::DestroyWidget(Widget* ToBeDestroyed)
                virtual void DestroyWidget(Widget* ToBeDestroyed);
                /// @brief Destroys all widgets being stored in this container.
                virtual void DestroyAllWidgets();
        };//RenderableContainer
    }//UI
}//Mezzanine

#endif
