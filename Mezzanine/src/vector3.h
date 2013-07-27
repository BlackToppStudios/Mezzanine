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
#ifndef _vector3_h
#define _vector3_h

#include "crossplatformexport.h"
#include "datatypes.h"
#include "enumerations.h"
#include "swig.h"
#include "XML/xml.h"

//Forward Declarations for wierd compatibility functions
class btVector3;
namespace Ogre
{
    class Vector3;
}

SWIG_INFO_BEGINCLASS

namespace Mezzanine
{

    //remove this after testing
    void PrintHello(); // Function to call from Lua

    class Quaternion;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class Vector3
    /// @headerfile vector3.h
    /// @brief This is used to represent a point in space, or a vector through space
    /// @details This contains an X, Y and a Z value used to represent coordinates.
    /// This also has a number of facilities to make converting from Physics subsystem
    /// vectors or graphics subsystems as easy as possible
    /// @note No operator<< existing in any scripting interface for this class
    class MEZZ_LIB Vector3
    {
    public:

        ///////////////////////////////////////////////////////////////////////////////
        // The Essentials

        /// @brief Coordinate on the X vector.
        Real X;
        /// @brief Coordinate on the Y vector.
        Real Y;
        /// @brief Coordinate on the Z vector.
        Real Z;

        #ifndef SWIG
        /// @brief Get The value associate with a certain Axis.
        /// @param Axis Which axis to retrieve.
        /// @note Despite the multiple scripting overloads of this Method, only Real GetAxisValue(const Whole& Axis) const; exists in the scripting interface
        /// @return Either X, Y or Z as indicated by the value passed in.
        Real GetAxisValue(const StandardAxis& Axis) const;
        #endif // \SWIG

        /// @copydoc GetAxisValue(StandardAxis Axis) const
        Real GetAxisValue(const Whole& Axis) const;

        #ifndef SWIG
        /// @brief Get The value associate with a certain Axis in such a way that it can readily be assigned in this Vector3.
        /// @param Axis Which axis to retrieve.
        /// @return Either X, Y or Z as indicated by the value passed in.
        Real& GetAxisValue(const StandardAxis& Axis);
        /// @copydoc GetAxisValue(StandardAxis Axis)
        Real& GetAxisValue(const Whole& Axis);
        #endif // \SWIG

        /// @copydoc GetAxisValue(StandardAxis Axis) const
        Real operator[] (const StandardAxis& Axis) const;
        /// @copydoc GetAxisValue(StandardAxis Axis) const
        Real operator[] (const Whole& Axis) const;
        /// @copydoc GetAxisValue(StandardAxis Axis)
        Real& operator[] (const StandardAxis& Axis);
        /// @copydoc GetAxisValue(StandardAxis Axis)
        Real& operator[] (const Whole& Axis);

        ///////////////////////////////////////////////////////////////////////////////
        // Constructors

        /// @brief Default Constructor.
        /// @details Basic all zero initialization constructor.
        Vector3();
        /// @brief Real value Constructor.
        /// @details Constructor that sets all three vectors.
        /// @param X Coordinate on the X vector.
        /// @param Y Coordinate on the Y vector.
        /// @param Z Coordinate on the Z vector.
        Vector3(const Real& X, const Real& Y, const Real& Z);
        /// @brief Ogre Value Constructor.
        /// @details Constructor that sets all values to match the Ogre vector.
        /// @param Vec The vector to be copied to make this vector.
        explicit Vector3(const Ogre::Vector3& Vec);
        /// @brief Bullet Value Constructor.
        /// @details Constructor that sets all values to match the Bullet vector.
        /// @param Vec The vector to be copied to make this vector.
        explicit Vector3(const btVector3& Vec);
        /// @brief Copy Constructor
        /// @param Vec The other Mezzanine::Vector3 to copy to make this one.
        Vector3(const Mezzanine::Vector3& Vec);
        /// @brief Deserializing constructor
        /// @param Vec The other Mezzanine::Vector3 to copy to make this one.
        explicit Vector3(XML::Node OneNode);

        ///////////////////////////////////////////////////////////////////////////////
        // Prebuilt Vectors

