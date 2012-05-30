//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _audiomanager_h
#define _audiomanager_h

#include "crossplatformexport.h"
#include "datatypes.h"
#include "managerbase.h"
#include "managerfactory.h"
#include "singleton.h"
#include "objectsettings.h"

namespace cAudio
{
    class IAudioManager;
    class IAudioSource;
    class IListener;
}

namespace Mezzanine
{
    namespace Audio
    {
        class Sound;
        class Listener;
        class SoundSet;
        class MusicPlayer;
    }

    // Used by the scripting language binder to help create bindgings for this class. SWIG does know to creation template instances
    #ifdef SWIG
    %template(SingletonAudioManager) Singleton<AudioManager>;
    #endif

    class AudioManagerInternalData;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class AudioManager
    /// @headerfile audiomanager.h
    /// @brief This is simply a place for storing all the Sound utilities and functions.
    /// @details This is a place for loading, storing, and running sound files as
    /// necessary in a given game.
    ///////////////////////////////////////////////////////////////////////////////
    class MEZZ_LIB AudioManager : public ManagerBase, public ObjectSettingsHandler, public Singleton<AudioManager>
    {
        protected:
            cAudio::IAudioManager* cAudioMan;
            AudioManagerInternalData* AMID;
            std::map< String, Audio::SoundSet* > SoundSets;
            std::map< String, Audio::Sound* > Sounds;
            std::vector< Audio::Sound* > AmbientSounds;
            std::vector< Audio::Sound* > DialogSounds;
            std::vector< Audio::Sound* > EffectSounds;
            std::vector< Audio::Sound* > MusicSounds;
            Real AmbientVolume;
            Real DialogVolume;
            Real EffectVolume;
            Real MusicVolume;
            Real MasterVolume;
            Real MuteStandby;
            bool Muted;
            Audio::Listener* Listener;
            Audio::MusicPlayer* MusicPlayer;
#ifdef MEZZXML
            virtual String GetObjectRootNodeName() const;
            virtual void AppendCurrentSettings(XML::Node& SettingsRootNode);
#endif
            virtual void ApplySettingGroupImpl(ObjectSettingGroup* Group);
        public:
            /// @brief Class Constructor
            /// @details This is the class constructor.  It gives you the option to start up the manager
            /// with default values if you choose.  If not then you'll need to setup and initialize the
            /// manager yourself after it is constructed.
            /// @param DefaultSettings Whether or not to load default settings and initialize the manager immediately.
            AudioManager(bool DefaultSettings=true);
#ifdef MEZZXML
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            AudioManager(XML::Node& XMLNode);
#endif
            /// @brief Class Destructor
            /// @details The class destructor.
            virtual ~AudioManager();

            ///////////////////////////////////////////////////////////////////////////////
            // Sound Management

            /// @brief Creates an ambient sound instance from a file that can be used to play sounds.
            /// @param SoundName The name of the Sound instance.
            /// @param FileName The name of the file.
            /// @param Group The resource group in which the file resides.
            /// @return Returns a pointer to the Sound Instance that was created.
            virtual Audio::Sound* CreateAmbientSound(ConstString& SoundName, ConstString& FileName, ConstString& Group);
            /// @brief Creates an dialog sound instance from a file that can be used to play sounds.
            /// @param SoundName The name of the Sound instance.
            /// @param FileName The name of the file.
            /// @param Group The resource group in which the file resides.
            /// @return Returns a pointer to the Sound Instance that was created.
            virtual Audio::Sound* CreateDialogSound(ConstString& SoundName, ConstString& FileName, ConstString& Group);
            /// @brief Creates an effect sound instance from a file that can be used to play sounds.
            /// @param SoundName The name of the Sound instance.
            /// @param FileName The name of the file.
            /// @param Group The resource group in which the file resides.
            /// @return Returns a pointer to the Sound Instance that was created.
            virtual Audio::Sound* CreateEffectSound(ConstString& SoundName, ConstString& FileName, ConstString& Group);
            /// @brief Creates an music sound instance from a file that can be used to play sounds.
            /// @param SoundName The name of the Sound instance.
            /// @param FileName The name of the file.
            /// @param Group The resource group in which the file resides.
            /// @return Returns a pointer to the Sound Instance that was created.
            virtual Audio::Sound* CreateMusicSound(ConstString& SoundName, ConstString& FileName, ConstString& Group);
            /// @brief Gets a sound by it's name.
            /// @return Returns a pointer to the specified sound.
            virtual Audio::Sound* GetSoundByName(const String& SoundName) const;
            /// @brief Deletes a Sound.
            /// @param SoundName A pointer to the Sound you want deleted.
            virtual void DestroySound(Audio::Sound* ToBeDestroyed);
            /// @brief Deletes all stored sounds.
            virtual void DestroyAllSounds();

