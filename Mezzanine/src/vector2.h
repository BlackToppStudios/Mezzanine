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
#ifndef _vector2_h
#define _vector2_h

#include "crossplatformexport.h"
#include "datatypes.h"
#ifndef SWIG
    #include "XML/xml.h"
    #include <limits>
#endif

namespace Ogre
{
    class Vector2;
}

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class Vector2
    /// @headerfile vector2.h
    /// @brief This is used to represent a point on a 2 dimentional area, such as a screen.
    /// @details This contains an X and Y value used to represent coordinates.
    /// This also has a number of facilities to make converting to graphics subsystems
    /// as easy as possible.
    ///////////////////////////////////////
    class MEZZ_LIB Vector2
    {
    public:
        /// @brief Coordinate on the X vector.
        Real X;
        /// @brief Coordinate on the Y vector.
        Real Y;

        /// @brief Default Constructor.
        Vector2();
        /// @brief Single Real value Constructor.
        /// @param xy Value to set both x and y to.
        Vector2(const Real& xy);
        /// @brief Real value Constructor.
        /// @param x Coordinate on the X vector.
        /// @param y Coordinate on the Y vector.
        Vector2(const Real& x, const Real& y);
        /// @brief Ogre Value Constructor.
        /// @param Vec The vector to be copied to make this vector.
        Vector2(const Ogre::Vector2& Vec);

        /// @brief Gets a Ogre vector2.
        /// @return Returns an Ogre Vector2 with the same values as this.
        Ogre::Vector2 GetOgreVector2() const;
        /// @brief Copies an existing Ogre vector2.
        /// @param Thiers The vector2 to be extracted.
        void ExtractOgreVector2(const Ogre::Vector2& Thiers);

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Sets the values of this vector2 to identity values(0,0).
        void SetIdentity();
        /// @brief Sets the X and Y values of this vector2.
        /// @param x The real that will have this vector's X member set to.
        /// @param y The real that will have this vector's Y member set to.
        void SetValues(const Real& x, const Real& y);
        ///////////////////////////////////////////////////////////////////////////////
        // Equality Comparison operators

        /// @brief Equality Comparison Operator.
        /// @param Vec2 This is the other Mezzanine::Vector2 to compare with.
        /// @return Returns true if X==X and Y==Y, otherwise returns false.
        Boolean operator==(const Mezzanine::Vector2& Vec2) const;
        /// @brief Equality Comparison Operator.
        /// @param Vec2 This is the other Mezzanine::Vector2.
        /// @return Returns true if X!=X or Y!=Y, otherwise returns false.
        Boolean operator!=(const Mezzanine::Vector2& Vec2) const;
        /// @brief Equality Comparison Operator.
        /// @param Vec2 This is the other Ogre::Vector2.
        /// @return Returns true if X==X and Y==Y, otherwise returns false.
        Boolean operator==(const Ogre::Vector2& Vec2) const;
        /// @brief Equality Comparison Operator.
        /// @param Vec2 This is the other Ogre::Vector2.
        /// @return Returns true if X!=X or Y!=Y, otherwise returns false.
        Boolean operator!=(const Ogre::Vector2& Vec2) const;

        /// @brief Less or Equal Comparison Operator.
        /// @details Returns true if X<=X and Y<=Y. If any of those do not hold this returns false.
        /// @param Vec This is the other Mezzanine::Vector2.
        /// @note Used primarily for testing. This is not implement for use with other kinds of Vector3 implementations as it is widely considered useless.
        Boolean operator<= (const Mezzanine::Vector2 &Vec) const;
        /// @brief Greater than or Equal Comparison Operator.
        /// @details Returns true if X>=X and Y>=Y . If any of those do not hold this returns false.
        /// @param Vec This is the other Mezzanine::Vector2.
        /// @note Used primarily for testing. This is not implement for use with other kinds of Vector3 implementations as it is widely considered useless.
        Boolean operator>= (const Mezzanine::Vector2 &Vec) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Vector2 Arithmetic with Real

        /// @brief Scaling by multiplication.
        /// @return This returns a Vector2 that has been scaled.
        /// @param scalar This is the amount to scale the Vector2 by.
        Vector2 operator* (const Real& scalar) const;
        /// @brief Scaling by Division.
        /// @return This returns a Vector2 that has been scaled.
        /// @param scalar This is the amount to scale the Vector2 by.
        Vector2 operator/ (const Real& scalar) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Vector2 Arithmetic and assignment with Real

        /// @brief Scaling by multiplication.
        /// @param scalar This is the amount to scale the Vector2 by.
        /// @return Returns a reference to this.
        Vector2& operator*= (const Real& scalar);
        /// @brief Scaling by Division.
        /// @param scalar This is the amount to scale the Vector2 by.
        /// @return Returns a reference to this.
        Vector2& operator/= (const Real& scalar);

        ///////////////////////////////////////////////////////////////////////////////
        // Arithmetic Operators

        /// @brief Addition Operator.
        /// @param Vec2 The other Vector2 to add to this.
        /// @return Returns a new Vector2 that is the result of this operation.
        Vector2 operator+ (const Vector2& Vec2) const;
        /// @brief Subraction Operator.
        /// @param Vec2 The other Vector2 to subtract from this.
        /// @return Returns a new Vector2 that is the result of this operation.
        Vector2 operator- (const Vector2& Vec2) const;
        /// @brief Multiplaction Operator.
        /// @param Vec2 The other Vector2 to multiply by this.
        /// @return Returns a new Vector2 that is the result of this operation.
        Vector2 operator* (const Vector2& Vec2) const;
        /// @brief Division Operator.
        /// @param Vec2 The other Vector2 to divide by.
        /// @return Returns a new Vector2 that is the result of this operation.
        Vector2 operator/ (const Vector2& Vec2) const;

        /// @brief Addition assignment Operator.
        /// @param Vec2 The other Vector2 to add to this.
        /// @return Returns a reference to this.
        Vector2& operator+= (const Vector2& Vec2);
        /// @brief Subraction assignment Operator.
        /// @param Vec2 The other Vector2 to subtract from this.
        /// @return Returns a reference to this.
        Vector2& operator-= (const Vector2& Vec2);
        /// @brief Multiplaction assignment Operator.
        /// @param Vec2 The other Vector2 to multiply by this.
        /// @return Returns a reference to this.
        Vector2& operator*= (const Vector2& Vec2);
        /// @brief Division assignment Operator.
        /// @param Vec2 The other Vector2 to divide by.
        /// @return Returns a reference to this.
        Vector2& operator/= (const Vector2& Vec2);

        ///////////////////////////////////////////////////////////////////////////////
        // Fancy Math

        /// @brief Generates a Vector2 that is perpendicular to this vector.
        /// @return Returns a new Vector2 that is perpendicular to this.
        Vector2 Perpendicular() const;
        /// @brief Normalizes this Vector2.
        /// @return Returns a reference to this.
        Vector2& Normalize();

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        /// @brief Convert this class to an XML::Node ready for serialization
        /// @param CurrentRoot The point in the XML hierarchy that all this vector2 should be appended to.
        void ProtoSerialize(XML::Node& CurrentRoot) const;
        /// @brief Take the data stored in an XML and overwrite this instance of this object with it
        /// @param OneNode and XML::Node containing the data.
        void ProtoDeSerialize(const XML::Node& OneNode);

        /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
        /// @return A string containing "Vector2"
        static String SerializableName();
    };//Vector2
}//Mezzanine

