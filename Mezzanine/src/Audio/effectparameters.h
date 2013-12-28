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
#ifndef _audioeffectparameters_h
#define _audioeffectparameters_h

#include "vector3.h"
#include "Audio/audioenumerations.h"

namespace Mezzanine
{
    namespace Audio
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a struct containing all the parameters needed to describe an EAX Reverb effect.
        /// @details The Reverb effect simulates sounds that bounce off of surfaces and are reflected back at the listener.
        ///////////////////////////////////////
        struct EAXReverbParameters
        {
            /// @brief The Reverb Modal Density controls the coloration of the late reverb.
            /// @remarks Lowering the value adds more coloration to the late reverb.  @n @n Range: 0.0 to 1.0
            Real Density;
            /// @brief The Reverb Diffusion property controls the echo density in the reverberation decay.
            /// @remarks It's set by default to 1.0, which provides the highest density. Reducing diffusion gives the reverberation a
            /// more "grainy" character that is especially noticeable with percussive sound sources. If you set a diffusion value of
            /// 0.0, the later reverberation sounds like a succession of distinct echoes.  @n @n Range: 0.0 to 1.0
            Real Diffusion;
            /// @brief The master volume control for the reflected sound.
            /// @remarks The Reverb Gain property is the master volume control for the reflected sound (both early reflections and reverberation)
            /// that the reverb effect adds to all sound sources. It sets the maximum amount of reflections and reverberation added to the final
            /// sound mix. The value of the Reverb Gain property ranges from 1.0 (0db) (the maximum amount) to 0.0 (-100db) (no reflected sound
            /// at all).  @n @n Range: 0.0 to 1.0
            Real Gain;
            /// @brief The Reverb Gain High Frequency property.
            /// @remarks The Reverb Gain HF property further tweaks reflected sound by attenuating it at high frequencies.  It controls a low-pass
            /// filter that applies globally to the reflected sound of all sound sources feeding the particular instance of the reverb effect. The
            /// value of the Reverb Gain HF property ranges from 1.0 (0db) (no filter) to 0.0 (-100db) (virtually no reflected sound). HF Reference
            /// sets the frequency at which the value of this property is measured.  @n @n Range: 0.0 to 1.0
            Real GainHF;
            /// @brief The Reverb Gain Low Frequency property.
            /// @remarks The Reverb Gain LF property further tweaks reflected sound by attenuating it at low frequencies.  It controls a high-pass
            /// filter that applies globally to the reflected sound of all sound sources feeding the particular instance of the reverb effect.
            /// The value of the Reverb Gain LF property ranges from 1.0 (0db) (no filter) to 0.0 (-100db) (virtually no reflected sound). LF
            /// Reference sets the frequency at which the value of this property is measured.  @n @n Range: 0.0 to 1.0
            Real GainLF;
            /// @brief The Decay Time property sets the reverberation decay time.
            /// @remarks It ranges from 0.1 (typically a small room with very dead surfaces) to 20.0 (typically a large room with very live
            /// surfaces).  @n @n Range: 0.1 to 20.0
            Real DecayTime;
            /// @brief The Decay HF Ratio property adjusts the spectral quality of the Decay Time parameter.
            /// @remarks It is the ratio of high-frequency decay time relative to the time set by Decay Time. The Decay HF Ratio value 1.0 is
            /// neutral: the decay time is equal for all frequencies. As Decay HF Ratio increases above 1.0, the high-frequency decay time
            /// increases so it's longer than the decay time at mid frequencies. You hear a more brilliant reverberation with a longer decay at
            /// high frequencies. As the Decay HF Ratio value decreases below 1.0, the high-frequency decay time decreases so it's shorter than
            /// the decay time of the mid frequencies. You hear a more natural reverberation.  @n @n Range: 0.1 to 20.0
            Real DecayHFRatio;
            /// @brief The Decay LF Ratio property adjusts the spectral quality of the Decay Time parameter.
            /// @remarks It is the ratio of low-frequency decay time relative to the time set by Decay Time. The Decay LF Ratio value 1.0 is
            /// neutral: the decay time is equal for all frequencies. As Decay LF Ratio increases above 1.0, the low-frequency decay time
            /// increases so it's longer than the decay time at mid frequencies. You hear a more booming reverberation with a longer decay at
            /// low frequencies. As the Decay LF Ratio value decreases below 1.0, the low-frequency decay time decreases so it's shorter than
            /// the decay time of the mid frequencies. You hear a more tinny reverberation.  @n @n Range: 0.1 to 20.0
            Real DecayLFRatio;
            /// @brief The Reflections Gain property controls the overall amount of initial reflections relative to the Gain property.
            /// @remarks The Gain property sets the overall amount of reflected sound: both initial reflections and later reverberation.
            /// The value of Reflections Gain ranges from a maximum of 3.16 (+10 dB) to a minimum of 0.0 (-100 dB) (no initial reflections
            /// at all), and is corrected by the value of the Gain property. The Reflections Gain property does not affect the subsequent
            /// reverberation decay.  @n @n Range: 0.0 to 3.16
            Real ReflectionsGain;
            /// @brief The Reflections Delay property is the amount of delay between the arrival time of the direct path from the source to the first reflection from the source.
            /// @remarks It ranges from 0 to 300 milliseconds. You can reduce or increase Reflections Delay to simulate closer or more distant
            /// reflective surfaces and therefore control the perceived size of the room.  @n @n Range: 0.0 to 0.3
            Real ReflectionsDelay;
            /// @brief The Reflections Pan property is a 3D vector that controls the spatial distribution of the cluster of early reflections.
            /// @remarks The direction of this vector controls the global direction of the reflections, whileits magnitude controls how focused
            /// the reflections are towards this direction. @n @n
            /// It is important to note that the direction of the vector is interpreted in the
            /// coordinate system of the user, without taking into account the orientation of the virtual listener. For instance, assuming a
            /// four-point loudspeaker playback system, setting Reflections Pan to (0, 0, 0.7) means that the reflections are panned to the front
            /// speaker pair, whereas as setting of (0, 0, -0.7) pans the reflections towards the rear speakers. These vectors follow the a
            /// left-handed co-ordinate system, unlike OpenAL uses a right-handed co-ordinate system. @n @n
            /// If the magnitude of Reflections Pan is zero (the default setting), the early reflections come evenly from all directions. As the
            /// magnitude increases, the reflections become more focused in the direction pointed to by the vector. A magnitude of 1.0 would
            /// represent the extreme case, where all reflections come from a single direction.
            Vector3 ReflectionsPan;
            /// @brief The Late Reverb Gain property controls the overall amount of later reverberation relative to the Gain property.
            /// @remarks The Gain property sets the overall amount of both initial reflections and later reverberation. The value of Late Reverb
            /// Gain ranges from a maximum of 10.0 (+20 dB) to a minimum of 0.0 (-100 dB) (no late reverberation at all).  @n @n Range: 0.0 to 10.0
            Real LateReverbGain;
            /// @brief The Late Reverb Delay property defines the begin time of the late reverberation relative to the time of the initial reflection.
            /// @remarks It ranges from 0 to 100 milliseconds. Reducing or increasing Late Reverb Delay is useful for simulating a smaller or larger
            /// room.  @n @n Range: 0.0 to 0.1
            Real LateReverbDelay;
            /// @brief The Late Reverb Pan property is a 3D vector that controls the spatial distribution of the late reverb.
            /// @remarks The direction of this vector controls the global direction of the reverb, while its magnitude controls how focused the reverb
            /// are towards this direction. The details under Reflections Pan, above, also apply to Late Reverb Pan.
            Vector3 LateReverbPan;
            /// @brief Echo Time controls the rate at which the cyclic echo repeats itself along the reverberation decay.
            /// @remarks For example, the default setting for Echo Time is 250 ms. causing the echo to occur 4 times per second. Therefore, if you were
            /// to clap your hands in this type of environment, you will hear four repetitions of clap per second.  @n @n Range: 0.075 to 0.25
            Real EchoTime;
            /// @brief Echo Depth introduces a cyclic echo in the reverberation decay, which will be noticeable with transient or percussive sounds.
            /// @remarks A larger value of Echo Depth will make this effect more prominent. Together with Reverb Diffusion, Echo Depth will control how
            /// long the echo effect will persist along the reverberation decay. In a more diffuse environment, echoes will wash out more quickly after
            /// the direct sound. In an environment that is less diffuse, you will be able to hear a larger number of repetitions of the echo, which
            /// will wash out later in the reverberation decay. If Diffusion is set to 0.0 and Echo Depth is set to 1.0, the echo will persist distinctly
            /// until the end of the reverberation decay.  @n @n Range: 0.0 to 1.0
            Real EchoDepth;
            /// @brief Modulation Time controls the amount of time to take when applying the set Modulation Depth.
            /// @remarks This will be most noticeable applied to sources that have tonal color or pitch. You can use this to make some trippy effects!
            /// Modulation Time controls the speed of the vibrato (rate of periodic changes in pitch).  @n @n
            /// Using this property and ModulationDepth, you can create a pitch modulation in the reverberant sound. @n @n Range: 0.004 to 4.0
            Real ModulationTime;
            /// @brief Modulation Depth controls the amount of pitch change.
            /// @remarks Low values of Diffusion will contribute to reinforcing the perceived effect by reducing the mixing of overlapping reflections in the
            /// reverberation decay.  @n @n
            /// Using this property and ModulationTime, you can create a pitch modulation in the reverberant sound. @n @n Range: 0.0 to 1.0
            Real ModulationDepth;
            /// @brief The Air Absorption Gain HF property controls the distance-dependent attenuation at high frequencies caused by the propagation medium.
            /// @remarks It applies to reflected sound only. You can use Air Absorption Gain HF to simulate sound transmission through foggy air, dry air,
            /// smoky atmosphere, and so on. The default value is 0.994 (-0.05 dB) per meter, which roughly corresponds to typical condition of atmospheric
            /// humidity, temperature, and so on. Lowering the value simulates a more absorbent medium (more humidity in the air, for example); raising the
            /// value simulates a less absorbent medium (dry desert air, for example).  @n @n Range: 0.892 to 1.0
            Real AirAbsorptionGainHF;
            /// @brief Determines the frequency at which high-frequency effects created by other properties are measured, such as @ref EAXReverbParameters::DecayHFRatio.
            /// @remarks Note that it is necessary to maintain a factor of at least 10 between this property and @ref EAXReverbParameters::LFReference so that low frequency
            /// and high-frequency properties can be accurately controlled and will produce independent effects.  @n @n Range: 1000.0 to 20000.0
            Real HFReference;
            /// @brief Determines the frequency at which low-frequency effects created by other properties are measured, such as @ref EAXReverbParameters::DecayLFRatio.
            /// @remarks Note that it is necessary to maintain a factor of at least 10 between this property and @ref EAXReverbParameters::HFReference so that low frequency
            /// and high-frequency properties can be accurately controlled and will produce independent effects.  @n @n Range: 20.0 to 1000.0
            Real LFReference;
            /// @brief The Room Rolloff Factor property is one of two methods available to attenuate the reflected sound according to source-listener distance.
            /// @remarks It's defined the same way as OpenAL's Rolloff Factor, but operates on reverb sound instead of direct-path sound. Setting the Room
            /// Rolloff Factor value to 1.0 specifies that the reflected sound will decay by 6 dB every time the distance doubles. Any value other than 1.0 is
            /// equivalent to a scaling factor applied to the quantity specified by ((Source listener distance) - (Reference Distance)). Reference Distance is
            /// an OpenAL source parameter that specifies the inner border for distance rolloff effects: if the source comes closer to the listener than the
            /// reference distance, the direct-path sound isn't increased as the source comes closer to the listener, and neither is the reflected sound.  @n @n
            /// The default value of Room Rolloff Factor is 0.0 because, by default, the Effects Extension reverb effect naturally manages the reflected sound
            /// level automatically for each sound source to simulate the natural rolloff of reflected sound vs. distance in typical rooms.  @n @n Range: 0.0 to 10.0
            Real RoomRolloffFactor;
            /// @brief Whether or not to limit high-frequency decay.
            /// @remarks When this flag is set, the high-frequency decay time automatically stays below a limit value that's derived from the setting of the
            /// property Air Absorption Gain HF. This limit applies regardless of the setting of the property Decay HF Ratio, and the limit doesn't affect
            /// the value of Decay HF Ratio. This limit, when on, maintains a natural sounding reverberation decay by allowing you to increase the value of
            /// Decay Time without the risk of getting an unnaturally long decay time at high frequencies. If this flag is set to false, high-frequency decay
            /// time isn't automatically limited.
            bool DecayHFLimit;

