// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#include "UI/textureatlashandler.h"
#include "UI/textureatlas.h"
#include "UI/hotkeyhandler.h"
#include "UI/actionhandler.h"
#include "UI/screen.h"
#include "UI/button.h"
#include "UI/widget.h"
#include "UI/glyph.h"

// includes for the creation and destruction of the global parser
#include "UI/defaultmarkupparser.h"

#include "Graphics/graphicsmanager.h"
#include "Graphics/viewport.h"
#include "Input/inputmanager.h"
#include "Input/mouse.h"
#include "Resource/resourcemanager.h"
#include "eventmanager.h"

#include "MathTools/mathtools.h"
#include "timer.h"
#include "entresol.h"

#include <Ogre.h>

namespace Mezzanine
{
    template<> UI::UIManager* Singleton<UI::UIManager>::SingletonPtr = NULL;

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
            { this->TargetManager->UpdateScreens(); }

        ///////////////////////////////////////////////////////////////////////////////
        // UIManager Methods

        const String UIManager::ImplementationName = "DefaultUIManager";
        const ManagerBase::ManagerType UIManager::InterfaceType = ManagerBase::MT_UIManager;

        UIManager::UIManager() :
            HoveredWidget(NULL),
            WidgetFocus(NULL),

            WidgetUpdateWork(NULL),
            ThreadResources(NULL),

            ButtonAutoRegister(false),
            MouseMoved(false)
        {
            Resource::ResourceManager::GetSingletonPtr()->CreateAssetGroup("UI");
            this->AtlasHandler = new TextureAtlasHandler();
            this->HKHandler = new HotKeyHandler();
            //this->ActHandler = new ActionHandler();

            // Parser Work
            MarkupParser* DefaultParser = new DefaultMarkupParser();
            this->RegisterMarkupParser(DefaultParser->GetName(),DefaultParser);
            this->RegisterMarkupParser("",DefaultParser);

            this->WidgetUpdateWork = new WidgetUpdateWorkUnit(this);
        }

        UIManager::UIManager(const XML::Node& XMLNode) :
            HoveredWidget(NULL),
            WidgetFocus(NULL),

            WidgetUpdateWork(NULL),
            ThreadResources(NULL),

            ButtonAutoRegister(false),
            MouseMoved(false)
        {
            /// @todo This class currently doesn't initialize anything from XML, if that changes this constructor needs to be expanded.
            Resource::ResourceManager::GetSingletonPtr()->CreateAssetGroup("UI");
            this->AtlasHandler = new TextureAtlasHandler();
            this->HKHandler = new HotKeyHandler();
            //this->ActHandler = new ActionHandler();

            // Parser Work
            MarkupParser* DefaultParser = new DefaultMarkupParser();
            this->RegisterMarkupParser(DefaultParser->GetName(),DefaultParser);
            this->RegisterMarkupParser("",DefaultParser);

            this->WidgetUpdateWork = new WidgetUpdateWorkUnit(this);
        }

        UIManager::~UIManager()
        {
            this->Deinitialize();
            this->DestroyAllScreens();
            this->DestroyAllMarkupParsers();

            delete this->AtlasHandler;
            this->AtlasHandler = NULL;

            delete this->HKHandler;
            this->HKHandler = NULL;

            delete this->WidgetUpdateWork;
            this->WidgetUpdateWork = NULL;
        }

        void UIManager::HandlePreFocusInput(const Input::MetaCode& Code)
        {
            if( Code.IsKeyboardEvent() )
                this->HandlePreFocusKeyboardInput( Code );
            else if( Code.IsMouseEvent() )
                this->HandlePreFocusMouseInput( Code );
            else if( Code.IsControllerEvent() )
                this->HandlePreFocusControllerInput( Code );
            /// @todo If other devices are added, appropriate functions should be added to this class, and called here.
        }

        void UIManager::HandlePostFocusInput(const Input::MetaCode& Code)
        {
            if( Code.IsKeyboardEvent() )
                this->HandlePostFocusKeyboardInput( Code );
            else if( Code.IsMouseEvent() )
                this->HandlePostFocusMouseInput( Code );
            else if( Code.IsControllerEvent() )
                this->HandlePostFocusControllerInput( Code );
            /// @todo If other devices are added, appropriate functions should be added to this class, and called here.
        }

