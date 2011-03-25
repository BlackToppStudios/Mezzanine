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
#ifndef _quaternion_h
#define _quaternion_h

#include "crossplatformexport.h"
#include "datatypes.h"
#include "xml.h"

class btQuaternion;
namespace Ogre
{
    class Quaternion;
}

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class Quaternion
    /// @headerfile quaternion.h
    /// @brief This is used to store information about rotation in 3d space
    /// @details This is used to store information about rotation in 3d space. The
    /// X, Y and Z are used to identify a ray from the origin (0,0,0), about which
    /// W represents an amount of rotation.
    /// @warning The Documentation for this class needs to be revised. It describes 2 mutually exclusive means of storing
    class PHYS_LIB Quaternion
    {
        public:
            ///////////////////////////////////////////////////////////////////////////////
            //  Data Members

            /// @brief Rotation on the X Axis.
            Real X;
            /// @brief Rotation on the Y Axis.
            Real Y;
            /// @brief Rotation on the Z Axis.
            Real Z;
            /// @brief Rotation on the W Axis.
            Real W;

            ///////////////////////////////////////////////////////////////////////////////
            // Constructors

            /// @brief Blank Constructor.
            /// @details Basic no-initialization constructor.
            Quaternion();

            /// @brief Constructor.
            /// @details Constructor that sets all four axis' of rotation.
            /// @param x Rotation on the X Axis.
            /// @param y Rotation on the Y Axis.
            /// @param z Rotation on the Z Axis.
            /// @param w Rotation on the W Axis.
            Quaternion(const Real &x, const Real &y, const Real &z, const Real &w);

            /// @brief Bullet Quaternion constructor.
            /// @details Constructor that sets all values to match the Bullet quaternion.
            /// @param Theirs The quaternion to be copied to make this quaternion.
            Quaternion(const btQuaternion& Theirs);

            /// @brief Ogre Quaternion constructor.
            /// @details Constructor that sets all values to match the Ogre quaternion.
            /// @param Theirs The quaternion to be copied to make this quaternion.
            Quaternion(const Ogre::Quaternion& Theirs);

            /// @brief Copy Constructor
            /// @param Other The Quaternion to copy
            Quaternion(const phys::Quaternion& Other);

            ///////////////////////////////////////////////////////////////////////////////
            // Explicit Conversion

            /// @brief Gets a Bullet quaternion.
            /// @details Creates a Bullet quaternion with values equal to this class and returns it.
            /// @param normalize Whether or not you want this function to normalize the quaternion for you.
            btQuaternion GetBulletQuaternion(bool normalize=false) const;

            /// @brief Copies an existing Bullet quaternion.
            /// @details This function will copy the values stored in an existing Bullet quaternion
            /// and set the values of this class to be the same.
            /// @param Ours The quaternion to be extracted.
            void ExtractBulletQuaternion(const btQuaternion &Ours);

            /// @brief Gets a Ogre quaternion.
            /// @details Creates a Ogre quaternion with values equal to this class and returns it.
            /// @param normalize Whether or not you want this function to normalize the quaternion for you.
            Ogre::Quaternion GetOgreQuaternion(bool normalize=false) const;

            /// @brief Copies an existing Ogre quaternion.
            /// @details This function will copy the values stored in an existing Ogre quaternion
            /// and set the values of this class to be the same.
            /// @param Ours The quaternion to be extracted.
            void ExtractOgreQuaternion(const Ogre::Quaternion &Ours);

            ///////////////////////////////////////////////////////////////////////////////
            // Left Hand Basic Arithmetic Operators
            Quaternion operator+ (const phys::Quaternion& Other) const;

            Quaternion operator+ (const Ogre::Quaternion& Other) const;

            Quaternion operator+ (const btQuaternion& Other) const;

            Quaternion operator- (const phys::Quaternion& Other) const;

            Quaternion operator- (const Ogre::Quaternion& Other) const;

            Quaternion operator- (const btQuaternion& Other) const;


            ///////////////////////////////////////////////////////////////////////////////
            // Increment and Decrement Operators

            Quaternion& operator+= (const phys::Quaternion& Other);

            Quaternion& operator+= (const Ogre::Quaternion& Other);

            Quaternion& operator+= (const btQuaternion& Other);

            Quaternion& operator-= (const phys::Quaternion& Other);

            Quaternion& operator-= (const Ogre::Quaternion& Other);

            Quaternion& operator-= (const btQuaternion& Other);

            ///////////////////////////////////////////////////////////////////////////////
            // Assignment Operators

            Quaternion& operator= (const phys::Quaternion& Other);

            Quaternion& operator= (const Ogre::Quaternion& Other);

            Quaternion& operator= (const btQuaternion& Other);

    };
}

