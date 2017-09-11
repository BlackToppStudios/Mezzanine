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
#ifndef _inputinputmanager_h
#define _inputinputmanager_h

#include "entresolmanager.h"
#include "entresolmanagerfactory.h"
#include "singleton.h"
#include "eventpublisher.h"
#include "Input/metacode.h"
#include "Input/sequencecontainer.h"
#include "Input/keyboard.h"
#include "Input/mouse.h"
#include "Threading/workunit.h"

namespace Mezzanine
{
    namespace Input
    {
        class InputManager;
        class Joystick;
        class Controller;

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the base Event class for input devices.
        ///////////////////////////////////////
        class MEZZ_LIB DeviceEvent : public Event
        {
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Public Data Members

            /// @brief The ID of the device that was connected or disconnected.
            DeviceIDType EventDevice;

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            /// @brief Class constructor.
            /// @param Name The name of the event being fired.
            /// @param Device The ID of the device that was connected or disconnected.
            DeviceEvent(const EventNameType& Name, const DeviceIDType Device) :
                Event(Name), EventDevice(Device)
                {  }
            /// @brief Class destructor.
            virtual ~DeviceEvent() = default;
        };//DeviceEvent

        /// @brief Convenience type for passing around DeviceEvent.
        using DeviceEventPtr = std::shared_ptr<DeviceEvent>;

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a Mezzanine::Threading::iWorkUnit for the updating of input device state from the OS.
        ///////////////////////////////////////
        class MEZZ_LIB DeviceUpdateWorkUnit : public Threading::DefaultWorkUnit
        {
        protected:
            /// @brief A pointer to the manager this work unit is processing.
            InputManager* TargetManager;
            /// @brief Protected copy constructor.  THIS IS NOT ALLOWED.
            /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
            DeviceUpdateWorkUnit(const DeviceUpdateWorkUnit& Other);
            /// @brief Protected assignment operator.  THIS IS NOT ALLOWED.
            /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
            DeviceUpdateWorkUnit& operator=(const DeviceUpdateWorkUnit& Other);
        public:
            /// @brief Class constructor.
            /// @param Target The InputManager this work unit will process during the frame.
            DeviceUpdateWorkUnit(InputManager* Target);
            /// @brief Class destructor.
            virtual ~DeviceUpdateWorkUnit();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief This does any required update of the input devices detected on this system.
            /// @param CurrentThreadStorage The storage class for all resources owned by this work unit during it's execution.
            virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
        };//DeviceUpdateWorkUnit

