//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _transform_h
#define _transform_h

#include "vector3.h"
#include "quaternion.h"

///////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief The defintion of the transform is stored in this file.
///////////////////////////////////////

class btTransform;

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief Stores information about relative location and rotation in 3d space
    /// @details This is simple a pair of a vector 3 to store location and a
    /// quaternion to store rotation. This is communly used to interact with the
    /// physics system. This also has a few helper functions to make it more
    /// useful than an std::pair<Vector3,Quaternion>.
    ///////////////////////////////////////
    class Transform
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

#ifdef PHYSXML
            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            // Serializable
            /// @brief Convert this class to an xml::Node ready for serialization
            /// @param CurrentRoot The point in the XML hierarchy that all this vectorw should be appended to.
            virtual void ProtoSerialize(xml::Node& CurrentRoot) const;

            // DeSerializable
            /// @brief Take the data stored in an XML and overwrite this instance of this object with it
            /// @param OneNode and xml::Node containing the data.
            /// @warning A precondition of using this is that all of the actors intended for use must already be Deserialized.
            virtual void ProtoDeSerialize(const xml::Node& OneNode);

            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "Transform"
            String SerializableName() const;
#endif // /PHYSXML

    };
}

/// @brief Overwrite the data in a btTransform with the data in a phys::Transform using an intelligent assignment operator (in this case we really couldn't code the real assignment operator).
/// @param lhs The item on the Left Hand of the Sign, the item to be assigned to.
/// @param rhs The item on the Right Hand of the Sign, the item that has the values to be copied.
/// @return This returns the original btTransform reference(with the new values) so furhter work can continue to be performed if required
btTransform& operator<< (btTransform& lhs, const phys::Transform& rhs);

/// @brief Overwrite the data in a phys::Transform with the data in a btTransform using an intelligent assignment operator (in this case we really couldn't code the real assignment operator).
/// @param lhs The item on the Left Hand of the Sign, the item to be assigned to.
/// @param rhs The item on the Right Hand of the Sign, the item that has the values to be copied.
/// @return This returns the original phys::Transform reference(with the new values) so furhter work can continue to be performed if required.
phys::Transform& operator<< (phys::Transform& lhs, const btTransform& rhs);



#endif
