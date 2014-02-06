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
#ifndef _audioeffectshandler_h
#define _audioeffectshandler_h

#include "Audio/effect.h"
#include "Audio/filter.h"
#include "Audio/effectparameters.h"

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
            // Utility

            ///////////////////////////////////////////////////////////////////////////////
            // Effect Handling

            /// @brief Checks to see if the given effect type is supported.
            /// @param Type The effect type to be checked.
            /// @return Returns true if the effect is supported, false if the effect isn't supported.
            virtual bool IsEffectSupported(const EffectType Type) const = 0;
            /// @brief Creates an Audio Effect for use with Audio::Sound instances.
            /// @return Returns a pointer to the created Effect, or NULL if there was an error.
            virtual iEffect* CreateEffect() = 0;
            /// @brief Gets an Audio::iEffect instance by index.
            /// @param Index The index of the Audio::iEffect instance to retrieve.
            /// @return Returns a pointer to the Audio::iEffect at the specified index.
            virtual iEffect* GetEffect(const UInt32 Index) const = 0;
            /// @brief Gets the number of Audio::iEffect instances being stored by this handler.
            /// @return Returns a UInt32 representing the number of Audio::iEffect instances contained in this handler.
            virtual UInt32 GetNumEffects() const = 0;
            /// @brief Destroys a single Audio::iEffect instance.
            /// @param ToBeDestroyed A pointer to the Audio::iEffect to be destroyed.
            virtual void DestroyEffect(iEffect* ToBeDestroyed) = 0;
            /// @brief Destroys all Audio::iEffect instances being stored by this handler.
            virtual void DestroyAllEffects() = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Filter Handling

            /// @brief Checks to see if the given filter type is supported.
            /// @param Type The filter type to be checked.
            /// @return Returns true if the filter is supported, false if the filter isn't supported.
            virtual bool IsFilterSupported(const FilterType Type) const = 0;
            /// @brief Creates an Audio Filter for use with @ref iSound instances.
            /// @return Returns a pointer to the created Filter, or NULL if there was an error.
            virtual iFilter* CreateFilter() = 0;
            /// @brief Gets an @ref Audio::iFilter instance by index.
            /// @param Index The index of the Audio::iFilter instance to retrieve.
            /// @return Returns a pointer to the @ref Audio::iFilter at the specified index.
            virtual iFilter* GetFilter(const UInt32 Index) const = 0;
            /// @brief Gets the number of @ref iFilter instances being stored by this handler.
            /// @return Returns a UInt32 representing the number of @ref iFilter instances contained in this handler.
            virtual UInt32 GetNumFilters() const = 0;
            /// @brief Destroys a single @ref iFilter instance.
            /// @param ToBeDestroyed A pointer to the @ref iFilter to be destroyed.
            virtual void DestroyFilter(iFilter* ToBeDestroyed) = 0;
            /// @brief Destroys all @ref iFilter instances being stored by this handler.
            virtual void DestroyAllFilters() = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Preset Handling

            /// @brief Gets whether or not an effect preset exists within this handler.
            /// @param Type The effect type to be checked.
            /// @param Name The name of the preset to check for.
            /// @return True if it exists, false if not.
            virtual bool EffectPresetExists(const EffectType Type, const String& Name) = 0;
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
            virtual void AddEAXReverbEffectPreset(const String& Name, const EAXReverbParameters& Setting) = 0;
            /// @brief Returns a previously registered preset for the EAX Reverb Effect.
            /// @param Name The name of the preset to retrieve.
            /// @return Returns the specified preset or the default parameters if the preset could not be found.
            virtual EAXReverbParameters GetEAXReverbEffectPreset(const String& Name) const = 0;

            /// @brief Adds a preset for the Reverb Audio Effect type.
            /// @param Name The name to be given to the preset.
            /// @param Setting Parameter struct containing all the parameters to be given to this preset.
            virtual void AddReverbEffectPreset(const String& Name, const ReverbParameters& Setting) = 0;
            /// @brief Returns a previously registered preset for the Reverb Effect.
            /// @param Name The name of the preset to retrieve.
            /// @return Returns the specified preset or the default parameters if the preset could not be found.
            virtual ReverbParameters GetReverbEffectPreset(const String& Name) const = 0;

            /// @brief Adds a preset for the Chorus Audio Effect type.
            /// @param Name The name to be given to the preset.
            /// @param Setting Parameter struct containing all the parameters to be given to this preset.
            virtual void AddChorusEffectPreset(const String& Name, const ChorusParameters& Setting) = 0;
            /// @brief Returns a previously registered preset for the Chorus Effect.
            /// @param Name The name of the preset to retrieve.
            /// @return Returns the specified preset or the default parameters if the preset could not be found.
            virtual ChorusParameters GetChorusEffectPreset(const String& Name) const = 0;

            /// @brief Adds a preset for the Distortion Audio Effect type.
            /// @param Name The name to be given to the preset.
            /// @param Setting Parameter struct containing all the parameters to be given to this preset.
            virtual void AddDistortionEffectPreset(const String& Name, const DistortionParameters& Setting) = 0;
            /// @brief Returns a previously registered preset for the Distortion Effect.
            /// @param Name The name of the preset to retrieve.
            /// @return Returns the specified preset or the default parameters if the preset could not be found.
            virtual DistortionParameters GetDistortionEffectPreset(const String& Name) const = 0;

            /// @brief Adds a preset for the Echo Audio Effect type.
            /// @param Name The name to be given to the preset.
            /// @param Setting Parameter struct containing all the parameters to be given to this preset.
            virtual void AddEchoEffectPreset(const String& Name, const EchoParameters& Setting) = 0;
            /// @brief Returns a previously registered preset for the Echo Effect.
            /// @param Name The name of the preset to retrieve.
            /// @return Returns the specified preset or the default parameters if the preset could not be found.
            virtual EchoParameters GetEchoEffectPreset(const String& Name) const = 0;

            /// @brief Adds a preset for the Flanger Audio Effect type.
            /// @param Name The name to be given to the preset.
            /// @param Setting Parameter struct containing all the parameters to be given to this preset.
            virtual void AddFlangerEffectPreset(const String& Name, const FlangerParameters& Setting) = 0;
            /// @brief Returns a previously registered preset for the Flanger Effect.
            /// @param Name The name of the preset to retrieve.
            /// @return Returns the specified preset or the default parameters if the preset could not be found.
            virtual FlangerParameters GetFlangerEffectPreset(const String& Name) const = 0;

            /// @brief Adds a preset for the Frequency Shift Audio Effect type.
            /// @param Name The name to be given to the preset.
            /// @param Setting Parameter struct containing all the parameters to be given to this preset.
            virtual void AddFrequencyShiftEffectPreset(const String& Name, const FrequencyShiftParameters& Setting) = 0;
            /// @brief Returns a previously registered preset for the Frequency Shift Effect.
            /// @param Name The name of the preset to retrieve.
            /// @return Returns the specified preset or the default parameters if the preset could not be found.
            virtual FrequencyShiftParameters GetFrequencyShiftEffectPreset(const String& Name) const = 0;

            /// @brief Adds a preset for the Vocal Morpher Audio Effect type.
            /// @param Name The name to be given to the preset.
            /// @param Setting Parameter struct containing all the parameters to be given to this preset.
            virtual void AddVocalMorpherEffectPreset(const String& Name, const VocalMorpherParameters& Setting) = 0;
            /// @brief Returns a previously registered preset for the Vocal Morpher Effect.
            /// @param Name The name of the preset to retrieve.
            /// @return Returns the specified preset or the default parameters if the preset could not be found.
            virtual VocalMorpherParameters GetVocalMorpherEffectPreset(const String& Name) const = 0;

            /// @brief Adds a preset for the Pitch Shifter Audio Effect type.
            /// @param Name The name to be given to the preset.
            /// @param Setting Parameter struct containing all the parameters to be given to this preset.
            virtual void AddPitchShifterEffectPreset(const String& Name, const PitchShifterParameters& Setting) = 0;
            /// @brief Returns a previously registered preset for the Pitch Shifter Effect.
            /// @param Name The name of the preset to retrieve.
            /// @return Returns the specified preset or the default parameters if the preset could not be found.
            virtual PitchShifterParameters GetPitchShifterEffectPreset(const String& Name) const = 0;

            /// @brief Adds a preset for the Ring Modulator Audio Effect type.
            /// @param Name The name to be given to the preset.
            /// @param Setting Parameter struct containing all the parameters to be given to this preset.
            virtual void AddRingModulatorEffectPreset(const String& Name, const RingModulatorParameters& Setting) = 0;
            /// @brief Returns a previously registered preset for the Ring Modulator Effect.
            /// @param Name The name of the preset to retrieve.
            /// @return Returns the specified preset or the default parameters if the preset could not be found.
            virtual RingModulatorParameters GetRingModulatorEffectPreset(const String& Name) const = 0;

            /// @brief Adds a preset for the Autowah Audio Effect type.
            /// @param Name The name to be given to the preset.
            /// @param Setting Parameter struct containing all the parameters to be given to this preset.
            virtual void AddAutowahEffectPreset(const String& Name, const AutowahParameters& Setting) = 0;
            /// @brief Returns a previously registered preset for the Autowah Effect.
            /// @param Name The name of the preset to retrieve.
            /// @return Returns the specified preset or the default parameters if the preset could not be found.
            virtual AutowahParameters GetAutowahEffectPreset(const String& Name) const = 0;

            /// @brief Adds a preset for the Compressor Audio Effect type.
            /// @param Name The name to be given to the preset.
            /// @param Setting Parameter struct containing all the parameters to be given to this preset.
            virtual void AddCompressorEffectPreset(const String& Name, const CompressorParameters& Setting) = 0;
            /// @brief Returns a previously registered preset for the Compressor Effect.
            /// @param Name The name of the preset to retrieve.
            /// @return Returns the specified preset or the default parameters if the preset could not be found.
            virtual CompressorParameters GetCompressorEffectPreset(const String& Name) const = 0;

            /// @brief Adds a preset for the Equalizer Audio Effect type.
            /// @param Name The name to be given to the preset.
            /// @param Setting Parameter struct containing all the parameters to be given to this preset.
            virtual void AddEqualizerEffectPreset(const String& Name, const EqualizerParameters& Setting) = 0;
            /// @brief Returns a previously registered preset for the Equalizer Effect.
            /// @param Name The name of the preset to retrieve.
            /// @return Returns the specified preset or the default parameters if the preset could not be found.
            virtual EqualizerParameters GetEqualizerEffectPreset(const String& Name) const = 0;
        };//iEffectsHandler
    }//Audio
}//Mezzanine

#endif