        // Used by the scripting language binder to help create bindings for this class. SWIG does know to creation template instances
        #ifdef SWIG
        %template(SingletonInputManager) Singleton<InputManager>;
        #endif

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the manager responsible for the handling of input devices and events.
        /// @details Inputs can be checked one of two ways in the input system.  Either you can get the class instance
        /// for the device you want to query and use the query methods on it to check it's current state, or you can
        /// access the inputs that were raised for the current frame by getting the container storing the Input Delta's. @n @n
        /// Another feature of the input system is the ability to set custom input sequences that can be raised via MetaCode
        /// for use elsewhere (such as the UI system).  These sequences can be any number of MetaCode's of any type (that is
        /// valid) in any order.  Any sequence passed in must be terminated with a "Null" MetaCode.  A "Null" MetaCode is a
        /// MetaCode constructed using the default constructor, having it's Input code set to "KEY_UNKNOWN", it's meta value
        /// to 0, and it's device index set to the max value of UInt16.  If the sequence is not terminated an exception will
        /// be thrown. @n @n
        /// Input Sequences can be stored on the manager, or any input device, and their behavior is different based on where
        /// it is inserted.  Input Sequences stored on the manager will look at all of the most recent inputs from all devices
        /// and use that to compare against the stored Input Sequences.  If you have a single player game on the PC and want to
        /// use an Input Sequence that combines input from both the mouse and keyboard, then the InputManager is the place to
        /// store it.  If however, you have a multiplayer fighter game (or split screen shooter) and want to track inputs from
        /// each of two or more controllers individually, then you'll want to place the Input Sequences on the proper controller
        /// device.  @n @n
        /// The InputManager can take any sequence, provided it is terminated.  Input devices however can only take sequences with
        /// MetaCode's that pertain to their own device, otherwise an exception will be thrown.  At each place of storage Input
        /// Sequences are forced to be unique(but they can exist on multiple devices).  Input Sequence ID's however are not forced
        /// to be unique at any point in the system.  Furthermore when a MetaCode is generated after an Input Sequence occurs, it
        /// is only given the ID the sequence, but not which sequence generated it.  In the case of controllers they will be given
        /// a device ID, but otherwise the origin will not be reported.  This allows you to provide multiple ways to generating
        /// the appropriate MetaCode(which could trigger something else in the UI system), but this also means great care must be
        /// taken when deciding on the ID's for each Input Sequence.
        ///////////////////////////////////////
        class MEZZ_LIB InputManager : public EntresolManager, public Singleton<InputManager>
        {
        public:
            /// @brief Basic container type for game Joystick instances.
            using JoystickContainer = std::vector< Joystick* >;
            /// @brief Iterator type for game Joystick instances stored by this class.
            using JoystickIterator = JoystickContainer::iterator;
            /// @brief Const Iterator type for game Joystick instances stored by this class.
            using ConstJoystickIterator = JoystickContainer::const_iterator;
            /// @brief Basic container type for game Controller instances.
            using ControllerContainer = std::vector< Controller* >;
            /// @brief Iterator type for game Controller instances stored by this class.
            using ControllerIterator = ControllerContainer::iterator;
            /// @brief Const Iterator type for game Controller instances stored by this class.
            using ConstControllerIterator = ControllerContainer::const_iterator;

            /// @brief A String containing the name of this manager implementation.
            static const String ImplementationName;
            /// @brief A ManagerType enum value used to describe the type of interface/functionality this manager provides.
            static const ManagerBase::ManagerType InterfaceType;

            /// @brief Identifier for the Event that is raised when a Joystick is added to the system.
            /// @details Events with this name are of type: DeviceEvent.
            static const EventNameType EventJoystickAdded;
            /// @brief Identifier for the Event that is raised when a Joystick is removed from the system.
            /// @details Events with this name are of type: DeviceEvent.
            static const EventNameType EventJoystickRemoved;
            /// @brief Identifier for the Event that is raised when a Controller is added to the system.
            /// @details Events with this name are of type: DeviceEvent.
            static const EventNameType EventControllerAdded;
            /// @brief Identifier for the Event that is raised when a Controller is removed from the system.
            /// @details Events with this name are of type: DeviceEvent.
            static const EventNameType EventControllerRemoved;
            /// @brief Identifier for the Event that is raised when a Controller has it's buttons remapped.
            /// @details Events with this name are of type: DeviceEvent.
            static const EventNameType EventControllerRemapped;
            /// @brief Identifier for the Event that is raised when the keyboard key layout is changed.
            /// @details Events with this name are of type: DeviceEvent.
            static const EventNameType EventKeymapChanged;
            /// @brief Identifier for the Event that is raised when the system clipboard has it's contents updated.
            /// @details Events with this name are of type: Event.
            static const EventNameType EventClipboardUpdated;
        protected:
            friend class DeviceUpdateWorkUnit;
            /// @brief A publisher for notifying when the system input configuration changes.
            DefaultEventPublisher InputPublisher;

            /// @brief Container storing all the detected Joysticks on this system.
            JoystickContainer Joysticks;
            /// @brief Container storing all the detected Controllers on this system.
            ControllerContainer Controllers;
            /// @brief Container storing all the MetaCodes generated for the current frame.
            MetaCodeContainer InputDeltas;
            /// @brief Container storing all the cross-device sequences this manager is to check for.
            SequenceContainer Sequences;

            /// @brief The pointer to the object representing the system mouse.
            Mouse* SystemMouse;
            /// @brief The pointer to the object representing the system keyboard.
            Keyboard* SystemKeyboard;

