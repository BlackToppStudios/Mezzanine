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

#include "Resource/datastream.h"

namespace Mezzanine
{
    namespace Audio
    {
        class iSound;
        class iRecorder;
        class iDecoder;
        class iDecoderFactory;
        class iEffectsHandler;
        class MusicPlayer;
        class SoundScapeManager;

        // Used by the scripting language binder to help create bindgings for this class. SWIG does know to creation template instances
        #ifdef SWIG
        %template(SingletonAudioManager) Singleton<AudioManager>;
        #endif

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the base manager class for the Audio subsystem and it's operations.
        /// @details This is a place for loading, storing, and running sound files as
        /// necessary in a given application.
        ///////////////////////////////////////////////////////////////////////////////
        class MEZZ_LIB AudioManager : public ManagerBase, public ObjectSettingsHandler, public Singleton<AudioManager>
        {
        public:
            /// @brief Basic container type for @ref iDecoderFactory storage by this class.
            typedef std::map<Audio::Encoding,iDecoderFactory*>  DecoderFactoryContainer;
            /// @brief Iterator type for @ref iDecoderFactory instances stored by this class.
            typedef DecoderFactoryContainer::iterator           DecoderFactoryIterator;
            /// @brief Const Iterator type for @ref iDecoderFactory instances stored by this class.
            typedef DecoderFactoryContainer::const_iterator     ConstDecoderFactoryIterator;
        protected:
            /// @internal
            /// @brief Container storing all registered @ref iDecoderFactory instances.
            DecoderFactoryContainer DecoderFactories;
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

            /// @brief Gets the handler responsible for audio effects.
            /// @return Returns a pointer to the @ref iEffectsHandler for this audio subsystem.
            virtual iEffectsHandler* GetEffectsHandler() const = 0;
            /// @brief Gets the music player for this audio subsystem.
            /// @return Returns a pointer to the Music player belonging to this system, or NULL if this manager does not support one.
            virtual MusicPlayer* GetMusicPlayer() const = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Sound Management

            /// @brief Creates a blank @ref iSound without a stream attached to it.
            /// @param Type A UInt16 that is the ID for the type of sound the created @ref iSound is to be categorized as.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateSound(const UInt16 Type) = 0;
            /// @brief Creates a new @ref iSound from a custom stream that is encoded.
            /// @param Type A UInt16 that is the ID for the type of sound the created @ref iSound is to be categorized as.
            /// @param Stream A Datastream containing the data to be streamed for this @ref iSound.
            /// @param Encode The encoding to be expected when decoding audio for this @ref iSound.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateSound(const UInt16 Type, Resource::DataStreamPtr Stream, const Audio::Encoding Encode) = 0;
            /// @brief Creates a new @ref iSound from a custom stream that is unencoded.
            /// @param Type A UInt16 that is the ID for the type of sound the created @ref iSound is to be categorized as.
            /// @param Stream A Datastream containing the data to be streamed for this @ref iSound.
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateSound(const UInt16 Type, Resource::DataStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config) = 0;
            /// @brief Creates a new @ref iSound from a file.
            /// @param Type A UInt16 that is the ID for the type of sound the created @ref iSound is to be categorized as.
            /// @param FileName The name of the file to read audio data from.
            /// @param Group The resource group in which the file resides.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateSound(const UInt16 Type, const String& FileName, const String& Group) = 0;
            /// @brief Creates a new @ref iSound from a memory buffer that is encoded.
            /// @note The buffer is copied and put into a memory stream. Thus safe for disposal afterward.
            /// @param Type A UInt16 that is the ID for the type of sound the created @ref iSound is to be categorized as.
            /// @param StreamName The name to be given to the stream being created.
            /// @param Buffer A pointer to the buffer where the audio data is located.
            /// @param Length The length of the buffer provided to this method(in bytes).
            /// @param Encode The encoding to be expected when decoding audio for this @ref iSound.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateSound(const UInt16 Type, const String& StreamName, const Char8* Buffer, const UInt32 Length, const Audio::Encoding Encode) = 0;
            /// @brief Creates a new @ref iSound from a memory buffer that is unencoded.
            /// @note The buffer is copied and put into a memory stream. Thus safe for disposal afterward.
            /// @param Type A UInt16 that is the ID for the type of sound the created @ref iSound is to be categorized as.
            /// @param StreamName The name to be given to the stream being created.
            /// @param Buffer A pointer to the buffer where the audio data is located.
            /// @param Length The length of the buffer provided to this method(in bytes).
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateSound(const UInt16 Type, const String& StreamName, const Char8* Buffer, const UInt32 Length, const UInt32 Frequency, const Audio::BitConfig Config) = 0;
            /// @brief Gets an @ref iSound instance by index.
            /// @param Index The index of the @ref iSound to be retrieved.
            /// @return Returns a pointer to the @ref iSound at the specified index.
            virtual iSound* GetSound(const UInt32 Index) const = 0;
            /// @brief Gets the number of @ref iSound instances in this manager.
            /// @return Returns a UInt32 representing the number of @ref iSound instances contained in this manager.
            virtual UInt32 GetNumSounds() const = 0;
            /// @brief Deletes a @ref iSound.
            /// @param ToBeDestroyed A pointer to the @ref iSound you want deleted.
            virtual void DestroySound(iSound* ToBeDestroyed) = 0;
            /// @brief Deletes all stored @ref iSound instances.
            virtual void DestroyAllSounds() = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Convenience Creation Methods

