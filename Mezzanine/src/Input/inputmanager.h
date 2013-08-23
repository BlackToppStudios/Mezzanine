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
#ifndef _inputinputmanager_h
#define _inputinputmanager_h

#include "managerbase.h"
#include "managerfactory.h"
#include "singleton.h"
#include "Input/metacode.h"
#include "Input/sequencecontainer.h"
#include "Threading/workunit.h"

namespace Mezzanine
{
    namespace Input
    {
        class Timer;
        class Controller;
        class Keyboard;
        class Mouse;
        class InputManager;
        class InputManagerInternalData;

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a Mezzanine::Threading::iWorkUnit for the updating of the physics debug drawer.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB DeviceUpdateWorkUnit : public Threading::DefaultWorkUnit
        {
        protected:
            /// @internal
            /// @brief A pointer to the manager this work unit is processing.
            InputManager* TargetManager;
            /// @internal
            /// @brief Protected copy constructor.  THIS IS NOT ALLOWED.
            /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
            DeviceUpdateWorkUnit(const DeviceUpdateWorkUnit& Other);
            /// @internal
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

        // Used by the scripting language binder to help create bindgings for this class. SWIG does know to creation template instances
        #ifdef SWIG
        %template(SingletonInputManager) Singleton<InputManager>;
        #endif

        ///////////////////////////////////////////////////////////////////////////////
        /// @class InputManager
        /// @headerfile inputmanager.h
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
        class MEZZ_LIB InputManager : public ManagerBase, public Singleton<InputManager>
        {
        public:
            typedef std::vector< Controller* >           ControllerContainer;
            typedef ControllerContainer::iterator        ControllerIterator;
            typedef ControllerContainer::const_iterator  ConstControllerIterator;
        protected:
            friend class DeviceUpdateWorkUnit;

            /// @internal
            /// @brief Container storing all the cross-device sequences this manager is to check for.
            SequenceContainer Sequences;
            /// @internal
            /// @brief Container storing all the MetaCodes generated for the current frame.
            MetaCodeContainer InputDeltas;

            /// @internal
            /// @brief The pointer to the internal data handled by this manager.
            InputManagerInternalData* IMID;
            /// @internal
            /// @brief The pointer to the object representing the system mouse.
            Mouse* SystemMouse;
            /// @internal
            /// @brief The pointer to the object representing the system keyboard.
            Keyboard* SystemKeyboard;

            /// @internal
            /// @brief The work unit that updates the input devices with the newest data.
            DeviceUpdateWorkUnit* DeviceUpdateWork;
            /// @internal
            /// @brief Can be used for thread safe logging and other thread specific resources.
            Threading::DefaultThreadSpecificStorage::Type* ThreadResources;
        public:
            /// @brief Class constructor.
            InputManager();
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            InputManager(XML::Node& XMLNode);
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

            /// @brief Gets a controller by index.
            /// @return Returns a pointer to the controller at the specified index.
            Controller* GetController(const UInt16 Index) const;
            /// @brief Gets the number of controllers detected.
            /// @return Returns the number of controllers that have been detected on this system.
            UInt16 GetNumControllers() const;

            ///////////////////////////////////////////////////////////////////////////////
            // InputDevice Detection

            /// @brief Gathers all of the controllers that are connected to the system and creates corresponding devices for each one.
            /// @return Returns the number of controllers that have been detected.
            UInt16 DetectControllers();
            /// @brief Releases all controller devices from this manager.
            void ReleaseAllControllers();

            ///////////////////////////////////////////////////////////////////////////////
            // Sequenced Input Management

            /// @copydoc SequenceContainer::AddInputSequence(const MetaCodeContainer& Codes, const Int32& SequenceID)
            void AddInputSequence(const MetaCodeContainer& Codes, const Int32& SequenceID);
            /// @copydoc SequenceContainer::InputSequenceExists(const MetaCodeContainer& Codes)
            bool InputSequenceExists(const MetaCodeContainer& Codes);
            /// @copydoc SequenceContainer::GetIDofInputSequence(const MetaCodeContainer& Codes)
            Int32 GetIDofInputSequence(const MetaCodeContainer& Codes);
            /// @copydoc SequenceContainer::RemoveInputSequence(const MetaCodeContainer& Codes)
            void RemoveInputSequence(const MetaCodeContainer& Codes);
            /// @copydoc SequenceContainer::RemoveAllInputSequences()
            void RemoveAllInputSequences();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

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

            ///////////////////////////////////////////////////////////////////////////////
            // Type Identifier Methods

            /// @copydoc ManagerBase::GetInterfaceType()
            virtual ManagerType GetInterfaceType() const;
            /// @copydoc ManagerBase::GetImplementationTypeName()
            virtual String GetImplementationTypeName() const;
        };//InputManager

        ///////////////////////////////////////////////////////////////////////////////
        /// @class DefaultInputManagerFactory
        /// @headerfile inputmanager.h
        /// @brief A factory responsible for the creation and destruction of the default inputmanager.
        ///////////////////////////////////////
        class MEZZ_LIB DefaultInputManagerFactory : public ManagerFactory
        {
        public:
            /// @brief Class constructor.
            DefaultInputManagerFactory();
            /// @brief Class destructor.
            virtual ~DefaultInputManagerFactory();

            /// @copydoc ManagerFactory::GetManagerTypeName()
            String GetManagerTypeName() const;

            /// @copydoc ManagerFactory::CreateManager(NameValuePairList&)
            ManagerBase* CreateManager(NameValuePairList& Params);
            /// @copydoc ManagerFactory::CreateManager(XML::Node&)
            ManagerBase* CreateManager(XML::Node& XMLNode);
            /// @copydoc ManagerFactory::DestroyManager(ManagerBase*)
            void DestroyManager(ManagerBase* ToBeDestroyed);
        };//DefaultInputManagerFactory
    }//Input
}//Mezzanine

#endif
