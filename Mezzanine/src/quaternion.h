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
#ifndef _quaternion_h
#define _quaternion_h

#include "crossplatformexport.h"
#include "datatypes.h"

#ifndef SWIG
    #include "XML/xml.h"
    #include <limits>
#endif

class btQuaternion;
namespace Ogre
{
    class Quaternion;
}

namespace Mezzanine
{
    class Vector3;
    class Matrix3x3;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class Quaternion
    /// @headerfile quaternion.h
    /// @brief This is used to store information about rotation in 3d space
    /// @details This is used to store information about rotation in 3d space. The
    /// X, Y and Z are used to identify a ray from the origin (0,0,0), about which
    /// W represents an amount of rotation.
    class MEZZ_LIB Quaternion
    {
    public:
        ///////////////////////////////////////////////////////////////////////////////
        //  Data Members

        /// @brief The X component of the Axis.
        Real X;
        /// @brief The Y component of the Axis.
        Real Y;
        /// @brief The Z component of the Axis.
        Real Z;
        /// @brief Rotation on the Axis X, Y and Z defined.
        Real W;

        ///////////////////////////////////////////////////////////////////////////////
        // Constructors

        /// @brief Blank Constructor.
        /// @details Basic no-initialization constructor.
        Quaternion();
        /// @brief Copy Constructor.
        /// @param Other The other Quaternion to be copied.
        Quaternion(const Mezzanine::Quaternion& Other) = default;
        /// @brief Move Constructor.
        /// @param Other The other Quaternion to be moved.
        Quaternion(Mezzanine::Quaternion&& Other) = default;
        /// @brief Constructor.
        /// @param X The X component of the Axis.
        /// @param Y The Y component of the Axis.
        /// @param Z The Z component of the Axis.
        /// @param W Rotation on the Axis X, Y and Z defined.
        Quaternion(const Real& X, const Real& Y, const Real& Z, const Real& W);
        /// @brief Axis and Rotation Constructor.
        /// @details This assembles a quaternion based on an axis and a rotation in radians.
        /// @param Angle Real representing the angle to be applied along the axis in radians.
        /// @param Axis Vector3 representing the axis to apply the rotation.
        Quaternion(const Real& Angle, const Vector3& Axis);
        /// @brief Rotation Matrix Constructor.
        /// @param Mat The matrix to set this quaternion from.
        Quaternion(const Matrix3x3& Mat);
        /// @brief Axes Constructor.
        /// @param AxisX The vector expressing the X axis.
        /// @param AxisY The vector expressing the Y axis.
        /// @param AxisZ The vector expressing the Z axis.
        Quaternion(const Vector3& AxisX, const Vector3& AxisY, const Vector3& AxisZ);
        /// @brief Rotation To Z constructor.
        /// @param DirectionAxis The axis to get the rotation to Z for.
        /// @param UpAxis The up axis in local space.
        Quaternion(const Vector3& DirectionAxis, const Vector3& UpAxis);
        /// @brief Bullet Quaternion constructor.
        /// @details Constructor that sets all values to match the Bullet quaternion.
        /// @param Theirs The quaternion to be copied to make this quaternion.
        explicit Quaternion(const btQuaternion& Theirs);
        /// @brief Ogre Quaternion constructor.
        /// @details Constructor that sets all values to match the Ogre quaternion.
        /// @param Theirs The quaternion to be copied to make this quaternion.
        explicit Quaternion(const Ogre::Quaternion& Theirs);
        /// @brief XML Constructor.
        /// @param OneNode The XML node to deserialize from.
        explicit Quaternion(const XML::Node& OneNode);

        ///////////////////////////////////////////////////////////////////////////////
        // Fancy Math and Utilities

