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
#ifndef _audiooalseffect_cpp
#define _audiooalseffect_cpp

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#define OALS_STRUCTS_DECLARED

#include "Audio/OALS/oalseffect.h"
#include "Audio/OALS/oalsfilter.h"

#include "Audio/OALS/oalsefxinterface.h.cpp"
#include "Audio/OALS/oalseffectshandler.h"

#include "exception.h"
#include "serialization.h"

#include <AL/efx.h>
#include <AL/efx-creative.h>
#include <AL/efx-presets.h>

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            /// @internal
            /// @brief Converts the Mezzanine EffectType to the internal type.
            /// @param Type The Mezzanine type to be converted.
            /// @return Returns the internal type corresponding to the provided Mezzanine type.
            ALenum ConvertMezzanineEffectType(const Audio::EffectType Type)
            {
                switch(Type)
                {
                    case Audio::ET_Null:                return AL_EFFECT_NULL;               break;
                    case Audio::ET_EAX_Reverb:          return AL_EFFECT_EAXREVERB;          break;
                    case Audio::ET_Reverb:              return AL_EFFECT_REVERB;             break;
                    case Audio::ET_Chorus:              return AL_EFFECT_CHORUS;             break;
                    case Audio::ET_Distortion:          return AL_EFFECT_DISTORTION;         break;
                    case Audio::ET_Echo:                return AL_EFFECT_ECHO;               break;
                    case Audio::ET_Flanger:             return AL_EFFECT_FLANGER;            break;
                    case Audio::ET_Frequency_Shifter:   return AL_EFFECT_FREQUENCY_SHIFTER;  break;
                    case Audio::ET_Vocal_Morpher:       return AL_EFFECT_VOCAL_MORPHER;      break;
                    case Audio::ET_Pitch_Shifter:       return AL_EFFECT_PITCH_SHIFTER;      break;
                    case Audio::ET_Ring_Modulator:      return AL_EFFECT_RING_MODULATOR;     break;
                    case Audio::ET_Autowah:             return AL_EFFECT_AUTOWAH;            break;
                    case Audio::ET_Compressor:          return AL_EFFECT_COMPRESSOR;         break;
                    case Audio::ET_Equalizer:           return AL_EFFECT_EQUALIZER;          break;
                    default:                            return AL_EFFECT_NULL;               break;
                }
            }
            /// @internal
            /// @brief Converts the internal type to the Mezzanine EffectType.
            /// @param Type The internal type to be converted.
            /// @return Returns the Mezzanine type corresponding to the provided internal type.
            Audio::EffectType ConvertInternalEffectType(const ALenum Type)
            {
                switch(Type)
                {
                    case AL_EFFECT_NULL:                return Audio::ET_Null;               break;
                    case AL_EFFECT_EAXREVERB:           return Audio::ET_EAX_Reverb;         break;
                    case AL_EFFECT_REVERB:              return Audio::ET_Reverb;             break;
                    case AL_EFFECT_CHORUS:              return Audio::ET_Chorus;             break;
                    case AL_EFFECT_DISTORTION:          return Audio::ET_Distortion;         break;
                    case AL_EFFECT_ECHO:                return Audio::ET_Echo;               break;
                    case AL_EFFECT_FLANGER:             return Audio::ET_Flanger;            break;
                    case AL_EFFECT_FREQUENCY_SHIFTER:   return Audio::ET_Frequency_Shifter;  break;
                    case AL_EFFECT_VOCAL_MORPHER:       return Audio::ET_Vocal_Morpher;      break;
                    case AL_EFFECT_PITCH_SHIFTER:       return Audio::ET_Pitch_Shifter;      break;
                    case AL_EFFECT_RING_MODULATOR:      return Audio::ET_Ring_Modulator;     break;
                    case AL_EFFECT_AUTOWAH:             return Audio::ET_Autowah;            break;
                    case AL_EFFECT_COMPRESSOR:          return Audio::ET_Compressor;         break;
                    case AL_EFFECT_EQUALIZER:           return Audio::ET_Equalizer;          break;
                    default:                            return Audio::ET_Null;               break;
                }
            }

            /// @internal
            /// @brief Converts the Mezzanine Modulator Waveform Type to the internal type.
            /// @param Type The Mezzanine type to be converted.
            /// @return Returns the internal type corresponding to the provided Mezzanine type.
            ALenum ConvertMezzanineModulatorWaveformType(const RingModulatorParameters::ModulatorWaveform Type)
            {
                switch(Type)
                {
                    case RingModulatorParameters::EMW_Sinusoid:  return AL_RING_MODULATOR_SINUSOID;          break;
                    case RingModulatorParameters::EMW_Saw:       return AL_RING_MODULATOR_SAWTOOTH;          break;
                    case RingModulatorParameters::EMW_Square:    return AL_RING_MODULATOR_SQUARE;            break;
                    default:                                     return AL_RING_MODULATOR_DEFAULT_WAVEFORM;  break;
                }
            }
            /// @internal
            /// @brief Converts the internal type to the Mezzanine Modulator Waveform Type.
            /// @param Type The internal type to be converted.
            /// @return Returns the Mezzanine type corresponding to the provided internal type.
            RingModulatorParameters::ModulatorWaveform ConvertInternalModulatorWaveformType(const ALenum Type)
            {
                switch(Type)
                {
                    case AL_RING_MODULATOR_SINUSOID:  return RingModulatorParameters::EMW_Sinusoid;  break;
                    case AL_RING_MODULATOR_SAWTOOTH:  return RingModulatorParameters::EMW_Saw;       break;
                    case AL_RING_MODULATOR_SQUARE:    return RingModulatorParameters::EMW_Square;    break;
                }
                return RingModulatorParameters::EMW_Sinusoid;
            }

            /// @internal
            /// @brief Converts the Mezzanine Chorus Waveform Type to the internal type.
            /// @param Type The Mezzanine type to be converted.
            /// @return Returns the internal type corresponding to the provided Mezzanine type.
            ALenum ConvertMezzanineChorusWaveformType(const ChorusParameters::ChorusWaveform Type)
            {
                switch(Type)
                {
                    case ChorusParameters::ECW_Sinusoid:  return AL_CHORUS_WAVEFORM_SINUSOID;   break;
                    case ChorusParameters::ECW_Triangle:  return AL_CHORUS_WAVEFORM_TRIANGLE;   break;
                    default:                              return AL_CHORUS_DEFAULT_WAVEFORM;    break;
                }
            }
            /// @internal
            /// @brief Converts the internal type to the Mezzanine Chorus Waveform Type.
            /// @param Type The internal type to be converted.
            /// @return Returns the Mezzanine type corresponding to the provided internal type.
            ChorusParameters::ChorusWaveform ConvertInternalChorusWaveformType(const ALenum Type)
            {
                switch(Type)
                {
                    case AL_CHORUS_WAVEFORM_SINUSOID:  return ChorusParameters::ECW_Sinusoid;  break;
                    case AL_CHORUS_WAVEFORM_TRIANGLE:  return ChorusParameters::ECW_Triangle;  break;
                }
                return ChorusParameters::ECW_Sinusoid;
            }

            /// @internal
            /// @brief Converts the Mezzanine Flanger Waveform Type to the internal type.
            /// @param Type The Mezzanine type to be converted.
            /// @return Returns the internal type corresponding to the provided Mezzanine type.
            ALenum ConvertMezzanineFlangerWaveformType(const FlangerParameters::FlangerWaveform Type)
            {
                switch(Type)
                {
                    case FlangerParameters::EFW_Sinusoid:  return AL_FLANGER_WAVEFORM_SINUSOID;   break;
                    case FlangerParameters::EFW_Triangle:  return AL_FLANGER_WAVEFORM_TRIANGLE;   break;
                    default:                               return AL_FLANGER_DEFAULT_WAVEFORM;    break;
                }
            }
            /// @internal
            /// @brief Converts the internal type to the Mezzanine Flanger Waveform Type.
            /// @param Type The internal type to be converted.
            /// @return Returns the Mezzanine type corresponding to the provided internal type.
            FlangerParameters::FlangerWaveform ConvertInternalFlangerWaveformType(const ALenum Type)
            {
                switch(Type)
                {
                    case AL_FLANGER_WAVEFORM_SINUSOID:  return FlangerParameters::EFW_Sinusoid;  break;
                    case AL_FLANGER_WAVEFORM_TRIANGLE:  return FlangerParameters::EFW_Triangle;  break;
                }
                return FlangerParameters::EFW_Sinusoid;
            }

            /// @internal
            /// @brief Converts the Mezzanine Shift Direction Type to the internal type.
            /// @param Type The Mezzanine type to be converted.
            /// @return Returns the internal type corresponding to the provided Mezzanine type.
            ALenum ConvertMezzanineShiftDirectionType(const FrequencyShiftParameters::ShiftDirection Type)
            {
                switch(Type)
                {
                    case FrequencyShiftParameters::ESD_Down:  return AL_FREQUENCY_SHIFTER_DIRECTION_DOWN;   break;
                    case FrequencyShiftParameters::ESD_Up:    return AL_FREQUENCY_SHIFTER_DIRECTION_UP;     break;
                    case FrequencyShiftParameters::ESD_Off:   return AL_FREQUENCY_SHIFTER_DIRECTION_OFF;    break;
                }
                return AL_FREQUENCY_SHIFTER_DIRECTION_OFF;
            }
            /// @internal
            /// @brief Converts the internal type to the Mezzanine Shift Direction Type.
            /// @param Type The internal type to be converted.
            /// @return Returns the Mezzanine type corresponding to the provided internal type.
            FrequencyShiftParameters::ShiftDirection ConvertInternalShiftDirectionType(const ALenum Type)
            {
                switch(Type)
                {
                    case AL_FREQUENCY_SHIFTER_DIRECTION_DOWN:  return FrequencyShiftParameters::ESD_Down;  break;
                    case AL_FREQUENCY_SHIFTER_DIRECTION_UP:    return FrequencyShiftParameters::ESD_Up;    break;
                    case AL_FREQUENCY_SHIFTER_DIRECTION_OFF:   return FrequencyShiftParameters::ESD_Off;   break;
                }
                return FrequencyShiftParameters::ESD_Off;
            }

            /// @internal
            /// @brief Converts the Mezzanine Morpher Waveform Type to the internal type.
            /// @param Type The Mezzanine type to be converted.
            /// @return Returns the internal type corresponding to the provided Mezzanine type.
            ALenum ConvertMezzanineMorpherWaveformType(const VocalMorpherParameters::MorpherWaveform Type)
            {
                switch(Type)
                {
                    case VocalMorpherParameters::EMW_Sinusoid:  return AL_VOCAL_MORPHER_WAVEFORM_SINUSOID;   break;
                    case VocalMorpherParameters::EMW_Triangle:  return AL_VOCAL_MORPHER_WAVEFORM_TRIANGLE;   break;
                    case VocalMorpherParameters::EMW_Saw:       return AL_VOCAL_MORPHER_WAVEFORM_SAWTOOTH;   break;
                    default:                                    return AL_VOCAL_MORPHER_DEFAULT_WAVEFORM;    break;
                }
            }
            /// @internal
            /// @brief Converts the internal type to the Mezzanine Morpher Waveform Type.
            /// @param Type The internal type to be converted.
            /// @return Returns the Mezzanine type corresponding to the provided internal type.
            VocalMorpherParameters::MorpherWaveform ConvertInternalMorpherWaveformType(const ALenum Type)
            {
                switch(Type)
                {
                    case AL_VOCAL_MORPHER_WAVEFORM_SINUSOID:  return VocalMorpherParameters::EMW_Sinusoid;  break;
                    case AL_VOCAL_MORPHER_WAVEFORM_TRIANGLE:  return VocalMorpherParameters::EMW_Triangle;  break;
                    case AL_VOCAL_MORPHER_WAVEFORM_SAWTOOTH:  return VocalMorpherParameters::EMW_Saw;       break;
                }
                return VocalMorpherParameters::EMW_Sinusoid;
            }

            /// @internal
            /// @brief Converts the Mezzanine Morpher Phoneme Type to the internal type.
            /// @param Type The Mezzanine type to be converted.
            /// @return Returns the internal type corresponding to the provided Mezzanine type.
            ALenum ConvertMezzanineMorpherPhonemeType(const VocalMorpherParameters::MorpherPhoneme Type)
            {
                switch(Type)
                {
                    case VocalMorpherParameters::EMP_A:   return AL_VOCAL_MORPHER_PHONEME_A;   break;
                    case VocalMorpherParameters::EMP_E:   return AL_VOCAL_MORPHER_PHONEME_E;   break;
                    case VocalMorpherParameters::EMP_I:   return AL_VOCAL_MORPHER_PHONEME_I;   break;
                    case VocalMorpherParameters::EMP_O:   return AL_VOCAL_MORPHER_PHONEME_O;   break;
                    case VocalMorpherParameters::EMP_U:   return AL_VOCAL_MORPHER_PHONEME_U;   break;
                    case VocalMorpherParameters::EMP_AA:  return AL_VOCAL_MORPHER_PHONEME_AA;  break;
                    case VocalMorpherParameters::EMP_AE:  return AL_VOCAL_MORPHER_PHONEME_AE;  break;
                    case VocalMorpherParameters::EMP_AH:  return AL_VOCAL_MORPHER_PHONEME_AH;  break;
                    case VocalMorpherParameters::EMP_AO:  return AL_VOCAL_MORPHER_PHONEME_AO;  break;
                    case VocalMorpherParameters::EMP_EH:  return AL_VOCAL_MORPHER_PHONEME_EH;  break;
                    case VocalMorpherParameters::EMP_ER:  return AL_VOCAL_MORPHER_PHONEME_ER;  break;
                    case VocalMorpherParameters::EMP_IH:  return AL_VOCAL_MORPHER_PHONEME_IH;  break;
                    case VocalMorpherParameters::EMP_IY:  return AL_VOCAL_MORPHER_PHONEME_IY;  break;
                    case VocalMorpherParameters::EMP_UH:  return AL_VOCAL_MORPHER_PHONEME_UH;  break;
                    case VocalMorpherParameters::EMP_UW:  return AL_VOCAL_MORPHER_PHONEME_UW;  break;
                    case VocalMorpherParameters::EMP_B:   return AL_VOCAL_MORPHER_PHONEME_B;   break;
                    case VocalMorpherParameters::EMP_D:   return AL_VOCAL_MORPHER_PHONEME_D;   break;
                    case VocalMorpherParameters::EMP_F:   return AL_VOCAL_MORPHER_PHONEME_F;   break;
                    case VocalMorpherParameters::EMP_G:   return AL_VOCAL_MORPHER_PHONEME_G;   break;
                    case VocalMorpherParameters::EMP_J:   return AL_VOCAL_MORPHER_PHONEME_J;   break;
                    case VocalMorpherParameters::EMP_K:   return AL_VOCAL_MORPHER_PHONEME_K;   break;
                    case VocalMorpherParameters::EMP_L:   return AL_VOCAL_MORPHER_PHONEME_L;   break;
                    case VocalMorpherParameters::EMP_M:   return AL_VOCAL_MORPHER_PHONEME_M;   break;
                    case VocalMorpherParameters::EMP_N:   return AL_VOCAL_MORPHER_PHONEME_N;   break;
                    case VocalMorpherParameters::EMP_P:   return AL_VOCAL_MORPHER_PHONEME_P;   break;
                    case VocalMorpherParameters::EMP_R:   return AL_VOCAL_MORPHER_PHONEME_R;   break;
                    case VocalMorpherParameters::EMP_S:   return AL_VOCAL_MORPHER_PHONEME_S;   break;
                    case VocalMorpherParameters::EMP_T:   return AL_VOCAL_MORPHER_PHONEME_T;   break;
                    case VocalMorpherParameters::EMP_V:   return AL_VOCAL_MORPHER_PHONEME_V;   break;
                    case VocalMorpherParameters::EMP_Z:   return AL_VOCAL_MORPHER_PHONEME_Z;   break;
                }
                return AL_VOCAL_MORPHER_PHONEME_A;
            }
            /// @internal
            /// @brief Converts the internal type to the Mezzanine Morpher Phoneme Type.
            /// @param Type The internal type to be converted.
            /// @return Returns the Mezzanine type corresponding to the provided internal type.
            VocalMorpherParameters::MorpherPhoneme ConvertInternalMorpherPhonemeType(const ALenum Type)
            {
                switch(Type)
                {
                    case AL_VOCAL_MORPHER_PHONEME_A:   return VocalMorpherParameters::EMP_A;   break;
                    case AL_VOCAL_MORPHER_PHONEME_E:   return VocalMorpherParameters::EMP_E;   break;
                    case AL_VOCAL_MORPHER_PHONEME_I:   return VocalMorpherParameters::EMP_I;   break;
                    case AL_VOCAL_MORPHER_PHONEME_O:   return VocalMorpherParameters::EMP_O;   break;
                    case AL_VOCAL_MORPHER_PHONEME_U:   return VocalMorpherParameters::EMP_U;   break;
                    case AL_VOCAL_MORPHER_PHONEME_AA:  return VocalMorpherParameters::EMP_AA;  break;
                    case AL_VOCAL_MORPHER_PHONEME_AE:  return VocalMorpherParameters::EMP_AE;  break;
                    case AL_VOCAL_MORPHER_PHONEME_AH:  return VocalMorpherParameters::EMP_AH;  break;
                    case AL_VOCAL_MORPHER_PHONEME_AO:  return VocalMorpherParameters::EMP_AO;  break;
                    case AL_VOCAL_MORPHER_PHONEME_EH:  return VocalMorpherParameters::EMP_EH;  break;
                    case AL_VOCAL_MORPHER_PHONEME_ER:  return VocalMorpherParameters::EMP_ER;  break;
                    case AL_VOCAL_MORPHER_PHONEME_IH:  return VocalMorpherParameters::EMP_IH;  break;
                    case AL_VOCAL_MORPHER_PHONEME_IY:  return VocalMorpherParameters::EMP_IY;  break;
                    case AL_VOCAL_MORPHER_PHONEME_UH:  return VocalMorpherParameters::EMP_UH;  break;
                    case AL_VOCAL_MORPHER_PHONEME_UW:  return VocalMorpherParameters::EMP_UW;  break;
                    case AL_VOCAL_MORPHER_PHONEME_B:   return VocalMorpherParameters::EMP_B;   break;
                    case AL_VOCAL_MORPHER_PHONEME_D:   return VocalMorpherParameters::EMP_D;   break;
                    case AL_VOCAL_MORPHER_PHONEME_F:   return VocalMorpherParameters::EMP_F;   break;
                    case AL_VOCAL_MORPHER_PHONEME_G:   return VocalMorpherParameters::EMP_G;   break;
                    case AL_VOCAL_MORPHER_PHONEME_J:   return VocalMorpherParameters::EMP_J;   break;
                    case AL_VOCAL_MORPHER_PHONEME_K:   return VocalMorpherParameters::EMP_K;   break;
                    case AL_VOCAL_MORPHER_PHONEME_L:   return VocalMorpherParameters::EMP_L;   break;
                    case AL_VOCAL_MORPHER_PHONEME_M:   return VocalMorpherParameters::EMP_M;   break;
                    case AL_VOCAL_MORPHER_PHONEME_N:   return VocalMorpherParameters::EMP_N;   break;
                    case AL_VOCAL_MORPHER_PHONEME_P:   return VocalMorpherParameters::EMP_P;   break;
                    case AL_VOCAL_MORPHER_PHONEME_R:   return VocalMorpherParameters::EMP_R;   break;
                    case AL_VOCAL_MORPHER_PHONEME_S:   return VocalMorpherParameters::EMP_S;   break;
                    case AL_VOCAL_MORPHER_PHONEME_T:   return VocalMorpherParameters::EMP_T;   break;
                    case AL_VOCAL_MORPHER_PHONEME_V:   return VocalMorpherParameters::EMP_V;   break;
                    case AL_VOCAL_MORPHER_PHONEME_Z:   return VocalMorpherParameters::EMP_Z;   break;
                }
                return VocalMorpherParameters::EMP_A;
            }

            /// @internal
            /// @brief Sets every EAX Reverb parameter to the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @param Params The parameters to set.
            void ApplyEAXReverbParameters(EFXInterface* EFX, const UInt32 EffectID, const EAXReverbParameters& Params)
            {
                EFX->alEffectf(EffectID,AL_EAXREVERB_DENSITY,Params.Density);
                EFX->alEffectf(EffectID,AL_EAXREVERB_DIFFUSION,Params.Diffusion);
                EFX->alEffectf(EffectID,AL_EAXREVERB_GAIN,Params.Gain);
                EFX->alEffectf(EffectID,AL_EAXREVERB_GAINHF,Params.GainHF);
                EFX->alEffectf(EffectID,AL_EAXREVERB_GAINLF,Params.GainLF);
                EFX->alEffectf(EffectID,AL_EAXREVERB_DECAY_TIME,Params.DecayTime);
                EFX->alEffectf(EffectID,AL_EAXREVERB_DECAY_HFRATIO,Params.DecayHFRatio);
                EFX->alEffectf(EffectID,AL_EAXREVERB_DECAY_LFRATIO,Params.DecayLFRatio);
                EFX->alEffectf(EffectID,AL_EAXREVERB_REFLECTIONS_GAIN,Params.ReflectionsGain);
                EFX->alEffectf(EffectID,AL_EAXREVERB_REFLECTIONS_DELAY,Params.ReflectionsDelay);
                EFX->alEffectfv(EffectID,AL_EAXREVERB_REFLECTIONS_PAN,&Params.ReflectionsPan.X);
                EFX->alEffectf(EffectID,AL_EAXREVERB_LATE_REVERB_GAIN,Params.LateReverbGain);
                EFX->alEffectf(EffectID,AL_EAXREVERB_LATE_REVERB_DELAY,Params.LateReverbDelay);
                EFX->alEffectfv(EffectID,AL_EAXREVERB_LATE_REVERB_PAN,&Params.LateReverbPan.X);
                EFX->alEffectf(EffectID,AL_EAXREVERB_ECHO_TIME,Params.EchoTime);
                EFX->alEffectf(EffectID,AL_EAXREVERB_ECHO_DEPTH,Params.EchoDepth);
                EFX->alEffectf(EffectID,AL_EAXREVERB_MODULATION_TIME,Params.ModulationTime);
                EFX->alEffectf(EffectID,AL_EAXREVERB_MODULATION_DEPTH,Params.ModulationDepth);
                EFX->alEffectf(EffectID,AL_EAXREVERB_HFREFERENCE,Params.HFReference);
                EFX->alEffectf(EffectID,AL_EAXREVERB_LFREFERENCE,Params.LFReference);
                EFX->alEffectf(EffectID,AL_EAXREVERB_ROOM_ROLLOFF_FACTOR,Params.RoomRolloffFactor);
                EFX->alEffectf(EffectID,AL_EAXREVERB_AIR_ABSORPTION_GAINHF,Params.AirAbsorptionGainHF);
                EFX->alEffecti(EffectID,AL_EAXREVERB_DECAY_HFLIMIT,(Params.DecayHFLimit ? AL_TRUE : AL_FALSE));
            }

            /// @internal
            /// @brief Gets every EAX Reverb parameter of the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @return Returns a EAXReverbParameters with the currently set parameters of the effect.
            EAXReverbParameters AquireEAXReverbParameters(EFXInterface* EFX, const UInt32 EffectID)
            {
                EAXReverbParameters Ret(0,0,0,0,0,0,0,0,0,0,Vector3(0,0,0),0,0,Vector3(0,0,0),0,0,0,0,0,0,0,false);

                EFX->alGetEffectf(EffectID,AL_EAXREVERB_DENSITY,&Ret.Density);
                EFX->alGetEffectf(EffectID,AL_EAXREVERB_DIFFUSION,&Ret.Diffusion);
                EFX->alGetEffectf(EffectID,AL_EAXREVERB_GAIN,&Ret.Gain);
                EFX->alGetEffectf(EffectID,AL_EAXREVERB_GAINHF,&Ret.GainHF);
                EFX->alGetEffectf(EffectID,AL_EAXREVERB_GAINLF,&Ret.GainLF);
                EFX->alGetEffectf(EffectID,AL_EAXREVERB_DECAY_TIME,&Ret.DecayTime);
                EFX->alGetEffectf(EffectID,AL_EAXREVERB_DECAY_HFRATIO,&Ret.DecayHFRatio);
                EFX->alGetEffectf(EffectID,AL_EAXREVERB_DECAY_LFRATIO,&Ret.DecayLFRatio);
                EFX->alGetEffectf(EffectID,AL_EAXREVERB_REFLECTIONS_GAIN,&Ret.ReflectionsGain);
                EFX->alGetEffectf(EffectID,AL_EAXREVERB_REFLECTIONS_DELAY,&Ret.ReflectionsDelay);
                EFX->alGetEffectfv(EffectID,AL_EAXREVERB_REFLECTIONS_PAN,&Ret.ReflectionsPan.X);
                EFX->alGetEffectf(EffectID,AL_EAXREVERB_LATE_REVERB_GAIN,&Ret.LateReverbGain);
                EFX->alGetEffectf(EffectID,AL_EAXREVERB_LATE_REVERB_DELAY,&Ret.LateReverbDelay);
                EFX->alGetEffectfv(EffectID,AL_EAXREVERB_LATE_REVERB_PAN,&Ret.LateReverbPan.X);
                EFX->alGetEffectf(EffectID,AL_EAXREVERB_ECHO_TIME,&Ret.EchoTime);
                EFX->alGetEffectf(EffectID,AL_EAXREVERB_ECHO_DEPTH,&Ret.EchoDepth);
                EFX->alGetEffectf(EffectID,AL_EAXREVERB_MODULATION_TIME,&Ret.ModulationTime);
                EFX->alGetEffectf(EffectID,AL_EAXREVERB_MODULATION_DEPTH,&Ret.ModulationDepth);
                EFX->alGetEffectf(EffectID,AL_EAXREVERB_HFREFERENCE,&Ret.HFReference);
                EFX->alGetEffectf(EffectID,AL_EAXREVERB_LFREFERENCE,&Ret.LFReference);
                EFX->alGetEffectf(EffectID,AL_EAXREVERB_ROOM_ROLLOFF_FACTOR,&Ret.RoomRolloffFactor);
                EFX->alGetEffectf(EffectID,AL_EAXREVERB_AIR_ABSORPTION_GAINHF,&Ret.AirAbsorptionGainHF);

                ALint BoolParam;
                EFX->alGetEffecti(EffectID,AL_EAXREVERB_DECAY_HFLIMIT,&BoolParam);
                Ret.DecayHFLimit = static_cast<Boole>(BoolParam);

                return Ret;
            }

            /// @internal
            /// @brief Sets every Reverb parameter to the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @param Params The parameters to set.
            void ApplyReverbParameters(EFXInterface* EFX, const UInt32 EffectID, const ReverbParameters& Params)
            {
                EFX->alEffectf(EffectID,AL_REVERB_DENSITY,Params.Density);
                EFX->alEffectf(EffectID,AL_REVERB_DIFFUSION,Params.Diffusion);
                EFX->alEffectf(EffectID,AL_REVERB_GAIN,Params.Gain);
                EFX->alEffectf(EffectID,AL_REVERB_GAINHF,Params.GainHF);
                EFX->alEffectf(EffectID,AL_REVERB_DECAY_TIME,Params.DecayTime);
                EFX->alEffectf(EffectID,AL_REVERB_DECAY_HFRATIO,Params.DecayHFRatio);
                EFX->alEffectf(EffectID,AL_REVERB_REFLECTIONS_GAIN,Params.ReflectionsGain);
                EFX->alEffectf(EffectID,AL_REVERB_REFLECTIONS_DELAY,Params.ReflectionsDelay);
                EFX->alEffectf(EffectID,AL_REVERB_LATE_REVERB_GAIN,Params.LateReverbGain);
                EFX->alEffectf(EffectID,AL_REVERB_LATE_REVERB_DELAY,Params.LateReverbDelay);
                EFX->alEffectf(EffectID,AL_REVERB_ROOM_ROLLOFF_FACTOR,Params.RoomRolloffFactor);
                EFX->alEffectf(EffectID,AL_REVERB_AIR_ABSORPTION_GAINHF,Params.AirAbsorptionGainHF);
                EFX->alEffecti(EffectID,AL_REVERB_DECAY_HFLIMIT,(Params.DecayHFLimit ? AL_TRUE : AL_FALSE));
            }

            /// @internal
            /// @brief Gets every Reverb parameter of the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @return Returns a ReverbParameters with the currently set parameters of the effect.
            ReverbParameters AquireReverbParameters(EFXInterface* EFX, const UInt32 EffectID)
            {
                ReverbParameters Ret(0,0,0,0,0,0,0,0,0,0,0,0,false);

                EFX->alGetEffectf(EffectID,AL_REVERB_DENSITY,&Ret.Density);
                EFX->alGetEffectf(EffectID,AL_REVERB_DIFFUSION,&Ret.Diffusion);
                EFX->alGetEffectf(EffectID,AL_REVERB_GAIN,&Ret.Gain);
                EFX->alGetEffectf(EffectID,AL_REVERB_GAINHF,&Ret.GainHF);
                EFX->alGetEffectf(EffectID,AL_REVERB_DECAY_TIME,&Ret.DecayTime);
                EFX->alGetEffectf(EffectID,AL_REVERB_DECAY_HFRATIO,&Ret.DecayHFRatio);
                EFX->alGetEffectf(EffectID,AL_REVERB_REFLECTIONS_GAIN,&Ret.ReflectionsGain);
                EFX->alGetEffectf(EffectID,AL_REVERB_REFLECTIONS_DELAY,&Ret.ReflectionsDelay);
                EFX->alGetEffectf(EffectID,AL_REVERB_LATE_REVERB_GAIN,&Ret.LateReverbGain);
                EFX->alGetEffectf(EffectID,AL_REVERB_LATE_REVERB_DELAY,&Ret.LateReverbDelay);
                EFX->alGetEffectf(EffectID,AL_REVERB_ROOM_ROLLOFF_FACTOR,&Ret.RoomRolloffFactor);
                EFX->alGetEffectf(EffectID,AL_REVERB_AIR_ABSORPTION_GAINHF,&Ret.AirAbsorptionGainHF);

                ALint BoolParam;
                EFX->alGetEffecti(EffectID,AL_REVERB_DECAY_HFLIMIT,&BoolParam);
                Ret.DecayHFLimit = static_cast<Boole>(BoolParam);

                return Ret;
            }

            /// @internal
            /// @brief Sets every Chorus parameter to the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @param Params The parameters to set.
            void ApplyChorusParameters(EFXInterface* EFX, const UInt32 EffectID, const ChorusParameters& Params)
            {
                EFX->alEffecti(EffectID,AL_CHORUS_WAVEFORM,ConvertMezzanineChorusWaveformType(Params.Waveform));
                EFX->alEffecti(EffectID,AL_CHORUS_PHASE,Params.Phase);
                EFX->alEffectf(EffectID,AL_CHORUS_RATE,Params.Rate);
                EFX->alEffectf(EffectID,AL_CHORUS_DEPTH,Params.Depth);
                EFX->alEffectf(EffectID,AL_CHORUS_FEEDBACK,Params.Feedback);
                EFX->alEffectf(EffectID,AL_CHORUS_DELAY,Params.Delay);
            }

            /// @internal
            /// @brief Gets every Chorus parameter of the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @return Returns a ChorusParameters with the currently set parameters of the effect.
            ChorusParameters AquireChorusParameters(EFXInterface* EFX, const UInt32 EffectID)
            {
                ChorusParameters Ret(ChorusParameters::ECW_Triangle,0,0,0,0,0);

                ALenum WaveformType;
                EFX->alGetEffecti(EffectID,AL_CHORUS_WAVEFORM,&WaveformType);
                Ret.Waveform = ConvertInternalChorusWaveformType(WaveformType);

                EFX->alGetEffecti(EffectID,AL_CHORUS_PHASE,&Ret.Phase);
                EFX->alGetEffectf(EffectID,AL_CHORUS_RATE,&Ret.Rate);
                EFX->alGetEffectf(EffectID,AL_CHORUS_DEPTH,&Ret.Depth);
                EFX->alGetEffectf(EffectID,AL_CHORUS_FEEDBACK,&Ret.Feedback);
                EFX->alGetEffectf(EffectID,AL_CHORUS_DELAY,&Ret.Delay);

                return Ret;
            }

            /// @internal
            /// @brief Sets every Distortion parameter to the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @param Params The parameters to set.
            void ApplyDistortionParameters(EFXInterface* EFX, const UInt32 EffectID, const DistortionParameters& Params)
            {
                EFX->alEffectf(EffectID,AL_DISTORTION_EDGE,Params.Edge);
                EFX->alEffectf(EffectID,AL_DISTORTION_GAIN,Params.Gain);
                EFX->alEffectf(EffectID,AL_DISTORTION_LOWPASS_CUTOFF,Params.LowpassCutoff);
                EFX->alEffectf(EffectID,AL_DISTORTION_EQCENTER,Params.EqCenter);
                EFX->alEffectf(EffectID,AL_DISTORTION_EQBANDWIDTH,Params.EqBandwidth);
            }

            /// @internal
            /// @brief Gets every Distortion parameter of the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @return Returns a DistortionParameters with the currently set parameters of the effect.
            DistortionParameters AquireDistortionParameters(EFXInterface* EFX, const UInt32 EffectID)
            {
                DistortionParameters Ret(0,0,0,0,0);

                EFX->alGetEffectf(EffectID,AL_DISTORTION_EDGE,&Ret.Edge);
                EFX->alGetEffectf(EffectID,AL_DISTORTION_GAIN,&Ret.Gain);
                EFX->alGetEffectf(EffectID,AL_DISTORTION_LOWPASS_CUTOFF,&Ret.LowpassCutoff);
                EFX->alGetEffectf(EffectID,AL_DISTORTION_EQCENTER,&Ret.EqCenter);
                EFX->alGetEffectf(EffectID,AL_DISTORTION_EQBANDWIDTH,&Ret.EqBandwidth);

                return Ret;
            }

            /// @internal
            /// @brief Sets every Echo parameter to the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @param Params The parameters to set.
            void ApplyEchoParameters(EFXInterface* EFX, const UInt32 EffectID, const EchoParameters& Params)
            {
                EFX->alEffectf(EffectID,AL_ECHO_DELAY,Params.Delay);
                EFX->alEffectf(EffectID,AL_ECHO_LRDELAY,Params.LRDelay);
                EFX->alEffectf(EffectID,AL_ECHO_DAMPING,Params.Damping);
                EFX->alEffectf(EffectID,AL_ECHO_FEEDBACK,Params.Feedback);
                EFX->alEffectf(EffectID,AL_ECHO_SPREAD,Params.Spread);
            }

            /// @internal
            /// @brief Gets every Echo parameter of the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @return Returns a EchoParameters with the currently set parameters of the effect.
            EchoParameters AquireEchoParameters(EFXInterface* EFX, const UInt32 EffectID)
            {
                EchoParameters Ret(0,0,0,0,0);

                EFX->alGetEffectf(EffectID,AL_ECHO_DELAY,&Ret.Delay);
                EFX->alGetEffectf(EffectID,AL_ECHO_LRDELAY,&Ret.LRDelay);
                EFX->alGetEffectf(EffectID,AL_ECHO_DAMPING,&Ret.Damping);
                EFX->alGetEffectf(EffectID,AL_ECHO_FEEDBACK,&Ret.Feedback);
                EFX->alGetEffectf(EffectID,AL_ECHO_SPREAD,&Ret.Spread);

                return Ret;
            }

            /// @internal
            /// @brief Sets every Flanger parameter to the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @param Params The parameters to set.
            void ApplyFlangerParameters(EFXInterface* EFX, const UInt32 EffectID, const FlangerParameters& Params)
            {
                EFX->alEffecti(EffectID,AL_FLANGER_WAVEFORM,ConvertMezzanineFlangerWaveformType(Params.Waveform));
                EFX->alEffecti(EffectID,AL_FLANGER_PHASE,Params.Phase);
                EFX->alEffectf(EffectID,AL_FLANGER_RATE,Params.Rate);
                EFX->alEffectf(EffectID,AL_FLANGER_DEPTH,Params.Depth);
                EFX->alEffectf(EffectID,AL_FLANGER_FEEDBACK,Params.Feedback);
                EFX->alEffectf(EffectID,AL_FLANGER_DELAY,Params.Delay);
            }

            /// @internal
            /// @brief Gets every Flanger parameter of the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @return Returns a FlangerParameters with the currently set parameters of the effect.
            FlangerParameters AquireFlangerParameters(EFXInterface* EFX, const UInt32 EffectID)
            {
                FlangerParameters Ret(FlangerParameters::EFW_Triangle,0,0,0,0,0);

                ALenum WaveformType;
                EFX->alGetEffecti(EffectID,AL_FLANGER_WAVEFORM,&WaveformType);
                Ret.Waveform = ConvertInternalFlangerWaveformType(WaveformType);

                EFX->alGetEffecti(EffectID,AL_FLANGER_PHASE,&Ret.Phase);
                EFX->alGetEffectf(EffectID,AL_FLANGER_RATE,&Ret.Rate);
                EFX->alGetEffectf(EffectID,AL_FLANGER_DEPTH,&Ret.Depth);
                EFX->alGetEffectf(EffectID,AL_FLANGER_FEEDBACK,&Ret.Feedback);
                EFX->alGetEffectf(EffectID,AL_FLANGER_DELAY,&Ret.Delay);

                return Ret;
            }

            /// @internal
            /// @brief Sets every Frequency Shift parameter to the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @param Params The parameters to set.
            void ApplyFrequencyShiftParameters(EFXInterface* EFX, const UInt32 EffectID, const FrequencyShiftParameters& Params)
            {
                EFX->alEffecti(EffectID,AL_FREQUENCY_SHIFTER_LEFT_DIRECTION,ConvertMezzanineShiftDirectionType(Params.Left));
                EFX->alEffecti(EffectID,AL_FREQUENCY_SHIFTER_RIGHT_DIRECTION,ConvertMezzanineShiftDirectionType(Params.Right));
                EFX->alEffectf(EffectID,AL_FREQUENCY_SHIFTER_FREQUENCY,Params.Frequency);
            }

            /// @internal
            /// @brief Gets every Frequency Shift parameter of the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @return Returns a FrequencyShiftParameters with the currently set parameters of the effect.
            FrequencyShiftParameters AquireFrequencyShiftParameters(EFXInterface* EFX, const UInt32 EffectID)
            {
                FrequencyShiftParameters Ret(0,FrequencyShiftParameters::ESD_Down,FrequencyShiftParameters::ESD_Down);

                ALenum LeftWaveformType;
                EFX->alGetEffecti(EffectID,AL_FREQUENCY_SHIFTER_LEFT_DIRECTION,&LeftWaveformType);
                Ret.Left = ConvertInternalShiftDirectionType(LeftWaveformType);

                ALenum RightWaveformType;
                EFX->alGetEffecti(EffectID,AL_FREQUENCY_SHIFTER_RIGHT_DIRECTION,&RightWaveformType);
                Ret.Right = ConvertInternalShiftDirectionType(RightWaveformType);

                EFX->alGetEffectf(EffectID,AL_FREQUENCY_SHIFTER_FREQUENCY,&Ret.Frequency);

                return Ret;
            }

            /// @internal
            /// @brief Sets every Vocal Morpher parameter to the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @param Params The parameters to set.
            void ApplyVocalMorpherParameters(EFXInterface* EFX, const UInt32 EffectID, const VocalMorpherParameters& Params)
            {
                EFX->alEffecti(EffectID,AL_VOCAL_MORPHER_PHONEMEA,ConvertMezzanineMorpherPhonemeType(Params.PhonemeA));
                EFX->alEffecti(EffectID,AL_VOCAL_MORPHER_PHONEMEB,ConvertMezzanineMorpherPhonemeType(Params.PhonemeB));
                EFX->alEffecti(EffectID,AL_VOCAL_MORPHER_WAVEFORM,ConvertMezzanineMorpherWaveformType(Params.Waveform));
                EFX->alEffecti(EffectID,AL_VOCAL_MORPHER_PHONEMEA_COARSE_TUNING,Params.PhonemeACoarseTune);
                EFX->alEffecti(EffectID,AL_VOCAL_MORPHER_PHONEMEB_COARSE_TUNING,Params.PhonemeBCoarseTune);
                EFX->alEffectf(EffectID,AL_VOCAL_MORPHER_RATE,Params.Rate);
            }

            /// @internal
            /// @brief Gets every Vocal Morpher parameter of the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @return Returns a VocalMorpherParameters with the currently set parameters of the effect.
            VocalMorpherParameters AquireVocalMorpherParameters(EFXInterface* EFX, const UInt32 EffectID)
            {
                VocalMorpherParameters Ret(VocalMorpherParameters::EMP_A,
                                           VocalMorpherParameters::EMP_ER,
                                           0,0,
                                           VocalMorpherParameters::EMW_Sinusoid,
                                           0);

                ALenum PhonemeAType;
                EFX->alGetEffecti(EffectID,AL_VOCAL_MORPHER_PHONEMEA,&PhonemeAType);
                Ret.PhonemeA = ConvertInternalMorpherPhonemeType(PhonemeAType);

                ALenum PhonemeBType;
                EFX->alGetEffecti(EffectID,AL_VOCAL_MORPHER_PHONEMEB,&PhonemeBType);
                Ret.PhonemeB = ConvertInternalMorpherPhonemeType(PhonemeBType);

                ALenum WaveformType;
                EFX->alGetEffecti(EffectID,AL_VOCAL_MORPHER_WAVEFORM,&WaveformType);
                Ret.Waveform = ConvertInternalMorpherWaveformType(WaveformType);

                EFX->alGetEffecti(EffectID,AL_VOCAL_MORPHER_PHONEMEA_COARSE_TUNING,&Ret.PhonemeACoarseTune);
                EFX->alGetEffecti(EffectID,AL_VOCAL_MORPHER_PHONEMEB_COARSE_TUNING,&Ret.PhonemeBCoarseTune);
                EFX->alGetEffectf(EffectID,AL_VOCAL_MORPHER_RATE,&Ret.Rate);

                return Ret;
            }

            /// @internal
            /// @brief Sets every Pitch Shifter parameter to the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @param Params The parameters to set.
            void ApplyPitchShifterParameters(EFXInterface* EFX, const UInt32 EffectID, const PitchShifterParameters& Params)
            {
                EFX->alEffecti(EffectID,AL_PITCH_SHIFTER_COARSE_TUNE,Params.CoarseTune);
                EFX->alEffecti(EffectID,AL_PITCH_SHIFTER_FINE_TUNE,Params.FineTune);
            }

            /// @internal
            /// @brief Gets every Pitch Shifter parameter of the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @return Returns a PitchShifterParameters with the currently set parameters of the effect.
            PitchShifterParameters AquirePitchShifterParameters(EFXInterface* EFX, const UInt32 EffectID)
            {
                PitchShifterParameters Ret(0,0);

                EFX->alGetEffecti(EffectID,AL_PITCH_SHIFTER_COARSE_TUNE,&Ret.CoarseTune);
                EFX->alGetEffecti(EffectID,AL_PITCH_SHIFTER_FINE_TUNE,&Ret.FineTune);

                return Ret;
            }

            /// @internal
            /// @brief Sets every Ring Modulator parameter to the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @param Params The parameters to set.
            void ApplyRingModulatorParameters(EFXInterface* EFX, const UInt32 EffectID, const RingModulatorParameters& Params)
            {
                EFX->alEffecti(EffectID,AL_RING_MODULATOR_WAVEFORM,ConvertMezzanineModulatorWaveformType(Params.Waveform));
                EFX->alEffectf(EffectID,AL_RING_MODULATOR_FREQUENCY,Params.Frequency);
                EFX->alEffectf(EffectID,AL_RING_MODULATOR_HIGHPASS_CUTOFF,Params.HighPassCutoff);
            }

            /// @internal
            /// @brief Gets every Ring Modulator parameter of the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @return Returns a RingModulatorParameters with the currently set parameters of the effect.
            RingModulatorParameters AquireRingModulatorParameters(EFXInterface* EFX, const UInt32 EffectID)
            {
                RingModulatorParameters Ret(0,0,RingModulatorParameters::EMW_Sinusoid);

                ALenum WaveformType;
                EFX->alGetEffecti(EffectID,AL_RING_MODULATOR_WAVEFORM,&WaveformType);
                Ret.Waveform = ConvertInternalModulatorWaveformType(WaveformType);

                EFX->alGetEffectf(EffectID,AL_RING_MODULATOR_FREQUENCY,&Ret.Frequency);
                EFX->alGetEffectf(EffectID,AL_RING_MODULATOR_HIGHPASS_CUTOFF,&Ret.HighPassCutoff);

                return Ret;
            }

            /// @internal
            /// @brief Sets every Autowah parameter to the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @param Params The parameters to set.
            void ApplyAutowahParameters(EFXInterface* EFX, const UInt32 EffectID, const AutowahParameters& Params)
            {
                EFX->alEffectf(EffectID,AL_AUTOWAH_ATTACK_TIME,Params.AttackTime);
                EFX->alEffectf(EffectID,AL_AUTOWAH_RELEASE_TIME,Params.ReleaseTime);
                EFX->alEffectf(EffectID,AL_AUTOWAH_RESONANCE,Params.Resonance);
                EFX->alEffectf(EffectID,AL_AUTOWAH_PEAK_GAIN,Params.PeakGain);
            }

            /// @internal
            /// @brief Gets every Autowah parameter of the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @return Returns a AutowahParameters with the currently set parameters of the effect.
            AutowahParameters AquireAutowahParameters(EFXInterface* EFX, const UInt32 EffectID)
            {
                AutowahParameters Ret(0,0,0,0);

                EFX->alGetEffectf(EffectID,AL_AUTOWAH_ATTACK_TIME,&Ret.AttackTime);
                EFX->alGetEffectf(EffectID,AL_AUTOWAH_RELEASE_TIME,&Ret.ReleaseTime);
                EFX->alGetEffectf(EffectID,AL_AUTOWAH_RESONANCE,&Ret.Resonance);
                EFX->alGetEffectf(EffectID,AL_AUTOWAH_PEAK_GAIN,&Ret.PeakGain);

                return Ret;
            }

            /// @internal
            /// @brief Sets every Compressor parameter to the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @param Params The parameters to set.
            void ApplyCompressorParameters(EFXInterface* EFX, const UInt32 EffectID, const CompressorParameters& Params)
            {
                EFX->alEffecti(EffectID,AL_COMPRESSOR_ONOFF,( Params.Active ? AL_TRUE : AL_FALSE ));
            }

            /// @internal
            /// @brief Gets every Compressor parameter of the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @return Returns a CompressorParameters with the currently set parameters of the effect.
            CompressorParameters AquireCompressorParameters(EFXInterface* EFX, const UInt32 EffectID)
            {
                CompressorParameters Ret(false);

                ALint BoolParam;
                EFX->alGetEffecti(EffectID,AL_COMPRESSOR_ONOFF,&BoolParam);
                Ret.Active = static_cast<Boole>(BoolParam);

                return Ret;
            }

            /// @internal
            /// @brief Sets every Equalizer parameter to the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @param Params The parameters to set.
            void ApplyEqualizerParameters(EFXInterface* EFX, const UInt32 EffectID, const EqualizerParameters& Params)
            {
                EFX->alEffectf(EffectID,AL_EQUALIZER_LOW_GAIN,Params.LowGain);
                EFX->alEffectf(EffectID,AL_EQUALIZER_LOW_CUTOFF,Params.LowCutoff);
                EFX->alEffectf(EffectID,AL_EQUALIZER_MID1_GAIN,Params.Mid1Gain);
                EFX->alEffectf(EffectID,AL_EQUALIZER_MID1_CENTER,Params.Mid1Center);
                EFX->alEffectf(EffectID,AL_EQUALIZER_MID1_WIDTH,Params.Mid1Width);
                EFX->alEffectf(EffectID,AL_EQUALIZER_MID2_GAIN,Params.Mid2Gain);
                EFX->alEffectf(EffectID,AL_EQUALIZER_MID2_CENTER,Params.Mid2Center);
                EFX->alEffectf(EffectID,AL_EQUALIZER_MID2_WIDTH,Params.Mid2Width);
                EFX->alEffectf(EffectID,AL_EQUALIZER_HIGH_GAIN,Params.HighGain);
                EFX->alEffectf(EffectID,AL_EQUALIZER_HIGH_CUTOFF,Params.HighCutoff);
            }

            /// @internal
            /// @brief Gets every Equalizer parameter of the internal Effect.
            /// @param EFX A pointer to the interface with the Audio effect control functions.
            /// @param EffectID The ID of the effect being worked on.
            /// @return Returns a EqualizerParameters with the currently set parameters of the effect.
            EqualizerParameters AquireEqualizerParameters(EFXInterface* EFX, const UInt32 EffectID)
            {
                EqualizerParameters Ret(0,0,0,0,0,0,0,0,0,0);

                EFX->alGetEffectf(EffectID,AL_EQUALIZER_LOW_GAIN,&Ret.LowGain);
                EFX->alGetEffectf(EffectID,AL_EQUALIZER_LOW_CUTOFF,&Ret.LowCutoff);
                EFX->alGetEffectf(EffectID,AL_EQUALIZER_MID1_GAIN,&Ret.Mid1Gain);
                EFX->alGetEffectf(EffectID,AL_EQUALIZER_MID1_CENTER,&Ret.Mid1Center);
                EFX->alGetEffectf(EffectID,AL_EQUALIZER_MID1_WIDTH,&Ret.Mid1Width);
                EFX->alGetEffectf(EffectID,AL_EQUALIZER_MID2_GAIN,&Ret.Mid2Gain);
                EFX->alGetEffectf(EffectID,AL_EQUALIZER_MID2_CENTER,&Ret.Mid2Center);
                EFX->alGetEffectf(EffectID,AL_EQUALIZER_MID2_WIDTH,&Ret.Mid2Width);
                EFX->alGetEffectf(EffectID,AL_EQUALIZER_HIGH_GAIN,&Ret.HighGain);
                EFX->alGetEffectf(EffectID,AL_EQUALIZER_HIGH_CUTOFF,&Ret.HighCutoff);

                return Ret;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Effect Methods

            Effect::Effect(OALS::EffectsHandler* EFXCreator) :
                EFX(EFXCreator->_GetEFXInterface()),
                Creator(EFXCreator),
                EffectFilter(NULL),
                Volume(1.0),
                InternalEffect(0),
                InternalEffectSlot(0),
                Dirty(false),
                Valid(false),
                IgnoreAtten(false)
            {
                this->Valid = this->EFX->Supported;

                if( this->EFX->Supported ) {
                    // Create our Effect
                    this->EFX->alGenEffects(1,&InternalEffect);
                    // Create our Effect Slot
                    this->EFX->alGenAuxiliaryEffectSlots(1,&InternalEffectSlot);
                    // Attach the two
                    this->EFX->alAuxiliaryEffectSloti(this->InternalEffectSlot,AL_EFFECTSLOT_EFFECT,this->InternalEffect);
                }
            }

            Effect::Effect(const XML::Node& SelfRoot, OALS::EffectsHandler* EFXCreator) :
                EFX(EFXCreator->_GetEFXInterface()),
                Creator(EFXCreator),
                EffectFilter(NULL),
                Volume(1.0),
                InternalEffect(0),
                InternalEffectSlot(0),
                Dirty(false),
                Valid(false),
                IgnoreAtten(false)
            {
                this->Valid = this->EFX->Supported;

                if( this->EFX->Supported ) {
                    // Create our Effect
                    this->EFX->alGenEffects(1,&InternalEffect);
                    // Create our Effect Slot
                    this->EFX->alGenAuxiliaryEffectSlots(1,&InternalEffectSlot);
                    // Attach the two
                    this->EFX->alAuxiliaryEffectSloti(this->InternalEffectSlot,AL_EFFECTSLOT_EFFECT,this->InternalEffect);
                }

                this->ProtoDeSerialize(SelfRoot);
            }

            Effect::~Effect()
            {
                // Detach our objects
                this->EFX->alAuxiliaryEffectSloti(this->InternalEffectSlot,AL_EFFECTSLOT_EFFECT,this->InternalEffect);

                if( this->InternalEffect != 0 ) {
                    // Obliterate our effect
                    this->EFX->alDeleteEffects(1,&InternalEffect);
                }

                if( this->InternalEffectSlot != 0 ) {
                    // Obliterate our effect slot
                    this->EFX->alDeleteAuxiliaryEffectSlots(1,&InternalEffectSlot);
                }
            }

            Boole Effect::CheckValid()
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
                        MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"OpenAL-Soft does not have an implementation for the Chorus Audio Effect.");
                        break;
                    }
                    case Audio::ET_Distortion:
                    {
                        MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"OpenAL-Soft does not have an implementation for the Distortion Audio Effect.");
                        break;
                    }
                    case Audio::ET_Echo:
                    {
                        this->EFX->alEffecti(this->InternalEffect,AL_EFFECT_TYPE,ConvertMezzanineEffectType(EffType));
                        break;
                    }
                    case Audio::ET_Flanger:
                    {
                        MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"OpenAL-Soft does not have an implementation for the Flanger Audio Effect.");
                        break;
                    }
                    case Audio::ET_Frequency_Shifter:
                    {
                        MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"OpenAL-Soft does not have an implementation for the Frequency Shift Audio Effect.");
                        break;
                    }
                    case Audio::ET_Vocal_Morpher:
                    {
                        MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"OpenAL-Soft does not have an implementation for the Vocal Morpher Audio Effect.");
                        break;
                    }
                    case Audio::ET_Pitch_Shifter:
                    {
                        MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"OpenAL-Soft does not have an implementation for the Pitch Shifter Audio Effect.");
                        break;
                    }
                    case Audio::ET_Ring_Modulator:
                    {
                        this->EFX->alEffecti(this->InternalEffect,AL_EFFECT_TYPE,ConvertMezzanineEffectType(EffType));
                        break;
                    }
                    case Audio::ET_Autowah:
                    {
                        MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"OpenAL-Soft does not have an implementation for the Autowah Audio Effect.");
                        break;
                    }
                    case Audio::ET_Compressor:
                    {
                        MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"OpenAL-Soft does not have an implementation for the Compressor Audio Effect.");
                        break;
                    }
                    case Audio::ET_Equalizer:
                    {
                        MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"OpenAL-Soft does not have an implementation for the Equalizer Audio Effect.");
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
                if( this->EffectFilter != Fil ) {
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
                if( this->EffectFilter != NULL ) {
                    this->EffectFilter = NULL;
                    this->Dirty = true;
                }
            }

            void Effect::SetVolume(const Real Vol)
            {
                if( this->Volume != Vol ) {
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
                if( this->IsIgnoringAttenuation() != Ignore ) {
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
                    ApplyEAXReverbParameters(this->EFX,this->InternalEffect,Param);
                    this->Valid = this->CheckValid();
                }
            }

            EAXReverbParameters Effect::GetEAXReverbParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_EAXREVERB ) {
                    return AquireEAXReverbParameters(this->EFX,this->InternalEffect);
                }
                return EAXReverbParameters(0,0,0,0,0,0,0,0,0,0,Vector3(0,0,0),0,0,Vector3(0,0,0),0,0,0,0,0,0,0,false);
            }

            void Effect::SetReverbParameters(const ReverbParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_REVERB ) {
                    ApplyReverbParameters(this->EFX,this->InternalEffect,Param);
                    this->Valid = this->CheckValid();
                }
            }

            ReverbParameters Effect::GetReverbParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_REVERB ) {
                    return AquireReverbParameters(this->EFX,this->InternalEffect);
                }
                return ReverbParameters(0,0,0,0,0,0,0,0,0,0,0,0,false);
            }

            void Effect::SetChorusParameters(const ChorusParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_CHORUS ) {
                    ApplyChorusParameters(this->EFX,this->InternalEffect,Param);
                    this->Valid = this->CheckValid();
                }
            }

            ChorusParameters Effect::GetChorusParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_CHORUS ) {
                    return AquireChorusParameters(this->EFX,this->InternalEffect);
                }
                return ChorusParameters(ChorusParameters::ECW_Triangle,0,0,0,0,0);
            }

            void Effect::SetDistortionParameters(const DistortionParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_DISTORTION ) {
                    ApplyDistortionParameters(this->EFX,this->InternalEffect,Param);
                    this->Valid = this->CheckValid();
                }
            }

            DistortionParameters Effect::GetDistortionParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_DISTORTION ) {
                    return AquireDistortionParameters(this->EFX,this->InternalEffect);
                }
                return DistortionParameters(0,0,0,0,0);
            }

            void Effect::SetEchoParameters(const EchoParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_ECHO ) {
                    ApplyEchoParameters(this->EFX,this->InternalEffect,Param);
                    this->Valid = this->CheckValid();
                }
            }

            EchoParameters Effect::GetEchoParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_ECHO ) {
                    return AquireEchoParameters(this->EFX,this->InternalEffect);
                }
                return EchoParameters(0,0,0,0,0);
            }

            void Effect::SetFlangerParameters(const FlangerParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_FLANGER ) {
                    ApplyFlangerParameters(this->EFX,this->InternalEffect,Param);
                    this->Valid = this->CheckValid();
                }
            }

            FlangerParameters Effect::GetFlangerParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_FLANGER ) {
                    return AquireFlangerParameters(this->EFX,this->InternalEffect);
                }
                return FlangerParameters(FlangerParameters::EFW_Triangle,0,0,0,0,0);
            }

            void Effect::SetFrequencyShiftParameters(const FrequencyShiftParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_FREQUENCY_SHIFTER ) {
                    ApplyFrequencyShiftParameters(this->EFX,this->InternalEffect,Param);
                    this->Valid = this->CheckValid();
                }
            }

            FrequencyShiftParameters Effect::GetFrequencyShiftParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_FREQUENCY_SHIFTER ) {
                    return AquireFrequencyShiftParameters(this->EFX,this->InternalEffect);
                }
                return FrequencyShiftParameters(0,FrequencyShiftParameters::ESD_Down,FrequencyShiftParameters::ESD_Down);
            }

            void Effect::SetVocalMorpherParameters(const VocalMorpherParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_VOCAL_MORPHER ) {
                    ApplyVocalMorpherParameters(this->EFX,this->InternalEffect,Param);
                    this->Valid = this->CheckValid();
                }
            }

            VocalMorpherParameters Effect::GetVocalMorpherParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_VOCAL_MORPHER ) {
                    return AquireVocalMorpherParameters(this->EFX,this->InternalEffect);
                }
                return VocalMorpherParameters(VocalMorpherParameters::EMP_A,
                                              VocalMorpherParameters::EMP_ER,
                                              0,0,
                                              VocalMorpherParameters::EMW_Sinusoid,
                                              0);
            }

            void Effect::SetPitchShifterParameters(const PitchShifterParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_PITCH_SHIFTER ) {
                    ApplyPitchShifterParameters(this->EFX,this->InternalEffect,Param);
                    this->Valid = this->CheckValid();
                }
            }

            PitchShifterParameters Effect::GetPitchShifterParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_PITCH_SHIFTER ) {
                    return AquirePitchShifterParameters(this->EFX,this->InternalEffect);
                }
                return PitchShifterParameters(0,0);
            }

            void Effect::SetRingModulatorParameters(const RingModulatorParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_RING_MODULATOR ) {
                    ApplyRingModulatorParameters(this->EFX,this->InternalEffect,Param);
                    this->Valid = this->CheckValid();
                }
            }

            RingModulatorParameters Effect::GetRingModulatorParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_RING_MODULATOR ) {
                    return AquireRingModulatorParameters(this->EFX,this->InternalEffect);
                }
                return RingModulatorParameters(0,0,RingModulatorParameters::EMW_Sinusoid);
            }

            void Effect::SetAutowahParameters(const AutowahParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_AUTOWAH ) {
                    ApplyAutowahParameters(this->EFX,this->InternalEffect,Param);
                    this->Valid = this->CheckValid();
                }
            }

            AutowahParameters Effect::GetAutowahParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_AUTOWAH ) {
                    return AquireAutowahParameters(this->EFX,this->InternalEffect);
                }
                return AutowahParameters(0,0,0,0);
            }

            void Effect::SetCompressorParameters(const CompressorParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_COMPRESSOR ) {
                    ApplyCompressorParameters(this->EFX,this->InternalEffect,Param);
                    this->Valid = this->CheckValid();
                }
            }

            CompressorParameters Effect::GetCompressorParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_COMPRESSOR ) {
                    return AquireCompressorParameters(this->EFX,this->InternalEffect);
                }
                return CompressorParameters(false);
            }

            void Effect::SetEqualizerParameters(const EqualizerParameters& Param)
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_EQUALIZER ) {
                    ApplyEqualizerParameters(this->EFX,this->InternalEffect,Param);
                    this->Valid = this->CheckValid();
                }
            }

            EqualizerParameters Effect::GetEqualizerParameters() const
            {
                ALenum InternalType = 0;
                this->EFX->alGetEffecti(this->InternalEffect,AL_EFFECT_TYPE,&InternalType);
                if( InternalType == AL_EFFECT_EQUALIZER ) {
                    return AquireEqualizerParameters(this->EFX,this->InternalEffect);
                }
                return EqualizerParameters(0,0,0,0,0,0,0,0,0,0);
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            void Effect::ProtoSerialize(XML::Node& ParentNode) const
            {
                XML::Node SelfRoot = ParentNode.AppendChild(Effect::GetSerializableName());

                if( SelfRoot.AppendAttribute("Version").SetValue("1") &&
                    SelfRoot.AppendAttribute("EffectType").SetValue( this->GetType() ) &&
                    SelfRoot.AppendAttribute("Volume").SetValue( this->GetVolume() ) &&
                    SelfRoot.AppendAttribute("IgnoreAttenuation").SetValue( this->IsIgnoringAttenuation() ) )
                {
                    XML::Node ParamsNode = SelfRoot.AppendChild("EffectParams");
                    switch( this->GetType() )
                    {
                        case Audio::ET_EAX_Reverb:         this->GetEAXReverbParameters().ProtoSerialize(ParamsNode);       break;
                        case Audio::ET_Reverb:             this->GetReverbParameters().ProtoSerialize(ParamsNode);          break;
                        case Audio::ET_Chorus:             this->GetChorusParameters().ProtoSerialize(ParamsNode);          break;
                        case Audio::ET_Distortion:         this->GetDistortionParameters().ProtoSerialize(ParamsNode);      break;
                        case Audio::ET_Echo:               this->GetEchoParameters().ProtoSerialize(ParamsNode);            break;
                        case Audio::ET_Flanger:            this->GetFlangerParameters().ProtoSerialize(ParamsNode);         break;
                        case Audio::ET_Frequency_Shifter:  this->GetFrequencyShiftParameters().ProtoSerialize(ParamsNode);  break;
                        case Audio::ET_Vocal_Morpher:      this->GetVocalMorpherParameters().ProtoSerialize(ParamsNode);    break;
                        case Audio::ET_Pitch_Shifter:      this->GetPitchShifterParameters().ProtoSerialize(ParamsNode);    break;
                        case Audio::ET_Ring_Modulator:     this->GetRingModulatorParameters().ProtoSerialize(ParamsNode);   break;
                        case Audio::ET_Autowah:            this->GetAutowahParameters().ProtoSerialize(ParamsNode);         break;
                        case Audio::ET_Compressor:         this->GetCompressorParameters().ProtoSerialize(ParamsNode);      break;
                        case Audio::ET_Equalizer:          this->GetEqualizerParameters().ProtoSerialize(ParamsNode);       break;
                        case Audio::ET_Null:  default:     break;
                    }

                    XML::Node FilterNode = SelfRoot.AppendChild("EffectFilter");
                    this->EffectFilter->ProtoSerialize( FilterNode );

                    return;
                }else{
                    SerializeError("Create XML Attribute Values",Effect::GetSerializableName(),true);
                }
            }

            void Effect::ProtoDeSerialize(const XML::Node& SelfRoot)
            {
                XML::Attribute CurrAttrib;

                if( SelfRoot.Name() == Effect::GetSerializableName() ) {
                    if(SelfRoot.GetAttribute("Version").AsInt() == 1) {
                        CurrAttrib = SelfRoot.GetAttribute("EffectType");
                        if( !CurrAttrib.Empty() )
                            this->SetType( static_cast<EffectType>( CurrAttrib.AsUint() ) );

                        CurrAttrib = SelfRoot.GetAttribute("Volume");
                        if( !CurrAttrib.Empty() )
                            this->SetVolume( CurrAttrib.AsReal() );

                        CurrAttrib = SelfRoot.GetAttribute("IgnoreAttenuation");
                        if( !CurrAttrib.Empty() )
                            this->IgnoreAttenuation( CurrAttrib.AsBool() );

                        XML::Node EffectParamsNode = SelfRoot.GetChild("EffectParams").GetFirstChild();
                        if( !EffectParamsNode.Empty() ) {
                            switch( this->GetType() )
                            {
                                case Audio::ET_EAX_Reverb: {
                                    EAXReverbParameters Params(EffectParamsNode);
                                    this->SetEAXReverbParameters(Params);
                                    break;
                                }
                                case Audio::ET_Reverb: {
                                    ReverbParameters Params(EffectParamsNode);
                                    this->SetReverbParameters(Params);
                                    break;
                                }
                                case Audio::ET_Chorus: {
                                    ChorusParameters Params(EffectParamsNode);
                                    this->SetChorusParameters(Params);
                                    break;
                                }
                                case Audio::ET_Distortion: {
                                    DistortionParameters Params(EffectParamsNode);
                                    this->SetDistortionParameters(Params);
                                    break;
                                }
                                case Audio::ET_Echo: {
                                    EchoParameters Params(EffectParamsNode);
                                    this->SetEchoParameters(Params);
                                    break;
                                }
                                case Audio::ET_Flanger: {
                                    FlangerParameters Params(EffectParamsNode);
                                    this->SetFlangerParameters(Params);
                                    break;
                                }
                                case Audio::ET_Frequency_Shifter: {
                                    FrequencyShiftParameters Params(EffectParamsNode);
                                    this->SetFrequencyShiftParameters(Params);
                                    break;
                                }
                                case Audio::ET_Vocal_Morpher: {
                                    VocalMorpherParameters Params(EffectParamsNode);
                                    this->SetVocalMorpherParameters(Params);
                                    break;
                                }
                                case Audio::ET_Pitch_Shifter: {
                                    PitchShifterParameters Params(EffectParamsNode);
                                    this->SetPitchShifterParameters(Params);
                                    break;
                                }
                                case Audio::ET_Ring_Modulator: {
                                    RingModulatorParameters Params(EffectParamsNode);
                                    this->SetRingModulatorParameters(Params);
                                    break;
                                }
                                case Audio::ET_Autowah: {
                                    AutowahParameters Params(EffectParamsNode);
                                    this->SetAutowahParameters(Params);
                                    break;
                                }
                                case Audio::ET_Compressor: {
                                    CompressorParameters Params(EffectParamsNode);
                                    this->SetCompressorParameters(Params);
                                    break;
                                }
                                case Audio::ET_Equalizer: {
                                    EqualizerParameters Params(EffectParamsNode);
                                    this->SetEqualizerParameters(Params);
                                    break;
                                }
                                case Audio::ET_Null:  default:  break;
                            }
                        }

                        XML::Node EffectFilterNode = SelfRoot.GetChild("EffectFilter").GetFirstChild();
                        if( !EffectFilterNode.Empty() ) {
                            this->EffectFilter = static_cast<OALS::Filter*>( Creator->CreateFilter(EffectFilterNode) );
                        }

                        this->Valid = this->CheckValid();
                    }else{
                        MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + Effect::GetSerializableName() + ": Not Version 1.");
                    }
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,Effect::GetSerializableName() + " was not found in the provided XML node, which was expected.");
                }
            }

            String Effect::GetSerializableName()
                { return "OALSEffect"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            UInt32 Effect::_GetInternalEffect() const
                { return this->InternalEffect; }

            UInt32 Effect::_GetInternalEffectSlot() const
                { return this->InternalEffectSlot; }

            Boole Effect::_IsDirty() const
                { return ( this->Dirty || ( this->EffectFilter && this->EffectFilter->_IsDirty() ) ); }

            void Effect::_Clean()
                { this->Dirty = false; }
        }//OALS
    }//Audio
}//Mezzanine

#endif