        /// @brief Gets a vector representing the X unit of a Vector3.
        /// @return A Vector3(1,0,0).
        static Vector3 Unit_X();
        /// @brief Gets a vector representing the Y unit of a vector.
        /// @return A Vector3(0,1,0).
        static Vector3 Unit_Y();
        /// @brief Gets a vector representing the Z unit of a vector.
        /// @return A Vector3(0,0,1).
        static Vector3 Unit_Z();
        /// @brief Gets a vector representing the negative X unit of a vector.
        /// @return A Vector3(-1,0,0).
        static Vector3 Neg_Unit_X();
        /// @brief Gets a vector representing the negative Y unit of a vector.
        /// @return A Vector3(0,-1,0).
        static Vector3 Neg_Unit_Y();
        /// @brief Gets a vector representing the negative Z unit of a vector.
        /// @return A Vector3(0,0,-1).
        static Vector3 Neg_Unit_Z();

        /// @brief Get a Unit Vector along the given Axis
        /// @param Axis The StandardAxis correlating to the Unit Vector you are retrieving
        /// @return A vector one unit in length along the Axis specified.
        static Vector3 UnitOnAxis(StandardAxis Axis);
        /// @brief Get a Unit Vector along the given Axis
        /// @return The Corresponding StandardAxis if a Vector equal to Unit_X, Unit_Y or Unit_Z is passed in.
        StandardAxis IsStandardUnitAxis() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Assignment Operators

        /// @brief Assignment operator to convert from Bullet Vectors
        /// @details This copies the x,y and z values from the bullet into this vector
        /// @param Vec This is a btVector3 that will be copied
        Vector3& operator= (const btVector3 &Vec);
        /// @brief Assignment operator to convert from Ogre Vectors
        /// @details This copies the x,y and z values from the bullet into this vector
        /// @param Vec This is a Ogre::Vector3 that will be copied.
        Vector3& operator= (const Ogre::Vector3 &Vec);

        ///////////////////////////////////////////////////////////////////////////////
        // Unary Operators

        /// @brief Additive Inverse Operator
        /// @details Returns the opposite Vector3 relative to 0,0,0
        Vector3 operator- ();

        ///////////////////////////////////////////////////////////////////////////////
        // Vector3 Arithmetic with Real

        /// @brief Scaling by multiplication
        /// @details This Multiplies X, Y and Z by scalar
        /// @return This returns a Vector3 that has been scaled
        /// @param scalar This is the amount to scale the Vector3 by
        Vector3 operator* (const Real &scalar) const;
        /// @brief Scaling by Division
        /// @details This Diisionn X, Y and Z by scalar
        /// @return This returns a Vector3 that has been scaled
        /// @param scalar This is the amount to scale the Vector3 by
        Vector3 operator/ (const Real &scalar) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Vector3 Arithmetic and assignment with Real

        /// @brief Scaling by multiplication.
        /// @details This Multiplies X, Y and Z by scalar and stores the changes in this Vector3.
        /// @param scalar This is the amount to scale the Vector3 by.
        Vector3& operator*= (const Real &scalar);
        /// @brief Scaling by Division
        /// @details This Division X, Y and Z by scalar and and stores the changes in this Vector3.
        /// @param scalar This is the amount to scale the Vector3 by
        Vector3& operator/= (const Real &scalar);

        ///////////////////////////////////////////////////////////////////////////////
        // Equality Comparison operators

        /// @brief Equality Comparison Operator.
        /// @details Returns true if X==X, Y==Y and Z==Z. If any of those do not match this returns false.
        /// @param Vec This is the other Mezzanine::Vector3.
        bool operator== (const Mezzanine::Vector3 &Vec) const;
        /// @brief Equality Comparison Operator.
        /// @details Returns true if X==getX(), Y==getY() and Z==getZ(). If any of those do not match this returns false.
        /// @param Vec This is an btVector3 that needs to be compared with this.
        bool operator== (const btVector3 &Vec) const;
        /// @brief Equality Comparison Operator.
        /// @details Returns true if X==x, Y==y and Z==z. If any of those do not match this returns false.
        /// @param Vec This is an Ogre::Vector3 that needs to be compared with this.
        bool operator== (const Ogre::Vector3 &Vec) const;
        /// @brief Inequality Comparison Operator.
        /// @details Returns true if X!=X, Y!=Y or Z!=Z. If all of those match this returns false.
        /// @param Vec This is the other Mezzanine::Vector3.
        bool operator!= (const Mezzanine::Vector3 &Vec) const;
        /// @brief Inequality Comparison Operator.
        /// @details Returns true if X!=getX(), Y!=getY() or Z!=getZ(). If all of those match this returns false.
        /// @param Vec This is an btVector3 that needs to be compared with this.
        bool operator!= (const btVector3 &Vec) const;
        /// @brief Inequality Comparison Operator.
        /// @details Returns true if X!=x, Y!=y or Z!=z. If all of those match this returns false.
        /// @param Vec This is an Ogre::Vector3 that needs to be compared with this.
        bool operator!= (const Ogre::Vector3 &Vec) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Arithmetic Operators

