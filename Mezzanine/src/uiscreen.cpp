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
#ifndef _uiscreen_cpp
#define _uiscreen_cpp

#include "uiscreen.h"
#include "uimanager.h"
#include "uilayer.h"
#include "uibutton.h"
#include "uiviewportupdatetool.h"
#include "viewport.h"
#include "world.h"
#include "internalGorilla.h.cpp"

namespace Mezzanine
{
    namespace UI
    {
        Screen::Screen(const String& name, const String& Atlas, Viewport* WindowViewport)
            : Name(name),
              PrimaryAtlas(Atlas),
              GameViewport(WindowViewport)
        {
            Manager = UIManager::GetSingletonPtr();

            GorillaScreen = Manager->GetSilverbackPointer()->createScreen(WindowViewport->GetOgreViewport());
            KnownViewportSize = Vector2((Real)GameViewport->GetActualWidth(),(Real)GameViewport->GetActualHeight());
        }

        Screen::~Screen()
        {
            Manager->GetSilverbackPointer()->destroyScreen(GorillaScreen);
        }

        String& Screen::GetName()
        {
            return Name;
        }

        void Screen::SetVisible(bool Visible)
        {
            GorillaScreen->setVisible(Visible);
        }

        bool Screen::IsVisible()
        {
            return GorillaScreen->isVisible();
        }

        void Screen::Show()
        {
            GorillaScreen->show();
        }

        void Screen::Hide()
        {
            GorillaScreen->hide();
        }

        Layer* Screen::CreateLayer(const String& Name, Whole Zorder)
        {
            Gorilla::Layer* layer = GorillaScreen->createLayer(Zorder);
            Layer* Mezzlayer = new Layer(Name, layer, this);
            std::pair<std::map<Whole,Layer*>::iterator,bool> TestPair = Layers.insert(std::pair<Whole,Layer*>(Zorder,Mezzlayer));
            if(TestPair.second)
            {
                return Mezzlayer;
            }else{
                /// @todo add an exception here or maybe log entry, some notification it failed.
                return 0;
            }
        }

        Layer* Screen::GetLayer(const String& Name)
        {
            for ( std::map<Whole,Layer*>::iterator it = Layers.begin() ; it != Layers.end() ; it++ )
            {
                if ( Name == (*it).second->GetName() )
                {
                    Layer* Layer = (*it).second;
                    return Layer;
                }
            }
            return 0;
        }

        Layer* Screen::GetLayer(Whole Index)
        {
            std::map<Whole,Layer*>::iterator it = Layers.begin();
            while(0 < Index)
            {
                it++;
                Index--;
            }
            if(it!=Layers.end())
                return (*it).second;
            return 0;
        }

        Layer* Screen::GetLayerbyZorder(Whole Zorder)
        {
            std::map<Whole,Layer*>::iterator it = Layers.find(Zorder);
            if(it!=Layers.end())
                return (*it).second;
            return 0;
        }

        Whole Screen::GetNumLayers()
        {
            return Layers.size();
        }

        void Screen::DestroyLayer(Layer* ToBeDestroyed)
        {
            if(Layers.empty())
                return;
            for( std::map<Whole,Layer*>::iterator it = Layers.begin() ; it != Layers.end() ; it++ )
            {
                if( ToBeDestroyed == (*it).second )
                {
                    delete (*it).second;
                    Layers.erase(it);
                    return;
                }
            }
        }

        const Vector2& Screen::GetViewportDimensions()
        {
            return KnownViewportSize;
        }

        Button* Screen::CheckButtonMouseIsOver()
        {
            for( std::map<Whole,Layer*>::reverse_iterator it = Layers.rbegin() ; it != Layers.rend() ; it++ )
            {
                if( (*it).second->IsVisible() )
                {
                    Button* button = (*it).second->CheckButtonMouseIsOver();
                    if(button)
                    {
                        return button;
                    }
                }
            }
            return 0;
        }

        Widget* Screen::CheckWidgetMouseIsOver()
        {
            for( std::map<Whole,Layer*>::reverse_iterator it = Layers.rbegin() ; it != Layers.rend() ; it++ )
            {
                if( (*it).second->IsVisible() )
                {
                    Widget* widget = (*it).second->CheckWidgetMouseIsOver();
                    if(widget)
                    {
                        return widget;
                    }
                }
            }
            return 0;
        }

        void Screen::SetPrimaryAtlas(const String& Atlas)
        {
            PrimaryAtlas = Atlas;
        }

        String Screen::GetPrimaryAtlas()
        {
            return PrimaryAtlas;
        }

        void Screen::CheckViewportSize()
        {
            Vector2 CurrentSize((Real)GameViewport->GetActualWidth(),(Real)GameViewport->GetActualHeight());
            if(KnownViewportSize == CurrentSize)
                return;

            ViewportUpdateTool::ConfigureSizes(KnownViewportSize,CurrentSize);
            KnownViewportSize = CurrentSize;
            for( std::map<Whole,Layer*>::iterator it = Layers.begin() ; it != Layers.end() ; it++ )
            {
                (*it).second->ViewportUpdate();
            }
        }

        /*void Screen::RenderOnce()
        {
            if(IsVisible() && Layers.size())
                GorillaScreen->renderOnce();
        }*/

        Gorilla::Screen* Screen::GetGorillaScreen()
        {
            return GorillaScreen;
        }
    }//ui
}//Mezzanine

#endif
