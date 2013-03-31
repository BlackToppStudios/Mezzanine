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
#ifndef _audiomanager_cpp
#define _audiomanager_cpp

#include "audiomanager.h"
#include "Audio/sound.h"
#include "Audio/listener.h"
#include "Audio/soundset.h"
#include "Audio/musicplayer.h"
#include "resourcemanager.h"
#include "entresol.h"
#include "stringtool.h"
#include <cAudio.h>

namespace Mezzanine
{
    template<> AudioManager* Singleton<AudioManager>::SingletonPtr = 0;

    class AudioManagerInternalData
    {
        public:
            AudioManagerInternalData(AudioManager* Man) : AudioMan(Man), OutputFrequency(-1), EAXEffectSlots(4) {  }
            ~AudioManagerInternalData() {  }

            AudioManager* AudioMan;
            String InitializedDevice;
            Integer OutputFrequency;
            Integer EAXEffectSlots;
    };

    AudioManager::AudioManager(bool DefaultSettings)
        : AmbientVolume(1.0),
          DialogVolume(1.0),
          EffectVolume(1.0),
          MusicVolume(1.0),
          MasterVolume(1.0),
          MuteStandby(0.0),
          Muted(false)
    {
        AMID = new AudioManagerInternalData(this);
        cAudioMan = cAudio::createAudioManager(DefaultSettings);
        //cAudioMan->getAvailableDevices();
        Listener = new Audio::Listener(cAudioMan->getListener());
        MusicPlayer = new Audio::MusicPlayer();
        this->Priority = 55;
        this->AutoGenFiles = false;
    }

    AudioManager::AudioManager(XML::Node& XMLNode)
        : AmbientVolume(1.0),
          DialogVolume(1.0),
          EffectVolume(1.0),
          MusicVolume(1.0),
          MasterVolume(1.0),
          MuteStandby(0.0),
          Muted(false)
    {
        AMID = new AudioManagerInternalData(this);
        cAudioMan = cAudio::createAudioManager(false);
        MusicPlayer = new Audio::MusicPlayer();
        this->Priority = 55;

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
        }else{
            InitializeDevice(GetDefaultDeviceName());
        }

