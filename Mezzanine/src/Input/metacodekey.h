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
#ifndef _inputmetacodekey_h
#define _inputmetacodekey_h

#include "Input/metacode.h"
#include "exception.h"

namespace Mezzanine
{
	namespace Input
	{
	    ///////////////////////////////////////////////////////////////////////////////
        /// @class MetaCodeKey
        /// @headerfile metacodekey.h
        /// @brief A key class for MetaCodes to be used in associative containers.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB MetaCodeKey
        {
        public:
            /// @brief Convenience typedef for the compare method to be used by this key.
            typedef Boole (DeviceCompare)(const MetaCode& First, const MetaCode& Second);
        protected:
            /// @internal
            /// @brief The metacode being used as the compare object.
            const MetaCode MCode;
            /// @internal
            /// @brief The compare method being used for this key.
            DeviceCompare* Compare;
        public:
            /// @brief Class constructor.
            /// @param Code The MetaCode this key is to use for sorting.
            MetaCodeKey(const MetaCode& Code);
            /// @brief Copy constructor.
            /// @param Other The other MetaCodeKey to copy.
            MetaCodeKey(const MetaCodeKey& Other);
            /// @brief Class destructor.
            ~MetaCodeKey();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the MetaCode this object is using for sorting.
            /// @return Returns a const reference to the MetaCode stored by this object.
            const MetaCode& GetKeyData() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Operators

            /// @brief Less-than operator.
            /// @param Other The other key to compare against.
            /// @return Returns true if this key is less than another key.
            Boole operator<(const MetaCodeKey& Other) const;
        };//MetaCodeKey
	}//Input
}//Mezzanine

#endif
