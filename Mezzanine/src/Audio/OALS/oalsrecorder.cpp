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
#ifndef _audiooalsrecorder_cpp
#define _audiooalsrecorder_cpp

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#define OALS_STRUCTS_DECLARED

#include "Audio/OALS/oalsrecorder.h"
#include "Audio/OALS/oalsdefines.h"

#include "exception.h"

#include <algorithm>

namespace
{
    /// @internal
    /// @brief Converts the BitConfig value to the internal type.
    /// @param Config The Mezzanine type to be converted.
    /// @return Returns the internal type corresponding to the provided Mezzanine type.
    ALenum ConvertBitConfigEnum(Mezzanine::Audio::BitConfig Config)
    {
        switch(Config)
		{
            case Mezzanine::Audio::BC_8Bit_Mono:     return AL_FORMAT_MONO8;     break;
            case Mezzanine::Audio::BC_16Bit_Mono:    return AL_FORMAT_MONO16;    break;
            case Mezzanine::Audio::BC_8Bit_Stereo:   return AL_FORMAT_STEREO8;   break;
            case Mezzanine::Audio::BC_16Bit_Stereo:  return AL_FORMAT_STEREO16;  break;
            default:                                 return AL_FORMAT_MONO8;     break;
		}
    }
}

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            bool Recorder::IsSupported()
            {
                return ( alcIsExtensionPresent(NULL, "ALC_EXT_CAPTURE") == AL_TRUE );
            }

            Recorder::Recorder()
                : RecorderDevice(NULL),
                  Recording(false),
                  BitConfiguration(Audio::BC_16Bit_Mono),
                  Frequency(22500),
                  InternalBufferSize(8192)
            {
            }

            Recorder::~Recorder()
            {

            }

            bool Recorder::InitializeDevice()
            {
                if(this->RecorderDevice != NULL)
                    this->ShutdownDevice();
                if(this->CurrentDeviceName.empty())
                    this->RecorderDevice = alcCaptureOpenDevice(NULL, this->Frequency, ConvertBitConfigEnum(this->BitConfiguration), this->InternalBufferSize / this->GetSampleSize());
                else
                    this->RecorderDevice = alcCaptureOpenDevice(this->CurrentDeviceName.c_str(), this->Frequency, ConvertBitConfigEnum(this->BitConfiguration), this->InternalBufferSize / this->GetSampleSize());

                if(this->RecorderDevice != NULL)
                {
                    this->CurrentDeviceName = alcGetString(this->RecorderDevice, ALC_CAPTURE_DEVICE_SPECIFIER);
                    return true;
                }
                return false;
            }

            void Recorder::ShutdownDevice()
            {
                if(this->Recording)
                    this->StopRecording();

                if(this->RecorderDevice != NULL)
                {
                    alcCaptureCloseDevice(RecorderDevice);
                    this->RecorderDevice = NULL;
                }
            }

            void Recorder::CheckError() const
            {
                if(RecorderDevice)
                {
                    int ErrorCode = alcGetError(RecorderDevice);
                    StringStream ExceptionStream;
                    switch(ErrorCode)
                    {
                        case AL_INVALID_NAME:
                        {
                            ExceptionStream << "Invalid name passed into OpenAL.  OpenAL error follows: " << alGetString(ErrorCode);
                            MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,ExceptionStream.str());
                            break;
                        }
                        case AL_INVALID_ENUM:
                        {
                            ExceptionStream << "Invalid enum passed into OpenAL.  OpenAL error follows: " << alGetString(ErrorCode);
                            MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,ExceptionStream.str());
                            break;
                        }
                        case AL_INVALID_VALUE:
                        {
                            ExceptionStream << "Invalid value passed into OpenAL.  OpenAL error follows: " << alGetString(ErrorCode);
                            MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,ExceptionStream.str());
                            break;
                        }
                        case AL_INVALID_OPERATION:
                        {
                            ExceptionStream << "Bad call to OpenAL.  OpenAL error follows: " << alGetString(ErrorCode);
                            MEZZ_EXCEPTION(Exception::INTERNAL_EXCEPTION,ExceptionStream.str());
                            break;
                        }
                        case AL_OUT_OF_MEMORY:
                        {
                            ExceptionStream << "Unable to allocate.  OpenAL error follows: " << alGetString(ErrorCode);
                            MEZZ_EXCEPTION(Exception::MM_OUT_OF_MEMORY_EXCEPTION,ExceptionStream.str());
                            break;
                        }
                    }
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Initialization and Shutdown

            bool Recorder::Initialize(const String& DeviceName, const UInt32 Freq, const BitConfig Config, const UInt32 IntBufSize)
            {
                this->CurrentDeviceName = DeviceName;
                this->Frequency = Freq;
                this->InternalBufferSize = IntBufSize;
                this->BitConfiguration = Config;

                this->ShutdownDevice();
                return this->InitializeDevice();
            }

            void Recorder::Shutdown()
            {
                this->ShutdownDevice();
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            bool Recorder::IsReady() const
            {
                return this->RecorderDevice != NULL;
            }

            bool Recorder::BeginRecording()
            {
                if(!this->Recording)
                {
                    if(this->RecorderDevice != NULL)
                    {
                        alcCaptureStart(this->RecorderDevice);
                        Recording = true;
                    }
                    this->CheckError();
                    return this->Recording;
                }
                this->CheckError();
                return false;
            }

            void Recorder::StopRecording()
            {
                if(this->RecorderDevice != NULL)
                {
                    alcCaptureStop(this->RecorderDevice);
                    this->CheckError();
                }
                Recording = false;
            }

            UInt32 Recorder::GetRecordedAudio(void* OutputBuffer, UInt32 OutputBufferSize)
            {
                if(this->RecorderDevice != NULL) {
                    Int32 AvailableSamples = 0;
                    alcGetIntegerv(this->RecorderDevice, ALC_CAPTURE_SAMPLES, 1, &AvailableSamples);

                    UInt32 SamplesToCopy = std::min(static_cast<UInt32>(AvailableSamples),OutputBufferSize / this->GetSampleSize());
                    alcCaptureSamples(this->RecorderDevice, OutputBuffer, SamplesToCopy);
                    this->CheckError();
                    return SamplesToCopy * this->GetSampleSize();
                }else{
                    return 0;
                }
            }

            UInt32 Recorder::GetBufferSize() const
            {
                if(this->RecorderDevice != NULL) {
                    Int32 AvailableSamples = 0;
                    alcGetIntegerv(this->RecorderDevice, ALC_CAPTURE_SAMPLES, 1, &AvailableSamples);
                    return AvailableSamples * this->GetSampleSize();
                    this->CheckError();
                }else{
                    return 0;
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Recording Configuration

            bool Recorder::SetDeviceName(const String& DeviceName)
            {
                this->CurrentDeviceName = DeviceName;
                if(this->RecorderDevice != NULL) {
                    this->ShutdownDevice();
                    return this->InitializeDevice();
                }else{
                    return true;
                }
            }

            String Recorder::GetDeviceName() const
            {
                return this->CurrentDeviceName;
            }

            bool Recorder::SetFrequency(const UInt32 Freq)
            {
                this->Frequency = Freq;
                if(this->RecorderDevice != NULL) {
                    this->ShutdownDevice();
                    return this->InitializeDevice();
                }else{
                    return true;
                }
            }

            UInt32 Recorder::GetFrequency() const
            {
                return this->Frequency;
            }

            bool Recorder::SetBitConfiguration(const BitConfig Config)
            {
                if( Config > Audio::BC_16Bit_Stereo )
                {
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Invalid BitConfiguration parameter.  The OLAS audio system does not support above 16-bit stereo.");
                }else{
                    this->BitConfiguration = Config;
                    if(this->RecorderDevice != NULL) {
                        this->ShutdownDevice();
                        return this->InitializeDevice();
                    }else{
                        return true;
                    }
                }
            }

            BitConfig Recorder::GetBitConfiguration() const
            {
                return this->BitConfiguration;
            }

            bool Recorder::SetInternalBufferSize(const UInt32 Size)
            {
                this->InternalBufferSize = Size;
                if(this->RecorderDevice != NULL) {
                    this->ShutdownDevice();
                    return this->InitializeDevice();
                }else{
                    return true;
                }
            }

            UInt32 Recorder::GetInternalBufferSize() const
            {
                return this->InternalBufferSize;
            }

            UInt32 Recorder::GetSampleSize() const
            {
                switch(this->BitConfiguration)
                {
                    case Audio::BC_8Bit_Mono:     return 1;  break;
                    case Audio::BC_8Bit_Stereo:   return 2;  break;
                    case Audio::BC_16Bit_Mono:    return 2;  break;
                    case Audio::BC_16Bit_Stereo:  return 4;  break;
                }
            }
        }//OALS
    }//Audio
}//Mezzanine

#endif
