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
#ifndef _audioaudiomanager_h
#define _audioaudiomanager_h

#include "datatypes.h"
#include "managerbase.h"
#include "managerfactory.h"
#include "singleton.h"
#include "objectsettings.h"
#include "audioenumerations.h"

namespace Mezzanine
{
    namespace Audio
    {
        class iSound;
        class iListener;
        class iRecorder;
        class iDecoder;
        class iDecoderFactory;
        class AudioManagerInternalData;

        // Used by the scripting language binder to help create bindgings for this class. SWIG does know to creation template instances
        #ifdef SWIG
        %template(SingletonAudioManager) Singleton<AudioManager>;
        #endif

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the base manager class for the Audio subsystem and it's operations.
        /// @details
        ///////////////////////////////////////////////////////////////////////////////
        class MEZZ_LIB SoundTypeHandler
        {
            public:
                /// @brief Basic container type for Sound storage by this class.
                typedef std::vector<iSound*>            SoundContainer;
                /// @brief Iterator type for Sound instances stored by this class.
                typedef SoundContainer::iterator        SoundIterator;
                /// @brief Const Iterator type for Sound instances stored by this class.
                typedef SoundContainer::const_iterator  ConstSoundIterator;

                ///////////////////////////////////////////////////////////////////////////////
                // Public Data Members

                /// @brief Whether or not this type of sound is muted.
                bool Mute;
                /// @brief The volume currently set for this type of sound.
                Real Volume;
                /// @brief Sound storage.
                SoundContainer TypeSounds;

                ///////////////////////////////////////////////////////////////////////////////
                // Construction and Destruction

                /// @brief Class constructor.
                SoundTypeHandler() : Mute(false), Volume(1.0) {}
                /// @brief Class destructor.
                ~SoundTypeHandler() {}
        };//SoundTypeHandler

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the base manager class for the Audio subsystem and it's operations.
        /// @details This is a place for loading, storing, and running sound files as
        /// necessary in a given application.
        ///////////////////////////////////////////////////////////////////////////////
        class MEZZ_LIB AudioManager : public ManagerBase, public ObjectSettingsHandler, public Singleton<AudioManager>
        {
            public:
                /// @brief Basic container type for Sound storage by this class.
                typedef std::map<String,iSound*>                    SoundContainer;
                /// @brief Iterator type for Sound instances stored by this class.
                typedef SoundContainer::iterator                    SoundIterator;
                /// @brief Const Iterator type for Sound instances stored by this class.
                typedef SoundContainer::const_iterator              ConstSoundIterator;
                /// @brief Basic container type for storing Sound instances by type in this class.
                typedef std::map<UInt16,SoundTypeHandler*>          SoundTypesContainer;
                /// @brief Iterator type for Sound instances stored by type in this class.
                typedef SoundTypesContainer::iterator               SoundTypesIterator;
                /// @brief Const Iterator type for Sound instances stored by type in this class.
                typedef SoundTypesContainer::const_iterator         ConstSoundTypesIterator;
                /// @brief Basic container type for iDecoderFactory storage by this class.
                typedef std::map<AudioFileFormat,iDecoderFactory*>  DecoderFactoryContainer;
                /// @brief Iterator type for iDecoderFactory instances stored by this class.
                typedef DecoderFactoryContainer::iterator           DecoderFactoryIterator;
                /// @brief Const Iterator type for iDecoderFactory instances stored by this class.
                typedef DecoderFactoryContainer::const_iterator     ConstDecoderFactoryIterator;
            protected:
                /// @internal
                /// @brief Internal data for this manager.
                AudioManagerInternalData* AMID;
                /// @internal
                /// @brief Mute variable for all Sound instances.
                bool MasterMute;
                /// @internal
                /// @brief Volume modifier for all Sound instances.
                Real MasterVolume;
                /// @internal
                /// @brief Container storing all Sound instances by name.
                SoundContainer Sounds;
                /// @internal
                /// @brief Container storing all Sound instances by their type.
                SoundTypesContainer SoundsByType;
                /// @internal
                /// @brief Container storing all registered iDecoderFactory instances.
                DecoderFactoryContainer DecoderFactories;