            /// @brief Struct constructor.
            /// @param density @copybrief EAXReverbParameters::Density
            /// @param diffusion @copybrief EAXReverbParameters::Diffusion
            /// @param gain @copybrief EAXReverbParameters::Gain
            /// @param gainHF @copybrief EAXReverbParameters::GainHF
            /// @param gainLF @copybrief EAXReverbParameters::GainLF
            /// @param decayTime @copybrief EAXReverbParameters::DecayTime
            /// @param decayHFRatio @copybrief EAXReverbParameters::DecayHFRatio
            /// @param decayLFRatio @copybrief EAXReverbParameters::DecayLFRatio
            /// @param reflectionsGain @copybrief EAXReverbParameters::ReflectionsGain
            /// @param reflectionsDelay @copybrief EAXReverbParameters::ReflectionsDelay
            /// @param reflectionsPan @copybrief EAXReverbParameters::ReflectionsPan
            /// @param lateReverbGain @copybrief EAXReverbParameters::LateReverbGain
            /// @param lateReverbDelay @copybrief EAXReverbParameters::LateReverbDelay
            /// @param lateReverbPan @copybrief EAXReverbParameters::LateReverbPan
            /// @param echoTime @copybrief EAXReverbParameters::EchoTime
            /// @param echoDepth @copybrief EAXReverbParameters::EchoDepth
            /// @param modulationTime @copybrief EAXReverbParameters::ModulationTime
            /// @param modulationDepth @copybrief EAXReverbParameters::ModulationDepth
            /// @param airAbsorptionGainHF @copybrief EAXReverbParameters::AirAbsorptionGainHF
            /// @param hFReference @copybrief EAXReverbParameters::HFReference
            /// @param lFReference @copybrief EAXReverbParameters::LFReference
            /// @param roomRolloffFactor @copybrief EAXReverbParameters::RoomRolloffFactor
            /// @param decayHFLimit @copybrief EAXReverbParameters::DecayHFLimit
            EAXReverbParameters(
                Real density = 1.0f,
                Real diffusion = 1.0f,
                Real gain = 0.32f,
                Real gainHF = 0.89f,
                Real gainLF = 0.0f,
                Real decayTime = 1.49f,
                Real decayHFRatio = 0.83f,
                Real decayLFRatio = 1.0f,
                Real reflectionsGain = 0.05f,
                Real reflectionsDelay = 0.007f,
                Vector3 reflectionsPan = Vector3(0.0f, 0.0f, 0.0f),
                Real lateReverbGain = 1.26f,
                Real lateReverbDelay = 0.011f,
                Vector3 lateReverbPan = Vector3(0.0f, 0.0f, 0.0f),
                Real echoTime = 0.25f,
                Real echoDepth = 0.0f,
                Real modulationTime = 0.25f,
                Real modulationDepth = 0.0f,
                Real airAbsorptionGainHF = 0.994f,
                Real hFReference = 5000.0f,
                Real lFReference = 250.0f,
                Real roomRolloffFactor = 0.0f,
                bool decayHFLimit = true) :
                Density(density), Diffusion(diffusion), Gain(gain), GainHF(gainHF), GainLF(gainLF),
                DecayTime(decayTime), DecayHFRatio(decayHFRatio), DecayLFRatio(decayLFRatio),
                ReflectionsGain(reflectionsGain), ReflectionsDelay(reflectionsDelay),
                ReflectionsPan(reflectionsPan), LateReverbGain(lateReverbGain),
                LateReverbDelay(lateReverbDelay), LateReverbPan(lateReverbPan),
                EchoTime(echoTime), EchoDepth(echoDepth),ModulationTime(modulationTime),
                ModulationDepth(modulationDepth), AirAbsorptionGainHF(airAbsorptionGainHF),
                HFReference(hFReference), LFReference(lFReference),
                RoomRolloffFactor(roomRolloffFactor), DecayHFLimit(decayHFLimit) { }
        };//EAXReverbParameters

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a struct containing all the parameters needed to describe a Reverb effect.
        /// @details This is similar in purpose to the @ref EAXReverbParameters struct, but has fewer parameters/features enabling it
        /// to run on more limited hardware.  Using this may be a good fallback option for audio reverb effects.
        ///////////////////////////////////////
        struct ReverbParameters
        {
            /// @brief Reverb Modal Density controls the coloration of the late reverb.
            /// @remarks Lowering the value adds more coloration to the late reverb.  @n @n Range: 0.0 to 1.0
            Real Density;
            /// @brief The Reverb Diffusion property controls the echo density in the reverberation decay.
            /// @remarks It's set by default to 1.0, which provides the highest density. Reducing diffusion gives the reverberation a more "grainy" character
            /// that is especially noticeable with percussive sound sources. If you set a diffusion value of 0.0, the later reverberation sounds like a
            /// succession of distinct echoes.  @n @n Range: 0.0 to 1.0
            Real Diffusion;
            /// @brief The Reverb Gain property is the master volume control for the reflected sound that the reverb effect adds to all sound sources.
            /// @remarks It sets the maximum amount of reflections and reverberation added to the final sound mix. The value of the Reverb Gain property
            /// ranges from 1.0 (0db) (the maximum amount) to 0.0 (-100db) (no reflected sound at all).  @n @n Range: 0.0 to 1.0
            Real Gain;
            /// @brief The Reverb Gain HF property further tweaks reflected sound by attenuating it at high frequencies.
            /// @remarks It controls a low-pass filter that applies globally to the reflected sound of all sound sources feeding the particular instance of
            /// the reverb effect. The value of the Reverb Gain HF property ranges from 1.0 (0db) (no filter) to 0.0 (-100db) (virtually no reflected sound).
            /// @n @n Range: 0.0 to 1.0
            Real GainHF;
            /// @brief The Decay Time property sets the reverberation decay time.
            /// @remarks It ranges from 0.1 (typically a small room with very dead surfaces) to 20.0 (typically a large room with very live surfaces).
            /// @n @n Range: 0.1 to 20.0
            Real DecayTime;
            /// @brief The Decay HF Ratio property sets the spectral quality of the Decay Time parameter.
            /// @remarks It is the ratio of high-frequency decay time relative to the time set by Decay Time. The Decay HF Ratio value 1.0 is neutral: the
            /// decay time is equal for all frequencies. As Decay HF Ratio increases above 1.0, the high-frequency decay time increases so it's longer than
            /// the decay time at low frequencies. You hear a more brilliant reverberation with a longer decay at high frequencies. As the Decay HF Ratio
            /// value decreases below 1.0, the high-frequency decay time decreases so it's shorter than the decay time of the low frequencies. You hear a
            /// more natural reverberation.  @n @n Range: 0.1 to 2.0
            Real DecayHFRatio;
            /// @brief The Reflections Gain property controls the overall amount of initial reflections relative to the Gain property.
            /// @remarks The Gain property sets the overall amount of reflected sound: both initial reflections and later reverberation. The value of Reflections
            /// Gain ranges from a maximum of 3.16 (+10 dB) to a minimum of 0.0 (-100 dB) (no initial reflections at all), and is corrected by the value of the Gain
            /// property. The Reflections Gain property does not affect the subsequent reverberation decay.  @n @n Range: 0.0 to 3.16
            Real ReflectionsGain;
            /// @brief The Reflections Delay property is the amount of delay between the arrival time of the direct path from the source to the first reflection from the source.
            /// @remarks It ranges from 0 to 300 milliseconds. You can reduce or increase Reflections Delay to simulate closer or more distant reflective surfaces and
            /// therefore control the perceived size of the room.  @n @n Range: 0.0 to 0.3
            Real ReflectionsDelay;
            /// @brief The Late Reverb Gain property controls the overall amount of later reverberation relative to the Gain property.
            /// @remarks The Gain property sets the overall amount of both initial reflections and later reverberation. The value of Late Reverb Gain ranges
            /// from a maximum of 10.0 (+20 dB) to a minimum of 0.0 (-100 dB) (no late reverberation at all).  @n @n Range: 0.0 to 10.0
            Real LateReverbGain;
            /// @brief The Late Reverb Delay property defines the begin time of the late reverberation relative to the time of the initial reflection.
            /// @remarks It ranges from 0 to 100 milliseconds. Reducing or increasing Late Reverb Delay is useful for simulating a smaller or larger room.
            /// @n @n Range: 0.0 to 0.1
            Real LateReverbDelay;
            /// @brief The Air Absorption Gain HF property controls the distance-dependent attenuation at high frequencies caused by the propagation medium.
            /// @remarks It applies to reflected sound only. You can use Air Absorption Gain HF to simulate sound transmission through foggy air, dry air, smoky
            /// atmosphere, and so on. The default value is 0.994 (-0.05 dB) per meter, which roughly corresponds to typical condition of atmospheric humidity,
            /// temperature, and so on. Lowering the value simulates a more absorbent medium (more humidity in the air, for example); raising the value simulates
            /// a less absorbent medium (dry desert air, for example).  @n @n Range: 0.892 to 1.0
            Real AirAbsorptionGainHF;
            /// @brief The Room Rolloff Factor property is one of two methods available to attenuate the reflected sound according to source-listener distance.
            /// @remarks It's defined the same way as OpenAL's Rolloff Factor, but operates on reverb sound instead of direct-path sound. Setting the Room Rolloff
            /// Factor value to 1.0 specifies that the reflected sound will decay by 6 dB every time the distance doubles. Any value other than 1.0 is equivalent to a
            /// scaling factor applied to the quantity specified by ((Source listener distance) - (Reference Distance)). Reference Distance is an OpenAL source
            /// parameter that specifies the inner border for distance rolloff effects: if the source comes closer to the listener than the reference distance, the
            /// direct-path sound isn't increased as the source comes closer to the listener, and neither is the reflected sound.  @n @n
            /// The default value of Room Rolloff Factor is 0.0 because, by default, the Effects Extension reverb effect naturally manages the reflected sound level
            /// automatically for each sound source to simulate the natural rolloff of reflected sound vs. distance in typical rooms.  @n @n Range: 0.0 to 10.0
            Real RoomRolloffFactor;
            /// @brief Whether or not to limit high-frequency decay.
            /// @remarks When this flag is set, the high-frequency decay time automatically stays below a limit value that's derived from the setting of the property
            /// Air Absorption Gain HF. This limit applies regardless of the setting of the property Decay HF Ratio, and the limit doesn't affect the value of Decay HF
            /// Ratio. This limit, when on, maintains a natural sounding reverberation decay by allowing you to increase the value of Decay Time without the risk of
            /// getting an unnaturally long decay time at high frequencies. If this flag is set to false, high-frequency decay time isn't automatically limited.
            bool DecayHFLimit;