        /// @brief Sets default/identity values to the members of this quaternion.
        void SetIdentity();
        /// @brief Sets the individual values of this quaterion directly.
        /// @param X The X component of the Axis.
        /// @param Y The Y component of the Axis.
        /// @param Z The Z component of the Axis.
        /// @param W Rotation on the Axis X, Y and Z defined.
        void SetValues(const Real& X, const Real& Y, const Real& Z, const Real& W);
        /// @brief Generates and sets the values of this quaternion to describe a rotation from an axis and angle on that axis.
        /// @param Angle Real representing the angle to be applied along the axis in radians.
        /// @param Axis Vector3 representing the axis to apply the rotation.
        void SetFromAxisAngle(const Real& Angle, const Vector3& Axis);
        /// @brief Sets this quaternions values to express the same rotation as a Matrix3x3.
        /// @param Mat The matrix to set this quaternion from.
        void SetFromMatrix3x3(const Matrix3x3& Mat);
        /// @brief Generates and sets the values of this quaternion from 3 Axis vectors.
        /// @param AxisX The vector expressing the X axis.
        /// @param AxisY The vector expressing the Y axis.
        /// @param AxisZ The vector expressing the Z axis.
        void SetFromAxes(const Vector3& AxisX, const Vector3& AxisY, const Vector3& AxisZ);
        /// @brief Generates and sets the values of this quaternion to describe a rotation from the direction axis to the Z axis.
        /// @param DirectionAxis The axis to get the rotation to Z for.
        /// @param UpAxis The up axis in local space.
        void SetFromAxisToZ(const Vector3& DirectionAxis, const Vector3& UpAxis);

        /// @brief Converts the rotation expressed by this Quaternion into it's individual rotation and axis components.
        /// @param Angle The amount of rotation to apply to the axis.
        /// @param Axis The Axis this quaternions rotation is on.
        void ConvertToAngleAxis(Real& Angle, Vector3& Axis) const;

        /// @brief Gets the Dot Product of this quaternion and another quaternion.
        /// @param Other The other quaternion to calculate the dot product from.
        /// @return Returns a Real that is the Dot Product of the two quaternions.
        Real DotProduct(const Quaternion& Other) const;
        /// @brief Gets the length of the quaternion.
        /// @return Returns a Real representing the length of the quaternion.
        Real Length() const;
        /// @brief Gets the squared length(len^2) of the quaternion.
        /// @return Returns a Real representing the squared length(len^2) of the quaternion.
        Real LengthSqrd() const;
        /// @brief Normalizes this Quaternion.
        /// @return Returns a normalized reference of this quaternion.
        Quaternion& Normalize();
        /// @brief Get a normalized copy of this Quaternion without changing this one.
        /// @return A Copy of this Quaternion after the copy has been normalized.
        Quaternion GetNormalizedCopy() const;
        /// @brief Inverses this Quaternion.
        /// @return Returns a quaternion that is a copy of this one after it has been inversed.
        Quaternion GetInverse() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Explicit Conversion

        /// @brief Gets a Bullet quaternion.
        /// @details Creates a Bullet quaternion with values equal to this class and returns it.
        /// @param normalize Whether or not you want this function to normalize the quaternion for you.
        /// @return A btQuaternion that has the same contents as this Mezzanine::Quaternion.
        btQuaternion GetBulletQuaternion(Boole normalize = false) const;
        /// @brief Copies an existing Bullet quaternion.
        /// @details This function will copy the values stored in an existing Bullet quaternion
        /// and set the values of this class to be the same.
        /// @param Ours The quaternion to be extracted.
        void ExtractBulletQuaternion(const btQuaternion &Ours);
        /// @brief Gets a Ogre quaternion.
        /// @details Creates a Ogre quaternion with values equal to this class and returns it.
        /// @param normalize Whether or not you want this function to normalize the quaternion for you.
        Ogre::Quaternion GetOgreQuaternion(Boole normalize = false) const;
        /// @brief Copies an existing Ogre quaternion.
        /// @details This function will copy the values stored in an existing Ogre quaternion
        /// and set the values of this class to be the same.
        /// @param Ours The quaternion to be extracted.
        void ExtractOgreQuaternion(const Ogre::Quaternion &Ours);

        ///////////////////////////////////////////////////////////////////////////////
        // Access Operators

