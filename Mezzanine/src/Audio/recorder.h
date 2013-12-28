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
// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio-ZLIBLicense.txt
#ifndef _audiorecorder_h
#define _audiorecorder_h

#include "datatypes.h"
#include "Audio/audioenumerations.h"

namespace Mezzanine
{
    namespace Audio
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is an interface class for the recording of audio.
        /// @details
        ///////////////////////////////////////
        class iRecorder
        {
        public:
            /// @brief Class constructor.
            iRecorder() { }
            /// @brief Class destructor.
            virtual ~iRecorder() { }

            ///////////////////////////////////////////////////////////////////////////////
            // Initialization and Shutdown

            /// @brief Initializes the recording device with the current configuration.
            /// @param DeviceName The name of the recording device to use when recording audio.  If left empty this will use the default device.
            /// @param Freq The frequency at which to record and store the audio data in Hz.
            /// @param Config The bit configuration to record and store the audio data.  See enum @ref BitConfig for more information.
            /// @param IntBufSize The size of the internal buffer to allocate for the recorded audio.
            /// @return Returns true if the device was successfully initialized, false if there was a problem.
            virtual bool Initialize(const String& DeviceName = "",  const UInt32 Freq = 22050, const BitConfig Config = BC_16Bit_Mono, const UInt32 IntBufSize = 8192) = 0;
            /// @brief Shuts down and releases the recording device.
            /// @note If you wish to reuse this recorder after it has been shut down, you will need to reinitialize it.
            virtual void Shutdown() = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets whether or not the current Recorder configuration is ready to start recording audio.
            /// @return Returns true if this Recorder is ready to start recording, false otherise.
            virtual bool IsReady() const = 0;

            /// @brief Starts recording audio from the initialized device.
            /// @note If there is any old data from previous recordings in the buffer, they will be wiped.  You should call @ref iRecorder::GetRecordedAudio before calling this.
            /// @return Returns true if recording was successfully started.
            virtual bool BeginRecording() = 0;
            /// @brief Stops the recording of audio if this is currently recording.
            virtual void StopRecording() = 0;
            /// @brief Writes the current contents of the recorder buffer to another buffer.
            /// @note This will clear the recorder buffer after being called.
            /// @param OutputBuffer A pointer to the buffer to write to.
            /// @param OutputBufferSize The available size of the OutputBuffer.
            /// @return Returns a UInt32 representing the number of bytes written to the OutputBuffer.
            virtual UInt32 GetRecordedAudio(void* OutputBuffer, UInt32 OutputBufferSize) = 0;
            /// @brief Gets the size of the buffer storing the recorded audio.
            /// @return Returns a UInt32 representing the current size of the recorder buffer.
            virtual UInt32 GetBufferSize() const = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Recording Configuration

            /// @brief Sets the device to be used for recording by name.
            /// @note If this recorder is already initialized, calling this method may re-initialize it and clear the recorded audio buffer in the process.
            /// @param DeviceName The name of the device to use to record audio.
            /// @return Returns true if there were no errors in setting the device name, false if there was a problem.
            virtual bool SetDeviceName(const String& DeviceName) = 0;
            /// @brief Gets the name of the currently set recording device.
            /// @return Returns a String containing the name of the device this recorder was initialized with, or an empty string if one hasn't been initialized.
            virtual String GetDeviceName() const = 0;
            /// @brief Sets the frequency then the recorded audio will have.
            /// @note If this recorder is already initialized, calling this method may re-initialize it and clear the recorded audio buffer in the process.
            /// @param Freq The frequency at which to record and store the audio data in Hz.
            /// @return Returns true if there were no errors in setting the frequency, false if there was a problem.
            virtual bool SetFrequency(const UInt32 Freq) = 0;
            /// @brief Gets the frequency of the captured audio.
            /// @return Returns a UInt32 containing the frequency of the captured audio in Hz.
            virtual UInt32 GetFrequency() const = 0;
            /// @brief Sets the bit configuration to use when recording data to the internal buffer.
            /// @note If this recorder is already initialized, calling this method may re-initialize it and clear the recorded audio buffer in the process.
            /// @param Config The bit configuration to record and store the audio data.  See enum @ref BitConfig for more information.
            /// @return Returns true if there were no errors in setting the bit configuration, false if there was a problem.
            virtual bool SetBitConfiguration(const BitConfig Config) = 0;
            /// @brief Gets the bit configuration for the audio captured by this recorder.
            /// @return Returns an BitConfig representing the bit format for the audio recorded.
            virtual BitConfig GetBitConfiguration() const = 0;
            /// @brief Sets the size of the internal buffer where the recorded audio may be placed.
            /// @param Size The size of the internal buffer in bytes.
            virtual bool SetInternalBufferSize(const UInt32 Size) = 0;
            /// @brief Gets the currently set size of the internal buffer in bytes.
            /// @return Returns a UInt32 representing the internal size of the buffer in bytes.
            virtual UInt32 GetInternalBufferSize() const = 0;
            /// @brief Gets the size of a single sample of recorded audio data.
            /// @return Returns a UInt32 representing the size of a sample from the recorder buffer.
            virtual UInt32 GetSampleSize() const = 0;
        };//Recorder
    }//Audio
}//Mezzanine

#endif