///////////////////////////////////////////////////////////////////////////////
// Right Hand Addition Operators
phys::Quaternion PHYS_LIB operator+ (const Ogre::Quaternion& Other, const phys::Quaternion& Other2);

phys::Quaternion PHYS_LIB operator+ (const btQuaternion& Other, const phys::Quaternion& Other2);

phys::Quaternion PHYS_LIB operator- (const Ogre::Quaternion& Other, const phys::Quaternion& Other2);

phys::Quaternion PHYS_LIB operator- (const btQuaternion& Other, const phys::Quaternion& Other2);


///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment

btQuaternion& PHYS_LIB operator<< ( btQuaternion& Other, const phys::Quaternion& Other2);

btQuaternion& PHYS_LIB operator<< ( btQuaternion& Other, const Ogre::Quaternion& Other2);

btQuaternion& PHYS_LIB operator<< ( btQuaternion& Other, const btQuaternion& Other2);


phys::Quaternion& PHYS_LIB operator<< ( phys::Quaternion& Other, const phys::Quaternion& Other2);

phys::Quaternion& PHYS_LIB operator<< ( phys::Quaternion& Other, const Ogre::Quaternion& Other2);

phys::Quaternion& PHYS_LIB operator<< ( phys::Quaternion& Other, const btQuaternion& Other2);


Ogre::Quaternion& PHYS_LIB operator<< ( Ogre::Quaternion& Other, const phys::Quaternion& Other2);

Ogre::Quaternion& PHYS_LIB operator<< ( Ogre::Quaternion& Other, const Ogre::Quaternion& Other2);

Ogre::Quaternion& PHYS_LIB operator<< ( Ogre::Quaternion& Other, const btQuaternion& Other2);


/// @brief Used to Serialize an phys::Quaternion to a human readable stream
/// @details If PHYSXML is disabled, this outputs to the format of [x,y,z,w], where x is replaced with the X value,
/// y is replaced with the Y value, Z is replaced with the Z value and W is replaced with the W value. For example [1,2,3,0] could be
/// a serialized Quaternion.\n\n
/// If PHYSXML is defined/enabled the square bracket format is replaced with proper XML output,
/// including versioning information which will be used to maintain backwards compatibility. The current XML format
/// will create one node with no child nodes. The name of the xml node will be "Quaternion". It will have 5 attributes.
/// "Version", will be set to a value of 1, indicating if came from version 1 compatible Quaternion. It will also have an "X", "Y",
/// "Z" and "W" attributes will values set appropriately. For example '<Quaternion Version="1" X="1" Y="2" Z="3" W="0" />'.
/// @param x The phys::Quaternion to be converted to characters.
/// @param stream The place to send the characters, that define the phys::Quaternion.
/// @return Get an std::ostream that was written to, this allow chaining of the << operators.
std::ostream& PHYS_LIB operator << (std::ostream& stream, const phys::Quaternion& x);

#ifdef PHYSXML
/// @brief Used to de-serialize an phys::Quaternion from a stream
/// @details If PHYSXML is enabled, this reads in the xml and sets the target vector according to values
/// from the stream.
/// @param Vec The phys::Quaternion that will accept the values from the xml
/// @param stream The place to get the characters from, that define the phys::Quaternion.
/// @return Get an std::ostream that was read from, this allow chaining of the >> operators.
/// @throw Can throw any exception that any function in the phys::xml namespace could throw in addition to a phys::Exception if the serialization version doesn't match.
std::istream& PHYS_LIB operator >> (std::istream& stream, phys::Quaternion& Vec);

/// @brief Converts an XML node into a phys::Quaternion
/// @details If PHYSXML is enabled, this will convert an xml::Node will a valid serialized phys::Quaternion into a phys::Quaternion
/// @param OneNode An XML Node containing the the text of a Quaternion
/// @param Vec the phys::Quaternion to store the deserialized Quaternion
/// @return This returns a reference to the xml::Node for operator chaining or whatever.
/// @throw Can throw any exception that any function in the phys::xml namespace could throw in addition to a phys::Exception if the serialization version doesn't match.
phys::xml::Node& PHYS_LIB  operator >> (const phys::xml::Node& OneNode, phys::Quaternion& Vec);
#endif // \PHYSXML

#endif
