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
#ifndef _uilayer_cpp
#define _uilayer_cpp

#include "uilayer.h"
#include "uimanager.h"
#include "uitextbutton.h"
#include "uirectangle.h"
#include "uicaption.h"
#include "uiwidget.h"
#include "uicheckbox.h"
#include "uimarkuptext.h"
#include "uilistbox.h"
#include "uiscreen.h"
#include "uiscrollbar.h"
#include "uilinelist.h"
#include "uiwindow.h"
#include "uimenu.h"
#include "uispinner.h"
#include "uiscrolledcellgrid.h"
#include "uipagedcellgrid.h"
#include "uidropdownlist.h"
#include "uitabset.h"
#include "uitextureatlas.h"
#include "uivertex.h"
#include "uisprite.h"

#include "graphicsmanager.h"
#include "world.h"

namespace Mezzanine
{
    namespace UI
    {
        Layer::Layer(const String& name, const Whole& Index, Screen* PScreen)
            : Name(name),
              Index(Index),
              AlphaModifier(1.0),
              Visible(true),
              Parent(PScreen)
        {
            Manager = UIManager::GetSingletonPtr();
        }

        Layer::~Layer()
        {
            for( Whole X = 0 ; X < Buttons.size() ; ++X )
                delete Buttons[X];
            Buttons.clear();
            for( Whole X = 0 ; X < Rectangles.size() ; ++X )
                delete Rectangles[X];
            Rectangles.clear();
            for( Whole X = 0 ; X < Captions.size() ; ++X )
                delete Captions[X];
            Captions.clear();
            for( Whole X = 0 ; X < MarkupTexts.size() ; ++X )
                delete MarkupTexts[X];
            MarkupTexts.clear();
            for( Whole X = 0 ; X < LineLists.size() ; ++X )
                delete LineLists[X];
            LineLists.clear();
            for( Whole X = 0 ; X < Widgets.size() ; ++X )
                delete Widgets[X];
            Widgets.clear();
        }

        const String& Layer::GetName() const
        {
            return Name;
        }

        Screen* Layer::GetParent() const
        {
            return Parent;
        }

        const Whole& Layer::GetIndex() const
        {
            return Index;
        }

        void Layer::SetAlphaModifier(const Real& Modifier)
        {
            AlphaModifier = Modifier;
        }

        Real Layer::GetAlphaModifier() const
        {
            return AlphaModifier;
        }

        void Layer::SetVisible(bool Visible)
        {
            if(this->Visible == Visible)
                return;
            this->Visible = Visible;
            _MarkDirty();
        }

        bool Layer::GetVisible() const
        {
            return Visible;
        }

        bool Layer::IsVisible() const
        {
            return Visible && Parent->IsVisible();
        }

        void Layer::Show()
        {
            if(this->Visible)
                return;
            this->Visible = true;
            _MarkDirty();
        }