// We can skip these operators when creating bindings with swig
#ifndef SWIG

/// @brief Used to Serialize an Mezzanine::Vector2 to a human readable stream
/// @details Sends proper XML to the output stream,
/// including versioning information which will be used to maintain backwards compatibility. The current XML format
/// will create one element with no sub-elements. The name of the xml node will be "Vector2". It will have 3 attributes.
/// "Version", will be set to a value of 1, indicating if came from version 1 compatible Vector2. It will also have an "X" and "Y"
/// attributes with values set appropriately. For example '<Vector2 Version="1" X="1" Y="2" />'.
/// @param x The Mezzanine::Vector2 to be converted to a stream of characters.
/// @param stream The place to send the characters, that define the Mezzanine::Vector2.
/// @return Get an std::ostream that was written to, this allow chaining of the << operators.
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Vector2& x);

/// @brief Used to de-serialize an Mezzanine::Vector2 from a stream
/// @details This reads in the xml and sets the target vector according to values from the stream.
/// @param Vec The Mezzanine::Vector2 that will accept the values from the xml
/// @param stream The place to get the characters from, that define the Mezzanine::Vector2.
/// @return Get an std::ostream that was read from, this allow chaining of the >> operators.
/// @throw Can throw any exception that any function in the Mezzanine::xml namespace could throw in addition to a Mezzanine::Exception if the serialization version doesn't match.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Vector2& Vec);

