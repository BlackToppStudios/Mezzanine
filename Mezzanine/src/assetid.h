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
#ifndef _resourceassetid_h
#define _resourceassetid_h

#include "datatypes.h"

namespace Mezzanine
{
    namespace Resource
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a simple class storing the name and group identification of an Asset/Stream.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB AssetID
        {
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Public Data Members

            /// @brief A string containing the name of this Asset.
            String AssetName;
            /// @brief A string containing the name of the group this Asset belongs to.
            String GroupName;

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            /// @brief Class constructor.
            AssetID() {  }
            /// @brief Class destructor.
            ~AssetID() {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Operators

            /// @brief Less-than operator.
            /// @param Other The other AssetID to compare with.
            /// @return Returns true if this Asset should be order before the other Asset, false otherwise.
            Boole operator<(const AssetID& Other)
            {
                if( this->GroupName == Other.GroupName ) {
                    return ( this->AssetName < Other.AssetName );
                }else{
                    return ( this->GroupName < Other.GroupName );
                }
            }
            /// @brief Greater-than operator.
            /// @param Other The other AssetID to compare with.
            /// @return Returns true if this Asset should be order after the other Asset, false otherwise.
            Boole operator>(const AssetID& Other)
            {
                if( this->GroupName == Other.GroupName ) {
                    return ( this->AssetName > Other.AssetName );
                }else{
                    return ( this->GroupName > Other.GroupName );
                }
            }

            /// @brief Equality operator.
            /// @param Other The other AssetID to compare with.
            /// @return Returns true if this Asset is the same as another asset, false otherwise.
            Boole operator==(const AssetID& Other)
            {
                return ( this->GroupName == Other.GroupName ) && ( this->AssetName == Other.AssetName );
            }
            /// @brief Inequality operator.
            /// @param Other The other AssetID to compare with.
            /// @return Returns true if this Asset is not the same as another asset, false otherwise..
            Boole operator!=(const AssetID& Other)
            {
                return ( this->GroupName != Other.GroupName ) || ( this->AssetName != Other.AssetName );
            }
        };//AssetID
    }//Resource
}//Mezzanine

#endif
