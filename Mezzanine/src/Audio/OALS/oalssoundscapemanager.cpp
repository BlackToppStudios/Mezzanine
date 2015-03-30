// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _audiooalssoundscapemanager_cpp
#define _audiooalssoundscapemanager_cpp

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#define OALS_STRUCTS_DECLARED

#include "Audio/rawdecoderfactory.h"

#include "Audio/OALS/oalslistener.h"
#include "Audio/OALS/oalssoundproxy.h"
#include "Audio/OALS/oalsdefines.h"

#include "Audio/OALS/oalsaudiomanager.h"
#include "Audio/OALS/oalssoundscapemanager.h"

#include "Resource/resourcemanager.h"

#include "entresol.h"
#include "stringtool.h"
#include "exception.h"

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            ///////////////////////////////////////////////////////////////////////////////
            // BufferUpdate3DWorkUnit Methods

            BufferUpdate3DWorkUnit::BufferUpdate3DWorkUnit(const BufferUpdate3DWorkUnit& Other)
                {  }

            BufferUpdate3DWorkUnit& BufferUpdate3DWorkUnit::operator=(const BufferUpdate3DWorkUnit& Other)
                { return *this; }

            BufferUpdate3DWorkUnit::BufferUpdate3DWorkUnit(OALS::SoundScapeManager* Target) :
                TargetManager(Target) {  }

            BufferUpdate3DWorkUnit::~BufferUpdate3DWorkUnit()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void BufferUpdate3DWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
            {
                for( SoundScapeManager::ProxyIterator ProxIt = this->TargetManager->Proxies.begin() ; ProxIt != this->TargetManager->Proxies.end() ; ++ProxIt )
                {
                    (*ProxIt)->_Update();
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // SoundScapeManager Methods

            const String SoundScapeManager::ImplementationName = "DefaultSoundScapeManager";
            const ManagerBase::ManagerType SoundScapeManager::InterfaceType = ManagerBase::MT_SoundScapeManager;

            SoundScapeManager::SoundScapeManager(World* Creator) :
                Audio::SoundScapeManager(Creator),
                BufferUpdate3DWork(NULL),
                ThreadResources(NULL)
            {
                this->BufferUpdate3DWork = new BufferUpdate3DWorkUnit(this);
                OALS::AudioManager* AudioMan = static_cast<OALS::AudioManager*>(Audio::AudioManager::GetSingletonPtr());
                AudioMan->_RegisterSoundScapeManager(this);

                // If the main audio manager is ready to go, then so are we.
                this->Initialized = AudioMan->IsInitialized();
                // Allocate our space for contexts
                this->Contexts.resize(OALS_MAX_LISTENERS_PER_MANAGER,NULL);
            }

            SoundScapeManager::SoundScapeManager(World* Creator, XML::Node& XMLNode) :
                Audio::SoundScapeManager(Creator),
                BufferUpdate3DWork(NULL),
                ThreadResources(NULL)
            {
                this->BufferUpdate3DWork = new BufferUpdate3DWorkUnit(this);
                OALS::AudioManager* AudioMan = static_cast<OALS::AudioManager*>(Audio::AudioManager::GetSingletonPtr());
                AudioMan->_RegisterSoundScapeManager(this);

                // If the main audio manager is ready to go, then so are we.
                this->Initialized = AudioMan->IsInitialized();
                // Allocate our space for contexts
                this->Contexts.resize(OALS_MAX_LISTENERS_PER_MANAGER,NULL);
            }

            SoundScapeManager::~SoundScapeManager()
            {
                this->Deinitialize();

                delete BufferUpdate3DWork;

                OALS::AudioManager* AudioMan = static_cast<OALS::AudioManager*>(Audio::AudioManager::GetSingletonPtr());
                if(AudioMan)
                {
                    AudioMan->_UnregisterSoundScapeManager(this);
                }

                this->DestroyAllProxies();
                this->DestroyAllListeners();
            }

            ALCcontext* SoundScapeManager::CreateContext()
            {
                OALS::AudioManager* AudioMan = static_cast<OALS::AudioManager*>(Audio::AudioManager::GetSingletonPtr());
                if( AudioMan->IsInitialized() == false )
                    { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Cannot create 3D context prior to AudioManager initialization.  An open Audio device is required."); }

                ALCdevice* AudioPlayback = AudioMan->_GetAudioDevice();
                ALCcontext* NewContext = alcCreateContext(AudioPlayback,NULL);
                if( alcMakeContextCurrent(NewContext) == false )
                {
                    alcDestroyContext(NewContext);
                    return NULL;
                }

                UInt32 ContextIndex = 0;
                for( UInt32 Index = 0 ; Index < this->Contexts.size() ; ++Index )
                {
                    if( this->Contexts.at(Index) == NULL )
                    {
                        this->Contexts[Index] = NewContext;
                    }
                    ContextIndex++;
                }

                for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
                {
                    (*ProxIt)->_OnContextCreated(ContextIndex,NewContext);
                }

                return NewContext;
            }

            void SoundScapeManager::DestroyContext(ALCcontext* Context)
            {
                OALS::AudioManager* AudioMan = static_cast<OALS::AudioManager*>(Audio::AudioManager::GetSingletonPtr());
                UInt32 ContextIndex = 0;
                for( UInt32 Index = 0 ; Index < this->Contexts.size() ; ++Index )
                {
                    if( this->Contexts.at(Index) == Context )
                    {
                        this->Contexts.erase( this->Contexts.begin() + Index );
                        break;
                    }
                    ContextIndex++;
                }

                for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
                {
                    (*ProxIt)->_OnContextDestroyed(ContextIndex,Context);
                }

                alcMakeContextCurrent(AudioMan->_GetNonSpacialContext());
                alcDestroyContext(Context);
            }

            void SoundScapeManager::DestroyAllContexts()
            {
                OALS::AudioManager* AudioMan = static_cast<OALS::AudioManager*>( Audio::AudioManager::GetSingletonPtr() );
                for( ContextIterator ConIt = this->Contexts.begin() ; ConIt != this->Contexts.end() ; ++ConIt )
                {
                    UInt32 ContextIndex = 0;
                    for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
                    {
                        (*ProxIt)->_OnContextDestroyed( ContextIndex, (*ConIt) );
                    }

                    if(AudioMan)
                        { alcMakeContextCurrent( AudioMan->_GetNonSpacialContext() ); }
                    alcDestroyContext( (*ConIt) );
                }
                this->Contexts.clear();
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Listener Management

            iListener* SoundScapeManager::CreateListener()
            {
                ALCcontext* ListenerContext = this->CreateContext();
                OALS::Listener* NewListener = new OALS::Listener(ListenerContext,this);
                this->Listeners.push_back(NewListener);
                return NewListener;
            }

            iListener* SoundScapeManager::GetListener(const UInt32 Index) const
            {
                return this->Listeners.at(Index);
            }

            UInt32 SoundScapeManager::GetNumListeners() const
            {
                return this->Listeners.size();
            }

            void SoundScapeManager::DestroyListener(iListener* ToBeDestroyed)
            {
                ALCcontext* ListenerContext = NULL;
                for( ListenerIterator ListIt = this->Listeners.begin() ; ListIt != this->Listeners.end() ; ++ListIt )
                {
                    if( (*ListIt) == ToBeDestroyed )
                    {
                        ListenerContext = static_cast<OALS::Listener*>(ToBeDestroyed)->_GetListenerContext();
                        delete ToBeDestroyed;
                        this->Listeners.erase(ListIt);
                    }
                }

                if( ListenerContext != NULL )
                    this->DestroyContext(ListenerContext);
            }

            void SoundScapeManager::DestroyAllListeners()
            {
                for( ListenerIterator ListIt = this->Listeners.begin() ; ListIt != this->Listeners.end() ; ++ListIt )
                {
                    delete (*ListIt);
                }
                this->Listeners.clear();
                this->DestroyAllContexts();
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Proxy Management

            Audio::SoundProxy* SoundScapeManager::CreateSoundProxy(const UInt16 Type, const Boole AddToWorld)
            {
                if( this->Initialized == false )
                    { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Cannot create a new SoundProxy without an audio device being initialized."); }

                OALS::SoundProxy* NewSoundProxy = new OALS::SoundProxy(Type,NULL,this->Contexts,this);
                this->Proxies.push_back(NewSoundProxy);
                if( AddToWorld ) {
                    NewSoundProxy->AddToWorld();
                }
                return NewSoundProxy;
            }

            Audio::SoundProxy* SoundScapeManager::CreateSoundProxy(const UInt16 Type, Resource::DataStreamPtr Stream, const Audio::Encoding Encode, const Boole AddToWorld)
            {
                if( this->Initialized == false )
                    { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Cannot create a new SoundProxy without an audio device being initialized."); }

                OALS::AudioManager* AudioMan = static_cast<OALS::AudioManager*>( Audio::AudioManager::GetSingletonPtr() );
                iDecoderFactory* Factory = AudioMan->GetDecoderFactory(Encode);
                if( Factory == NULL )
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Unsupported encoding requested.  Did you enable the proper encoding in CMake?"); }

                iDecoder* SoundDecoder = Factory->CreateDecoder(Stream);
                OALS::SoundProxy* NewSoundProxy = new OALS::SoundProxy(Type,SoundDecoder,this->Contexts,this);
                this->Proxies.push_back(NewSoundProxy);
                if( AddToWorld ) {
                    NewSoundProxy->AddToWorld();
                }
                return NewSoundProxy;
            }

            Audio::SoundProxy* SoundScapeManager::CreateSoundProxy(const UInt16 Type, Resource::DataStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config, const Boole AddToWorld)
            {
                if( this->Initialized == false )
                    { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Cannot create a new SoundProxy without an audio device being initialized."); }

                OALS::AudioManager* AudioMan = static_cast<OALS::AudioManager*>( Audio::AudioManager::GetSingletonPtr() );
                iDecoderFactory* Factory = AudioMan->GetDecoderFactory(Audio::Enc_RAW);
                if( Factory == NULL )
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Unsupported encoding requested.  Did you enable the proper encoding in CMake?"); }

                iDecoder* SoundDecoder = static_cast<RawDecoderFactory*>(Factory)->CreateDecoder(Stream,Frequency,Config);
                OALS::SoundProxy* NewSoundProxy = new OALS::SoundProxy(Type,SoundDecoder,this->Contexts,this);
                this->Proxies.push_back(NewSoundProxy);
                if( AddToWorld ) {
                    NewSoundProxy->AddToWorld();
                }
                return NewSoundProxy;
            }

            Audio::SoundProxy* SoundScapeManager::CreateSoundProxy(const UInt16 Type, const String& FileName, const String& Group, const Boole AddToWorld)
            {
                if( this->Initialized == false )
                    { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Cannot create a new SoundProxy without an audio device being initialized."); }

                // Setup our needed parameters
                Audio::Encoding Encode = Audio::Enc_RAW;
                Resource::DataStreamPtr SoundStream = Resource::ResourceManager::GetSingletonPtr()->OpenAssetStream(FileName,Group);

                // Figure out the encoding from the filename
                String Extension = FileName.substr(FileName.find_last_of(".")+1);
                StringTools::ToLowerCase(Extension);
                if( Extension == "wav" ) {
                    Encode = Audio::Enc_WAV;
                }else if( Extension == "flac" ) {
                    Encode = Audio::Enc_FLAC;
                }else if( Extension == "ogg" ) {
                    Encode = Audio::Enc_VORBIS;
                }else if( Extension == "mp3" ) {
                    Encode = Audio::Enc_MP3;
                }else if( Extension == "spx" ) {
                    Encode = Audio::Enc_SPEEX;
                }else if( Extension == "opus" ) {
                    Encode = Audio::Enc_OPUS;
                }else{
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting playback of audio with unsupported encoding.");
                }

                return this->CreateSoundProxy(Type,SoundStream,Encode,AddToWorld);
            }

            Audio::SoundProxy* SoundScapeManager::CreateSoundProxy(const UInt16 Type, const String& StreamName, Char8* Buffer, const UInt32 Length, const Audio::Encoding Encode, const Boole AddToWorld)
            {
                if( this->Initialized == false )
                    { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Cannot create a new SoundProxy without an audio device being initialized."); }

                // Create our stream and get on with it
                Resource::DataStreamPtr SoundStream = Resource::ResourceManager::GetSingletonPtr()->CreateDataStream(StreamName,Buffer,Length);

                return this->CreateSoundProxy(Type,SoundStream,Encode,AddToWorld);
            }

            Audio::SoundProxy* SoundScapeManager::CreateSoundProxy(const UInt16 Type, const String& StreamName, Char8* Buffer, const UInt32 Length, const UInt32 Frequency, const Audio::BitConfig Config, const Boole AddToWorld)
            {
                if( this->Initialized == false )
                    { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Cannot create a new SoundProxy without an audio device being initialized."); }

                // Create our stream and get on with it
                Resource::DataStreamPtr SoundStream = Resource::ResourceManager::GetSingletonPtr()->CreateDataStream(StreamName,Buffer,Length);

                return this->CreateSoundProxy(Type,SoundStream,Frequency,Config,AddToWorld);
            }

            Audio::SoundProxy* SoundScapeManager::GetSoundProxy(const UInt32 Index) const
            {
                return this->Proxies.at(Index);
            }

            UInt32 SoundScapeManager::GetNumSoundProxies() const
            {
                return this->Proxies.size();
            }

            void SoundScapeManager::DestroySoundProxy(Audio::SoundProxy* ToBeDestroyed)
            {
                for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
                {
                    if( (*ProxIt) == ToBeDestroyed )
                    {
                        delete ToBeDestroyed;
                        this->Proxies.erase(ProxIt);
                    }
                }
            }

            void SoundScapeManager::DestroyAllProxies()
            {
                for( ProxyIterator ProxIt = this->Proxies.begin() ; ProxIt != this->Proxies.end() ; ++ProxIt )
                {
                    delete (*ProxIt);
                }
                this->Proxies.clear();
            }

            SoundScapeManager::ProxyIterator SoundScapeManager::BeginSoundProxy()
                { return this->Proxies.begin(); }

            SoundScapeManager::ProxyIterator SoundScapeManager::EndSoundProxy()
                { return this->Proxies.end(); }

            SoundScapeManager::ConstProxyIterator SoundScapeManager::BeginSoundProxy() const
                { return this->Proxies.begin(); }

            SoundScapeManager::ConstProxyIterator SoundScapeManager::EndSoundProxy() const
                { return this->Proxies.end(); }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void SoundScapeManager::Pause(const UInt32 PL)
            {
                // Do nothing currently
            }

            void SoundScapeManager::Initialize()
            {
                if( !this->Initialized )
                {
                    WorldManager::Initialize();

                    OALS::AudioManager* AudioMan = static_cast<OALS::AudioManager*>(Audio::AudioManager::GetSingletonPtr());
                    if( AudioMan == NULL )
                        { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"A valid AudioManager is required for SoundScape operations."); }

                    if( AudioMan->IsInitialized() == false )
                    {
                        // Attempt to initialize
                        if( AudioMan->InitializePlaybackDevice(AudioMan->GetDefaultPlaybackDeviceName()) == false )
                            { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Failed to initialize AudioManager prior to SoundScapeManager.  An open Audio device is required."); }
                    }

                    this->TheEntresol->GetScheduler().AddWorkUnitMain( this->BufferUpdate3DWork, "AudioBufferUpdate3DWork" );
                    this->BufferUpdate3DWork->AddDependency( AudioMan->GetBufferUpdate2DWork() );

                    this->Initialized = true;
                }
            }

            void SoundScapeManager::Deinitialize()
            {
                if( this->Initialized )
                {
                    this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->BufferUpdate3DWork );
                    this->BufferUpdate3DWork->ClearDependencies();
                    this->Initialized = false;
                }
            }

            iBufferUpdate3DWorkUnit* SoundScapeManager::GetBufferUpdate3DWork()
            {
                return this->BufferUpdate3DWork;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Type Identifier Methods

            String SoundScapeManager::GetImplementationTypeName() const
            {
                return OALS::SoundScapeManager::ImplementationName;
            }
        }//OALS
    }//Audio
}//Mezzanine

#endif
