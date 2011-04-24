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
#ifndef _uimanager_cpp
#define _uimanager_cpp

#include "uimanager.h"
#include "inputquerytool.h"
#include "internalGorilla.h.cpp"
#include "world.h"
#include "cameramanager.h"
#include "graphicsmanager.h"
#include "viewport.h"
#include "uiscreen.h"
#include "uibutton.h"
#include "uilayer.h"
#include "uiwidget.h"

#include <Ogre.h>

namespace phys
{
    UIManager::UIManager()
        : HoveredButton(NULL),
          HoveredWidget(NULL),
          WidgetFocus(NULL)
    {
        Silver = new Gorilla::Silverback();
        Priority = -35;
    }

    UIManager::~UIManager()
    {
        DestroyAllScreens();
        delete Silver;
    }

    void UIManager::HoverChecks()
    {
        if(HoveredButton)
        {
            if(HoveredButton->CheckMouseHover())
            {
                return;
            }
        }
        if(HoveredWidget)
        {
            if(HoveredWidget->CheckMouseHover())
            {
                HoveredWidget->Update();
                return;
            }
        }
        HoveredButton = CheckButtonMouseIsOver();
        if(!HoveredButton)
        {
            HoveredWidget = CheckWidgetMouseIsOver();
            if(HoveredWidget)
            {
                //HoveredButton = HoveredWidget->GetHoveredButton();
                HoveredWidget->Update();
            }
        }
    }

    void UIManager::WidgetFocusUpdate()
    {
        if(HoveredWidget || WidgetFocus)
        {
            MetaCode::ButtonState State = InputQueryTool::GetMouseButtonState(1);
            if(MetaCode::BUTTON_PRESSING == State)
            {
                WidgetFocus = HoveredWidget;
            }
            else if(MetaCode::BUTTON_DOWN == State)
            {
                if(HoveredWidget != WidgetFocus && WidgetFocus)
                    WidgetFocus->Update(true);
            }
            else if(MetaCode::BUTTON_LIFTING == State)
            {
                if(HoveredWidget != WidgetFocus)
                    WidgetFocus->Update(true);
                WidgetFocus = NULL;
            }
        }
    }

    void UIManager::Initialize()
    {
    }

    void UIManager::DoMainLoopItems()
    {
        InputQueryTool::GatherEvents();
        HoverChecks();
        WidgetFocusUpdate();
    }

    void UIManager::LoadGorilla(const String& Name)
    {
        Silver->loadAtlas(Name);
    }

    void UIManager::RedrawAll(bool Force)
    {
        for( std::vector<UI::Screen*>::iterator it = Screens.begin() ; it != Screens.end() ; it++ )
        {
            (*it)->GetGorillaScreen()->_redrawAllIndexes(Force);
        }
    }

    UI::Button* UIManager::GetHoveredButton()
    {
        return HoveredButton;
    }

    UI::Widget* UIManager::GetHoveredWidget()
    {
        return HoveredWidget;
    }

    UI::Widget* UIManager::GetWidgetFocus()
    {
        return WidgetFocus;
    }

    UI::Screen* UIManager::CreateScreen(const String& ScreenName, const String& Atlas, Viewport* WindowViewport)
    {
        Ogre::Viewport* OgrePort = WindowViewport->GetOgreViewport();
        Gorilla::Screen* guiscreen = Silver->createScreen(OgrePort, Atlas);
        UI::Screen* physscreen = new UI::Screen(ScreenName, guiscreen, WindowViewport);
        Screens.push_back(physscreen);
        return physscreen;
    }

    UI::Screen* UIManager::GetScreen(const String& Name)
    {
        std::vector<UI::Screen*>::iterator it = Screens.begin();
        for ( std::vector<UI::Screen*>::iterator it = Screens.begin() ; it != Screens.end() ; it++ )
        {
            if ( Name == (*it)->GetName() )
            {
                UI::Screen* Screen = (*it);
                return Screen;
            }
        }
        return 0;
    }

    UI::Screen* UIManager::GetScreen(Whole Index)
    {
        return Screens[Index];
    }

    Whole UIManager::GetNumScreens()
    {
        return Screens.size();
    }

    void UIManager::DestroyScreen(UI::Screen* Screen)
    {
        if(Screens.empty())
            return;
        for( std::vector<UI::Screen*>::iterator it = Screens.begin() ; it != Screens.end() ; it++ )
        {
            if( Screen == (*it) )
            {
                delete (*it);
                Screens.erase(it);
                return;
            }
        }
    }

    void UIManager::DestroyAllScreens()
    {
        if(Screens.empty())
            return;
        for( std::vector<UI::Screen*>::iterator it = Screens.begin() ; it != Screens.end() ; it++ )
        {
            delete (*it);
        }
        Screens.clear();
        return;
    }

    UI::Layer* UIManager::GetLayer(String& Name)
    {
        for( Whole x=0 ; x < Screens.size() ; x++ )
        {
            UI::Layer* Layer = Screens[x]->GetLayer(Name);
            if(NULL!=Layer)
                return Layer;
        }
        return 0;
    }

    UI::Button* UIManager::CheckButtonMouseIsOver()
    {
        for( Whole x=0 ; x < Screens.size() ; x++ )
        {
            if( Screens[x]->IsVisible() )
            {
                UI::Button* button = Screens[x]->CheckButtonMouseIsOver();
                if(button)
                {
                    return button;
                }
            }
        }
        return 0;
    }

    UI::Widget* UIManager::CheckWidgetMouseIsOver()
    {
        for( Whole x=0 ; x < Screens.size() ; x++ )
        {
            if( Screens[x]->IsVisible() )
            {
                UI::Widget* widget = Screens[x]->CheckWidgetMouseIsOver();
                if(widget)
                {
                    return widget;
                }
            }
        }
        return 0;
    }

    bool UIManager::MouseIsInUISystem()
    {
        if(HoveredButton || HoveredWidget || WidgetFocus)
        {
            return true;
        }else{
            return false;
        }
    }

    ManagerBase::ManagerTypeName UIManager::GetType() const
        { return ManagerBase::UIManager; }

    Gorilla::Silverback* UIManager::GetSilverbackPointer()
    {
        return Silver;
    }
}//phys

#endif
