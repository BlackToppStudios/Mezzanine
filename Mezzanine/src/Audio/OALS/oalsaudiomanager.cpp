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
#ifndef _audiooalsaudiomanager_cpp
#define _audiooalsaudiomanager_cpp

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#define OALS_STRUCTS_DECLARED

#include "Audio/OALS/oalsaudiomanager.h"
#include "Audio/OALS/oalsrecorder.h"
#include "Audio/OALS/oalssound.h"
#include "Audio/OALS/oalseffectshandler.h"
#include "Audio/OALS/oalssoundscapemanager.h"

#include "Audio/rawdecoderfactory.h"
#include "Audio/musicplayer.h"

#include "entresol.h"
#include "stringtool.h"
#include "exception.h"
#include "resourcemanager.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include <memory>
#include <algorithm>
#include <cstring>

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            OALS::AudioManager::AudioManager()
                : InternalDevice(NULL),
                  NonSpacialContext(NULL),
                  EffHandler(NULL),
                  MPlayer(NULL),
                  MasterMute(false),
                  MasterVolume(1.0),
                  ContextOutputFrequency(44100)
            {
                this->Priority = 55;
                this->AutoGenFiles = false;
                this->GetAvailableDeviceNames();
            }

            OALS::AudioManager::AudioManager(XML::Node& XMLNode)
                : InternalDevice(NULL),
                  NonSpacialContext(NULL),
                  EffHandler(NULL),
                  MPlayer(NULL),
                  MasterMute(false),
                  MasterVolume(1.0),
                  ContextOutputFrequency(44100)
            {
                /// @todo Research possibly moving XML settings serialization to the AudioManager base class.
                /// The likelyhood of having implementation specific settings being serialized is somewhat low.
                /// Doing so would make settings files be implementation agnostic.
                this->Priority = 55;
                this->GetAvailableDeviceNames();

                XML::Attribute CurrAttrib;
                String PathPreset;
                // Get whether or not to autogen the directory path and settings file.
                XML::Node AutoGenNode = XMLNode.GetChild("AutoCreateSettings");
                if(!AutoGenNode.Empty())
                {
                    CurrAttrib = AutoGenNode.GetAttribute("Auto");
                    if(!CurrAttrib.Empty())
                        AutoGenPath = AutoGenFiles = StringTools::ConvertToBool( CurrAttrib.AsString() );
                }
                // Get preset path to default to if a path is not provided.
                XML::Node PathNode = XMLNode.GetChild("SettingsPath");
                if(!PathNode.Empty())
                {
                    CurrAttrib = PathNode.GetAttribute("Path");
                    if(!CurrAttrib.Empty())
                        PathPreset = CurrAttrib.AsString();

                    if(!PathPreset.empty())
                        SetSettingsFilePath(PathPreset);
                }
                // Get the files to be loaded, and load them.
                XML::Node FilesNode = XMLNode.GetChild("SettingsFiles");
                if(!FilesNode.Empty())
                {
                    for( XML::NodeIterator SetFileIt = FilesNode.begin() ; SetFileIt != FilesNode.end() ; ++SetFileIt )
                    {
                        String FileName, FilePath, FileGroup;
                        // Get the filename to load
                        CurrAttrib = (*SetFileIt).GetAttribute("FileName");
                        if(!CurrAttrib.Empty())
                            FileName = CurrAttrib.AsString();
                        // Get the path
                        CurrAttrib = (*SetFileIt).GetAttribute("Path");
                        if(!CurrAttrib.Empty())
                            FilePath = CurrAttrib.AsString();
                        else
                        {
                            CurrAttrib = (*SetFileIt).GetAttribute("Group");
                            if(!CurrAttrib.Empty())
                                FileGroup = CurrAttrib.AsString();
                        }

                        if(FilePath.empty())
                        {
                            if(FileGroup.empty()) LoadSettings(FileName);
                            else LoadSettingsFromGroup(FileName,FileGroup);
                        }
                        else LoadSettings(FileName,FilePath);
                    }
                }
            }

            OALS::AudioManager::~AudioManager()
            {
                if( this->AutoGenFiles )
                    this->SaveAllSettings();

                this->DestroyAllSounds();
                this->EffHandler->DestroyAllEffects();
                this->EffHandler->DestroyAllFilters();
                this->DestroyAllRecorders();
                this->ShutdownPlaybackDevice();
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
                    MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,ExceptionStream.str());
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
                // Create and initialize the volume settings
                XML::Node VolumeSettingsNode = CurrentSettings.AppendChild("Volume");
                VolumeSettingsNode.AppendAttribute("Ambient").SetValue( StringTools::ConvertToString(this->GetAmbientVolume()) );
                VolumeSettingsNode.AppendAttribute("Dialog").SetValue( StringTools::ConvertToString(this->GetDialogVolume()) );
                VolumeSettingsNode.AppendAttribute("Effects").SetValue( StringTools::ConvertToString(this->GetEffectVolume()) );
                VolumeSettingsNode.AppendAttribute("Music").SetValue( StringTools::ConvertToString(this->GetMusicVolume()) );
                VolumeSettingsNode.AppendAttribute("Master").SetValue( StringTools::ConvertToString(this->GetMasterVolume()) );
                // Create and initialize the mute setting
                XML::Node MuteSettingNode = CurrentSettings.AppendChild("Mute");
                MuteSettingNode.AppendAttribute("Muted").SetValue( StringTools::ConvertToString(this->IsMuted()) );
            }

            void OALS::AudioManager::ApplySettingGroupImpl(ObjectSettingGroup* Group)
            {
                for( ObjectSettingSetContainer::SubSetIterator SubSetIt = Group->SubSetBegin() ; SubSetIt != Group->SubSetEnd() ; ++SubSetIt )
                {
                    String CurrSettingValue;
                    if( "PlaybackDeviceSettings" == (*SubSetIt)->GetName() )
                    {
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

                        if( "Default" == DeviceName )
                        {
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
                            Entresol::GetSingletonPtr()->Log("WARNING: Attempting to apply new device settings after the AudioManager has been initialized.  "
                                                          "These Settings will be applied the next time settings are loaded during manager construction if current settings are saved.");
                            this->CurrentDeviceName = DeviceName;
                            this->ContextOutputFrequency = OutputFreq;
                        }
                    }
                    else if( "Volume" == (*SubSetIt)->GetName() )
                    {
                        // Setup the data to populate
                        Real AmbientVol = -1.0;
                        Real DialogVol = -1.0;
                        Real EffectVol = -1.0;
                        Real MusicVol = -1.0;
                        Real MasterVol = -1.0;
                        // Get the values
                        CurrSettingValue = (*SubSetIt)->GetSettingValue("Ambient");
                        if(!CurrSettingValue.empty())
                            AmbientVol = StringTools::ConvertToReal(CurrSettingValue);
                        CurrSettingValue = (*SubSetIt)->GetSettingValue("Dialog");
                        if(!CurrSettingValue.empty())
                            DialogVol = StringTools::ConvertToReal(CurrSettingValue);
                        CurrSettingValue = (*SubSetIt)->GetSettingValue("Effects");
                        if(!CurrSettingValue.empty())
                            EffectVol = StringTools::ConvertToReal(CurrSettingValue);
                        CurrSettingValue = (*SubSetIt)->GetSettingValue("Music");
                        if(!CurrSettingValue.empty())
                            MusicVol = StringTools::ConvertToReal(CurrSettingValue);
                        CurrSettingValue = (*SubSetIt)->GetSettingValue("Master");
                        if(!CurrSettingValue.empty())
                            MasterVol = StringTools::ConvertToReal(CurrSettingValue);
                        // Set the values
                        if( 0.0 <= AmbientVol ) this->SetAmbientVolume(AmbientVol);
                        if( 0.0 <= DialogVol ) this->SetDialogVolume(DialogVol);
                        if( 0.0 <= EffectVol ) this->SetEffectVolume(EffectVol);
                        if( 0.0 <= MusicVol ) this->SetMusicVolume(MusicVol);
                        if( 0.0 <= MasterVol ) this->SetMasterVolume(MasterVol);
                    }
                    else if( "Mute" == (*SubSetIt)->GetName() )
                    {
                        // Setup the data to populate
                        bool MuteState = false;
                        // Get the values
                        CurrSettingValue = (*SubSetIt)->GetSettingValue("Muted");
                        if(!CurrSettingValue.empty())
                            MuteState = StringTools::ConvertToBool(CurrSettingValue,false);
                        // Set the values
                        this->Mute(MuteState);
                    }
                }
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
            // Utility

            iEffectsHandler* OALS::AudioManager::GetEffectsHandler() const
            {
                return this->EffHandler;
            }

            MusicPlayer* OALS::AudioManager::GetMusicPlayer() const
            {
                return this->MPlayer;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Sound Management

            iSound* OALS::AudioManager::CreateSound(const UInt16 Type)
            {
                if( this->Initialized == false )
                    { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Cannot create a new Sound without an audio device being initialized."); }

                OALS::Sound* NewSound = new OALS::Sound(Type,NULL,this->NonSpacialContext);
                Sounds.push_back(NewSound);
                return NewSound;
            }

            iSound* OALS::AudioManager::CreateSound(const UInt16 Type, Resource::DataStreamPtr Stream, const Audio::Encoding Encode)
            {
                if( this->Initialized == false )
                    { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Cannot create a new Sound without an audio device being initialized."); }

                iDecoderFactory* Factory = this->GetDecoderFactory(Encode);
                if( Factory == NULL )
                    { MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Unsupported encoding requested.  Did you enable the proper encoding in CMake?"); }

                iDecoder* SoundDecoder = Factory->CreateDecoder(Stream);
                OALS::Sound* NewSound = new OALS::Sound(Type,SoundDecoder,this->NonSpacialContext);
                Sounds.push_back(NewSound);
                return NewSound;
            }

            iSound* OALS::AudioManager::CreateSound(const UInt16 Type, Resource::DataStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config)
            {
                if( this->Initialized == false )
                    { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Cannot create a new Sound without an audio device being initialized."); }

                iDecoderFactory* Factory = this->GetDecoderFactory(Audio::Enc_RAW);
                if( Factory == NULL )
                    { MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Unsupported encoding requested.  Did you enable the proper encoding in CMake?"); }

                iDecoder* SoundDecoder = static_cast<RawDecoderFactory*>(Factory)->CreateDecoder(Stream,Frequency,Config);
                OALS::Sound* NewSound = new OALS::Sound(Type,SoundDecoder,this->NonSpacialContext);
                Sounds.push_back(NewSound);
                return NewSound;
            }

            iSound* OALS::AudioManager::CreateSound(const UInt16 Type, const String& FileName, const String& Group)
            {
                if( this->Initialized == false )
                    { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Cannot create a new Sound without an audio device being initialized."); }

                // Setup our needed parameters
                Audio::Encoding Encode = Audio::Enc_RAW;
                Resource::DataStreamPtr SoundStream = ResourceManager::GetSingletonPtr()->OpenAssetStream(FileName,Group);

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
                    MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Attempting playback of audio with unsupported encoding.");
                }

                return this->CreateSound(Type,SoundStream,Encode);
            }

            iSound* OALS::AudioManager::CreateSound(const UInt16 Type, const String& SoundName, Char8* Buffer, const UInt32 Length, const Audio::Encoding Encode)
            {
                if( this->Initialized == false )
                    { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Cannot create a new Sound without an audio device being initialized."); }

                // Create our stream and get on with it
                Resource::DataStreamPtr SoundStream = ResourceManager::GetSingletonPtr()->CreateDataStream(SoundName,Buffer,Length);

                return this->CreateSound(Type,SoundStream,Encode);
            }

            iSound* OALS::AudioManager::CreateSound(const UInt16 Type, const String& SoundName, Char8* Buffer, const UInt32 Length, const UInt32 Frequency, const Audio::BitConfig Config)
            {
                if( this->Initialized == false )
                    { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Cannot create a new Sound without an audio device being initialized."); }

                // Create our stream and get on with it
                Resource::DataStreamPtr SoundStream = ResourceManager::GetSingletonPtr()->CreateDataStream(SoundName,Buffer,Length);

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
                return ( Handler->Mute || this->IsMuted() ? 0 : Handler->Volume );
            }

            void OALS::AudioManager::MuteType(const UInt16 Type, bool Enable)
            {
                SoundTypeHandler* Handler = this->GetOrCreateSoundTypeHandler(Type);
                Handler->Mute = Enable;
            }

            bool OALS::AudioManager::IsTypeMuted(const UInt16 Type) const
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

            void OALS::AudioManager::Mute(bool Enable)
            {
                this->MasterMute = Enable;
            }

            bool OALS::AudioManager::IsMuted() const
            {
                return this->MasterMute;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Recorder Management

            iRecorder* OALS::AudioManager::CreateRecorder()
            {
                if( this->Initialized == false )
                    { MEZZ_EXCEPTION(Exception::INVALID_STATE_EXCEPTION,"Cannot create a new Recorder without an audio device being initialized."); }

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

            bool OALS::AudioManager::PlaybackDeviceNameValid(const String& DeviceName) const
            {
                for( StringVector::const_iterator NameIt = AvailablePlaybackDevices.begin() ; NameIt != this->AvailablePlaybackDevices.end() ; ++NameIt )
                {
                    if( DeviceName == (*NameIt) )
                        return true;
                }
                return false;
            }

            bool OALS::AudioManager::InitializePlaybackDevice(const String& DeviceName, const Integer OutputFrequency)
            {
                /// @todo We should create a "reinitialize" method that preserves sources.
                if( this->Initialized )
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

                this->Initialized = true;
            }

            void OALS::AudioManager::ShutdownPlaybackDevice()
            {
                if( this->Initialized )
                {
                    // Eradicate all sounds
                    this->DestroyAllSounds();
                    for( SoundScapeManagerIterator SSM = this->SoundScapeManagers.begin() ; SSM != this->SoundScapeManagers.end() ; ++SSM )
                    {
                        (*SSM)->DestroyAllSoundProxies();
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

                    this->Initialized = false;
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Recording Device Management

            String OALS::AudioManager::GetAvailableRecordingDeviceName(const UInt32 Index)
            {
                return this->AvailableRecorderDevices.at(Index);
            }

            UInt32 OALS::AudioManager::GetAvailableRecordingDeviceCount()
            {
                return this->AvailableRecorderDevices.size();
            }

            String OALS::AudioManager::GetDefaultRecordingDeviceName()
            {
                return alcGetString(NULL,ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Inherited from Managerbase

            void OALS::AudioManager::Initialize()
            {
                if( this->Initialized == false)
                    this->InitializePlaybackDevice(this->GetDefaultPlaybackDeviceName());
                if( this->AutoGenFiles )
                    this->SaveAllSettings();
                //this->Initialized = true;
            }

            void OALS::AudioManager::DoMainLoopItems()
            {
                // Update our non-spacialized sounds
                for( SoundIterator SoundIt = this->Sounds.begin() ; SoundIt != this->Sounds.end() ; ++SoundIt )
                {
                    (*SoundIt)->_Update();
                }

                // Clean any and all effects and filters that may be dirty
                this->EffHandler->_CleanAll();
                // Update our music player
                this->MPlayer->Update();
            }

            String OALS::AudioManager::GetImplementationTypeName() const
            {
                return "OALSAudioManager";
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