                /// @internal
                /// @brief Gets a SoundTypeHandler by type ID if it exists, or creates a new one if it does not.
                /// @note Unlike GetSoundTypeHandler(const UInt16 Type) const, this method will always return a valid pointer.
                /// @param Type A UInt16 that is the ID for the type of Sound instances to get the handler for.
                /// @return Returns a pointer to the requested SoundTypeHandler.
                virtual SoundTypeHandler* GetOrCreateSoundTypeHandler(const UInt16 Type);
                /// @internal
                /// @brief Gets the handler storing information on a specific type of Sound instance.
                /// @param Type A UInt16 that is the ID for the type of Sound instances to get the handler for.
                /// @return Returns a pointer to the SoundTypeHandler requested, or NULL if it does not exist.
                virtual SoundTypeHandler* GetSoundTypeHandler(const UInt16 Type) const;
                /// @internal
                /// @brief Gets the handler storing information on a specific type of Sound instance.
                /// @exception If this method fails to find the requested handler, it will throw a "II_IDENTITY_NOT_FOUND_EXCEPTION" exception.
                /// @param Type A UInt16 that is the ID for the type of Sound instances to get the handler for.
                /// @return Returns a pointer to the SoundTypeHandler requested.
                virtual SoundTypeHandler* GetSoundTypeHandlerExcept(const UInt16 Type) const;
            public:
                /// @brief Class Constructor.
                AudioManager();
                /// @brief XML constructor.
                /// @param XMLNode The node of the xml document to construct from.
                AudioManager(XML::Node& XMLNode);
                /// @brief Class Destructor.
                /// @details The class destructor.
                virtual ~AudioManager();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @brief Gets the name of the device currently in use.
                /// @return Returns a String containing the name of the device this manager was initiailzed with, or empty if one hasn't been set.
                virtual String GetCurrentDeviceName() const = 0;
                /// @brief Retrieve's the listener for this sound manager.
                /// @return Returns a pointer to the system @ref iListener.
                virtual iListener* GetListener() const = 0;

                ///////////////////////////////////////////////////////////////////////////////
                // Sound Management

                /// @brief Creates a new sound from a file.
                /// @param Type A UInt16 that is the ID for the type of sound this Sound is to be categorized as.
                /// @param SoundName The name of the Sound instance.
                /// @param FileName The name of the file to load the sound from.
                /// @param Group The resource group in which the file resides.
                /// @return Returns a pointer to the Sound instance that was created.
                virtual iSound* CreateSound(const UInt16 Type, const String& SoundName, const String& FileName, const String& Group) = 0;
                /// @brief Gets a sound by it's name.
                /// @return Returns a pointer to the specified sound.
                virtual iSound* GetSound(const String& SoundName) const = 0;
                /// @brief Deletes a Sound.
                /// @param SoundName A pointer to the Sound you want deleted.
                virtual void DestroySound(iSound* ToBeDestroyed) = 0;
                /// @brief Deletes all stored sounds.
                virtual void DestroyAllSounds() = 0;

                ///////////////////////////////////////////////////////////////////////////////
                // Convenience Creation Methods