            /// @brief Struct constructor.
            /// @param density @copybrief ReverbParameters::Density
            /// @param diffusion @copybrief ReverbParameters::Diffusion
            /// @param gain @copybrief ReverbParameters::Gain
            /// @param gainHF @copybrief ReverbParameters::GainHF
            /// @param decayTime @copybrief ReverbParameters::DecayTime
            /// @param decayHFRatio @copybrief ReverbParameters::DecayHFRatio
            /// @param reflectionsGain @copybrief ReverbParameters::ReflectionsGain
            /// @param reflectionsDelay @copybrief ReverbParameters::ReflectionsDelay
            /// @param lateReverbGain @copybrief ReverbParameters::LateReverbGain
            /// @param lateReverbDelay @copybrief ReverbParameters::LateReverbDelay
            /// @param airAbsorptionGainHF @copybrief ReverbParameters::AirAbsorptionGainHF
            /// @param roomRolloffFactor @copybrief ReverbParameters::RoomRolloffFactor
            /// @param decayHFLimit @copybrief ReverbParameters::DecayHFLimit
            ReverbParameters(
                Real density = 1.0f,
                Real diffusion = 1.0f,
                Real gain = 0.32f,
                Real gainHF = 0.89f,
                Real decayTime = 1.49f,
                Real decayHFRatio = 0.83f,
                Real reflectionsGain = 0.05f,
                Real reflectionsDelay = 0.007f,
                Real lateReverbGain = 1.26f,
                Real lateReverbDelay = 0.011f,
                Real airAbsorptionGainHF = 0.994f,
                Real roomRolloffFactor = 0.0f,
                bool decayHFLimit = true) :
                Density(density), Diffusion(diffusion), Gain(gain), GainHF(gainHF),
                DecayTime(decayTime), DecayHFRatio(decayHFRatio),
                ReflectionsGain(reflectionsGain), ReflectionsDelay(reflectionsDelay),
                LateReverbGain(lateReverbGain), LateReverbDelay(lateReverbDelay),
                AirAbsorptionGainHF(airAbsorptionGainHF), RoomRolloffFactor(roomRolloffFactor),
                DecayHFLimit(decayHFLimit) { }
        };//ReverbParameters

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a struct containing all the parameters needed to describe a Chorus effect.
        /// @details The chorus effect essentially replays the input audio accompanied by another slightly delayed version of the signal, creating a "doubling" effect.
        ///////////////////////////////////////
        struct ChorusParameters
        {
            /// @enum ChorusWaveform
            /// @brief The shape of the wave if it were viewed on an oscillator.
            enum ChorusWaveform
            {
                ECW_Sinusoid,   ///< A curved oscillation with smoother transitions at peak frequencies.
                ECW_Triangle    ///< A linear oscillation with rapid transitions at peak frequencies.
            };