        /// @brief Allows Array style access to the members of this class.
        /// @param Index The number corresponding to the Index you want.  0 = X, 1 = Y, 2 = Z, 3 = W.
        /// @return Returns a copy of the number at the index requested.
        Real operator[](const Whole& Index) const;
        /// @brief Allows Array style access to the members of this class.
        /// @param Index The number corresponding to the Index you want.  0 = X, 1 = Y, 2 = Z, 3 = W.
        /// @return Returns a writable reference to the number at the index requested.
        Real& operator[](const Whole& Index);

        ///////////////////////////////////////////////////////////////////////////////
        // Arithmetic By Real Operators

        /// @brief Scaling by multiplication.
        /// @param Scalar This is the amount to scale the quaternion by.
        /// @return Returns a scaled quaternion.
        Quaternion operator* (const Real& Scalar) const;
        /// @brief Scaling by division.
        /// @param Scalar This is the amount to scale the quaternion by.
        /// @return Returns a scaled quaternion.
        Quaternion operator/ (const Real& Scalar) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Left Hand Basic Arithmetic Operators

        /// @brief Addition operator with Mezzanine::Quaternion and Mezzanine::Quaternion.
        /// @param Other The other Quaternion to add to this one.
        /// @return A Mezzanine::Quaternion with the sum.
        Quaternion operator+ (const Mezzanine::Quaternion& Other) const;
        /// @brief Addition operator with Mezzanine::Quaternion and Ogre::Quaternion.
        /// @param Other The other Quaternion to add to this one.
        /// @return A Mezzanine::Quaternion with the sum.
        Quaternion operator+ (const Ogre::Quaternion& Other) const;
        /// @brief Addition operator with Mezzanine::Quaternion and btQuaternion.
        /// @param Other The other Quaternion to add to this one.
        /// @return A Mezzanine::Quaternion with the sum.
        Quaternion operator+ (const btQuaternion& Other) const;
        /// @brief Subtraction operator with Mezzanine::Quaternion and Mezzanine::Quaternion.
        /// @param Other The other Quaternion to subtract from this one.
        /// @return A Mezzanine::Quaternion with the difference.
        Quaternion operator- (const Mezzanine::Quaternion& Other) const;
        /// @brief Subtraction operator with Mezzanine::Quaternion and Ogre::Quaternion.
        /// @param Other The other Quaternion to subtract from this one.
        /// @return A Mezzanine::Quaternion with the difference.
        Quaternion operator- (const Ogre::Quaternion& Other) const;
        /// @brief Subtraction operator with Mezzanine::Quaternion and btQuaternion.
        /// @param Other The other Quaternion to subtract from this one.
        /// @return A Mezzanine::Quaternion with the difference.
        Quaternion operator- (const btQuaternion& Other) const;
        /// @brief Multiplication operator with Mezzanine::Quaternion and Mezzanine::Quaternion.
        /// @param Other The other Quaternion to multiply from this one.
        /// @return A Mezzanine::Quaternion with the result.
        Quaternion operator* (const Mezzanine::Quaternion& Other) const;
        /// @brief Multiplication operator with Mezzanine::Quaternion and Ogre::Quaternion.
        /// @param Other The other Quaternion to multiply from this one.
        /// @return A Mezzanine::Quaternion with the result.
        Quaternion operator* (const Ogre::Quaternion& Other) const;
        /// @brief Multiplication operator with Mezzanine::Quaternion and btQuaternion.
        /// @param Other The other Quaternion to multiply from this one.
        /// @return A Mezzanine::Quaternion with the result.
        Quaternion operator* (const btQuaternion& Other) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Vector Rotation Operators

        /// @brief Rotates a vector by the provided quaternion.
        /// @param Other The vector to rotate.
        /// @return Returns a rotated version of the provided vector.
        Vector3 operator* (const Vector3& Other) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Increment and Decrement Operators

