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
#ifndef _uimanager_cpp
#define _uimanager_cpp

#include "uimanager.h"
#include "inputquerytool.h"
#include "mathtool.h"
#include "uitextureatlas.h"
#include "world.h"
#include "cameramanager.h"
#include "graphicsmanager.h"
#include "eventmanager.h"
#include "eventuserinput.h"
#include "resourcemanager.h"
#include "viewport.h"
#include "uiscreen.h"
#include "uibutton.h"
#include "uilayer.h"
#include "uiwidget.h"
#include "uiglyph.h"

#include <Ogre.h>

namespace Mezzanine
{
    template<> UIManager* Singleton<UIManager>::SingletonPtr = 0;

    UIManager::UIManager()
        : HoveredButton(NULL),
          HoveredWidgetButton(NULL),
          HoveredWidget(NULL),
          WidgetFocus(NULL),
          InputCapture(NULL),
          LastWidgetSelected(NULL),
          ButtonAutoRegister(false)
    {
        ResourceManager::GetSingletonPtr()->CreateAssetGroup("UI");
        Priority = 10;
    }

#ifdef MEZZXML
    UIManager::UIManager(xml::Node& XMLNode)
        : HoveredButton(NULL),
          HoveredWidgetButton(NULL),
          HoveredWidget(NULL),
          WidgetFocus(NULL),
          InputCapture(NULL),
          LastWidgetSelected(NULL),
          ButtonAutoRegister(false)
    {
        ResourceManager::GetSingletonPtr()->CreateAssetGroup("UI");
        Priority = 10;
        /// @todo This class currently doesn't initialize anything from XML, if that changes this constructor needs to be expanded.
    }
#endif

    UIManager::~UIManager()
    {
        DestroyAllScreens();
    }

    void UIManager::HoverChecks()
    {
        if(HoveredButton)
        {
            if(HoveredButton->CheckMouseHover())
            {
                HoveredWidget = NULL;
                HoveredWidgetButton = NULL;
                MouseActivationCheck(HoveredButton);
                return;
            }
        }
        if(HoveredWidget)
        {
            if(HoveredWidgetButton)
            {
                if(HoveredWidgetButton->CheckMouseHover())
                {
                    HoveredButton = NULL;
                    MouseActivationCheck(HoveredWidgetButton);
                    return;
                }
            }
            else if(HoveredWidget->CheckMouseHover())
            {
                HoveredButton = NULL;
                HoveredWidgetButton = HoveredWidget->GetHoveredButton();
                if(HoveredWidgetButton)
                    MouseActivationCheck(HoveredWidgetButton);
                return;
            }
        }
        HoveredButton = CheckButtonMouseIsOver();
        if(HoveredButton)
        {
            MouseActivationCheck(HoveredButton);
            HoveredWidget = NULL;
            HoveredWidgetButton = NULL;
        }else{
            HoveredWidget = CheckWidgetMouseIsOver();
            if(HoveredWidget)
            {
                HoveredButton = NULL;
                HoveredWidgetButton = HoveredWidget->GetHoveredButton();
                if(HoveredWidgetButton)
                    MouseActivationCheck(HoveredWidgetButton);
            }
        }
    }

    void UIManager::HotKeyAndInputCaptureChecks()
    {
        if(HotKeys.empty())
            return;
        std::vector<MetaCode::InputCode> CapturedCodes;
        std::list<EventUserInput*>* InputList = EventManager::GetSingletonPtr()->GetAllUserInputEvents();
        MetaCode::InputCode CurrCode;
        for( std::list<EventUserInput*>::iterator Ilit = InputList->begin() ; Ilit != InputList->end() ; Ilit++ )
        {
            for( Whole X = 0 ; X < (*Ilit)->size() ; X++ )
            {
                CurrCode = (*Ilit)->GetMetaCode(X).GetCode();
                if(LastWidgetSelected && LastWidgetSelected->IsInputCaptureWidget())
                {
                    CapturedCodes.push_back(CurrCode);
                }else{
                    HotKeyActivationCheck(CurrCode);
                }
            }
        }
        if(!CapturedCodes.empty())
            LastWidgetSelected->GetInputCaptureData()->UpdateCapturedInputs(CapturedCodes);
    }