            /// @brief This property sets the waveform shape of the Low Frequency Oscillation that controls the delay time of the delayed signals.
            ChorusWaveform Waveform;
            /// @brief This property controls the phase difference between the left and rightLow Frequency Oscillation's.
            /// @remarks At zero degrees the two LFOs are synchronized. Use this parameter to create the illusion of an expanded stereo field of the output signal.
            /// @n @n Range: -180 to 180
            Integer Phase;
            /// @brief This property sets the modulation rate of the Low Frequency Oscillation that controls the delay time of the delayed signals.
            /// @remarks Range: 0.0 to 10.0
            Real Rate;
            /// @brief This property controls the amount by which the delay time is modulated by the Low Frequency Oscillation.
            /// @remarks Range: 0.0 to 1.0
            Real Depth;
            /// @brief This property controls the amount of processed signal that is fed back to the input of the chorus effect.
            /// @remarks Negative values will reverse the phase of the feedback signal. At full magnitude the identical sample will repeat endlessly. At lower
            /// magnitudes the sample will repeat and fade out over time. Use this parameter to create a "cascading" chorus effect.  @n @n Range: -1.0 to 1.0
            Real Feedback;
            /// @brief This property controls the average amount of time the sample is delayed before it is played back.
            /// @remarks This property can also control with feedback, the amount of time between iterations of the sample. Larger values lower the pitch. Smaller values
            /// make the chorus sound like a flanger, but with different frequency characteristics.  @n @n Range: 0.0 to 0.016
            Real Delay;

