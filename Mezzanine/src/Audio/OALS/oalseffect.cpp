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
#ifndef _audiooalseffect_cpp
#define _audiooalseffect_cpp

#include "Audio/OALS/oalseffect.h"
#include "Audio/OALS/oalsfilter.h"
#include "exception.h"

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
    /// @brief Converts the Mezzanine EffectType to the internal type.
    /// @param Type The Mezzanine type to be converted.
    /// @return Returns the internal type corresponding to the provided Mezzanine type.
    ALenum ConvertMezzanineEffectType(const Mezzanine::Audio::EffectType Type)
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
    /// @internal
    /// @brief Converts the internal type to the Mezzanine EffectType.
    /// @param Type The internal type to be converted.
    /// @return Returns the Mezzanine type corresponding to the provided internal type.
    Mezzanine::Audio::EffectType ConvertInternalEffectType(const ALenum Type)
    {
        switch(Type)
        {
            case AL_EFFECT_NULL:                return Mezzanine::Audio::ET_Null;               break;
            case AL_EFFECT_EAXREVERB:           return Mezzanine::Audio::ET_EAX_Reverb;         break;
            case AL_EFFECT_REVERB:              return Mezzanine::Audio::ET_Reverb;             break;
            case AL_EFFECT_CHORUS:              return Mezzanine::Audio::ET_Chorus;             break;
            case AL_EFFECT_DISTORTION:          return Mezzanine::Audio::ET_Distortion;         break;
            case AL_EFFECT_ECHO:                return Mezzanine::Audio::ET_Echo;               break;
            case AL_EFFECT_FLANGER:             return Mezzanine::Audio::ET_Flanger;            break;
            case AL_EFFECT_FREQUENCY_SHIFTER:   return Mezzanine::Audio::ET_Frequency_Shifter;  break;
            case AL_EFFECT_VOCAL_MORPHER:       return Mezzanine::Audio::ET_Vocal_Morpher;      break;
            case AL_EFFECT_PITCH_SHIFTER:       return Mezzanine::Audio::ET_Pitch_Shifter;      break;
            case AL_EFFECT_RING_MODULATOR:      return Mezzanine::Audio::ET_Ring_Modulator;     break;
            case AL_EFFECT_AUTOWAH:             return Mezzanine::Audio::ET_Autowah;            break;
            case AL_EFFECT_COMPRESSOR:          return Mezzanine::Audio::ET_Compressor;         break;
            case AL_EFFECT_EQUALIZER:           return Mezzanine::Audio::ET_Equalizer;          break;
            default:                            return Mezzanine::Audio::ET_Null;               break;
        }
    }

    /// @internal
    /// @brief Converts the Mezzanine Modulator Waveform Type to the internal type.
    /// @param Type The Mezzanine type to be converted.
    /// @return Returns the internal type corresponding to the provided Mezzanine type.
    ALenum ConvertMezzanineModulatorWaveformType(const Mezzanine::Audio::RingModulatorParameters::ModulatorWaveform Type)
    {
        switch(Type)
        {
            case Mezzanine::Audio::RingModulatorParameters::EMW_Sinusoid:  return AL_RING_MODULATOR_SINUSOID;          break;
            case Mezzanine::Audio::RingModulatorParameters::EMW_Saw:       return AL_RING_MODULATOR_SAWTOOTH;          break;
            case Mezzanine::Audio::RingModulatorParameters::EMW_Square:    return AL_RING_MODULATOR_SQUARE;            break;
            default:                                                       return AL_RING_MODULATOR_DEFAULT_WAVEFORM;  break;
        }
    }
    /// @internal
    /// @brief Converts the internal type to the Mezzanine Modulator Waveform Type.
    /// @param Type The internal type to be converted.
    /// @return Returns the Mezzanine type corresponding to the provided internal type.
    Mezzanine::Audio::RingModulatorParameters::ModulatorWaveform ConvertInternalModulatorWaveformType(const ALenum Type)
    {
        switch(Type)
        {
            case AL_RING_MODULATOR_SINUSOID:  return Mezzanine::Audio::RingModulatorParameters::EMW_Sinusoid;  break;
            case AL_RING_MODULATOR_SAWTOOTH:  return Mezzanine::Audio::RingModulatorParameters::EMW_Saw;       break;
            case AL_RING_MODULATOR_SQUARE:    return Mezzanine::Audio::RingModulatorParameters::EMW_Square;    break;
        }
        return Mezzanine::Audio::RingModulatorParameters::EMW_Sinusoid;
    }

    /// @internal
    /// @brief Converts the Mezzanine Chorus Waveform Type to the internal type.
    /// @param Type The Mezzanine type to be converted.
    /// @return Returns the internal type corresponding to the provided Mezzanine type.
    ALenum ConvertMezzanineChorusWaveformType(const Mezzanine::Audio::ChorusParameters::ChorusWaveform Type)
    {
        switch(Type)
        {
            case Mezzanine::Audio::ChorusParameters::ECW_Sinusoid:  return AL_CHORUS_WAVEFORM_SINUSOID;   break;
            case Mezzanine::Audio::ChorusParameters::ECW_Triangle:  return AL_CHORUS_WAVEFORM_TRIANGLE;   break;
            default:                                                return AL_CHORUS_DEFAULT_WAVEFORM;    break;
        }
    }
    /// @internal
    /// @brief Converts the internal type to the Mezzanine Chorus Waveform Type.
    /// @param Type The internal type to be converted.
    /// @return Returns the Mezzanine type corresponding to the provided internal type.
    Mezzanine::Audio::ChorusParameters::ChorusWaveform ConvertInternalChorusWaveformType(const ALenum Type)
    {
        switch(Type)
        {
            case AL_CHORUS_WAVEFORM_SINUSOID:  return Mezzanine::Audio::ChorusParameters::ECW_Sinusoid;  break;
            case AL_CHORUS_WAVEFORM_TRIANGLE:  return Mezzanine::Audio::ChorusParameters::ECW_Triangle;  break;
        }
        return Mezzanine::Audio::ChorusParameters::ECW_Sinusoid;
    }

    /// @internal
    /// @brief Converts the Mezzanine Flanger Waveform Type to the internal type.
    /// @param Type The Mezzanine type to be converted.
    /// @return Returns the internal type corresponding to the provided Mezzanine type.
    ALenum ConvertMezzanineFlangerWaveformType(const Mezzanine::Audio::FlangerParameters::FlangerWaveform Type)
    {
        switch(Type)
        {
            case Mezzanine::Audio::FlangerParameters::EFW_Sinusoid:  return AL_FLANGER_WAVEFORM_SINUSOID;   break;
            case Mezzanine::Audio::FlangerParameters::EFW_Triangle:  return AL_FLANGER_WAVEFORM_TRIANGLE;   break;
            default:                                                 return AL_FLANGER_DEFAULT_WAVEFORM;    break;
        }
    }
    /// @internal
    /// @brief Converts the internal type to the Mezzanine Flanger Waveform Type.
    /// @param Type The internal type to be converted.
    /// @return Returns the Mezzanine type corresponding to the provided internal type.
    Mezzanine::Audio::FlangerParameters::FlangerWaveform ConvertInternalFlangerWaveformType(const ALenum Type)
    {
        switch(Type)
        {
            case AL_FLANGER_WAVEFORM_SINUSOID:  return Mezzanine::Audio::FlangerParameters::EFW_Sinusoid;  break;
            case AL_FLANGER_WAVEFORM_TRIANGLE:  return Mezzanine::Audio::FlangerParameters::EFW_Triangle;  break;
        }
        return Mezzanine::Audio::FlangerParameters::EFW_Sinusoid;
    }

    /// @internal
    /// @brief Converts the Mezzanine Shift Direction Type to the internal type.
    /// @param Type The Mezzanine type to be converted.
    /// @return Returns the internal type corresponding to the provided Mezzanine type.
    ALenum ConvertMezzanineShiftDirectionType(const Mezzanine::Audio::FrequencyShiftParameters::ShiftDirection Type)
    {
        switch(Type)
        {
            case Mezzanine::Audio::FrequencyShiftParameters::ESD_Down:  return AL_FREQUENCY_SHIFTER_DIRECTION_DOWN;   break;
            case Mezzanine::Audio::FrequencyShiftParameters::ESD_Up:    return AL_FREQUENCY_SHIFTER_DIRECTION_UP;     break;
            case Mezzanine::Audio::FrequencyShiftParameters::ESD_Off:   return AL_FREQUENCY_SHIFTER_DIRECTION_OFF;    break;
        }
        return AL_FREQUENCY_SHIFTER_DIRECTION_OFF;
    }
    /// @internal
    /// @brief Converts the internal type to the Mezzanine Shift Direction Type.
    /// @param Type The internal type to be converted.
    /// @return Returns the Mezzanine type corresponding to the provided internal type.
    Mezzanine::Audio::FrequencyShiftParameters::ShiftDirection ConvertInternalShiftDirectionType(const ALenum Type)
    {
        switch(Type)
        {
            case AL_FREQUENCY_SHIFTER_DIRECTION_DOWN:  return Mezzanine::Audio::FrequencyShiftParameters::ESD_Down;  break;
            case AL_FREQUENCY_SHIFTER_DIRECTION_UP:    return Mezzanine::Audio::FrequencyShiftParameters::ESD_Up;    break;
            case AL_FREQUENCY_SHIFTER_DIRECTION_OFF:   return Mezzanine::Audio::FrequencyShiftParameters::ESD_Off;   break;
        }
        return Mezzanine::Audio::FrequencyShiftParameters::ESD_Off;
    }

    /// @internal
    /// @brief Converts the Mezzanine Morpher Waveform Type to the internal type.
    /// @param Type The Mezzanine type to be converted.
    /// @return Returns the internal type corresponding to the provided Mezzanine type.
    ALenum ConvertMezzanineMorpherWaveformType(const Mezzanine::Audio::VocalMorpherParameters::MorpherWaveform Type)
    {
        switch(Type)
        {
            case Mezzanine::Audio::VocalMorpherParameters::EMW_Sinusoid:  return AL_VOCAL_MORPHER_WAVEFORM_SINUSOID;   break;
            case Mezzanine::Audio::VocalMorpherParameters::EMW_Triangle:  return AL_VOCAL_MORPHER_WAVEFORM_TRIANGLE;   break;
            case Mezzanine::Audio::VocalMorpherParameters::EMW_Saw:       return AL_VOCAL_MORPHER_WAVEFORM_SAWTOOTH;   break;
            default:                                                      return AL_VOCAL_MORPHER_DEFAULT_WAVEFORM;    break;
        }
    }
    /// @internal
    /// @brief Converts the internal type to the Mezzanine Morpher Waveform Type.
    /// @param Type The internal type to be converted.
    /// @return Returns the Mezzanine type corresponding to the provided internal type.
    Mezzanine::Audio::VocalMorpherParameters::MorpherWaveform ConvertInternalMorpherWaveformType(const ALenum Type)
    {
        switch(Type)
        {
            case AL_VOCAL_MORPHER_WAVEFORM_SINUSOID:  return Mezzanine::Audio::VocalMorpherParameters::EMW_Sinusoid;  break;
            case AL_VOCAL_MORPHER_WAVEFORM_TRIANGLE:  return Mezzanine::Audio::VocalMorpherParameters::EMW_Triangle;  break;
            case AL_VOCAL_MORPHER_WAVEFORM_SAWTOOTH:  return Mezzanine::Audio::VocalMorpherParameters::EMW_Saw;       break;
        }
        return Mezzanine::Audio::VocalMorpherParameters::EMW_Sinusoid;
    }

    /// @internal
    /// @brief Converts the Mezzanine Morpher Phoneme Type to the internal type.
    /// @param Type The Mezzanine type to be converted.
    /// @return Returns the internal type corresponding to the provided Mezzanine type.
    ALenum ConvertMezzanineMorpherPhonemeType(const Mezzanine::Audio::VocalMorpherParameters::MorpherPhoneme Type)
    {
        switch(Type)
        {
            case Mezzanine::Audio::VocalMorpherParameters::EMP_A:   return AL_VOCAL_MORPHER_PHONEME_A;   break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_E:   return AL_VOCAL_MORPHER_PHONEME_E;   break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_I:   return AL_VOCAL_MORPHER_PHONEME_I;   break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_O:   return AL_VOCAL_MORPHER_PHONEME_O;   break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_U:   return AL_VOCAL_MORPHER_PHONEME_U;   break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_AA:  return AL_VOCAL_MORPHER_PHONEME_AA;  break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_AE:  return AL_VOCAL_MORPHER_PHONEME_AE;  break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_AH:  return AL_VOCAL_MORPHER_PHONEME_AH;  break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_AO:  return AL_VOCAL_MORPHER_PHONEME_AO;  break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_EH:  return AL_VOCAL_MORPHER_PHONEME_EH;  break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_ER:  return AL_VOCAL_MORPHER_PHONEME_ER;  break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_IH:  return AL_VOCAL_MORPHER_PHONEME_IH;  break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_IY:  return AL_VOCAL_MORPHER_PHONEME_IY;  break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_UH:  return AL_VOCAL_MORPHER_PHONEME_UH;  break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_UW:  return AL_VOCAL_MORPHER_PHONEME_UW;  break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_B:   return AL_VOCAL_MORPHER_PHONEME_B;   break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_D:   return AL_VOCAL_MORPHER_PHONEME_D;   break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_F:   return AL_VOCAL_MORPHER_PHONEME_F;   break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_G:   return AL_VOCAL_MORPHER_PHONEME_G;   break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_J:   return AL_VOCAL_MORPHER_PHONEME_J;   break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_K:   return AL_VOCAL_MORPHER_PHONEME_K;   break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_L:   return AL_VOCAL_MORPHER_PHONEME_L;   break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_M:   return AL_VOCAL_MORPHER_PHONEME_M;   break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_N:   return AL_VOCAL_MORPHER_PHONEME_N;   break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_P:   return AL_VOCAL_MORPHER_PHONEME_P;   break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_R:   return AL_VOCAL_MORPHER_PHONEME_R;   break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_S:   return AL_VOCAL_MORPHER_PHONEME_S;   break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_T:   return AL_VOCAL_MORPHER_PHONEME_T;   break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_V:   return AL_VOCAL_MORPHER_PHONEME_V;   break;
            case Mezzanine::Audio::VocalMorpherParameters::EMP_Z:   return AL_VOCAL_MORPHER_PHONEME_Z;   break;
        }
        return AL_VOCAL_MORPHER_PHONEME_A;
    }
    /// @internal
    /// @brief Converts the internal type to the Mezzanine Morpher Phoneme Type.
    /// @param Type The internal type to be converted.
    /// @return Returns the Mezzanine type corresponding to the provided internal type.
    Mezzanine::Audio::VocalMorpherParameters::MorpherPhoneme ConvertInternalMorpherPhonemeType(const ALenum Type)
    {
        switch(Type)
        {
            case AL_VOCAL_MORPHER_PHONEME_A:   return Mezzanine::Audio::VocalMorpherParameters::EMP_A;   break;
            case AL_VOCAL_MORPHER_PHONEME_E:   return Mezzanine::Audio::VocalMorpherParameters::EMP_E;   break;
            case AL_VOCAL_MORPHER_PHONEME_I:   return Mezzanine::Audio::VocalMorpherParameters::EMP_I;   break;
            case AL_VOCAL_MORPHER_PHONEME_O:   return Mezzanine::Audio::VocalMorpherParameters::EMP_O;   break;
            case AL_VOCAL_MORPHER_PHONEME_U:   return Mezzanine::Audio::VocalMorpherParameters::EMP_U;   break;
            case AL_VOCAL_MORPHER_PHONEME_AA:  return Mezzanine::Audio::VocalMorpherParameters::EMP_AA;  break;
            case AL_VOCAL_MORPHER_PHONEME_AE:  return Mezzanine::Audio::VocalMorpherParameters::EMP_AE;  break;
            case AL_VOCAL_MORPHER_PHONEME_AH:  return Mezzanine::Audio::VocalMorpherParameters::EMP_AH;  break;
            case AL_VOCAL_MORPHER_PHONEME_AO:  return Mezzanine::Audio::VocalMorpherParameters::EMP_AO;  break;
            case AL_VOCAL_MORPHER_PHONEME_EH:  return Mezzanine::Audio::VocalMorpherParameters::EMP_EH;  break;
            case AL_VOCAL_MORPHER_PHONEME_ER:  return Mezzanine::Audio::VocalMorpherParameters::EMP_ER;  break;
            case AL_VOCAL_MORPHER_PHONEME_IH:  return Mezzanine::Audio::VocalMorpherParameters::EMP_IH;  break;
            case AL_VOCAL_MORPHER_PHONEME_IY:  return Mezzanine::Audio::VocalMorpherParameters::EMP_IY;  break;
            case AL_VOCAL_MORPHER_PHONEME_UH:  return Mezzanine::Audio::VocalMorpherParameters::EMP_UH;  break;
            case AL_VOCAL_MORPHER_PHONEME_UW:  return Mezzanine::Audio::VocalMorpherParameters::EMP_UW;  break;
            case AL_VOCAL_MORPHER_PHONEME_B:   return Mezzanine::Audio::VocalMorpherParameters::EMP_B;   break;
            case AL_VOCAL_MORPHER_PHONEME_D:   return Mezzanine::Audio::VocalMorpherParameters::EMP_D;   break;
            case AL_VOCAL_MORPHER_PHONEME_F:   return Mezzanine::Audio::VocalMorpherParameters::EMP_F;   break;
            case AL_VOCAL_MORPHER_PHONEME_G:   return Mezzanine::Audio::VocalMorpherParameters::EMP_G;   break;
            case AL_VOCAL_MORPHER_PHONEME_J:   return Mezzanine::Audio::VocalMorpherParameters::EMP_J;   break;
            case AL_VOCAL_MORPHER_PHONEME_K:   return Mezzanine::Audio::VocalMorpherParameters::EMP_K;   break;
            case AL_VOCAL_MORPHER_PHONEME_L:   return Mezzanine::Audio::VocalMorpherParameters::EMP_L;   break;
            case AL_VOCAL_MORPHER_PHONEME_M:   return Mezzanine::Audio::VocalMorpherParameters::EMP_M;   break;
            case AL_VOCAL_MORPHER_PHONEME_N:   return Mezzanine::Audio::VocalMorpherParameters::EMP_N;   break;
            case AL_VOCAL_MORPHER_PHONEME_P:   return Mezzanine::Audio::VocalMorpherParameters::EMP_P;   break;
            case AL_VOCAL_MORPHER_PHONEME_R:   return Mezzanine::Audio::VocalMorpherParameters::EMP_R;   break;
            case AL_VOCAL_MORPHER_PHONEME_S:   return Mezzanine::Audio::VocalMorpherParameters::EMP_S;   break;
            case AL_VOCAL_MORPHER_PHONEME_T:   return Mezzanine::Audio::VocalMorpherParameters::EMP_T;   break;
            case AL_VOCAL_MORPHER_PHONEME_V:   return Mezzanine::Audio::VocalMorpherParameters::EMP_V;   break;
            case AL_VOCAL_MORPHER_PHONEME_Z:   return Mezzanine::Audio::VocalMorpherParameters::EMP_Z;   break;
        }
        return Mezzanine::Audio::VocalMorpherParameters::EMP_A;
    }
}

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            Effect::Effect(EFXInterface* EFXMethods)
                : EFX(EFXMethods),
                  EffectFilter(NULL),
                  Dirty(false),
                  Valid(false),
                  IgnoreAtten(false),
                  Volume(1.0),
                  InternalEffect(0),
                  InternalEffectSlot(0)
            {
                this->Valid = EFX->Supported;

                if( EFX->Supported )
                {
                    // Create our Effect
                    EFX->alGenEffects(1,&InternalEffect);

                    // Create our Effect Slot
                    EFX->alGenAuxiliaryEffectSlots(1,&InternalEffectSlot);

                    // Attach the two
                    EFX->alAuxiliaryEffectSloti(this->InternalEffectSlot,AL_EFFECTSLOT_EFFECT,this->InternalEffect);
                }
            }

            Effect::~Effect()
            {
                // Detach our objects
                EFX->alAuxiliaryEffectSloti(this->InternalEffectSlot,AL_EFFECTSLOT_EFFECT,this->InternalEffect);

                if( this->InternalEffect != 0 )
                {
                    // Obliterate our effect
                    EFX->alDeleteEffects(1,&InternalEffect);
                }

                if( this->InternalEffectSlot != 0 )
                {
                    // Obliterate our effect slot
                    EFX->alDeleteAuxiliaryEffectSlots(1,&InternalEffectSlot);
                }
            }

            Boole Effect::CheckError()
            {
                Integer ALError = alGetError();
                return ( ALError == AL_NO_ERROR );
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            Boole Effect::IsValid() const
            {
                return this->Valid;
            }

            void Effect::SetType(const EffectType& EffType)
            {
                switch(EffType)
                {
                    case Audio::ET_Null:
                    case Audio::ET_EAX_Reverb:
                    case Audio::ET_Reverb:
                    {
                        this->EFX->alEffecti(this->InternalEffect,AL_EFFECT_TYPE,ConvertMezzanineEffectType(EffType));
                        break;
                    }
                    case Audio::ET_Chorus:
                    {
                        MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"OpenAL-Soft does not have an implementation for the Chorus Audio Effect.");
                        break;
                    }
                    case Audio::ET_Distortion:
                    {
                        MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"OpenAL-Soft does not have an implementation for the Distortion Audio Effect.");
                        break;
                    }
                    case Audio::ET_Echo:
                    {
                        this->EFX->alEffecti(this->InternalEffect,AL_EFFECT_TYPE,ConvertMezzanineEffectType(EffType));
                        break;
                    }
                    case Audio::ET_Flanger:
                    {
                        MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"OpenAL-Soft does not have an implementation for the Flanger Audio Effect.");
                        break;
                    }
                    case Audio::ET_Frequency_Shifter:
                    {
                        MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"OpenAL-Soft does not have an implementation for the Frequency Shift Audio Effect.");
                        break;
                    }
                    case Audio::ET_Vocal_Morpher:
                    {
                        MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"OpenAL-Soft does not have an implementation for the Vocal Morpher Audio Effect.");
                        break;
                    }
                    case Audio::ET_Pitch_Shifter:
                    {
                        MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"OpenAL-Soft does not have an implementation for the Pitch Shifter Audio Effect.");
                        break;
                    }
                    case Audio::ET_Ring_Modulator:
                    {
                        this->EFX->alEffecti(this->InternalEffect,AL_EFFECT_TYPE,ConvertMezzanineEffectType(EffType));
                        break;
                    }
                    case Audio::ET_Autowah:
                    {
                        MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"OpenAL-Soft does not have an implementation for the Autowah Audio Effect.");
                        break;
                    }
                    case Audio::ET_Compressor:
                    {
                        MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"OpenAL-Soft does not have an implementation for the Compressor Audio Effect.");
                        break;
                    }
                    case Audio::ET_Equalizer:
                    {
                        MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"OpenAL-Soft does not have an implementation for the Equalizer Audio Effect.");
                        break;
                    }
                }
            }

            EffectType Effect::GetType() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                return ConvertInternalEffectType(InternalType);
            }

            void Effect::AttachFilter(iFilter* Fil)
            {
                if( this->EffectFilter != Fil )
                {
                    this->EffectFilter = static_cast<OALS::Filter*>(Fil);
                    this->Dirty = true;
                }
            }

            iFilter* Effect::GetFilter() const
            {
                return this->EffectFilter;
            }

            void Effect::RemoveFilter()
            {
                this->EffectFilter = NULL;
            }

            void Effect::SetVolume(const Real Vol)
            {
                if( this->Volume != Vol )
                {
                    this->EFX->alAuxiliaryEffectSlotf(this->InternalEffectSlot,AL_EFFECTSLOT_GAIN,Vol);
                    this->Dirty = true;
                }
            }

            Real Effect::GetVolume() const
            {
                return this->Volume;
            }

            void Effect::IgnoreAttenuation(Boole Ignore)
            {
                if( this->IsIgnoringAttenuation() != Ignore )
                {
                    this->EFX->alAuxiliaryEffectSloti(this->InternalEffectSlot,AL_EFFECTSLOT_AUXILIARY_SEND_AUTO,( this->IgnoreAtten ? AL_TRUE : AL_FALSE ));
                    this->Dirty = true;
                }
            }

            Boole Effect::IsIgnoringAttenuation() const
            {
                return this->IgnoreAtten;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Parameter Configuration

            void Effect::SetEAXReverbParameters(const EAXReverbParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_EAXREVERB ) {
                    this->EFX->alEffectf(this->InternalEffect,AL_EAXREVERB_DENSITY,Param.Density);
                    this->EFX->alEffectf(this->InternalEffect,AL_EAXREVERB_DIFFUSION,Param.Diffusion);
                    this->EFX->alEffectf(this->InternalEffect,AL_EAXREVERB_GAIN,Param.Gain);
                    this->EFX->alEffectf(this->InternalEffect,AL_EAXREVERB_GAINHF,Param.GainHF);
                    this->EFX->alEffectf(this->InternalEffect,AL_EAXREVERB_GAINLF,Param.GainLF);
                    this->EFX->alEffectf(this->InternalEffect,AL_EAXREVERB_DECAY_TIME,Param.DecayTime);
                    this->EFX->alEffectf(this->InternalEffect,AL_EAXREVERB_DECAY_HFRATIO,Param.DecayHFRatio);
                    this->EFX->alEffectf(this->InternalEffect,AL_EAXREVERB_DECAY_LFRATIO,Param.DecayLFRatio);
                    this->EFX->alEffectf(this->InternalEffect,AL_EAXREVERB_REFLECTIONS_GAIN,Param.ReflectionsGain);
                    this->EFX->alEffectf(this->InternalEffect,AL_EAXREVERB_REFLECTIONS_DELAY,Param.ReflectionsDelay);
                    this->EFX->alEffectfv(this->InternalEffect,AL_EAXREVERB_REFLECTIONS_PAN,&Param.ReflectionsPan.X);
                    this->EFX->alEffectf(this->InternalEffect,AL_EAXREVERB_LATE_REVERB_GAIN,Param.LateReverbGain);
                    this->EFX->alEffectf(this->InternalEffect,AL_EAXREVERB_LATE_REVERB_DELAY,Param.LateReverbDelay);
                    this->EFX->alEffectfv(this->InternalEffect,AL_EAXREVERB_LATE_REVERB_PAN,&Param.LateReverbPan.X);
                    this->EFX->alEffectf(this->InternalEffect,AL_EAXREVERB_ECHO_TIME,Param.EchoTime);
                    this->EFX->alEffectf(this->InternalEffect,AL_EAXREVERB_ECHO_DEPTH,Param.EchoDepth);
                    this->EFX->alEffectf(this->InternalEffect,AL_EAXREVERB_MODULATION_TIME,Param.ModulationTime);
                    this->EFX->alEffectf(this->InternalEffect,AL_EAXREVERB_MODULATION_DEPTH,Param.ModulationDepth);
                    this->EFX->alEffectf(this->InternalEffect,AL_EAXREVERB_HFREFERENCE,Param.HFReference);
                    this->EFX->alEffectf(this->InternalEffect,AL_EAXREVERB_LFREFERENCE,Param.LFReference);
                    this->EFX->alEffectf(this->InternalEffect,AL_EAXREVERB_ROOM_ROLLOFF_FACTOR,Param.RoomRolloffFactor);
                    this->EFX->alEffectf(this->InternalEffect,AL_EAXREVERB_AIR_ABSORPTION_GAINHF,Param.AirAbsorptionGainHF);
                    this->EFX->alEffecti(this->InternalEffect,AL_EAXREVERB_DECAY_HFLIMIT,(Param.DecayHFLimit ? AL_TRUE : AL_FALSE));
                    this->Valid = this->CheckError();
                }
            }

            EAXReverbParameters Effect::GetEAXReverbParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_EAXREVERB ) {
                    EAXReverbParameters Ret;

                    this->EFX->alGetEffectf(this->InternalEffect,AL_EAXREVERB_DENSITY,&Ret.Density);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EAXREVERB_DIFFUSION,&Ret.Diffusion);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EAXREVERB_GAIN,&Ret.Gain);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EAXREVERB_GAINHF,&Ret.GainHF);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EAXREVERB_GAINLF,&Ret.GainLF);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EAXREVERB_DECAY_TIME,&Ret.DecayTime);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EAXREVERB_DECAY_HFRATIO,&Ret.DecayHFRatio);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EAXREVERB_DECAY_LFRATIO,&Ret.DecayLFRatio);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EAXREVERB_REFLECTIONS_GAIN,&Ret.ReflectionsGain);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EAXREVERB_REFLECTIONS_DELAY,&Ret.ReflectionsDelay);
                    this->EFX->alGetEffectfv(this->InternalEffect,AL_EAXREVERB_REFLECTIONS_PAN,&Ret.ReflectionsPan.X);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EAXREVERB_LATE_REVERB_GAIN,&Ret.LateReverbGain);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EAXREVERB_LATE_REVERB_DELAY,&Ret.LateReverbDelay);
                    this->EFX->alGetEffectfv(this->InternalEffect,AL_EAXREVERB_LATE_REVERB_PAN,&Ret.LateReverbPan.X);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EAXREVERB_ECHO_TIME,&Ret.EchoTime);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EAXREVERB_ECHO_DEPTH,&Ret.EchoDepth);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EAXREVERB_MODULATION_TIME,&Ret.ModulationTime);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EAXREVERB_MODULATION_DEPTH,&Ret.ModulationDepth);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EAXREVERB_HFREFERENCE,&Ret.HFReference);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EAXREVERB_LFREFERENCE,&Ret.LFReference);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EAXREVERB_ROOM_ROLLOFF_FACTOR,&Ret.RoomRolloffFactor);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EAXREVERB_AIR_ABSORPTION_GAINHF,&Ret.AirAbsorptionGainHF);

                    ALint BoolParam;
                    this->EFX->alGetEffecti(this->InternalEffect,AL_EAXREVERB_DECAY_HFLIMIT,&BoolParam);
                    Ret.DecayHFLimit = static_cast<Boole>(BoolParam);

                    return Ret;
                }else{
                    return EAXReverbParameters();
                }
            }

            void Effect::SetReverbParameters(const ReverbParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_REVERB ) {
                    this->EFX->alEffectf(this->InternalEffect,AL_REVERB_DENSITY,Param.Density);
                    this->EFX->alEffectf(this->InternalEffect,AL_REVERB_DIFFUSION,Param.Diffusion);
                    this->EFX->alEffectf(this->InternalEffect,AL_REVERB_GAIN,Param.Gain);
                    this->EFX->alEffectf(this->InternalEffect,AL_REVERB_GAINHF,Param.GainHF);
                    this->EFX->alEffectf(this->InternalEffect,AL_REVERB_DECAY_TIME,Param.DecayTime);
                    this->EFX->alEffectf(this->InternalEffect,AL_REVERB_DECAY_HFRATIO,Param.DecayHFRatio);
                    this->EFX->alEffectf(this->InternalEffect,AL_REVERB_REFLECTIONS_GAIN,Param.ReflectionsGain);
                    this->EFX->alEffectf(this->InternalEffect,AL_REVERB_REFLECTIONS_DELAY,Param.ReflectionsDelay);
                    this->EFX->alEffectf(this->InternalEffect,AL_REVERB_LATE_REVERB_GAIN,Param.LateReverbGain);
                    this->EFX->alEffectf(this->InternalEffect,AL_REVERB_LATE_REVERB_DELAY,Param.LateReverbDelay);
                    this->EFX->alEffectf(this->InternalEffect,AL_REVERB_ROOM_ROLLOFF_FACTOR,Param.RoomRolloffFactor);
                    this->EFX->alEffectf(this->InternalEffect,AL_REVERB_AIR_ABSORPTION_GAINHF,Param.AirAbsorptionGainHF);
                    this->EFX->alEffecti(this->InternalEffect,AL_REVERB_DECAY_HFLIMIT,(Param.DecayHFLimit ? AL_TRUE : AL_FALSE));
                    this->Valid = this->CheckError();
                }
            }

            ReverbParameters Effect::GetReverbParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_REVERB ) {
                    ReverbParameters Ret;

                    this->EFX->alGetEffectf(this->InternalEffect,AL_REVERB_DENSITY,&Ret.Density);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_REVERB_DIFFUSION,&Ret.Diffusion);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_REVERB_GAIN,&Ret.Gain);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_REVERB_GAINHF,&Ret.GainHF);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_REVERB_DECAY_TIME,&Ret.DecayTime);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_REVERB_DECAY_HFRATIO,&Ret.DecayHFRatio);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_REVERB_REFLECTIONS_GAIN,&Ret.ReflectionsGain);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_REVERB_REFLECTIONS_DELAY,&Ret.ReflectionsDelay);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_REVERB_LATE_REVERB_GAIN,&Ret.LateReverbGain);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_REVERB_LATE_REVERB_DELAY,&Ret.LateReverbDelay);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_REVERB_ROOM_ROLLOFF_FACTOR,&Ret.RoomRolloffFactor);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_REVERB_AIR_ABSORPTION_GAINHF,&Ret.AirAbsorptionGainHF);

                    ALint BoolParam;
                    this->EFX->alGetEffecti(this->InternalEffect,AL_REVERB_DECAY_HFLIMIT,&BoolParam);
                    Ret.DecayHFLimit = static_cast<Boole>(BoolParam);

                    return Ret;
                }else{
                    return ReverbParameters();
                }
            }

            void Effect::SetChorusParameters(const ChorusParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_CHORUS ) {
                    this->EFX->alEffecti(this->InternalEffect,AL_CHORUS_WAVEFORM,ConvertMezzanineChorusWaveformType(Param.Waveform));
                    this->EFX->alEffecti(this->InternalEffect,AL_CHORUS_PHASE,Param.Phase);
                    this->EFX->alEffectf(this->InternalEffect,AL_CHORUS_RATE,Param.Rate);
                    this->EFX->alEffectf(this->InternalEffect,AL_CHORUS_DEPTH,Param.Depth);
                    this->EFX->alEffectf(this->InternalEffect,AL_CHORUS_FEEDBACK,Param.Feedback);
                    this->EFX->alEffectf(this->InternalEffect,AL_CHORUS_DELAY,Param.Delay);
                    this->Valid = this->CheckError();
                }
            }

            ChorusParameters Effect::GetChorusParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_CHORUS ) {
                    ChorusParameters Ret;

                    ALenum WaveformType;
                    this->EFX->alGetEffecti(this->InternalEffect,AL_CHORUS_WAVEFORM,&WaveformType);
                    Ret.Waveform = ConvertInternalChorusWaveformType(WaveformType);

                    this->EFX->alGetEffecti(this->InternalEffect,AL_CHORUS_PHASE,&Ret.Phase);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_CHORUS_RATE,&Ret.Rate);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_CHORUS_DEPTH,&Ret.Depth);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_CHORUS_FEEDBACK,&Ret.Feedback);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_CHORUS_DELAY,&Ret.Delay);
                    return Ret;
                }else{
                    return ChorusParameters();
                }
            }

            void Effect::SetDistortionParameters(const DistortionParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_DISTORTION ) {
                    this->EFX->alEffectf(this->InternalEffect,AL_DISTORTION_EDGE,Param.Edge);
                    this->EFX->alEffectf(this->InternalEffect,AL_DISTORTION_GAIN,Param.Gain);
                    this->EFX->alEffectf(this->InternalEffect,AL_DISTORTION_LOWPASS_CUTOFF,Param.LowpassCutoff);
                    this->EFX->alEffectf(this->InternalEffect,AL_DISTORTION_EQCENTER,Param.EqCenter);
                    this->EFX->alEffectf(this->InternalEffect,AL_DISTORTION_EQBANDWIDTH,Param.EqBandwidth);
                    this->Valid = this->CheckError();
                }
            }

            DistortionParameters Effect::GetDistortionParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_DISTORTION ) {
                    DistortionParameters Ret;

                    this->EFX->alGetEffectf(this->InternalEffect,AL_DISTORTION_EDGE,&Ret.Edge);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_DISTORTION_GAIN,&Ret.Gain);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_DISTORTION_LOWPASS_CUTOFF,&Ret.LowpassCutoff);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_DISTORTION_EQCENTER,&Ret.EqCenter);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_DISTORTION_EQBANDWIDTH,&Ret.EqBandwidth);
                    return Ret;
                }else{
                    return DistortionParameters();
                }
            }

            void Effect::SetEchoParameters(const EchoParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_ECHO ) {
                    this->EFX->alEffectf(this->InternalEffect,AL_ECHO_DELAY,Param.Delay);
                    this->EFX->alEffectf(this->InternalEffect,AL_ECHO_LRDELAY,Param.LRDelay);
                    this->EFX->alEffectf(this->InternalEffect,AL_ECHO_DAMPING,Param.Damping);
                    this->EFX->alEffectf(this->InternalEffect,AL_ECHO_FEEDBACK,Param.Feedback);
                    this->EFX->alEffectf(this->InternalEffect,AL_ECHO_SPREAD,Param.Spread);
                    this->Valid = this->CheckError();
                }
            }

            EchoParameters Effect::GetEchoParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_ECHO ) {
                    EchoParameters Ret;

                    this->EFX->alGetEffectf(this->InternalEffect,AL_ECHO_DELAY,&Ret.Delay);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_ECHO_LRDELAY,&Ret.LRDelay);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_ECHO_DAMPING,&Ret.Damping);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_ECHO_FEEDBACK,&Ret.Feedback);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_ECHO_SPREAD,&Ret.Spread);
                    return Ret;
                }else{
                    return EchoParameters();
                }
            }

            void Effect::SetFlangerParameters(const FlangerParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_FLANGER ) {
                    this->EFX->alEffecti(this->InternalEffect,AL_FLANGER_WAVEFORM,ConvertMezzanineFlangerWaveformType(Param.Waveform));
                    this->EFX->alEffecti(this->InternalEffect,AL_FLANGER_PHASE,Param.Phase);
                    this->EFX->alEffectf(this->InternalEffect,AL_FLANGER_RATE,Param.Rate);
                    this->EFX->alEffectf(this->InternalEffect,AL_FLANGER_DEPTH,Param.Depth);
                    this->EFX->alEffectf(this->InternalEffect,AL_FLANGER_FEEDBACK,Param.Feedback);
                    this->EFX->alEffectf(this->InternalEffect,AL_FLANGER_DELAY,Param.Delay);
                    this->Valid = this->CheckError();
                }
            }

            FlangerParameters Effect::GetFlangerParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_FLANGER ) {
                    FlangerParameters Ret;

                    ALenum WaveformType;
                    this->EFX->alGetEffecti(this->InternalEffect,AL_FLANGER_WAVEFORM,&WaveformType);
                    Ret.Waveform = ConvertInternalFlangerWaveformType(WaveformType);

                    this->EFX->alGetEffecti(this->InternalEffect,AL_FLANGER_PHASE,&Ret.Phase);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_FLANGER_RATE,&Ret.Rate);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_FLANGER_DEPTH,&Ret.Depth);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_FLANGER_FEEDBACK,&Ret.Feedback);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_FLANGER_DELAY,&Ret.Delay);
                    return Ret;
                }else{
                    return FlangerParameters();
                }
            }

            void Effect::SetFrequencyShiftParameters(const FrequencyShiftParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_FREQUENCY_SHIFTER ) {
                    this->EFX->alEffecti(this->InternalEffect,AL_FREQUENCY_SHIFTER_LEFT_DIRECTION,ConvertMezzanineShiftDirectionType(Param.Left));
                    this->EFX->alEffecti(this->InternalEffect,AL_FREQUENCY_SHIFTER_RIGHT_DIRECTION,ConvertMezzanineShiftDirectionType(Param.Right));
                    this->EFX->alEffectf(this->InternalEffect,AL_FREQUENCY_SHIFTER_FREQUENCY,Param.Frequency);
                    this->Valid = this->CheckError();
                }
            }

            FrequencyShiftParameters Effect::GetFrequencyShiftParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_FREQUENCY_SHIFTER ) {
                    FrequencyShiftParameters Ret;

                    ALenum LeftWaveformType;
                    this->EFX->alGetEffecti(this->InternalEffect,AL_FREQUENCY_SHIFTER_LEFT_DIRECTION,&LeftWaveformType);
                    Ret.Left = ConvertInternalShiftDirectionType(LeftWaveformType);

                    ALenum RightWaveformType;
                    this->EFX->alGetEffecti(this->InternalEffect,AL_FREQUENCY_SHIFTER_RIGHT_DIRECTION,&RightWaveformType);
                    Ret.Right = ConvertInternalShiftDirectionType(RightWaveformType);

                    this->EFX->alGetEffectf(this->InternalEffect,AL_FREQUENCY_SHIFTER_FREQUENCY,&Ret.Frequency);
                    return Ret;
                }else{
                    return FrequencyShiftParameters();
                }
            }

            void Effect::SetVocalMorpherParameters(const VocalMorpherParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_VOCAL_MORPHER ) {
                    this->EFX->alEffecti(this->InternalEffect,AL_VOCAL_MORPHER_PHONEMEA,ConvertMezzanineMorpherPhonemeType(Param.PhonemeA));
                    this->EFX->alEffecti(this->InternalEffect,AL_VOCAL_MORPHER_PHONEMEB,ConvertMezzanineMorpherPhonemeType(Param.PhonemeB));
                    this->EFX->alEffecti(this->InternalEffect,AL_VOCAL_MORPHER_WAVEFORM,ConvertMezzanineMorpherWaveformType(Param.Waveform));
                    this->EFX->alEffecti(this->InternalEffect,AL_VOCAL_MORPHER_PHONEMEA_COARSE_TUNING,Param.PhonemeACoarseTune);
                    this->EFX->alEffecti(this->InternalEffect,AL_VOCAL_MORPHER_PHONEMEB_COARSE_TUNING,Param.PhonemeBCoarseTune);
                    this->EFX->alEffectf(this->InternalEffect,AL_VOCAL_MORPHER_RATE,Param.Rate);
                    this->Valid = this->CheckError();
                }
            }

            VocalMorpherParameters Effect::GetVocalMorpherParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_VOCAL_MORPHER ) {
                    VocalMorpherParameters Ret;

                    ALenum PhonemeAType;
                    this->EFX->alGetEffecti(this->InternalEffect,AL_VOCAL_MORPHER_PHONEMEA,&PhonemeAType);
                    Ret.PhonemeA = ConvertInternalMorpherPhonemeType(PhonemeAType);

                    ALenum PhonemeBType;
                    this->EFX->alGetEffecti(this->InternalEffect,AL_VOCAL_MORPHER_PHONEMEB,&PhonemeBType);
                    Ret.PhonemeB = ConvertInternalMorpherPhonemeType(PhonemeBType);

                    ALenum WaveformType;
                    this->EFX->alGetEffecti(this->InternalEffect,AL_VOCAL_MORPHER_WAVEFORM,&WaveformType);
                    Ret.Waveform = ConvertInternalMorpherWaveformType(WaveformType);

                    this->EFX->alGetEffecti(this->InternalEffect,AL_VOCAL_MORPHER_PHONEMEA_COARSE_TUNING,&Ret.PhonemeACoarseTune);
                    this->EFX->alGetEffecti(this->InternalEffect,AL_VOCAL_MORPHER_PHONEMEB_COARSE_TUNING,&Ret.PhonemeBCoarseTune);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_VOCAL_MORPHER_RATE,&Ret.Rate);
                    return Ret;
                }else{
                    return VocalMorpherParameters();
                }
            }

            void Effect::SetPitchShifterParameters(const PitchShifterParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_PITCH_SHIFTER ) {
                    this->EFX->alEffecti(this->InternalEffect,AL_PITCH_SHIFTER_COARSE_TUNE,Param.CoarseTune);
                    this->EFX->alEffecti(this->InternalEffect,AL_PITCH_SHIFTER_FINE_TUNE,Param.FineTune);
                    this->Valid = this->CheckError();
                }
            }

            PitchShifterParameters Effect::GetPitchShifterParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_PITCH_SHIFTER ) {
                    PitchShifterParameters Ret;

                    this->EFX->alGetEffecti(this->InternalEffect,AL_PITCH_SHIFTER_COARSE_TUNE,&Ret.CoarseTune);
                    this->EFX->alGetEffecti(this->InternalEffect,AL_PITCH_SHIFTER_FINE_TUNE,&Ret.FineTune);
                    return Ret;
                }else{
                    return PitchShifterParameters();
                }
            }

            void Effect::SetRingModulatorParameters(const RingModulatorParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_RING_MODULATOR ) {
                    this->EFX->alEffecti(this->InternalEffect,AL_RING_MODULATOR_WAVEFORM,ConvertMezzanineModulatorWaveformType(Param.Waveform));
                    this->EFX->alEffectf(this->InternalEffect,AL_RING_MODULATOR_FREQUENCY,Param.Frequency);
                    this->EFX->alEffectf(this->InternalEffect,AL_RING_MODULATOR_HIGHPASS_CUTOFF,Param.HighPassCutoff);
                    this->Valid = this->CheckError();
                }
            }

            RingModulatorParameters Effect::GetRingModulatorParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_RING_MODULATOR ) {
                    RingModulatorParameters Ret;

                    ALenum WaveformType;
                    this->EFX->alGetEffecti(this->InternalEffect,AL_RING_MODULATOR_WAVEFORM,&WaveformType);
                    Ret.Waveform = ConvertInternalModulatorWaveformType(WaveformType);

                    this->EFX->alGetEffectf(this->InternalEffect,AL_RING_MODULATOR_FREQUENCY,&Ret.Frequency);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_RING_MODULATOR_HIGHPASS_CUTOFF,&Ret.HighPassCutoff);
                    return Ret;
                }else{
                    return RingModulatorParameters();
                }
            }

            void Effect::SetAutowahParameters(const AutowahParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_AUTOWAH ) {
                    this->EFX->alEffectf(this->InternalEffect,AL_AUTOWAH_ATTACK_TIME,Param.AttackTime);
                    this->EFX->alEffectf(this->InternalEffect,AL_AUTOWAH_RELEASE_TIME,Param.ReleaseTime);
                    this->EFX->alEffectf(this->InternalEffect,AL_AUTOWAH_RESONANCE,Param.Resonance);
                    this->EFX->alEffectf(this->InternalEffect,AL_AUTOWAH_PEAK_GAIN,Param.PeakGain);
                    this->Valid = this->CheckError();
                }
            }

            AutowahParameters Effect::GetAutowahParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_AUTOWAH ) {
                    AutowahParameters Ret;

                    this->EFX->alGetEffectf(this->InternalEffect,AL_AUTOWAH_ATTACK_TIME,&Ret.AttackTime);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_AUTOWAH_RELEASE_TIME,&Ret.ReleaseTime);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_AUTOWAH_RESONANCE,&Ret.Resonance);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_AUTOWAH_PEAK_GAIN,&Ret.PeakGain);
                    return Ret;
                }else{
                    return AutowahParameters();
                }
            }

            void Effect::SetCompressorParameters(const CompressorParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_COMPRESSOR ) {
                    this->EFX->alEffecti(this->InternalEffect,AL_COMPRESSOR_ONOFF,( Param.Active ? AL_TRUE : AL_FALSE ));
                    this->Valid = this->CheckError();
                }
            }

            CompressorParameters Effect::GetCompressorParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_COMPRESSOR ) {
                    CompressorParameters Ret;

                    ALint BoolParam;
                    this->EFX->alGetEffecti(this->InternalEffect,AL_COMPRESSOR_ONOFF,&BoolParam);
                    Ret.Active = static_cast<Boole>(BoolParam);

                    return Ret;
                }else{
                    return CompressorParameters();
                }
            }

            void Effect::SetEqualizerParameters(const EqualizerParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_EQUALIZER ) {
                    this->EFX->alEffectf(this->InternalEffect,AL_EQUALIZER_LOW_GAIN,Param.LowGain);
                    this->EFX->alEffectf(this->InternalEffect,AL_EQUALIZER_LOW_CUTOFF,Param.LowCutoff);
                    this->EFX->alEffectf(this->InternalEffect,AL_EQUALIZER_MID1_GAIN,Param.Mid1Gain);
                    this->EFX->alEffectf(this->InternalEffect,AL_EQUALIZER_MID1_CENTER,Param.Mid1Center);
                    this->EFX->alEffectf(this->InternalEffect,AL_EQUALIZER_MID1_WIDTH,Param.Mid1Width);
                    this->EFX->alEffectf(this->InternalEffect,AL_EQUALIZER_MID2_GAIN,Param.Mid2Gain);
                    this->EFX->alEffectf(this->InternalEffect,AL_EQUALIZER_MID2_CENTER,Param.Mid2Center);
                    this->EFX->alEffectf(this->InternalEffect,AL_EQUALIZER_MID2_WIDTH,Param.Mid2Width);
                    this->EFX->alEffectf(this->InternalEffect,AL_EQUALIZER_HIGH_GAIN,Param.HighGain);
                    this->EFX->alEffectf(this->InternalEffect,AL_EQUALIZER_HIGH_CUTOFF,Param.HighCutoff);
                    this->Valid = this->CheckError();
                }
            }

            EqualizerParameters Effect::GetEqualizerParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_EQUALIZER ) {
                    EqualizerParameters Ret;

                    this->EFX->alGetEffectf(this->InternalEffect,AL_EQUALIZER_LOW_GAIN,&Ret.LowGain);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EQUALIZER_LOW_CUTOFF,&Ret.LowCutoff);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EQUALIZER_MID1_GAIN,&Ret.Mid1Gain);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EQUALIZER_MID1_CENTER,&Ret.Mid1Center);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EQUALIZER_MID1_WIDTH,&Ret.Mid1Width);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EQUALIZER_MID2_GAIN,&Ret.Mid2Gain);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EQUALIZER_MID2_CENTER,&Ret.Mid2Center);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EQUALIZER_MID2_WIDTH,&Ret.Mid2Width);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EQUALIZER_HIGH_GAIN,&Ret.HighGain);
                    this->EFX->alGetEffectf(this->InternalEffect,AL_EQUALIZER_HIGH_CUTOFF,&Ret.HighCutoff);
                    return Ret;
                }else{
                    return EqualizerParameters();
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            UInt32 Effect::_GetInternalEffect() const
            {
                return this->InternalEffect;
            }

            UInt32 Effect::_GetInternalEffectSlot() const
            {
                return this->InternalEffectSlot;
            }

            Boole Effect::_IsDirty() const
            {
                return (this->Dirty || this->EffectFilter->_IsDirty());
            }

            void Effect::_Clean()
            {
                this->Dirty = false;
            }
        }//OALS
    }//Audio
}//Mezzanine

#endif
