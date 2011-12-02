//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _colourvalue_h
#define _colourvalue_h

#include "crossplatformexport.h"
#include "datatypes.h"
#include "xml.h"

namespace Ogre
{
    class ColourValue;
}

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ColourValue
    /// @headerfile colourvalue.h
    /// @brief This is a simple class for holding 4 reals representing the colour any give object or lightsource can have.
    ///////////////////////////////////////
    class MEZZ_LIB ColourValue
    {
        public:
            /// @brief Value from 0.0 to 1.0 representing the amount of red present in the colour. 1.0 if very red, 0.0 is no red.
            Real R;
            /// @brief Value from 0.0 to 1.0 representing the amount of green present in the colour. 1.0 if very green, 0.0 is no green.
            Real G;
            /// @brief Value from 0.0 to 1.0 representing the amount of blue present in the colour. 1.0 if very blue, 0.0 is no blue.
            Real B;
            /// @brief Value from 0.0 to 1.0 representing the transparency of the colours. 1.0 is opaque and 0.0 is clear.
            Real A;

/*
            /// @brief Non-Alpha constructor.
            /// @details This constructor assumes the alpha value to be 1.0 (completely opaque).
            /// @param red Real representing the amount of red present in the colour.
            /// @param green Real representing the amount of green present in the colour.
            /// @param blue Real representing the amount of blue present in the colour.
            ColourValue(Real red, Real green, Real blue);
*/
            /// @brief 4 Reals constructor.
            /// @details This constructor allows you to set any values, if unset, they default to 1.0.
            /// @param Red Real representing the amount of red present in the colour.
            /// @param Green Real representing the amount of green present in the colour.
            /// @param Blue Real representing the amount of blue present in the colour.
            /// @param Alpha Real representing the transparency of the colours.
            ColourValue(Real Red=1.0, Real Green=1.0, Real Blue=1.0, Real Alpha=1.0);

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

            ///////////////////////////////////////////////////////////////////////////////
            // Prefab Colour fetchers

            /// @brief Creates a ColourValue representing no colour.
            /// @return Returns the created ColourValue.
            static ColourValue Transparent();
            /// @brief Creates a ColourValue representing the colour White.
            /// @return Returns the created ColourValue.
            static ColourValue White();
            /// @brief Creates a ColourValue representing the colour Black.
            /// @return Returns the created ColourValue.
            static ColourValue Black();

            /// @brief Creates a ColourValue representing the colour Red.
            /// @return Returns the created ColourValue.
            static ColourValue Red();
            /// @brief Creates a ColourValue representing the colour Green.
            /// @return Returns the created ColourValue.
            static ColourValue Green();
            /// @brief Creates a ColourValue representing the colour Blue.
            /// @return Returns the created ColourValue.
            static ColourValue Blue();

            /// @brief Creates a ColourValue representing the colour Yellow.
            /// @return Returns the created ColourValue.
            static ColourValue Yellow();
            /// @brief Creates a ColourValue representing the colour Cyan.
            /// @return Returns the created ColourValue.
            static ColourValue Cyan();
            /// @brief Creates a ColourValue representing the colour Magenta.
            /// @return Returns the created ColourValue.
            static ColourValue Magenta();

#ifdef MEZZXML
            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            // Serializable
            /// @brief Convert this class to an xml::Node ready for serialization
            /// @param CurrentRoot The point in the XML hierarchy that all this quaternion should be appended to.
            virtual void ProtoSerialize(xml::Node& CurrentRoot) const;

            // DeSerializable
            /// @brief Take the data stored in an XML and overwrite this instance of this object with it
            /// @param OneNode and xml::Node containing the data.
            virtual void ProtoDeSerialize(const xml::Node& OneNode);

            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "ColourValue"
            static String SerializableName();
#endif

    };//colorvalue
}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef MEZZXML

/// @brief Serializes the passed Mezzanine::ColourValue to XML
/// @param stream The ostream to send the xml to.
/// @param Ev the Mezzanine::ColourValue to be serialized
/// @return this returns the ostream, now with the serialized data
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::ColourValue& Ev);

/// @brief Deserialize a Mezzanine::ColourValue
/// @param stream The istream to get the xml from to (re)make the Mezzanine::ColourValue.
/// @param Ev the Mezzanine::ColourValue to be deserialized.
/// @return this returns the ostream, advanced past the Mezzanine::ColourValue that was recreated onto Ev.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::ColourValue& Ev);

/// @brief Set all values of a Mezzanine::ColourValue from parsed xml.
/// @param OneNode The istream to get the xml from to (re)make the Mezzanine::ColourValue.
/// @param Ev the Mezzanine::ColourValue to be reset.
void MEZZ_LIB operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::ColourValue& Ev);

#endif // \MEZZXML


#endif
