// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _audiooalsaudiomanager_h
#define _audiooalsaudiomanager_h

#include "Audio/audiomanager.h"

// OpenAL forward declares
#ifndef OALS_STRUCTS_DECLARED
struct ALCdevice;
struct ALCcontext;
#endif //OALS_STRUCTS_DECLARED

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            class Sound;
            class Recorder;
            class EffectsHandler;
            class SoundScapeManager;
            class AudioManager;

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief This is the work unit for updating audio buffers as necessary for audio playback.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB BufferUpdate2DWorkUnit : public Audio::iBufferUpdate2DWorkUnit
            {
            protected:
                /// @internal
                /// @brief A pointer to the manager this work unit is processing.
                OALS::AudioManager* TargetManager;
                /// @internal
                /// @brief Protected copy constructor.  THIS IS NOT ALLOWED.
                /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
                BufferUpdate2DWorkUnit(const BufferUpdate2DWorkUnit& Other);
                /// @internal
                /// @brief Protected assignment operator.  THIS IS NOT ALLOWED.
                /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
                BufferUpdate2DWorkUnit& operator=(const BufferUpdate2DWorkUnit& Other);
            public:
                /// @brief Class constructor.
                /// @param Target The InputManager this work unit will process during the frame.
                BufferUpdate2DWorkUnit(OALS::AudioManager* Target);
                /// @brief Class destructor.
                virtual ~BufferUpdate2DWorkUnit();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @brief This does any required updating of audio buffers belonging to sources in this manager.
                /// @param CurrentThreadStorage The storage class for all resources owned by this work unit during it's execution.
                virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
            };//BufferUpdate2DWorkUnit

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief This is the work unit for marking all effects and filters as clean after sounds have been processed.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB EffectFilterCleanWorkUnit : public Audio::iEffectFilterCleanWorkUnit
            {
            protected:
                /// @internal
                /// @brief A pointer to the manager this work unit is processing.
                OALS::AudioManager* TargetManager;
                /// @internal
                /// @brief Protected copy constructor.  THIS IS NOT ALLOWED.
                /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
                EffectFilterCleanWorkUnit(const EffectFilterCleanWorkUnit& Other);
                /// @internal
                /// @brief Protected assignment operator.  THIS IS NOT ALLOWED.
                /// @param Other The other work unit being copied from.  WHICH WILL NEVER HAPPEN.
                EffectFilterCleanWorkUnit& operator=(const EffectFilterCleanWorkUnit& Other);
            public:
                /// @brief Class constructor.
                /// @param Target The InputManager this work unit will process during the frame.
                EffectFilterCleanWorkUnit(OALS::AudioManager* Target);
                /// @brief Class destructor.
                virtual ~EffectFilterCleanWorkUnit();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @brief This does the required cleaning of all effects and filters in use by this manager.
                /// @param CurrentThreadStorage The storage class for all resources owned by this work unit during it's execution.
                virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage);
            };//EffectFilterCleanWorkUnit

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief This is the base manager class for the Audio subsystem and it's operations.
            /// @details
            ///////////////////////////////////////////////////////////////////////////////
            class MEZZ_LIB SoundTypeHandler
            {
            public:
                /// @brief Basic container type for Sound storage by this class.
                typedef std::vector<OALS::Sound*>       SoundContainer;
                /// @brief Iterator type for Sound instances stored by this class.
                typedef SoundContainer::iterator        SoundIterator;
                /// @brief Const Iterator type for Sound instances stored by this class.
                typedef SoundContainer::const_iterator  ConstSoundIterator;

                ///////////////////////////////////////////////////////////////////////////////
                // Public Data Members

                /// @brief Whether or not this type of sound is muted.
                Boole Mute;
                /// @brief The volume currently set for this type of sound.
                Real Volume;
                /// @brief Sound storage.
                //SoundContainer TypeSounds;

                ///////////////////////////////////////////////////////////////////////////////
                // Construction and Destruction

                /// @brief Class constructor.
                SoundTypeHandler() : Mute(false), Volume(1.0) {}
                /// @brief Class destructor.
                ~SoundTypeHandler() {}
            };//SoundTypeHandler

            ///////////////////////////////////////////////////////////////////////////////
            /// @brief This is the OALS AudioManager implementation.
            /// @details
            ///////////////////////////////////////////////////////////////////////////////
            class MEZZ_LIB AudioManager : public Audio::AudioManager
            {
            public:
                /// @brief Basic container type for @ref OALS::Sound storage by this class.
                typedef std::vector<OALS::Sound*>                   SoundContainer;
                /// @brief Iterator type for @ref OALS::Sound instances stored by this class.
                typedef SoundContainer::iterator                    SoundIterator;
                /// @brief Const Iterator type for @ref OALS::Sound instances stored by this class.
                typedef SoundContainer::const_iterator              ConstSoundIterator;
                /// @brief Basic container type for storing @ref OALS::Sound instances by type in this class.
                typedef std::map<UInt16,SoundTypeHandler*>          SoundTypesContainer;
                /// @brief Iterator type for @ref OALS::Sound instances stored by type in this class.
                typedef SoundTypesContainer::iterator               SoundTypesIterator;
                /// @brief Const Iterator type for @ref OALS::Sound instances stored by type in this class.
                typedef SoundTypesContainer::const_iterator         ConstSoundTypesIterator;
                /// @brief Basic container type for @ref OALS::Recorder storage by this class.
                typedef std::vector<OALS::Recorder*>                RecorderContainer;
                /// @brief Iterator type for @ref OALS::Recorder instances stored by this class.
                typedef RecorderContainer::iterator                 RecorderIterator;
                /// @brief Const Iterator type for @ref OALS::Recorder instances stored by this class.
                typedef RecorderContainer::const_iterator           ConstRecorderIterator;
                /// @brief Basic container type for @ref OALS::SoundScapeManager instances registered to this class.
                typedef std::vector<OALS::SoundScapeManager*>       SoundScapeManagerContainer;
                /// @brief Iterator type for @ref OALS::SoundScapeManager instances registered to this class.
                typedef SoundScapeManagerContainer::iterator        SoundScapeManagerIterator;
                /// @brief Const Iterator type for @ref OALS::SoundScapeManager instances registered to this class.
                typedef SoundScapeManagerContainer::const_iterator  ConstSoundScapeManagerIterator;

                /// @brief A String containing the name of this manager implementation.
                static const String ImplementationName;
                /// @brief A ManagerType enum value used to describe the type of interface/functionality this manager provides.
                static const ManagerBase::ManagerType InterfaceType;
            protected:
                friend class BufferUpdate2DWorkUnit;
                friend class EffectFilterCleanWorkUnit;

                /// @brief Container storing all @ref OALS::SoundScapeManager instances registered to this manager.
                SoundScapeManagerContainer SoundScapeManagers;
                /// @brief Container storing all @ref OALS::Recorder instances.
                RecorderContainer Recorders;
                /// @brief Container storing all @ref OALS::Sound instances by their type.
                mutable SoundTypesContainer SoundsByType;
                /// @brief Container storing all @ref OALS::Sound instances.
                SoundContainer Sounds;
                /// @brief A vector containing the names of the available playback devices on the system.
                StringVector AvailablePlaybackDevices;
                /// @brief A vector containing the names of the available recording devices on the system.
                StringVector AvailableRecorderDevices;
                /// @brief The name of the current device being used for audio playback.
                String CurrentDeviceName;
                /// @brief A pointer to the internal device this manager and all it's operations take place on.
                ALCdevice* InternalDevice;
                /// @brief A pointer to the context used for the management of all non-spacialized audio.
                ALCcontext* NonSpacialContext;
                /// @brief A pointer to the handler for all EFX related operations.
                OALS::EffectsHandler* EffHandler;
                /// @brief A pointer to the system Music Player.
                MusicPlayer* MPlayer;
                /// @brief The workunit this will use to complete its buffer updates.
                OALS::BufferUpdate2DWorkUnit* BufferUpdate2DWork;
                /// @brief The workunit this will use to complete its buffer updates.
                OALS::EffectFilterCleanWorkUnit* EffectFilterCleanWork;
                /// @brief Can be used for thread safe logging and other thread specific resources.
                Threading::DefaultThreadSpecificStorage::Type* ThreadResources;
                /// @brief The output frequency of the rendered audio belonging to the non-spacial context.
                Integer ContextOutputFrequency;
                /// @brief Volume modifier for all @ref iSound instances.
                Real MasterVolume;
                /// @brief Mute variable for all @ref iSound instances.
                Boole MasterMute;

                /// @brief Gets a SoundTypeHandler by type ID if it exists, or creates a new one if it does not.
                /// @note Unlike GetSoundTypeHandler(const UInt16 Type) const, this method will always return a valid pointer.
                /// @param Type A UInt16 that is the ID for the type of Sound instances to get the handler for.
                /// @return Returns a pointer to the requested SoundTypeHandler.
                virtual SoundTypeHandler* GetOrCreateSoundTypeHandler(const UInt16 Type) const;
                /// @brief Gets the handler storing information on a specific type of Sound instance.
                /// @param Type A UInt16 that is the ID for the type of Sound instances to get the handler for.
                /// @return Returns a pointer to the SoundTypeHandler requested, or NULL if it does not exist.
                virtual SoundTypeHandler* GetSoundTypeHandler(const UInt16 Type) const;
                /// @brief Gets the handler storing information on a specific type of Sound instance.
                /// @exception If this method fails to find the requested handler, it will throw a "II_IDENTITY_NOT_FOUND_EXCEPTION" exception.
                /// @param Type A UInt16 that is the ID for the type of Sound instances to get the handler for.
                /// @return Returns a pointer to the SoundTypeHandler requested.
                virtual SoundTypeHandler* GetSoundTypeHandlerExcept(const UInt16 Type) const;

                /// @brief Gets the name of the Root XML Node of this object when it is serialized.
                /// @return Returns a String containing the name of the XML Node for this manager.
                virtual String GetObjectRootNodeName() const;
                /// @brief Adds all of this classes current configuration to an XML node.
                /// @param SettingsRootNode The XML Node the current settings will be applied to.
                virtual void AppendCurrentSettings(XML::Node& SettingsRootNode);
                /// @brief Applies a group of configuration settings to this manager.
                /// @param Group The Settings Group to update this managers state with.
                virtual void ApplySettingGroupImpl(ObjectSettingGroup* Group);

                /// @brief Gets all the available devices on this system and puts them into easier to access string vectors.
                virtual void GetAvailableDeviceNames();
            public:
                /// @brief Class Constructor.
                AudioManager();
                /// @brief XML constructor.
                /// @param XMLNode The node of the xml document to construct from.
                AudioManager(const XML::Node& XMLNode);
                /// @brief Class Destructor.
                virtual ~AudioManager();

                ///////////////////////////////////////////////////////////////////////////////
                // Sound Management

                /// @copydoc Audio::AudioManager::CreateSound(const UInt16)
                virtual iSound* CreateSound(const UInt16 Type) override;
                /// @copydoc Audio::AudioManager::CreateSound(const UInt16, IStreamPtr, const Audio::Encoding)
                virtual iSound* CreateSound(const UInt16 Type, IStreamPtr Stream, const Audio::Encoding Encode) override;
                /// @copydoc Audio::AudioManager::CreateSound(const UInt16, IStreamPtr, const UInt32, const Audio::BitConfig)
                virtual iSound* CreateSound(const UInt16 Type, IStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config) override;
                /// @copydoc Audio::AudioManager::CreateSound(const UInt16, const String&, const String&, const String&)
                virtual iSound* CreateSound(const UInt16 Type, const String& FileName, const String& Group) override;
                /// @copydoc Audio::AudioManager::GetSound(const UInt32) const
                virtual iSound* GetSound(const UInt32 Index) const override;
                /// @copydoc Audio::AudioManager::GetNumSounds() const
                virtual UInt32 GetNumSounds() const override;
                /// @copydoc Audio::AudioManager::DestroySound(iSound* ToBeDestroyed)
                virtual void DestroySound(iSound* ToBeDestroyed) override;
                /// @copydoc Audio::AudioManager::DestroyAllSounds()
                virtual void DestroyAllSounds() override;

                ///////////////////////////////////////////////////////////////////////////////
                // Volume Management

                /// @copydoc Audio::AudioManager::SetTypeVolume(const UInt16, const Real)
                virtual void SetTypeVolume(const UInt16 Type, const Real Vol) override;
                /// @copydoc Audio::AudioManager::GetTypeVolume(const UInt16) const
                virtual Real GetTypeVolume(const UInt16 Type) const override;
                /// @copydoc Audio::AudioManager::MuteType(const UInt16, Boole)
                virtual void MuteType(const UInt16 Type, Boole Enable) override;
                /// @copydoc Audio::AudioManager::IsTypeMuted(const UInt16) const
                virtual Boole IsTypeMuted(const UInt16 Type) const override;

                /// @copydoc Audio::AudioManager::SetMasterVolume(const Real&)
                virtual void SetMasterVolume(const Real& Master) override;
                /// @copydoc Audio::AudioManager::GetMasterVolume() const
                virtual Real GetMasterVolume() const override;
                /// @copydoc Audio::AudioManager::Mute(Boole)
                virtual void SetMasterMute(Boole Enable) override;
                /// @copydoc Audio::AudioManager::IsMuted() const
                virtual Boole IsMuted() const override;

                ///////////////////////////////////////////////////////////////////////////////
                // Recorder Management

                /// @copydoc Audio::AudioManager::CreateRecorder()
                virtual iRecorder* CreateRecorder() override;
                /// @copydoc Audio::AudioManager::GetRecorder(const UInt32) const
                virtual iRecorder* GetRecorder(const UInt32 Index) const override;
                /// @copydoc Audio::AudioManager::GetNumRecorders() const
                virtual UInt32 GetNumRecorders() const override;
                /// @copydoc Audio::AudioManager::DestroyRecorder(iRecorder*)
                virtual void DestroyRecorder(iRecorder* ToBeDestroyed) override;
                /// @copydoc Audio::AudioManager::DestroyAllRecorders()
                virtual void DestroyAllRecorders() override;

                ///////////////////////////////////////////////////////////////////////////////
                // Playback Device Management

                /// @copydoc Audio::AudioManager::GetCurrentPlaybackDeviceName() const
                virtual String GetCurrentPlaybackDeviceName() const override;

                /// @copydoc Audio::AudioManager::GetAvailablePlaybackDeviceName(const Whole&) const
                virtual String GetAvailablePlaybackDeviceName(const Whole& Index) const override;
                /// @copydoc Audio::AudioManager::GetAvailablePlaybackDeviceCount() const
                virtual Whole GetAvailablePlaybackDeviceCount() const override;
                /// @copydoc Audio::AudioManager::GetDefaultPlaybackDeviceName() const
                virtual String GetDefaultPlaybackDeviceName() const override;
                /// @copydoc Audio::AudioManager::PlaybackDeviceNameValid(const String&) const
                virtual Boole PlaybackDeviceNameValid(const String& DeviceName) const override;

                /// @copydoc Audio::AudioManager::InitializePlaybackDevice(const String&, const Integer)
                virtual Boole InitializePlaybackDevice(const String& DeviceName, const Integer OutputFrequency = 44100) override;
                /// @copydoc Audio::AudioManager::ShutdownPlaybackDevice()
                virtual void ShutdownPlaybackDevice() override;

                ///////////////////////////////////////////////////////////////////////////////
                // Recording Device Management

                /// @copydoc Audio::AudioManager::GetAvailableRecordingDeviceName(const UInt32)
                virtual String GetAvailableRecordingDeviceName(const UInt32 Index) override;
                /// @copydoc Audio::AudioManager::GetAvailableRecordingDeviceCount()
                virtual UInt32 GetAvailableRecordingDeviceCount() override;
                /// @copydoc Audio::AudioManager::GetDefaultRecordingDeviceName()
                virtual String GetDefaultRecordingDeviceName() override;

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc ManagerBase::Initialize()
                virtual void Initialize() override;
                /// @copydoc ManagerBase::Deinitialize()
                virtual void Deinitialize() override;

                /// @copydoc Audio::AudioManager::GetEffectsHandler() const
                virtual iEffectsHandler* GetEffectsHandler() const override;
                /// @copydoc Audio::AudioManager::GetMusicPlayer() const
                virtual MusicPlayer* GetMusicPlayer() const override;

                /// @copydoc Audio::AudioManager::GetBufferUpdate2DWork()
                virtual iBufferUpdate2DWorkUnit* GetBufferUpdate2DWork() override;
                /// @copydoc Audio::AudioManager::GetEffectFilterCleanWork()
                virtual iEffectFilterCleanWorkUnit* GetEffectFilterCleanWork() override;

                ///////////////////////////////////////////////////////////////////////////////
                // Type Identifier Methods

                /// @copydoc ManagerBase::GetImplementationTypeName()
                virtual String GetImplementationTypeName() const override;

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Methods

                /// @internal
                /// @brief Gets the 2D context owned by this manager.
                ALCcontext* _GetNonSpacialContext() const;
                /// @internal
                /// @brief Gets the open audio device owned by this manager.
                ALCdevice* _GetAudioDevice() const;
                /// @copydoc Audio::AudioManager::_RegisterSoundScapeManager(Audio::SoundScapeManager*)
                void _RegisterSoundScapeManager(Audio::SoundScapeManager* Manager) override;
                /// @copydoc Audio::AudioManager::_UnregisterSoundScapeManager(Audio::SoundScapeManager*)
                void _UnregisterSoundScapeManager(Audio::SoundScapeManager* Manager) override;
            };//AudioManager
        }//OALS
    }//Audio
}//Mezzanine

#endif