                /// @brief Creates a new Ambient sound from a file.
                /// @param SoundName The name of the Sound instance.
                /// @param FileName The name of the file to load the sound from.
                /// @param Group The resource group in which the file resides.
                /// @return Returns a pointer to the Sound instance that was created.
                virtual iSound* CreateAmbientSound(const String& SoundName, const String& FileName, const String& Group);
                /// @brief Creates a new Dialog sound from a file.
                /// @param SoundName The name of the Sound instance.
                /// @param FileName The name of the file to load the sound from.
                /// @param Group The resource group in which the file resides.
                /// @return Returns a pointer to the Sound instance that was created.
                virtual iSound* CreateDialogSound(const String& SoundName, const String& FileName, const String& Group);
                /// @brief Creates a new Effect sound from a file.
                /// @param SoundName The name of the Sound instance.
                /// @param FileName The name of the file to load the sound from.
                /// @param Group The resource group in which the file resides.
                /// @return Returns a pointer to the Sound instance that was created.
                virtual iSound* CreateEffectSound(const String& SoundName, const String& FileName, const String& Group);
                /// @brief Creates a new Music sound from a file.
                /// @param SoundName The name of the Sound instance.
                /// @param FileName The name of the file to load the sound from.
                /// @param Group The resource group in which the file resides.
                /// @return Returns a pointer to the Sound instance that was created.
                virtual iSound* CreateMusicSound(const String& SoundName, const String& FileName, const String& Group);

                ///////////////////////////////////////////////////////////////////////////////
                // Volume Management

                /// @brief Sets the volume for all stored Sound instances of the specified type.
                /// @note Although this can accept any UInt16 value, be mindful of the SoundType enum to avoid type collisions.
                /// @exception This method can throw a "II_IDENTITY_NOT_FOUND_EXCEPTION" if the a handler of the specified type does not exist.
                /// @param Type A UInt16 that is the ID for the type of Sound instances to set the volume for.
                /// @param Vol The new volume to apply to all sounds of the specified type.
                virtual void SetTypeVolume(const UInt16 Type, const Real Vol);
                /// @brief Gets the volume for all stored Sound instances of the specified type.
                /// @note Although this can accept any UInt16 value, be mindful of the SoundType enum to avoid type collisions.
                /// @exception This method can throw a "II_IDENTITY_NOT_FOUND_EXCEPTION" if the a handler of the specified type does not exist.
                /// @param Type A UInt16 that is the ID for the type of Sound instances to get the volume of.
                /// @return Returns a Real representing the current volume of the specified sound type.
                virtual Real GetTypeVolume(const UInt16 Type) const;
                /// @brief Sets whether or not to mute all Sound instances of a specified type.
                /// @note Although this can accept any UInt16 value, be mindful of the SoundType enum to avoid type collisions.
                /// @exception This method can throw a "II_IDENTITY_NOT_FOUND_EXCEPTION" if the a handler of the specified type does not exist.
                /// @param Type A UInt16 that is the ID for the type of Sound instances to mute or unmute.
                /// @param Enable True to mute sounds of the specified type, false to unmute them.
                virtual void MuteType(const UInt16 Type, bool Enable);
                /// @brief Gets whether or not Sound instances of the specified type are currently muted.
                /// @note Although this can accept any UInt16 value, be mindful of the SoundType enum to avoid type collisions.
                /// @exception This method can throw a "II_IDENTITY_NOT_FOUND_EXCEPTION" if the a handler of the specified type does not exist.
                /// @param Type A UInt16 that is the ID for the type of Sound instances to check.
                /// @return Returns true if sounds of the specifed type are muted, false otherwise.
                virtual bool IsTypeMuted(const UInt16 Type) const;

                /// @brief Sets the volume for all stored Sound instances.
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

                ///////////////////////////////////////////////////////////////////////////////
                // Convenience Type Volume Methods

                /// @brief Sets the volume for all stored Ambient Sound instances.
                /// @param Ambient The volume to apply to all Ambient sounds.
                virtual void SetAmbientVolume(const Real& Ambient);
                /// @brief Gets the currently set Ambient volume.
                /// @return Returns a Real representing the current Ambient volume.
                virtual Real GetAmbientVolume() const;
                /// @brief Sets whether or not to mute all Ambient Sound instances.
                /// @param Enable True to mute Ambient sounds, false to unmute them.
                virtual void MuteAmbient(bool Enable);
                /// @brief Gets whether or not Ambient Sound instances are muted.
                /// @return Returns true if Ambient sounds are muted, false otherwise.
                virtual bool IsAmbientMuted() const;