        /// @brief Addition Operator
        /// @details Allows for addition from a Mezzanine::Vector3
        /// @param Vec This is the other Mezzanine::Vector3
        Vector3 operator+ (const Vector3 &Vec) const;
        /// @brief Subraction Operator
        /// @details Allows for subtraction from a Mezzanine::Vector3
        /// @param Vec This is the other Mezzanine::Vector3
        Vector3 operator- (const Vector3 &Vec) const;
        /// @brief Multiplaction Operator
        /// @details Allows for multiplaction from a Mezzanine::Vector3
        /// @param Vec This is the other Mezzanine::Vector3
        Vector3 operator* (const Vector3 &Vec) const;
        /// @brief Division Operator
        /// @details Allows for division from a Mezzanine::Vector3
        /// @param Vec This is the other Mezzanine::Vector3
        Vector3 operator/ (const Vector3 &Vec) const;

        /////////////////////////////////////////////////////////////////////
        // Arithmetic Operators with btVector3

        /// @brief Bullet Addition Operator
        /// @details Allows for addition  between a Mezzanine::Vector3 and a btVector3
        /// @param Vec This is the btVector3 to be added
        Vector3 operator+ (const btVector3 &Vec) const;
        /// @brief Bullet Subtraction Operator
        /// @details Allows for subtraction  between a Mezzanine::Vector3 and a btVector3
        /// @param Vec This is the btVector3 to be subtracted
        Vector3 operator- (const btVector3 &Vec) const;
        /// @brief Bullet Multiplication Operator
        /// @details Allows for multiplication  between a Mezzanine::Vector3 and a btVector3
        /// @param Vec This is the btVector3 to be multiplied
        Vector3 operator* (const btVector3 &Vec) const;
        /// @brief Bullet Division Operator
        /// @details Allows for division  between a Mezzanine::Vector3 and a btVector3
        /// @param Vec This is the btVector3 to be divided
        Vector3 operator/ (const btVector3 &Vec) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Arithmetic Operators with Ogre::Vector3

        /// @brief Ogre Addition Operator
        /// @details Allows for addition  between a Mezzanine::Vector3 and a Ogre::Vector3
        /// @param Vec This is the Ogre::Vector3 to be added
        Vector3 operator+ (const Ogre::Vector3 &Vec) const;
        /// @brief Ogre Subtraction Operator
        /// @details Allows for subtraction  between a Mezzanine::Vector3 and a Ogre::Vector3
        /// @param Vec This is the Ogre::Vector3 to be subtracted
        Vector3 operator- (const Ogre::Vector3 &Vec) const;
        /// @brief Ogre Multiplication Operator
        /// @details Allows for multiplying  between a Mezzanine::Vector3 and a Ogre::Vector3
        /// @param Vec This is the Ogre::Vector3 to be multiplied
        Vector3 operator* (const Ogre::Vector3 &Vec) const;
        /// @brief Ogre Division Operator
        /// @details Allows for division  between a Mezzanine::Vector3 and a Ogre::Vector3
        /// @param Vec This is the Ogre::Vector3 to be divided
        Vector3 operator/ (const Ogre::Vector3 &Vec) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Fancy Math

