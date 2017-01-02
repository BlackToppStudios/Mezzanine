// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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

#include "exception.h"
#include "serialization.h"

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
            Filter::Filter(EFXInterface* EFXMethods) :
                EFX(EFXMethods),
                Volume(1.0),
                LFVolume(1.0),
                HFVolume(1.0),
                InternalFilter(0),
                Dirty(false),
                Valid(false)
            {
                this->Valid = this->EFX->Supported;

                if( this->EFX->Supported ) {
                    // Create our filter
                    this->EFX->alGenFilters(1,&InternalFilter);
                }
            }

            Filter::Filter(const XML::Node& SelfRoot, EFXInterface* EFXMethods) :
                EFX(EFXMethods),
                Volume(1.0),
                LFVolume(1.0),
                HFVolume(1.0),
                InternalFilter(0),
                Dirty(false),
                Valid(false)
            {
                this->Valid = this->EFX->Supported;

                if( this->EFX->Supported ) {
                    // Create our filter
                    this->EFX->alGenFilters(1,&InternalFilter);
                }

                this->ProtoDeSerialize(SelfRoot);
            }

            Filter::~Filter()
            {
                if( InternalFilter != 0 ) {
                    // Obliterate our filter
                    this->EFX->alDeleteFilters(1,&InternalFilter);
                }
            }

            void Filter::UpdateVolume()
            {
                ALenum Type;
                this->EFX->alGetFilteri(this->InternalFilter,AL_FILTER_TYPE,&Type);
                switch(Type)
                {
                    case AL_FILTER_LOWPASS:   this->EFX->alFilterf(this->InternalFilter,AL_LOWPASS_GAIN,this->Volume);    break;
                    case AL_FILTER_HIGHPASS:  this->EFX->alFilterf(this->InternalFilter,AL_HIGHPASS_GAIN,this->Volume);   break;
                    case AL_FILTER_BANDPASS:  this->EFX->alFilterf(this->InternalFilter,AL_BANDPASS_GAIN,this->Volume);   break;
                }
            }

            void Filter::UpdateFrequencyVolume()
            {
                ALenum Type;
                EFX->alGetFilteri(this->InternalFilter,AL_FILTER_TYPE,&Type);
                switch(Type)
                {
                    case AL_FILTER_LOWPASS:   this->EFX->alFilterf(this->InternalFilter,AL_LOWPASS_GAINHF,this->HFVolume);   break;
                    case AL_FILTER_HIGHPASS:  this->EFX->alFilterf(this->InternalFilter,AL_HIGHPASS_GAINLF,this->LFVolume);  break;
                    case AL_FILTER_BANDPASS:
                    {
                        this->EFX->alFilterf(this->InternalFilter,AL_LOWPASS_GAINHF,this->HFVolume);
                        this->EFX->alFilterf(this->InternalFilter,AL_HIGHPASS_GAINLF,this->LFVolume);
                        break;
                    }
                }
            }

            Boole Filter::CheckValid()
            {
                Integer ALError = alGetError();
                return ( ALError == AL_NO_ERROR );
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            Boole Filter::IsValid() const
            {
                return this->Valid;
            }

            void Filter::SetType(const FilterType FilType)
            {
                if( this->Valid ) {
                    this->EFX->alFilteri(this->InternalFilter,AL_FILTER_TYPE,ConvertMezzanineFilterType(FilType));
                    this->UpdateVolume();
                    this->UpdateFrequencyVolume();
                    this->Dirty = true;
                    this->Valid = CheckValid();
                }
            }

            FilterType Filter::GetType() const
            {
                if( this->Valid ) {
                    ALenum Type;
                    this->EFX->alGetFilteri(this->InternalFilter,AL_FILTER_TYPE,&Type);
                    return ConvertInternalFilterType(Type);
                }else{
                    return Audio::FT_Null;
                }
            }

            void Filter::SetVolume(const Real Vol)
            {
                if( this->Volume != Vol && this->Valid ) {
                    this->Volume = Vol;
                    this->Dirty = true;
                    this->UpdateVolume();
                    this->Valid = CheckValid();
                }
            }

            Real Filter::GetVolume() const
            {
                return this->Volume;
            }

            void Filter::SetHighFrequencyVolume(const Real HFVol)
            {
                if( this->HFVolume != HFVol && this->Valid ) {
                    this->HFVolume = HFVol;
                    this->Dirty = true;
                    this->UpdateFrequencyVolume();
                    this->Valid = CheckValid();
                }
            }

            Real Filter::GetHighFrequencyVolume() const
            {
                return this->HFVolume;
            }

            void Filter::SetLowFrequencyVolume(const Real LFVol)
            {
                if( this->LFVolume != LFVol && this->Valid ) {
                    this->LFVolume = LFVol;
                    this->Dirty = true;
                    this->UpdateFrequencyVolume();
                    this->Valid = CheckValid();
                }
            }

            Real Filter::GetLowFrequencyVolume() const
            {
                return this->LFVolume;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            void Filter::ProtoSerialize(XML::Node& ParentNode) const
            {
                XML::Node SelfRoot = ParentNode.AppendChild(Filter::GetSerializableName());

                if( SelfRoot.AppendAttribute("Version").SetValue("1") &&
                    SelfRoot.AppendAttribute("FilterType").SetValue( this->GetType() ) &&
                    SelfRoot.AppendAttribute("Volume").SetValue( this->GetVolume() ) &&
                    SelfRoot.AppendAttribute("LowFreqVolume").SetValue( this->GetLowFrequencyVolume() ) &&
                    SelfRoot.AppendAttribute("HighFreqVolume").SetValue( this->GetHighFrequencyVolume() ) )
                {
                    return;
                }else{
                    SerializeError("Create XML Attribute Values",Filter::GetSerializableName(),true);
                }
            }

            void Filter::ProtoDeSerialize(const XML::Node& SelfRoot)
            {
                XML::Attribute CurrAttrib;

                if( SelfRoot.Name() == Filter::GetSerializableName() ) {
                    if(SelfRoot.GetAttribute("Version").AsInt() == 1) {
                        CurrAttrib = SelfRoot.GetAttribute("FilterType");
                        if( !CurrAttrib.Empty() )
                            this->SetType( static_cast<FilterType>( CurrAttrib.AsUint() ) );

                        CurrAttrib = SelfRoot.GetAttribute("Volume");
                        if( !CurrAttrib.Empty() )
                            this->SetVolume( CurrAttrib.AsReal() );

                        CurrAttrib = SelfRoot.GetAttribute("LowFreqVolume");
                        if( !CurrAttrib.Empty() )
                            this->SetLowFrequencyVolume( CurrAttrib.AsReal() );

                        CurrAttrib = SelfRoot.GetAttribute("HighFreqVolume");
                        if( !CurrAttrib.Empty() )
                            this->SetHighFrequencyVolume( CurrAttrib.AsReal() );
                    }else{
                        MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + Filter::GetSerializableName() + ": Not Version 1.");
                    }
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,Filter::GetSerializableName() + " was not found in the provided XML node, which was expected.");
                }
            }

            String Filter::GetSerializableName()
                { return "OALSFilter"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            UInt32 Filter::_GetInternalFilter() const
                { return this->InternalFilter; }

            Boole Filter::_IsDirty() const
                { return this->Dirty; }

            void Filter::_Clean()
                { this->Dirty = false; }
        }//OALS
    }//Audio
}//Mezzanine

#endif
