// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _inputinputmanager_cpp
#define _inputinputmanager_cpp

#include "Input/inputmanager.h"
#include "Input/mouse.h"
#include "Input/keyboard.h"
#include "Input/controller.h"
#include "Input/joystick.h"
#include "Input/metacodekey.h"

#include "entresol.h"
#include "timer.h"

#include "SDL.h"

#include <assert.h>

namespace Mezzanine
{
    template<> Input::InputManager* Singleton<Input::InputManager>::SingletonPtr = NULL;

    namespace Input
    {
        ///////////////////////////////////////////////////////////////////////////////
        // DeviceUpdateWorkUnit Methods

        DeviceUpdateWorkUnit::DeviceUpdateWorkUnit(const DeviceUpdateWorkUnit& Other)
            {  }

        DeviceUpdateWorkUnit& DeviceUpdateWorkUnit::operator=(const DeviceUpdateWorkUnit& Other)
            { return *this; }

        DeviceUpdateWorkUnit::DeviceUpdateWorkUnit(InputManager* Target) :
            TargetManager(Target) {  }

        DeviceUpdateWorkUnit::~DeviceUpdateWorkUnit()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void DeviceUpdateWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
        {
            this->TargetManager->ThreadResources = &CurrentThreadStorage;
            this->TargetManager->UpdateInputDevices();
            this->TargetManager->ThreadResources = nullptr;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // InputManager Methods

        using SubscriberType = InputManager::EventPublisherType::SubscriberType;

        const String InputManager::ImplementationName = "DefaultInputManager";
        const ManagerBase::ManagerType InputManager::InterfaceType = ManagerBase::MT_InputManager;

        const EventNameType InputManager::EventJoystickAdded = "JoystickAdded";
        const EventNameType InputManager::EventJoystickRemoved = "JoystickRemoved";
        const EventNameType InputManager::EventControllerAdded = "ControllerAdded";
        const EventNameType InputManager::EventControllerRemoved = "ControllerRemoved";
        const EventNameType InputManager::EventControllerRemapped = "ControllerRemapped";
        const EventNameType InputManager::EventKeymapChanged = "KeymapChanged";
        const EventNameType InputManager::EventClipboardUpdated = "ClipboardUpdated";

        InputManager::InputManager() :
            InputDeltas(8),
            SystemMouse(NULL),
            SystemKeyboard(NULL),
            DeviceUpdateWork(NULL),
            ThreadResources(NULL)
            { this->ConstructManager(); }

        InputManager::InputManager(const XML::Node& XMLNode) :
            InputDeltas(8),
            SystemMouse(NULL),
            SystemKeyboard(NULL),
            DeviceUpdateWork(NULL),
            ThreadResources(NULL)
            { this->ConstructManager(); }

        InputManager::~InputManager()
        {
            this->Deinitialize();

            delete this->DeviceUpdateWork;

            delete this->SystemMouse;
            delete this->SystemKeyboard;
            this->ReleaseAllDevices();
        }

        void InputManager::ConstructManager()
        {
            UInt32 InitSDLSystems = SDL_WasInit(0);
            if( (SDL_INIT_JOYSTICK & InitSDLSystems) == 0 ) {
                if( SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE) < 0 ) {
                    MEZZ_EXCEPTION(ExceptionBase::INTERNAL_EXCEPTION,String("Failed to Initialize SDL for Joystick input, SDL Error: ") + SDL_GetError());
                }
            }
            if( (SDL_INIT_GAMECONTROLLER & InitSDLSystems) == 0 ) {
                if( SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_NOPARACHUTE) < 0 ) {
                    MEZZ_EXCEPTION(ExceptionBase::INTERNAL_EXCEPTION,String("Failed to Initialize SDL for Game Controller input, SDL Error: ") + SDL_GetError());
                }
            }

            this->SystemMouse = new Mouse();
            this->SystemKeyboard = new Keyboard();
            this->DetectDevices();