        /// @brief This is used to calculate the crossproduct of this and another vector
        /// @details This creates a third vector, which should be on a line perpendicular
        /// to the line that contains the origin and the other vector \n\n
        /// Thanks to the guys at Ogre3d for the well written version of this function
        /// that we based this on.
        /// @param Vec the Vector to work with to create the cross product
        /// @return A Vector3 containing crossproduct of this vector and Vec
        Vector3 CrossProduct(const Vector3& Vec) const;
        /// @brief This is used to calculate the dotproduct of this and another vector
        /// @details This calculates the sum of the products of X, Y and Z. \n\n
        /// Thanks to the guys at Ogre3d for the well written version of this function
        /// that we based this on.
        /// @param Vec The vector to work with to create the cross product
        /// @return This is the dotproduct of this vector and vec
        Real DotProduct(const Vector3& Vec) const;
        /// @brief This will change this point into it's own normal relative to the origin
        /// @details This will change this vector into one that is the same direction from the origin, but only one unit a away.
        /// @return Returns a reference to the normalized vector.
        Vector3& Normalize();
        /// @brief This returns the normal for this relative to the origin
        /// @details This will return a vector that is 1 unit in away from the origin, if a line were starting and the origin it would pass through
        /// both the normal and the original point.
        /// @return At a vector3 that is the normal of this Vector3 or 0,0,0 if the current Vector is all 0s
        Vector3 GetNormal() const;
        /// @brief This will get the direction between two points.
        /// @details This returns the direction expressed as a vector between this vector and another provided vector.
        /// The vector the results vector is relative to this vector.
        /// @param Destination The point in space to determine the direction for.
        Vector3 GetDirection(const Vector3& Destination) const;
        /// @brief This will inverse the reals in the vector.
        /// @details This function will inverse all the reals in the vector.
        /// @return A copy of of the current Vector3
        Vector3 Inverse();
        /// @brief Gets a reflection vector to the plane with the given normal.
        /// @param Normal The normal of the plane being reflected off of.
        /// @return Returns a Vector3 containing the reflection vector.
        Vector3 Reflect(const Vector3& Normal);
        /// @brief Gets the distance between this and another vector.
        /// @details This uses a 3d extension of pythagoras thereom to calculate the distance between
        /// this Vector3 and another.
        /// @param OtherVec This is the other point to measure the distance to.
        /// @return Returns a Real representing the distance.
        Real Distance(const Vector3& OtherVec) const;
        /// @brief Gets the squared distance between this and another vector.
        /// @param OtherVec This is the other point to measure the distance to.
        /// @return Returns a Real representing the distance squared.
        Real SquaredDistance(const Vector3& OtherVec) const;
        /// @brief Gets the length of this vector.
        /// @return Returns a real representing the length of this vector.
        Real Length() const;
        /// @brief Gets the length of this vector squared.
        /// @return Returns a real representing the squared length of this vector.
        Real SquaredLength() const;
        /// @brief Checks to see if the length of this vector is zero.
        /// @return Returns true if this vector has zero length, false otherwise.
        bool IsZeroLength() const;
        /// @brief Gets the rotation needed to rotate this vector as an axis to another axis.
        /// @param Axis The target axis to rotate to.
        /// @return Returns a Quaternion representing the needed rotation to the specified axis.
        Quaternion GetRotationToAxis(const Vector3& Axis, const Vector3& FallBackAxis = Vector3()) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Functions

        /// @brief Sets all the members of this vector3 to zero.
        void Zero();
        /// @brief Manually sets all the members of this vector3.
        /// @param X Value to set for X.
        /// @param Y Value to set for Y.
        /// @param Z Value to set for Z.
        void SetValues(const Real& X, const Real& Y, const Real& Z);

        ///////////////////////////////////////////////////////////////////////////////
        // Manual Conversions

        /// @brief Gets a Bullet vector3.
        /// @details Creates a Bullet vector3 with values equal to this class and returns it.
        btVector3 GetBulletVector3() const;
        /// @brief Copies an existing Bullet vector3.
        /// @details This function will copy the values stored in an existing Bullet vector3
        /// and set the values of this class to be the same.
        /// @param temp The vector3 to be extracted.
        void ExtractBulletVector3(const btVector3& temp);
        /// @brief Gets a Ogre vector3.
        /// @details Creates a Ogre vector3 with values equal to this class and returns it.
        Ogre::Vector3 GetOgreVector3() const;
        /// @brief Copies an existing Ogre vector3.
        /// @details This function will copy the values stored in an existing Ogre vector3
        /// and set the values of this class to be the same.
        /// @param temp The vector3 to be extracted.
        void ExtractOgreVector3(const Ogre::Vector3& temp);

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        /// @brief Convert this class to an XML::Node ready for serialization
        /// @param CurrentRoot The point in the XML hierarchy that all this vector3 should be appended to.
        void ProtoSerialize(XML::Node& CurrentRoot) const;
        /// @brief Take the data stored in an XML and overwrite this instance of this object with it
        /// @param OneNode and XML::Node containing the data.
        void ProtoDeSerialize(const XML::Node& OneNode);
        /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
        /// @return A string containing "Vector3"
        static String SerializableName();
    };//Vector3
}//Mezzanine

