//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _uikerning_h
#define _uikerning_h

namespace Mezzanine
{
    namespace UI
    {
        /// @struct KerningInfo
        /// @headerfile uikerning.h
        /// @brief Basic class used to describe Kerning for a given Glyph.
        /// @details Kerning is a special spacing amount for abnormally large or small glyphs.  For example, you wouldn't increment
        /// the spacing for the glyph "W" the same amount as you would for "I" in most cases.
        struct MEZZ_LIB KerningInfo
        {
            /// @brief Class Constructor.
            /// @param C The Character this Kerning applies to.
            /// @param K The amount of Kerning to apply to the Glyph.
            KerningInfo(const UInt32& C, const Real& K) : Character(C), Kerning(K) {};
            /// @brief The Character this Kerning applies to.
            UInt32 Character;
            /// @brief The amount of Kerning to apply to the Glyph.
            Real Kerning;
        };//Kerning
    }//UI
}//Mezzanine

#endif
