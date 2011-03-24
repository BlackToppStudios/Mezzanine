//© Copyright 2010 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#ifndef _colourvalue_h
#define _colourvalue_h

#include "crossplatformexport.h"
#include "datatypes.h"
#include "xml.h"

namespace Ogre
{
    class ColourValue;
}

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ColourValue
    /// @headerfile colourvalue.h
    /// @brief This is a simple class for holding 4 reals representing the colour any give object or lightsource can have.
    ///////////////////////////////////////
    class PHYS_LIB ColourValue
    {
        public:
            /// @brief Value from 0.0 to 1.0 representing the amount of red present in the colour. 1.0 if very red, 0.0 is no red.
            Real Red;
            /// @brief Value from 0.0 to 1.0 representing the amount of green present in the colour. 1.0 if very green, 0.0 is no green.
            Real Green;
            /// @brief Value from 0.0 to 1.0 representing the amount of blue present in the colour. 1.0 if very blue, 0.0 is no blue.
            Real Blue;
            /// @brief Value from 0.0 to 1.0 representing the transparency of the colours. 1.0 is opaque and 0.0 is clear.
            Real Alpha;

            /// @brief Non-Alpha constructor.
            /// @details This constructor assumes the alpha value to be 1.0 (completely opaque).
            /// @param red Real representing the amount of red present in the colour.
            /// @param green Real representing the amount of green present in the colour.
            /// @param blue Real representing the amount of blue present in the colour.
            ColourValue(Real red, Real green, Real blue);

            /// @brief Alpha constructor.
            /// @details This constructor allows you to set the alpha value.
            /// @param red Real representing the amount of red present in the colour.
            /// @param green Real representing the amount of green present in the colour.
            /// @param blue Real representing the amount of blue present in the colour.
            /// @param alpha Real representing the transparency of the colours.
            ColourValue(Real red, Real green, Real blue, Real alpha);

            /// @brief Ogre constructor.
            /// @details Internal use only.  Constructs a colourvalue class from an ogre colourvalue.
            /// @param OgreValues The Ogre ColourValue class to base this class on.
            ColourValue(const Ogre::ColourValue& OgreValues);

            /// @brief Copy Constructor
            /// @param OtherColour
            ColourValue(const ColourValue& OtherColour);

            /// @brief Class destructor.
            ~ColourValue();

            /// @brief Creates and returns an Ogre ColourValue class with values equal to this one.
            /// @details This function is intended for internal use only.
            /// @return Returns an Ogre ColourValue class that has values equal to this one.
            Ogre::ColourValue GetOgreColourValue() const;

            /// @brief Equality Comparison Operator
            /// @param Colour This is another ColourValue to compare with.
            /// @return True if the colors match perfectly, false otherwise
            bool operator== (const ColourValue &Colour);
            /// @brief Inequality Comparison Operator
            /// @param Colour This is another ColourValue to compare with.
            /// @return False if the colors match perfectly, True otherwise
            bool operator!= (const ColourValue &Colour);
            /// @brief Assignment operator.
            /// @param OtherColour The colour values you want to overwrite this colour's values with.
            void operator= (const ColourValue &OtherColour);

            /// @brief Creates a ColourValue representing no colour.
            /// @return Returns the created ColourValue.
            static ColourValue GetBlank();
            /// @brief Creates a ColourValue representing the colour White.
            /// @return Returns the created ColourValue.
            static ColourValue GetWhite();
            /// @brief Creates a ColourValue representing the colour Black.
            /// @return Returns the created ColourValue.
            static ColourValue GetBlack();

            /// @brief Creates a ColourValue representing the colour Red.
            /// @return Returns the created ColourValue.
            static ColourValue GetRed();
            /// @brief Creates a ColourValue representing the colour Green.
            /// @return Returns the created ColourValue.
            static ColourValue GetGreen();
            /// @brief Creates a ColourValue representing the colour Blue.
            /// @return Returns the created ColourValue.
            static ColourValue GetBlue();

            /// @brief Creates a ColourValue representing the colour Yellow.
            /// @return Returns the created ColourValue.
            static ColourValue GetYellow();
            /// @brief Creates a ColourValue representing the colour Cyan.
            /// @return Returns the created ColourValue.
            static ColourValue GetCyan();
            /// @brief Creates a ColourValue representing the colour Magenta.
            /// @return Returns the created ColourValue.
            static ColourValue GetMagenta();
    };//colorvalue
}//phys

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef PHYSXML

/// @brief Serializes the passed phys::ColourValue to XML
/// @param stream The ostream to send the xml to.
/// @param Ev the phys::ColourValue to be serialized
/// @return this returns the ostream, now with the serialized data
std::ostream& PHYS_LIB operator << (std::ostream& stream, const phys::ColourValue& Ev);

/// @brief Deserialize a phys::ColourValue
/// @param stream The istream to get the xml from to (re)make the phys::ColourValue.
/// @param Ev the phys::ColourValue to be deserialized.
/// @return this returns the ostream, advanced past the phys::ColourValue that was recreated onto Ev.
std::istream& PHYS_LIB operator >> (std::istream& stream, phys::ColourValue& Ev);

/// @brief Set all values of a phys::ColourValue from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the phys::ColourValue.
/// @param Ev the phys::ColourValue to be reset.
/// @return This returns thexml::Node that was passed in.
phys::xml::Node& PHYS_LIB operator >> (const phys::xml::Node& OneNode, phys::ColourValue& Ev);

#endif // \PHYSXML


#endif