        /// @brief Incrementing operator with Mezzanine::Quaternion and Mezzanine::Quaternion.
        /// @param Other The other Quaternion to add to this one.
        /// @return This Mezzanine::Quaternion with the sum.
        Quaternion& operator+= (const Mezzanine::Quaternion& Other);
        /// @brief Incrementing operator with Mezzanine::Quaternion and Ogre::Quaternion.
        /// @param Other The other Quaternion to add to this one.
        /// @return This Mezzanine::Quaternion with the sum.
        Quaternion& operator+= (const Ogre::Quaternion& Other);
        /// @brief Incrementing operator with Mezzanine::Quaternion and btQuaternion.
        /// @param Other The other Quaternion to add to this one.
        /// @return This Mezzanine::Quaternion with the sum.
        Quaternion& operator+= (const btQuaternion& Other);
        /// @brief Decrementing operator with Mezzanine::Quaternion and Mezzanine::Quaternion.
        /// @param Other The other Quaternion to subtract from this one.
        /// @return This Mezzanine::Quaternion with the difference.
        Quaternion& operator-= (const Mezzanine::Quaternion& Other);
        /// @brief Decrementing operator with Mezzanine::Quaternion and Ogre::Quaternion.
        /// @param Other The other Quaternion to subtract from this one.
        /// @return This Mezzanine::Quaternion with the difference.
        Quaternion& operator-= (const Ogre::Quaternion& Other);
        /// @brief Decrementing operator with Mezzanine::Quaternion and btQuaternion.
        /// @param Other The other Quaternion to subtract from this one.
        /// @return This Mezzanine::Quaternion with the difference.
        Quaternion& operator-= (const btQuaternion& Other);

        ///////////////////////////////////////////////////////////////////////////////
        // Assignment Operators

        /// @brief Assignment Operator.
        /// @param Other The other Quaternion to be copied.
        /// @return Returns a reference to this.
        Quaternion& operator=(const Mezzanine::Quaternion& Other) = default;
        /// @brief Move assignment Operator.
        /// @param Other The other Quaternion to be moved.
        /// @return Returns a reference to this.
        Quaternion& operator=(Mezzanine::Quaternion&& Other) = default;
        /// @brief Assignment Operator from Ogre::Quaternion.
        /// @param Other The other Quaternion to be copied.
        /// @return Returns a reference to this.
        Quaternion& operator=(const Ogre::Quaternion& Other);
        /// @brief Assignment Operator from btQuaternion.
        /// @param Other The other Quaternion to be copied.
        /// @return Returns a reference to this.
        Quaternion& operator=(const btQuaternion& Other);

        ///////////////////////////////////////////////////////////////////////////////
        // Equality Comparison Operators

        /// @brief Equality Comparison Operator from Mezzanine::Quaternion.
        /// @param Other The other quaternion to compare with.
        /// @return True if the Quaternions are semantically equal, false otherwise.
        Boole operator==(const Mezzanine::Quaternion& Other) const;
        /// @brief Equality Comparison Operator from Ogre::Quaternion.
        /// @param Other The other quaternion to compare with.
        /// @return True if the Quaternions are semantically equal, false otherwise.
        Boole operator==(const Ogre::Quaternion& Other) const;
        /// @brief Equality Comparison Operator from btQuaternion.
        /// @param Other The other quaternion to compare with.
        /// @return True if the Quaternions are semantically equal, false otherwise.
        Boole operator==(const btQuaternion& Other) const;

        /// @brief Inequality Comparison Operator from Mezzanine::Quaternion.
        /// @param Other The other quaternion to compare with.
        /// @return True if the Quaternions are not semantically equal, false otherwise.
        Boole operator!=(const Mezzanine::Quaternion& Other) const;
        /// @brief Inequality Comparison Operator from Ogre::Quaternion.
        /// @param Other The other quaternion to compare with.
        /// @return True if the Quaternions are not semantically equal, false otherwise.
        Boole operator!=(const Ogre::Quaternion& Other) const;
        /// @brief Inequality Comparison Operator from btQuaternion.
        /// @param Other The other quaternion to compare with.
        /// @return True if the Quaternions are not semantically equal, false otherwise.
        Boole operator!=(const btQuaternion& Other) const;

