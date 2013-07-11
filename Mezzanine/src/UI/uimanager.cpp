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
#ifndef _uiuimanager_cpp
#define _uiuimanager_cpp

#include "UI/uimanager.h"
#include "entresol.h"
#include "mathtool.h"
#include "UI/textureatlas.h"
#include "cameramanager.h"
#include "Graphics/graphicsmanager.h"
#include "eventmanager.h"
#include "eventuserinput.h"
#include "resourcemanager.h"
#include "Graphics/viewport.h"
#include "UI/screen.h"
#include "UI/button.h"
#include "UI/widget.h"
#include "UI/glyph.h"
#include "Input/inputmanager.h"
#include "Input/mouse.h"

#include <Ogre.h>

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // WidgetUpdateWorkUnit Methods

        WidgetUpdateWorkUnit::WidgetUpdateWorkUnit(const WidgetUpdateWorkUnit& Other)
            {  }

        WidgetUpdateWorkUnit& WidgetUpdateWorkUnit::operator=(const WidgetUpdateWorkUnit& Other)
            { return *this; }

        WidgetUpdateWorkUnit::WidgetUpdateWorkUnit(UIManager* Target) :
            TargetManager(Target) {  }

        WidgetUpdateWorkUnit::~WidgetUpdateWorkUnit()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void WidgetUpdateWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
        {
            this->TargetManager->ViewportUpdateChecks();
            this->TargetManager->ClearButtonActivations();
            this->TargetManager->HoverChecks();
            this->TargetManager->HotKeyAndInputCaptureChecks();
            this->TargetManager->WidgetUpdates();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // UIManager Methods

        template<> UIManager* Singleton<UIManager>::SingletonPtr = NULL;

        UIManager::UIManager() :
            HoveredWidget(NULL),
            WidgetFocus(NULL),
            InputCapture(NULL),
            LastWidgetSelected(NULL),
            ButtonAutoRegister(false),

            WidgetUpdateWork(NULL),
            ThreadResources(NULL)
        {
            ResourceManager::GetSingletonPtr()->CreateAssetGroup("UI");

            this->WidgetUpdateWork = new WidgetUpdateWorkUnit(this);
        }

        UIManager::UIManager(XML::Node& XMLNode) :
            HoveredWidget(NULL),
            WidgetFocus(NULL),
            InputCapture(NULL),
            LastWidgetSelected(NULL),
            ButtonAutoRegister(false),

            WidgetUpdateWork(NULL),
            ThreadResources(NULL)
        {
            ResourceManager::GetSingletonPtr()->CreateAssetGroup("UI");
            /// @todo This class currently doesn't initialize anything from XML, if that changes this constructor needs to be expanded.

            this->WidgetUpdateWork = new WidgetUpdateWorkUnit(this);
        }

        UIManager::~UIManager()
        {
            this->Deinitialize();

            delete WidgetUpdateWork;

            this->DestroyAllScreens();
        }

        void UIManager::HoverChecks()
        {
            HoveredWidget = CheckWidgetMouseIsOver();
            if(HoveredWidget)
            {
                Widget* BottomMost = HoveredWidget->GetBottomMostHoveredWidget();
                if(Widget::W_Button == BottomMost->GetType())
                {
                    MouseActivationCheck(static_cast<Button*>(BottomMost));
                }
            }
        }

        void UIManager::HotKeyAndInputCaptureChecks()
        {
            if(HotKeys.empty())
                return;
            std::vector<Input::InputCode> CapturedCodes;
            std::list<EventUserInput*>* InputList = EventManager::GetSingletonPtr()->GetAllUserInputEvents();
            Input::InputCode CurrCode;
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
                Input::ButtonState State = Input::InputManager::GetSingletonPtr()->GetSystemMouse()->GetButtonState(1);
                if(Input::BUTTON_PRESSING == State)
                {
                    WidgetFocus = HoveredWidget;
                }
                else if(Input::BUTTON_DOWN == State)
                {
                    if(HoveredWidget != WidgetFocus && WidgetFocus)
                        WidgetFocus->Update(true);
                }
                else if(Input::BUTTON_LIFTING == State)
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
                Input::ButtonState State = Input::InputManager::GetSingletonPtr()->GetSystemMouse()->GetButtonState(1);
                if(LastWidgetSelected && Input::BUTTON_LIFTING == State)
                    LastWidgetSelected = NULL;
            }
        }

        void UIManager::ClearButtonActivations()
        {
            if(ActivatedButtons.empty())
                return;
            for(ButtonIterator It = ActivatedButtons.begin() ; It != ActivatedButtons.end() ; It++ )
                (*It)->_SetActivation(false);
            ActivatedButtons.clear();
        }

        void UIManager::ViewportUpdateChecks()
        {
            for( Whole x=0 ; x < Screens.size() ; x++ )
                Screens[x]->CheckViewportSize();
        }

        void UIManager::MouseActivationCheck(Button* ToCheck)
        {
            if(!ToCheck)
                return;

            Input::Mouse* SysMouse = Input::InputManager::GetSingletonPtr()->GetSystemMouse();
            const std::vector<Input::InputCode>& MouseCodes = ToCheck->GetMouseActivationButtons();
            Input::ButtonState State = ToCheck->IsLiftActivation() ? Input::BUTTON_LIFTING : Input::BUTTON_PRESSING;

            for( Whole X = 0 ; X < MouseCodes.size() ; X++ )
            {
                Input::InputCode Code = MouseCodes.at(X);
                if( SysMouse->CheckButtonState(Code,State) )
                {
                    ToCheck->_SetActivation(true);
                    ActivatedButtons.push_back(ToCheck);
                }
            }
        }

        void UIManager::HotKeyActivationCheck(const Input::InputCode& Code)
        {
            if( Input::KEY_FIRST < Code && Input::KEY_LAST > Code )
            {
                std::pair<const HotKeyIterator,const HotKeyIterator> Result = HotKeys.equal_range(Code);
                if( (*Result.first).first != Code )
                    return;
                for( HotKeyIterator It = Result.first ; It != Result.second ; It++ )
                {
                    if((*It).second->IsVisible())
                        (*It).second->_SetActivation(true);
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Texture Atlas Management

        void UIManager::LoadMTA(const String& Name, const String& Group)
        {
            TextureAtlas* NewAtlas = new TextureAtlas(Name + ".mta",Group);
            Atlases[Name] = NewAtlas;
        }

        TextureAtlas* UIManager::GetAtlas(const String& AtlasName)
        {
            /// @todo These NULL returns should probably be exceptions.
            if("" == AtlasName)
            {
                AtlasIterator It = Atlases.begin();
                if(It != Atlases.end()) return (*It).second;
                else return NULL;
            }
            AtlasIterator It = Atlases.find(AtlasName);
            if(It != Atlases.end()) return (*It).second;
            else return NULL;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Screen Management

        Screen* UIManager::CreateScreen(const String& ScreenName, const String& Atlas, Graphics::Viewport* WindowViewport)
        {
            Screen* MezzScreen = new Screen(ScreenName, Atlas, WindowViewport);
            Screens.push_back(MezzScreen);

            VisibleScreenIterator It = VisibleScreens.find(WindowViewport);
            if( VisibleScreens.end() == It )
            {
                VisibleScreens.insert( std::pair<Graphics::Viewport*,Screen*>(WindowViewport,MezzScreen) );
            }else{
                MezzScreen->Visible = false;
            }

            return MezzScreen;
        }

        Screen* UIManager::GetScreen(const String& Name)
        {
            for( ScreenIterator it = Screens.begin() ; it != Screens.end() ; it++ )
            {
                if ( Name == (*it)->GetName() )
                {
                    Screen* Screen = (*it);
                    return Screen;
                }
            }
            return 0;
        }

        Screen* UIManager::GetScreen(const Whole& Index)
        {
            return Screens[Index];
        }

        Whole UIManager::GetNumScreens()
        {
            return Screens.size();
        }

        void UIManager::DestroyScreen(Screen* Screen)
        {
            if(Screens.empty())
                return;
            for( ScreenIterator it = Screens.begin() ; it != Screens.end() ; it++ )
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
            for( ScreenIterator it = Screens.begin() ; it != Screens.end() ; it++ )
            {
                delete (*it);
            }
            Screens.clear();
            return;
        }

        Screen* UIManager::GetVisibleScreenOnViewport(Graphics::Viewport* WindowViewport)
        {
            VisibleScreenIterator It = VisibleScreens.find(WindowViewport);
            if( VisibleScreens.end() != It ) return (*It).second;
            else return NULL;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // HotKey and Activation Management

        void UIManager::BindHotKey(const Input::InputCode& HotKey, Button* BoundButton)
        {
            if( Input::KEY_FIRST < HotKey && Input::KEY_LAST > HotKey )
                HotKeys.insert(std::pair<Input::InputCode,Button*>(HotKey,BoundButton));
        }

        void UIManager::UnbindHotKey(const Input::InputCode& HotKey, Button* BoundButton)
        {
            std::pair<const HotKeyIterator,const HotKeyIterator> Result = HotKeys.equal_range(HotKey);
            for( HotKeyIterator It = Result.first ; It != Result.second ; It++ )
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

        void UIManager::AddAutoRegisterCode(Input::InputCode Code)
        {
            for( CodeIterator It = AutoRegisterCodes.begin() ; It != AutoRegisterCodes.end() ; It++ )
            {
                if((*It)==Code)
                    return;
            }
            AutoRegisterCodes.push_back(Code);
        }

        void UIManager::RemoveAutoRegisterCode(Input::InputCode Code)
        {
            for( CodeIterator It = AutoRegisterCodes.begin() ; It != AutoRegisterCodes.end() ; It++ )
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

        UIManager::CodeContainer* UIManager::GetAutoRegisteredCodes()
        {
            return &AutoRegisterCodes;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Fetch Methods

        Widget* UIManager::GetHoveredWidget()
        {
            return HoveredWidget;
        }

        Widget* UIManager::GetWidgetFocus()
        {
            return WidgetFocus;
        }

        Widget* UIManager::GetWidgetCapturingInput()
        {
            return InputCapture;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void UIManager::RedrawAll(bool Force)
        {
            for( ScreenIterator it = Screens.begin() ; it != Screens.end() ; it++ )
            {
                (*it)->_RedrawAllIndexes(Force);
            }
        }

        Widget* UIManager::CheckWidgetMouseIsOver()
        {
            for( VisibleScreenIterator It = VisibleScreens.begin() ; It != VisibleScreens.end() ; ++It )
            {
                if( (*It).second )
                {
                    Widget* widget = (*It).second->CheckWidgetMouseIsOver();
                    if(widget)
                    {
                        return widget;
                    }
                }
            }
            return NULL;
        }

        bool UIManager::MouseIsInUISystem()
        {
            return (HoveredWidget || WidgetFocus);
        }

        UIManager::GlyphIndexResult UIManager::SuggestGlyphIndex(const Whole& Height, const String& Atlas)
        {
            TextureAtlas* TheAtlas = NULL;
            AtlasIterator It = Atlases.find(Atlas);
            if(It != Atlases.end()) TheAtlas = (*It).second;
            else
            {
                MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to access TextureAtlas \"" + Atlas + "\", which does not exist or is not loaded.");
            }
            std::map<UInt32,GlyphData*>& Glyphs = TheAtlas->GetGlyphs();
            Whole LargerMatch = 0;
            Whole SmallerMatch = 0;
            Real LargerHeight = 0;
            Real SmallerHeight = 0;
            Real LargerMatchDiff = 1000000.0;
            Real SmallerMatchDiff = 1000000.0;
            Real RequestedHeight = (Real)Height;

            for( std::map<UInt32,GlyphData*>::iterator it = Glyphs.begin() ; it != Glyphs.end() ; it++ )
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
                return GlyphIndexResult(LargerMatch,Scale);
            }else{
                Scale = RequestedHeight / SmallerHeight;
                return GlyphIndexResult(SmallerMatch,Scale);
            }
        }

        void UIManager::Initialize()
        {
            if( !this->Initialized )
            {
                this->TheEntresol->GetScheduler().AddWorkUnitMain( this->WidgetUpdateWork );
                Input::InputManager* InputMan = Input::InputManager::GetSingletonPtr();
                if( InputMan )
                    this->WidgetUpdateWork->AddDependency( InputMan->GetDeviceUpdateWork() );

                this->Initialized = true;
            }
        }

        void UIManager::Deinitialize()
        {
            if( this->Initialized )
            {
                this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->WidgetUpdateWork );
                this->WidgetUpdateWork->ClearDependencies();

                this->Initialized = false;
            }
        }

        WidgetUpdateWorkUnit* UIManager::GetWidgetUpdateWork()
        {
            return this->WidgetUpdateWork;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        ManagerBase::ManagerType UIManager::GetInterfaceType() const
            { return ManagerBase::UIManager; }

        String UIManager::GetImplementationTypeName() const
            { return "DefaultUIManager"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        void UIManager::_NotifyScreenVisibility(Screen* BeingShown, bool Visible)
        {
            VisibleScreenIterator It = VisibleScreens.find(BeingShown->GetViewport());
            if( VisibleScreens.end() != It )
            {
                if( (*It).second )
                    (*It).second->Visible = false;

                if(Visible) (*It).second = BeingShown;
                else (*It).second = NULL;
            }else{
                VisibleScreens.insert( std::pair<Graphics::Viewport*,Screen*>(BeingShown->GetViewport(), ( Visible ? BeingShown : NULL ) ) );
            }
        }

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

        ManagerBase* DefaultUIManagerFactory::CreateManager(XML::Node& XMLNode)
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
    }//UI
}//Mezzanine

#endif
