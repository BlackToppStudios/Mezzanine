//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _uirenderablecontainer_cpp
#define _uirenderablecontainer_cpp

#include "uimanager.h"
#include "UI/renderablecontainer.h"
#include "UI/button.h"
#include "UI/rectangle.h"
#include "UI/caption.h"
#include "UI/widget.h"
#include "UI/checkbox.h"
#include "UI/markuptext.h"
#include "UI/listbox.h"
#include "UI/screen.h"
#include "UI/scrollbar.h"
#include "UI/linelist.h"
#include "UI/window.h"
#include "UI/menu.h"
#include "UI/spinner.h"
#include "UI/scrolledcellgrid.h"
#include "UI/pagedcellgrid.h"
#include "UI/dropdownlist.h"
#include "UI/tabset.h"
#include "UI/renderablecontainerwidget.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // RenderableFactory Methods

        RenderableFactory::RenderableFactory()
        {
            Manager = UIManager::GetSingletonPtr();
        }

        RenderableFactory::~RenderableFactory()
        {
        }

        void RenderableFactory::ButtonActivationSetup(Button* ToSet)
        {
            if(Manager->ButtonAutoRegisterEnabled())
            {
                std::vector<MetaCode::InputCode>* Codes = Manager->GetAutoRegisteredCodes();
                for( Whole X = 0 ; X < Codes->size() ; X++ )
                    ToSet->BindActivationKeyOrButton(Codes->at(X));
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Creating  Basic UI Elements

        Rectangle* RenderableFactory::CreateRectangle(const RenderableRect& Rect)
        {
            return new Rectangle(Rect,GetScreen());
        }

        Caption* RenderableFactory::CreateCaption(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text)
        {
            return new Caption(Name,Rect,Glyph,Text,GetScreen());
        }

        Caption* RenderableFactory::CreateCaption(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text)
        {
            return new Caption(Name,Rect,LineHeight,Text,GetScreen());
        }

        MarkupText* RenderableFactory::CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text)
        {
            return new MarkupText(Name,Rect,Glyph,Text,GetScreen());
        }

        MarkupText* RenderableFactory::CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text)
        {
            return new MarkupText(Name,Rect,LineHeight,Text,GetScreen());
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Destroying BasicRenderables

        void RenderableFactory::DestroyBasicRenderable(BasicRenderable* ToBeDestroyed)
        {
            delete ToBeDestroyed;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Creating Widgets

        Button* RenderableFactory::CreateButton(ConstString& Name, const RenderableRect& Rect)
        {
            Button* button = new Button(Name,Rect,GetScreen());
            ButtonActivationSetup(button);
            return button;
        }

        Button* RenderableFactory::CreateButton(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, ConstString& Text)
        {
            Button* button = new Button(Name,Rect,Glyph,Text,GetScreen());
            ButtonActivationSetup(button);
            return button;
        }

        Button* RenderableFactory::CreateButton(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, ConstString& Text)
        {
            Button* button = new Button(Name,Rect,LineHeight,Text,GetScreen());
            ButtonActivationSetup(button);
            return button;
        }

        Scrollbar* RenderableFactory::CreateScrollbar(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& Style)
        {
            return new Scrollbar(Name,Rect,Style,GetScreen());
        }

        CheckBox* RenderableFactory::CreateCheckBox(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& LabelText)
        {
            return new CheckBox(Name,Rect,LineHeight,LabelText,GetScreen());
        }

        ListBox* RenderableFactory::CreateListBox(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& ScrollStyle)
        {
            return new ListBox(Name,Rect,ScrollStyle,GetScreen());
        }

        Spinner* RenderableFactory::CreateSpinner(ConstString& Name, const RenderableRect& Rect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
        {
            return new Spinner(Name,Rect,SStyle,GlyphHeight,GetScreen());
        }

        ScrolledCellGrid* RenderableFactory::CreateScrolledCellGrid(ConstString& Name, const RenderableRect& Rect, const Real& Thickness, const UI::ScrollbarStyle& Style)
        {
            return new ScrolledCellGrid(Name,Rect,Thickness,Style,GetScreen());
        }

        PagedCellGrid* RenderableFactory::CreatePagedCellGrid(ConstString& Name, const RenderableRect& Rect, const RenderableRect& SpnRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
        {
            return new PagedCellGrid(Name,Rect,SpnRect,SStyle,GlyphHeight,GetScreen());
        }

        DropDownList* RenderableFactory::CreateDropDownList(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const UI::ScrollbarStyle& ScrollStyle)
        {
            return new DropDownList(Name,Rect,LineHeight,ScrollStyle,GetScreen());
        }

        TabSet* RenderableFactory::CreateTabSet(ConstString& Name, const RenderableRect& SetRect)
        {
            return new TabSet(Name,SetRect,GetScreen());
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Destroying Widgets

        void RenderableFactory::DestroyWidget(Widget* ToBeDestroyed)
        {
            delete ToBeDestroyed;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // ExtendedRenderableFactory Methods

        ExtendedRenderableFactory::ExtendedRenderableFactory()
        {
        }

        ExtendedRenderableFactory::~ExtendedRenderableFactory()
        {
        }

        LineList* ExtendedRenderableFactory::CreateLineList(ConstString& Name)
        {
            return new LineList(Name,GetScreen());
        }

        Window* ExtendedRenderableFactory::CreateWidgetWindow(ConstString& Name, const RenderableRect& Rect)
        {
            return new Window(Name,Rect,GetScreen());
        }

        Menu* ExtendedRenderableFactory::CreateMenu(ConstString& Name, const RenderableRect& Rect)
        {
            return new Menu(Name,Rect,GetScreen());
        }

        OpenRenderableContainerWidget* ExtendedRenderableFactory::CreateOpenRenderableContainerWidget(ConstString& Name)
        {
            return new OpenRenderableContainerWidget(Name,GetScreen());
        }

        ///////////////////////////////////////////////////////////////////////////////
        // RenderableContainer Methods

        RenderableContainer::RenderableContainer()
        {
        }

        RenderableContainer::~RenderableContainer()
        {
            DestroyAllWidgets();
            DestroyAllBasicRenderables();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Creating and working with All Basic UI Elements

        Rectangle* RenderableContainer::CreateRectangle(const RenderableRect& Rect)
        {
            Rectangle* rectangle = RenderableFactory::CreateRectangle(Rect);
            BasicRenderables.push_back(rectangle);
            return rectangle;
        }

        Caption* RenderableContainer::CreateCaption(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text)
        {
            Caption* caption = RenderableFactory::CreateCaption(Name,Rect,Glyph,Text);
            BasicRenderables.push_back(caption);
            return caption;
        }

        Caption* RenderableContainer::CreateCaption(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text)
        {
            Caption* caption = RenderableFactory::CreateCaption(Name,Rect,LineHeight,Text);
            BasicRenderables.push_back(caption);
            return caption;
        }

        MarkupText* RenderableContainer::CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text)
        {
            MarkupText* markup = RenderableFactory::CreateMarkupText(Name,Rect,Glyph,Text);
            BasicRenderables.push_back(markup);
            return markup;
        }

        MarkupText* RenderableContainer::CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text)
        {
            MarkupText* markup = RenderableFactory::CreateMarkupText(Name,Rect,LineHeight,Text);
            BasicRenderables.push_back(markup);
            return markup;
        }

        LineList* RenderableContainer::CreateLineList(ConstString& Name)
        {
            LineList* list = ExtendedRenderableFactory::CreateLineList(Name);
            BasicRenderables.push_back(list);
            return list;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Working with all BasicRenderables

        void RenderableContainer::AddBasicRenderable(BasicRenderable* ToAdd)
        {
            BasicRenderables.push_back(ToAdd);
        }

        BasicRenderable* RenderableContainer::GetBasicRenderable(ConstString& Name)
        {
            for( BasicRenderableIterator it = BasicRenderables.begin() ; it != BasicRenderables.end() ; it++ )
            {
                if( Name == (*it)->GetName() )
                {
                    BasicRenderable* rend = (*it);
                    return rend;
                }
            }
            return NULL;
        }

        BasicRenderable* RenderableContainer::GetBasicRenderable(const Whole& Index)
        {
            return BasicRenderables.at(Index);
        }

        Whole RenderableContainer::GetNumBasicRenderables()
        {
            return BasicRenderables.size();
        }

        void RenderableContainer::DestroyBasicRenderable(BasicRenderable* ToBeDestroyed)
        {
            for( BasicRenderableIterator it = BasicRenderables.begin() ; it != BasicRenderables.end() ; it++ )
            {
                if( ToBeDestroyed == (*it) )
                {
                    RenderableFactory::DestroyBasicRenderable(ToBeDestroyed);
                    BasicRenderables.erase(it);
                    return;
                }
            }
        }

        void RenderableContainer::DestroyAllBasicRenderables()
        {
            for( BasicRenderableIterator it = BasicRenderables.begin() ; it != BasicRenderables.end() ; it++ )
            {
                RenderableFactory::DestroyBasicRenderable( *it );
            }
            BasicRenderables.clear();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Creating Widgets

        Button* RenderableContainer::CreateButton(ConstString& Name, const RenderableRect& Rect)
        {
            Button* button = RenderableFactory::CreateButton(Name,Rect);
            Widgets.push_back(button);
            return button;
        }

        Button* RenderableContainer::CreateButton(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, ConstString& Text)
        {
            Button* button = RenderableFactory::CreateButton(Name,Rect,Glyph,Text);
            Widgets.push_back(button);
            return button;
        }

        Button* RenderableContainer::CreateButton(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, ConstString& Text)
        {
            Button* button = RenderableFactory::CreateButton(Name,Rect,LineHeight,Text);
            Widgets.push_back(button);
            return button;
        }

        Scrollbar* RenderableContainer::CreateScrollbar(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& Style)
        {
            Scrollbar* Scroll = RenderableFactory::CreateScrollbar(Name,Rect,Style);
            Widgets.push_back(Scroll);
            return Scroll;
        }

        CheckBox* RenderableContainer::CreateCheckBox(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String &LabelText)
        {
            CheckBox* Check = RenderableFactory::CreateCheckBox(Name,Rect,LineHeight,LabelText);
            Widgets.push_back(Check);
            return Check;
        }

        ListBox* RenderableContainer::CreateListBox(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& ScrollStyle)
        {
            ListBox* LB = RenderableFactory::CreateListBox(Name,Rect,ScrollStyle);
            Widgets.push_back(LB);
            return LB;
        }

        Spinner* RenderableContainer::CreateSpinner(ConstString& Name, const RenderableRect& Rect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
        {
            Spinner* Spn = RenderableFactory::CreateSpinner(Name,Rect,SStyle,GlyphHeight);
            Widgets.push_back(Spn);
            return Spn;
        }

        ScrolledCellGrid* RenderableContainer::CreateScrolledCellGrid(ConstString& Name, const RenderableRect& Rect, const Real& Thickness, const UI::ScrollbarStyle& Style)
        {
            ScrolledCellGrid* SCG = RenderableFactory::CreateScrolledCellGrid(Name,Rect,Thickness,Style);
            Widgets.push_back(SCG);
            return SCG;
        }

        PagedCellGrid* RenderableContainer::CreatePagedCellGrid(ConstString& Name, const RenderableRect& Rect, const RenderableRect& SpnRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
        {
            PagedCellGrid* PCG = RenderableFactory::CreatePagedCellGrid(Name,Rect,SpnRect,SStyle,GlyphHeight);
            Widgets.push_back(PCG);
            return PCG;
        }

        DropDownList* RenderableContainer::CreateDropDownList(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const UI::ScrollbarStyle& ScrollStyle)
        {
            DropDownList* DDL = RenderableFactory::CreateDropDownList(Name,Rect,LineHeight,ScrollStyle);
            Widgets.push_back(DDL);
            return DDL;
        }

        TabSet* RenderableContainer::CreateTabSet(ConstString& Name, const RenderableRect& SetRect)
        {
            TabSet* TS = RenderableFactory::CreateTabSet(Name,SetRect);
            Widgets.push_back(TS);
            return TS;
        }

        Window* RenderableContainer::CreateWidgetWindow(ConstString& Name, const RenderableRect& Rect)
        {
            Window* Win = ExtendedRenderableFactory::CreateWidgetWindow(Name,Rect);
            Widgets.push_back(Win);
            return Win;
        }

        Menu* RenderableContainer::CreateMenu(ConstString& Name, const RenderableRect& Rect)
        {
            Menu* Men = ExtendedRenderableFactory::CreateMenu(Name,Rect);
            Widgets.push_back(Men);
            return Men;
        }

        OpenRenderableContainerWidget* RenderableContainer::CreateOpenRenderableContainerWidget(ConstString& Name)
        {
            OpenRenderableContainerWidget* ORCW = ExtendedRenderableFactory::CreateOpenRenderableContainerWidget(Name);
            Widgets.push_back(ORCW);
            return ORCW;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Working with all Widgets

        void RenderableContainer::AddWidget(Widget* ToAdd)
        {
            Widgets.push_back(ToAdd);
        }

        Widget* RenderableContainer::GetWidget(ConstString& Name)
        {
            for( WidgetIterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                if( Name == (*it)->GetName() )
                {
                    Widget* widget = (*it);
                    return widget;
                }
            }
            return NULL;
        }

        Widget* RenderableContainer::GetWidget(const Whole& Index)
        {
            return Widgets.at(Index);
        }

        Whole RenderableContainer::GetNumWidgets()
        {
            return Widgets.size();
        }

        void RenderableContainer::DestroyWidget(Widget* ToBeDestroyed)
        {
            for( WidgetIterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                if( ToBeDestroyed == (*it) )
                {
                    RenderableFactory::DestroyWidget(ToBeDestroyed);
                    Widgets.erase(it);
                    return;
                }
            }
        }

        void RenderableContainer::DestroyAllWidgets()
        {
            for( WidgetIterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                RenderableFactory::DestroyWidget( *it );
            }
            Widgets.clear();
        }
    }//UI
}//Mezzanine

#endif