            /// @brief Creates a blank Ambient @ref iSound without a stream attached to it.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateAmbientSound();
            /// @brief Creates a blank Dialog @ref iSound without a stream attached to it.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateDialogSound();
            /// @brief Creates a blank Effect @ref iSound without a stream attached to it.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateEffectSound();
            /// @brief Creates a blank Music @ref iSound without a stream attached to it.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateMusicSound();

            /// @brief Creates a new Ambient @ref iSound from a custom stream that is encoded.
            /// @param Stream A Datastream containing the data to be streamed for this @ref iSound.
            /// @param Encode The encoding to be expected when decoding audio for this @ref iSound.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateAmbientSound(Resource::DataStreamPtr Stream, const Audio::Encoding Encode);
            /// @brief Creates a new Dialog @ref iSound from a custom stream that is encoded.
            /// @param Stream A Datastream containing the data to be streamed for this @ref iSound.
            /// @param Encode The encoding to be expected when decoding audio for this @ref iSound.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateDialogSound(Resource::DataStreamPtr Stream, const Audio::Encoding Encode);
            /// @brief Creates a new Effect @ref iSound from a custom stream that is encoded.
            /// @param Stream A Datastream containing the data to be streamed for this @ref iSound.
            /// @param Encode The encoding to be expected when decoding audio for this @ref iSound.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateEffectSound(Resource::DataStreamPtr Stream, const Audio::Encoding Encode);
            /// @brief Creates a new Music @ref iSound from a custom stream that is encoded.
            /// @param Stream A Datastream containing the data to be streamed for this @ref iSound.
            /// @param Encode The encoding to be expected when decoding audio for this @ref iSound.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateMusicSound(Resource::DataStreamPtr Stream, const Audio::Encoding Encode);

            /// @brief Creates a new Ambient @ref iSound from a custom stream that is unencoded.
            /// @param Stream A Datastream containing the data to be streamed for this @ref iSound.
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateAmbientSound(Resource::DataStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config);
            /// @brief Creates a new Dialog @ref iSound from a custom stream that is unencoded.
            /// @param Stream A Datastream containing the data to be streamed for this @ref iSound.
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateDialogSound(Resource::DataStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config);
            /// @brief Creates a new Effect @ref iSound from a custom stream that is unencoded.
            /// @param Stream A Datastream containing the data to be streamed for this @ref iSound.
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateEffectSound(Resource::DataStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config);
            /// @brief Creates a new Music @ref iSound from a custom stream that is unencoded.
            /// @param Stream A Datastream containing the data to be streamed for this @ref iSound.
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateMusicSound(Resource::DataStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config);