///////////////////////////////////////////////////////////////////////////////
// Right Hand Arithmetic Operators
/// @brief Right Hand Addition Operator for Bullet Vectors with a Mezzanine::Vector3.
/// @param Vec The Bullet Vector to be added.
/// @param lhs The Mezzanine::Vector3 to be added.
/// @return A Mezzanine::Vector3 with the Sum.
Mezzanine::Vector3 MEZZ_LIB operator+ (const btVector3  &Vec, const Mezzanine::Vector3& lhs);

/// @brief Right Hand Subtraction Operator for Bullet Vectors with a Mezzanine::Vector3.
/// @param Vec The Bullet Vector to be subtracted from.
/// @param lhs The Mezzanine::Vector3 to be subtracted.
/// @return A Mezzanine::Vector3 with the difference.
Mezzanine::Vector3 MEZZ_LIB operator- (const btVector3  &Vec, const Mezzanine::Vector3& lhs);

/// @brief Right Hand Multiplication Operator for Bullet Vectors with a Mezzanine::Vector3.
/// @param Vec The Bullet Vector to be multiplied.
/// @param lhs The Mezzanine::Vector3 to be multiplied.
/// @return A Mezzanine::Vector3 with the product.
Mezzanine::Vector3 MEZZ_LIB operator* (const btVector3  &Vec, const Mezzanine::Vector3& lhs);

/// @brief Right Hand Division Operator for Bullet Vectors with a Mezzanine::Vector3.
/// @param Vec The Bullet Vector to be divided by.
/// @param lhs The Mezzanine::Vector3 to be divided.
/// @return A Mezzanine::Vector3 with the results
Mezzanine::Vector3 MEZZ_LIB operator/ (const btVector3  &Vec, const Mezzanine::Vector3& lhs);


/// @brief Right Hand Addition Operator for Ogre Vectors with a Mezzanine::Vector3.
/// @param Vec The Ogre Vector to be added.
/// @param lhs The Mezzanine::Vector3 to be added.
/// @return A Mezzanine::Vector3 with the Sum.
Mezzanine::Vector3 MEZZ_LIB operator+ (const Ogre::Vector3 &Vec, const Mezzanine::Vector3& lhs);

/// @brief Right Hand Subtraction Operator for Ogre Vectors with a Mezzanine::Vector3.
/// @param Vec The Ogre Vector to be subtracted from.
/// @param lhs The Mezzanine::Vector3 to be subtracted.
/// @return A Mezzanine::Vector3 with the difference.
Mezzanine::Vector3 MEZZ_LIB operator- (const Ogre::Vector3 &Vec, const Mezzanine::Vector3& lhs);

/// @brief Right Hand Multiplication Operator for Ogre Vectors with a Mezzanine::Vector3.
/// @param Vec The Ogre Vector to be multiplied.
/// @param lhs The Mezzanine::Vector3 to be multiplied.
/// @return A Mezzanine::Vector3 with the product.
Mezzanine::Vector3 MEZZ_LIB operator* (const Ogre::Vector3 &Vec, const Mezzanine::Vector3& lhs);

/// @brief Right Hand Division Operator for Ogre Vectors with a Mezzanine::Vector3.
/// @param Vec The Ogre Vector to be divided by.
/// @param lhs The Mezzanine::Vector3 to be divided.
/// @return A Mezzanine::Vector3 with the results
Mezzanine::Vector3 MEZZ_LIB operator/ (const Ogre::Vector3 &Vec, const Mezzanine::Vector3& lhs);

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment

// We can skip these operators when creating bindings with swig
#ifndef SWIG

/// @brief Used to Serialize an Mezzanine::Vector3 to a human readable stream
/// @details The current XML format
/// will create one node with no child nodes. The name of the xml node will be "Vector3". It will have 4 attributes.
/// "Version", will be set to a value of 1, indicating if came from version 1 compatible Vector3. It will also have an "X", "Y" and
/// "Z" attributes will values set appropriately. For example '<Vector3 Version="1" X="1" Y="2" Z="3" />'.
/// @param x The Mezzanine::Vector3 to be converted to characters.
/// @param stream The place to send the characters, that define the Mezzanine::Vector3.
/// @return Get an std::ostream that was written to, this allow chaining of the << operators.
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Vector3& x);