            /// @brief Struct constructor.
            /// @param waveform @copybrief ChorusParameters::Waveform
            /// @param phase @copybrief ChorusParameters::Phase
            /// @param rate @copybrief ChorusParameters::Rate
            /// @param depth @copybrief ChorusParameters::Depth
            /// @param feedback @copybrief ChorusParameters::Feedback
            /// @param delay @copybrief ChorusParameters::Delay
            ChorusParameters(
                ChorusWaveform waveform = ECW_Triangle,
                Integer phase = 90,
                Real rate = 1.1f,
                Real depth = 0.1f,
                Real feedback = 0.25f,
                Real delay = 0.016f) :
                Waveform(waveform), Phase(phase), Rate(rate), Depth(depth), Feedback(feedback),
                Delay(delay) { }
        };// ©horusParameters

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a struct containing all the parameters needed to describe a Distortion effect.
        /// @details The distortion effect simulates turning up (overdriving) the gain stage on a guitar amplifier or adding a distortion pedal to an instrument's output.
        ///////////////////////////////////////
        struct DistortionParameters
        {
            /// @brief This property controls the shape of the distortion.
            /// @remarks The higher the value for Edge, the "dirtier" and "fuzzier" the effect.  @n @n Range: 0.0 to 1.0
            Real Edge;
            /// @brief This property allows you to attenuate the distorted sound.
            /// @remarks Range: 0.01 to 1.0
            Real Gain;
            /// @brief Input signal can have a low pass filter applied, to limit the amount of high frequency signal feeding into the distortion effect.
            /// @remarks Range: 80.0 to 24000.0
            Real LowpassCutoff;
            /// @brief This property controls the frequency at which the post-distortion attenuation (Gain) is active.
            /// @remarks Range: 80.0 to 24000.0
            Real EqCenter;
            /// @brief This property controls the bandwidth of the post-distortion attenuation.
            /// @remarks Range: 80.0 to 24000.0
            Real EqBandwidth;

            /// @brief Struct constructor.
            /// @param edge @copybrief DistortionParameters::Edge
            /// @param gain @copybrief DistortionParameters::Gain
            /// @param lowpassCutoff @copybrief DistortionParameters::LowpassCutoff
            /// @param eqCenter @copybrief DistortionParameters::EqCenter
            /// @param eqBandwidth @copybrief DistortionParameters::EqBandwidth
            DistortionParameters(
                Real edge = 0.2f,
                Real gain = 0.05f,
                Real lowpassCutoff = 8000.0f,
                Real eqCenter = 3600.0f,
                Real eqBandwidth = 3600.0f) :
                Edge(edge), Gain(gain), LowpassCutoff(lowpassCutoff), EqCenter(eqCenter),
                EqBandwidth(eqBandwidth) { }
        };//DistortionParameters

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a struct containing all the parameters needed to describe an echo effect.
        /// @details The echo effect generates discrete, delayed instances of the input signal.
        ///////////////////////////////////////
        struct EchoParameters
        {
            /// @brief This property controls the delay between the original sound and the first "tap", or echo instance.
            /// @remarks Range: 0.0 to 0.207
            Real Delay;
            /// @brief This property controls the delay between the first "tap" and the second "tap".
            /// @remarks Range: 0.0 to 0.404
            Real LRDelay;
            /// @brief This property controls the amount of high frequency damping applied to each echo.
            /// @remarks As the sound is subsequently fed back for further echoes, damping results in an echo which progressively gets softer in tone as well as
            /// intensity.  @n @n Range: 0.0 to 0.99
            Real Damping;
            /// @brief This property controls the amount of feedback the output signal fed back into the input.
            /// @remarks Use this parameter to create "cascading" echoes. At full magnitude, the identical sample will repeat endlessly. Below full magnitude,
            /// the sample will repeat and fade.  @n @n Range: 0.0 to 1.0
            Real Feedback;
            /// @brief This property controls how hard panned the individual echoes are.
            /// @remarks With a value of 1.0, the first "tap" will be panned hard left, and the second "tap" hard right. A value of -1.0 gives the opposite result.
            /// Settings nearer to 0.0 result in less emphasized panning.  @n @n Range: -1.0 to 1.0
            Real Spread;