                /// @brief Sets the volume for all stored Dialog Sound instances.
                /// @param Dialog The volume to apply to all Dialog sounds.
                virtual void SetDialogVolume(const Real& Dialog);
                /// @brief Gets the currently set Dialog volume.
                /// @return Returns a Real representing the current Dialog volume.
                virtual Real GetDialogVolume() const;
                /// @brief Sets whether or not to mute all Dialog Sound instances.
                /// @param Enable True to mute Dialog sounds, false to unmute them.
                virtual void MuteDialog(bool Enable);
                /// @brief Gets whether or not Dialog Sound instances are muted.
                /// @return Returns true if Dialog sounds are muted, false otherwise.
                virtual bool IsDialogMuted() const;

                /// @brief Sets the volume for all stored Effect Sound instances.
                /// @param Effect The volume to apply to all Effect sounds.
                virtual void SetEffectVolume(const Real& Effect);
                /// @brief Gets the currently set Effect volume.
                /// @return Returns a Real representing the current Effect volume.
                virtual Real GetEffectVolume() const;
                /// @brief Sets whether or not to mute all Effect Sound instances.
                /// @param Enable True to mute Effect sounds, false to unmute them.
                virtual void MuteEffect(bool Enable);
                /// @brief Gets whether or not Effect Sound instances are muted.
                /// @return Returns true if Effect sounds are muted, false otherwise.
                virtual bool IsEffectMuted() const;

                /// @brief Sets the volume for all stored Music Sound instances.
                /// @param Music The volume to apply to all Music sounds.
                virtual void SetMusicVolume(const Real& Music);
                /// @brief Gets the currently set Music volume.
                /// @return Returns a Real representing the current Music volume.
                virtual Real GetMusicVolume() const;
                /// @brief Sets whether or not to mute all Music Sound instances.
                /// @param Enable True to mute Music sounds, false to unmute them.
                virtual void MuteMusic(bool Enable);
                /// @brief Gets whether or not Music Sound instances are muted.
                /// @return Returns true if Music sounds are muted, false otherwise.
                virtual bool IsMusicMuted() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Playback Device Management

                /// @brief Gets the name of an available device by index.
                /// @details This function will retrieve the name of a device by it's index on the sound managers device list.
                /// @param Index The position on the device list you wish to access the name of.
                /// @return Returns the name of the device.
                virtual String GetAvailableDeviceName(const Whole& Index) const = 0;
                /// @brief Gets the number of available devices.
                /// @details This function will return the total number of available devices, including the default.
                /// @return Returns the number of available devices.
                virtual Whole GetAvailableDeviceCount() const = 0;
                /// @brief Gets the name of the default device.
                /// @details This function will return the name of the system default device.
                /// @return Returns the name of the default device.
                virtual String GetDefaultDeviceName() const = 0;
                /// @brief Checks to see if the named device is available.
                /// @param DeviceName The name of the device to check for.
                /// @return Returns true if the device is available on this system, false otherwise.
                virtual bool DeviceNameValid(const String& DeviceName) const = 0;

                /// @brief Initializes the Device.
                /// @details This function will initialize the device using the parameters provided.
                /// You need to call this function if you passed false into the sound manager constructor before
                /// you can use the manager.
                /// @param DeviceName The name of the device you wish to have this manager interface with/use.
                /// @param OutputFrequency Frequency of the output audio, -1 for the devices default.
                /// @param EAXEffectSlots The number of effects per sound allowed to be applied.
                /// @return Returns true if the device was initialized successfully.  False on a failure.
                virtual bool InitializeDevice(ConstString& DeviceName, int OutputFrequency = -1, int EAXEffectSlots = 4) = 0;

                ///////////////////////////////////////////////////////////////////////////////
                // Decoder Management