            /// @brief Creates a new Ambient @ref iSound from a file.
            /// @param FileName The name of the file to read audio data from.
            /// @param Group The resource group in which the file resides.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateAmbientSound(const String& FileName, const String& Group);
            /// @brief Creates a new Dialog @ref iSound from a file.
            /// @param FileName The name of the file to read audio data from.
            /// @param Group The resource group in which the file resides.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateDialogSound(const String& FileName, const String& Group);
            /// @brief Creates a new Effect @ref iSound from a file.
            /// @param FileName The name of the file to read audio data from.
            /// @param Group The resource group in which the file resides.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateEffectSound(const String& FileName, const String& Group);
            /// @brief Creates a new Music @ref iSound from a file.
            /// @param FileName The name of the file to read audio data from.
            /// @param Group The resource group in which the file resides.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateMusicSound(const String& FileName, const String& Group);

            /// @brief Creates a new Ambient @ref iSound from a memory buffer that is encoded.
            /// @note The buffer is copied and put into a memory stream. Thus safe for disposal afterward.
            /// @param StreamName The name to be given to the stream being created.
            /// @param Buffer A pointer to the buffer where the audio data is located.
            /// @param Length The length of the buffer provided to this method(in bytes).
            /// @param Encode The encoding to be expected when decoding audio for this @ref iSound.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateAmbientSound(const String& StreamName, const Char8* Buffer, const UInt32 Length, const Audio::Encoding Encode);
            /// @brief Creates a new Dialog @ref iSound from a memory buffer that is encoded.
            /// @note The buffer is copied and put into a memory stream. Thus safe for disposal afterward.
            /// @param SoundName The name of the @ref iSound instance.
            /// @param Buffer A pointer to the buffer where the audio data is located.
            /// @param Length The length of the buffer provided to this method(in bytes).
            /// @param Encode The encoding to be expected when decoding audio for this @ref iSound.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateDialogSound(const String& StreamName, const Char8* Buffer, const UInt32 Length, const Audio::Encoding Encode);
            /// @brief Creates a new Effect @ref iSound from a memory buffer that is encoded.
            /// @note The buffer is copied and put into a memory stream. Thus safe for disposal afterward.
            /// @param StreamName The name to be given to the stream being created.
            /// @param Buffer A pointer to the buffer where the audio data is located.
            /// @param Length The length of the buffer provided to this method(in bytes).
            /// @param Encode The encoding to be expected when decoding audio for this @ref iSound.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateEffectSound(const String& StreamName, const Char8* Buffer, const UInt32 Length, const Audio::Encoding Encode);
            /// @brief Creates a new Music @ref iSound from a memory buffer that is encoded.
            /// @note The buffer is copied and put into a memory stream. Thus safe for disposal afterward.
            /// @param StreamName The name to be given to the stream being created.
            /// @param Buffer A pointer to the buffer where the audio data is located.
            /// @param Length The length of the buffer provided to this method(in bytes).
            /// @param Encode The encoding to be expected when decoding audio for this @ref iSound.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateMusicSound(const String& StreamName, const Char8* Buffer, const UInt32 Length, const Audio::Encoding Encode);

