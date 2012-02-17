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
#ifndef _uibutton_cpp
#define _uibutton_cpp

#include "uibutton.h"
#include "uilayer.h"
#include "uiscreen.h"
#include "uimanager.h"
#include "uiviewportupdatetool.h"
#include "eventmanager.h"
#include "inputquerytool.h"
#include "world.h"

namespace Mezzanine
{
    namespace UI
    {
        Button::Button(ConstString& name, const RenderableRect& Rect, Layer* PLayer)
            : Rectangle(name,Rect,PLayer),
              NormalSprite(NULL),
              HoveredSprite(NULL),
              UserSprite(NULL),
              Callback(NULL),
              Activated(false),
              UserSpriteEnabled(false),
              MultipleActivations(false),
              ActCond(UI::AC_OnLift)
        {
        }

        Button::~Button()
        {
            if(Callback)
                delete Callback;
        }

        void Button::SetHovered(bool Hovered)
        {
            if(Hovered == MouseHover)
                return;
            if(Hovered && Callback)
                Callback->DoHoverItems();
            if(HoveredSprite)
            {
                if(Hovered && !MouseHover)
                    SetSprite(HoveredSprite);
                else if(!Hovered && MouseHover)
                {
                    if(UserSpriteEnabled) SetSprite(UserSprite);
                    else SetSprite(NormalSprite);
                }
            }
            MouseHover = Hovered;
        }

        bool Button::IsTextButton()
        {
            return false;
        }

        void Button::SetButtonCallback(ButtonCallback* CB)
        {
            if(Callback != CB && Callback)
                delete Callback;
            CB->SetCaller(this);
            Callback = CB;
        }

        void Button::BindActivationKeyOrButton(const MetaCode::InputCode& Code)
        {
            if(MetaCode::KEY_FIRST < Code && MetaCode::KEY_LAST > Code)
            {
                for( std::vector<MetaCode::InputCode>::iterator It = KeyboardActivationKeys.begin() ; It != KeyboardActivationKeys.end() ; It++ )
                {
                    if((*It)==Code)
                        return;
                }
                KeyboardActivationKeys.push_back(Code);

            }else if(MetaCode::MOUSE_FIRST < Code && MetaCode::MOUSE_LAST > Code)
            {
                for( std::vector<MetaCode::InputCode>::iterator It = MouseActivationButtons.begin() ; It != MouseActivationButtons.end() ; It++ )
                {
                    if((*It)==Code)
                        return;
                }
                MouseActivationButtons.push_back(Code);
            }else{
                /// @todo Throw an error?
            }
        }

        void Button::UnbindActivationKeyOrButton(const MetaCode::InputCode& Code)
        {
            if(MetaCode::KEY_FIRST < Code && MetaCode::KEY_LAST > Code)
            {
                for( std::vector<MetaCode::InputCode>::iterator It = KeyboardActivationKeys.begin() ; It != KeyboardActivationKeys.end() ; It++ )
                {
                    if((*It)==Code)
                    {
                        KeyboardActivationKeys.erase(It);
                        return;
                    }
                }

            }else if(MetaCode::MOUSE_FIRST < Code && MetaCode::MOUSE_LAST > Code)
            {
                for( std::vector<MetaCode::InputCode>::iterator It = MouseActivationButtons.begin() ; It != MouseActivationButtons.end() ; It++ )
                {
                    if((*It)==Code)
                    {
                        MouseActivationButtons.erase(It);
                        return;
                    }
                }
            }
        }

        void Button::UnbindAllKeyboardActivationKeys()
        {
            KeyboardActivationKeys.clear();
        }

        void Button::UnbindAllMouseActivationButtons()
        {
            MouseActivationButtons.clear();
        }

        void Button::UnbindAllActivationKeysAndButtons()
        {
            UnbindAllKeyboardActivationKeys();
            UnbindAllMouseActivationButtons();
        }

        void Button::SetActivationCondition(const UI::ActivationCondition& Condition)
        {
            ActCond = Condition;
        }

        UI::ActivationCondition Button::GetActivationCondition()
        {
            return ActCond;
        }

        void Button::SetActivation(bool Activate)
        {
            if(Activate && Activated && !MultipleActivations)
                return;
            if(Activate && Callback)
                Callback->DoActivateItems();
            Activated = Activate;
        }

        bool Button::IsActivated()
        {
            return Activated;
        }

        void Button::EnableMultipleActivations(bool Enable)
        {
            MultipleActivations = Enable;
        }

        bool Button::IsMultipleActivationsEnabled()
        {
            return MultipleActivations;
        }

        void Button::SetBackgroundSprite(Sprite* PSprite)
        {
            NormalSprite = PSprite;
            if(!MouseHover && !UserSpriteEnabled)
            {
                SetSprite(PSprite);
            }
        }

        void Button::SetBackgroundSprite(const String& SpriteName)
        {
            Rectangle::SetBackgroundSprite(SpriteName);
        }

        void Button::SetBackgroundSprite(const String& SpriteName, const String& Atlas)
        {
            Rectangle::SetBackgroundSprite(SpriteName,Atlas);
        }

        void Button::SetHoveredSprite(Sprite* PSprite)
        {
            HoveredSprite = PSprite;
            if(MouseHover)
            {
                SetSprite(PSprite);
            }
        }

        void Button::SetHoveredSprite(const String& SpriteName)
        {
            Sprite* PSprite = ParentLayer->GetSprite(SpriteName,PriAtlas);
            SetHoveredSprite(PSprite);
        }

        void Button::SetHoveredSprite(const String& SpriteName, const String& Atlas)
        {
            Sprite* PSprite = ParentLayer->GetSprite(SpriteName,Atlas);
            SetHoveredSprite(PSprite);
        }

        void Button::SetUserSprite(Sprite* PSprite)
        {
            UserSprite = PSprite;
            if(!MouseHover && UserSpriteEnabled)
            {
                SetSprite(PSprite);
            }
        }

        void Button::SetUserSprite(const String& SpriteName)
        {
            Sprite* PSprite = ParentLayer->GetSprite(SpriteName,PriAtlas);
            SetUserSprite(PSprite);
        }

        void Button::SetUserSprite(const String& SpriteName, const String& Atlas)
        {
            Sprite* PSprite = ParentLayer->GetSprite(SpriteName,Atlas);
            SetUserSprite(PSprite);
        }

        void Button::EnableUserSprite(bool Enable)
        {
            if(UserSpriteEnabled == Enable)
                return;
            if(Enable && !MouseHover)
            {
                SetSprite(UserSprite);
            }
            else if(!Enable && !MouseHover)
            {
                SetSprite(NormalSprite);
            }
            UserSpriteEnabled = Enable;
        }

        std::vector<MetaCode::InputCode>* Button::GetKeyboardActivationKeys()
        {
            return &KeyboardActivationKeys;
        }

        std::vector<MetaCode::InputCode>* Button::GetMouseActivationButtons()
        {
            return &MouseActivationButtons;
        }

        ButtonCallback::ButtonCallback()
            : Caller(NULL)
        {
        }

        ButtonCallback::~ButtonCallback()
        {
        }

        void ButtonCallback::SetCaller(Button* Caller)
        {
            this->Caller = Caller;
        }
    }//UI
}//Mezzanine

#endif