            this->DeviceUpdateWork = new DeviceUpdateWorkUnit(this);

            this->InputPublisher.AddSubscriptionTable(InputManager::EventJoystickAdded);
            this->InputPublisher.AddSubscriptionTable(InputManager::EventJoystickRemoved);
            this->InputPublisher.AddSubscriptionTable(InputManager::EventControllerAdded);
            this->InputPublisher.AddSubscriptionTable(InputManager::EventControllerRemoved);
            this->InputPublisher.AddSubscriptionTable(InputManager::EventControllerRemapped);
            this->InputPublisher.AddSubscriptionTable(InputManager::EventClipboardUpdated);
        }

        Joystick* InputManager::ConstructJoystick(int Index)
        {
            SDL_Joystick* InternalStick = SDL_JoystickOpen( Index );
            Input::Joystick* NewStick = new Input::Joystick( InternalStick, Index );
            this->Joysticks.push_back( NewStick );
            return NewStick;
        }

        Controller* InputManager::ConstructController(int Index)
        {
            SDL_GameController* InternalControl = SDL_GameControllerOpen( Index );
            Input::Controller* NewController = new Input::Controller( InternalControl, Index );
            this->Controllers.push_back( NewController );
            return NewController;
        }

        void InputManager::PumpInternalEvents()
        {
            /* A listing of all the SDL_Events we care about in the Input System.
            // A * next to the Event denotes working support for that event does not yet exist.
            // A + next to the Event denotes that the input manager handles conversions for that Event instead of MetaCode.
            // A - next to the Event denotes that the event is disabled by Mezzanine.
            // Keyboard events
            SDL_KEYDOWN        = 0x300, // Key pressed
            SDL_KEYUP,                  // Key released
            *SDL_TEXTEDITING,           // Keyboard text editing (composition)
            SDL_TEXTINPUT,              // Keyboard text input
            *SDL_KEYMAPCHANGED,         // Keymap changed due to a system event such as an input language or keyboard layout change.
            // Mouse events
            SDL_MOUSEMOTION    = 0x400, // Mouse moved
            SDL_MOUSEBUTTONDOWN,        // Mouse button pressed
            SDL_MOUSEBUTTONUP,          // Mouse button released
            SDL_MOUSEWHEEL,             // Mouse wheel motion
            // Joystick events
            SDL_JOYAXISMOTION  = 0x600, // Joystick axis motion
            SDL_JOYBALLMOTION,          // Joystick trackball motion
            SDL_JOYHATMOTION,           // Joystick hat position change
            SDL_JOYBUTTONDOWN,          // Joystick button pressed
            SDL_JOYBUTTONUP,            // Joystick button released
            +SDL_JOYDEVICEADDED,        // A new joystick has been inserted into the system
            +SDL_JOYDEVICEREMOVED,      // An opened joystick has been removed
            // Game controller events
            *SDL_CONTROLLERAXISMOTION  = 0x650, // Game controller axis motion
            *SDL_CONTROLLERBUTTONDOWN,          // Game controller button pressed
            *SDL_CONTROLLERBUTTONUP,            // Game controller button released
            +SDL_CONTROLLERDEVICEADDED,         // A new Game controller has been inserted into the system
            +SDL_CONTROLLERDEVICEREMOVED,       // An opened Game controller has been removed
            +SDL_CONTROLLERDEVICEREMAPPED,      // The controller mapping was updated
            // Touch events
            *SDL_FINGERDOWN      = 0x700,
            *SDL_FINGERUP,
            *SDL_FINGERMOTION,
            // Gesture events
            *SDL_DOLLARGESTURE   = 0x800,
            *SDL_DOLLARRECORD,
            *SDL_MULTIGESTURE,
            // Clipboard events
            +SDL_CLIPBOARDUPDATE = 0x900, // The clipboard changed
            */

            // Make a fixed size array and grab our events.
            // Internally the event queue is capped at 128 events.  32 seems sane for this, but may need modifying.
            // SDL_KEYDOWN to SDL_MULTIGESTURE are all input events.  Everything in between those values, as well as those values, will be pulled.
            this->InputDeltas.clear();
            std::array<SDL_Event,32> InternalEvents;
            Integer NumEvents = SDL_PeepEvents(InternalEvents.data(),InternalEvents.size(),SDL_GETEVENT,SDL_KEYDOWN,SDL_MULTIGESTURE);
            assert( NumEvents >= 0 && SDL_GetError() );

            for( Integer CurrEv = 0 ; CurrEv < NumEvents ; ++CurrEv )
            {
                UInt32 EventType = InternalEvents[CurrEv].type;
                MetaCodeContainer ConvertedCodes;
                switch( EventType )
                {
                    case SDL_KEYDOWN:        case SDL_KEYUP:            case SDL_TEXTEDITING:    case SDL_TEXTINPUT:
                    case SDL_MOUSEMOTION:    case SDL_MOUSEBUTTONDOWN:  case SDL_MOUSEBUTTONUP:  case SDL_MOUSEWHEEL:
                    case SDL_JOYAXISMOTION:  case SDL_JOYBALLMOTION:    case SDL_JOYHATMOTION:   case SDL_JOYBUTTONDOWN:
                    case SDL_JOYBUTTONUP:
                    case SDL_CONTROLLERAXISMOTION:  case SDL_CONTROLLERBUTTONDOWN:  case SDL_CONTROLLERBUTTONUP:
                    {
                        ConvertedCodes = std::move( MetaCode::CreateMetaCodes( InternalEvents[CurrEv] ) );
                        for( MetaCode CurrCode : ConvertedCodes )
                        {
                            MetaCodeIterator InsertPos = std::lower_bound(this->InputDeltas.begin(),this->InputDeltas.end(),CurrCode,MultiDeviceCompare);
                            this->InputDeltas.insert(InsertPos,CurrCode);
                        }
                        break;
                    }
                    /*case SDL_KEYMAPCHANGED:
                    {
                        DeviceIDType RemapID = std::numeric_limits<DeviceIDType>::max();
                        DeviceEventPtr DeviceRemapped = std::make_shared<DeviceEvent>(EventKeymapChanged,RemapID);
                        this->InputPublisher.DispatchEvent(DeviceRemapped);
                        break;
                    }//*/
                    case SDL_JOYDEVICEADDED:
                    {
                        Joystick* Added = this->ConstructJoystick( InternalEvents[CurrEv].jdevice.which );
                        DeviceIDType AddID = Added->GetDeviceID();
                        DeviceEventPtr DeviceAdded = std::make_shared<DeviceEvent>(EventJoystickAdded,AddID);
                        this->InputPublisher.DispatchEvent(EventJoystickAdded,&SubscriberType::operator(),DeviceAdded);
                        break;
                    }
                    case SDL_JOYDEVICEREMOVED:
                    {
                        DeviceIDType RemoveID = InternalEvents[CurrEv].jdevice.which;
                        DeviceEventPtr DeviceRemoved = std::make_shared<DeviceEvent>(EventJoystickRemoved,RemoveID);
                        this->InputPublisher.DispatchEvent(EventJoystickRemoved,&SubscriberType::operator(),DeviceRemoved);
                        break;
                    }
                    case SDL_CONTROLLERDEVICEADDED:
                    {
                        Controller* Added = this->ConstructController( InternalEvents[CurrEv].cdevice.which );
                        DeviceIDType AddID = Added->GetDeviceID();
                        DeviceEventPtr DeviceAdded = std::make_shared<DeviceEvent>(EventControllerAdded,AddID);
                        this->InputPublisher.DispatchEvent(EventControllerAdded,&SubscriberType::operator(),DeviceAdded);
                        break;
                    }
                    case SDL_CONTROLLERDEVICEREMOVED:
                    {
                        DeviceIDType RemoveID = InternalEvents[CurrEv].cdevice.which;
                        DeviceEventPtr DeviceRemoved = std::make_shared<DeviceEvent>(EventControllerRemoved,RemoveID);
                        this->InputPublisher.DispatchEvent(EventControllerRemoved,&SubscriberType::operator(),DeviceRemoved);
                        break;
                    }
                    case SDL_CONTROLLERDEVICEREMAPPED:
                    {
                        DeviceIDType RemapID = InternalEvents[CurrEv].cdevice.which;
                        DeviceEventPtr DeviceRemapped = std::make_shared<DeviceEvent>(EventControllerRemapped,RemapID);
                        this->InputPublisher.DispatchEvent(EventControllerRemapped,&SubscriberType::operator(),DeviceRemapped);
                        break;
                    }
                    case SDL_FINGERDOWN:
                    case SDL_FINGERUP:
                    case SDL_FINGERMOTION:
                    {
                        // Not currently supported.  Do nothing.
                        break;
                    }
                    case SDL_DOLLARGESTURE:
                    case SDL_DOLLARRECORD:
                    case SDL_MULTIGESTURE:
                    {
                        // Not currently supported.  Do nothing.
                        break;
                    }
                    case SDL_CLIPBOARDUPDATE:
                    {
                        EventPtr ClipboardUpdated = std::make_shared<Event>(EventClipboardUpdated);
                        this->InputPublisher.DispatchEvent(EventClipboardUpdated,&SubscriberType::operator(),ClipboardUpdated);
                        break;
                    }
                    default: // Ignore the event.
                        { break; }
                }// switch event type
            }// for each event
        }