            /// @brief Creates a new Ambient @ref iSound from a memory buffer that is unencoded.
            /// @note The buffer is copied and put into a memory stream. Thus safe for disposal afterward.
            /// @param StreamName The name to be given to the stream being created.
            /// @param Buffer A pointer to the buffer where the audio data is located.
            /// @param Length The length of the buffer provided to this method(in bytes).
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateAmbientSound(const String& StreamName, const Char8* Buffer, const UInt32 Length, const UInt32 Frequency, const Audio::BitConfig Config);
            /// @brief Creates a new Dialog @ref iSound from a memory buffer that is unencoded.
            /// @note The buffer is copied and put into a memory stream. Thus safe for disposal afterward.
            /// @param StreamName The name to be given to the stream being created.
            /// @param Buffer A pointer to the buffer where the audio data is located.
            /// @param Length The length of the buffer provided to this method(in bytes).
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateDialogSound(const String& StreamName, const Char8* Buffer, const UInt32 Length, const UInt32 Frequency, const Audio::BitConfig Config);
            /// @brief Creates a new Effect @ref iSound from a memory buffer that is unencoded.
            /// @note The buffer is copied and put into a memory stream. Thus safe for disposal afterward.
            /// @param StreamName The name to be given to the stream being created.
            /// @param Buffer A pointer to the buffer where the audio data is located.
            /// @param Length The length of the buffer provided to this method(in bytes).
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateEffectSound(const String& StreamName, const Char8* Buffer, const UInt32 Length, const UInt32 Frequency, const Audio::BitConfig Config);
            /// @brief Creates a new Music @ref iSound from a memory buffer that is unencoded.
            /// @note The buffer is copied and put into a memory stream. Thus safe for disposal afterward.
            /// @param StreamName The name to be given to the stream being created.
            /// @param Buffer A pointer to the buffer where the audio data is located.
            /// @param Length The length of the buffer provided to this method(in bytes).
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the @ref iSound instance that was created.
            virtual iSound* CreateMusicSound(const String& StreamName, const Char8* Buffer, const UInt32 Length, const UInt32 Frequency, const Audio::BitConfig Config);

            ///////////////////////////////////////////////////////////////////////////////
            // Volume Management

            /// @brief Sets the volume for all stored Sound instances of the specified type.
            /// @note Although this can accept any UInt16 value, be mindful of the SoundType enum to avoid type collisions.
            /// @exception This method can throw a "II_IDENTITY_NOT_FOUND_EXCEPTION" if the a handler of the specified type does not exist.
            /// @param Type A UInt16 that is the ID for the type of Sound instances to set the volume for.
            /// @param Vol The new volume to apply to all sounds of the specified type.
            virtual void SetTypeVolume(const UInt16 Type, const Real Vol) = 0;
            /// @brief Gets the volume for all stored Sound instances of the specified type.
            /// @note Although this can accept any UInt16 value, be mindful of the SoundType enum to avoid type collisions.
            /// @exception This method can throw a "II_IDENTITY_NOT_FOUND_EXCEPTION" if the a handler of the specified type does not exist.
            /// @param Type A UInt16 that is the ID for the type of Sound instances to get the volume of.
            /// @return Returns a Real representing the current volume of the specified sound type.
            virtual Real GetTypeVolume(const UInt16 Type) const = 0;
            /// @brief Sets whether or not to mute all Sound instances of a specified type.
            /// @note Although this can accept any UInt16 value, be mindful of the SoundType enum to avoid type collisions.
            /// @exception This method can throw a "II_IDENTITY_NOT_FOUND_EXCEPTION" if the a handler of the specified type does not exist.
            /// @param Type A UInt16 that is the ID for the type of Sound instances to mute or unmute.
            /// @param Enable True to mute sounds of the specified type, false to unmute them.
            virtual void MuteType(const UInt16 Type, bool Enable) = 0;
            /// @brief Gets whether or not Sound instances of the specified type are currently muted.
            /// @note Although this can accept any UInt16 value, be mindful of the SoundType enum to avoid type collisions.
            /// @exception This method can throw a "II_IDENTITY_NOT_FOUND_EXCEPTION" if the a handler of the specified type does not exist.
            /// @param Type A UInt16 that is the ID for the type of Sound instances to check.
            /// @return Returns true if sounds of the specifed type are muted, false otherwise.
            virtual bool IsTypeMuted(const UInt16 Type) const = 0;

