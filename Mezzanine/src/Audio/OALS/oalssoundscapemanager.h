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
#ifndef _audiooalssoundscapemanager_h
#define _audiooalssoundscapemanager_h

#include "Audio/soundscapemanager.h"

// OALS forward declares
#ifndef OALS_STRUCTS_DECLARED
struct ALCcontext;
#endif //OALS_STRUCTS_DECLARED

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            class Listener;
            class SoundProxy;
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief This is the base manager class for audio being played in a 3D environment.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB SoundScapeManager : public Audio::SoundScapeManager
            {
            public:
                /// @brief Basic container type for OALS context storage by this class.
                typedef std::vector<ALCcontext*>                    ContextContainer;
                /// @brief Iterator type for OALS context instances stored by this class.
                typedef ContextContainer::iterator                  ContextIterator;
                /// @brief Const Iterator type for OALS context instances stored by this class.
                typedef ContextContainer::const_iterator            ConstContextIterator;
                /// @brief Basic container type for @ref OALS::Listener storage by this class.
                typedef std::vector<OALS::Listener*>                ListenerContainer;
                /// @brief Iterator type for @ref OALS::Listener instances stored by this class.
                typedef ListenerContainer::iterator                 ListenerIterator;
                /// @brief Const Iterator type for @ref OALS::Listener instances stored by this class.
                typedef ListenerContainer::const_iterator           ConstListenerIterator;
                /// @brief Basic container type for @ref OALS::SoundProxy storage by this class.
                typedef std::vector<OALS::SoundProxy*>              SoundProxyContainer;
                /// @brief Iterator type for @ref OALS::SoundProxy instances stored by this class.
                typedef SoundProxyContainer::iterator               SoundProxyIterator;
                /// @brief Const Iterator type for @ref OALS::SoundProxy instances stored by this class.
                typedef SoundProxyContainer::const_iterator         ConstSoundProxyIterator;
            protected:
                /// @internal
                /// @brief Container storing all OALS context instances.
                ContextContainer Contexts;
                /// @internal
                /// @brief Container storing all @ref OALS::Listener instances.
                ListenerContainer Listeners;
                /// @internal
                /// @brief Container storing all @ref OALS::SoundProxy instances.
                SoundProxyContainer SoundProxies;

                /// @internal
                /// @brief Creates a new context for use by objects in this manager.
                ALCcontext* CreateContext();
                /// @internal
                /// @brief Destroys an existing context owned by this manager.
                void DestroyContext(ALCcontext* Context);
                /// @internal
                /// @brief Destroys all contexts owned by this manager.
                void DestroyAllContexts();
            public:
                /// @brief Class constructor.
                SoundScapeManager();
                /// @brief XML constructor.
                /// @param XMLNode The node of the xml document to construct from.
                SoundScapeManager(XML::Node& XMLNode);
                /// @brief Class destructor.
                virtual ~SoundScapeManager();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                ///////////////////////////////////////////////////////////////////////////////
                // Listener Management

                /// @copydoc Audio::SoundScapeManager::CreateListener()
                virtual iListener* CreateListener();
                /// @copydoc Audio::SoundScapeManager::GetListener(const UInt32) const
                virtual iListener* GetListener(const UInt32 Index) const;
                /// @copydoc Audio::SoundScapeManager::GetNumListeners() const
                virtual UInt32 GetNumListeners() const;
                /// @copydoc Audio::SoundScapeManager::DestroyListener(iListener*)
                virtual void DestroyListener(iListener* ToBeDestroyed);
                /// @copydoc Audio::SoundScapeManager::DestroyAllListeners()
                virtual void DestroyAllListeners();

                ///////////////////////////////////////////////////////////////////////////////
                // Proxy Management

                /// @copydoc Audio::SoundScapeManager::CreateSoundProxy(const UInt16)
                virtual Audio::SoundProxy* CreateSoundProxy(const UInt16 Type);
                /// @copydoc Audio::SoundScapeManager::CreateSoundProxy(const UInt16, Resource::DataStreamPtr, const Audio::Encoding)
                virtual Audio::SoundProxy* CreateSoundProxy(const UInt16 Type, Resource::DataStreamPtr Stream, const Audio::Encoding Encode);
                /// @copydoc Audio::SoundScapeManager::CreateSoundProxy(const UInt16, Resource::DataStreamPtr, const UInt32, const Audio::BitConfig)
                virtual Audio::SoundProxy* CreateSoundProxy(const UInt16 Type, Resource::DataStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config);
                /// @copydoc Audio::SoundScapeManager::CreateSoundProxy(const UInt16, const String&, const String&)
                virtual Audio::SoundProxy* CreateSoundProxy(const UInt16 Type, const String& FileName, const String& Group);
                /// @copydoc Audio::SoundScapeManager::CreateSoundProxy(const UInt16, const String&, Char8*, const UInt32, const Audio::Encoding)
                virtual Audio::SoundProxy* CreateSoundProxy(const UInt16 Type, const String& StreamName, Char8* Buffer, const UInt32 Length, const Audio::Encoding Encode);
                /// @copydoc Audio::SoundScapeManager::CreateSoundProxy(const UInt16, const String&, Char8*, const UInt32, const UInt32, const Audio::BitConfig)
                virtual Audio::SoundProxy* CreateSoundProxy(const UInt16 Type, const String& StreamName, Char8* Buffer, const UInt32 Length, const UInt32 Frequency, const Audio::BitConfig Config);
                /// @copydoc Audio::SoundScapeManager::GetSoundProxy(const UInt32) const
                virtual Audio::SoundProxy* GetSoundProxy(const UInt32 Index) const;
                /// @copydoc Audio::SoundScapeManager::GetNumSoundProxies() const
                virtual UInt32 GetNumSoundProxies() const;
                /// @copydoc Audio::SoundScapeManager::DestroySoundProxy(SoundProxy*)
                virtual void DestroySoundProxy(Audio::SoundProxy* ToBeDestroyed);
                /// @copydoc Audio::SoundScapeManager::DestroyAllSoundProxies()
                virtual void DestroyAllSoundProxies();

                ///////////////////////////////////////////////////////////////////////////////
                // Inherited from Managerbase

                /// @copydoc ManagerBase::Initialize()
                virtual void Initialize();
                /// @copydoc ManagerBase::DoMainLoopItems()
                virtual void DoMainLoopItems();
                /// @copydoc ManagerBase::GetImplementationTypeName()
                virtual String GetImplementationTypeName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Methods

                void _NotifyDeviceInitialize();
                void _NotifyDeviceShutdown();
            };//SoundScapeManager
        }//OALS
    }//Audio
}//Mezzanine

#endif
