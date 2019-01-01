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
#ifndef _audiosoundscapemanager_h
#define _audiosoundscapemanager_h

#include "entitycomponentmanager.h"
#include "managerfactory.h"
#include "Audio/audioenumerations.h"
#include "Threading/workunit.h"

namespace Mezzanine
{
    namespace Audio
    {
        class iListener;
        class SoundProxy;

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the work unit for updating audio buffers as necessary for audio playback.
        ///////////////////////////////////////
        class MEZZ_LIB iBufferUpdate3DWorkUnit : public Threading::DefaultWorkUnit
        {
        public:
            /// @brief Class destructor.
            virtual ~iBufferUpdate3DWorkUnit() {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief This does any required updating of audio buffers belonging to sound proxies in this manager.
            /// @param CurrentThreadStorage The storage class for all resources owned by this work unit during it's execution.
            virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage) = 0;
        };//iBufferUpdate3DWorkUnit

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the base manager class for audio being played in a 3D environment.
        ///////////////////////////////////////
        class MEZZ_LIB SoundScapeManager : public EntityComponentManager
        {
        public:
        protected:
        public:
            /// @brief Class constructor.
            SoundScapeManager(World* Creator);
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            SoundScapeManager(World* Creator, XML::Node& XMLNode);
            /// @brief Class destructor.
            virtual ~SoundScapeManager();

            ///////////////////////////////////////////////////////////////////////////////
            // Proxy Creation

            /// @brief Creates a new iListener.
            /// @return Returns a pointer to the iListener instance that was created.
            virtual iListener* CreateListener() = 0;
            /// @brief Creates a new iListener.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            /// @return Returns a pointer to the iListener instance that was created.
            virtual iListener* CreateListener(const XML::Node& SelfRoot) = 0;

            /// @brief Creates a blank SoundProxy without a stream attached to it.
            /// @param Type A UInt16 that is the ID for the type of sound the created SoundProxy is to be categorized as.
            /// @return Returns a pointer to the SoundProxy instance that was created.
            virtual SoundProxy* CreateSoundProxy(const UInt16 Type) = 0;
            /// @brief Creates a new SoundProxy from a custom stream that is encoded.
            /// @param Type A UInt16 that is the ID for the type of sound the created SoundProxy is to be categorized as.
            /// @param Stream A Datastream containing the data to be streamed for this SoundProxy.
            /// @param Encode The encoding to be expected when decoding audio for this SoundProxy.
            /// @return Returns a pointer to the SoundProxy instance that was created.
            virtual SoundProxy* CreateSoundProxy(const UInt16 Type, IStreamPtr Stream, const Audio::Encoding Encode) = 0;
            /// @brief Creates a new SoundProxy from a custom stream that is unencoded.
            /// @param Type A UInt16 that is the ID for the type of sound the created SoundProxy is to be categorized as.
            /// @param Stream A Datastream containing the data to be streamed for this SoundProxy.
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the SoundProxy instance that was created.
            virtual SoundProxy* CreateSoundProxy(const UInt16 Type, IStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config) = 0;
            /// @brief Creates a new SoundProxy from a file.
            /// @param Type A UInt16 that is the ID for the type of sound the created SoundProxy is to be categorized as.
            /// @param FileName The name of the file to read audio data from.
            /// @param Group The resource group in which the file resides.
            /// @return Returns a pointer to the SoundProxy instance that was created.
            virtual SoundProxy* CreateSoundProxy(const UInt16 Type, const String& FileName, const String& Group) = 0;
            /// @brief Creates a new SoundProxy.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            /// @return Returns a pointer to the SoundProxy instance that was created.
            virtual SoundProxy* CreateSoundProxy(const XML::Node& SelfRoot) = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Convenience Creation Methods

            /// @brief Creates a blank Ambient SoundProxy without a stream attached to it.
            /// @return Returns a pointer to the SoundProxy instance that was created.
            virtual SoundProxy* CreateAmbientSoundProxy();
            /// @brief Creates a blank Dialog SoundProxy without a stream attached to it.
            /// @return Returns a pointer to the SoundProxy instance that was created.
            virtual SoundProxy* CreateDialogSoundProxy();
            /// @brief Creates a blank Effect SoundProxy without a stream attached to it.
            /// @return Returns a pointer to the SoundProxy instance that was created.
            virtual SoundProxy* CreateEffectSoundProxy();
            /// @brief Creates a blank Music SoundProxy without a stream attached to it.
            /// @return Returns a pointer to the SoundProxy instance that was created.
            virtual SoundProxy* CreateMusicSoundProxy();