            /// @brief Sets the volume for all stored Sound instances.
            /// @param Music The volume to apply to all sounds.
            virtual void SetMasterVolume(const Real& Master) = 0;
            /// @brief Gets the currently set Master volume.
            /// @return Returns a Real representing the current Master volume.
            virtual Real GetMasterVolume() const = 0;
            /// @brief Sets whether or not to mute all Audio.
            /// @param Enable Whether or not all sounds should be muted.
            virtual void Mute(bool Enable) = 0;
            /// @brief Gets whether or not the Audio subsystem is muted.
            /// @return Returns a bool indicating whether or not the Audio subsystem is currently muted.
            virtual bool IsMuted() const = 0;

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
            // Recorder Management

            /// @brief Creates a new @ref iRecorder.
            /// @return Returns a pointer to the created @ref iRecorder.
            virtual iRecorder* CreateRecorder() = 0;
            /// @brief Gets a @ref iRecorder by index.
            /// @param Index The index of the @ref iRecorder to retrieve.
            /// @return Returns a pointer to the @ref iRecorder at the specified index.
            virtual iRecorder* GetRecorder(const UInt32 Index) const = 0;
            /// @brief Gets the number of @ref iRecorder instances being stored by this manager.
            /// @return Returns a UInt32 representing the number of @ref iRecorder instances within this manager.
            virtual UInt32 GetNumRecorders() const = 0;
            /// @brief Destroys a @ref iRecorder instance by pointer.
            /// @param ToBeDestroyed A pointer to the @ref iRecorder instance to be destroyed.
            virtual void DestroyRecorder(iRecorder* ToBeDestroyed) = 0;
            /// @brief Destroys all @ref iRecorder instances being stored by this manager.
            virtual void DestroyAllRecorders() = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Playback Device Management

            /// @brief Gets the name of the playback device currently in use.
            /// @return Returns a String containing the name of the device this manager was initiailzed with, or empty if one hasn't been set.
            virtual String GetCurrentPlaybackDeviceName() const = 0;

            /// @brief Gets the name of an available playback device by index.
            /// @details This function will retrieve the name of a device by it's index on the sound managers device list.
            /// @param Index The position on the device list you wish to access the name of.
            /// @return Returns the name of the device.
            virtual String GetAvailablePlaybackDeviceName(const Whole& Index) const = 0;
            /// @brief Gets the number of available playback devices.
            /// @details This function will return the total number of available devices, including the default.
            /// @return Returns the number of available devices.
            virtual Whole GetAvailablePlaybackDeviceCount() const = 0;
            /// @brief Gets the name of the default playback device.
            /// @details This function will return the name of the system default device.
            /// @return Returns the name of the default device.
            virtual String GetDefaultPlaybackDeviceName() const = 0;
            /// @brief Checks to see if the named playback device is available.
            /// @param DeviceName The name of the device to check for.
            /// @return Returns true if the device is available on this system, false otherwise.
            virtual bool PlaybackDeviceNameValid(const String& DeviceName) const = 0;

            /// @brief Initializes the playback device.
            /// @details This function will initialize the device using the parameters provided.
            /// You need to call this function if you passed false into the sound manager constructor before
            /// you can use the manager.
            /// @param DeviceName The name of the device you wish to have this manager interface with/use.
            /// @param OutputFrequency Frequency of the output audio, -1 for the devices default.
            /// @param EAXEffectSlots The number of effects per sound allowed to be applied.
            /// @return Returns true if the device was initialized successfully.  False on a failure.
            virtual bool InitializePlaybackDevice(const String& DeviceName, const Integer OutputFrequency = -1, const Integer EAXEffectSlots = 4) = 0;
            /// @brief Shuts down the current playback device and unloads all sounds.
            /// @warning This will destroy all sounds and sound proxies in the system.
            virtual void ShutdownPlaybackDevice() = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Recording Device Management

