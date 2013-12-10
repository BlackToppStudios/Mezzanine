//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _transform_h
#define _transform_h

#include "vector3.h"
#include "quaternion.h"

///////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief The defintion of the transform is stored in this file.
///////////////////////////////////////

class btTransform;

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Stores information about relative location and rotation in 3d space
    /// @details This is simple a pair of a vector 3 to store location and a
    /// quaternion to store rotation. This is communly used to interact with the
    /// physics system. This also has a few helper functions to make it more
    /// useful than an std::pair<Vector3,Quaternion>.
    ///////////////////////////////////////
    class MEZZ_LIB Transform
    {
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Data Members
            /// @brief The location or relative location is stored in a Vector3
            Vector3 Location;

            /// @brief The Rotation or relative rotation is stored in a Quaternion.
            Quaternion Rotation;

            ///////////////////////////////////////////////////////////////////////////////
            // Creation
            /// @brief The multipurpose constructor
            /// @param Vec The starting Vector#/Location you would like this transform to have. If not passed, a default Vector3 is used.
            /// @param Quat The starting Quaternion/Rotation you would like this transform to have. If not passed, a default Quaternion is used.
            Transform(const Vector3& Vec = Vector3(), const Quaternion& Quat = Quaternion());

            /// @brief The Conversion Constructor
            /// @param Btt The btTransform from bullet physics.
            Transform(const btTransform& Btt);

            /// @brief The copy constructor
            /// @param TheOther Another Transform to be copied
            Transform(const Transform& TheOther);

            ///////////////////////////////////////////////////////////////////////////////
            // Utilities
            /// @brief Sets default construction values for all members.
            void SetIdentity();

            ///////////////////////////////////////////////////////////////////////////////
            // Conversion
            /// @brief Gets a Bullet Transform
            /// @details Creates a Bullet Transform with values equal to this class instance and returns it.
            btTransform GetBulletTransform() const;
            /// @brief Copies an existing Bullet transform
            /// @details This function will copy the values stored in an existing Bullet transform
            /// and set the values of this class instance to be the same.
            /// @param temp The btTransfrom to have its values extracted.
            void ExtractBulletTransform(const btTransform& temp);

            /// @brief Set the values of this Transform to match an existing Transform
            /// @param rhs The item storing the values to copy.
            /// @return A reference to the freshly overwritten Transform.
            Transform& operator= (const Transform& rhs);
            /// @brief Set the values of this Transform to match an existing btTransform.
            /// @param rhs The item storing the values to copy.
            /// @return A reference to the freshly overwritten Transform.
            Transform& operator= (const btTransform& rhs);

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            // Serializable
            /// @brief Convert this class to an XML::Node ready for serialization
            /// @param CurrentRoot The point in the XML hierarchy that all this vectorw should be appended to.
            void ProtoSerialize(XML::Node& CurrentRoot) const;

            // DeSerializable
            /// @brief Take the data stored in an XML and overwrite this instance of this object with it
            /// @param OneNode and XML::Node containing the data.
            /// @warning A precondition of using this is that all of the actors intended for use must already be Deserialized.
            void ProtoDeSerialize(const XML::Node& OneNode);

            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "Transform"
            static String SerializableName();


            ///////////////////////////////////////////////////////////////////////////////
            // Math and test operations
            /// @brief Create a Transform with the difference of this and another
            /// @param rhs The Transform on the right hand side of the sign.
            /// @return A Transform with
            Transform operator- (const Transform& rhs) const;
            /// @brief Create a Transform with the sum of this and another
            /// @param rhs The Transform on the right hand side of the sign.
            /// @return A Transform with
            Transform operator+ (const Transform& rhs) const;
            /// @brief Multiply all the values of this by a single scalar.
            /// @param rhs The Transform on the right hand side of the sign.
            /// @return A Transform with
            Transform operator* (Real rhs) const;
            /// @brief Divide all the values of this by a single scalar.
            /// @param rhs The Transform on the right hand side of the sign.
            /// @return A Transform with
            Transform operator/ (Real rhs) const;

            /// @brief Is every value in this Transform less than or equal to its corresponding value in another.
            /// @param rhs The Transform on the right hand side of the sign.
            /// @note Used primarily for testing. This is not implemented for use with other kinds of Transform implementations as it is widely considered useless.
            bool operator<= (const Transform& rhs) const;
            /// @brief Is every value in this Transform greater than or equal to its corresponding value in another.
            /// @param rhs The Transform on the right hand side of the sign.
            /// @note Used primarily for testing. This is not implemented for use with other kinds of Transform implementations as it is widely considered useless.
            bool operator>= (const Transform& rhs) const;


    };
}

#ifndef SWIG
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Transform& x);

/// @brief Overwrite the data in a btTransform with the data in a Mezzanine::Transform using an intelligent assignment operator (in this case we really couldn't code the real assignment operator).
/// @param lhs The item on the Left Hand of the Sign, the item to be assigned to.
/// @param rhs The item on the Right Hand of the Sign, the item that has the values to be copied.
/// @return This returns the original btTransform reference(with the new values) so furhter work can continue to be performed if required
btTransform& MEZZ_LIB operator<< (btTransform& lhs, const Mezzanine::Transform& rhs);