            /// @brief Creates a new Ambient SoundProxy from a custom stream that is encoded.
            /// @param Stream A Datastream containing the data to be streamed for this SoundProxy.
            /// @param Encode The encoding to be expected when decoding audio for this SoundProxy.
            /// @return Returns a pointer to the SoundProxy instance that was created.
            virtual SoundProxy* CreateAmbientSoundProxy(IStreamPtr Stream, const Audio::Encoding Encode);
            /// @brief Creates a new Dialog SoundProxy from a custom stream that is encoded.
            /// @param Stream A Datastream containing the data to be streamed for this SoundProxy.
            /// @param Encode The encoding to be expected when decoding audio for this SoundProxy.
            /// @return Returns a pointer to the SoundProxy instance that was created.
            virtual SoundProxy* CreateDialogSoundProxy(IStreamPtr Stream, const Audio::Encoding Encode);
            /// @brief Creates a new Effect SoundProxy from a custom stream that is encoded.
            /// @param Stream A Datastream containing the data to be streamed for this SoundProxy.
            /// @param Encode The encoding to be expected when decoding audio for this SoundProxy.
            /// @return Returns a pointer to the SoundProxy instance that was created.
            virtual SoundProxy* CreateEffectSoundProxy(IStreamPtr Stream, const Audio::Encoding Encode);
            /// @brief Creates a new Music SoundProxy from a custom stream that is encoded.
            /// @param Stream A Datastream containing the data to be streamed for this SoundProxy.
            /// @param Encode The encoding to be expected when decoding audio for this SoundProxy.
            /// @return Returns a pointer to the SoundProxy instance that was created.
            virtual SoundProxy* CreateMusicSoundProxy(IStreamPtr Stream, const Audio::Encoding Encode);

            /// @brief Creates a new Ambient SoundProxy from a custom stream that is unencoded.
            /// @param Stream A Datastream containing the data to be streamed for this SoundProxy.
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the SoundProxy instance that was created.
            virtual SoundProxy* CreateAmbientSoundProxy(IStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config);
            /// @brief Creates a new Dialog SoundProxy from a custom stream that is unencoded.
            /// @param Stream A Datastream containing the data to be streamed for this SoundProxy.
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the SoundProxy instance that was created.
            virtual SoundProxy* CreateDialogSoundProxy(IStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config);
            /// @brief Creates a new Effect SoundProxy from a custom stream that is unencoded.
            /// @param Stream A Datastream containing the data to be streamed for this SoundProxy.
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the SoundProxy instance that was created.
            virtual SoundProxy* CreateEffectSoundProxy(IStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config);
            /// @brief Creates a new Music SoundProxy from a custom stream that is unencoded.
            /// @param Stream A Datastream containing the data to be streamed for this SoundProxy.
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the SoundProxy instance that was created.
            virtual SoundProxy* CreateMusicSoundProxy(IStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config);

            /// @brief Creates a new Ambient SoundProxy from a file.
            /// @param FileName The name of the file to read audio data from.
            /// @param Group The resource group in which the file resides.
            /// @return Returns a pointer to the SoundProxy instance that was created.
            virtual SoundProxy* CreateAmbientSoundProxy(const String& FileName, const String& Group);
            /// @brief Creates a new Dialog SoundProxy from a file.
            /// @param FileName The name of the file to read audio data from.
            /// @param Group The resource group in which the file resides.
            /// @return Returns a pointer to the SoundProxy instance that was created.
            virtual SoundProxy* CreateDialogSoundProxy(const String& FileName, const String& Group);
            /// @brief Creates a new Effect SoundProxy from a file.
            /// @param FileName The name of the file to read audio data from.
            /// @param Group The resource group in which the file resides.
            /// @return Returns a pointer to the SoundProxy instance that was created.
            virtual SoundProxy* CreateEffectSoundProxy(const String& FileName, const String& Group);
            /// @brief Creates a new Music SoundProxy from a file.
            /// @param FileName The name of the file to read audio data from.
            /// @param Group The resource group in which the file resides.
            /// @return Returns a pointer to the SoundProxy instance that was created.
            virtual SoundProxy* CreateMusicSoundProxy(const String& FileName, const String& Group);

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc WorldManager::Pause(const UInt32)
            virtual void Pause(const UInt32 PL) = 0;

            /// @copydoc WorldManager::Initialize()
            virtual void Initialize() = 0;
            /// @copydoc ManagerBase::Deinitialize()
            virtual void Deinitialize() = 0;

            /// @brief Gets the work unit responsible for updating the buffers of soundproxies.
            /// @return Returns a pointer to the iBufferUpdate3DWorkUnit used by this manager.
            virtual iBufferUpdate3DWorkUnit* GetBufferUpdate3DWork() = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Type Identifier Methods

            /// @copydoc ManagerBase::GetInterfaceType()
            virtual ManagerType GetInterfaceType() const;
        };//SoundScapeManager
    }//Audio
}//Mezzanine

#endif