            /// @brief Struct constructor.
            /// @param delay @copybrief EchoParameters::Delay
            /// @param lRDelay @copybrief EchoParameters::LRDelay
            /// @param damping @copybrief EchoParameters::Damping
            /// @param feedback @copybrief EchoParameters::Feedback
            /// @param spread @copybrief EchoParameters::Spread
            EchoParameters(
                Real delay = 0.1f,
                Real lRDelay = 0.1f,
                Real damping = 0.5f,
                Real feedback = 0.5f,
                Real spread = -1.0f) :
                Delay(delay), LRDelay(lRDelay), Damping(damping), Feedback(feedback),
                Spread(spread) { }
        };//EchoParameters

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a struct containing all the parameters needed to describe an flanger effect.
        /// @details The flanger effect creates a "tearing" or "whooshing" sound (like a jet flying overhead).
        ///////////////////////////////////////
        struct FlangerParameters
        {
            /// @enum FlangerWaveform
            /// @brief The shape of the wave if it were viewed on an oscillator.
            enum FlangerWaveform
            {
                EFW_Sinusoid,   ///< A curved oscillation with smoother transitions at peak frequencies.
                EFW_Triangle    ///< A linear oscillation with rapid transitions at peak frequencies.
            };

            /// @brief Selects the shape of the LFO waveform that controls the amount of the delay of the sampled signal.
            FlangerWaveform Waveform;
            /// @brief This changes the phase difference between the left and right LFO's.
            /// @remarks At zero degrees the two LFOs are synchronized.  @n @n Range: -180 to 180
            Integer Phase;
            /// @brief The number of times per second the LFO controlling the amount of delay repeats.
            /// @remarks Higher values increase the pitch modulation.  @n @n Range: 0.0 to 10.0
            Real Rate;
            /// @brief The ratio by which the delay time is modulated by the LFO.
            /// @remarks Use this parameter to increase the pitch modulation.  @n @n Range: 0.0 to 1.0
            Real Depth;
            /// @brief This is the amount of the output signal level fed back into the effect's input.
            /// @remarks A negative value will reverse the phase of the feedback signal. Use this parameter to create an "intense metallic" effect. At full magnitude,
            /// the identical sample will repeat endlessly. At less than full magnitude, the sample will repeat and fade out over time.  @n @n Range: -1.0 to 1.0
            Real Feedback;
            /// @brief The average amount of time the sample is delayed before it is played back; with feedback, the amount of time between iterations of the sample.
            /// @remarks Range: 0.0 to 0.004
            Real Delay;

            /// @brief Struct constructor.
            /// @param waveform @copybrief FlangerParameters::Waveform
            /// @param phase @copybrief FlangerParameters::Phase
            /// @param rate @copybrief FlangerParameters::Rate
            /// @param depth @copybrief FlangerParameters::Depth
            /// @param feedback @copybrief FlangerParameters::Feedback
            /// @param delay @copybrief FlangerParameters::Delay
            FlangerParameters(
                FlangerWaveform waveform = EFW_Triangle,
                Integer phase = 0,
                Real rate = 0.27f,
                Real depth = 1.0f,
                Real feedback = -0.5f,
                Real delay = 0.002f) :
                Waveform(waveform), Phase(phase), Rate(rate), Depth(depth), Feedback(feedback),
                Delay(delay) { }
        };//FlangerParameters

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a struct containing all the parameters needed to describe a frequency shift effect.
        /// @details The frequency shifter is a single-sideband modulator, which translates all the component frequencies of the input signal by an equal amount.
        ///////////////////////////////////////
        struct FrequencyShiftParameters
        {
            /// @enum ShiftDirection
            /// @brief An enum describing the direction the frequency of the input signal will be shifted.
            enum ShiftDirection
            {
                ESD_Down,
                ESD_Up,
                ESD_Off
            };

            /// @brief This is the carrier frequency.
            /// @remarks For carrier frequencies below the audible range, the singlesideband modulator may produce phaser effects, spatial effects or a slight pitch-shift.
            /// As the carrier frequency increases, the timbre of the sound is affected; a piano or guitar note becomes like a bell's chime, and a human voice sounds
            /// extraterrestrial!  @n @n Range: 0.0 to 24000.0
            Real Frequency;
            /// @brief These select which internal signals are added together to produce the output.
            /// @remarks Different combinations of values will produce slightly different tonal and spatial effects.
            ShiftDirection Left;
            /// @brief These select which internal signals are added together to produce the output.
            /// @remarks Different combinations of values will produce slightly different tonal and spatial effects.
            ShiftDirection Right;

            /// @brief Struct constructor.
            /// @param frequency @copybrief FrequencyShiftParameters::Frequency
            /// @param left @copybrief FrequencyShiftParameters::Left
            /// @param right @copybrief FrequencyShiftParameters::Right
            FrequencyShiftParameters(
                Real frequency = 0.0f,
                ShiftDirection left = ESD_Down,
                ShiftDirection right = ESD_Down) :
                Frequency(frequency), Left(left), Right(right) { }
        };//FrequencyShiftParameters

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a struct containing all the parameters needed to describe a vocal morpher effect.
        /// @details The vocal morpher consists of a pair of 4-band formant filters, used to impose vocal tract effects upon the input signal.
        ///////////////////////////////////////
        struct VocalMorpherParameters
        {
            /// @enum MorpherPhoneme
            /// @brief An enum describing the basic phoneme's of the english language.
            enum MorpherPhoneme
            {
                EMP_A,
                EMP_E,
                EMP_I,
                EMP_O,
                EMP_U,
                EMP_AA,
                EMP_AE,
                EMP_AH,
                EMP_AO,
                EMP_EH,
                EMP_ER,
                EMP_IH,
                EMP_IY,
                EMP_UH,
                EMP_UW,
                EMP_B,
                EMP_D,
                EMP_F,
                EMP_G,
                EMP_J,
                EMP_K,
                EMP_L,
                EMP_M,
                EMP_N,
                EMP_P,
                EMP_R,
                EMP_S,
                EMP_T,
                EMP_V,
                EMP_Z
            };

