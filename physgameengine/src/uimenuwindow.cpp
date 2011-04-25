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
#ifndef _uimenuwindow_cpp
#define _uimenuwindow_cpp

#include "uimenuwindow.h"
#include "uimenu.h"
#include "uibutton.h"
#include "uilayer.h"

namespace phys
{
    namespace UI
    {
        MenuWindow::MenuWindow(ConstString& Name, const Vector2 Position, const Vector2 Size, UI::Menu* TheMenu,Layer* PLayer)
            : Window(Name,Position,Size,PLayer),
              BackButton(NULL),
              ParentWindow(NULL),
              MasterMenu(TheMenu)
        {
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

        MenuWindow* MenuWindow::GetParentWindow()
        {
            return ParentWindow;
        }

        Button* MenuWindow::CreateBackButton(const Vector2 Position, const Vector2 Size)
        {
            if(!BackButton && ParentWindow)
            {
                BackButton = new Button(Name+"back",Position,Size,Parent);
                Vector2 Offset = Position - RelPosition;
                OffsetButtonInfo backbuttonoff(BackButton,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
                Buttons.push_back(backbuttonoff);
                return BackButton;
            }
            return 0;
        }

        Button* MenuWindow::GetBackButton()
        {
            return BackButton;
        }

        Button* MenuWindow::CreateAccessorButton(ConstString& Name, const Vector2 Position, const Vector2 Size)
        {
            Vector2 Offset = Position - RelPosition;
            Button* AccBut = this->CreateButton(Name,Position,Size);
            OffsetButtonInfo buttonoff(AccBut,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Buttons.push_back(buttonoff);
            ChildWindows.push_back(std::pair<Button*,MenuWindow*>(AccBut,NULL));
            return buttonoff.Object;
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

        MenuWindow* MenuWindow::CreateChildMenuWindow(ConstString& Name, const Vector2 Position, const Vector2 Size, Button* Accessor)
        {
            std::pair<Button*,MenuWindow*>* AccessorPair = NULL;
            for( std::vector<std::pair<Button*,MenuWindow*> >::iterator it = ChildWindows.begin() ; it != ChildWindows.end() ; it++ )
            {
                if(Accessor == (*it).first)
                    AccessorPair = &(*it);
            }
            if(NULL==AccessorPair)
                return 0;
            MenuWindow* MenWin = new MenuWindow(Name,Position,Size,MasterMenu,Parent);
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

        MenuWindow* MenuWindow::GetChildMenuWindow(const Whole Index)
        {
            return ChildWindows[Index].second;
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
                    DestroyButton(But);
                    delete ToBeDestroyed;
                    ChildWindows.erase(it);
                    return;
                }
            }
        }
    }
}

#endif
