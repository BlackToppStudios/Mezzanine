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
#ifndef _audiooalsfilter_h
#define _audiooalsfilter_h

#include "Audio/filter.h"

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            class EFXInterface;
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief This is the OpenALSoft implemenation for an Audio Filter.
            ///////////////////////////////////////
            class MEZZ_LIB Filter : public iFilter
            {
            protected:
                /// @brief Pointer to the class holding function pointers to all the OALS EFX methods
                EFXInterface* EFX;
                /// @brief This stores the volume modifier to be applied to filtered sounds.
                Real Volume;
                /// @brief This stores the volume to be used by the filter at low frequencies.
                Real LFVolume;
                /// @brief This stores the volume to be used by the filter at high frequencies.
                Real HFVolume;
                /// @brief This stores the internal ID for this filter.
                UInt32 InternalFilter;
                /// @brief This stores whether or not this filter has been modified recently.
                Boole Dirty;
                /// @brief This stores whether or not there was an error with the last settings change.
                Boole Valid;

                /// @brief Updates the appropriate volume based on the type.
                void UpdateVolume();
                /// @brief Updates the volume used on high and low frequency sounds based on the type.
                void UpdateFrequencyVolume();
                /// @brief Checks to see if there was a configuration error after setting a filter property.
                /// @return Returns true if there was no error, false if there was.
                Boole CheckValid();
            public:
                /// @brief Class constructor.
                /// @param EFXMethods A pointer to the class storing all the methods to the EFX extension.
                Filter(EFXInterface* EFXMethods);
                /// @brief Serialization constructor.
                /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
                /// @param EFXMethods A pointer to the class storing all the methods to the EFX extension.
                Filter(const XML::Node& SelfRoot, EFXInterface* EFXMethods);
                /// @brief Class destructor.
                virtual ~Filter();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc iFilter::IsValid() const
                virtual Boole IsValid() const;

                /// @copydoc iFilter::SetType(const FilterType FilType)
                virtual void SetType(const FilterType FilType);
                /// @copydoc iFilter::GetType() const
                virtual FilterType GetType() const;

                /// @copydoc iFilter::SetVolume(const Real Vol)
                virtual void SetVolume(const Real Vol);
                /// @copydoc iFilter::GetVolume() const
                virtual Real GetVolume() const;
                /// @copydoc iFilter::SetHighFrequencyVolume(const Real HFVol)
                virtual void SetHighFrequencyVolume(const Real HFVol);
                /// @copydoc iFilter::GetHighFrequencyVolume() const
                virtual Real GetHighFrequencyVolume() const;
                /// @copydoc iFilter::SetLowFrequencyVolume(const Real LFVol)
                virtual void SetLowFrequencyVolume(const Real LFVol);
                /// @copydoc iFilter::GetLowFrequencyVolume() const
                virtual Real GetLowFrequencyVolume() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Serialization

                /// @copydoc iFilter::ProtoSerialize(XML::Node&) const
                virtual void ProtoSerialize(XML::Node& ParentNode) const;
                /// @copydoc iFilter::ProtoDeSerialize(const XML::Node&)
                virtual void ProtoDeSerialize(const XML::Node& SelfRoot);

                /// @brief Get the name of the the XML tag the proxy class will leave behind as its instances are serialized.
                /// @return A string containing the name of this class.
                static String GetSerializableName();

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Methods

                /// @internal
                /// @brief Gets the internal Filter ID for this Filter.
                /// @return Returns a UInt32 representing this Filter in OALS.
                UInt32 _GetInternalFilter() const;
                /// @internal
                /// @brief Checks to see if this Filter has been modified since last frame sounds were processed.
                /// @return Returns true if this Filter needs to be re-applied, false otherwise.
                Boole _IsDirty() const;
                /// @internal
                /// @brief Clears the dirty flag.
                void _Clean();
            };//Filter
        }//OALS
    }//Audio
}//Mezzanine

#endif
