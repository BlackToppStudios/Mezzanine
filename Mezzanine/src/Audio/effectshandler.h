// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#ifndef _audioeffectshandler_h
#define _audioeffectshandler_h

#include "Audio/effect.h"
#include "Audio/filter.h"

namespace Mezzanine
{
    namespace Audio
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is an interface class for the creation, destruction, and overall management of audio effects.
        /// @details
        ///////////////////////////////////////
        class iEffectsHandler
        {
            public:
                /// @brief Class constructor.
                iEffectsHandler() {  }
                /// @brief Class destructor.
                virtual ~iEffectsHandler() {  }

                ///////////////////////////////////////////////////////////////////////////////
                // Creation

                /// @brief Creates an Audio Effect for use with @ref Sound instances.
                /// @return Returns a pointer to the created Effect, or NULL if there was an error.
                virtual iEffect* CreateEffect() = 0;
                /// @brief Creates an Audio Filter for use with @ref Sound instances.
                /// @return Returns a pointer to the created Filter, or NULL if there was an error.
                virtual iFilter* CreateFilter() = 0;

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @brief Gets the max number of effects that can be run at the same time on this system.
                /// @return Returns the max number of effects supported on this system.
                virtual UInt32 GetMaxEffectsSupported() const = 0;
                /// @brief Checks to see if the given effect type is supported.
                /// @param Type The effect type to be checked.
                /// @return Returns true if the effect is supported, false if the effect isn't supported.
                virtual bool IsEffectSupported(const EffectType Type) const = 0;
                /// @brief Checks to see if the given filter type is supported.
                /// @param Type The filter type to be checked.
                /// @return Returns true if the filter is supported, false if the filter isn't supported.
                virtual bool IsFilterSupported(const FilterType Type) const = 0;

                /// @brief Returns if a effect preset of a certain name is registered.
                /// @param Type The effect type to be checked.
                /// @param Name The name of the preset to check for.
                /// @return True if it exists, false if not.
                virtual bool IsEffectPresetRegistered(const EffectType Type, const String& Name) = 0;
                /// @brief Removes a previously registered effect preset.
                /// @param Type The type of effect to remove.
                /// @param Name The name of the preset to remove.
                virtual void RemoveEffectPreset(const EffectType Type, const String& Name) = 0;
                /// @brief Removes all effect presets for a specific effect type.
                /// @param Type The type of effect to remove presets for, or ET_Null to remove all of them.
                virtual void RemoveAllEffectPresets(const EffectType Type) = 0;

                ///////////////////////////////////////////////////////////////////////////////
                // Individual effect type settings

                /// @brief Adds a preset for the EAX Reverb Audio Effect type.
                /// @param Name The name to be given to the preset.
                /// @param Setting Parameter struct containing all the parameters to be given to this preset.
                /// @return Returns true on success, false on failure.
                virtual bool AddEAXReverbEffectPreset(const String& Name, const EAXReverbParameters& Setting) = 0;
                /// @brief Returns a previously registered preset for the EAX Reverb Effect.
                /// @param Name The name of the preset to retrieve.
                /// @return Returns the specified preset or the default parameters if the preset could not be found.
                virtual EAXReverbParameters GetEAXReverbEffectPreset(const String& Name) = 0;

                /// @brief Adds a preset for the Reverb Audio Effect type.
                /// @param Name The name to be given to the preset.
                /// @param Setting Parameter struct containing all the parameters to be given to this preset.
                /// @return Returns true on success, false on failure.
                virtual bool AddReverbEffectPreset(const String& Name, const ReverbParameters& Setting) = 0;
                /// @brief Returns a previously registered preset for the Reverb Effect.
                /// @param Name The name of the preset to retrieve.
                /// @return Returns the specified preset or the default parameters if the preset could not be found.
                virtual ReverbParameters GetReverbEffectPreset(const String& Name) = 0;

                /// @brief Adds a preset for the Chorus Audio Effect type.
                /// @param Name The name to be given to the preset.
                /// @param Setting Parameter struct containing all the parameters to be given to this preset.
                /// @return Returns true on success, false on failure.
                virtual bool AddChorusEffectPreset(const String& Name, const ChorusParameters& Setting) = 0;
                /// @brief Returns a previously registered preset for the Chorus Effect.
                /// @param Name The name of the preset to retrieve.
                /// @return Returns the specified preset or the default parameters if the preset could not be found.
                virtual ChorusParameters GetChorusEffectPreset(const String& Name) = 0;

                /// @brief Adds a preset for the Distortion Audio Effect type.
                /// @param Name The name to be given to the preset.
                /// @param Setting Parameter struct containing all the parameters to be given to this preset.
                /// @return Returns true on success, false on failure.
                virtual bool AddDistortionEffectPreset(const String& Name, const DistortionParameters& Setting) = 0;
                /// @brief Returns a previously registered preset for the Distortion Effect.
                /// @param Name The name of the preset to retrieve.
                /// @return Returns the specified preset or the default parameters if the preset could not be found.
                virtual DistortionParameters GetDistortionEffectPreset(const String& Name) = 0;

                /// @brief Adds a preset for the Echo Audio Effect type.
                /// @param Name The name to be given to the preset.
                /// @param Setting Parameter struct containing all the parameters to be given to this preset.
                /// @return Returns true on success, false on failure.
                virtual bool AddEchoEffectPreset(const String& Name, const EchoParameters& Setting) = 0;
                /// @brief Returns a previously registered preset for the Echo Effect.
                /// @param Name The name of the preset to retrieve.
                /// @return Returns the specified preset or the default parameters if the preset could not be found.
                virtual EchoParameters GetEchoEffectPreset(const String& Name) = 0;