/// @brief Converts an XML node into a Mezzanine::Vector2
/// @details This will convert an XML::Node will a valid serialized Mezzanine::Vector2 into a Mezzanine::Vector2
/// @param OneNode An XML Node containing the the text of a Vector2
/// @param Vec the Mezzanine::Vector2 to store the deserialized Vector2
/// @return This returns a reference to the XML::Node for operator chaining or whatever.
/// @throw Can throw any exception that any function in the Mezzanine::xml namespace could throw in addition to a Mezzanine::Exception if the serialization version doesn't match.
void MEZZ_LIB  operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Vector2& Vec);

#endif // \SWIG

namespace std
{
    /// @brief Get Numeric details on Vector2
    template<>
    class numeric_limits<Mezzanine::Vector2>
    {
        public:
            /// @brief Does this class (numeric_limits<Mezzanine::Vector2>) exist
            static const bool is_specialized = true;
            /// @brief Does this support negative values?
            static const bool is_signed = true;
            /// @brief Can this only store integer types.
            static const bool is_integer = false;
            /// @brief The Vector2 uses Real, which is typically a machine dependedant which can be inexact
            static const bool is_exact = std::numeric_limits<Mezzanine::Real>::is_exact;
            /// @brief Can This represent an infinitely large value in X, Y or Z?
            static const bool has_infinity = std::numeric_limits<Mezzanine::Real>::has_infinity;
            /// @brief ??? Required by std::numeric to be compliant
            /// @todo Learn why this exists and document it.
            static const bool has_quiet_NaN = std::numeric_limits<Mezzanine::Real>::has_quiet_NaN;
            /// @brief ??? Required by std::numeric to be compliant
            /// @todo Learn why this exists and document it.
            static const bool has_signaling_NaN = std::numeric_limits<Mezzanine::Real>::has_signaling_NaN;
            /// @brief Does this support exceptionally small numbers near 0?
            static const std::float_denorm_style has_denorm = std::numeric_limits<Mezzanine::Real>::has_denorm;
            /// @brief When extra precision near 0 is lost, can this type distinguish that from other imprecision.
            static const bool has_denorm_loss = std::numeric_limits<Mezzanine::Real>::has_denorm_loss;
            /// @brief How items that fit between the precise amount a Real can represent will be adapted.
            static const std::float_round_style round_style = std::numeric_limits<Mezzanine::Real>::round_style;
            /// @brief Do X, Y and Z adhere to iec 559?
            static const bool is_iec559 = std::numeric_limits<Mezzanine::Real>::is_iec559;
            /// @brief Is overflow of this type handle by modulo overflow?
            static const bool is_modulo = std::numeric_limits<Mezzanine::Real>::is_modulo;
            /// @brief How many integer digits(in machine base) of precision can this handle in each X, Y or Z without floating point component or error?
            static const int digits = std::numeric_limits<Mezzanine::Real>::digits;
            /// @brief How many integer digits in base 10 of precision can this handle in each X, Y or Z without floating point component or error?
            static const int digits10 = std::numeric_limits<Mezzanine::Real>::digits10;
            /// @brief The base of the number system that this is implemented in
            static const int radix = std::numeric_limits<Mezzanine::Real>::radix;
            /// @brief The smallest power of the radix that is valid floating point value
            static const int min_exponent = std::numeric_limits<Mezzanine::Real>::min_exponent;
            /// @brief The smallest power of 10 that is valid floating point value
            static const int min_exponent10 = std::numeric_limits<Mezzanine::Real>::min_exponent10;
            /// @brief The largest power of the radix that is valid floating point value
            static const int max_exponent = std::numeric_limits<Mezzanine::Real>::max_exponent;
            /// @brief The largest power of 10 that is valid floating point value
            static const int max_exponent10 = std::numeric_limits<Mezzanine::Real>::max_exponent10;
            /// @brief Can this generate a trap?
            static const bool traps = std::numeric_limits<Mezzanine::Real>::traps;
            /// @brief Are tiny values respected during rounding?
            static const bool tinyness_before = std::numeric_limits<Mezzanine::Real>::tinyness_before;