    void UIManager::WidgetUpdates()
    {
        if(HoveredWidget)
            HoveredWidget->Update();
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
                if(HoveredWidget != WidgetFocus && WidgetFocus)
                    WidgetFocus->Update(true);
                else if(HoveredWidget && HoveredWidget == WidgetFocus)
                    LastWidgetSelected = HoveredWidget;
                WidgetFocus = NULL;
            }
        }
        else if(!HoveredWidget && !WidgetFocus)
        {
            MetaCode::ButtonState State = InputQueryTool::GetMouseButtonState(1);
            if(LastWidgetSelected && MetaCode::BUTTON_LIFTING == State)
                LastWidgetSelected = NULL;
        }
    }

    void UIManager::ClearButtonActivations()
    {
        if(ActivatedButtons.empty())
            return;
        for(std::vector<UI::Button*>::iterator It = ActivatedButtons.begin() ; It != ActivatedButtons.end() ; It++ )
            (*It)->SetActivation(false);
        ActivatedButtons.clear();
    }

    void UIManager::ViewportUpdateChecks()
    {
        for( Whole x=0 ; x < Screens.size() ; x++ )
            Screens[x]->CheckViewportSize();
    }

    void UIManager::MouseActivationCheck(UI::Button* ToCheck)
    {
        if(!ToCheck)
            return;
        std::vector<MetaCode::InputCode>* MouseCodes = ToCheck->GetMouseActivationButtons();
        UI::ActivationCondition Condition = ToCheck->GetActivationCondition();
        MetaCode::InputCode Code;
        for( Whole X = 0 ; X < MouseCodes->size() ; X++ )
        {
            Code = MouseCodes->at(X);
            if((Condition == UI::AC_OnLift ? MetaCode::BUTTON_LIFTING : MetaCode::BUTTON_PRESSING) == InputQueryTool::GetMouseButtonState(Code))
            {
                ToCheck->SetActivation(true);
                ActivatedButtons.push_back(ToCheck);
            }
        }
    }

    void UIManager::HotKeyActivationCheck(const MetaCode::InputCode& Code)
    {
        if( MetaCode::KEY_FIRST < Code && MetaCode::KEY_LAST > Code )
        {
            std::pair<const std::multimap<MetaCode::InputCode,UI::Button*>::iterator,const std::multimap<MetaCode::InputCode,UI::Button*>::iterator> Result = HotKeys.equal_range(Code);
            if( (*Result.first).first != Code )
                return;
            for( std::multimap<MetaCode::InputCode,UI::Button*>::iterator It = Result.first ; It != Result.second ; It++ )
            {
                if((*It).second->IsVisible())
                    (*It).second->SetActivation(true);
            }
        }
    }

    void UIManager::LoadMTA(const String& Name, const String& Group)
    {
        UI::TextureAtlas* NewAtlas = new UI::TextureAtlas(Name + ".mta",Group);
        Atlases[Name] = NewAtlas;
    }

    UI::TextureAtlas* UIManager::GetAtlas(const String& AtlasName)
    {
        /// @todo These NULL returns should probably be exceptions.
        if("" == AtlasName)
        {
            std::map<String,UI::TextureAtlas*>::iterator It = Atlases.begin();
            if(It != Atlases.end()) return (*It).second;
            else return NULL;
        }
        std::map<String,UI::TextureAtlas*>::iterator It = Atlases.find(AtlasName);
        if(It != Atlases.end()) return (*It).second;
        else return NULL;
    }

    void UIManager::RedrawAll(bool Force)
    {
        for( std::vector<UI::Screen*>::iterator it = Screens.begin() ; it != Screens.end() ; it++ )
        {
            (*it)->_RedrawAllIndexes(Force);
        }
    }

    void UIManager::BindHotKey(const MetaCode::InputCode& HotKey, UI::Button* BoundButton)
    {
        if( MetaCode::KEY_FIRST < HotKey && MetaCode::KEY_LAST > HotKey )
            HotKeys.insert(std::pair<MetaCode::InputCode,UI::Button*>(HotKey,BoundButton));
    }

    void UIManager::UnbindHotKey(const MetaCode::InputCode& HotKey, UI::Button* BoundButton)
    {
        std::pair<const std::multimap<MetaCode::InputCode,UI::Button*>::iterator,const std::multimap<MetaCode::InputCode,UI::Button*>::iterator> Result = HotKeys.equal_range(HotKey);
        for( std::multimap<MetaCode::InputCode,UI::Button*>::iterator It = Result.first ; It != Result.second ; It++ )
        {
            if(BoundButton == (*It).second)
            {
                HotKeys.erase(It);
                return;
            }
        }
    }

    void UIManager::RemoveAllHotKeys()
    {
        HotKeys.clear();
    }

    void UIManager::EnableButtonAutoRegister(bool Enable)
    {
        ButtonAutoRegister = Enable;
    }

    bool UIManager::ButtonAutoRegisterEnabled()
    {
        return ButtonAutoRegister;
    }

    void UIManager::AddAutoRegisterCode(MetaCode::InputCode Code)
    {
        for( std::vector<MetaCode::InputCode>::iterator It = AutoRegisterCodes.begin() ; It != AutoRegisterCodes.end() ; It++ )
        {
            if((*It)==Code)
                return;
        }
        AutoRegisterCodes.push_back(Code);
    }

    void UIManager::RemoveAutoRegisterCode(MetaCode::InputCode Code)
    {
        for( std::vector<MetaCode::InputCode>::iterator It = AutoRegisterCodes.begin() ; It != AutoRegisterCodes.end() ; It++ )
        {
            if((*It)==Code)
            {
                AutoRegisterCodes.erase(It);
                return;
            }
        }
    }

    void UIManager::RemoveAllAutoRegisterCodes()
    {
        AutoRegisterCodes.clear();
    }

    std::vector<MetaCode::InputCode>* UIManager::GetAutoRegisteredCodes()
    {
        return &AutoRegisterCodes;
    }

    UI::Button* UIManager::GetHoveredButton()
    {
        return HoveredButton;
    }

    UI::Button* UIManager::GetHoveredWidgetButton()
    {
        return HoveredWidgetButton;
    }

    UI::Widget* UIManager::GetHoveredWidget()
    {
        return HoveredWidget;
    }

    UI::Widget* UIManager::GetWidgetFocus()
    {
        return WidgetFocus;
    }

    UI::Widget* UIManager::GetWidgetCapturingInput()
    {
        return InputCapture;
    }

    UI::Screen* UIManager::CreateScreen(const String& ScreenName, const String& Atlas, Viewport* WindowViewport)
    {
        UI::Screen* MezzScreen = new UI::Screen(ScreenName, Atlas, WindowViewport);
        Screens.push_back(MezzScreen);
        return MezzScreen;
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

    UI::Layer* UIManager::GetLayer(const String& Name)
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

    std::pair<Whole,Real> UIManager::SuggestGlyphIndex(const Whole& Height, const String& Atlas)
    {
        UI::TextureAtlas* TheAtlas = NULL;
        std::map<String,UI::TextureAtlas*>::iterator It = Atlases.find(Atlas);
        if(It != Atlases.end()) TheAtlas = (*It).second;
        else
        {
            std::stringstream logstream;
            logstream << "Attempting to access TextureAtlas \"" << Atlas << "\", which does not exist or is not loaded, in UIManager::SuggestGlyphIndex.";
            World::GetWorldPointer()->LogAndThrow(Exception(logstream.str()));
        }
        std::map<UInt32,UI::GlyphData*>& Glyphs = TheAtlas->GetGlyphs();
        Whole LargerMatch = 0;
        Whole SmallerMatch = 0;
        Real LargerHeight = 0;
        Real SmallerHeight = 0;
        Real LargerMatchDiff = 1000000.0;
        Real SmallerMatchDiff = 1000000.0;
        Real RequestedHeight = (Real)Height;

        for( std::map<UInt32,UI::GlyphData*>::iterator it = Glyphs.begin() ; it != Glyphs.end() ; it++ )
        {
            Real Diff = 0.0;
            if((*it).second->LineHeight > RequestedHeight)
            {
                Diff = (*it).second->LineHeight - RequestedHeight;
                if(Diff < LargerMatchDiff)
                {
                    LargerMatch = (*it).first;
                    LargerHeight = (*it).second->LineHeight;
                    LargerMatchDiff = Diff;
                    continue;
                }
            }else{
                Diff = RequestedHeight - (*it).second->LineHeight;
                if(Diff < SmallerMatchDiff)
                {
                    SmallerMatch = (*it).first;
                    SmallerHeight = (*it).second->LineHeight;
                    SmallerMatchDiff = Diff;
                    continue;
                }
            }
        }

        Real Scale = 1;
        if(LargerMatch != 0)
        {
            Scale = RequestedHeight / LargerHeight;
            return std::pair<Whole,Real>(LargerMatch,Scale);
        }else{
            Scale = RequestedHeight / SmallerHeight;
            return std::pair<Whole,Real>(SmallerMatch,Scale);
        }
    }

    void UIManager::Initialize()
    {
        Initialized = true;
    }

    void UIManager::DoMainLoopItems()
    {
        InputQueryTool::GatherEvents();
        ViewportUpdateChecks();
        ClearButtonActivations();
        HoverChecks();
        HotKeyAndInputCaptureChecks();
        WidgetUpdates();
    }

    ManagerBase::ManagerType UIManager::GetInterfaceType() const
        { return ManagerBase::UIManager; }

    String UIManager::GetImplementationTypeName() const
        { return "DefaultUIManager"; }

    ///////////////////////////////////////////////////////////////////////////////
    // DefaultUIManagerFactory Methods

    DefaultUIManagerFactory::DefaultUIManagerFactory()
    {
    }

    DefaultUIManagerFactory::~DefaultUIManagerFactory()
    {
    }

    String DefaultUIManagerFactory::GetManagerTypeName() const
    {
        return "DefaultUIManager";
    }

    ManagerBase* DefaultUIManagerFactory::CreateManager(NameValuePairList& Params)
    {
        if(UIManager::SingletonValid())
        {
            /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
            return UIManager::GetSingletonPtr();
        }else return new UIManager();
    }

    ManagerBase* DefaultUIManagerFactory::CreateManager(xml::Node& XMLNode)
    {
        if(UIManager::SingletonValid())
        {
            /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
            return UIManager::GetSingletonPtr();
        }else return new UIManager(XMLNode);
    }

    void DefaultUIManagerFactory::DestroyManager(ManagerBase* ToBeDestroyed)
    {
        delete ToBeDestroyed;
    }
}//Mezzanine

#endif