        /// @brief Is every value in this Quaternion less than or equal to its corresponding value in another.
        /// @param Other The Quaternion on the right hand side of the sign.
        /// @note Used primarily for testing. This is not implemented for use with other kinds of Quaternion implementations as it is widely considered useless.
        Boole operator<= (const Mezzanine::Quaternion& Other) const;
        /// @brief Is every value in this Quaternion greater than or equal to its corresponding value in another.
        /// @param Other The Quaternion on the right hand side of the sign.
        /// @note Used primarily for testing. This is not implemented for use with other kinds of Quaternion implementations as it is widely considered useless.
        Boole operator>= (const Mezzanine::Quaternion& Other) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        /// @brief Convert this class to an XML::Node ready for serialization
        /// @param CurrentRoot The point in the XML hierarchy that all this quaternion should be appended to.
        void ProtoSerialize(XML::Node& CurrentRoot) const;
        /// @brief Take the data stored in an XML and overwrite this instance of this object with it
        /// @param OneNode and XML::Node containing the data.
        void ProtoDeSerialize(const XML::Node& OneNode);
        /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
        /// @return A string containing "Quaternion"
        static String GetSerializableName();
    };//Quaternion
}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Right Hand Arithmetic Operators

/// @brief Addition operator with Mezzanine::Quaternion and Ogre::Quaternion.
/// @param Other The first Quaternion to add.
/// @param Other2 The Mezzanine::Quaternion to add.
/// @return A Mezzanine::Quaternion with the sum.
Mezzanine::Quaternion MEZZ_LIB operator+ (const Ogre::Quaternion& Other, const Mezzanine::Quaternion& Other2);

/// @brief Addition operator with Mezzanine::Quaternion and btQuaternion.
/// @param Other The first Quaternion to add.
/// @param Other2 The Mezzanine::Quaternion to add.
/// @return A Mezzanine::Quaternion with the sum.
Mezzanine::Quaternion MEZZ_LIB operator+ (const btQuaternion& Other, const Mezzanine::Quaternion& Other2);

/// @brief Subtraction operator with Mezzanine::Quaternion and Ogre::Quaternion.
/// @param Other The first Quaternion to subtract from.
/// @param Other2 The Mezzanine::Quaternion to subtract.
/// @return A Mezzanine::Quaternion with the sum.
Mezzanine::Quaternion MEZZ_LIB operator- (const Ogre::Quaternion& Other, const Mezzanine::Quaternion& Other2);

/// @brief Subtraction operator with Mezzanine::Quaternion and btQuaternion.
/// @param Other The first Quaternion to subtract from.
/// @param Other2 The Mezzanine::Quaternion to subtract.
/// @return A Mezzanine::Quaternion with the sum.
Mezzanine::Quaternion MEZZ_LIB operator- (const btQuaternion& Other, const Mezzanine::Quaternion& Other2);


///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment

#ifndef SWIG
/// @brief Conversion operator to btQuaternion.
/// @param Other The Quaternion to store the fresh contents.
/// @param Other2 The Quaternion to be converted.
/// @return A btQuaternion containing the contents of the converted Quaternion.
btQuaternion& MEZZ_LIB operator<< ( btQuaternion& Other, const Mezzanine::Quaternion& Other2);

/// @brief Conversion operator to btQuaternion.
/// @param Other The Quaternion to store the fresh contents.
/// @param Other2 The Quaternion to be converted.
/// @return A btQuaternion containing the contents of the converted Quaternion.
btQuaternion& MEZZ_LIB operator<< ( btQuaternion& Other, const Ogre::Quaternion& Other2);

/// @brief Conversion operator to Mezzanine::Quaternion.
/// @param Other The Quaternion to store the fresh contents.
/// @param Other2 The Quaternion to be converted.
/// @return A Mezzanine::Quaternion containing the contents of the converted Quaternion.
Mezzanine::Quaternion& MEZZ_LIB operator<< ( Mezzanine::Quaternion& Other, const Ogre::Quaternion& Other2);