            /// @brief Get the lowest positive finite value this can represent
            /// @return A vector with 2 very small numbers
            static Mezzanine::Vector2 min()
            {
                return Mezzanine::Vector2(std::numeric_limits<Mezzanine::Real>::min(),
                                          std::numeric_limits<Mezzanine::Real>::min()
                                         );
            }

            /// @brief Get the highest positive finite value this can represent
            /// @return A vector with 2 very large numbers
            static Mezzanine::Vector2 max()
            {
                return Mezzanine::Vector2(std::numeric_limits<Mezzanine::Real>::max(),
                                          std::numeric_limits<Mezzanine::Real>::max()
                                         );
            }

            /// @brief The smallest value representable from 1.0,1.0 to the next value
            /// @return A vector with a very small number
            static Mezzanine::Vector2 epsilon()
            {
                return Mezzanine::Vector2(std::numeric_limits<Mezzanine::Real>::epsilon(),
                                          std::numeric_limits<Mezzanine::Real>::epsilon()
                                         );
            }

            /// @brief Get the largest possible rounding error
            /// @return A vector containing 2 values indicating how much they could be rounded.
            static Mezzanine::Vector2 round_error()
            {
                return Mezzanine::Vector2(std::numeric_limits<Mezzanine::Real>::round_error(),
                                          std::numeric_limits<Mezzanine::Real>::round_error()
                                         );
            }

            /// @brief Get the special value "Positive infinity"
            /// @return A vector containing 2 values.
            static Mezzanine::Vector2 infinity()
            {
                return Mezzanine::Vector2(std::numeric_limits<Mezzanine::Real>::infinity(),
                                          std::numeric_limits<Mezzanine::Real>::infinity()
                                         );
            }

            /// @brief Get the special value "Quiet Not actual Number"
            /// @return A vector containing 2 values.
            static Mezzanine::Vector2 quiet_NaN()
            {
                return Mezzanine::Vector2(std::numeric_limits<Mezzanine::Real>::quiet_NaN(),
                                          std::numeric_limits<Mezzanine::Real>::quiet_NaN()
                                         );
            }

            /// @brief Get the special value "Signaling Not actual Number"
            /// @return A vector containing 2 values.
            static Mezzanine::Vector2 signaling_NaN()
            {
                return Mezzanine::Vector2(std::numeric_limits<Mezzanine::Real>::signaling_NaN(),
                                          std::numeric_limits<Mezzanine::Real>::signaling_NaN()
                                         );
            }

            /// @brief Get the closest value to 0 that is not 0 this can represent, including extra precision for being close to 0 if supported.
            /// @return A vector containing 2 very small values.
            static Mezzanine::Vector2 denorm_min()
            {
                return Mezzanine::Vector2(std::numeric_limits<Mezzanine::Real>::denorm_min(),
                                          std::numeric_limits<Mezzanine::Real>::denorm_min()
                                         );
            }

    }; //Numeric Limits

} // std



#endif