        MetaCodeContainer InputManager::UpdateKeyboard(MetaCodeIterator& UpdateBegin, const MetaCodeIterator RangeEnd)
        {
            MetaCodeIterator UpdateEnd = std::lower_bound(UpdateBegin,RangeEnd,MetaCode(0,Input::KEY_LAST),MultiDeviceCompare);
            MetaCodeContainer Ret = std::move( this->SystemKeyboard->_Update(UpdateBegin,UpdateEnd) );
            UpdateBegin = UpdateEnd;
            return Ret;
        }

        MetaCodeContainer InputManager::UpdateMouse(MetaCodeIterator& UpdateBegin, const MetaCodeIterator RangeEnd)
        {
            MetaCodeIterator UpdateEnd = std::lower_bound(UpdateBegin,RangeEnd,MetaCode(0,Input::MOUSE_LAST),MultiDeviceCompare);
            MetaCodeContainer Ret = std::move( this->SystemMouse->_Update(UpdateBegin,UpdateEnd) );
            UpdateBegin = UpdateEnd;
            return Ret;
        }

        MetaCodeContainer InputManager::UpdateJoysticks(MetaCodeIterator& UpdateBegin, const MetaCodeIterator RangeEnd)
        {
            MetaCodeContainer Ret;
            for( Whole X = 0 ; X < this->GetNumControllers() ; ++X )
            {
                MetaCodeIterator UpdateEnd = std::lower_bound(UpdateBegin,RangeEnd,MetaCode(0,Input::JOYSTICK_LAST,X),MultiDeviceCompare);
                MetaCodeContainer Temp = std::move( this->Joysticks[X]->_Update(UpdateBegin,UpdateEnd) );
                Ret.insert(Ret.end(),Temp.begin(),Temp.end());
                UpdateBegin = UpdateEnd;
            }
            return Ret;
        }

