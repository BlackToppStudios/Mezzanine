// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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
// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio-ZLIBLicense.txt
#ifndef _audiooalsaudiomanager_cpp
#define _audiooalsaudiomanager_cpp

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#define OALS_STRUCTS_DECLARED

#include "Audio/rawdecoderfactory.h"
#include "Audio/musicplayer.h"

#include "Audio/OALS/oalsrecorder.h"
#include "Audio/OALS/oalssound.h"
#include "Audio/OALS/oalseffectshandler.h"
#include "Audio/OALS/oalsdefines.h"

#include "Audio/OALS/oalsaudiomanager.h"
#include "Audio/OALS/oalssoundscapemanager.h"

#include "Resource/resourcemanager.h"

#include "entresol.h"
#include "stringtool.h"
#include "exception.h"

#include <memory>
#include <algorithm>
#include <cstring>

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            ///////////////////////////////////////////////////////////////////////////////
            // BufferUpdate2DWorkUnit Methods

            BufferUpdate2DWorkUnit::BufferUpdate2DWorkUnit(const BufferUpdate2DWorkUnit& Other)
                {  }

            BufferUpdate2DWorkUnit& BufferUpdate2DWorkUnit::operator=(const BufferUpdate2DWorkUnit& Other)
                { return *this; }

            BufferUpdate2DWorkUnit::BufferUpdate2DWorkUnit(OALS::AudioManager* Target) :
                TargetManager(Target) {  }

            BufferUpdate2DWorkUnit::~BufferUpdate2DWorkUnit()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void BufferUpdate2DWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
            {
                Logger& Log = CurrentThreadStorage.GetUsableLogger();
                Log << "Updating Audio Items." << std::endl;

                // Update our non-spacialized sounds
                for( OALS::AudioManager::SoundIterator SoundIt = this->TargetManager->Sounds.begin() ; SoundIt != this->TargetManager->Sounds.end() ; ++SoundIt )
                    { (*SoundIt)->_Update(); }

                // Update our music player
                this->TargetManager->MPlayer->Update();
            }

            ///////////////////////////////////////////////////////////////////////////////
            // EffectFilterCleanWorkUnit Methods

            EffectFilterCleanWorkUnit::EffectFilterCleanWorkUnit(const EffectFilterCleanWorkUnit& Other)
                {  }

            EffectFilterCleanWorkUnit& EffectFilterCleanWorkUnit::operator=(const EffectFilterCleanWorkUnit& Other)
                { return *this; }

            EffectFilterCleanWorkUnit::EffectFilterCleanWorkUnit(OALS::AudioManager* Target) :
                TargetManager(Target) {  }

            EffectFilterCleanWorkUnit::~EffectFilterCleanWorkUnit()
                {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void EffectFilterCleanWorkUnit::DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage)
            {
                // Clean any and all effects and filters that may be dirty
                this->TargetManager->EffHandler->_CleanAll();
            }

            ///////////////////////////////////////////////////////////////////////////////
            // AudioManager Methods

            const String OALS::AudioManager::ImplementationName = "OALSAudioManager";
            const ManagerBase::ManagerType OALS::AudioManager::InterfaceType = ManagerBase::MT_AudioManager;

            OALS::AudioManager::AudioManager() :
                InternalDevice(NULL),
                NonSpacialContext(NULL),
                EffHandler(NULL),
                MPlayer(NULL),

                BufferUpdate2DWork(NULL),
                EffectFilterCleanWork(NULL),
                ThreadResources(NULL),

                ContextOutputFrequency(44100),
                MasterVolume(1.0),
                MasterMute(false)
            {
                this->AutoGenFiles = false;
                this->GetAvailableDeviceNames();
                this->BufferUpdate2DWork = new BufferUpdate2DWorkUnit(this);
                this->EffectFilterCleanWork = new EffectFilterCleanWorkUnit(this);
            }

            OALS::AudioManager::AudioManager(XML::Node& XMLNode) :
                InternalDevice(NULL),
                NonSpacialContext(NULL),
                EffHandler(NULL),
                MPlayer(NULL),

                BufferUpdate2DWork(NULL),
                EffectFilterCleanWork(NULL),
                ThreadResources(NULL),

                ContextOutputFrequency(44100),
                MasterVolume(1.0),
                MasterMute(false)
            {
                /// @todo Research possibly moving XML settings serialization to the AudioManager base class.
                /// The likelyhood of having implementation specific settings being serialized is somewhat low.
                /// Doing so would make settings files be implementation agnostic.
                this->GetAvailableDeviceNames();
                this->BufferUpdate2DWork = new BufferUpdate2DWorkUnit(this);
                this->EffectFilterCleanWork = new EffectFilterCleanWorkUnit(this);

                this->ObjectSettingsHandler::ProtoDeSerialize(XMLNode);
            }

            OALS::AudioManager::~AudioManager()
            {
                this->DestroyAllSounds();
                if( this->EffHandler )
                {
                    this->EffHandler->DestroyAllEffects();
                    this->EffHandler->DestroyAllFilters();
                }
                this->DestroyAllRecorders();
                //this->ShutdownPlaybackDevice();

                this->Deinitialize();

                delete BufferUpdate2DWork;
                delete EffectFilterCleanWork;
            }

            SoundTypeHandler* OALS::AudioManager::GetOrCreateSoundTypeHandler(const UInt16 Type) const
            {
                SoundTypeHandler* Ret = GetSoundTypeHandler(Type);
                if( Ret == NULL )
                {
                    Ret = new SoundTypeHandler();
                    SoundsByType.insert( std::pair<UInt16,SoundTypeHandler*>(Type,Ret) );
                }
                return Ret;
            }

            SoundTypeHandler* OALS::AudioManager::GetSoundTypeHandler(const UInt16 Type) const
            {
                ConstSoundTypesIterator TypeIt = SoundsByType.find(Type);
                if( TypeIt != SoundsByType.end() ) return (*TypeIt).second;
                else return NULL;
            }

            SoundTypeHandler* OALS::AudioManager::GetSoundTypeHandlerExcept(const UInt16 Type) const
            {
                ConstSoundTypesIterator TypeIt = SoundsByType.find(Type);
                if( TypeIt == SoundsByType.end() )
                {
                    StringStream ExceptionStream;
                    ExceptionStream << "No SoundTypeHandler of Type \"" << Type << "\" found.";
                    MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,ExceptionStream.str());
                }
                else return (*TypeIt).second;
            }

            String OALS::AudioManager::GetObjectRootNodeName() const
            {
                return "OALSAudioManagerSettings";
            }

            void OALS::AudioManager::AppendCurrentSettings(XML::Node& SettingsRootNode)
            {
                // Create the Group node to be returned
                XML::Node CurrentSettings = SettingsRootNode.AppendChild("Current");
                // Create and initialize the device settings
                XML::Node PlaybackDeviceSettingsNode = CurrentSettings.AppendChild("PlaybackDeviceSettings");
                PlaybackDeviceSettingsNode.AppendAttribute("DeviceName").SetValue( this->GetCurrentPlaybackDeviceName() );
                PlaybackDeviceSettingsNode.AppendAttribute("OutputFrequency").SetValue( StringTools::ConvertToString(this->ContextOutputFrequency) );
                // Create and initialize the master settings
                XML::Node MasterSettingsNode = CurrentSettings.AppendChild("MasterSettings");
                MasterSettingsNode.AppendAttribute("Volume").SetValue( StringTools::ConvertToString( this->MasterVolume ) );
                MasterSettingsNode.AppendAttribute("Mute").SetValue( StringTools::ConvertToString( this->MasterMute ) );
                // Create and initialize the volume settings
                XML::Node VolumeSettingsNode = CurrentSettings.AppendChild("SettingsByType");
                for( SoundTypesIterator TypeIt = this->SoundsByType.begin() ; TypeIt != this->SoundsByType.end() ; ++TypeIt )
                {
                    XML::Node TypeNode = VolumeSettingsNode.AppendChild("Type");
                    TypeNode.AppendAttribute("ID").SetValue( StringTools::ConvertToString( (*TypeIt).first ) );
                    TypeNode.AppendAttribute("Volume").SetValue( StringTools::ConvertToString( (*TypeIt).second->Volume ) );
                    TypeNode.AppendAttribute("Mute").SetValue( StringTools::ConvertToString( (*TypeIt).second->Mute ) );
                }
            }

            void OALS::AudioManager::ApplySettingGroupImpl(ObjectSettingGroup* Group)
            {
                for( ObjectSettingSetContainer::SubSetIterator SubSetIt = Group->SubSetBegin() ; SubSetIt != Group->SubSetEnd() ; ++SubSetIt )
                {
                    String CurrSettingValue;
                    if( "PlaybackDeviceSettings" == (*SubSetIt)->GetName() ) {
                        // Setup the data to populate
                        String DeviceName("Default");
                        Integer OutputFreq = -1;
                        // Get the values
                        CurrSettingValue = (*SubSetIt)->GetSettingValue("DeviceName");
                        if(!CurrSettingValue.empty())
                            DeviceName = CurrSettingValue;
                        CurrSettingValue = (*SubSetIt)->GetSettingValue("OutputFrequency");
                        if(!CurrSettingValue.empty())
                            OutputFreq = StringTools::ConvertToInteger(CurrSettingValue);

                        if( "Default" == DeviceName ) {
                            DeviceName = this->GetDefaultPlaybackDeviceName();
                        }else{
                            if( !this->PlaybackDeviceNameValid(DeviceName) )
                                DeviceName = this->GetDefaultPlaybackDeviceName();
                        }
                        // Check if everything is initialized and set the settings appropriately
                        if(Initialized == false)
                        {
                            this->InitializePlaybackDevice(DeviceName,OutputFreq);
                        }else{
                            /// @todo May want to make some other data member so that people can accurately get what is set now, instead of what will be set.
                            Entresol::GetSingletonPtr()->_Log("WARNING: Attempting to apply new device settings after the AudioManager has been initialized.  "
                                                          "These Settings will be applied the next time settings are loaded during manager construction if current settings are saved.");
                            this->CurrentDeviceName = DeviceName;
                            this->ContextOutputFrequency = OutputFreq;
                        }
                    }else if( "MasterSettings" == (*SubSetIt)->GetName() ) {
                        // Setup the data to populate
                        Real Volume = 1.0;
                        Boole Mute = false;
                        // Get the values
                        CurrSettingValue = (*SubSetIt)->GetSettingValue("Volume");
                        if(!CurrSettingValue.empty())
                            Volume = StringTools::ConvertToReal(CurrSettingValue);
                        CurrSettingValue = (*SubSetIt)->GetSettingValue("Mute");
                        if(!CurrSettingValue.empty())
                            Mute = StringTools::ConvertToBool(CurrSettingValue);

                        this->SetMasterVolume(Volume);
                        this->SetMasterMute(Mute);
                    }else if( "SettingsByType" == (*SubSetIt)->GetName() ) {
                        for( ObjectSettingSetContainer::SubSetIterator TypeIt = (*SubSetIt)->SubSetBegin() ; TypeIt != (*SubSetIt)->SubSetEnd() ; ++TypeIt )
                        {
                            // Setup the data to populate
                            UInt16 TypeID = 0;
                            Real Volume = 1.0;
                            Boole Mute = false;
                            // Get the values
                            CurrSettingValue = (*TypeIt)->GetSettingValue("ID");
                            if( !CurrSettingValue.empty() )
                                TypeID = StringTools::ConvertToReal(CurrSettingValue);
                            CurrSettingValue = (*TypeIt)->GetSettingValue("Volume");
                            if( !CurrSettingValue.empty() )
                                Volume = StringTools::ConvertToReal(CurrSettingValue);
                            CurrSettingValue = (*TypeIt)->GetSettingValue("Mute");
                            if( !CurrSettingValue.empty() )
                                Mute = StringTools::ConvertToBool(CurrSettingValue);

                            if( TypeID > 0 ) {
                                SoundTypeHandler* Handler = this->GetOrCreateSoundTypeHandler(TypeID);
                                Handler->Volume = Volume;
                                Handler->Mute = Mute;
                            }// if type id
                        }// for each type
                    }// if "SettingsByType"
                }// for each node
            }

            void OALS::AudioManager::GetAvailableDeviceNames()
            {
                // Get our Playback Device list
                const char* pDeviceList = NULL;
                if( alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT") == AL_TRUE )
                {
                    pDeviceList = alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);
                }
                else if( alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT") == AL_TRUE )
                {
                    pDeviceList = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
                }
                // Go through our playback devices and update our string vectors
                if( pDeviceList )
                {
                    while( *pDeviceList )
                    {
                        String device(pDeviceList);
                        this->AvailablePlaybackDevices.push_back(device);
                        pDeviceList += strlen(pDeviceList) + 1;
                    }
                }
                // Get our Recording Device list
                const char* rDeviceList = NULL;
                if( alcIsExtensionPresent(NULL, "ALC_EXT_CAPTURE") == AL_TRUE )
                {
                    rDeviceList = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);
                }
                // Go through our recorder devices and update our string vectors
                if( rDeviceList )
                {
                    while( *rDeviceList )
                    {
                        String device(rDeviceList);
                        this->AvailableRecorderDevices.push_back(device);
                        rDeviceList += strlen(rDeviceList) + 1;
                    }
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Sound Management

            iSound* OALS::AudioManager::CreateSound(const UInt16 Type)
            {
                if( this->Initialized == false )
                    { MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Cannot create a new Sound without an audio device being initialized."); }

                OALS::Sound* NewSound = new OALS::Sound(Type,NULL,this->NonSpacialContext);
                Sounds.push_back(NewSound);
                return NewSound;
            }

            iSound* OALS::AudioManager::CreateSound(const UInt16 Type, Resource::DataStreamPtr Stream, const Audio::Encoding Encode)
            {
                if( this->Initialized == false )
                    { MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Cannot create a new Sound without an audio device being initialized."); }

                iDecoderFactory* Factory = this->GetDecoderFactory(Encode);
                if( Factory == NULL )
                    { MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Unsupported encoding requested.  Did you enable the proper encoding in CMake?"); }

                iDecoder* SoundDecoder = Factory->CreateDecoder(Stream);
                OALS::Sound* NewSound = new OALS::Sound(Type,SoundDecoder,this->NonSpacialContext);
                Sounds.push_back(NewSound);
                return NewSound;
            }

            iSound* OALS::AudioManager::CreateSound(const UInt16 Type, Resource::DataStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config)
            {
                if( this->Initialized == false )
                    { MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Cannot create a new Sound without an audio device being initialized."); }

                iDecoderFactory* Factory = this->GetDecoderFactory(Audio::Enc_RAW);
                if( Factory == NULL )
                    { MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Unsupported encoding requested.  Did you enable the proper encoding in CMake?"); }

                iDecoder* SoundDecoder = static_cast<RawDecoderFactory*>(Factory)->CreateDecoder(Stream,Frequency,Config);
                OALS::Sound* NewSound = new OALS::Sound(Type,SoundDecoder,this->NonSpacialContext);
                Sounds.push_back(NewSound);
                return NewSound;
            }

            iSound* OALS::AudioManager::CreateSound(const UInt16 Type, const String& FileName, const String& Group)
            {
                if( this->Initialized == false )
                    { MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Cannot create a new Sound without an audio device being initialized."); }

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
                    MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Attempting playback of audio with unsupported encoding.");
                }

                return this->CreateSound(Type,SoundStream,Encode);
            }

            iSound* OALS::AudioManager::CreateSound(const UInt16 Type, const String& SoundName, Char8* Buffer, const UInt32 Length, const Audio::Encoding Encode)
            {
                if( this->Initialized == false )
                    { MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Cannot create a new Sound without an audio device being initialized."); }

                // Create our stream and get on with it
                Resource::DataStreamPtr SoundStream = Resource::ResourceManager::GetSingletonPtr()->CreateDataStream(SoundName,Buffer,Length);

                return this->CreateSound(Type,SoundStream,Encode);
            }

            iSound* OALS::AudioManager::CreateSound(const UInt16 Type, const String& SoundName, Char8* Buffer, const UInt32 Length, const UInt32 Frequency, const Audio::BitConfig Config)
            {
                if( this->Initialized == false )
                    { MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Cannot create a new Sound without an audio device being initialized."); }

                // Create our stream and get on with it
                Resource::DataStreamPtr SoundStream = Resource::ResourceManager::GetSingletonPtr()->CreateDataStream(SoundName,Buffer,Length);

                return this->CreateSound(Type,SoundStream,Frequency,Config);
            }

            iSound* OALS::AudioManager::GetSound(const UInt32 Index) const
            {
                return this->Sounds.at(Index);
            }

            UInt32 OALS::AudioManager::GetNumSounds() const
            {
                return this->Sounds.size();
            }

            void OALS::AudioManager::DestroySound(iSound* ToBeDestroyed)
            {
                for( SoundIterator SoundIt = this->Sounds.begin() ; SoundIt != this->Sounds.end() ; ++SoundIt )
                {
                    if( ToBeDestroyed == (*SoundIt) )
                    {
                        this->Sounds.erase(SoundIt);
                        delete ToBeDestroyed;
                        return;
                    }
                }
            }

            void OALS::AudioManager::DestroyAllSounds()
            {
                for( SoundIterator SoundIt = this->Sounds.begin() ; SoundIt != this->Sounds.end() ; ++SoundIt )
                {
                    delete (*SoundIt);
                }
                this->Sounds.clear();
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Volume Management

            void OALS::AudioManager::SetTypeVolume(const UInt16 Type, const Real Vol)
            {
                SoundTypeHandler* Handler = this->GetOrCreateSoundTypeHandler(Type);
                Handler->Volume = Vol;
            }

            Real OALS::AudioManager::GetTypeVolume(const UInt16 Type) const
            {
                SoundTypeHandler* Handler = this->GetOrCreateSoundTypeHandler(Type);
                return ( Handler != NULL ? Handler->Volume : 0 );
            }

            void OALS::AudioManager::MuteType(const UInt16 Type, Boole Enable)
            {
                SoundTypeHandler* Handler = this->GetOrCreateSoundTypeHandler(Type);
                Handler->Mute = Enable;
            }

            Boole OALS::AudioManager::IsTypeMuted(const UInt16 Type) const
            {
                SoundTypeHandler* Handler = this->GetOrCreateSoundTypeHandler(Type);
                return Handler->Mute || this->IsMuted();
            }

            void OALS::AudioManager::SetMasterVolume(const Real& Master)
            {
                this->MasterVolume = Master;
            }

            Real OALS::AudioManager::GetMasterVolume() const
            {
                return ( this->IsMuted() ? 0 : this->MasterVolume );
            }

            void OALS::AudioManager::SetMasterMute(Boole Enable)
            {
                this->MasterMute = Enable;
            }

            Boole OALS::AudioManager::IsMuted() const
            {
                return this->MasterMute;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Recorder Management

            iRecorder* OALS::AudioManager::CreateRecorder()
            {
                if( this->Initialized == false )
                    { MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Cannot create a new Recorder without an audio device being initialized."); }

                OALS::Recorder* NewRecorder = new OALS::Recorder();
                this->Recorders.push_back(NewRecorder);
                return NewRecorder;
            }

            iRecorder* OALS::AudioManager::GetRecorder(const UInt32 Index) const
            {
                return this->Recorders.at(Index);
            }

            UInt32 OALS::AudioManager::GetNumRecorders() const
            {
                return this->Recorders.size();
            }

            void OALS::AudioManager::DestroyRecorder(iRecorder* ToBeDestroyed)
            {
                for( RecorderIterator RecIt = Recorders.begin() ; RecIt != Recorders.end() ; ++RecIt )
                {
                    if( ToBeDestroyed == (*RecIt) )
                    {
                        this->Recorders.erase(RecIt);
                        delete ToBeDestroyed;
                        return;
                    }
                }
            }

            void OALS::AudioManager::DestroyAllRecorders()
            {
                for( RecorderIterator RecIt = Recorders.begin() ; RecIt != Recorders.end() ; ++RecIt )
                {
                    delete (*RecIt);
                }
                this->Recorders.clear();
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Playback Device Management

            String OALS::AudioManager::GetCurrentPlaybackDeviceName() const
            {
                return this->CurrentDeviceName;
                //return ( this->InternalDevice != NULL ? alcGetString(this->InternalDevice,ALC_DEVICE_SPECIFIER) : "");
            }

            String OALS::AudioManager::GetAvailablePlaybackDeviceName(const Whole& Index) const
            {
                return this->AvailablePlaybackDevices.at(Index);
            }

            Whole OALS::AudioManager::GetAvailablePlaybackDeviceCount() const
            {
                return this->AvailablePlaybackDevices.size();
            }

            String OALS::AudioManager::GetDefaultPlaybackDeviceName() const
            {
                return alcGetString(NULL,ALC_DEFAULT_DEVICE_SPECIFIER);
            }

            Boole OALS::AudioManager::PlaybackDeviceNameValid(const String& DeviceName) const
            {
                for( StringVector::const_iterator NameIt = AvailablePlaybackDevices.begin() ; NameIt != this->AvailablePlaybackDevices.end() ; ++NameIt )
                {
                    if( DeviceName == (*NameIt) )
                        return true;
                }
                return false;
            }

            Boole OALS::AudioManager::InitializePlaybackDevice(const String& DeviceName, const Integer OutputFrequency)
            {
                /// @todo We should create a "reinitialize" method that preserves sources.
                if( this->InternalDevice )
                    //this->ShutdownPlaybackDevice();
                    return false;

                // Set up our attrib data
                ALint Attribs[6] = { 0 };
                UInt8 AttribIndex = 0;
                // Sort out our frequency attrib
                if( OutputFrequency > 0 )
                {
                    Attribs[AttribIndex++] = ALC_FREQUENCY;
                    Attribs[AttribIndex++] = OutputFrequency;
                }
                // Sort out our effect slots attrib
                Attribs[AttribIndex++] = ALC_MAX_AUXILIARY_SENDS;
                Attribs[AttribIndex++] = OALS_SOURCE_MAX_EFFECT_SLOTS;

                // Create the device we'll use
                this->InternalDevice = alcOpenDevice(DeviceName.c_str());
                if( this->InternalDevice == NULL )
                {
                    // Well shit
                    return false;
                }

                // Create our context
                this->NonSpacialContext = alcCreateContext(this->InternalDevice,Attribs);
                if( this->NonSpacialContext == NULL )
                {
                    alcCloseDevice(this->InternalDevice);
                    this->InternalDevice = NULL;
                    return false;
                }

                if( alcMakeContextCurrent(this->NonSpacialContext) == false )
                {
                    alcDestroyContext(this->NonSpacialContext);
                    alcCloseDevice(this->InternalDevice);
                    this->NonSpacialContext = NULL;
                    this->InternalDevice = NULL;
                    return false;
                }

                this->CurrentDeviceName = DeviceName;
                this->ContextOutputFrequency = OutputFrequency;

                this->EffHandler = new OALS::EffectsHandler(this->InternalDevice);
                this->MPlayer = new MusicPlayer();

                return true;
            }

            void OALS::AudioManager::ShutdownPlaybackDevice()
            {
                if( this->InternalDevice )
                {
                    // Eradicate all sounds
                    this->DestroyAllSounds();
                    for( SoundScapeManagerIterator SSM = this->SoundScapeManagers.begin() ; SSM != this->SoundScapeManagers.end() ; ++SSM )
                    {
                        (*SSM)->DestroyAllProxies();
                        (*SSM)->DestroyAllListeners();
                    }

                    // Disable the context
                    alcMakeContextCurrent(NULL);
                    // Destroy the context
                    alcDestroyContext(this->NonSpacialContext);
                    this->NonSpacialContext = NULL;
                    // Destroy/close the device
                    alcCloseDevice(this->InternalDevice);
                    // If you are getting Segfaults in the line above, then linking is most likely broken between openAL
                    // and your Operating Systems default sound API.  For example, removing pulseaudio will cause Ubuntu 11.04 to Segfault with a SIGSEGV.
                    this->InternalDevice = NULL;

                    // Clean up effects as well
                    if( this->EffHandler )
                    {
                        delete this->EffHandler;
                        this->EffHandler = NULL;
                    }

                    // Get the music player too
                    if( this->MPlayer )
                    {
                        delete this->MPlayer;
                        this->MPlayer = NULL;
                    }
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Recording Device Management

            String OALS::AudioManager::GetAvailableRecordingDeviceName(const UInt32 Index)
                { return this->AvailableRecorderDevices.at(Index); }

            UInt32 OALS::AudioManager::GetAvailableRecordingDeviceCount()
                { return this->AvailableRecorderDevices.size(); }

            String OALS::AudioManager::GetDefaultRecordingDeviceName()
                { return alcGetString(NULL,ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER); }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            void OALS::AudioManager::Initialize()
            {
                if( !this->Initialized )
                {
                    if( this->InternalDevice == NULL ) {
                        if( this->InitializePlaybackDevice( this->GetDefaultPlaybackDeviceName() ) == false )
                            { MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Unable to initialize an audio device."); }
                    }

                    this->TheEntresol->GetScheduler().AddWorkUnitMain( this->BufferUpdate2DWork, "AudioBufferUpdate2DWork" );

                    this->TheEntresol->GetScheduler().AddWorkUnitMain( this->EffectFilterCleanWork, "AudioEffectFilterCleanWork" );
                    this->EffectFilterCleanWork->AddDependency( this->BufferUpdate2DWork );
                    /// @todo Some of this logic may need to be moved to the registration method if we want to support dynamic created and destroyed worlds.
                    /*for( SoundScapeManagerIterator SSM = this->SoundScapeManagers.begin() ; SSM != this->SoundScapeManagers.end() ; ++SSM )
                        this->EffectFilterCleanWork->AddDependency( (*SSM)->GetBufferUpdate3DWork() );// */

                    if( this->AutoGenFiles )
                        this->SaveAllSettings();

                    this->Initialized = true;
                }
            }

            void OALS::AudioManager::Deinitialize()
            {
                if( this->Initialized )
                {
                    if( this->InternalDevice != NULL ) {
                        this->ShutdownPlaybackDevice();
                    }

                    this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->BufferUpdate2DWork );
                    this->TheEntresol->GetScheduler().RemoveWorkUnitMain( this->EffectFilterCleanWork );

                    this->BufferUpdate2DWork->ClearDependencies();
                    this->EffectFilterCleanWork->ClearDependencies();

                    if( this->AutoGenFiles )
                        this->SaveAllSettings();

                    this->Initialized = false;
                }
            }

            iEffectsHandler* OALS::AudioManager::GetEffectsHandler() const
                { return this->EffHandler; }

            MusicPlayer* OALS::AudioManager::GetMusicPlayer() const
                { return this->MPlayer; }

            iBufferUpdate2DWorkUnit* OALS::AudioManager::GetBufferUpdate2DWork()
                { return this->BufferUpdate2DWork; }

            iEffectFilterCleanWorkUnit* OALS::AudioManager::GetEffectFilterCleanWork()
                { return this->EffectFilterCleanWork; }

            ///////////////////////////////////////////////////////////////////////////////
            // Type Identifier Methods

            String OALS::AudioManager::GetImplementationTypeName() const
            {
                return OALS::AudioManager::ImplementationName;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            ALCcontext* OALS::AudioManager::_GetNonSpacialContext() const
            {
                return this->NonSpacialContext;
            }

            ALCdevice* OALS::AudioManager::_GetAudioDevice() const
            {
                return this->InternalDevice;
            }

            void OALS::AudioManager::_RegisterSoundScapeManager(Audio::SoundScapeManager* Manager)
            {
                this->SoundScapeManagers.push_back( static_cast<OALS::SoundScapeManager*>(Manager) );
            }

            void OALS::AudioManager::_UnregisterSoundScapeManager(Audio::SoundScapeManager* Manager)
            {
                for( SoundScapeManagerIterator SSM = this->SoundScapeManagers.begin() ; SSM != this->SoundScapeManagers.end() ; ++SSM )
                {
                    if( static_cast<OALS::SoundScapeManager*>(Manager) == (*SSM) )
                    {
                        this->SoundScapeManagers.erase(SSM);
                        return;
                    }
                }
            }
        }//OALS
    }//Audio
}//Mezzanine

#endif