/// @brief Overwrite the data in a Mezzanine::Transform with the data in a btTransform using an intelligent assignment operator (in this case we really couldn't code the real assignment operator).
/// @param lhs The item on the Left Hand of the Sign, the item to be assigned to.
/// @param rhs The item on the Right Hand of the Sign, the item that has the values to be copied.
/// @return This returns the original Mezzanine::Transform reference(with the new values) so furhter work can continue to be performed if required.
Mezzanine::Transform& MEZZ_LIB operator<< (Mezzanine::Transform& lhs, const btTransform& rhs);
#endif

namespace std
{
    /// @brief Get Numeric details on Transform
    template<>
    class numeric_limits<Mezzanine::Transform>
    {
        public:
            /// @brief Does this class (numeric_limits<Mezzanine::Transform>) exist
            static const bool is_specialized = true;
            /// @brief Does this support negative values?
            static const bool is_signed = true;
            /// @brief Can this only store integer types.
            static const bool is_integer = false;
            /// @brief The Transform uses Real, which is typically a machine dependedant which can be inexact
            static const bool is_exact = std::numeric_limits<Mezzanine::Real>::is_exact;
            /// @brief Can This represent an infinitely large value in subvalues?
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
            /// @brief Do subvalues adhere to iec 559?
            static const bool is_iec559 = std::numeric_limits<Mezzanine::Real>::is_iec559;
            /// @brief Is overflow of this type handle by modulo overflow?
            static const bool is_modulo = std::numeric_limits<Mezzanine::Real>::is_modulo;
            /// @brief How many integer digits(in machine base) of precision can this handle in each subvalue without floating point component or error?
            static const int digits = std::numeric_limits<Mezzanine::Real>::digits;
            /// @brief How many integer digits in base 10 of precision can this handle in each subvalue without floating point component or error?
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
            /// @return A Transform with 3 very small numbers in the location and 4 very small values in the rotation
            static Mezzanine::Transform min()
            {
                return Mezzanine::Transform(std::numeric_limits<Mezzanine::Vector3>::min(),
                                            std::numeric_limits<Mezzanine::Quaternion>::min()
                                         );
            }

            /// @brief Get the highest positive finite value this can represent
            /// @return A Transform with 3 very large numbers in the location and 4 very large values in the rotation
            static Mezzanine::Transform max()
            {
                return Mezzanine::Transform(std::numeric_limits<Mezzanine::Vector3>::max(),
                                            std::numeric_limits<Mezzanine::Quaternion>::max()
                                         );
            }

            /// @brief The smallest value representable from 1,1,1/1,1,1,1 to the next value
            /// @return A Transform with values larger than 1, but only just so.
            static Mezzanine::Transform epsilon()
            {
                return Mezzanine::Transform(std::numeric_limits<Mezzanine::Vector3>::epsilon(),
                                            std::numeric_limits<Mezzanine::Quaternion>::epsilon()
                                         );
            }

            /// @brief Get the largest possible rounding error
            /// @return A Transform containing with each value indicating how much they could be rounded.
            static Mezzanine::Transform round_error()
            {
                return Mezzanine::Transform(std::numeric_limits<Mezzanine::Vector3>::round_error(),
                                            std::numeric_limits<Mezzanine::Quaternion>::round_error()
                                         );
            }

            /// @brief Get the special value "Positive infinity"
            /// @return A transform containing 7 infinities.
            static Mezzanine::Transform infinity()
            {
                return Mezzanine::Transform(std::numeric_limits<Mezzanine::Vector3>::infinity(),
                                            std::numeric_limits<Mezzanine::Quaternion>::infinity()
                                         );
            }

            /// @brief Get the special value "Quiet Not actual Number"
            /// @return A Tranform containing 7 values
            static Mezzanine::Transform quiet_NaN()
            {
                return Mezzanine::Transform(std::numeric_limits<Mezzanine::Vector3>::quiet_NaN(),
                                            std::numeric_limits<Mezzanine::Quaternion>::quiet_NaN()
                                         );
            }

            /// @brief Get the special value "Signaling Not actual Number"
            /// @return A Tranform containing 7 values
            static Mezzanine::Transform signaling_NaN()
            {
                return Mezzanine::Transform(std::numeric_limits<Mezzanine::Vector3>::signaling_NaN(),
                                            std::numeric_limits<Mezzanine::Quaternion>::signaling_NaN()
                                         );
            }

            /// @brief Get the closest value to 0 that is not 0 this can represent, including extra precision for being close to 0 if supported.
            /// @return A Tranform containing 7 small values
            static Mezzanine::Transform denorm_min()
            {
                return Mezzanine::Transform(std::numeric_limits<Mezzanine::Vector3>::denorm_min(),
                                            std::numeric_limits<Mezzanine::Quaternion>::denorm_min()
                                         );
            }

    }; //Numeric Limits

} // std

#endif
