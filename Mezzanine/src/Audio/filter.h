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
#ifndef _audiofilter_h
#define _audiofilter_h

#include "datatypes.h"
#include "Audio/audioenumerations.h"

namespace Mezzanine
{
    namespace Audio
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is an interface class for the application of filters to audio playback.
        /// @details
        ///////////////////////////////////////
        class iFilter
        {
        public:
            /// @brief Class constructor.
            iFilter() {  }
            /// @brief Class destructor.
            virtual ~iFilter() {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets whether or not this filter is ready to be used.
            /// @return Returns true if this filter is ready for use, or false if there is an error.
            virtual Boole IsValid() const = 0;

            /// @brief Sets the type of filter this is.
            /// @param FilType The type of filter to set this as.  See enum @ref FilterType for more information.
            virtual void SetType(const FilterType FilType) = 0;
            /// @brief Gets the type of filter this is.
            /// @return Returns the type of filter this filter is currently set as.
            virtual FilterType GetType() const = 0;

            /// @brief Sets the master volume of this filter.
            /// @note Valid range: 0.0 to 1.0.
            /// @param Vol The volume scale to apply to all audio passed through this filter.
            virtual void SetVolume(const Real Vol) = 0;
            /// @brief Gets the master volume of this filter.
            /// @return Returns a Real representing the volume scale being applied to all audio passing through this filter.
            virtual Real GetVolume() const = 0;
            /// @brief Sets the high frequency volume of this filter.
            /// @note Valid range: 0.0 to 1.0.
            /// @param HFVol The volume scale to apply to higher frequency audio passed through this filter.
            virtual void SetHighFrequencyVolume(const Real HFVol) = 0;
            /// @brief Gets the high frequency volume of this filter.
            /// @return Returns a Real representing the volume scale being applied to higher frequency audio passing through this filter.
            virtual Real GetHighFrequencyVolume() const = 0;
            /// @brief Sets the low frequency volume of this filter.
            /// @note Valid range: 0.0 to 1.0.
            /// @param LFVol The volume scale to apply to lower frequency audio passed through this filter.
            virtual void SetLowFrequencyVolume(const Real LFVol) = 0;
            /// @brief Gets the low frequency volume of this filter.
            /// @return Returns a Real representing the volume scale being applied to lower frequency audio passing through this filter.
            virtual Real GetLowFrequencyVolume() const = 0;
        };//iFilter
    }//Audio
}//Mezzanine

#endif
