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
#ifndef _audiooalssoundscapemanager_h
#define _audiooalssoundscapemanager_h

#include "Audio/soundscapemanager.h"
#include "uidgenerator.h"

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
            class SoundScapeManager;

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief This is the work unit for updating audio buffers as necessary for audio playback.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB BufferUpdate3DWorkUnit : public Audio::iBufferUpdate3DWorkUnit
            {
            protected:
                /// @internal
                /// @brief A pointer to the manager this work unit is processing.
                OALS::SoundScapeManager* TargetManager;
                /// @internal
                /// @brief Protected copy constructor.  THIS IS NOT ALLOWED.
                /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
                BufferUpdate3DWorkUnit(const BufferUpdate3DWorkUnit& Other);
                /// @internal
                /// @brief Protected assignment operator.  THIS IS NOT ALLOWED.
                /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
                BufferUpdate3DWorkUnit& operator=(const BufferUpdate3DWorkUnit& Other);
            public:
                /// @brief Class constructor.
                /// @param Target The InputManager this work unit will process during the frame.
                BufferUpdate3DWorkUnit(OALS::SoundScapeManager* Target);
                /// @brief Class destructor.
                virtual ~BufferUpdate3DWorkUnit();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @brief This does any required updating of audio buffers belonging to sound proxies in this manager.
                /// @param CurrentThreadStorage The storage class for all resources owned by this work unit during it's execution.
                virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
            };//BufferUpdate3DWorkUnit

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
                typedef std::vector<OALS::SoundProxy*>              ProxyContainer;
                /// @brief Iterator type for @ref OALS::SoundProxy instances stored by this class.
                typedef ProxyContainer::iterator                    ProxyIterator;
                /// @brief Const Iterator type for @ref OALS::SoundProxy instances stored by this class.
                typedef ProxyContainer::const_iterator              ConstProxyIterator;

                /// @brief A String containing the name of this manager implementation.
                static const String ImplementationName;
                /// @brief A ManagerType enum value used to describe the type of interface/functionality this manager provides.
                static const ManagerBase::ManagerType InterfaceType;
            protected:
                friend class BufferUpdate3DWorkUnit;

                /// @internal
                /// @brief Generator responsible for creating unique IDs for @ref OALS::Listener and @ref OALS::SoundProxy instances.
                UIDGenerator ProxyIDGen;
                /// @internal
                /// @brief Container storing all OALS context instances.
                ContextContainer Contexts;
                /// @internal
                /// @brief Container storing all @ref OALS::Listener instances.
                ListenerContainer Listeners;
                /// @internal
                /// @brief Container storing all @ref OALS::SoundProxy instances.
                ProxyContainer Proxies;

                /// @internal
                /// @brief The workunit this will use to complete its buffer updates.
                OALS::BufferUpdate3DWorkUnit* BufferUpdate3DWork;
                /// @internal
                /// @brief Can be used for thread safe logging and other thread specific resources.
                Threading::DefaultThreadSpecificStorage::Type* ThreadResources;

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
                /// @param Creator The parent world that is creating the manager.
                SoundScapeManager(World* Creator);
                /// @brief XML constructor.
                /// @param Creator The parent world that is creating the manager.
                /// @param XMLNode The node of the xml document to construct from.
                SoundScapeManager(World* Creator, const XML::Node& XMLNode);
                /// @brief Class destructor.
                virtual ~SoundScapeManager();

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

                /// @copydoc Audio::SoundScapeManager::CreateSoundProxy(const UInt16, const Boole)
                virtual Audio::SoundProxy* CreateSoundProxy(const UInt16 Type, const Boole AddToWorld);
                /// @copydoc Audio::SoundScapeManager::CreateSoundProxy(const UInt16, Resource::DataStreamPtr, const Audio::Encoding, const Boole)
                virtual Audio::SoundProxy* CreateSoundProxy(const UInt16 Type, Resource::DataStreamPtr Stream, const Audio::Encoding Encode, const Boole AddToWorld);
                /// @copydoc Audio::SoundScapeManager::CreateSoundProxy(const UInt16, Resource::DataStreamPtr, const UInt32, const Audio::BitConfig, const Boole)
                virtual Audio::SoundProxy* CreateSoundProxy(const UInt16 Type, Resource::DataStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config, const Boole AddToWorld);
                /// @copydoc Audio::SoundScapeManager::CreateSoundProxy(const UInt16, const String&, const String&, const Boole)
                virtual Audio::SoundProxy* CreateSoundProxy(const UInt16 Type, const String& FileName, const String& Group, const Boole AddToWorld);
                /// @copydoc Audio::SoundScapeManager::CreateSoundProxy(const UInt16, const String&, Char8*, const UInt32, const Audio::Encoding, const Boole)
                virtual Audio::SoundProxy* CreateSoundProxy(const UInt16 Type, const String& StreamName, Char8* Buffer, const UInt32 Length, const Audio::Encoding Encode, const Boole AddToWorld);
                /// @copydoc Audio::SoundScapeManager::CreateSoundProxy(const UInt16, const String&, Char8*, const UInt32, const UInt32, const Audio::BitConfig, const Boole)
                virtual Audio::SoundProxy* CreateSoundProxy(const UInt16 Type, const String& StreamName, Char8* Buffer, const UInt32 Length, const UInt32 Frequency, const Audio::BitConfig Config, const Boole AddToWorld);

                /// @copydoc Audio::SoundScapeManager::GetSoundProxy(const UInt32) const
                virtual Audio::SoundProxy* GetProxy(const UInt32 Index) const;
                /// @copydoc Audio::SoundScapeManager::GetProxy(const Mezzanine::ProxyType, UInt32) const
                virtual Audio::SoundProxy* GetProxy(const Mezzanine::ProxyType Type, UInt32 Which) const;
                /// @copydoc Audio::SoundScapeManager::GetNumSoundProxies() const
                virtual UInt32 GetNumProxies() const;
                /// @copydoc Audio::SoundScapeManager::DestroySoundProxy(SoundProxy*)
                virtual void DestroyProxy(Audio::SoundProxy* ToBeDestroyed);
                /// @copydoc Audio::SoundScapeManager::DestroyAllProxies()
                virtual void DestroyAllProxies();

                #ifndef SWIG
                /// @brief Gets an iterator to the first Sound Proxy in this manager.
                /// @return Returns an iterator to the first Sound Proxy being stored by this manager.
                ProxyIterator BeginSoundProxy();
                /// @brief Gets an iterator to one passed the last Sound Proxy in this manager.
                /// @return Returns an iterator to one passed the last Sound Proxy being stored by this manager.
                ProxyIterator EndSoundProxy();
                /// @brief Gets a const iterator to the first Sound Proxy in this manager.
                /// @return Returns a const iterator to the first Sound Proxy being stored by this manager.
                ConstProxyIterator BeginSoundProxy() const;
                /// @brief Gets a const iterator to one passed the last Sound Proxy in this manager.
                /// @return Returns a const iterator to one passed the last Sound Proxy being stored by this manager.
                ConstProxyIterator EndSoundProxy() const;
                #endif

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc WorldManager::Pause(const UInt32)
                virtual void Pause(const UInt32 PL);

                /// @copydoc WorldManager::Initialize()
                virtual void Initialize();
                /// @copydoc ManagerBase::Deinitialize()
                virtual void Deinitialize();

                /// @copydoc Audio::SoundScapeManager::GetBufferUpdate3DWork()
                virtual iBufferUpdate3DWorkUnit* GetBufferUpdate3DWork();

                ///////////////////////////////////////////////////////////////////////////////
                // Type Identifier Methods

                /// @copydoc ManagerBase::GetImplementationTypeName()
                virtual String GetImplementationTypeName() const;
            };//SoundScapeManager
        }//OALS
    }//Audio
}//Mezzanine

#endif