        MetaCodeContainer InputManager::UpdateControllers(MetaCodeIterator& UpdateBegin, const MetaCodeIterator RangeEnd)
        {
            MetaCodeContainer Ret;
            for( Whole X = 0 ; X < this->GetNumControllers() ; ++X )
            {
                MetaCodeIterator UpdateEnd = std::lower_bound(UpdateBegin,RangeEnd,MetaCode(0,Input::CONTROLLER_LAST,X),MultiDeviceCompare);
                MetaCodeContainer Temp = std::move( this->Controllers[X]->_Update(UpdateBegin,UpdateEnd) );
                Ret.insert(Ret.end(),Temp.begin(),Temp.end());
                UpdateBegin = UpdateEnd;
            }
            return Ret;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // InputDevice Management

        Mouse* InputManager::GetSystemMouse() const
            { return this->SystemMouse; }

        Keyboard* InputManager::GetSystemKeyboard() const
            { return this->SystemKeyboard; }

        Joystick* InputManager::GetJoystickByID(const DeviceIDType ID) const
        {
            for( Joystick* CurrStick : this->Joysticks )
            {
                if( CurrStick->GetDeviceID() == ID ) {
                    return CurrStick;
                }
            }
            return NULL;
        }

        Joystick* InputManager::GetJoystick(const UInt16 Index) const
            { return this->Joysticks.at(Index); }

        UInt16 InputManager::GetNumJoysticks() const
            { return this->Joysticks.size(); }

        Controller* InputManager::GetControllerByID(const DeviceIDType ID) const
        {
            for( Controller* CurrControl : this->Controllers )
            {
                if( CurrControl->GetDeviceID() == ID ) {
                    return CurrControl;
                }
            }
            return NULL;
        }

        Controller* InputManager::GetController(const UInt16 Index) const
            { return this->Controllers.at(Index); }

        UInt16 InputManager::GetNumControllers() const
            { return this->Controllers.size(); }

        ///////////////////////////////////////////////////////////////////////////////
        // InputDevice Detection

        UInt16 InputManager::DetectDevices()
        {
            UInt16 DeviceTotal = SDL_NumJoysticks();
            for( UInt16 DeviceIndex = 0 ; DeviceIndex < DeviceTotal ; ++DeviceIndex )
            {
                if( SDL_IsGameController( DeviceIndex ) ) {
                    this->ConstructController( DeviceIndex );
                }else{
                    this->ConstructJoystick( DeviceIndex );
                }
            }
            return DeviceTotal;
        }

        void InputManager::ReleaseAllDevices()
        {
            if( !this->Joysticks.empty() ) {
                for( Joystick* CurrStick : this->Joysticks )
                {
                    SDL_JoystickClose( CurrStick->_GetInternalDevice() );
                    delete CurrStick;
                }
                this->Joysticks.clear();
            }
            if( !this->Controllers.empty() ) {
                for( Controller* CurrControl : this->Controllers )
                {
                    SDL_GameControllerClose( CurrControl->_GetInternalDevice() );
                    delete CurrControl;
                }
                this->Controllers.clear();
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Sequenced Input Management

        void InputManager::AddInputSequence(const MetaCodeContainer& Codes, const Int32& SequenceID)
            { this->Sequences.AddInputSequence(Codes,SequenceID); }

        Boole InputManager::InputSequenceExists(const MetaCodeContainer& Codes)
            { return this->Sequences.InputSequenceExists(Codes); }

        Int32 InputManager::GetIDofInputSequence(const MetaCodeContainer& Codes)
            { return this->Sequences.GetIDofInputSequence(Codes); }

        void InputManager::RemoveInputSequence(const MetaCodeContainer& Codes)
            { this->Sequences.RemoveInputSequence(Codes); }

        void InputManager::RemoveAllInputSequences()
            { this->Sequences.RemoveAllInputSequences(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        void InputManager::UpdateInputDevices()
        {
            this->PumpInternalEvents();

            /// @todo We can't escape early here because devices have updates to perform
            /// regardless of whether there are new deltas.
            //if( this->InputDeltas.empty() ) {
            //    return;
            //}

            // Setup some containers.
            MetaCodeContainer GeneratedCodes(4);
            MetaCodeContainer TempCodes;
            // Setup some iterators.
            const MetaCodeIterator RangeEnd = this->InputDeltas.end();
            MetaCodeIterator UpdateBegin = std::lower_bound(this->InputDeltas.begin(),RangeEnd,MetaCode(0,Input::KEY_FIRST),MultiDeviceCompare);

            TempCodes = std::move( this->UpdateKeyboard(UpdateBegin,RangeEnd) );
            GeneratedCodes.insert(GeneratedCodes.end(),TempCodes.begin(),TempCodes.end());

            TempCodes = std::move( this->UpdateMouse(UpdateBegin,RangeEnd) );
            GeneratedCodes.insert(GeneratedCodes.end(),TempCodes.begin(),TempCodes.end());

            TempCodes = std::move( this->UpdateJoysticks(UpdateBegin,RangeEnd) );
            GeneratedCodes.insert(GeneratedCodes.end(),TempCodes.begin(),TempCodes.end());

            TempCodes = std::move( this->UpdateControllers(UpdateBegin,RangeEnd) );
            GeneratedCodes.insert(GeneratedCodes.end(),TempCodes.begin(),TempCodes.end());

            // Do sub-system wide sequence checks if we've done anything
            if( !this->InputDeltas.empty() )
                this->Sequences.DetectSequence(this->InputDeltas.begin(),RangeEnd);
            // Update our delta's if there is anything to update
            if( !GeneratedCodes.empty() )
                this->InputDeltas.insert(this->InputDeltas.end(),GeneratedCodes.begin(),GeneratedCodes.end());
        }

        const MetaCodeContainer& InputManager::GetInputDeltas() const
            { return this->InputDeltas; }

        void InputManager::Initialize()
        {
            if( !this->Initialized ) {
                this->TheEntresol->GetScheduler().AddWorkUnitMain( this->DeviceUpdateWork, "DeviceUpdateWork" );

                this->Initialized = true;
            }
        }

        void InputManager::Deinitialize()
        {
            if( this->Initialized ) {
                this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->DeviceUpdateWork );
                this->DeviceUpdateWork->ClearDependencies();

                this->Initialized = false;
            }
        }

        DeviceUpdateWorkUnit* InputManager::GetDeviceUpdateWork()
            { return this->DeviceUpdateWork; }

        InputManager::EventPublisherType& InputManager::GetInputPublisher()
            { return this->InputPublisher; }

        const InputManager::EventPublisherType& InputManager::GetInputPublisher() const
            { return this->InputPublisher; }

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        ManagerBase::ManagerType InputManager::GetInterfaceType() const
            { return InputManager::InterfaceType; }

        String InputManager::GetImplementationTypeName() const
            { return InputManager::ImplementationName; }

        ///////////////////////////////////////////////////////////////////////////////
        // DefaultInputManagerFactory Methods

        DefaultInputManagerFactory::DefaultInputManagerFactory()
            {  }

        DefaultInputManagerFactory::~DefaultInputManagerFactory()
            {  }

        String DefaultInputManagerFactory::GetManagerImplName() const
            { return InputManager::ImplementationName; }

        ManagerBase::ManagerType DefaultInputManagerFactory::GetManagerType() const
            { return InputManager::InterfaceType; }

        EntresolManager* DefaultInputManagerFactory::CreateManager(const NameValuePairList& Params)
        {
            if( InputManager::SingletonValid() ) {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return InputManager::GetSingletonPtr();
            }else return new InputManager();
        }

        EntresolManager* DefaultInputManagerFactory::CreateManager(const XML::Node& XMLNode)
        {
            if( InputManager::SingletonValid() ) {
                /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
                return InputManager::GetSingletonPtr();
            }else return new InputManager(XMLNode);
        }

        void DefaultInputManagerFactory::DestroyManager(EntresolManager* ToBeDestroyed)
            { delete ToBeDestroyed; }
    }//Input
}//Mezzanine

#endif