            /// @enum MorpherWaveform
            /// @brief The shape of the wave if it were viewed on an oscillator.
            enum MorpherWaveform
            {
                EMW_Sinusoid,   ///< A curved oscillation with smoother transitions at peak frequencies.
                EMW_Triangle,   ///< A linear oscillation with rapid transitions at peak frequencies.
                EMW_Saw         ///< A curved upward oscillation with a rapid drop at it's peak frequency.
            };

            /// @brief The source phoneme.
            /// @remarks If both parameters are set to the same phoneme, that determines the filtering effect that will be heard. If these two parameters are set to
            /// different phonemes, the filtering effect will morph between the two settings at a rate specified by Rate.
            MorpherPhoneme PhonemeA;
            /// @brief The destination phoneme.
            /// @remarks If both parameters are set to the same phoneme, that determines the filtering effect that will be heard. If these two parameters are set to
            /// different phonemes, the filtering effect will morph between the two settings at a rate specified by Rate.
            MorpherPhoneme PhonemeB;
            /// @brief This is used to adjust the pitch of phoneme filter A in 1-semitone increments.
            /// @remarks Range: -24 to 24
            Integer PhonemeACoarseTune;
            /// @brief This is used to adjust the pitch of phoneme filter B in 1-semitone increments.
            /// @remarks Range: -24 to 24
            Integer PhonemeBCoarseTune;
            /// @brief This controls the shape of the low-frequency oscillator used to morph between the two phoneme filters.
            MorpherWaveform Waveform;
            /// @brief This controls the frequency of the low-frequency oscillator used to morph between the two phoneme filters.
            /// @remarks Range: 0.0 to 10.0
            Real Rate;

            /// @brief Struct constructor.
            /// @param phonemeA @copybrief VocalMorpherParamters::PhonemeA
            /// @param phonemeB @copybrief VocalMorpherParamters::PhonemeB
            /// @param phonemeACoarseTune @copybrief VocalMorpherParamters::PhonemeACoarseTune
            /// @param phonemeBCoarseTune @copybrief VocalMorpherParamters::PhonemeBCoarseTune
            /// @param waveform @copybrief VocalMorpherParamters::Waveform
            /// @param rate @copybrief VocalMorpherParamters::Rate
            VocalMorpherParameters(
                MorpherPhoneme phonemeA = EMP_A,
                MorpherPhoneme phonemeB = EMP_ER,
                Integer phonemeACoarseTune = 0,
                Integer phonemeBCoarseTune = 0,
                MorpherWaveform waveform = EMW_Sinusoid,
                Real rate = 1.41f) :
                PhonemeA(phonemeA), PhonemeB(phonemeB), PhonemeACoarseTune(phonemeACoarseTune),
                PhonemeBCoarseTune(phonemeBCoarseTune), Waveform(waveform), Rate(rate) { }
        };//VocalMorpherParameters

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a struct containing all the parameters needed to describe a pitch shift effect.
        /// @details The pitch shifter applies time-invariant pitch shifting to the input signal, over a one octave range and controllable at a semi-tone and cent resolution.
        ///////////////////////////////////////
        struct PitchShifterParameters
        {
            /// @brief This sets the number of semitones by which the pitch is shifted.
            /// @remarks There are 12 semitones per octave. Negative values create a downwards shift in pitch, positive values pitch the sound upwards.  @n @n Range: -12 to 12
            Integer CoarseTune;
            /// @brief This sets the number of cents between Semitones a pitch is shifted.
            /// @remarks A Cent is 1/100th of a Semitone. Negative values create a downwards shift in pitch, positive values pitch the sound upwards.  @n @n Range: -50 to 50
            Integer FineTune;

            /// @brief Struct constructor.
            /// @param coarseTune @copybrief PitchShifterParameters::CoarseTune
            /// @param fineTune @copybrief PitchShifterParameters::FineTune
            PitchShifterParameters(
                Integer coarseTune = 12,
                Integer fineTune = 0) :
                CoarseTune(coarseTune), FineTune(fineTune) { }
        };//PitchShifterParameters

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a struct containing all the parameters needed to describe a ring modulation effect.
        /// @details The ring modulator multiplies an input signal by a carrier signal in the time domain, resulting in tremolo or inharmonic effects.
        ///////////////////////////////////////
        struct RingModulatorParameters
        {
            /// @enum ModulatorWaveform
            /// @brief The shape of the wave if it were viewed on an oscillator.
            enum ModulatorWaveform
            {
                EMW_Sinusoid,   ///< A curved oscillation with smoother transitions at peak frequencies.
                EMW_Saw,        ///< A curved upward oscillation with a rapid drop at it's peak frequency.
                EMW_Square      ///< A digital waveform that only expresses the highest and lowest frequencies.  Effectively "on" or "off".
            };

            /// @brief This is the frequency of the carrier signal.
            /// @remarks If the carrier signal is slowly varying (less than 20 Hz), the result is a tremolo (slow amplitude variation) effect. If the carrier signal is
            /// in the audio range, audible upper and lower sidebands begin to appear, causing an inharmonic effect. The carrier signal itself is not heard in the output.
            /// @n @n Range: 0.0 to 8000.0
            Real Frequency;
            /// @brief This controls the cutoff frequency at which the input signal is high-pass filtered before being ring modulated.
            /// @remarks If the cutoff frequency is 0, the entire signal will be ring modulated. If the cutoff frequency is high, very little of the signal (only those
            /// parts above the cutoff) will be ring modulated.  @n @n Range: 0.0 to 24000.0
            Real HighPassCutoff;
            /// @brief This controls which waveform is used as the carrier signal.
            /// @remarks Traditional ring modulator and tremolo effects generally use a sinusoidal carrier. Sawtooth and square waveforms are may cause unpleasant aliasing.
            ModulatorWaveform Waveform;