            /// @brief The work unit that updates the input devices with the newest data.
            DeviceUpdateWorkUnit* DeviceUpdateWork;
            /// @brief Can be used for thread safe logging and other thread specific resources.
            Threading::DefaultThreadSpecificStorage::Type* ThreadResources;

            /// @brief Common construction logic for multiple constructors.
            void ConstructManager();
            /// @brief Creates a new Joystick from a device index.
            /// @param Index The device index of the Joystick to construct.
            /// @return Returns a pointer to the created Joystick.
            Joystick* ConstructJoystick(int Index);
            /// @brief Creates a new Controller from a device index.
            /// @param Index The device index of the Controller to construct.
            /// @return Returns a pointer to the created Controller.
            Controller* ConstructController(int Index);

            /// @brief Pumps the internal event queue and populates our input deltas.
            void PumpInternalEvents();
            /// @brief Updates the Keyboard state from a range of MetaCodes.
            /// @param UpdateBegin A mutable iterator to the start of the range to update.
            /// @param RangeEnd A const iterator to the end of the entire range.
            /// @return Returns a container of the codes the device generated during the update.
            MetaCodeContainer UpdateKeyboard(MetaCodeIterator& UpdateBegin, const MetaCodeIterator RangeEnd);
            /// @brief Updates the Mouse state from a range of MetaCodes.
            /// @param UpdateBegin A mutable iterator to the start of the range to update.
            /// @param RangeEnd A const iterator to the end of the entire range.
            /// @return Returns a container of the codes the device generated during the update.
            MetaCodeContainer UpdateMouse(MetaCodeIterator& UpdateBegin, const MetaCodeIterator RangeEnd);
            /// @brief Updates the Joystick state from a range of MetaCodes.
            /// @param UpdateBegin A mutable iterator to the start of the range to update.
            /// @param RangeEnd A const iterator to the end of the entire range.
            /// @return Returns a container of the codes the device generated during the update.
            MetaCodeContainer UpdateJoysticks(MetaCodeIterator& UpdateBegin, const MetaCodeIterator RangeEnd);
            /// @brief Updates the Controller state from a range of MetaCodes.
            /// @param UpdateBegin A mutable iterator to the start of the range to update.
            /// @param RangeEnd A const iterator to the end of the entire range.
            /// @return Returns a container of the codes the device generated during the update.
            MetaCodeContainer UpdateControllers(MetaCodeIterator& UpdateBegin, const MetaCodeIterator RangeEnd);
        public:
            /// @brief Class constructor.
            InputManager();
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            InputManager(const XML::Node& XMLNode);
            /// @brief Class destructor.
            virtual ~InputManager();

            ///////////////////////////////////////////////////////////////////////////////
            // InputDevice Management

            /// @brief Gets the system mouse.
            /// @return Returns a pointer to the mouse device.
            Mouse* GetSystemMouse() const;
            /// @brief Gets the system keyboard.
            /// @return Returns a pointer to the keyboard device.
            Keyboard* GetSystemKeyboard() const;

            /// @brief Gets a Joystick by device ID.
            /// @param ID The ID of the device to retrieve.
            /// @return Returns the Joystick with the specified ID, or NULL if no Joystick with that ID exists.
            Joystick* GetJoystickByID(const DeviceIDType ID) const;
            /// @brief Gets a Joysticks by index.
            /// @param Index The index of the Joystick to retrieve.
            /// @return Returns a pointer to the Joysticks at the specified index.
            Joystick* GetJoystick(const UInt16 Index) const;
            /// @brief Gets the number of Joysticks detected.
            /// @return Returns the number of Joysticks that have been detected on this system.
            UInt16 GetNumJoysticks() const;

            /// @brief Gets a Controller by device ID.
            /// @param ID The ID of the device to retrieve.
            /// @return Returns the Controller with the specified ID, or NULL if no Controller with that ID exists.
            Controller* GetControllerByID(const DeviceIDType ID) const;
            /// @brief Gets a Controller by index.
            /// @param Index The index of the Controller to retrieve.
            /// @return Returns a pointer to the Controller at the specified index.
            Controller* GetController(const UInt16 Index) const;
            /// @brief Gets the number of Controllers detected.
            /// @return Returns the number of Controllers that have been detected on this system.
            UInt16 GetNumControllers() const;

