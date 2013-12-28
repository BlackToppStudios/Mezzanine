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
#ifndef _audiooalsrecorder_h
#define _audiooalsrecorder_h

#include "Audio/recorder.h"

//OAL forward declare
#ifndef OALS_STRUCTS_DECLARED
struct ALCdevice;
#endif //OALS_STRUCTS_DECLARED

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief This is the OpenALSoft implemenation for an Audio Recorder.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB Recorder : public iRecorder
            {
            public:
                /// @brief Checks to see if recorder functionality is available with the current backend and hardware.
                /// @return Returns true if audio recording is available, false otherwise.
                static bool IsSupported();
            protected:
                /// @internal
                /// @brief A pointer to the internal device doing the recording.
                ALCdevice* RecorderDevice;
                /// @internal
                /// @brief Stores whether audio is currently being recorded.
                bool Recording;
                /// @internal
                /// @brief The configuation to use when generating the recorded audio.
                BitConfig BitConfiguration;
                /// @internal
                /// @brief The audio frequency of the recorded audio.
                UInt32 Frequency;
                /// @internal
                /// @brief The size of the internal buffer where recorded audio is being placed.
                UInt32 InternalBufferSize;
                /// @internal
                /// @brief The name of the device currently in use by this recorder.
                String CurrentDeviceName;
                /// @internal
                /// @brief Initializes the recording device based on all the parameters set in this recorder instance.
                bool InitializeDevice();
                /// @internal
                /// @brief Shuts down the recording device and free's its buffers.
                void ShutdownDevice();
                /// @internal
                /// @brief Checks to see if there is an error message pending in the internal library.
                void CheckError() const;
            public:
                /// @brief Class constructor.
                Recorder();
                /// @brief Class destructor.
                virtual ~Recorder();

                ///////////////////////////////////////////////////////////////////////////////
                // Initialization and Shutdown

                /// @copydoc iRecorder::Initialize(const String&,  const UInt32, const BitConfig, const UInt32)
                virtual bool Initialize(const String& DeviceName = "",  const UInt32 Freq = 22050, const BitConfig Config = BC_16Bit_Mono, const UInt32 IntBufSize = 8192);
                /// @copydoc iRecorder::Shutdown()
                virtual void Shutdown();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc iRecorder::IsReady() const
                virtual bool IsReady() const;

                /// @copydoc iRecorder::BeginRecording()
                virtual bool BeginRecording();
                /// @copydoc iRecorder::StopRecording()
                virtual void StopRecording();
                /// @copydoc iRecorder::GetRecordedAudio(void*, UInt32)
                virtual UInt32 GetRecordedAudio(void* OutputBuffer, UInt32 OutputBufferSize);
                /// @copydoc iRecorder::GetBufferSize() const
                virtual UInt32 GetBufferSize() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Recording Configuration

                /// @copydoc iRecorder::SetDeviceName(const String&)
                virtual bool SetDeviceName(const String& DeviceName);
                /// @copydoc iRecorder::GetDeviceName() const
                virtual String GetDeviceName() const;
                /// @copydoc iRecorder::SetFrequency(const UInt32)
                virtual bool SetFrequency(const UInt32 Freq);
                /// @copydoc iRecorder::GetFrequency() const
                virtual UInt32 GetFrequency() const;
                /// @copydoc iRecorder::SetBitConfiguration(const BitConfig)
                virtual bool SetBitConfiguration(const BitConfig Config);
                /// @copydoc iRecorder::GetBitConfiguration() const
                virtual BitConfig GetBitConfiguration() const;
                /// @copydoc iRecorder::SetInternalBufferSize(const UInt32)
                virtual bool SetInternalBufferSize(const UInt32 Size);
                /// @copydoc iRecorder::GetInternalBufferSize() const
                virtual UInt32 GetInternalBufferSize() const;
                /// @copydoc iRecorder::GetSampleSize() const
                virtual UInt32 GetSampleSize() const;
            };//Recorder
        }//OALS
    }//Audio
}//Mezzanine

#endif