        void Layer::Hide()
        {
            if(!this->Visible)
                return;
            this->Visible = false;
            _MarkDirty();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Atlas Info Functions
        ///////////////////////////////////////

        Vector2 Layer::GetSolidUV(const String& Atlas) const
        {
            return GetAtlas(Atlas)->GetWhitePixel();
        }

        Sprite* Layer::GetSprite(const String& SpriteName,const String& Atlas) const
        {
            return GetAtlas(Atlas)->GetSprite(SpriteName);
        }

        GlyphData* Layer::GetGlyphData(const Whole& ID,const String& Atlas) const
        {
            return GetAtlas(Atlas)->GetGlyphData(ID);
        }

        Vector2 Layer::GetTextureSize(const String& Atlas) const
        {
            return GetAtlas(Atlas)->GetTextureSize();
        }

        TextureAtlas* Layer::GetAtlas(const String& Atlas) const
        {
            return Manager->GetAtlas(Atlas);
        }

        Real Layer::GetTexelX() const
        {
            return Parent->GetTexelOffsetX();
        }

        Real Layer::GetTexelY() const
        {
            return Parent->GetTexelOffsetY();
        }

        ColourValue Layer::GetMarkupColour(const Whole& Index,const String& Atlas) const
        {
            return GetAtlas(Atlas)->GetMarkupColour(Index);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Creating and working with All Basic UI Elements
        ///////////////////////////////////////

        Button* Layer::CreateButton(ConstString& Name, const RenderableRect& Rect)
        {
            Button* button = new Button(Name,Rect,this);
            Buttons.push_back(button);
            if(Manager->ButtonAutoRegisterEnabled())
            {
                std::vector<MetaCode::InputCode>* Codes = Manager->GetAutoRegisteredCodes();
                for( Whole X = 0 ; X < Codes->size() ; X++ )
                    button->BindActivationKeyOrButton(Codes->at(X));
            }
            _MarkDirty();
            return button;
        }

        TextButton* Layer::CreateTextButton(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, ConstString& Text)
        {
            TextButton* tbutton = new TextButton(Name,Rect,Glyph,Text,this);
            Buttons.push_back(tbutton);
            if(Manager->ButtonAutoRegisterEnabled())
            {
                std::vector<MetaCode::InputCode>* Codes = Manager->GetAutoRegisteredCodes();
                for( Whole X = 0 ; X < Codes->size() ; X++ )
                    tbutton->BindActivationKeyOrButton(Codes->at(X));
            }
            _MarkDirty();
            return tbutton;
        }

        TextButton* Layer::CreateTextButton(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, ConstString& Text)
        {
            TextButton* tbutton = new TextButton(Name,Rect,LineHeight,Text,this);
            Buttons.push_back(tbutton);
            if(Manager->ButtonAutoRegisterEnabled())
            {
                std::vector<MetaCode::InputCode>* Codes = Manager->GetAutoRegisteredCodes();
                for( Whole X = 0 ; X < Codes->size() ; X++ )
                    tbutton->BindActivationKeyOrButton(Codes->at(X));
            }
            _MarkDirty();
            return tbutton;
        }

        Button* Layer::GetButton(ConstString& Name)
        {
            for ( std::vector<Button*>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                if ( Name == (*it)->GetName() )
                {
                    Button* button = (*it);
                    return button;
                }
            }
            return 0;
        }

        Button* Layer::GetButton(const Whole& Index)
        {
            return Buttons[Index];
        }

        Whole Layer::GetNumButtons()
        {
            return Buttons.size();
        }

        void Layer::DestroyButton(Button* ToBeDestroyed)
        {
            for ( std::vector<Button*>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it) )
                {
                    delete ToBeDestroyed;
                    Buttons.erase(it);
                    return;
                }
            }
        }

        Rectangle* Layer::CreateRectangle(const RenderableRect& Rect)
        {
            Rectangle* rectangle = new Rectangle(Rect, this);
            Rectangles.push_back(rectangle);
            _MarkDirty();
            return rectangle;
        }

        Rectangle* Layer::GetRectangle(const Whole& Index)
        {
            return Rectangles[Index];
        }

        Whole Layer::GetNumRectangles()
        {
            return Rectangles.size();
        }

