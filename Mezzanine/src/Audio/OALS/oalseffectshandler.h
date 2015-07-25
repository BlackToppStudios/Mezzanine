// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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
#ifndef _audiooalseffectshandler_h
#define _audiooalseffectshandler_h

#include "Audio/effectshandler.h"

// OALS forward declares
#ifndef OALS_STRUCTS_DECLARED
struct ALCdevice;
#endif //OALS_STRUCTS_DECLARED

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            class Effect;
            class Filter;
            class EFXInterface;
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief This is the OpenALSoft implemenation for an Audio EffectsHandler.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB EffectsHandler : public iEffectsHandler
            {
            public:
                /// @brief The container type used for the storage of @ref OALS::iEffect instances in this handler.
                typedef std::vector<OALS::Effect*>                    EffectContainer;
                /// @brief Iterator type for @ref OALS::Effect instances stored by this class.
                typedef EffectContainer::iterator                     EffectIterator;
                /// @brief Const Iterator type for @ref  instances stored by this class.
                typedef EffectContainer::const_iterator               ConstEffectIterator;
                /// @brief The container type used for the storage of @ref OALS::iFilter instances in this handler.
                typedef std::vector<OALS::Filter*>                    FilterContainer;
                /// @brief Iterator type for @ref OALS::Filter instances stored by this class.
                typedef FilterContainer::iterator                     FilterIterator;
                /// @brief Const Iterator type for @ref OALS::Filter instances stored by this class.
                typedef FilterContainer::const_iterator               ConstFilterIterator;
                /// @brief The container type used for the storage of @ref EAXReverbParameters in this handler.
                typedef std::map<String,EAXReverbParameters>          EAXReverbParamContainer;
                /// @brief Iterator type for @ref EAXReverbParameters instances stored by this class.
                typedef EAXReverbParamContainer::iterator             EAXReverbParamIterator;
                /// @brief Const Iterator type for @ref EAXReverbParameters instances stored by this class.
                typedef EAXReverbParamContainer::const_iterator       ConstEAXReverbParamIterator;
                /// @brief The container type used for the storage of @ref ReverbParameters in this handler.
                typedef std::map<String,ReverbParameters>             ReverbParamContainer;
                /// @brief Iterator type for @ref ReverbParameters instances stored by this class.
                typedef ReverbParamContainer::iterator                ReverbParamIterator;
                /// @brief Const Iterator type for @ref ReverbParameters instances stored by this class.
                typedef ReverbParamContainer::const_iterator          ConstReverbParamIterator;
                /// @brief The container type used for the storage of @ref ChorusParameters in this handler.
                typedef std::map<String,ChorusParameters>             ChorusParamContainer;
                /// @brief Iterator type for @ref ChorusParameters instances stored by this class.
                typedef ChorusParamContainer::iterator                ChorusParamIterator;
                /// @brief Const Iterator type for @ref ChorusParameters instances stored by this class.
                typedef ChorusParamContainer::const_iterator          ConstChorusParamIterator;
                /// @brief The container type used for the storage of @ref DistortionParameters in this handler.
                typedef std::map<String,DistortionParameters>         DistortionParamContainer;
                /// @brief Iterator type for @ref DistortionParameters instances stored by this class.
                typedef DistortionParamContainer::iterator            DistortionParamIterator;
                /// @brief Const Iterator type for @ref DistortionParameters instances stored by this class.
                typedef DistortionParamContainer::const_iterator      ConstDistortionParamIterator;
                /// @brief The container type used for the storage of @ref EchoParameters in this handler.
                typedef std::map<String,EchoParameters>               EchoParamContainer;
                /// @brief Iterator type for @ref EchoParameters instances stored by this class.
                typedef EchoParamContainer::iterator                  EchoParamIterator;
                /// @brief Const Iterator type for @ref EchoParameters instances stored by this class.
                typedef EchoParamContainer::const_iterator            ConstEchoParamIterator;
                /// @brief The container type used for the storage of @ref FlangerParameters in this handler.
                typedef std::map<String,FlangerParameters>            FlangerParamContainer;
                /// @brief Iterator type for @ref FlangerParameters instances stored by this class.
                typedef FlangerParamContainer::iterator               FlangerParamIterator;
                /// @brief Const Iterator type for @ref FlangerParameters instances stored by this class.
                typedef FlangerParamContainer::const_iterator         ConstFlangerParamIterator;
                /// @brief The container type used for the storage of @ref FrequencyShiftParameters in this handler.
                typedef std::map<String,FrequencyShiftParameters>     FrequencyShiftParamContainer;
                /// @brief Iterator type for @ref FrequencyShiftParameters instances stored by this class.
                typedef FrequencyShiftParamContainer::iterator        FrequencyShiftParamIterator;
                /// @brief Const Iterator type for @ref FrequencyShiftParameters instances stored by this class.
                typedef FrequencyShiftParamContainer::const_iterator  ConstFrequencyShiftParamIterator;
                /// @brief The container type used for the storage of @ref VocalMorpherParameters in this handler.
                typedef std::map<String,VocalMorpherParameters>       VocalMorpherParamContainer;
                /// @brief Iterator type for @ref VocalMorpherParameters instances stored by this class.
                typedef VocalMorpherParamContainer::iterator          VocalMorpherParamIterator;
                /// @brief Const Iterator type for @ref VocalMorpherParameters instances stored by this class.
                typedef VocalMorpherParamContainer::const_iterator    ConstVocalMorpherParamIterator;
                /// @brief The container type used for the storage of @ref PitchShifterParameters in this handler.
                typedef std::map<String,PitchShifterParameters>       PitchShifterParamContainer;
                /// @brief Iterator type for @ref PitchShifterParameters instances stored by this class.
                typedef PitchShifterParamContainer::iterator          PitchShifterParamIterator;
                /// @brief Const Iterator type for @ref PitchShifterParameters instances stored by this class.
                typedef PitchShifterParamContainer::const_iterator    ConstPitchShifterParamIterator;
                /// @brief The container type used for the storage of @ref RingModulatorParameters in this handler.
                typedef std::map<String,RingModulatorParameters>      RingModulatorParamContainer;
                /// @brief Iterator type for @ref RingModulatorParameters instances stored by this class.
                typedef RingModulatorParamContainer::iterator         RingModulatorParamIterator;
                /// @brief Const Iterator type for @ref RingModulatorParameters instances stored by this class.
                typedef RingModulatorParamContainer::const_iterator   ConstRingModulatorParamIterator;
                /// @brief The container type used for the storage of @ref AutowahParameters in this handler.
                typedef std::map<String,AutowahParameters>            AutowahParamContainer;
                /// @brief Iterator type for @ref AutowahParameters instances stored by this class.
                typedef AutowahParamContainer::iterator               AutowahParamIterator;
                /// @brief Const Iterator type for @ref AutowahParameters instances stored by this class.
                typedef AutowahParamContainer::const_iterator         ConstAutowahParamIterator;
                /// @brief The container type used for the storage of @ref CompressorParameters in this handler.
                typedef std::map<String,CompressorParameters>         CompressorParamContainer;
                /// @brief Iterator type for @ref CompressorParameters instances stored by this class.
                typedef CompressorParamContainer::iterator            CompressorParamIterator;
                /// @brief Const Iterator type for @ref CompressorParameters instances stored by this class.
                typedef CompressorParamContainer::const_iterator      ConstCompressorParamIterator;
                /// @brief The container type used for the storage of @ref EqualizerParameters in this handler.
                typedef std::map<String,EqualizerParameters>          EqualizerParamContainer;
                /// @brief Iterator type for @ref EqualizerParameters instances stored by this class.
                typedef EqualizerParamContainer::iterator             EqualizerParamIterator;
                /// @brief Const Iterator type for @ref EqualizerParameters instances stored by this class.
                typedef EqualizerParamContainer::const_iterator       ConstEqualizerParamIterator;
            protected:
                /// @internal
                /// @brief Pointer to the class holding function pointers to all the OALS EFX methods
                EFXInterface* EFX;
                /// @internal
                /// @brief Container responsible for the storage of @ref iEffect instances.
                EffectContainer Effects;
                /// @internal
                /// @brief Container responsible for the storage of @ref iFilter instances.
                FilterContainer Filters;
                /// @internal
                /// @brief Container responsible for the storage of @ref EAXReverbParameters.
                EAXReverbParamContainer EAXReverbParams;
                /// @internal
                /// @brief Container responsible for the storage of @ref ReverbParameters.
                ReverbParamContainer ReverbParams;
                /// @internal
                /// @brief Container responsible for the storage of @ref ChorusParameters.
                ChorusParamContainer ChorusParams;
                /// @internal
                /// @brief Container responsible for the storage of @ref DistortionParameters.
                DistortionParamContainer DistortionParams;
                /// @internal
                /// @brief Container responsible for the storage of @ref EchoParameters.
                EchoParamContainer EchoParams;
                /// @internal
                /// @brief Container responsible for the storage of @ref FlangerParameters.
                FlangerParamContainer FlangerParams;
                /// @internal
                /// @brief Container responsible for the storage of @ref FrequencyShiftParameters.
                FrequencyShiftParamContainer FrequencyShiftParams;
                /// @internal
                /// @brief Container responsible for the storage of @ref VocalMorpherParameters.
                VocalMorpherParamContainer VocalMorpherParams;
                /// @internal
                /// @brief Container responsible for the storage of @ref PitchShifterParameters.
                PitchShifterParamContainer PitchShifterParams;
                /// @internal
                /// @brief Container responsible for the storage of @ref RingModulatorParameters.
                RingModulatorParamContainer RingModulatorParams;
                /// @internal
                /// @brief Container responsible for the storage of @ref AutowahParameters.
                AutowahParamContainer AutowahParams;
                /// @internal
                /// @brief Container responsible for the storage of @ref CompressorParameters.
                CompressorParamContainer CompressorParams;
                /// @internal
                /// @brief Container responsible for the storage of @ref EqualizerParameters.
                EqualizerParamContainer EqualizerParams;
            public:
                /// @brief Class constructor.
                /// @param Device The playback device selected and will enable effects on.
                EffectsHandler(ALCdevice* Device);
                /// @brief Class destructor.
                virtual ~EffectsHandler();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                ///////////////////////////////////////////////////////////////////////////////
                // Effect Handling

                /// @copydoc iEffectsHandler::IsEffectSupported(const EffectType Type) const
                virtual Boole IsEffectSupported(const EffectType Type) const;
                /// @copydoc iEffectsHandler::CreateEffect()
                virtual iEffect* CreateEffect();
                /// @copydoc iEffectsHandler::GetEffect(const UInt32 Index) const
                virtual iEffect* GetEffect(const UInt32 Index) const;
                /// @copydoc iEffectsHandler::GetNumEffects() const
                virtual UInt32 GetNumEffects() const;
                /// @copydoc iEffectsHandler::DestroyEffect(iEffect* ToBeDestroyed)
                virtual void DestroyEffect(iEffect* ToBeDestroyed);
                /// @copydoc iEffectsHandler::DestroyAllEffects()
                virtual void DestroyAllEffects();

                ///////////////////////////////////////////////////////////////////////////////
                // Filter Handling

                /// @copydoc iEffectsHandler::IsFilterSupported(const FilterType Type) const
                virtual Boole IsFilterSupported(const FilterType Type) const;
                /// @copydoc iEffectsHandler::CreateFilter()
                virtual iFilter* CreateFilter();
                /// @copydoc iEffectsHandler::GetFilter(const UInt32 Index) const
                virtual iFilter* GetFilter(const UInt32 Index) const;
                /// @copydoc iEffectsHandler::GetNumFilters() const
                virtual UInt32 GetNumFilters() const;
                /// @copydoc iEffectsHandler::DestroyFilter(iFilter* ToBeDestroyed)
                virtual void DestroyFilter(iFilter* ToBeDestroyed);
                /// @copydoc iEffectsHandler::DestroyAllFilters()
                virtual void DestroyAllFilters();

                ///////////////////////////////////////////////////////////////////////////////
                // Preset Handling

                /// @copydoc iEffectsHandler::EffectPresetExists(const EffectType Type, const String& Name)
                virtual Boole EffectPresetExists(const EffectType Type, const String& Name);
                /// @copydoc iEffectsHandler::RemoveEffectPreset(const EffectType Type, const String& Name)
                virtual void RemoveEffectPreset(const EffectType Type, const String& Name);
                /// @copydoc iEffectsHandler::RemoveAllEffectPresets(const EffectType Type)
                virtual void RemoveAllEffectPresets(const EffectType Type);

                ///////////////////////////////////////////////////////////////////////////////
                // Individual effect type settings

                /// @copydoc iEffectsHandler::AddEAXReverbEffectPreset(const String& Name, const EAXReverbParameters& Setting)
                virtual void AddEAXReverbEffectPreset(const String& Name, const EAXReverbParameters& Setting);
                /// @copydoc iEffectsHandler::GetEAXReverbEffectPreset(const String& Name)
                virtual EAXReverbParameters GetEAXReverbEffectPreset(const String& Name) const;

                /// @copydoc iEffectsHandler::AddReverbEffectPreset(const String& Name, const ReverbParameters& Setting)
                virtual void AddReverbEffectPreset(const String& Name, const ReverbParameters& Setting);
                /// @copydoc iEffectsHandler::GetReverbEffectPreset(const String& Name)
                virtual ReverbParameters GetReverbEffectPreset(const String& Name) const;

                /// @copydoc iEffectsHandler::AddChorusEffectPreset(const String& Name, const ChorusParameters& Setting)
                virtual void AddChorusEffectPreset(const String& Name, const ChorusParameters& Setting);
                /// @copydoc iEffectsHandler::GetChorusEffectPreset(const String& Name)
                virtual ChorusParameters GetChorusEffectPreset(const String& Name) const;

                /// @copydoc iEffectsHandler::AddDistortionEffectPreset(const String& Name, const DistortionParameters& Setting)
                virtual void AddDistortionEffectPreset(const String& Name, const DistortionParameters& Setting);
                /// @copydoc iEffectsHandler::GetDistortionEffectPreset(const String& Name)
                virtual DistortionParameters GetDistortionEffectPreset(const String& Name) const;

                /// @copydoc iEffectsHandler::AddEchoEffectPreset(const String& Name, const EchoParameters& Setting)
                virtual void AddEchoEffectPreset(const String& Name, const EchoParameters& Setting);
                /// @copydoc iEffectsHandler::GetEchoEffectPreset(const String& Name)
                virtual EchoParameters GetEchoEffectPreset(const String& Name) const;

                /// @copydoc iEffectsHandler::AddFlangerEffectPreset(const String& Name, const FlangerParameters& Setting)
                virtual void AddFlangerEffectPreset(const String& Name, const FlangerParameters& Setting);
                /// @copydoc iEffectsHandler::GetFlangerEffectPreset(const String& Name)
                virtual FlangerParameters GetFlangerEffectPreset(const String& Name) const;

                /// @copydoc iEffectsHandler::AddFrequencyShiftEffectPreset(const String& Name, const FrequencyShiftParameters& Setting)
                virtual void AddFrequencyShiftEffectPreset(const String& Name, const FrequencyShiftParameters& Setting);
                /// @copydoc iEffectsHandler::GetFrequencyShiftEffectPreset(const String& Name)
                virtual FrequencyShiftParameters GetFrequencyShiftEffectPreset(const String& Name) const;

                /// @copydoc iEffectsHandler::AddVocalMorpherEffectPreset(const String& Name, const VocalMorpherParameters& Setting)
                virtual void AddVocalMorpherEffectPreset(const String& Name, const VocalMorpherParameters& Setting);
                /// @copydoc iEffectsHandler::GetVocalMorpherEffectPreset(const String& Name)
                virtual VocalMorpherParameters GetVocalMorpherEffectPreset(const String& Name) const;

                /// @copydoc iEffectsHandler::AddPitchShifterEffectPreset(const String& Name, const PitchShifterParameters& Setting)
                virtual void AddPitchShifterEffectPreset(const String& Name, const PitchShifterParameters& Setting);
                /// @copydoc iEffectsHandler::GetPitchShifterEffectPreset(const String& Name)
                virtual PitchShifterParameters GetPitchShifterEffectPreset(const String& Name) const;

                /// @copydoc iEffectsHandler::AddRingModulatorEffectPreset(const String& Name, const RingModulatorParameters& Setting)
                virtual void AddRingModulatorEffectPreset(const String& Name, const RingModulatorParameters& Setting);
                /// @copydoc iEffectsHandler::GetRingModulatorEffectPreset(const String& Name)
                virtual RingModulatorParameters GetRingModulatorEffectPreset(const String& Name) const;

                /// @copydoc iEffectsHandler::AddAutowahEffectPreset(const String& Name, const AutowahParameters& Setting)
                virtual void AddAutowahEffectPreset(const String& Name, const AutowahParameters& Setting);
                /// @copydoc iEffectsHandler::GetAutowahEffectPreset(const String& Name)
                virtual AutowahParameters GetAutowahEffectPreset(const String& Name) const;

                /// @copydoc iEffectsHandler::AddCompressorEffectPreset(const String& Name, const CompressorParameters& Setting)
                virtual void AddCompressorEffectPreset(const String& Name, const CompressorParameters& Setting);
                /// @copydoc iEffectsHandler::GetCompressorEffectPreset(const String& Name)
                virtual CompressorParameters GetCompressorEffectPreset(const String& Name) const;

                /// @copydoc iEffectsHandler::AddEqualizerEffectPreset(const String& Name, const EqualizerParameters& Setting)
                virtual void AddEqualizerEffectPreset(const String& Name, const EqualizerParameters& Setting);
                /// @copydoc iEffectsHandler::GetEqualizerEffectPreset(const String& Name)
                virtual EqualizerParameters GetEqualizerEffectPreset(const String& Name) const;

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Methods

                /// @internal
                /// @brief Gets a pointer to the class holding all the EFX function pointers.
                /// @return Returns a pointer to the @ref EFXInterface.
                virtual EFXInterface* _GetEFXInterface() const;
                /// @internal
                /// @brief Removes the dirty flag from all @ref iEffect instances and @ref iFilter instances.
                virtual void _CleanAll();
            };//EffectsHandler
        }//OALS
    }//Audio
}//Mezzanine

#endif
