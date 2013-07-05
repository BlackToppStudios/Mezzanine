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
// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio-ZLIBLicense.txt
#ifndef _audiooalsfilter_cpp
#define _audiooalsfilter_cpp

#include "Audio/OALS/oalsfilter.h"

#include "Audio/OALS/oalsefxinterface.h.cpp"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include <AL/efx.h>
#include <AL/efx-creative.h>
#include <AL/efx-presets.h>

namespace
{
    /// @internal
    /// @brief Converts the Mezzanine FilterType to the internal type.
    /// @param Type The Mezzanine type to be converted.
    /// @return Returns the internal type corresponding to the provided Mezzanine type.
    ALenum ConvertMezzanineFilterType(const Mezzanine::Audio::FilterType Type)
    {
        switch(Type)
        {
            case Mezzanine::Audio::FT_Null:       return AL_FILTER_NULL;      break;
            case Mezzanine::Audio::FT_LowPass:    return AL_FILTER_LOWPASS;   break;
            case Mezzanine::Audio::FT_HighPass:   return AL_FILTER_HIGHPASS;  break;
            case Mezzanine::Audio::FT_BandPass:   return AL_FILTER_BANDPASS;  break;
            default:                              return AL_FILTER_NULL;      break;
        }
    }
    /// @internal
    /// @brief Converts the internal type to the Mezzanine FilterType.
    /// @param Type The internal type to be converted.
    /// @return Returns the Mezzanine type corresponding to the provided internal type.
    Mezzanine::Audio::FilterType ConvertInternalFilterType(const ALenum Type)
    {
        switch(Type)
        {
            case AL_FILTER_NULL:      return Mezzanine::Audio::FT_Null;       break;
            case AL_FILTER_LOWPASS:   return Mezzanine::Audio::FT_LowPass;    break;
            case AL_FILTER_HIGHPASS:  return Mezzanine::Audio::FT_HighPass;   break;
            case AL_FILTER_BANDPASS:  return Mezzanine::Audio::FT_BandPass;   break;
            default:                  return Mezzanine::Audio::FT_Null;       break;
        }
    }
}

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            Filter::Filter(EFXInterface* EFXMethods)
                : EFX(EFXMethods),
                  Dirty(false),
                  Valid(false),
                  Volume(1.0),
                  LFVolume(1.0),
                  HFVolume(1.0),
                  InternalFilter(0)
            {
                this->Valid = EFX->Supported;

                if( EFX->Supported )
                {
                    // Create our filter
                    EFX->alGenFilters(1,&InternalFilter);
                }
            }

            Filter::~Filter()
            {
                if( InternalFilter != 0 )
                {
                    // Obliterate our filter
                    EFX->alDeleteFilters(1,&InternalFilter);
                }
            }

            void Filter::UpdateVolume()
            {
                ALenum Type;
                EFX->alGetFilteri(this->InternalFilter,AL_FILTER_TYPE,&Type);
                switch(Type)
                {
                    case AL_FILTER_LOWPASS:   EFX->alFilterf(this->InternalFilter,AL_LOWPASS_GAIN,this->Volume);    break;
                    case AL_FILTER_HIGHPASS:  EFX->alFilterf(this->InternalFilter,AL_HIGHPASS_GAIN,this->Volume);   break;
                    case AL_FILTER_BANDPASS:  EFX->alFilterf(this->InternalFilter,AL_BANDPASS_GAIN,this->Volume);   break;
                }
            }

            void Filter::UpdateFrequencyVolume()
            {
                ALenum Type;
                EFX->alGetFilteri(this->InternalFilter,AL_FILTER_TYPE,&Type);
                switch(Type)
                {
                    case AL_FILTER_LOWPASS:   EFX->alFilterf(this->InternalFilter,AL_LOWPASS_GAINHF,this->HFVolume);   break;
                    case AL_FILTER_HIGHPASS:  EFX->alFilterf(this->InternalFilter,AL_HIGHPASS_GAINLF,this->LFVolume);  break;
                    case AL_FILTER_BANDPASS:
                    {
                        EFX->alFilterf(this->InternalFilter,AL_LOWPASS_GAINHF,this->HFVolume);
                        EFX->alFilterf(this->InternalFilter,AL_HIGHPASS_GAINLF,this->LFVolume);
                        break;
                    }
                }
            }

            bool Filter::CheckError()
            {
                Integer ALError = alGetError();
                return ( ALError == AL_NO_ERROR );
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            bool Filter::IsValid() const
            {
                return this->Valid;
            }

            void Filter::SetType(const FilterType FilType)
            {
                if( this->Valid )
                {
                    EFX->alFilteri(this->InternalFilter,AL_FILTER_TYPE,ConvertMezzanineFilterType(FilType));
                    this->UpdateVolume();
                    this->UpdateFrequencyVolume();
                    this->Dirty = true;
                    this->Valid = CheckError();
                }
            }

            FilterType Filter::GetType() const
            {
                if( this->Valid ) {
                    ALenum Type;
                    EFX->alGetFilteri(this->InternalFilter,AL_FILTER_TYPE,&Type);
                    return ConvertInternalFilterType(Type);
                }else{
                    return Audio::FT_Null;
                }
            }

            void Filter::SetVolume(const Real Vol)
            {
                if( this->Volume != Vol && this->Valid )
                {
                    this->Volume = Vol;
                    this->Dirty = true;
                    this->UpdateVolume();
                    this->Valid = CheckError();
                }
            }

            Real Filter::GetVolume() const
            {
                return this->Volume;
            }

            void Filter::SetHighFrequencyVolume(const Real HFVol)
            {
                if( this->HFVolume != HFVol && this->Valid )
                {
                    this->HFVolume = HFVol;
                    this->Dirty = true;
                    this->UpdateFrequencyVolume();
                    this->Valid = CheckError();
                }
            }

            Real Filter::GetHighFrequencyVolume() const
            {
                return this->HFVolume;
            }

            void Filter::SetLowFrequencyVolume(const Real LFVol)
            {
                if( this->LFVolume != LFVol && this->Valid )
                {
                    this->LFVolume = LFVol;
                    this->Dirty = true;
                    this->UpdateFrequencyVolume();
                    this->Valid = CheckError();
                }
            }

            Real Filter::GetLowFrequencyVolume() const
            {
                return this->LFVolume;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            UInt32 Filter::_GetInternalFilter() const
            {
                return this->InternalFilter;
            }

            bool Filter::_IsDirty() const
            {
                return this->Dirty;
            }

            void Filter::_Clean()
            {
                this->Dirty = false;
            }
        }//OALS
    }//Audio
}//Mezzanine

#endif
