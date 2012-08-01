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
#ifndef _uimenuwindow_cpp
#define _uimenuwindow_cpp

#include "UI/menuwindow.h"
#include "UI/menu.h"
#include "UI/button.h"
#include "UI/screen.h"

namespace Mezzanine
{
    namespace UI
    {
        MenuWindow::MenuWindow(ConstString& Name, const RenderableRect& Rect, UI::Menu* TheMenu, Screen* PScreen)
            : Window(Name,Rect,PScreen),
              BackButton(NULL),
              ParentWindow(NULL),
              MasterMenu(TheMenu),
              AutoHideWindow(true)
        {
            Type = Widget::W_MenuWindow;
        }

        MenuWindow::~MenuWindow()
        {
            while(!ChildWindows.empty())
            {
                UI::MenuWindow* window = ChildWindows.back().second;
                delete window;
                ChildWindows.pop_back();
            }
        }

        void MenuWindow::SetAutoHide(bool AutoHide)
        {
            AutoHideWindow = AutoHide;
        }

        bool MenuWindow::GetAutoHide()
        {
            return AutoHideWindow;
        }

        MenuWindow* MenuWindow::GetParentWindow()
        {
            return ParentWindow;
        }

        Button* MenuWindow::CreateBackButton(const RenderableRect& Rect)
        {
            if(!BackButton && ParentWindow)
            {
                BackButton = this->CreateButton(Name+"back",Rect);
                return BackButton;
            }
            return 0;
        }

        Button* MenuWindow::CreateBackButton(const RenderableRect& Rect, const Whole& Glyph, const String& Text)
        {
            if(!BackButton && ParentWindow)
            {
                BackButton = this->CreateButton(Name+"back",Rect,Glyph,Text);
                return BackButton;
            }
            return 0;
        }

        Button* MenuWindow::CreateBackButton(const RenderableRect& Rect, const Real& LineHeight, const String& Text)
        {
            if(!BackButton && ParentWindow)
            {
                BackButton = this->CreateButton(Name+"back",Rect,LineHeight,Text);
                return BackButton;
            }
            return 0;
        }

        Button* MenuWindow::GetBackButton()
        {
            return BackButton;
        }

        Button* MenuWindow::CreateAccessorButton(ConstString& Name, const RenderableRect& Rect)
        {
            Button* AccBut = this->CreateButton(Name,Rect);
            ChildWindows.push_back(std::pair<Button*,MenuWindow*>(AccBut,NULL));
            return AccBut;
        }

        Button* MenuWindow::CreateAccessorButton(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text)
        {
            Button* AccBut = this->CreateButton(Name,Rect,Glyph,Text);
            ChildWindows.push_back(std::pair<Button*,MenuWindow*>(AccBut,NULL));
            return AccBut;
        }

        Button* MenuWindow::CreateAccessorButton(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text)
        {
            Button* AccBut = this->CreateButton(Name,Rect,LineHeight,Text);
            ChildWindows.push_back(std::pair<Button*,MenuWindow*>(AccBut,NULL));
            return AccBut;
        }

        Button* MenuWindow::GetAccessorButton(ConstString& Name)
        {
            for( std::vector<std::pair<Button*,MenuWindow*> >::iterator it = ChildWindows.begin() ; it != ChildWindows.end() ; it++ )
            {
                if(Name == (*it).first->GetName())
                {
                    Button* But = (*it).first;
                    return But;
                }
            }
            return 0;
        }

        MenuWindow* MenuWindow::GetWindowOfAccessButton(Button* Accessor)
        {
            for( std::vector<std::pair<Button*,MenuWindow*> >::iterator it = ChildWindows.begin() ; it != ChildWindows.end() ; it++ )
            {
                if(Accessor == (*it).first)
                {
                    MenuWindow* MenWin = (*it).second;
                    return MenWin;
                }
            }
            return 0;
        }

        MenuWindow* MenuWindow::CreateChildMenuWindow(ConstString& Name, const RenderableRect& Rect, Button* Accessor)
        {
            std::pair<Button*,MenuWindow*>* AccessorPair = NULL;
            for( std::vector<std::pair<Button*,MenuWindow*> >::iterator it = ChildWindows.begin() ; it != ChildWindows.end() ; it++ )
            {
                if(Accessor == (*it).first)
                    AccessorPair = &(*it);
            }
            if(NULL==AccessorPair)
                return 0;
            MenuWindow* MenWin = new MenuWindow(Name,Rect,MasterMenu,ParentScreen);
            MenWin->ParentWindow = this;
            AccessorPair->second = MenWin;
            MenWin->Hide();
            return MenWin;
        }

        MenuWindow* MenuWindow::GetChildMenuWindow(ConstString& Name)
        {
            for ( std::vector<std::pair<Button*,MenuWindow*> >::iterator it = ChildWindows.begin() ; it != ChildWindows.end() ; it++ )
            {
                if ( Name == (*it).second->GetName() )
                {
                    MenuWindow* menuwindow = (*it).second;
                    return menuwindow;
                }
            }
            return 0;
        }

        MenuWindow* MenuWindow::GetChildMenuWindow(const Whole& Index)
        {
            return ChildWindows.at(Index).second;
        }

        Whole MenuWindow::GetNumChildMenuWindows()
        {
            return ChildWindows.size();
        }

        void MenuWindow::DestroyChildMenuWindow(MenuWindow* ToBeDestroyed)
        {
            for ( std::vector<std::pair<Button*,MenuWindow*> >::iterator it = ChildWindows.begin() ; it != ChildWindows.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it).second )
                {
                    Button* But = (*it).first;
                    DestroyWidget(But);
                    delete ToBeDestroyed;
                    ChildWindows.erase(it);
                    return;
                }
            }
        }

        void MenuWindow::UpdateDimensions()
        {
            Window::UpdateDimensions();
            for( std::vector<std::pair<Button*,MenuWindow*> >::iterator it = ChildWindows.begin() ; it != ChildWindows.end() ; it++ )
            {
                (*it).second->UpdateDimensions();
            }
        }
    }
}

#endif