                /// @brief Adds a iDecoderFactory to this manager allowing the playback of new types of sound formats.
                /// @param ToBeAdded A pointer to the iDecoderFactory to be added.
                void AddDecoderFactory(iDecoderFactory* ToBeAdded);
                /// @brief Checks if a iDecoderFactory is already registered with this manager.
                /// @param Format The file format supported by the iDecoderFactory to look for.
                /// @return Returns true if a iDecoderFactory supporting that file format exists, false otherwise.
                bool DecoderFactoryExists(const AudioFileFormat Format);
                /// @brief Gets an iDecoderFactory by it's supported file format.
                /// @param Format The file format supported by the iDecoderFactory to retrieve.
                /// @return Returns a pointer to the specified iDecoderFactory.
                iDecoderFactory* GetDecoderFactory(const AudioFileFormat Format);
                /// @brief Removes (but does not destroy) an iDecoderFactory.
                /// @param ToBeRemoved A pointer to the iDecoderFactory to be removed.
                void RemoveDecoderFactory(iDecoderFactory* ToBeRemoved);
                /// @brief Removes (but does not destroy) an iDecoderFactory.
                /// @param Format The file format supported by the iDecoderFactory to remove.
                void RemoveDecoderFactory(const AudioFileFormat Format);
                /// @brief Removes (but does not destroy) all registered iDecoderFactory instances.
                void RemoveAllDecoderFactories();
                /// @brief Destroy a registered iDecoderFactory.
                /// @param ToBeDestroyed A pointer to the iDecoderFactory to be destroyed.
                void DestroyDecoderFactory(iDecoderFactory* ToBeDestroyed);
                /// @brief Destroy a registered iDecoderFactory.
                /// @param Format The file format supported by the iDecoderFactory to be destroyed.
                void DestroyDecoderFactory(const AudioFileFormat Format);
                /// @brief Destroys all registered iDecoderFactory instances.
                void DestroyAllDecoderFactories();

                ///////////////////////////////////////////////////////////////////////////////
                // Inherited from Managerbase

                /// @copydoc Mezzanine::ManagerBase::Initialize()
                virtual void Initialize() = 0;
                /// @copydoc Mezzanine::ManagerBase::DoMainLoopItems()
                virtual void DoMainLoopItems() = 0;
                /// @copydoc ManagerBase::GetInterfaceType()
                virtual ManagerType GetInterfaceType() const;
                /// @copydoc ManagerBase::GetImplementationTypeName()
                virtual String GetImplementationTypeName() const = 0;
        };//AudioManager
    }//Audio
}//Mezzanine

#endif


namespace cAudio
{
	//! Interface for the playback capabilities of cAudio.
    class IAudioManager
    {
		//! Creates an Audio Source from a memory buffer using a specific audio codec.
		/**
		\param name: Name of the audio source.
		\param data: Pointer to a buffer in memory to load the data from.
		\param length: Length of the data buffer.
		\param extension: Extension for the audio codec of the data in the memory buffer.
		\return A pointer to an Audio Source or NULL if creation failed.
		*/
		virtual IAudioSource* createFromMemory(const char* name, const char* data, size_t length, const char* extension) = 0;

		//! Creates an Audio Source from raw audio data in a memory buffer.
		/**
		\param name: Name of the audio source.
		\param data: Pointer to a buffer in memory to load the data from.
		\param length: Length of the data buffer.
		\param frequency: Frequency (or sample rate) of the audio data.
		\param format: Format of the audio data.
		\return A pointer to an Audio Source or NULL if creation failed.
		*/
		virtual IAudioSource* createFromRaw(const char* name, const char* data, size_t length, unsigned int frequency, AudioFormats format) = 0;

#ifdef CAUDIO_EFX_ENABLED
		//! Returns the interface for audio effects.
		virtual IAudioEffects* getEffects() = 0;
#endif
    };
}
#endif //! IAUDIOMANAGER_H
