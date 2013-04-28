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
#ifndef _audioeffect_h
#define _audioeffect_h

#include "datatypes.h"
#include "Audio/effectparameters.h"
#include "Audio/filter.h"

namespace Mezzanine
{
    namespace Audio
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is an interface class for an effect that can be applied to a sound.
        /// @details
        ///////////////////////////////////////
        class iEffect
        {
            public:
                /// @brief Class constructor.
                iEffect() {  }
                /// @brief Class destructor.
                virtual ~iEffect() {  }

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @brief Gets whether or not this effect is ready for use.
                /// @return Returns if this effect is ready to be used or if it has encountered a fatal error.
                virtual bool IsValid() const = 0;

                /// @brief Sets the type of this effect.
                /// @param EffType Type of effect to switch to.
                virtual void SetType(const EffectType& EffType) = 0;
                /// @brief Gets the type of effect this is.
                /// @return Returns the current type this effect object is set to.
                virtual const EffectType& GetType() const = 0;
                /// @brief Attaches a filter to this effect.
                /// @param Filter A Pointer to the filter to attach.
                virtual void AttachFilter(iFilter* Filter) = 0;
                /// @brief Gets the filter being used by this effect.
                /// @return Returns the filter attached to this effect.
                virtual iFilter* GetFilter() const = 0;
                /// @brief Removes the currently attached filter.
                virtual void RemoveFilter() = 0;

                /// @brief Sets the master volume for this effect.
                /// @remarks This volume scales the amount of effect audible from all attached sources.  @n @n 1.0f equal no volume change.  Range: 0.0f to 1.0
                /// @param Vol The volume to be set.
                virtual void SetVolume(const Real Vol) = 0;
                /// @brief Gets the volume for this effect.
                /// @remarks This volume scales the amount of effect audible from all attached sources.
                /// @return Returns a Real representing the currently set volume.
                virtual Real GetVolume() const = 0;
                /// @brief Sets whether the effect for each attached source is attenuated by distance.
                /// @remarks If set to true, can cause some interesting and non-realistic effects, so be careful with it.
                /// @param Ignore Whether or not to ignore attenuation.
                virtual void IgnoreAttenuation(bool Ignore) = 0;
                /// @brief Gets whether or not attached sources are attenuated by distance.
                /// @return Returns true if the effect for each attached source is attenuated by distance.
                virtual bool IsIgnoringAttenuation() const = 0;

                ///////////////////////////////////////////////////////////////////////////////
                // Parameter Configuration