            ///////////////////////////////////////////////////////////////////////////////
            // SoundSet Management

            /// @brief Creates a sound set.
            /// @details This function will create a sound set vector you can use to store similiar sound instances.
            /// @param SoundSetName The name you wish the sound set to have.
            /// @return Returns a pointer to the created Vector.
            virtual Audio::SoundSet* CreateSoundSet(const String& SoundSetName);
            /// @brief Gets an existing sound set.
            /// @details This function will find the specified sound set and return a pointer to it.
            /// @param SoundSetName The name of the sound set to retrieve.
            /// @return Returns a pointer to the named Vector.
            virtual Audio::SoundSet* GetSoundSet(const String& SoundSetName) const;
            /// @brief Add's a sound to the defined set.
            /// @details This function will add a sound instance to a created sound set.
            /// @param SoundName The sound instance to be added.
            virtual void AddSoundToSoundSet(const String& SoundSetName, Audio::Sound* SoundInst);

            ///////////////////////////////////////////////////////////////////////////////
            // Volume Management

            /// @brief Sets the volume for all stored Ambient sounds.
            /// @param Ambient The volume to apply to all Ambient sounds.
            virtual void SetAmbientVolume(const Real& Ambient);
            /// @brief Gets the currently set Ambient volume.
            /// @return Returns a Real representing the current Ambient volume.
            virtual Real GetAmbientVolume() const;
            /// @brief Sets the volume for all stored Dialog sounds.
            /// @param Dialog The volume to apply to all Dialog sounds.
            virtual void SetDialogVolume(const Real& Dialog);
            /// @brief Gets the currently set Dialog volume.
            /// @return Returns a Real representing the current Dialog volume.
            virtual Real GetDialogVolume() const;
            /// @brief Sets the volume for all stored Effect sounds.
            /// @param Effect The volume to apply to all Effect sounds.
            virtual void SetEffectVolume(const Real& Effect);
            /// @brief Gets the currently set Effect volume.
            /// @return Returns a Real representing the current Effect volume.
            virtual Real GetEffectVolume() const;
            /// @brief Sets the volume for all stored Music sounds.
            /// @param Music The volume to apply to all Music sounds.
            virtual void SetMusicVolume(const Real& Music);
            /// @brief Gets the currently set Music volume.
            /// @return Returns a Real representing the current Music volume.
            virtual Real GetMusicVolume() const;
            /// @brief Sets the volume for all stored sounds.
            /// @param Music The volume to apply to all sounds.
            virtual void SetMasterVolume(const Real& Master);
            /// @brief Gets the currently set Master volume.
            /// @return Returns a Real representing the current Master volume.
            virtual Real GetMasterVolume() const;
            /// @brief Sets whether or not to mute all Audio.
            /// @param Enable Whether or not all sounds should be muted.
            virtual void Mute(bool Enable);
            /// @brief Gets whether or not the Audio subsystem is muted.
            /// @return Returns a bool indicating whether or not the Audio subsystem is currently muted.
            virtual bool IsMuted() const;
            /// @brief Calls on all sounds stored in this manager to update their volume after a settings change.
            /// @note You shouldn't need to call this manually as it is automatically called every time a setting is changed.
            virtual void UpdateAllVolumes();

            ///////////////////////////////////////////////////////////////////////////////
            // Audio Device