        void UIManager::HandlePreFocusKeyboardInput(const Input::MetaCode& Code)
        {

        }

        void UIManager::HandlePreFocusMouseInput(const Input::MetaCode& Code)
        {
            // Perform our simple checks for the focus switching/locking
            if( Code.IsMouseButton() ) {
                if( Input::BUTTON_PRESSING == Code.GetMetaValue() ) {
                    this->SwitchFocus(HoveredWidget);

                    // If we got a valid focus, there is more work to be done.
                    if( this->WidgetFocus ) {
                        // Regardless of it has changed, lock the focus.
                        if( !this->FocusIsLocked() ) {
                            this->WidgetFocus->_OnFocusLocked();
                            this->FocusLockCode = Code;
                        }
                    }
                }else if( Input::BUTTON_LIFTING == Code.GetMetaValue() ) {
                    if( this->WidgetFocus ) {
                        // Check the code to see if we're releasing the focus lock.
                        if( this->FocusLockCode.GetCode() == Code.GetCode() && this->FocusLockCode.GetDeviceIndex() == Code.GetDeviceIndex() ) {
                            this->WidgetFocus->_OnFocusUnlocked();
                            this->WidgetFocus->_OnMouseDragEnd();
                            this->FocusLockCode.SetNullValues();
                        }
                    }
                }
            }

            // Perform the check for dragging
            if( this->FocusIsLocked() && this->MouseMoved ) {
                if( !this->WidgetFocus->IsDragged() ) {
                    this->WidgetFocus->_OnMouseDragStart();
                }
            }
        }

        void UIManager::HandlePreFocusControllerInput(const Input::MetaCode& Code)
        {
            /// @todo There is currently nothing that the UI system as a whole needs controller inputs.
            /// Perhaps when we implement consoles this should be expanded, maybe even sooner then that.
        }

        void UIManager::HandlePostFocusKeyboardInput(const Input::MetaCode& Code)
        {

        }

        void UIManager::HandlePostFocusMouseInput(const Input::MetaCode& Code)
        {
            if( Code.IsMouseMotionEvent() )
            {

            }
        }

        void UIManager::HandlePostFocusControllerInput(const Input::MetaCode& Code)
        {
            /// @todo There is currently nothing that the UI system as a whole needs controller inputs.
            /// Perhaps when we implement consoles this should be expanded, maybe even sooner then that.
        }

        Boole UIManager::SwitchFocus(Widget* NewFocus)
        {
            // Check if the focus is changing at all
            if( this->WidgetFocus != NewFocus && !this->FocusIsLocked() )
            {
                // If we already have a focus, drop it
                if( this->WidgetFocus )
                    this->WidgetFocus->_OnFocusLost();

                // Assign the new focus
                this->WidgetFocus = NewFocus;

                // If the new focus is valid, notify it of it's new status
                if( this->WidgetFocus )
                    this->WidgetFocus->_OnFocusGained();

                // Acknowledge the focus was changed
                return true;
            }
            // Nothing changed
            return false;
        }