            ///////////////////////////////////////////////////////////////////////////////
            // InputDevice Detection

            /// @brief Detects all of the Controllers and Joysticks that are connected to the system and prepares them for use.
            /// @return Returns the number of devices that have been detected.
            UInt16 DetectDevices();
            /// @brief Releases all Controller and Joystick devices from this manager.
            void ReleaseAllDevices();

            ///////////////////////////////////////////////////////////////////////////////
            // Sequenced Input Management

            /// @copydoc SequenceContainer::AddInputSequence(const MetaCodeContainer& Codes, const Int32& SequenceID)
            void AddInputSequence(const MetaCodeContainer& Codes, const Int32& SequenceID);
            /// @copydoc SequenceContainer::InputSequenceExists(const MetaCodeContainer& Codes)
            Boole InputSequenceExists(const MetaCodeContainer& Codes);
            /// @copydoc SequenceContainer::GetIDofInputSequence(const MetaCodeContainer& Codes)
            Int32 GetIDofInputSequence(const MetaCodeContainer& Codes);
            /// @copydoc SequenceContainer::RemoveInputSequence(const MetaCodeContainer& Codes)
            void RemoveInputSequence(const MetaCodeContainer& Codes);
            /// @copydoc SequenceContainer::RemoveAllInputSequences()
            void RemoveAllInputSequences();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Updates the state of all input devices from the application event queue.
            void UpdateInputDevices();
            /// @brief Gets all the input codes that were generated this frame.
            /// @return Returns a const reference to a vector containing all the inputs updated this frame.
            const MetaCodeContainer& GetInputDeltas() const;

            /// @copydoc ManagerBase::Initialize()
            virtual void Initialize();
            /// @copydoc ManagerBase::Deinitialize()
            virtual void Deinitialize();

            /// @brief Gets the work unit responsible for updating the input device classes.
            /// @return Returns a pointer to the DeviceUpdateWorkUnit used by this manager.
            DeviceUpdateWorkUnit* GetDeviceUpdateWork();
            /// @brief Gets the publisher for the Input system events.
            /// @return Returns a reference to the publisher that will dispatch Input events.
            DefaultEventPublisher& GetInputPublisher();
            /// @brief Gets the publisher for the Input system events.
            /// @return Returns a const reference to the publisher that will dispatch Input events.
            const DefaultEventPublisher& GetInputPublisher() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Type Identifier Methods

            /// @copydoc ManagerBase::GetInterfaceType()
            virtual ManagerType GetInterfaceType() const;
            /// @copydoc ManagerBase::GetImplementationTypeName()
            virtual String GetImplementationTypeName() const;
        };//InputManager

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A factory responsible for the creation and destruction of the default inputmanager.
        ///////////////////////////////////////
        class MEZZ_LIB DefaultInputManagerFactory : public EntresolManagerFactory
        {
        public:
            /// @brief Class constructor.
            DefaultInputManagerFactory();
            /// @brief Class destructor.
            virtual ~DefaultInputManagerFactory();

            /// @copydoc ManagerFactory::GetManagerImplName()
            String GetManagerImplName() const;
            /// @copydoc ManagerFactory::GetManagerType() const
            ManagerBase::ManagerType GetManagerType() const;

            /// @copydoc EntresolManagerFactory::CreateManager(const NameValuePairList&)
            EntresolManager* CreateManager(const NameValuePairList& Params);
            /// @copydoc EntresolManagerFactory::CreateManager(const XML::Node&)
            EntresolManager* CreateManager(const XML::Node& XMLNode);
            /// @copydoc EntresolManagerFactory::DestroyManager(EntresolManager*)
            void DestroyManager(EntresolManager* ToBeDestroyed);
        };//DefaultInputManagerFactory
    }//Input
}//Mezzanine

#endif