/// @brief Conversion operator to btQuaternion.
/// @param Other The Quaternion to store the fresh contents.
/// @param Other2 The Quaternion to be converted.
/// @return A btQuaternion containing the contents of the converted Quaternion.
Mezzanine::Quaternion& MEZZ_LIB operator<< ( Mezzanine::Quaternion& Other, const btQuaternion& Other2);

/// @brief Conversion operator to Ogre::Quaternion.
/// @param Other The Quaternion to store the fresh contents.
/// @param Other2 The Quaternion to be converted.
/// @return A Ogre::Quaternion containing the contents of the converted Quaternion.
Ogre::Quaternion& MEZZ_LIB operator<< ( Ogre::Quaternion& Other, const Mezzanine::Quaternion& Other2);

/// @brief Conversion operator to Ogre::Quaternion.
/// @param Other The Quaternion to store the fresh contents.
/// @param Other2 The Quaternion to be converted.
/// @return A Ogre::Quaternion containing the contents of the converted Quaternion.
Ogre::Quaternion& MEZZ_LIB operator<< ( Ogre::Quaternion& Other, const btQuaternion& Other2);

/// @brief Used to Serialize an Mezzanine::Quaternion to a human readable stream
/// @details This puts proper XML output to the output stream,
/// including versioning information which will be used to maintain backwards compatibility. The current XML format
/// will create one node with no child nodes. The name of the xml node will be "Quaternion". It will have 5 attributes.
/// "Version", will be set to a value of 1, indicating if came from version 1 compatible Quaternion. It will also have an "X", "Y",
/// "Z" and "W" attributes will values set appropriately. For example '<Quaternion Version="1" X="1" Y="2" Z="3" W="0" />'.
/// @param x The Mezzanine::Quaternion to be converted to characters.
/// @param stream The place to send the characters, that define the Mezzanine::Quaternion.
/// @return Get an std::ostream that was written to, this allow chaining of the << operators.
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Quaternion& x);

/// @brief Used to de-serialize an Mezzanine::Quaternion from a stream
/// @details This reads in the xml and sets the target vector according to values
/// from the stream.
/// @param Vec The Mezzanine::Quaternion that will accept the values from the xml
/// @param stream The place to get the characters from, that define the Mezzanine::Quaternion.
/// @return Get an std::ostream that was read from, this allow chaining of the >> operators.
/// @throw Can throw any exception that any function in the Mezzanine::xml namespace could throw in addition to a Mezzanine::Exception if the serialization version doesn't match.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Quaternion& Vec);

/// @brief Converts an XML node into a Mezzanine::Quaternion
/// @details This will convert an XML::Node will a valid serialized Mezzanine::Quaternion into a Mezzanine::Quaternion
/// @param OneNode An XML Node containing the the text of a Quaternion
/// @param Vec the Mezzanine::Quaternion to store the deserialized Quaternion
/// @return This returns a reference to the XML::Node for operator chaining or whatever.
/// @throw Can throw any exception that any function in the Mezzanine::xml namespace could throw in addition to a Mezzanine::Exception if the serialization version doesn't match.
void MEZZ_LIB  operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Quaternion& Vec);
#endif // \ SWIG

namespace std
{
    #ifndef SWIG
    /// @brief Get Numeric details on Quaternion
    template<>
    class numeric_limits<Mezzanine::Quaternion>
    {
        public:
            /// @brief Does this class (numeric_limits<Mezzanine::Quaternion>) exist
            static const bool is_specialized = true;
            /// @brief Does this support negative values?
            static const bool is_signed = true;
            /// @brief Can this only store integer types.
            static const bool is_integer = false;
            /// @brief The Quaternion uses Real, which is typically a machine dependedant which can be inexact
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
            /// @return A Quaternion with 4 very small numbers
            static Mezzanine::Quaternion min()
            {
                return Mezzanine::Quaternion(std::numeric_limits<Mezzanine::Real>::min(),
                                             std::numeric_limits<Mezzanine::Real>::min(),
                                             std::numeric_limits<Mezzanine::Real>::min(),
                                             std::numeric_limits<Mezzanine::Real>::min()
                                            );
            }