            /// @brief Gets the name of an available device.
            /// @details This function will retrieve the name of a device by it's index on the sound managers device list.
            /// @param Index The position on the device list you wish to access the name of.
            /// @return Returns the name of the device.
            virtual String GetAvailableDeviceNameByIndex(const Whole& Index) const;
            /// @brief Gets the name of the default device.
            /// @details This function will return the name of the system default device.
            /// @return Returns the name of the default device.
            virtual String GetDefaultDeviceName() const;
            /// @brief Gets the number of available devices.
            /// @details This function will return the total number of available devices, including the default.
            /// @return Returns the number of available devices.
            virtual Whole GetAvailableDeviceCount() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Logging

            /// @brief This gets the logs that the audio subystem creates
            /// @details Internally the Mezzanine engine currently uses cAudio to process 3d sound. It has it's own
            /// logging system that we have customized to work with our logger.
            /// @return This gets the log of what actions the audio system has performed
            virtual std::stringstream* GetLogs() const;
            /// @brief This empties logs that the audio subystem creates
            /// @details Internally the Mezzanine engine currently uses cAudio to process 3d sound. It has it's own
            /// logging system that we have customized to work with our logger. This clears that data allow us to
            /// work with it as we need
            virtual void ClearLogs();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Initializes the Device.
            /// @details This function will initialize the device using the parameters provided.
            /// You need to call this function if you passed false into the sound manager constructor before
            /// you can use the manager.
            /// @param DeviceName The name of the device you wish to have this manager interface with/use.
            /// @param OutputFrequency Frequency of the output audio, -1 for the devices default.
            /// @param EAXEffectSlots The number of effects per sound allowed to be applied.
            virtual void InitializeDevice(ConstString& DeviceName, int OutputFrequency=-1, int EAXEffectSlots=4);
            /// @brief Retrieve's the listener for this sound manager.
            /// @details This function will return the listener for this manager which can be used to help create 3D sound.
            /// @return Returns a pointer to the managers Sound Listener.
            virtual Audio::Listener* GetListener() const;
            /// @brief Gets the Music Player for this sound manager.
            /// @return Returns a pointer to the managers Music Player.
            virtual Audio::MusicPlayer* GetMusicPlayer() const;
            /// @brief Gets the internal cAudioManager this manager is based on.
            /// @return Returns a pointer to the internal cAudio manager.
            virtual cAudio::IAudioManager* GetcAudioManager() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Inherited from Managerbase

            /// @copydoc Mezzanine::ManagerBase::Initialize()
            virtual void Initialize();
            /// @copydoc Mezzanine::ManagerBase::DoMainLoopItems()
            virtual void DoMainLoopItems();
            /// @copydoc ManagerBase::GetInterfaceType()
            virtual ManagerType GetInterfaceType() const;
            /// @copydoc ManagerBase::GetImplementationTypeName()
            virtual String GetImplementationTypeName() const;
    };//AudioManager

    ///////////////////////////////////////////////////////////////////////////////
    /// @class DefaultAudioManagerFactory
    /// @headerfile audiomanager.h
    /// @brief A factory responsible for the creation and destruction of the default audiomanager.
    ///////////////////////////////////////
    class MEZZ_LIB DefaultAudioManagerFactory : public ManagerFactory
    {
        public:
            /// @brief Class constructor.
            DefaultAudioManagerFactory();
            /// @brief Class destructor.
            virtual ~DefaultAudioManagerFactory();

            /// @copydoc ManagerFactory::GetManagerTypeName()
            String GetManagerTypeName() const;
            /// @copydoc ManagerFactory::CreateManager(NameValuePairList&)
            ManagerBase* CreateManager(NameValuePairList& Params);
#ifdef MEZZXML
            /// @copydoc ManagerFactory::CreateManager(XML::Node&)
            ManagerBase* CreateManager(XML::Node& XMLNode);
#endif
            /// @copydoc ManagerFactory::DestroyManager(ManagerBase*)
            void DestroyManager(ManagerBase* ToBeDestroyed);
    };//DefaultAudioManagerFactory
}//Mezzanine

#endif
