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
#include "internalGorilla.h.cpp"

namespace Mezzanine
{
    namespace UI
    {
        Button::Button(ConstString& name, const RenderableRect& Rect, Layer* PLayer)
            : BasicRenderable(name,PLayer),
              NormalSprite(NULL),
              HoveredSprite(NULL),
              UserSprite(NULL),
              Callback(NULL),
              MouseHover(false),
              Activated(false),
              MultipleActivations(false),
              ActCond(UI::AC_OnLift)
        {
            if(Rect.Relative)
            {
                RelPosition = Rect.Position;
                RelSize = Rect.Size;

                const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
                GorillaRectangle = Parent->GetGorillaLayer()->createRectangle((Rect.Position * WinDim).GetOgreVector2(),(Rect.Size * WinDim).GetOgreVector2());
            }else{
                RelPosition = Rect.Position / Parent->GetParent()->GetViewportDimensions();
                RelSize = Rect.Size / Parent->GetParent()->GetViewportDimensions();

                GorillaRectangle = Parent->GetGorillaLayer()->createRectangle(Rect.Position.GetOgreVector2(),Rect.Size.GetOgreVector2());
            }
            GorillaRectangle->SetNameFile(Parent->GetParent()->GetPrimaryAtlas());
        }

        Button::~Button()
        {
            Parent->GetGorillaLayer()->destroyRectangle(GorillaRectangle);
            if(Callback)
                delete Callback;
        }

        void Button::SetHovered(bool Hovered)
        {
            if(Hovered && Callback)
                Callback->DoHoverItems();
            if(HoveredSprite)
            {
                if(Hovered && !MouseHover)
                    GorillaRectangle->background_image(HoveredSprite);
                else if(!Hovered && MouseHover)
                    GorillaRectangle->background_image(NormalSprite);
            }
            MouseHover = Hovered;
        }

        void Button::SetVisible(bool Visible)
        {
            GorillaRectangle->SetVisible(Visible);
        }

        bool Button::IsVisible() const
        {
            return GorillaRectangle->IsVisible() && Parent->IsVisible() && Parent->GetParent()->IsVisible();
        }

        void Button::Show()
        {
            GorillaRectangle->Show();
        }