        void Layer::DestroyRectangle(Rectangle* ToBeDestroyed)
        {
            for ( std::vector<Rectangle*>::iterator it = Rectangles.begin() ; it != Rectangles.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it) )
                {
                    delete ToBeDestroyed;
                    Rectangles.erase(it);
                    return;
                }
            }
        }

        Caption* Layer::CreateCaption(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text)
        {
            Caption* caption = new Caption(Name,Rect,Glyph,Text,this);
            Captions.push_back(caption);
            _MarkDirty();
            return caption;
        }

        Caption* Layer::CreateCaption(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text)
        {
            Caption* caption = new Caption(Name,Rect,LineHeight,Text,this);
            Captions.push_back(caption);
            _MarkDirty();
            return caption;
        }

        Caption* Layer::GetCaption(ConstString& Name)
        {
            for ( std::vector<Caption*>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
            {
                if ( Name == (*it)->GetName() )
                {
                    Caption* caption = (*it);
                    return caption;
                }
            }
            return 0;
        }

        Caption* Layer::GetCaption(const Whole& Index)
        {
            return Captions[Index];
        }

        Whole Layer::GetNumCaptions()
        {
            return Captions.size();
        }

        void Layer::DestroyCaption(Caption* ToBeDestroyed)
        {
            for ( std::vector<Caption*>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it) )
                {
                    delete ToBeDestroyed;
                    Captions.erase(it);
                    return;
                }
            }
        }

        MarkupText* Layer::CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text)
        {
            MarkupText* markup = new MarkupText(Name,Rect,Glyph,Text,this);
            MarkupTexts.push_back(markup);
            _MarkDirty();
            return markup;
        }

        MarkupText* Layer::CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text)
        {
            MarkupText* markup = new MarkupText(Name,Rect,LineHeight,Text,this);
            MarkupTexts.push_back(markup);
            _MarkDirty();
            return markup;
        }

        MarkupText* Layer::GetMarkupText(ConstString& Name)
        {
            for ( std::vector<MarkupText*>::iterator it = MarkupTexts.begin() ; it != MarkupTexts.end() ; it++ )
            {
                if ( Name == (*it)->GetName() )
                {
                    MarkupText* markup = (*it);
                    return markup;
                }
            }
            return 0;
        }

        MarkupText* Layer::GetMarkupText(const Whole& Index)
        {
            return MarkupTexts[Index];
        }

        Whole Layer::GetNumMarkupTexts()
        {
            return MarkupTexts.size();
        }

        void Layer::DestroyMarkupText(MarkupText* ToBeDestroyed)
        {
            for ( std::vector<MarkupText*>::iterator it = MarkupTexts.begin() ; it != MarkupTexts.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it) )
                {
                    delete ToBeDestroyed;
                    MarkupTexts.erase(it);
                    return;
                }
            }
        }

        LineList* Layer::CreateLineList(const String& Name)
        {
            LineList* linelist = new LineList(Name,this);
            LineLists.push_back(linelist);
            _MarkDirty();
            return linelist;
        }

        LineList* Layer::GetLineList(const Whole& Index)
        {
            return LineLists[Index];
        }

        Whole Layer::GetNumLineLists()
        {
            return LineLists.size();
        }

        void Layer::DestroyLineList(LineList* ToBeDestroyed)
        {
            for ( std::vector<LineList*>::iterator it = LineLists.begin() ; it != LineLists.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it) )
                {
                    delete ToBeDestroyed;
                    LineLists.erase(it);
                    return;
                }
            }
        }

        void Layer::AddWidget(Widget* ToAdd)
        {
            Widgets.push_back(ToAdd);
            _MarkDirty();
        }

        Widget* Layer::GetWidget(ConstString& Name)
        {
            for ( std::vector<Widget*>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                if ( Name == (*it)->GetName() )
                {
                    Widget* widget = (*it);
                    return widget;
                }
            }
            return 0;
        }

        Widget* Layer::GetWidget(Whole Index)
        {
            return Widgets[Index];
        }

        Whole Layer::GetNumWidgets()
        {
            return Widgets.size();
        }

        void Layer::DestroyWidget(Widget* ToBeDestroyed)
        {
            for ( std::vector<Widget*>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it) )
                {
                    delete ToBeDestroyed;
                    Widgets.erase(it);
                    return;
                }
            }
        }

        Scrollbar* Layer::CreateScrollbar(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& Style)
        {
            Scrollbar* Scroll = new Scrollbar(Name,Rect,Style,this);
            Widgets.push_back(Scroll);
            _MarkDirty();
            return Scroll;
        }

        CheckBox* Layer::CreateCheckBox(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String &LabelText)
        {
            CheckBox* Check = new CheckBox(Name,Rect,LineHeight,LabelText,this);
            Widgets.push_back(Check);
            _MarkDirty();
            return Check;
        }

        ListBox* Layer::CreateListBox(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& ScrollStyle)
        {
            ListBox* LB = new ListBox(Name,Rect,ScrollStyle,this);
            Widgets.push_back(LB);
            _MarkDirty();
            return LB;
        }

        Window* Layer::CreateWidgetWindow(ConstString& Name, const RenderableRect& Rect)
        {
            Window* Win = new Window(Name,Rect,this);
            Widgets.push_back(Win);
            _MarkDirty();
            return Win;
        }

        Menu* Layer::CreateMenu(ConstString& Name, const RenderableRect& Rect)
        {
            Menu* Men = new Menu(Name,Rect,this);
            Widgets.push_back(Men);
            _MarkDirty();
            return Men;
        }

        Spinner* Layer::CreateSpinner(ConstString& Name, const RenderableRect& Rect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
        {
            Spinner* Spn = new Spinner(Name,Rect,SStyle,GlyphHeight,this);
            Widgets.push_back(Spn);
            _MarkDirty();
            return Spn;
        }

        ScrolledCellGrid* Layer::CreateScrolledCellGrid(ConstString& Name, const RenderableRect& Rect, const Real& Thickness, const UI::ScrollbarStyle& Style)
        {
            ScrolledCellGrid* SCG = new ScrolledCellGrid(Name,Rect,Thickness,Style,this);
            Widgets.push_back(SCG);
            _MarkDirty();
            return SCG;
        }

        PagedCellGrid* Layer::CreatePagedCellGrid(ConstString& Name, const RenderableRect& Rect, const RenderableRect& SpnRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
        {
            PagedCellGrid* PCG = new PagedCellGrid(Name,Rect,SpnRect,SStyle,GlyphHeight,this);
            Widgets.push_back(PCG);
            _MarkDirty();
            return PCG;
        }

        DropDownList* Layer::CreateDropDownList(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const UI::ScrollbarStyle& ScrollStyle)
        {
            DropDownList* DDL = new DropDownList(Name,Rect,LineHeight,ScrollStyle,this);
            Widgets.push_back(DDL);
            _MarkDirty();
            return DDL;
        }

        TabSet* Layer::CreateTabSet(ConstString& Name, const RenderableRect& SetRect)
        {
            TabSet* TS = new TabSet(Name,SetRect,this);
            Widgets.push_back(TS);
            _MarkDirty();
            return TS;
        }

        Button* Layer::CheckButtonMouseIsOver()
        {
            if(Buttons.empty())
                return 0;
            if(!IsVisible())
                return 0;
            Button* button = NULL;
            for( std::vector<Button*>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                button = (*it);
                if(button->IsVisible())
                {
                    if(button->CheckMouseHover())
                    {
                        return button;
                    }
                }
            }
            return 0;
        }

        Widget* Layer::CheckWidgetMouseIsOver()
        {
            if(Widgets.empty())
                return 0;
            if(!IsVisible())
                return 0;
            Widget* widget = NULL;
            for( std::vector<Widget*>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                widget = (*it);
                if(widget->IsVisible())
                {
                    if(widget->CheckMouseHover())
                    {
                        return widget;
                    }
                }
            }
            return 0;
        }

        void Layer::ViewportUpdate()
        {
            for ( std::vector<Button*>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                (*it)->UpdateDimensions();
            }
            for ( std::vector<Rectangle*>::iterator it = Rectangles.begin() ; it != Rectangles.end() ; it++ )
            {
                (*it)->UpdateDimensions();
            }
            for ( std::vector<Caption*>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
            {
                (*it)->UpdateDimensions();
            }
            for ( std::vector<MarkupText*>::iterator it = MarkupTexts.begin() ; it != MarkupTexts.end() ; it++ )
            {
                (*it)->UpdateDimensions();
            }
            //for ( std::vector<LineList*>::iterator it = LineLists.begin() ; it != LineLists.end() ; it++ )
            //{
            //    (*it)->
            //}
            for( std::vector<Widget*>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                (*it)->UpdateDimensions();
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Functions
        ///////////////////////////////////////

        void Layer::_MarkDirty()
        {
            Parent->_RequestIndexRedraw(Index);
        }

        void Layer::_Render(std::vector<VertexData>& Vertices, bool Force)
        {
            if( AlphaModifier == 0.0 )
                return;

            Whole Begin = Vertices.size();
            Whole I = 0;

            for( int RP = 0 ; RP < 3 ; RP++ )
            {
                // Render/redraw rectangles
                for( RectangleIterator it = Rectangles.begin() ; it != Rectangles.end() ; it++ )
                {
                    if( RP == (*it)->GetRenderPriority() )
                    {
                        if( (*it)->Dirty || Force )
                            (*it)->_Redraw();
                        (*it)->_AppendVertices(Vertices);
                    }
                }

                // Render/redraw buttons
                for( ButtonIterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
                {
                    if( RP == (*it)->GetRenderPriority() )
                    {
                        if( (*it)->Dirty || Force )
                            (*it)->_Redraw();
                        (*it)->_AppendVertices(Vertices);
                    }
                }

                // Render/redraw polygons
                /*for( PolygonIterator it = Polygons.begin() ; it != Polygons.end() ; it++ )
                {
                    if( RP == (*it)->GetRenderPriority() )
                    {
                        if( (*it)->Dirty || Force )
                            (*it)->_Redraw();
                        (*it)->_AppendVertices(Vertices);
                    }
                }// */

                // Render/redraw quad lists
                /*for( QuadListIterator it = QuadLists.begin() ; it != QuadLists.end() ; it++ )
                {
                    if( RP == (*it)->GetRenderPriority() )
                    {
                        if( (*it)->Dirty || Force )
                            (*it)->_Redraw();
                        (*it)->_AppendVertices(Vertices);
                    }
                }// */

                // Render/redraw caption
                for(CaptionIterator it = Captions.begin(); it != Captions.end(); it++)
                {
                    if( RP == (*it)->GetRenderPriority() )
                    {
                        if( (*it)->Dirty || Force )
                            (*it)->_Redraw();
                        (*it)->_AppendVertices(Vertices);
                    }
                }

                // Render/redraw markuptext
                for(MarkupTextIterator it = MarkupTexts.begin(); it != MarkupTexts.end(); it++)
                {
                    if( RP == (*it)->GetRenderPriority() )
                    {
                        if( (*it)->TextDirty || Force )
                            (*it)->_CalculateCharacters();
                        if( (*it)->Dirty || Force )
                            (*it)->_Redraw();
                        (*it)->_AppendVertices(Vertices);
                    }
                }

                // Render/redraw Widgets
                for( WidgetIterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
                {
                    if( RP == (*it)->GetRenderPriority() )
                    {
                        if( (*it)->Dirty || Force )
                            (*it)->_Redraw();
                        (*it)->_AppendVertices(Vertices);
                    }
                }

                // Render/redraw line lists
                for( LineListIterator it = LineLists.begin() ; it != LineLists.end() ; it++ )
                {
                    if( RP == (*it)->GetRenderPriority() )
                    {
                        if( (*it)->Dirty || Force )
                            (*it)->_Redraw();
                        (*it)->_AppendVertices(Vertices);
                    }
                }
            }
            if(AlphaModifier != 1.0)
            {
                for( I = Begin ; I < Vertices.size() ; I++ )
                    Vertices[I].Vert.Colour.A *= AlphaModifier;
            }
            Parent->_Transform(Vertices,Begin,Vertices.size());
        }
    }//ui
}//Mezzanine

#endif
