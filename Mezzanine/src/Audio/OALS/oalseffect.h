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
#ifndef _audiooalseffect_h
#define _audiooalseffect_h

#include "Audio/effect.h"

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            class Filter;
            class EFXInterface;
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief This is the OpenALSoft implemenation for an Audio Effect.
            ///////////////////////////////////////
            class MEZZ_LIB Effect : public iEffect
            {
            protected:
                /// @brief Pointer to the class holding function pointers to all the OALS EFX methods
                EFXInterface* EFX;
                /// @brief The filter processing audio emitted by this effect.
                OALS::Filter* EffectFilter;
                /// @brief This stores the volume modifier to be applied to processed Audio.
                Real Volume;
                /// @brief This stores the internal ID for this effect.
                UInt32 InternalEffect;
                /// @brief This stores the internal ID for this for the effect slot being populated by the internal effect.
                UInt32 InternalEffectSlot;
                /// @brief This stores whether or not this effect has been modified recently.
                Boole Dirty;
                /// @brief This stores whether or not there was an error with the last settings change.
                Boole Valid;
                /// @brief This stores whether or not audio processed through this effect will ignore attenuation based on distance.
                Boole IgnoreAtten;

                /// @brief Checks to see if there was a configuration error after setting a filter property.
                /// @return Returns true if there was no error, false if there was.
                Boole CheckValid();
            public:
                /// @brief Class constructor.
                /// @param EFXMethods A pointer to the class storing all the methods to the EFX extension.
                Effect(EFXInterface* EFXMethods);
                /// @brief Serialization constructor.
                /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
                /// @param EFXMethods A pointer to the class storing all the methods to the EFX extension.
                Effect(const XML::Node& SelfRoot, EFXInterface* EFXMethods);
                /// @brief Class destructor.
                virtual ~Effect();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc iEffect::IsValid() const
                virtual Boole IsValid() const;

                /// @copydoc iEffect::SetType(const EffectType& EffType)
                virtual void SetType(const EffectType& EffType);
                /// @copydoc iEffect::GetType() const
                virtual EffectType GetType() const;
                /// @copydoc iEffect::AttachFilter(iFilter* Fil)
                virtual void AttachFilter(iFilter* Fil);
                /// @copydoc iEffect::GetFilter() const
                virtual iFilter* GetFilter() const;
                /// @copydoc iEffect::RemoveFilter()
                virtual void RemoveFilter();

                /// @copydoc iEffect::SetVolume(const Real Vol)
                virtual void SetVolume(const Real Vol);
                /// @copydoc iEffect::GetVolume() const
                virtual Real GetVolume() const;
                /// @copydoc iEffect::IgnoreAttenuation(Boole Ignore)
                virtual void IgnoreAttenuation(Boole Ignore);
                /// @copydoc iEffect::IsIgnoringAttenuation() const
                virtual Boole IsIgnoringAttenuation() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Parameter Configuration

                /// @copydoc iEffect::SetEAXReverbParameters(const EAXReverbParameters& Param)
                virtual void SetEAXReverbParameters(const EAXReverbParameters& Param);
                /// @copydoc iEffect::GetEAXReverbParameters() const
                virtual EAXReverbParameters GetEAXReverbParameters() const;
                /// @copydoc iEffect::SetReverbParameters(const ReverbParameters& Param)
                virtual void SetReverbParameters(const ReverbParameters& Param);
                /// @copydoc iEffect::GetReverbParameters() const
                virtual ReverbParameters GetReverbParameters() const;
                /// @copydoc iEffect::SetChorusParameters(const ChorusParameters& Param)
                virtual void SetChorusParameters(const ChorusParameters& Param);
                /// @copydoc iEffect::GetChorusParameters() const
                virtual ChorusParameters GetChorusParameters() const;
                /// @copydoc iEffect::SetDistortionParameters(const DistortionParameters& Param)
                virtual void SetDistortionParameters(const DistortionParameters& Param);
                /// @copydoc iEffect::GetDistortionParameters() const
                virtual DistortionParameters GetDistortionParameters() const;
                /// @copydoc iEffect::SetEchoParameters(const EchoParameters& Param)
                virtual void SetEchoParameters(const EchoParameters& Param);
                /// @copydoc iEffect::GetEchoParameters() const
                virtual EchoParameters GetEchoParameters() const;
                /// @copydoc iEffect::SetFlangerParameters(const FlangerParameters& Param)
                virtual void SetFlangerParameters(const FlangerParameters& Param);
                /// @copydoc iEffect::GetFlangerParameters() const
                virtual FlangerParameters GetFlangerParameters() const;
                /// @copydoc iEffect::SetFrequencyShiftParameters(const FrequencyShiftParameters& Param)
                virtual void SetFrequencyShiftParameters(const FrequencyShiftParameters& Param);
                /// @copydoc iEffect::GetFrequencyShiftParameters() const
                virtual FrequencyShiftParameters GetFrequencyShiftParameters() const;
                /// @copydoc iEffect::SetVocalMorpherParameters(const VocalMorpherParameters& Param)
                virtual void SetVocalMorpherParameters(const VocalMorpherParameters& Param);
                /// @copydoc iEffect::GetVocalMorpherParameters() const
                virtual VocalMorpherParameters GetVocalMorpherParameters() const;
                /// @copydoc iEffect::SetPitchShifterParameters(const PitchShifterParameters& Param)
                virtual void SetPitchShifterParameters(const PitchShifterParameters& Param);
                /// @copydoc iEffect::GetPitchShifterParameters() const
                virtual PitchShifterParameters GetPitchShifterParameters() const;
                /// @copydoc iEffect::SetRingModulatorParameters(const RingModulatorParameters& Param)
                virtual void SetRingModulatorParameters(const RingModulatorParameters& Param);
                /// @copydoc iEffect::GetRingModulatorParameters() const
                virtual RingModulatorParameters GetRingModulatorParameters() const;
                /// @copydoc iEffect::SetAutowahParameters(const AutowahParameters& Param)
                virtual void SetAutowahParameters(const AutowahParameters& Param);
                /// @copydoc iEffect::GetAutowahParameters() const
                virtual AutowahParameters GetAutowahParameters() const;
                /// @copydoc iEffect::SetCompressorParameters(const CompressorParameters& Param)
                virtual void SetCompressorParameters(const CompressorParameters& Param);
                /// @copydoc iEffect::GetCompressorParameters() const
                virtual CompressorParameters GetCompressorParameters() const;
                /// @copydoc iEffect::SetEqualizerParameters(const EqualizerParameters& Param)
                virtual void SetEqualizerParameters(const EqualizerParameters& Param);
                /// @copydoc iEffect::GetEqualizerParameters() const
                virtual EqualizerParameters GetEqualizerParameters() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Serialization

                /// @copydoc iEffect::ProtoSerialize(XML::Node&) const
                virtual void ProtoSerialize(XML::Node& ParentNode) const;
                /// @copydoc iEffect::ProtoDeSerialize(const XML::Node&)
                virtual void ProtoDeSerialize(const XML::Node& SelfRoot);

                /// @brief Get the name of the the XML tag the proxy class will leave behind as its instances are serialized.
                /// @return A string containing the name of this class.
                static String GetSerializableName();

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Methods

                /// @internal
                /// @brief Gets the internal Effect ID for this Effect.
                /// @return Returns a UInt32 representing this Effect in OALS.
                UInt32 _GetInternalEffect() const;
                /// @internal
                /// @brief Gets the internal Effect Slot ID for this Effect.
                /// @return Returns a UInt32 representing this Effect Slot in OALS.
                UInt32 _GetInternalEffectSlot() const;
                /// @internal
                /// @brief Checks to see if this Filter has been modified since last frame sounds were processed.
                /// @return Returns true if this Filter needs to be re-applied, false otherwise.
                Boole _IsDirty() const;
                /// @internal
                /// @brief Clears the dirty flag.
                void _Clean();
            };//Effect
        }//OALS
    }//Audio
}//Mezzanine

#endif