                /// @brief Adds a preset for the Flanger Audio Effect type.
                /// @param Name The name to be given to the preset.
                /// @param Setting Parameter struct containing all the parameters to be given to this preset.
                /// @return Returns true on success, false on failure.
                virtual bool AddFlangerEffectPreset(const String& Name, const FlangerParameters& Setting) = 0;
                /// @brief Returns a previously registered preset for the Flanger Effect.
                /// @param Name The name of the preset to retrieve.
                /// @return Returns the specified preset or the default parameters if the preset could not be found.
                virtual FlangerParameters GetFlangerEffectPreset(const String& Name) = 0;

                /// @brief Adds a preset for the Frequency Shift Audio Effect type.
                /// @param Name The name to be given to the preset.
                /// @param Setting Parameter struct containing all the parameters to be given to this preset.
                /// @return Returns true on success, false on failure.
                virtual bool AddFrequencyShiftEffectPreset(const String& Name, const FrequencyShiftParameters& Setting) = 0;
                /// @brief Returns a previously registered preset for the Frequency Shift Effect.
                /// @param Name The name of the preset to retrieve.
                /// @return Returns the specified preset or the default parameters if the preset could not be found.
                virtual FrequencyShiftParameters GetFrequencyShiftEffectPreset(const String& Name) = 0;

                /// @brief Adds a preset for the Vocal Morpher Audio Effect type.
                /// @param Name The name to be given to the preset.
                /// @param Setting Parameter struct containing all the parameters to be given to this preset.
                /// @return Returns true on success, false on failure.
                virtual bool AddVocalMorpherEffectPreset(const String& Name, const VocalMorpherParameters& Setting) = 0;
                /// @brief Returns a previously registered preset for the Vocal Morpher Effect.
                /// @param Name The name of the preset to retrieve.
                /// @return Returns the specified preset or the default parameters if the preset could not be found.
                virtual VocalMorpherParameters GetVocalMorpherEffectPreset(const String& Name) = 0;

                /// @brief Adds a preset for the Pitch Shifter Audio Effect type.
                /// @param Name The name to be given to the preset.
                /// @param Setting Parameter struct containing all the parameters to be given to this preset.
                /// @return Returns true on success, false on failure.
                virtual bool AddPitchShifterEffectPreset(const String& Name, const PitchShifterParameters& Setting) = 0;
                /// @brief Returns a previously registered preset for the Pitch Shifter Effect.
                /// @param Name The name of the preset to retrieve.
                /// @return Returns the specified preset or the default parameters if the preset could not be found.
                virtual PitchShifterParameters GetPitchShifterEffectPreset(const String& Name) = 0;

                /// @brief Adds a preset for the Ring Modulator Audio Effect type.
                /// @param Name The name to be given to the preset.
                /// @param Setting Parameter struct containing all the parameters to be given to this preset.
                /// @return Returns true on success, false on failure.
                virtual bool AddRingModulatorEffectPreset(const String& Name, const RingModulatorParameters& Setting) = 0;
                /// @brief Returns a previously registered preset for the Ring Modulator Effect.
                /// @param Name The name of the preset to retrieve.
                /// @return Returns the specified preset or the default parameters if the preset could not be found.
                virtual RingModulatorParameters GetRingModulatorEffectPreset(const String& Name) = 0;

                /// @brief Adds a preset for the Autowah Audio Effect type.
                /// @param Name The name to be given to the preset.
                /// @param Setting Parameter struct containing all the parameters to be given to this preset.
                /// @return Returns true on success, false on failure.
                virtual bool AddAutowahEffectPreset(const String& Name, const AutowahParameters& Setting) = 0;
                /// @brief Returns a previously registered preset for the Autowah Effect.
                /// @param Name The name of the preset to retrieve.
                /// @return Returns the specified preset or the default parameters if the preset could not be found.
                virtual AutowahParameters GetAutowahEffectPreset(const String& Name) = 0;

                /// @brief Adds a preset for the Compressor Audio Effect type.
                /// @param Name The name to be given to the preset.
                /// @param Setting Parameter struct containing all the parameters to be given to this preset.
                /// @return Returns true on success, false on failure.
                virtual bool AddCompressorEffectPreset(const String& Name, const CompressorParameters& Setting) = 0;
                /// @brief Returns a previously registered preset for the Compressor Effect.
                /// @param Name The name of the preset to retrieve.
                /// @return Returns the specified preset or the default parameters if the preset could not be found.
                virtual CompressorParameters GetCompressorEffectPreset(const String& Name) = 0;

                /// @brief Adds a preset for the Equalizer Audio Effect type.
                /// @param Name The name to be given to the preset.
                /// @param Setting Parameter struct containing all the parameters to be given to this preset.
                /// @return Returns true on success, false on failure.
                virtual bool AddEqualizerEffectPreset(const String& Name, const EqualizerParameters& Setting) = 0;
                /// @brief Returns a previously registered preset for the Equalizer Effect.
                /// @param Name The name of the preset to retrieve.
                /// @return Returns the specified preset or the default parameters if the preset could not be found.
                virtual EqualizerParameters GetEqualizerEffectPreset(const String& Name) = 0;
        };//iEffectsHandler
    }//Audio
}//Mezzanine

#endif