            /// @brief Gets the name of an available recording device by index.
            /// @param Index The index of the device to retrieve the name for.
            /// @return Returns a String containing the available device name at the specified index.
            virtual String GetAvailableRecordingDeviceName(const UInt32 Index) = 0;
            /// @brief Gets the number of currently available recording devices.
            /// @return Returns a UInt32 containing the number of recording devices on this system.
            virtual UInt32 GetAvailableRecordingDeviceCount() = 0;
            /// @brief Gets the name of the default recording device.
            /// @return Returns a String containing the name of the default recording device on this system.
            virtual String GetDefaultRecordingDeviceName() = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Decoder Management

            /// @brief Adds a @ref iDecoderFactory to this manager allowing the playback of new types of sound formats.
            /// @note If an @ref iDecoderFactory is already registered to the same @ref Audio::Encoding, it will be replaced.  It will not be deleted.  Use with care.
            /// @param ToBeAdded A pointer to the @ref iDecoderFactory to be added.
            void AddDecoderFactory(iDecoderFactory* ToBeAdded);
            /// @brief Checks if a @ref iDecoderFactory is already registered with this manager.
            /// @param Format The @ref Audio::Encoding supported by the @ref iDecoderFactory to look for.
            /// @return Returns true if a @ref iDecoderFactory supporting that @ref Audio::Encoding exists, false otherwise.
            bool DecoderFactoryExists(const Audio::Encoding Format);
            /// @brief Gets an @ref iDecoderFactory by it's supported @ref Audio::Encoding.
            /// @param Format The @ref Audio::Encoding supported by the @ref iDecoderFactory to retrieve.
            /// @return Returns a pointer to the @ref iDecoderFactory registered to the provided @ref Audio::Encoding, or NULL if none are registered.
            iDecoderFactory* GetDecoderFactory(const Audio::Encoding Format);
            /// @brief Removes (but does not destroy) an @ref iDecoderFactory.
            /// @param ToBeRemoved A pointer to the @ref iDecoderFactory to be removed.
            void RemoveDecoderFactory(iDecoderFactory* ToBeRemoved);
            /// @brief Removes (but does not destroy) an @ref iDecoderFactory.
            /// @param Format The @ref Audio::Encoding supported by the @ref iDecoderFactory to remove.
            void RemoveDecoderFactory(const Audio::Encoding Format);
            /// @brief Removes (but does not destroy) all registered @ref iDecoderFactory instances.
            void RemoveAllDecoderFactories();
            /// @brief Destroy a registered @ref iDecoderFactory.
            /// @param ToBeDestroyed A pointer to the @ref iDecoderFactory to be destroyed.
            void DestroyDecoderFactory(iDecoderFactory* ToBeDestroyed);
            /// @brief Destroy a registered @ref iDecoderFactory.
            /// @param Format The @ref Audio::Encoding supported by the @ref iDecoderFactory to be destroyed.
            void DestroyDecoderFactory(const Encoding Format);
            /// @brief Destroys all registered @ref iDecoderFactory instances.
            void DestroyAllDecoderFactories();

            ///////////////////////////////////////////////////////////////////////////////
            // Inherited from Managerbase

            /// @copydoc ManagerBase::GetInterfaceType()
            virtual ManagerType GetInterfaceType() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Notifies this AudioManager of the creation of a new SoundScapeManager.
            /// @param Manager A pointer to the manager being registered.
            virtual void _RegisterSoundScapeManager(Audio::SoundScapeManager* Manager) = 0;
            /// @internal
            /// @brief Notifies this AudioManager of the destruction of a SoundScapeManager.
            /// @param Manager A pointer to the manager being unregistered.
            virtual void _UnregisterSoundScapeManager(Audio::SoundScapeManager* Manager) = 0;
        };//AudioManager
    }//Audio
}//Mezzanine

#endif