        void Button::Hide()
        {
            GorillaRectangle->Hide();
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

        bool Button::CheckMouseHover()
        {
            if(!IsVisible())
                return false;
            Vector2 MouseLoc = InputQueryTool::GetMouseCoordinates();
            bool Hovered = GorillaRectangle->intersects(MouseLoc.GetOgreVector2());
            if(Hovered != MouseHover)
                SetHovered(Hovered);
            return MouseHover;
        }

        bool Button::GetMouseHover()
        {
            return MouseHover;
        }

        void Button::SetBackgroundColour(const ColourValue& Colour)
        {
            GorillaRectangle->background_colour(Colour.GetOgreColourValue());
        }

        void Button::SetBackgroundSprite(const String& Name)
        {
            Gorilla::Sprite* GSprite = Parent->GetGorillaLayer()->_getSprite(Name,*GorillaRectangle->GetNameFile());
            NormalSprite = GSprite;
            GorillaRectangle->background_image(GSprite);
        }

        void Button::SetBackgroundSprite(const String& Name, const String& Atlas)
        {
            Gorilla::Sprite* GSprite = Parent->GetGorillaLayer()->_getSprite(Name,Atlas);
            NormalSprite = GSprite;
            GorillaRectangle->background_image(GSprite);
        }

        void Button::SetHoveredSprite(const String& Name)
        {
            Gorilla::Sprite* GSprite = Parent->GetGorillaLayer()->_getSprite(Name,*GorillaRectangle->GetNameFile());
            HoveredSprite = GSprite;
            if(MouseHover)
                GorillaRectangle->background_image(HoveredSprite);
        }

        void Button::SetHoveredSprite(const String& Name, const String& Atlas)
        {
            Gorilla::Sprite* GSprite = Parent->GetGorillaLayer()->_getSprite(Name,Atlas);
            HoveredSprite = GSprite;
        }

        void Button::SetUserSprite(const String& Name)
        {
            Gorilla::Sprite* GSprite = Parent->GetGorillaLayer()->_getSprite(Name,*GorillaRectangle->GetNameFile());
            UserSprite = GSprite;
        }

        void Button::SetUserSprite(const String& Name, const String& Atlas)
        {
            Gorilla::Sprite* GSprite = Parent->GetGorillaLayer()->_getSprite(Name,Atlas);
            UserSprite = GSprite;
        }

        void Button::EnableUserSprite(bool Enable)
        {
            if(Enable)
            {
                GorillaRectangle->background_image(UserSprite);
            }else{
                GorillaRectangle->background_image(NormalSprite);
            }
        }

        void Button::SetBorder(const Real& Width, const ColourValue& Colour)
        {
            GorillaRectangle->border(Width, Colour.GetOgreColourValue());
        }

        void Button::NoBorder()
        {
            GorillaRectangle->no_border();
        }

        void Button::SetPosition(const Vector2& Position)
        {
            RelPosition = Position;
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
            GorillaRectangle->left(WinDim.X * RelPosition.X);
            GorillaRectangle->top(WinDim.Y * RelPosition.Y);
        }

        Vector2 Button::GetPosition() const
        {
            return RelPosition;
        }

        void Button::SetActualPosition(const Vector2& Position)
        {
            RelPosition = Position / Parent->GetParent()->GetViewportDimensions();
            GorillaRectangle->left(Position.X);
            GorillaRectangle->top(Position.Y);
        }

        Vector2 Button::GetActualPosition() const
        {
            Vector2 Pos(GorillaRectangle->left(), GorillaRectangle->top());
            return Pos;
        }

        void Button::SetSize(const Vector2& Size)
        {
            RelSize = Size;
            const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
            GorillaRectangle->width(WinDim.X * RelSize.X);
            GorillaRectangle->height(WinDim.Y * RelSize.Y);
        }

        Vector2 Button::GetSize() const
        {
            return RelSize;
        }

        void Button::SetActualSize(const Vector2& Size)
        {
            RelSize = Size / Parent->GetParent()->GetViewportDimensions();
            GorillaRectangle->width(Size.X);
            GorillaRectangle->height(Size.Y);
        }

        Vector2 Button::GetActualSize() const
        {
            Vector2 Pos(GorillaRectangle->width(), GorillaRectangle->height());
            return Pos;
        }

        void Button::SetRenderPriority(const UI::RenderPriority& Priority)
        {
            Gorilla::RenderPriority RP;
            switch(Priority)
            {
                case UI::RP_Low:
                    RP = Gorilla::RP_Low;
                    break;
                case UI::RP_Medium:
                    RP = Gorilla::RP_Medium;
                    break;
                case UI::RP_High:
                    RP = Gorilla::RP_High;
                    break;
                default:
                    break;
            }
            GorillaRectangle->RenderPriority(RP);
        }

        UI::RenderPriority Button::GetRenderPriority() const
        {
            Gorilla::RenderPriority RP = this->GorillaRectangle->RenderPriority();
            switch(RP)
            {
                case Gorilla::RP_Low:
                    return UI::RP_Low;
                    break;
                case Gorilla::RP_Medium:
                    return UI::RP_Medium;
                    break;
                case Gorilla::RP_High:
                    return UI::RP_High;
                    break;
                default:
                    break;
            }
            return UI::RP_Medium;
        }

        void Button::SetPrimaryAtlas(const String& Atlas)
        {
            GorillaRectangle->SetNameFile(Atlas);
        }

        String Button::GetPrimaryAtlas() const
        {
            return *GorillaRectangle->GetNameFile();
        }

        std::vector<MetaCode::InputCode>* Button::GetKeyboardActivationKeys()
        {
            return &KeyboardActivationKeys;
        }

        std::vector<MetaCode::InputCode>* Button::GetMouseActivationButtons()
        {
            return &MouseActivationButtons;
        }

        void Button::UpdateDimensions()
        {
            ViewportUpdateTool::UpdateRenderable(this);
            //this->SetActualPosition(RelPosition * Parent->GetParent()->GetViewportDimensions());
            //this->SetActualSize(RelSize * Parent->GetParent()->GetViewportDimensions());
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