            /// @brief Get the highest positive finite value this can represent
            /// @return A Quaternion with 4 very large numbers
            static Mezzanine::Quaternion max()
            {
                return Mezzanine::Quaternion(std::numeric_limits<Mezzanine::Real>::max(),
                                             std::numeric_limits<Mezzanine::Real>::max(),
                                             std::numeric_limits<Mezzanine::Real>::max(),
                                             std::numeric_limits<Mezzanine::Real>::max()
                                            );
            }

            /// @brief The smallest value representable from 1.0,1.0,1.0 to the next value
            /// @return A Quaternion with very small numbers
            static Mezzanine::Quaternion epsilon()
            {
                return Mezzanine::Quaternion(std::numeric_limits<Mezzanine::Real>::epsilon(),
                                             std::numeric_limits<Mezzanine::Real>::epsilon(),
                                             std::numeric_limits<Mezzanine::Real>::epsilon(),
                                             std::numeric_limits<Mezzanine::Real>::epsilon()
                                            );
            }

            /// @brief Get the largest possible rounding error
            /// @return A Quaternion containing 4 values indicating how much they could be rounded.
            static Mezzanine::Quaternion round_error()
            {
                return Mezzanine::Quaternion(std::numeric_limits<Mezzanine::Real>::round_error(),
                                             std::numeric_limits<Mezzanine::Real>::round_error(),
                                             std::numeric_limits<Mezzanine::Real>::round_error(),
                                             std::numeric_limits<Mezzanine::Real>::round_error()
                                            );
            }

            /// @brief Get the special value "Positive infinity"
            /// @return A Quaternion containing 4 values.
            static Mezzanine::Quaternion infinity()
            {
                return Mezzanine::Quaternion(std::numeric_limits<Mezzanine::Real>::infinity(),
                                             std::numeric_limits<Mezzanine::Real>::infinity(),
                                             std::numeric_limits<Mezzanine::Real>::infinity(),
                                             std::numeric_limits<Mezzanine::Real>::infinity()
                                            );
            }

            /// @brief Get the special value "Quiet Not actual Number"
            /// @return A Quaternion containing 4 values.
            static Mezzanine::Quaternion quiet_NaN()
            {
                return Mezzanine::Quaternion(std::numeric_limits<Mezzanine::Real>::quiet_NaN(),
                                             std::numeric_limits<Mezzanine::Real>::quiet_NaN(),
                                             std::numeric_limits<Mezzanine::Real>::quiet_NaN(),
                                             std::numeric_limits<Mezzanine::Real>::quiet_NaN()
                                            );
            }

            /// @brief Get the special value "Signaling Not actual Number"
            /// @return A Quaternion containing 4 special values.
            static Mezzanine::Quaternion signaling_NaN()
            {
                return Mezzanine::Quaternion(std::numeric_limits<Mezzanine::Real>::signaling_NaN(),
                                             std::numeric_limits<Mezzanine::Real>::signaling_NaN(),
                                             std::numeric_limits<Mezzanine::Real>::signaling_NaN(),
                                             std::numeric_limits<Mezzanine::Real>::signaling_NaN()
                                            );
            }

            /// @brief Get the closest value to 0 that is not 0 this can represent, including extra precision for being close to 0 if supported.
            /// @return A vector containing 3 very small values.
            static Mezzanine::Quaternion denorm_min()
            {
                return Mezzanine::Quaternion(std::numeric_limits<Mezzanine::Real>::denorm_min(),
                                             std::numeric_limits<Mezzanine::Real>::denorm_min(),
                                             std::numeric_limits<Mezzanine::Real>::denorm_min(),
                                             std::numeric_limits<Mezzanine::Real>::denorm_min()
                                            );
            }
    }; //Numeric Limits
    #endif // \SWIG

} // std


#endif