/// @brief Used to de-serialize an Mezzanine::Vector3 from a stream
/// @details This reads in the xml and sets the target vector according to values
/// from the stream.
/// @param Vec The Mezzanine::Vector3 that will accept the values from the xml
/// @param stream The place to get the characters from, that define the Mezzanine::Vector3.
/// @return Get an std::ostream that was read from, this allow chaining of the >> operators.
/// @throw Can throw any exception that any function in the Mezzanine::xml namespace could throw in addition to a Mezzanine::Exception if the serialization version doesn't match.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Vector3& Vec);

/// @brief Converts an XML node into a Mezzanine::Vector3
/// @details This will convert an XML::Node will a valid serialized Mezzanine::Vector3 into a Mezzanine::Vector3
/// @param OneNode An XML Node containing the the text of a Vector3
/// @param Vec the Mezzanine::Vector3 to store the deserialized Vector3
/// @return This returns a reference to the XML::Node for operator chaining or whatever.
/// @throw Can throw any exception that any function in the Mezzanine::xml namespace could throw in addition to a Mezzanine::Exception if the serialization version doesn't match.
void MEZZ_LIB  operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Vector3& Vec);

/// @brief Conversion Assignment Operator to Ogre::Vector3.
/// @param VecTo The left hand side vector, is an Ogre::Vector3. The values of VecFrom will be copied here.
/// @param VecFrom The right hand side, is a\ Mezzanine::Vector3, this vector will be copied and unchanged.
/// @return An Ogre::Vector3 in case multiple operators are chainged together (not usually a good idea).
Ogre::Vector3& MEZZ_LIB operator << (Ogre::Vector3& VecTo, const Mezzanine::Vector3& VecFrom);

/// @brief Conversion Assignment Operator to Ogre::Vector3.
/// @param VecTo The left hand side vector, is an Ogre::Vector3. The values of VecFrom will be copied here.
/// @param VecFrom The right hand side, is a btVector3, this vector will be copied and unchanged.
/// @return An Ogre::Vector3 in case multiple operators are chainged together (not usually a good idea).
Ogre::Vector3& MEZZ_LIB operator << (Ogre::Vector3& VecTo, const btVector3& VecFrom);

/// @brief Conversion Assignment Operator to btVector3.
/// @param VecTo The left hand side vector, is an btVector3. The values of VecFrom will be copied here.
/// @param VecFrom The right hand side, is a Ogre::Vector3, this vector will be copied and unchanged.
/// @return An btVector3 in case multiple operators are chainged together (not usually a good idea).
btVector3& MEZZ_LIB operator << (btVector3& VecTo, const Ogre::Vector3& VecFrom);

/// @brief Conversion Assignment Operator to btVector3.
/// @param VecTo The left hand side vector, is an btVector3. The values of VecFrom will be copied here.
/// @param VecFrom The right hand side, is a Mezzanine::Vector3, this vector will be copied and unchanged.
/// @return An btVector3 in case multiple operators are chainged together (not usually a good idea).
btVector3& MEZZ_LIB operator << (btVector3& VecTo, const Mezzanine::Vector3& VecFrom);

/// @brief Conversion Assignment Operator to Mezzanine::Vector3.
/// @param VecTo The left hand side vector, is an Mezzanine::Vector3. The values of VecFrom will be copied here.
/// @param VecFrom The right hand side, is a Ogre::Vector3, this vector will be copied and unchanged.
/// @return An Mezzanine::Vector3 in case multiple operators are chainged together (not usually a good idea).
Mezzanine::Vector3& MEZZ_LIB operator << (Mezzanine::Vector3& VecTo, const Ogre::Vector3& VecFrom);

/// @brief Conversion Assignment Operator to Mezzanine::Vector3.
/// @param VecTo The left hand side vector, is an Mezzanine::Vector3. The values of VecFrom will be copied here.
/// @param VecFrom The right hand side, is a btVector3, this vector will be copied and unchanged.
/// @return An Mezzanine::Vector3 in case multiple operators are chainged together (not usually a good idea).
Mezzanine::Vector3& MEZZ_LIB operator << (Mezzanine::Vector3& VecTo, const btVector3& VecFrom);
#endif // \SWIG

SWIG_INFO_ENDCLASS



#endif // \include gaurd