            /// @brief Struct constructor.
            /// @param frequency @copybrief RingModulatorParameters::Frequency
            /// @param highPassCutoff @copybrief RingModulatorParameters::HighPassCutoff
            /// @param waveform @copybrief RingModulatorParameters::Waveform
            RingModulatorParameters(
                Real frequency = 440.0f,
                Real highPassCutoff = 800.0f,
                ModulatorWaveform waveform = EMW_Sinusoid) :
                Frequency(frequency), HighPassCutoff(highPassCutoff), Waveform(waveform) { }
        };//RingModulatorParameters

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a struct containing all the parameters needed to describe an Auto-Wah effect.
        /// @details The Auto-wah effect emulates the sound of a wah-wah pedal used with an electric guitar, or a mute on a brass instrument.
        ///////////////////////////////////////
        struct AutowahParameters
        {
            /// @brief This property controls the time the filtering effect takes to sweep from minimum to maximum center frequency when it is triggered by input signal.
            /// @remarks Range: 0.0001 to 1.0
            Real AttackTime;
            /// @brief This property controls the time the filtering effect takes to sweep from maximum back to base center frequency, when the input signal ends.
            /// @remarks Range: 0.0001 to 1.0
            Real ReleaseTime;
            /// @brief This property controls the resonant peak, sometimes known as emphasis or Q, of the auto-wah band-pass filter.
            /// @remarks Resonance occurs when the effect boosts the frequency content of the sound around the point at which the filter is working. A high value promotes
            /// a highly resonant, sharp sounding effect.  @n @n Range: 2.0 to 1000.0
            Real Resonance;
            /// @brief This property controls the input signal level at which the band-pass filter will be fully opened.
            /// @remarks Range: 0.00003 to 31621.0
            Real PeakGain;

            /// @brief Struct constructor.
            /// @param attackTime @copybrief AutowahParameters::AttackTime
            /// @param releaseTime @copybrief AutowahParameters::ReleaseTime
            /// @param resonance @copybrief AutowahParameters::Resonance
            /// @param peakGain @copybrief AutowahParameters::PeakGain
            AutowahParameters(
                Real attackTime = 0.06f,
                Real releaseTime = 0.06f,
                Real resonance = 1000.0f,
                Real peakGain = 11.22f) :
                AttackTime(attackTime), ReleaseTime(releaseTime), Resonance(resonance),
                PeakGain(peakGain) { }
        };//AutowahParameters

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a struct containing all the parameters needed to describe a compressor effect.
        /// @details The Automatic Gain Control effect performs the same task as a studio compressor, evening out the audio dynamic range of an input sound.
        ///////////////////////////////////////
        struct CompressorParameters
        {
            /// @brief The Compressor can only be switched on and off  it cannot be adjusted.
            bool Active;

            /// @brief Struct constructor.
            /// @param active @copybrief CompressorParameters::Active
            CompressorParameters(
                bool active = true) :
                Active(active) { }
        };// ©ompressorParameters

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a struct containing all the parameters needed to describe an equalizer effect.
        /// @details The OpenAL Effects Extension EQ is very flexible, providing tonal control over four different adjustable frequency ranges.
        ///////////////////////////////////////
        struct EqualizerParameters
        {
            /// @brief This property controls amount of cut or boost on the low frequency range.
            /// @remarks Range: 0.126 to 7.943
            Real LowGain;
            /// @brief This property controls the low frequency below which signal will be cut off.
            /// @remarks Range: 50.0 to 800.0
            Real LowCutoff;
            /// @brief This property allows you to cut / boost signal on the "mid1" range.
            /// @remarks Range: 0.126 to 7.943
            Real Mid1Gain;
            /// @brief This property sets the center frequency for the "mid1" range.
            /// @remarks Range: 200.0 to 3000.0
            Real Mid1Center;
            /// @brief This property controls the width of the "mid1" range.
            /// @remarks Range: 0.01 to 1.0
            Real Mid1Width;
            /// @brief This property allows you to cut / boost signal on the "mid2" range.
            /// @remarks Range: 0.126 to 7.943
            Real Mid2Gain;
            /// @brief This property sets the center frequency for the "mid2" range.
            /// @remarks Range: 1000.0 to 8000.0
            Real Mid2Center;
            /// @brief This property controls the width of the "mid2" range.
            /// @remarks Range: 0.01 to 1.0
            Real Mid2Width;
            /// @brief This property allows you to cut / boost the signal at high frequencies.
            /// @remarks Range: 0.126 to 7.943
            Real HighGain;
            /// @brief This property controls the high frequency above which signal will be cut off.
            /// @remarks Range: 4000.0 to 16000.0
            Real HighCutoff;

            /// @brief Struct constructor.
            /// @param lowGain @copybrief EqualizerParameters::LowGain
            /// @param lowCutoff @copybrief EqualizerParameters::LowCutoff
            /// @param mid1Gain @copybrief EqualizerParameters::Mid1Gain
            /// @param mid1Center @copybrief EqualizerParameters::Mid1Center
            /// @param mid1Width @copybrief EqualizerParameters::Mid1Width
            /// @param mid2Gain @copybrief EqualizerParameters::Mid2Gain
            /// @param mid2Center @copybrief EqualizerParameters::Mid2Center
            /// @param mid2Width @copybrief EqualizerParameters::Mid2Width
            /// @param highGain @copybrief EqualizerParameters::HighGain
            /// @param highCutoff @copybrief EqualizerParameters::HighCutoff
            EqualizerParameters(
                Real lowGain = 1.0f,
                Real lowCutoff = 200.0f,
                Real mid1Gain = 1.0f,
                Real mid1Center = 500.0f,
                Real mid1Width = 1.0f,
                Real mid2Gain = 1.0f,
                Real mid2Center = 3000.0f,
                Real mid2Width = 1.0f,
                Real highGain = 1.0f,
                Real highCutoff = 6000.0f) :
                LowGain(lowGain), LowCutoff(lowCutoff), Mid1Gain(mid1Gain),
                Mid1Center(mid1Center), Mid1Width(mid1Width), Mid2Gain(mid2Gain),
                Mid2Center(mid2Center), Mid2Width(mid2Width), HighGain(highGain),
                HighCutoff(highCutoff) { }
        };//EqualizerParameters
    }//Audio
}//Mezzanine

#endif