        Listener = new Audio::Listener(cAudioMan->getListener());
    }

    AudioManager::~AudioManager()
    {
        SaveAllSettings();

        DestroyAllSounds();
        cAudio::destroyAudioManager(cAudioMan);
        delete AMID;
    }

    String AudioManager::GetObjectRootNodeName() const
    {
        return "DefaultAudioManagerSettings";
    }

    void AudioManager::AppendCurrentSettings(XML::Node& SettingsRootNode)
    {
        // Create the Group node to be returned
        XML::Node CurrentSettings = SettingsRootNode.AppendChild("Current");
        // Create and initialize the device settings
        XML::Node DeviceSettingsNode = CurrentSettings.AppendChild("DeviceSettings");
        DeviceSettingsNode.AppendAttribute("DeviceName").SetValue( AMID->InitializedDevice );
        DeviceSettingsNode.AppendAttribute("OutputFrequency").SetValue( StringTools::ConvertToString(AMID->OutputFrequency) );
        DeviceSettingsNode.AppendAttribute("EAXEffectSlots").SetValue( StringTools::ConvertToString(AMID->EAXEffectSlots) );
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

    void AudioManager::ApplySettingGroupImpl(ObjectSettingGroup* Group)
    {
        for( ObjectSettingSetContainer::SubSetIterator SubSetIt = Group->SubSetBegin() ; SubSetIt != Group->SubSetEnd() ; ++SubSetIt )
        {
            String CurrSettingValue;
            if( "DeviceSettings" == (*SubSetIt)->GetName() )
            {
                // Setup the data to populate
                String DeviceName("Default");
                Integer OutputFreq = -1;
                Integer EAXSlots = 4;
                // Get the values
                CurrSettingValue = (*SubSetIt)->GetSettingValue("DeviceName");
                if(!CurrSettingValue.empty())
                    DeviceName = CurrSettingValue;
                CurrSettingValue = (*SubSetIt)->GetSettingValue("OutputFrequency");
                if(!CurrSettingValue.empty())
                    OutputFreq = StringTools::ConvertToInteger(CurrSettingValue);
                CurrSettingValue = (*SubSetIt)->GetSettingValue("EAXEffectSlots");
                if(!CurrSettingValue.empty())
                    EAXSlots = StringTools::ConvertToInteger(CurrSettingValue);

                if( "Default" == DeviceName )
                {
                    DeviceName = GetDefaultDeviceName();
                }
                else
                {
                    if( !DeviceNameValid(DeviceName) )
                        DeviceName = GetDefaultDeviceName();
                }
                // Check if everything is initialized and set the settings appropriately
                if(!Initialized)
                {
                    InitializeDevice(DeviceName,OutputFreq,EAXSlots);
                }else{
                    /// @todo May want to make some other data member so that people can accurately get what is set now, instead of what will be set.
                    Entresol::GetSingletonPtr()->Log("WARNING: Attempting to apply new device settings after the AudioManager has been initialized.  "
                                                  "These Settings will be applied the next time settings are loaded during manager construction if current settings are saved.");
                    AMID->InitializedDevice = DeviceName;
                    AMID->OutputFrequency = OutputFreq;
                    AMID->EAXEffectSlots = EAXSlots;
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
                if( 0.0 <= AmbientVol ) SetAmbientVolume(AmbientVol);
                if( 0.0 <= DialogVol ) SetDialogVolume(DialogVol);
                if( 0.0 <= EffectVol ) SetEffectVolume(EffectVol);
                if( 0.0 <= MusicVol ) SetMusicVolume(MusicVol);
                if( 0.0 <= MasterVol ) SetMasterVolume(MasterVol);
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
                Mute(MuteState);
            }
        }
    }

    Audio::Sound* AudioManager::CreateAmbientSound(ConstString& SoundName, ConstString& FileName, ConstString& Group)
    {
        Audio::Sound* pSound = new Audio::Sound(SoundName,FileName,Group,Audio::ST_Ambient);
        Sounds.insert(std::pair<String,Audio::Sound*>(SoundName,pSound));
        AmbientSounds.push_back(pSound);
        return pSound;
    }

    Audio::Sound* AudioManager::CreateDialogSound(ConstString& SoundName, ConstString& FileName, ConstString& Group)
    {
        Audio::Sound* pSound = new Audio::Sound(SoundName,FileName,Group,Audio::ST_Dialog);
        Sounds.insert(std::pair<String,Audio::Sound*>(SoundName,pSound));
        DialogSounds.push_back(pSound);
        return pSound;
    }

    Audio::Sound* AudioManager::CreateEffectSound(ConstString& SoundName, ConstString& FileName, ConstString& Group)
    {
        Audio::Sound* pSound = new Audio::Sound(SoundName,FileName,Group,Audio::ST_Effect);
        Sounds.insert(std::pair<String,Audio::Sound*>(SoundName,pSound));
        EffectSounds.push_back(pSound);
        return pSound;
    }

    Audio::Sound* AudioManager::CreateMusicSound(ConstString& SoundName, ConstString& FileName, ConstString& Group)
    {
        Audio::Sound* pSound = new Audio::Sound(SoundName,FileName,Group,Audio::ST_Music);
        Sounds.insert(std::pair<String,Audio::Sound*>(SoundName,pSound));
        MusicSounds.push_back(pSound);
        return pSound;
    }

    Audio::Sound* AudioManager::GetSoundByName(const String& SoundName) const
    {
        std::map<String,Audio::Sound*>::const_iterator SoundIt = Sounds.find(SoundName);
        if(SoundIt != Sounds.end()) return (*SoundIt).second;
        else return NULL;
    }

    void AudioManager::DestroySound(Audio::Sound* ToBeDestroyed)
    {
        for( std::map<String,Audio::Sound*>::iterator It = Sounds.begin() ; It != Sounds.end() ; ++It )
        {
            if(ToBeDestroyed == (*It).second)
            {
                delete (*It).second;
                Sounds.erase(It);
                return;
            }
        }
    }

    void AudioManager::DestroyAllSounds()
    {
        for( std::map<String,Audio::Sound*>::iterator It = Sounds.begin() ; It != Sounds.end() ; ++It )
        {
            delete (*It).second;
        }
        Sounds.clear();
        cAudioMan->releaseAllSources();
    }

    Audio::SoundSet* AudioManager::CreateSoundSet(const String& SoundSetName)
    {
        Audio::SoundSet* Set = new Audio::SoundSet();
        this->SoundSets[SoundSetName] = Set;
        return Set;
    }

    Audio::SoundSet* AudioManager::GetSoundSet(const String& SoundSetName) const
    {
        std::map< String, Audio::SoundSet* >::const_iterator SS = this->SoundSets.find(SoundSetName);
        if(SS != SoundSets.end()) return (*SS).second;
        else return NULL;
    }

    void AudioManager::AddSoundToSoundSet(const String& SoundSetName, Audio::Sound* SoundInst)
    {
        Audio::SoundSet* Set = this->SoundSets[SoundSetName];
        Set->push_back(SoundInst);
    }

    void AudioManager::SetAmbientVolume(const Real& Ambient)
    {
        AmbientVolume = Ambient;
        for( std::vector<Audio::Sound*>::iterator it = AmbientSounds.begin() ; it != AmbientSounds.end() ; it++ )
        {
            (*it)->UpdateVolume();
        }
    }

    Real AudioManager::GetAmbientVolume() const
    {
        return AmbientVolume;
    }

    void AudioManager::SetDialogVolume(const Real& Dialog)
    {
        DialogVolume = Dialog;
        for( std::vector<Audio::Sound*>::iterator it = DialogSounds.begin() ; it != DialogSounds.end() ; it++ )
        {
            (*it)->UpdateVolume();
        }
    }

    Real AudioManager::GetDialogVolume() const
    {
        return DialogVolume;
    }

    void AudioManager::SetEffectVolume(const Real& Effect)
    {
        EffectVolume = Effect;
        for( std::vector<Audio::Sound*>::iterator it = EffectSounds.begin() ; it != EffectSounds.end() ; it++ )
        {
            (*it)->UpdateVolume();
        }
    }

    Real AudioManager::GetEffectVolume() const
    {
        return EffectVolume;
    }

    void AudioManager::SetMusicVolume(const Real& Music)
    {
        MusicVolume = Music;
        for( std::vector<Audio::Sound*>::iterator it = MusicSounds.begin() ; it != MusicSounds.end() ; it++ )
        {
            (*it)->UpdateVolume();
        }
    }

    Real AudioManager::GetMusicVolume() const
    {
        return MusicVolume;
    }

    void AudioManager::SetMasterVolume(const Real& Master)
    {
        MasterVolume = Master;
        UpdateAllVolumes();
    }

    Real AudioManager::GetMasterVolume() const
    {
        return MasterVolume;
    }

    void AudioManager::Mute(bool Enable)
    {
        if(Enable == Muted)
            return;
        if(Enable)
        {
            MuteStandby = MasterVolume;
            MasterVolume = 0;
        }else{
            MasterVolume = MuteStandby;
            MuteStandby = 0;
        }
        Muted = Enable;
        UpdateAllVolumes();
    }

    bool AudioManager::IsMuted() const
    {
        return Muted;
    }

    void AudioManager::UpdateAllVolumes()
    {
        for( std::map<String,Audio::Sound*>::iterator it = Sounds.begin() ; it != Sounds.end() ; it++ )
        {
            (*it).second->UpdateVolume();
        }
    }

    String AudioManager::GetAvailableDeviceNameByIndex(const Whole& Index) const
    {
        String Device(cAudioMan->getAvailableDeviceName(Index));
        return Device;
    }

    String AudioManager::GetDefaultDeviceName() const
    {
        String Device(cAudioMan->getDefaultDeviceName());
        if(0==Device.length())
        {
            if(cAudioMan->getAvailableDeviceCount())
            {
                Device = cAudioMan->getAvailableDeviceName(0);
            }else{
                MEZZ_EXCEPTION(Exception::INTERNAL_EXCEPTION,"No Available sound devices.");
            }
        }
        return Device;
    }

    Whole AudioManager::GetAvailableDeviceCount() const
    {
        return cAudioMan->getAvailableDeviceCount();
    }

    bool AudioManager::DeviceNameValid(const String& DeviceName) const
    {
        for( Whole Dev = 0 ; Dev < GetAvailableDeviceCount() ; Dev++ )
        {
            if( DeviceName == GetAvailableDeviceNameByIndex(Dev) )
                return true;
        }
        return false;
    }

    std::stringstream* AudioManager::GetLogs() const
    {
        return 0; //cAudioManager->GetLogs();
    }

    void AudioManager::ClearLogs()
    {
        //cAudioManager->ClearLogs();
    }

    void AudioManager::InitializeDevice(ConstString &DeviceName, int OutputFrequency, int EAXEffectSlots)
    {
        cAudioMan->initialize(DeviceName.c_str(), OutputFrequency, EAXEffectSlots);
        AMID->InitializedDevice = DeviceName;
        AMID->OutputFrequency = OutputFrequency;
        AMID->EAXEffectSlots = EAXEffectSlots;
        Initialized = true;
    }

    Audio::Listener* AudioManager::GetListener() const
    {
        return Listener;
    }

    Audio::MusicPlayer* AudioManager::GetMusicPlayer() const
    {
        return MusicPlayer;
    }

    cAudio::IAudioManager* AudioManager::GetcAudioManager() const
    {
        return cAudioMan;
    }

    void AudioManager::Initialize()
    {
        if(!Initialized)
            InitializeDevice(GetDefaultDeviceName(),-1,4);
        if(AutoGenFiles)
            SaveAllSettings();
        Initialized = true;
    }

    void AudioManager::DoMainLoopItems()
        { MusicPlayer->Update(); }

    ManagerBase::ManagerType AudioManager::GetInterfaceType() const
        { return ManagerBase::AudioManager; }

    String AudioManager::GetImplementationTypeName() const
        { return "DefaultAudioManager"; }

    ///////////////////////////////////////////////////////////////////////////////
    // DefaultAudioManagerFactory Methods

    DefaultAudioManagerFactory::DefaultAudioManagerFactory()
    {
    }

    DefaultAudioManagerFactory::~DefaultAudioManagerFactory()
    {
    }

    String DefaultAudioManagerFactory::GetManagerTypeName() const
    {
        return "DefaultAudioManager";
    }

    ManagerBase* DefaultAudioManagerFactory::CreateManager(NameValuePairList& Params)
    {
        if(AudioManager::SingletonValid())
        {
            /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
            return AudioManager::GetSingletonPtr();
        }else{
            if(Params.empty()) return new AudioManager();
            else
            {
                bool DefaultSettings = true;
                for( NameValuePairList::iterator ParIt = Params.begin() ; ParIt != Params.end() ; ++ParIt )
                {
                    String Lower = (*ParIt).first;
                    StringTools::ToLowerCase(Lower);
                    if( "defaultsettings" == Lower )
                    {
                        DefaultSettings = StringTools::ConvertToBool( (*ParIt).second );
                    }
                }
                return new AudioManager(DefaultSettings);
            }
        }
    }

    ManagerBase* DefaultAudioManagerFactory::CreateManager(XML::Node& XMLNode)
    {
        if(AudioManager::SingletonValid())
        {
            /// @todo Add something to log a warning that the manager exists and was requested to be constructed when we have a logging manager set up.
            return AudioManager::GetSingletonPtr();
        }else return new AudioManager(XMLNode);
    }

    void DefaultAudioManagerFactory::DestroyManager(ManagerBase* ToBeDestroyed)
    {
        delete ToBeDestroyed;
    }
}//Mezzanine

#endif
