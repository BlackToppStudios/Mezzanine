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
#ifndef _audiooalseffectshandler_cpp
#define _audiooalseffectshandler_cpp

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#define OALS_STRUCTS_DECLARED

#include "Audio/OALS/oalseffectshandler.h"
#include "Audio/OALS/oalsfilter.h"
#include "Audio/OALS/oalseffect.h"

#include "Audio/OALS/oalsefxinterface.h.cpp"

#include <AL/efx.h>
#include <AL/efx-creative.h>
#include <AL/efx-presets.h>

namespace
{
    /// @internal
    /// @brief Converts the FilterType value to the internal type.
    /// @param Type The Mezzanine type to be converted.
    /// @return Returns the internal type corresponding to the provided Mezzanine type.
    ALenum ConvertFilterType(const Mezzanine::Audio::FilterType Type)
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
    /// @brief Converts the EffectType value to the internal type.
    /// @param Type The Mezzanine type to be converted.
    /// @return Returns the internal type corresponding to the provided Mezzanine type.
    ALenum ConvertEffectType(const Mezzanine::Audio::EffectType Type)
    {
        switch(Type)
        {
            case Mezzanine::Audio::ET_Null:                return AL_EFFECT_NULL;               break;
            case Mezzanine::Audio::ET_EAX_Reverb:          return AL_EFFECT_EAXREVERB;          break;
            case Mezzanine::Audio::ET_Reverb:              return AL_EFFECT_REVERB;             break;
            case Mezzanine::Audio::ET_Chorus:              return AL_EFFECT_CHORUS;             break;
            case Mezzanine::Audio::ET_Distortion:          return AL_EFFECT_DISTORTION;         break;
            case Mezzanine::Audio::ET_Echo:                return AL_EFFECT_ECHO;               break;
            case Mezzanine::Audio::ET_Flanger:             return AL_EFFECT_FLANGER;            break;
            case Mezzanine::Audio::ET_Frequency_Shifter:   return AL_EFFECT_FREQUENCY_SHIFTER;  break;
            case Mezzanine::Audio::ET_Vocal_Morpher:       return AL_EFFECT_VOCAL_MORPHER;      break;
            case Mezzanine::Audio::ET_Pitch_Shifter:       return AL_EFFECT_PITCH_SHIFTER;      break;
            case Mezzanine::Audio::ET_Ring_Modulator:      return AL_EFFECT_RING_MODULATOR;     break;
            case Mezzanine::Audio::ET_Autowah:             return AL_EFFECT_AUTOWAH;            break;
            case Mezzanine::Audio::ET_Compressor:          return AL_EFFECT_COMPRESSOR;         break;
            case Mezzanine::Audio::ET_Equalizer:           return AL_EFFECT_EQUALIZER;          break;
            default:                                       return AL_EFFECT_NULL;               break;
        }
    }
}

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            EffectsHandler::EffectsHandler(ALCdevice* Device) :
                EFX(NULL)
                { this->EFX = new OALS::EFXInterface(Device); }

            EffectsHandler::~EffectsHandler()
            {
                this->DestroyAllFilters();
                this->DestroyAllEffects();
                delete this->EFX;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            ///////////////////////////////////////////////////////////////////////////////
            // Effect Handling

            Boole EffectsHandler::IsEffectSupported(const EffectType Type) const
            {
                if( this->EFX->Supported == false )
                    return false;

                ALuint EffectID;
                this->EFX->alGenEffects(1,&EffectID);
                this->EFX->alEffecti(EffectID,AL_EFFECT_TYPE,ConvertEffectType(Type));

                Boole Success = (alGetError() == AL_NO_ERROR);
                this->EFX->alDeleteEffects(1,&EffectID);
                return Success;
            }

            iEffect* EffectsHandler::CreateEffect()
            {
                /// @todo Throw an exception instead?
                if( this->EFX->Supported == false )
                    return NULL;

                OALS::Effect* NewEffect = new OALS::Effect(this);
                this->Effects.push_back(NewEffect);
                return NewEffect;
            }

            iEffect* EffectsHandler::CreateEffect(const XML::Node& SelfRoot)
            {
                /// @todo Throw an exception instead?
                if( this->EFX->Supported == false )
                    return NULL;

                OALS::Effect* NewEffect = new OALS::Effect(SelfRoot,this);
                this->Effects.push_back(NewEffect);
                return NewEffect;
            }

            iEffect* EffectsHandler::GetEffect(const UInt32 Index) const
                { return this->Effects[Index]; }

            UInt32 EffectsHandler::GetNumEffects() const
                { return this->Effects.size(); }

            void EffectsHandler::DestroyEffect(iEffect* ToBeDestroyed)
            {
                for( EffectIterator EffIt = this->Effects.begin() ; EffIt != this->Effects.end() ; ++EffIt )
                {
                    if( (*EffIt) == ToBeDestroyed ) {
                        this->Effects.erase(EffIt);
                        delete ToBeDestroyed;
                        return;
                    }
                }
            }

            void EffectsHandler::DestroyAllEffects()
            {
                for( EffectIterator EffIt = this->Effects.begin() ; EffIt != this->Effects.end() ; ++EffIt )
                    delete (*EffIt);

                this->Effects.clear();
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Filter Handling

            Boole EffectsHandler::IsFilterSupported(const FilterType Type) const
            {
                if( this->EFX->Supported == false )
                    return false;

                ALuint FilterID;
                this->EFX->alGenFilters(1,&FilterID);
                this->EFX->alFilteri(FilterID,AL_FILTER_TYPE,ConvertFilterType(Type));

                Boole Success = (alGetError() == AL_NO_ERROR);
                this->EFX->alDeleteFilters(1,&FilterID);
                return Success;
            }

            iFilter* EffectsHandler::CreateFilter()
            {
                /// @todo Throw an exception instead?
                if( this->EFX->Supported == false )
                    return NULL;

                OALS::Filter* NewFilter = new OALS::Filter(this->EFX);
                this->Filters.push_back(NewFilter);
                return NewFilter;
            }

            iFilter* EffectsHandler::CreateFilter(const XML::Node& SelfRoot)
            {
                /// @todo Throw an exception instead?
                if( this->EFX->Supported == false )
                    return NULL;

                OALS::Filter* NewFilter = new OALS::Filter(SelfRoot,this->EFX);
                this->Filters.push_back(NewFilter);
                return NewFilter;
            }

            iFilter* EffectsHandler::GetFilter(const UInt32 Index) const
                { return this->Filters[Index]; }

            UInt32 EffectsHandler::GetNumFilters() const
                { return this->Filters.size(); }

            void EffectsHandler::DestroyFilter(iFilter* ToBeDestroyed)
            {
                for( FilterIterator FilIt = this->Filters.begin() ; FilIt != this->Filters.end() ; ++FilIt )
                {
                    if( (*FilIt) == ToBeDestroyed ) {
                        this->Filters.erase(FilIt);
                        delete ToBeDestroyed;
                        return;
                    }
                }
            }

            void EffectsHandler::DestroyAllFilters()
            {
                for( FilterIterator FilIt = this->Filters.begin() ; FilIt != this->Filters.end() ; ++FilIt )
                    delete (*FilIt);

                this->Filters.clear();
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Preset Handling

            Boole EffectsHandler::EffectPresetExists(const EffectType Type, const String& Name)
            {
                switch(Type)
                {
                    case Audio::ET_EAX_Reverb:         return EAXReverbParams.count(Name);       break;
                    case Audio::ET_Reverb:             return ReverbParams.count(Name);          break;
                    case Audio::ET_Chorus:             return ChorusParams.count(Name);          break;
                    case Audio::ET_Distortion:         return DistortionParams.count(Name);      break;
                    case Audio::ET_Echo:               return EchoParams.count(Name);            break;
                    case Audio::ET_Flanger:            return FlangerParams.count(Name);         break;
                    case Audio::ET_Frequency_Shifter:  return FrequencyShiftParams.count(Name);  break;
                    case Audio::ET_Vocal_Morpher:      return VocalMorpherParams.count(Name);    break;
                    case Audio::ET_Pitch_Shifter:      return PitchShifterParams.count(Name);    break;
                    case Audio::ET_Ring_Modulator:     return RingModulatorParams.count(Name);   break;
                    case Audio::ET_Autowah:            return AutowahParams.count(Name);         break;
                    case Audio::ET_Compressor:         return CompressorParams.count(Name);      break;
                    case Audio::ET_Equalizer:          return EqualizerParams.count(Name);       break;
                    default:                           return false;                             break;
                }
            }

            void EffectsHandler::RemoveEffectPreset(const EffectType Type, const String& Name)
            {
                switch(Type)
                {
                    case Audio::ET_EAX_Reverb:         EAXReverbParams.erase(Name);       break;
                    case Audio::ET_Reverb:             ReverbParams.erase(Name);          break;
                    case Audio::ET_Chorus:             ChorusParams.erase(Name);          break;
                    case Audio::ET_Distortion:         DistortionParams.erase(Name);      break;
                    case Audio::ET_Echo:               EchoParams.erase(Name);            break;
                    case Audio::ET_Flanger:            FlangerParams.erase(Name);         break;
                    case Audio::ET_Frequency_Shifter:  FrequencyShiftParams.erase(Name);  break;
                    case Audio::ET_Vocal_Morpher:      VocalMorpherParams.erase(Name);    break;
                    case Audio::ET_Pitch_Shifter:      PitchShifterParams.erase(Name);    break;
                    case Audio::ET_Ring_Modulator:     RingModulatorParams.erase(Name);   break;
                    case Audio::ET_Autowah:            AutowahParams.erase(Name);         break;
                    case Audio::ET_Compressor:         CompressorParams.erase(Name);      break;
                    case Audio::ET_Equalizer:          EqualizerParams.erase(Name);       break;
                    case Audio::ET_Null:               return;                            break;
                }
            }

            void EffectsHandler::RemoveAllEffectPresets(const EffectType Type)
            {
                switch(Type)
                {
                    case Audio::ET_EAX_Reverb:         EAXReverbParams.clear();       break;
                    case Audio::ET_Reverb:             ReverbParams.clear();          break;
                    case Audio::ET_Chorus:             ChorusParams.clear();          break;
                    case Audio::ET_Distortion:         DistortionParams.clear();      break;
                    case Audio::ET_Echo:               EchoParams.clear();            break;
                    case Audio::ET_Flanger:            FlangerParams.clear();         break;
                    case Audio::ET_Frequency_Shifter:  FrequencyShiftParams.clear();  break;
                    case Audio::ET_Vocal_Morpher:      VocalMorpherParams.clear();    break;
                    case Audio::ET_Pitch_Shifter:      PitchShifterParams.clear();    break;
                    case Audio::ET_Ring_Modulator:     RingModulatorParams.clear();   break;
                    case Audio::ET_Autowah:            AutowahParams.clear();         break;
                    case Audio::ET_Compressor:         CompressorParams.clear();      break;
                    case Audio::ET_Equalizer:          EqualizerParams.clear();       break;
                    case Audio::ET_Null:               return;                        break;
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Individual effect type settings

            void EffectsHandler::AddEAXReverbEffectPreset(const String& Name, const EAXReverbParameters& Setting)
                { this->EAXReverbParams[Name] = Setting; }

            EAXReverbParameters EffectsHandler::GetEAXReverbEffectPreset(const String& Name) const
            {
                ConstEAXReverbParamIterator ParamIt = this->EAXReverbParams.find(Name);
                if( ParamIt != this->EAXReverbParams.end() ) return (*ParamIt).second;
                else return EAXReverbParameters();
            }

            void EffectsHandler::AddReverbEffectPreset(const String& Name, const ReverbParameters& Setting)
                { this->ReverbParams[Name] = Setting; }

            ReverbParameters EffectsHandler::GetReverbEffectPreset(const String& Name) const
            {
                ConstReverbParamIterator ParamIt = this->ReverbParams.find(Name);
                if( ParamIt != this->ReverbParams.end() ) return (*ParamIt).second;
                else return ReverbParameters();
            }

            void EffectsHandler::AddChorusEffectPreset(const String& Name, const ChorusParameters& Setting)
                { this->ChorusParams[Name] = Setting; }

            ChorusParameters EffectsHandler::GetChorusEffectPreset(const String& Name) const
            {
                ConstChorusParamIterator ParamIt = this->ChorusParams.find(Name);
                if( ParamIt != this->ChorusParams.end() ) return (*ParamIt).second;
                else return ChorusParameters();
            }

            void EffectsHandler::AddDistortionEffectPreset(const String& Name, const DistortionParameters& Setting)
                { this->DistortionParams[Name] = Setting; }

            DistortionParameters EffectsHandler::GetDistortionEffectPreset(const String& Name) const
            {
                ConstDistortionParamIterator ParamIt = this->DistortionParams.find(Name);
                if( ParamIt != this->DistortionParams.end() ) return (*ParamIt).second;
                else return DistortionParameters();
            }

            void EffectsHandler::AddEchoEffectPreset(const String& Name, const EchoParameters& Setting)
                { this->EchoParams[Name] = Setting; }

            EchoParameters EffectsHandler::GetEchoEffectPreset(const String& Name) const
            {
                ConstEchoParamIterator ParamIt = this->EchoParams.find(Name);
                if( ParamIt != this->EchoParams.end() ) return (*ParamIt).second;
                else return EchoParameters();
            }

            void EffectsHandler::AddFlangerEffectPreset(const String& Name, const FlangerParameters& Setting)
                { this->FlangerParams[Name] = Setting; }

            FlangerParameters EffectsHandler::GetFlangerEffectPreset(const String& Name) const
            {
                ConstFlangerParamIterator ParamIt = this->FlangerParams.find(Name);
                if( ParamIt != this->FlangerParams.end() ) return (*ParamIt).second;
                else return FlangerParameters();
            }

            void EffectsHandler::AddFrequencyShiftEffectPreset(const String& Name, const FrequencyShiftParameters& Setting)
                { this->FrequencyShiftParams[Name] = Setting; }

            FrequencyShiftParameters EffectsHandler::GetFrequencyShiftEffectPreset(const String& Name) const
            {
                ConstFrequencyShiftParamIterator ParamIt = this->FrequencyShiftParams.find(Name);
                if( ParamIt != this->FrequencyShiftParams.end() ) return (*ParamIt).second;
                else return FrequencyShiftParameters();
            }

            void EffectsHandler::AddVocalMorpherEffectPreset(const String& Name, const VocalMorpherParameters& Setting)
                { this->VocalMorpherParams[Name] = Setting; }

            VocalMorpherParameters EffectsHandler::GetVocalMorpherEffectPreset(const String& Name) const
            {
                ConstVocalMorpherParamIterator ParamIt = this->VocalMorpherParams.find(Name);
                if( ParamIt != this->VocalMorpherParams.end() ) return (*ParamIt).second;
                else return VocalMorpherParameters();
            }

            void EffectsHandler::AddPitchShifterEffectPreset(const String& Name, const PitchShifterParameters& Setting)
                { this->PitchShifterParams[Name] = Setting; }

            PitchShifterParameters EffectsHandler::GetPitchShifterEffectPreset(const String& Name) const
            {
                ConstPitchShifterParamIterator ParamIt = this->PitchShifterParams.find(Name);
                if( ParamIt != this->PitchShifterParams.end() ) return (*ParamIt).second;
                else return PitchShifterParameters();
            }

            void EffectsHandler::AddRingModulatorEffectPreset(const String& Name, const RingModulatorParameters& Setting)
                { this->RingModulatorParams[Name] = Setting; }

            RingModulatorParameters EffectsHandler::GetRingModulatorEffectPreset(const String& Name) const
            {
                ConstRingModulatorParamIterator ParamIt = this->RingModulatorParams.find(Name);
                if( ParamIt != this->RingModulatorParams.end() ) return (*ParamIt).second;
                else return RingModulatorParameters();
            }

            void EffectsHandler::AddAutowahEffectPreset(const String& Name, const AutowahParameters& Setting)
                { this->AutowahParams[Name] = Setting; }

            AutowahParameters EffectsHandler::GetAutowahEffectPreset(const String& Name) const
            {
                ConstAutowahParamIterator ParamIt = this->AutowahParams.find(Name);
                if( ParamIt != this->AutowahParams.end() ) return (*ParamIt).second;
                else return AutowahParameters();
            }

            void EffectsHandler::AddCompressorEffectPreset(const String& Name, const CompressorParameters& Setting)
                { this->CompressorParams[Name] = Setting; }

            CompressorParameters EffectsHandler::GetCompressorEffectPreset(const String& Name) const
            {
                ConstCompressorParamIterator ParamIt = this->CompressorParams.find(Name);
                if( ParamIt != this->CompressorParams.end() ) return (*ParamIt).second;
                else return CompressorParameters();
            }

            void EffectsHandler::AddEqualizerEffectPreset(const String& Name, const EqualizerParameters& Setting)
                { this->EqualizerParams[Name] = Setting; }

            EqualizerParameters EffectsHandler::GetEqualizerEffectPreset(const String& Name) const
            {
                ConstEqualizerParamIterator ParamIt = this->EqualizerParams.find(Name);
                if( ParamIt != this->EqualizerParams.end() ) return (*ParamIt).second;
                else return EqualizerParameters();
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            EFXInterface* EffectsHandler::_GetEFXInterface() const
                { return this->EFX; }

            void EffectsHandler::_CleanAll()
            {
                for( FilterIterator FilIt = this->Filters.begin() ; FilIt != this->Filters.end() ; ++FilIt )
                    { (*FilIt)->_Clean(); }

                for( EffectIterator EffIt = this->Effects.begin() ; EffIt != this->Effects.end() ; ++EffIt )
                    { (*EffIt)->_Clean(); }
            }
        }//OALS
    }//Audio
}//Mezzanine

#endif