                /// @brief Sets the parameters for the EAX Reverb Effect.
                /// @param Param Parameter struct populated with the settings for this effect.
                virtual void SetEAXReverbParameters(const EAXReverbParameters& Param) = 0;
                /// @brief Gets the current parameters for the EAX Reverb Effect.
                /// @return Returns an EAXReverbParameters struct containing the currently set parameters.
                virtual const EAXReverbParameters& GetEAXReverbParameters() const = 0;
                /// @brief Sets the parameters for the Reverb Effect.
                /// @param Param Parameter struct populated with the settings for this effect.
                virtual void SetReverbParameters(const ReverbParameters& Param) = 0;
                /// @brief Gets the current parameters for the Reverb Effect.
                /// @return Returns an ReverbParameters struct containing the currently set parameters.
                virtual const ReverbParameters& GetReverbParameters() const = 0;
                /// @brief Sets the parameters for the Chorus Effect.
                /// @param Param Parameter struct populated with the settings for this effect.
                virtual void SetChorusParameters(const ChorusParameters& Param) = 0;
                /// @brief Gets the current parameters for the Chorus Effect.
                /// @return Returns an ChorusParameters struct containing the currently set parameters.
                virtual const ChorusParameters& GetChorusParameters() const = 0;
                /// @brief Sets the parameters for the Distortion Effect.
                /// @param Param Parameter struct populated with the settings for this effect.
                virtual void SetDistortionParameters(const DistortionParameters& Param) = 0;
                /// @brief Gets the current parameters for the Distortion Effect.
                /// @return Returns an DistortionParameters struct containing the currently set parameters.
                virtual const DistortionParameters& GetDistortionParameters() const = 0;
                /// @brief Sets the parameters for the Echo Effect.
                /// @param Param Parameter struct populated with the settings for this effect.
                virtual void SetEchoParameters(const EchoParameters& Param) = 0;
                /// @brief Gets the current parameters for the Echo Effect.
                /// @return Returns an EchoParameters struct containing the currently set parameters.
                virtual const EchoParameters& GetEchoParameters() const = 0;
                /// @brief Sets the parameters for the Flanger Effect.
                /// @param Param Parameter struct populated with the settings for this effect.
                virtual void SetFlangerParameters(const FlangerParameters& Param) = 0;
                /// @brief Gets the current parameters for the Flanger Effect.
                /// @return Returns an FlangerParameters struct containing the currently set parameters.
                virtual const FlangerParameters& GetFlangerParameters() const = 0;
                /// @brief Sets the parameters for the Frequency Shift Effect.
                /// @param Param Parameter struct populated with the settings for this effect.
                virtual void SetFrequencyShiftParameters(const FrequencyShiftParameters& Param) = 0;
                /// @brief Gets the current parameters for the Frequency Shift Effect.
                /// @return Returns an FrequencyShiftParameters struct containing the currently set parameters.
                virtual const FrequencyShiftParameters& GetFrequencyShiftParameters() const = 0;
                /// @brief Sets the parameters for the Vocal Morpher Effect.
                /// @param Param Parameter struct populated with the settings for this effect.
                virtual void SetVocalMorpherParameters(const VocalMorpherParameters& Param) = 0;
                /// @brief Gets the current parameters for the Vocal Morpher Effect.
                /// @return Returns an VocalMorpherParameters struct containing the currently set parameters.
                virtual const VocalMorpherParameters& GetVocalMorpherParameters() const = 0;
                /// @brief Sets the parameters for the Pitch Shifter Effect.
                /// @param Param Parameter struct populated with the settings for this effect.
                virtual void SetPitchShifterParameters(const PitchShifterParameters& Param) = 0;
                /// @brief Gets the current parameters for the PitchShifter Effect.
                /// @return Returns an PitchShifterParameters struct containing the currently set parameters.
                virtual const PitchShifterParameters& GetPitchShifterParameters() const = 0;
                /// @brief Sets the parameters for the Ring Modulator Effect.
                /// @param Param Parameter struct populated with the settings for this effect.
                virtual void SetRingModulatorParameters(const RingModulatorParameters& Param) = 0;
                /// @brief Gets the current parameters for the Ring Modulator Effect.
                /// @return Returns an RingModulatorParameters struct containing the currently set parameters.
                virtual const RingModulatorParameters& GetRingModulatorParameters() const = 0;
                /// @brief Sets the parameters for the Autowah Effect.
                /// @param Param Parameter struct populated with the settings for this effect.
                virtual void SetAutowahParameters(const AutowahParameters& Param) = 0;
                /// @brief Gets the current parameters for the Autowah Effect.
                /// @return Returns an AutowahParameters struct containing the currently set parameters.
                virtual const AutowahParameters& GetAutowahParameters() const = 0;
                /// @brief Sets the parameters for the Compressor Effect.
                /// @param Param Parameter struct populated with the settings for this effect.
                virtual void SetCompressorParameters(const CompressorParameters& Param) = 0;
                /// @brief Gets the current parameters for the Compressor Effect.
                /// @return Returns an CompressorParameters struct containing the currently set parameters.
                virtual const CompressorParameters& GetCompressorParameters() const = 0;
                /// @brief Sets the parameters for the Equalizer Effect.
                /// @param Param Parameter struct populated with the settings for this effect.
                virtual void SetEqualizerParameters(const EqualizerParameters& Param) = 0;
                /// @brief Gets the current parameters for the Equalizer Effect.
                /// @return Returns an EqualizerParameters struct containing the currently set parameters.
                virtual const EqualizerParameters& GetEqualizerParameters() const = 0;

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Methods

                /// @internal
                /// @brief
                /// @remarks Used internally by the engine to update attached sources.
                /// @return Returns a timestamp indicating the last time settings on this effect were changed.  Used internally by the engine to update attached sources.
                virtual TimeMarker GetLastUpdated() const = 0;
        };//iEffect
    }//Audio
}//Mezzanine

#endif