        Boole UIManager::FocusIsLocked() const
        {
            return ( Input::KEY_UNKNOWN != FocusLockCode.GetCode() );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Texture Atlas Management

        void UIManager::LoadMTA(const String& Name, const String& Group)
            { this->AtlasHandler->LoadAtlasFromFile(Name,Group); }

        TextureAtlas* UIManager::GetAtlas(const String& AtlasName)
            { return this->AtlasHandler->GetAtlas(AtlasName); }

        TextureAtlas* UIManager::GetAtlasExcept(const String& AtlasName)
            { return this->AtlasHandler->GetAtlasExcept(AtlasName); }

        void UIManager::DestroyAtlas(TextureAtlas* ToBeDestroyed)
            { this->AtlasHandler->DestroyAtlas(ToBeDestroyed); }

        void UIManager::DestroyAllAtlases()
            { this->AtlasHandler->DestroyAllAtlases(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Screen Management

        Screen* UIManager::CreateScreen(const String& ScreenName, const String& Atlas, Graphics::Viewport* WindowViewport, const UInt16 ZOrder)
        {
            Screen* MezzScreen = new Screen(ScreenName,Atlas,WindowViewport,this);
            MezzScreen->_SetZOrder(ZOrder);

            for( ScreenIterator ScreenIt = this->Screens.begin() ; ScreenIt != this->Screens.end() ; ++ScreenIt )
            {
                if( (*ScreenIt)->GetZOrder() > ZOrder ) {
                    this->Screens.insert(ScreenIt,MezzScreen);
                    return MezzScreen;
                }
            }
            this->Screens.push_back(MezzScreen);
            return MezzScreen;
        }

        Screen* UIManager::GetScreen(const String& Name) const
        {
            for( ConstScreenIterator it = this->Screens.begin() ; it != this->Screens.end() ; it++ )
            {
                if ( Name == (*it)->GetName() ) {
                    Screen* MezzScreen = (*it);
                    return MezzScreen;
                }
            }
            return 0;
        }

        Screen* UIManager::GetScreen(const Whole& Index) const
        {
            return this->Screens[Index];
        }

        Whole UIManager::GetNumScreens() const
        {
            return this->Screens.size();
        }

        void UIManager::DestroyScreen(Screen* Screen)
        {
            for( ScreenIterator it = this->Screens.begin() ; it != this->Screens.end() ; it++ )
            {
                if( Screen == (*it) ) {
                    delete (*it);
                    this->Screens.erase(it);
                    return;
                }
            }
        }

        void UIManager::DestroyAllScreens()
        {
            for( ScreenIterator it = this->Screens.begin() ; it != this->Screens.end() ; it++ )
            {
                delete (*it);
            }
            this->Screens.clear();
        }

        void UIManager::ShowScreensOnViewport(Graphics::Viewport* WindowViewport, Screen* Exclude)
        {
            for( ScreenIterator ScreenIt = this->Screens.begin() ; ScreenIt != this->Screens.end() ; ++ScreenIt )
            {
                if( (*ScreenIt)->GetViewport() == WindowViewport && (*ScreenIt) != Exclude ) {
                    (*ScreenIt)->Show();
                }
            }
        }

        void UIManager::ShowAllScreens(Screen* Exclude)
        {
            for( ScreenIterator ScreenIt = this->Screens.begin() ; ScreenIt != this->Screens.end() ; ++ScreenIt )
            {
                if( (*ScreenIt) != Exclude ) {
                    (*ScreenIt)->Show();
                }
            }
        }

        void UIManager::HideScreensOnViewport(Graphics::Viewport* WindowViewport, Screen* Exclude)
        {
            for( ScreenIterator ScreenIt = this->Screens.begin() ; ScreenIt != this->Screens.end() ; ++ScreenIt )
            {
                if( (*ScreenIt)->GetViewport() == WindowViewport && (*ScreenIt) != Exclude ) {
                    (*ScreenIt)->Hide();
                }
            }
        }

        void UIManager::HideAllScreens(Screen* Exclude)
        {
            for( ScreenIterator ScreenIt = this->Screens.begin() ; ScreenIt != this->Screens.end() ; ++ScreenIt )
            {
                if( (*ScreenIt) != Exclude ) {
                    (*ScreenIt)->Hide();
                }
            }
        }

        Screen* UIManager::GetLowestVisibleScreenOnViewport(Graphics::Viewport* WindowViewport) const
        {
            for( ConstScreenIterator ScreenIt = this->Screens.begin() ; ScreenIt != this->Screens.end() ; ++ScreenIt )
            {
                if( (*ScreenIt)->GetViewport() == WindowViewport && (*ScreenIt)->IsVisible() ) {
                    return (*ScreenIt);
                }
            }
            return NULL;
        }

        Screen* UIManager::GetHighestVisibleScreenOnViewport(Graphics::Viewport* WindowViewport) const
        {
            for( ConstReverseScreenIterator ScreenIt = this->Screens.rbegin() ; ScreenIt != this->Screens.rend() ; ++ScreenIt )
            {
                if( (*ScreenIt)->GetViewport() == WindowViewport && (*ScreenIt)->IsVisible() ) {
                    return (*ScreenIt);
                }
            }
            return NULL;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // HotKey and Activation Management

        void UIManager::BindHotKey(const Input::MetaCode& HotKey, Button* BoundButton)
        {
            this->HKHandler->BindHotKey(HotKey,BoundButton);
        }

        void UIManager::UnbindHotKey(const Input::MetaCode& HotKey, Button* BoundButton)
        {
            this->HKHandler->UnbindHotKey(HotKey,BoundButton);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Activation Management

        void UIManager::EnableButtonAutoRegister(Boole Enable)
            { this->ButtonAutoRegister = Enable; }

        Boole UIManager::ButtonAutoRegisterEnabled() const
            { return this->ButtonAutoRegister; }

        void UIManager::AddAutoRegisterCode(const Input::MetaCode& Code)
        {
            for( InputIterator It = this->AutoRegisterCodes.begin() ; It != this->AutoRegisterCodes.end() ; It++ )
            {
                if( (*It) == Code )
                    return;
            }
            this->AutoRegisterCodes.push_back(Code);
        }

        void UIManager::RemoveAutoRegisterCode(const Input::MetaCode& Code)
        {
            for( InputIterator It = this->AutoRegisterCodes.begin() ; It != this->AutoRegisterCodes.end() ; It++ )
            {
                if( (*It) == Code ) {
                    this->AutoRegisterCodes.erase(It);
                    return;
                }
            }
        }

        void UIManager::RemoveAllAutoRegisterCodes()
            { this->AutoRegisterCodes.clear(); }

        const UIManager::InputContainer& UIManager::GetAutoRegisteredCodes() const
            { return this->AutoRegisterCodes; }

        ///////////////////////////////////////////////////////////////////////////////
        // MarkupParser Management

        void UIManager::RegisterMarkupParser(const String& ParserName, MarkupParser* ToAdd)
        {
            MarkupParserIterator MarkupIt = this->MarkupParsers.find(ParserName);
            if( MarkupIt != this->MarkupParsers.end() ) {
                MEZZ_EXCEPTION(ExceptionBase::II_DUPLICATE_IDENTITY_EXCEPTION,"A MarkupParser with the name \"" + ParserName + "\" is already registered.");
            }else{
                this->MarkupParsers.insert( std::pair<String,UI::MarkupParser*>(ParserName,ToAdd) );
            }
        }

        Boole UIManager::IsMarkupParserRegistered(const String& ParserName) const
        {
            ConstMarkupParserIterator MarkupIt = this->MarkupParsers.find(ParserName);
            return ( MarkupIt != this->MarkupParsers.end() );
        }

        MarkupParser* UIManager::GetMarkupParser(const String& ParserName) const
        {
            ConstMarkupParserIterator MarkupIt = this->MarkupParsers.find(ParserName);
            if( MarkupIt != this->MarkupParsers.end() ) return (*MarkupIt).second;
            else return NULL;
        }

        void UIManager::UnregisterMarkupParser(const String& ParserName)
        {
            MarkupParserIterator MarkupIt = this->MarkupParsers.find(ParserName);
            if( MarkupIt != this->MarkupParsers.end() )
                this->MarkupParsers.erase(MarkupIt);
        }

        void UIManager::UnregisterAllMarkupParsers()
        {
            this->MarkupParsers.clear();
        }

        void UIManager::DestroyMarkupParser(const String& ParserName)
        {
            MarkupParserIterator MarkupIt = this->MarkupParsers.find(ParserName);
            if( MarkupIt != this->MarkupParsers.end() ) {
                delete (*MarkupIt).second;
                this->MarkupParsers.erase(MarkupIt);
            }
        }

        void UIManager::DestroyAllMarkupParsers()
        {
            // Since we inserted the default parser into this container under two names, we need to remove one to prevent a double delete.
            this->UnregisterMarkupParser("");
            for( MarkupParserIterator MarkupIt = this->MarkupParsers.begin() ; MarkupIt != this->MarkupParsers.end() ; ++MarkupIt )
                { delete (*MarkupIt).second; }
            this->MarkupParsers.clear();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Fetch Methods

        Widget* UIManager::GetHoveredWidget() const
            { return this->HoveredWidget; }

        Widget* UIManager::GetWidgetFocus() const
            { return this->WidgetFocus; }

        TextureAtlasHandler* UIManager::GetAtlasHandler() const
            { return this->AtlasHandler; }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void UIManager::InjectInput(const Input::MetaCode& Code)
        {
            switch( Code.GetCode() )
            {
                case Input::MOUSEHORIZONTAL:
                case Input::MOUSEVERTICAL:
                {
                    this->MouseMoved = true;
                    break;
                }
                default:  /* Do Nothing */  break;
            }

            this->InjectedInputs.push_back(Code);
        }

        Widget* UIManager::CheckWidgetUnderPoint(Graphics::Viewport* VP, const Vector2& Point)
        {
            for( ReverseScreenIterator ScreenIt = this->Screens.rbegin() ; ScreenIt != this->Screens.rend() ; ++ScreenIt )
            {
                Widget* Result = (*ScreenIt)->FindHoveredWidget(Point);
                if( Result != NULL ) {
                    return Result;
                }
            }
            return NULL;
        }

        Boole UIManager::MouseIsInUISystem() const
        {
            return (HoveredWidget || WidgetFocus);
        }

        UIManager::FontResult UIManager::SuggestGlyphIndex(const Whole& Height, const String& Atlas)
        {
            TextureAtlas* TheAtlas = AtlasHandler->GetAtlas(Atlas);
            if(NULL == TheAtlas)
            {
                MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Attempting to access TextureAtlas \"" + Atlas + "\", which does not exist or is not loaded.");
            }
            TextureAtlas::FontDataContainer& Fonts = TheAtlas->GetFonts();
            String LargerMatch, SmallerMatch;
            Real LargerHeight = 0;
            Real SmallerHeight = 0;
            Real LargerMatchDiff = 1000000.0;
            Real SmallerMatchDiff = 1000000.0;
            Real RequestedHeight = (Real)Height;

            for( TextureAtlas::FontDataIterator it = Fonts.begin() ; it != Fonts.end() ; it++ )
            {
                Real Diff = 0.0;
                if((*it).second->GetLineHeight() > RequestedHeight)
                {
                    Diff = (*it).second->GetLineHeight() - RequestedHeight;
                    if(Diff < LargerMatchDiff)
                    {
                        LargerMatch = (*it).first;
                        LargerHeight = (*it).second->GetLineHeight();
                        LargerMatchDiff = Diff;
                        continue;
                    }
                }else{
                    Diff = RequestedHeight - (*it).second->GetLineHeight();
                    if(Diff < SmallerMatchDiff)
                    {
                        SmallerMatch = (*it).first;
                        SmallerHeight = (*it).second->GetLineHeight();
                        SmallerMatchDiff = Diff;
                        continue;
                    }
                }
            }

            Real Scale = 1;
            if(!LargerMatch.empty())
            {
                Scale = RequestedHeight / LargerHeight;
                return FontResult(LargerMatch,Scale);
            }else{
                Scale = RequestedHeight / SmallerHeight;
                return FontResult(SmallerMatch,Scale);
            }
        }

        void UIManager::UpdateScreens()
        {
            Input::Mouse* SysMouse = Input::InputManager::GetSingletonPtr()->GetSystemMouse();

            // Tell all screens to check if the veiwport dimensions have changed
            for( ScreenIterator ScIt = this->Screens.begin() ; ScIt != this->Screens.end() ; ++ScIt )
                (*ScIt)->CheckViewportSize();

            // Inject input delta's from the input system
            const MetaCodeContainer& Inputs = Input::InputManager::GetSingletonPtr()->GetInputDeltas();
            // InjectedInputs.insert(InjectedInputs.end(),Inputs.begin(),Inputs.end());  // Commented because we want to use "InjectInput()" so we can filter the codes.
            for( ConstMetaCodeIterator MCIt = Inputs.begin() ; MCIt != Inputs.end() ; ++MCIt )
                this->InjectInput( (*MCIt) );

            // Get the hovered quad based on mouse position
            // We don't care what is hovered so much if the mouse hasn't moved, or is hidden due to relative mode.
            if( this->MouseMoved && !SysMouse->GetRelativeMode() ) {
                Widget* TempWidget = this->CheckWidgetUnderPoint(SysMouse->GetHoveredViewport(),SysMouse->GetViewportPosition());
                if( TempWidget != this->HoveredWidget )
                {
                    if( this->HoveredWidget ) {
                        this->HoveredWidget->_OnMouseExit();
                    }

                    this->HoveredWidget = TempWidget;

                    if( this->HoveredWidget ) {
                        this->HoveredWidget->_OnMouseEnter();
                    }
                }
            }

            // Check the focus to see if it's still visible.  If it isn't, attempt to pass focus to the next available parent.
            if( this->WidgetFocus && !this->WidgetFocus->GetVisible() ) {
                // Clear the focus lock
                if( this->FocusIsLocked() ) {
                    this->WidgetFocus->_OnFocusUnlocked();
                    this->FocusLockCode.SetNullValues();
                }

                // Find the next available parent
                Widget* PassedFocus = this->WidgetFocus;
                do{
                    PassedFocus = ( PassedFocus->GetParent()->IsWidget() ? static_cast<Widget*>(PassedFocus->GetParent()) : NULL );
                }while( PassedFocus != NULL && !PassedFocus->GetVisible() );

                // Switch the focus, even if it is NULL
                this->SwitchFocus(PassedFocus);
            }

            // Look at and process all injected inputs accordingly
            for( InputIterator InIt = this->InjectedInputs.begin() ; InIt != this->InjectedInputs.end() ; ++InIt )
            {
                // Allow our pre processes to get first dibs.
                this->HandlePreFocusInput( (*InIt) );

                // Allow the focus to consume the input if it needs/wants to.
                if( this->WidgetFocus && this->WidgetFocus->_HandleInput( (*InIt) ) )
                    continue;

                // If the focus doesn't consume the input, we pass it back to the system.
                this->HandlePostFocusInput( (*InIt) );
            }
            this->MouseMoved = false;
            this->InjectedInputs.clear();
        }

        void UIManager::Initialize()
        {
            if( !this->Initialized ) {
                this->TheEntresol->GetScheduler().AddWorkUnitMain( this->WidgetUpdateWork, "WidgetUpdateWork" );
                Input::InputManager* InputMan = Input::InputManager::GetSingletonPtr();
                if( InputMan )
                    this->WidgetUpdateWork->AddDependency( InputMan->GetDeviceUpdateWork() );

                this->Initialized = true;
            }
        }

        void UIManager::Deinitialize()
        {
            if( this->Initialized ) {
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
            { return UIManager::InterfaceType; }

        String UIManager::GetImplementationTypeName() const
            { return UIManager::ImplementationName; }

        ///////////////////////////////////////////////////////////////////////////////
        // DefaultUIManagerFactory Methods

        DefaultUIManagerFactory::DefaultUIManagerFactory()
            {  }

        DefaultUIManagerFactory::~DefaultUIManagerFactory()
            {  }

        String DefaultUIManagerFactory::GetManagerImplName() const
            { return UIManager::ImplementationName; }

        ManagerBase::ManagerType DefaultUIManagerFactory::GetManagerType() const
            { return UIManager::InterfaceType; }

        EntresolManager* DefaultUIManagerFactory::CreateManager(const NameValuePairList& Params)
        {
            if( UIManager::SingletonValid() ) {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return UIManager::GetSingletonPtr();
            }else return new UIManager();
        }

        EntresolManager* DefaultUIManagerFactory::CreateManager(const XML::Node& XMLNode)
        {
            if( UIManager::SingletonValid() ) {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return UIManager::GetSingletonPtr();
            }else return new UIManager(XMLNode);
        }

        void DefaultUIManagerFactory::DestroyManager(EntresolManager* ToBeDestroyed)
            { delete ToBeDestroyed; }
    }//UI
}//Mezzanine

#endif
