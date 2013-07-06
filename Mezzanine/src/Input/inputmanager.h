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

namespace Mezzanine
{
    // Used by the scripting language binder to help create bindgings for this class. SWIG does know to creation template instances
    #ifdef SWIG
    %template(SingletonInputManager) Singleton<InputManager>;
    #endif
    namespace Input
    {
        class Controller;
        class Keyboard;
        class Mouse;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class InputManager
        /// @headerfile inputmanager.h
        /// @brief This is the manager responsible for the handling of input devices and events.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB InputManager : public ManagerBase, public Singleton<InputManager>
        {
            public:
                typedef std::vector< Controller* >    ControllerContainer;
                typedef ControllerContainer::iterator        ControllerIterator;
                typedef ControllerContainer::const_iterator  ConstControllerIterator;
            protected:
                Mouse* SystemMouse;
                Keyboard* SystemKeyboard;

                ControllerContainer Controllers;
                std::vector<void*> InternalControllers;
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
                // Inherited from ManagerBase

                /// @copydoc ManagerBase::Initialize()
                virtual void Initialize();
                /// @copydoc ManagerBase::DoMainLoopItems()
                virtual void DoMainLoopItems();
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
