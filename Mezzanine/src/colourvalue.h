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
            bool operator== (const ColourValue &Colour) const;
            /// @brief Inequality Comparison Operator
            /// @param Colour This is another ColourValue to compare with.
            /// @return False if the colors match perfectly, True otherwise
            bool operator!= (const ColourValue &Colour) const;
            /// @brief Assignment operator.
            /// @param OtherColour The colour values you want to overwrite this colour's values with.
            void operator= (const ColourValue &OtherColour);

            ///////////////////////////////////////////////////////////////////////////////
            // Prefab Colour fetchers

            /// @brief Creates a ColourValue representing no colour.
            /// @return Returns the created ColourValue.
            static ColourValue Transparent();

            ///////////////////////////////////////////////////////////////////////////////
            // X11 Colour Prefabs

            /// @brief Creates a ColourValue representing the colour AliceBlue.
            /// @return Returns the created ColourValue.
            static ColourValue AliceBlue();
            /// @brief Creates a ColourValue representing the colour AntiqueWhite.
            /// @return Returns the created ColourValue.
            static ColourValue AntiqueWhite();
            /// @brief Creates a ColourValue representing the colour Aqua.
            /// @return Returns the created ColourValue.
            static ColourValue Aqua();
            /// @brief Creates a ColourValue representing the colour Aquamarine.
            /// @return Returns the created ColourValue.
            static ColourValue Aquamarine();
            /// @brief Creates a ColourValue representing the colour Azure.
            /// @return Returns the created ColourValue.
            static ColourValue Azure();
            /// @brief Creates a ColourValue representing the colour Beige.
            /// @return Returns the created ColourValue.
            static ColourValue Beige();
            /// @brief Creates a ColourValue representing the colour Bisque.
            /// @return Returns the created ColourValue.
            static ColourValue Bisque();
            /// @brief Creates a ColourValue representing the colour Black.
            /// @return Returns the created ColourValue.
            static ColourValue Black();
            /// @brief Creates a ColourValue representing the colour Blanchedalmond.
            /// @return Returns the created ColourValue.
            static ColourValue Blanchedalmond();
            /// @brief Creates a ColourValue representing the colour Blue.
            /// @return Returns the created ColourValue.
            static ColourValue Blue();
            /// @brief Creates a ColourValue representing the colour BlueViolet.
            /// @return Returns the created ColourValue.
            static ColourValue BlueViolet();
            /// @brief Creates a ColourValue representing the colour Brown.
            /// @return Returns the created ColourValue.
            static ColourValue Brown();
            /// @brief Creates a ColourValue representing the colour BurlyWood.
            /// @return Returns the created ColourValue.
            static ColourValue BurlyWood();
            /// @brief Creates a ColourValue representing the colour CadetBlue.
            /// @return Returns the created ColourValue.
            static ColourValue CadetBlue();
            /// @brief Creates a ColourValue representing the colour Chartreuse.
            /// @return Returns the created ColourValue.
            static ColourValue Chartreuse();
            /// @brief Creates a ColourValue representing the colour Chocolate.
            /// @return Returns the created ColourValue.
            static ColourValue Chocolate();
            /// @brief Creates a ColourValue representing the colour Coral.
            /// @return Returns the created ColourValue.
            static ColourValue Coral();
            /// @brief Creates a ColourValue representing the colour CornFlowerBlue.
            /// @return Returns the created ColourValue.
            static ColourValue CornFlowerBlue();
            /// @brief Creates a ColourValue representing the colour CornSilk.
            /// @return Returns the created ColourValue.
            static ColourValue CornSilk();
            /// @brief Creates a ColourValue representing the colour Crimson.
            /// @return Returns the created ColourValue.
            static ColourValue Crimson();
            /// @brief Creates a ColourValue representing the colour Cyan.
            /// @return Returns the created ColourValue.
            static ColourValue Cyan();
            /// @brief Creates a ColourValue representing the colour DarkBlue.
            /// @return Returns the created ColourValue.
            static ColourValue DarkBlue();
            /// @brief Creates a ColourValue representing the colour DarkCyan.
            /// @return Returns the created ColourValue.
            static ColourValue DarkCyan();
            /// @brief Creates a ColourValue representing the colour DarkGoldenRod.
            /// @return Returns the created ColourValue.
            static ColourValue DarkGoldenRod();
            /// @brief Creates a ColourValue representing the colour DarkGray.
            /// @return Returns the created ColourValue.
            static ColourValue DarkGray();
            /// @brief Creates a ColourValue representing the colour DarkGreen.
            /// @return Returns the created ColourValue.
            static ColourValue DarkGreen();
            /// @brief Creates a ColourValue representing the colour DarkKhaki.
            /// @return Returns the created ColourValue.
            static ColourValue DarkKhaki();
            /// @brief Creates a ColourValue representing the colour DarkMagenta.
            /// @return Returns the created ColourValue.
            static ColourValue DarkMagenta();
            /// @brief Creates a ColourValue representing the colour DarkOliveGreen.
            /// @return Returns the created ColourValue.
            static ColourValue DarkOliveGreen();
            /// @brief Creates a ColourValue representing the colour DarkOrange.
            /// @return Returns the created ColourValue.
            static ColourValue DarkOrange();
            /// @brief Creates a ColourValue representing the colour DarkOrchid.
            /// @return Returns the created ColourValue.
            static ColourValue DarkOrchid();
            /// @brief Creates a ColourValue representing the colour DarkRed.
            /// @return Returns the created ColourValue.
            static ColourValue DarkRed();
            /// @brief Creates a ColourValue representing the colour DarkSalmon.
            /// @return Returns the created ColourValue.
            static ColourValue DarkSalmon();
            /// @brief Creates a ColourValue representing the colour DarkSeaGreen.
            /// @return Returns the created ColourValue.
            static ColourValue DarkSeaGreen();
            /// @brief Creates a ColourValue representing the colour DarkSlateBlue.
            /// @return Returns the created ColourValue.
            static ColourValue DarkSlateBlue();
            /// @brief Creates a ColourValue representing the colour DarkSlateGray.
            /// @return Returns the created ColourValue.
            static ColourValue DarkSlateGray();
            /// @brief Creates a ColourValue representing the colour DarkTurquoise.
            /// @return Returns the created ColourValue.
            static ColourValue DarkTurquoise();
            /// @brief Creates a ColourValue representing the colour DarkViolet.
            /// @return Returns the created ColourValue.
            static ColourValue DarkViolet();
            /// @brief Creates a ColourValue representing the colour DeepPink.
            /// @return Returns the created ColourValue.
            static ColourValue DeepPink();
            /// @brief Creates a ColourValue representing the colour DeepSkyBlue.
            /// @return Returns the created ColourValue.
            static ColourValue DeepSkyBlue();
            /// @brief Creates a ColourValue representing the colour DimGray.
            /// @return Returns the created ColourValue.
            static ColourValue DimGray();
            /// @brief Creates a ColourValue representing the colour DodgerBlue.
            /// @return Returns the created ColourValue.
            static ColourValue DodgerBlue();
            /// @brief Creates a ColourValue representing the colour FireBrick.
            /// @return Returns the created ColourValue.
            static ColourValue FireBrick();
            /// @brief Creates a ColourValue representing the colour FloralWhite.
            /// @return Returns the created ColourValue.
            static ColourValue FloralWhite();
            /// @brief Creates a ColourValue representing the colour ForestGreen.
            /// @return Returns the created ColourValue.
            static ColourValue ForestGreen();
            /// @brief Creates a ColourValue representing the colour Fuchsia.
            /// @return Returns the created ColourValue.
            static ColourValue Fuchsia();
            /// @brief Creates a ColourValue representing the colour Gainsboro.
            /// @return Returns the created ColourValue.
            static ColourValue Gainsboro();
            /// @brief Creates a ColourValue representing the colour GhostWhite.
            /// @return Returns the created ColourValue.
            static ColourValue GhostWhite();
            /// @brief Creates a ColourValue representing the colour Gold.
            /// @return Returns the created ColourValue.
            static ColourValue Gold();
            /// @brief Creates a ColourValue representing the colour GoldenRod.
            /// @return Returns the created ColourValue.
            static ColourValue GoldenRod();
            /// @brief Creates a ColourValue representing the colour Gray.
            /// @return Returns the created ColourValue.
            static ColourValue Gray();
            /// @brief Creates a ColourValue representing the colour Green.
            /// @return Returns the created ColourValue.
            static ColourValue Green();
            /// @brief Creates a ColourValue representing the colour GreenYellow.
            /// @return Returns the created ColourValue.
            static ColourValue GreenYellow();
            /// @brief Creates a ColourValue representing the colour HoneyDew.
            /// @return Returns the created ColourValue.
            static ColourValue HoneyDew();
            /// @brief Creates a ColourValue representing the colour HotPink.
            /// @return Returns the created ColourValue.
            static ColourValue HotPink();
            /// @brief Creates a ColourValue representing the colour IndianRed.
            /// @return Returns the created ColourValue.
            static ColourValue IndianRed();
            /// @brief Creates a ColourValue representing the colour Indigo.
            /// @return Returns the created ColourValue.
            static ColourValue Indigo();
            /// @brief Creates a ColourValue representing the colour Ivory.
            /// @return Returns the created ColourValue.
            static ColourValue Ivory();
            /// @brief Creates a ColourValue representing the colour Khaki.
            /// @return Returns the created ColourValue.
            static ColourValue Khaki();
            /// @brief Creates a ColourValue representing the colour Lavender.
            /// @return Returns the created ColourValue.
            static ColourValue Lavender();
            /// @brief Creates a ColourValue representing the colour LavenderBlush.
            /// @return Returns the created ColourValue.
            static ColourValue LavenderBlush();
            /// @brief Creates a ColourValue representing the colour LawnGreen.
            /// @return Returns the created ColourValue.
            static ColourValue LawnGreen();
            /// @brief Creates a ColourValue representing the colour LemonChiffon.
            /// @return Returns the created ColourValue.
            static ColourValue LemonChiffon();
            /// @brief Creates a ColourValue representing the colour LightBlue.
            /// @return Returns the created ColourValue.
            static ColourValue LightBlue();
            /// @brief Creates a ColourValue representing the colour LightCoral.
            /// @return Returns the created ColourValue.
            static ColourValue LightCoral();
            /// @brief Creates a ColourValue representing the colour LightCyan.
            /// @return Returns the created ColourValue.
            static ColourValue LightCyan();
            /// @brief Creates a ColourValue representing the colour LightGoldenRodYellow.
            /// @return Returns the created ColourValue.
            static ColourValue LightGoldenRodYellow();
            /// @brief Creates a ColourValue representing the colour LightGray.
            /// @return Returns the created ColourValue.
            static ColourValue LightGray();
            /// @brief Creates a ColourValue representing the colour LightGreen.
            /// @return Returns the created ColourValue.
            static ColourValue LightGreen();
            /// @brief Creates a ColourValue representing the colour LightPink.
            /// @return Returns the created ColourValue.
            static ColourValue LightPink();
            /// @brief Creates a ColourValue representing the colour LightSalmon.
            /// @return Returns the created ColourValue.
            static ColourValue LightSalmon();
            /// @brief Creates a ColourValue representing the colour LightSeaGreen.
            /// @return Returns the created ColourValue.
            static ColourValue LightSeaGreen();
            /// @brief Creates a ColourValue representing the colour LightSkyBlue.
            /// @return Returns the created ColourValue.
            static ColourValue LightSkyBlue();
            /// @brief Creates a ColourValue representing the colour LightSlateGray.
            /// @return Returns the created ColourValue.
            static ColourValue LightSlateGray();
            /// @brief Creates a ColourValue representing the colour LightSteelBlue.
            /// @return Returns the created ColourValue.
            static ColourValue LightSteelBlue();
            /// @brief Creates a ColourValue representing the colour LightYellow.
            /// @return Returns the created ColourValue.
            static ColourValue LightYellow();
            /// @brief Creates a ColourValue representing the colour Lime.
            /// @return Returns the created ColourValue.
            static ColourValue Lime();
            /// @brief Creates a ColourValue representing the colour LimeGreen.
            /// @return Returns the created ColourValue.
            static ColourValue LimeGreen();
            /// @brief Creates a ColourValue representing the colour Linen.
            /// @return Returns the created ColourValue.
            static ColourValue Linen();
            /// @brief Creates a ColourValue representing the colour Magenta.
            /// @return Returns the created ColourValue.
            static ColourValue Magenta();
            /// @brief Creates a ColourValue representing the colour Maroon.
            /// @return Returns the created ColourValue.
            static ColourValue Maroon();
            /// @brief Creates a ColourValue representing the colour MediumAquamarine.
            /// @return Returns the created ColourValue.
            static ColourValue MediumAquamarine();
            /// @brief Creates a ColourValue representing the colour MediumBlue.
            /// @return Returns the created ColourValue.
            static ColourValue MediumBlue();
            /// @brief Creates a ColourValue representing the colour MediumOrchid.
            /// @return Returns the created ColourValue.
            static ColourValue MediumOrchid();
            /// @brief Creates a ColourValue representing the colour MediumPurple.
            /// @return Returns the created ColourValue.
            static ColourValue MediumPurple();
            /// @brief Creates a ColourValue representing the colour MediumSeaGreen.
            /// @return Returns the created ColourValue.
            static ColourValue MediumSeaGreen();
            /// @brief Creates a ColourValue representing the colour MediumSlateBlue.
            /// @return Returns the created ColourValue.
            static ColourValue MediumSlateBlue();
            /// @brief Creates a ColourValue representing the colour MediumSpringGreen.
            /// @return Returns the created ColourValue.
            static ColourValue MediumSpringGreen();
            /// @brief Creates a ColourValue representing the colour MediumTurquoise.
            /// @return Returns the created ColourValue.
            static ColourValue MediumTurquoise();
            /// @brief Creates a ColourValue representing the colour MediumVioletRed.
            /// @return Returns the created ColourValue.
            static ColourValue MediumVioletRed();
            /// @brief Creates a ColourValue representing the colour MidnightBlue.
            /// @return Returns the created ColourValue.
            static ColourValue MidnightBlue();
            /// @brief Creates a ColourValue representing the colour MintCream.
            /// @return Returns the created ColourValue.
            static ColourValue MintCream();
            /// @brief Creates a ColourValue representing the colour MistyRose.
            /// @return Returns the created ColourValue.
            static ColourValue MistyRose();
            /// @brief Creates a ColourValue representing the colour Moccasin.
            /// @return Returns the created ColourValue.
            static ColourValue Moccasin();
            /// @brief Creates a ColourValue representing the colour NavajoWhite.
            /// @return Returns the created ColourValue.
            static ColourValue NavajoWhite();
            /// @brief Creates a ColourValue representing the colour Navy.
            /// @return Returns the created ColourValue.
            static ColourValue Navy();
            /// @brief Creates a ColourValue representing the colour OldLace.
            /// @return Returns the created ColourValue.
            static ColourValue OldLace();
            /// @brief Creates a ColourValue representing the colour Olive.
            /// @return Returns the created ColourValue.
            static ColourValue Olive();
            /// @brief Creates a ColourValue representing the colour OliveDrab.
            /// @return Returns the created ColourValue.
            static ColourValue OliveDrab();
            /// @brief Creates a ColourValue representing the colour Orange.
            /// @return Returns the created ColourValue.
            static ColourValue Orange();
            /// @brief Creates a ColourValue representing the colour OrangeRed.
            /// @return Returns the created ColourValue.
            static ColourValue OrangeRed();
            /// @brief Creates a ColourValue representing the colour Orchid.
            /// @return Returns the created ColourValue.
            static ColourValue Orchid();
            /// @brief Creates a ColourValue representing the colour PaleGoldenRod.
            /// @return Returns the created ColourValue.
            static ColourValue PaleGoldenRod();
            /// @brief Creates a ColourValue representing the colour PaleGreen.
            /// @return Returns the created ColourValue.
            static ColourValue PaleGreen();
            /// @brief Creates a ColourValue representing the colour PaleTurquoise.
            /// @return Returns the created ColourValue.
            static ColourValue PaleTurquoise();
            /// @brief Creates a ColourValue representing the colour PaleVioletRed.
            /// @return Returns the created ColourValue.
            static ColourValue PaleVioletRed();
            /// @brief Creates a ColourValue representing the colour PapayaWhip.
            /// @return Returns the created ColourValue.
            static ColourValue PapayaWhip();
            /// @brief Creates a ColourValue representing the colour PeachPuff.
            /// @return Returns the created ColourValue.
            static ColourValue PeachPuff();
            /// @brief Creates a ColourValue representing the colour Peru.
            /// @return Returns the created ColourValue.
            static ColourValue Peru();
            /// @brief Creates a ColourValue representing the colour Pink.
            /// @return Returns the created ColourValue.
            static ColourValue Pink();
            /// @brief Creates a ColourValue representing the colour Plum.
            /// @return Returns the created ColourValue.
            static ColourValue Plum();
            /// @brief Creates a ColourValue representing the colour PowderBlue.
            /// @return Returns the created ColourValue.
            static ColourValue PowderBlue();
            /// @brief Creates a ColourValue representing the colour Purple.
            /// @return Returns the created ColourValue.
            static ColourValue Purple();
            /// @brief Creates a ColourValue representing the colour Red.
            /// @return Returns the created ColourValue.
            static ColourValue Red();
            /// @brief Creates a ColourValue representing the colour RosyBrown.
            /// @return Returns the created ColourValue.
            static ColourValue RosyBrown();
            /// @brief Creates a ColourValue representing the colour RoyalBlue.
            /// @return Returns the created ColourValue.
            static ColourValue RoyalBlue();
            /// @brief Creates a ColourValue representing the colour SaddleBrown.
            /// @return Returns the created ColourValue.
            static ColourValue SaddleBrown();
            /// @brief Creates a ColourValue representing the colour Salmon.
            /// @return Returns the created ColourValue.
            static ColourValue Salmon();
            /// @brief Creates a ColourValue representing the colour SandyBrown.
            /// @return Returns the created ColourValue.
            static ColourValue SandyBrown();
            /// @brief Creates a ColourValue representing the colour SeaGreen.
            /// @return Returns the created ColourValue.
            static ColourValue SeaGreen();
            /// @brief Creates a ColourValue representing the colour SeaShell.
            /// @return Returns the created ColourValue.
            static ColourValue SeaShell();
            /// @brief Creates a ColourValue representing the colour Sienna.
            /// @return Returns the created ColourValue.
            static ColourValue Sienna();
            /// @brief Creates a ColourValue representing the colour Silver.
            /// @return Returns the created ColourValue.
            static ColourValue Silver();
            /// @brief Creates a ColourValue representing the colour SkyBlue.
            /// @return Returns the created ColourValue.
            static ColourValue SkyBlue();
            /// @brief Creates a ColourValue representing the colour SlateBlue.
            /// @return Returns the created ColourValue.
            static ColourValue SlateBlue();
            /// @brief Creates a ColourValue representing the colour SlateGray.
            /// @return Returns the created ColourValue.
            static ColourValue SlateGray();
            /// @brief Creates a ColourValue representing the colour Snow.
            /// @return Returns the created ColourValue.
            static ColourValue Snow();
            /// @brief Creates a ColourValue representing the colour SpringGreen.
            /// @return Returns the created ColourValue.
            static ColourValue SpringGreen();
            /// @brief Creates a ColourValue representing the colour SteelBlue.
            /// @return Returns the created ColourValue.
            static ColourValue SteelBlue();
            /// @brief Creates a ColourValue representing the colour Tan.
            /// @return Returns the created ColourValue.
            static ColourValue Tan();
            /// @brief Creates a ColourValue representing the colour Teal.
            /// @return Returns the created ColourValue.
            static ColourValue Teal();
            /// @brief Creates a ColourValue representing the colour Thistle.
            /// @return Returns the created ColourValue.
            static ColourValue Thistle();
            /// @brief Creates a ColourValue representing the colour Tomato.
            /// @return Returns the created ColourValue.
            static ColourValue Tomato();
            /// @brief Creates a ColourValue representing the colour Turquoise.
            /// @return Returns the created ColourValue.
            static ColourValue Turquoise();
            /// @brief Creates a ColourValue representing the colour Violet.
            /// @return Returns the created ColourValue.
            static ColourValue Violet();
            /// @brief Creates a ColourValue representing the colour Wheat.
            /// @return Returns the created ColourValue.
            static ColourValue Wheat();
            /// @brief Creates a ColourValue representing the colour White.
            /// @return Returns the created ColourValue.
            static ColourValue White();
            /// @brief Creates a ColourValue representing the colour WhiteSmoke.
            /// @return Returns the created ColourValue.
            static ColourValue WhiteSmoke();
            /// @brief Creates a ColourValue representing the colour Yellow.
            /// @return Returns the created ColourValue.
            static ColourValue Yellow();
            /// @brief Creates a ColourValue representing the colour YellowGreen.
            /// @return Returns the created ColourValue.
            static ColourValue YellowGreen();

#ifdef MEZZXML
            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            // Serializable
            /// @brief Convert this class to an XML::Node ready for serialization
            /// @param CurrentRoot The point in the XML hierarchy that all this quaternion should be appended to.
            virtual void ProtoSerialize(XML::Node& CurrentRoot) const;

            // DeSerializable
            /// @brief Take the data stored in an XML and overwrite this instance of this object with it
            /// @param OneNode and XML::Node containing the data.
            virtual void ProtoDeSerialize(const XML::Node& OneNode);

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
void MEZZ_LIB operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::ColourValue& Ev);

#endif // \MEZZXML


#endif
