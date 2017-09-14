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
#ifndef _audioenumerations_h
#define _audioenumerations_h
///////////////////////////////////////////////////////////////////////////////
//Any global enumerations shared between multiple classes in the Audio namespace is to be declared here.
///////////////////////////////////////

namespace Mezzanine
{
    namespace Audio
    {
		/// @brief Used to describe the different bit configurations supported by this audio system.
		enum BitConfig
		{
		    BC_8Bit_Mono = 1,
		    BC_8Bit_Stereo,
		    BC_16Bit_Mono,
		    BC_16Bit_Stereo,
		    BC_24Bit_Mono,
		    BC_24Bit_Stereo
		};

        /// @brief The encoding to use when reading or writing an audio buffer.
        enum Encoding
        {
            Enc_RAW    = 1,  ///< Unknown or error condition.
            Enc_WAV    = 2,  ///< Used in .wav files.
            Enc_FLAC   = 3,  ///< Used in .flac files.
            Enc_VORBIS = 4,  ///< Used in .ogg files.
            Enc_MP3    = 5,  ///< Used in .mp3 files.
            Enc_SPEEX  = 6,  ///< Used in .spx files.
            Enc_OPUS   = 7   ///< Used in .opus files.
        };

		/// @brief Used by the iEffect class to describe what type of effect it is.
        enum EffectType
        {
            ET_Null,              ///< Null type with no values.
            ET_EAX_Reverb,        ///< See @ref EAXReverbParameters.
            ET_Reverb,            ///< See @ref ReverbParameters.
            ET_Chorus,            ///< See @ref ChorusParameters.
            ET_Distortion,        ///< See @ref DistortionParameters.
            ET_Echo,              ///< See @ref EchoParameters.
            ET_Flanger,           ///< See @ref FlangerParameters.
            ET_Frequency_Shifter, ///< See @ref FrequencyShiftParameters.
            ET_Vocal_Morpher,     ///< See @ref VocalMorpherParameters.
            ET_Pitch_Shifter,     ///< See @ref PitchShifterParameters.
            ET_Ring_Modulator,    ///< See @ref RingModulatorParameters.
            ET_Autowah,           ///< See @ref AutowahParameters.
            ET_Compressor,        ///< See @ref CompressorParameters.
            ET_Equalizer          ///< See @ref EqualizerParameters.
        };

		/// @brief Used by the iFilter class to describe what type of filter it is.
        enum FilterType
        {
            FT_Null,        ///< Null type with no values.
            FT_LowPass,     ///< Filters out high frequency audio from a sound beyond a certain threshold.
            FT_HighPass,    ///< Filters out low frequency audio from a sound below a certain threshold.
            FT_BandPass     ///< Filters out both high and low frequency audio from a sound outside of specified thresholds.
        };

        /// @brief Used by the iSound class to describe what type of sound it is.
        enum SoundType
        {
            ST_Ambient = 1,  ///< Environmental sounds.  Bird's chirping, water washing against rocks, etc.
            ST_Dialog  = 2,  ///< Character sounds and speech.
            ST_Effect  = 3,  ///< Various effects such as objects colliding, explosions, guns firing, etc.
            ST_Music   = 4   ///< Self explanitory.  The game SoundTrack.
        };
    }//audio
}//Mezzanine

#endif
