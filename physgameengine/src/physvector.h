//© Copyright 2010 Joseph Toppi and John Blackwood
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
#ifndef _physvector_h
#define _physvector_h

#include "datatypes.h"

using namespace phys;

//Forward Declarations for wierd compatibility functions
class btVector3;
namespace Ogre
{
    class Vector3;
}

class PhysVector3
{
	public:
        /// @brief Coordinate on the X vector.
		Real X;
		/// @brief Coordinate on the Y vector.
		Real Y;
		/// @brief Coordinate on the Z vector.q
		Real Z;

        /// @brief Default Constructor.
        /// @details Basic all zero initialization constructor.
        PhysVector3();

        /// @brief Real value Constructor.
        /// @details Constructor that sets all three vectors.
        /// @param X Coordinate on the X vector.
        /// @param Y Coordinate on the Y vector.
        /// @param Z Coordinate on the Z vector.
		PhysVector3(Real X, Real Y, Real Z);

		/// @brief Ogre Value Constructor.
        /// @details Constructor that sets all values to match the Ogre vector.
        /// @param Vec The vector to be copied to make this vector
		PhysVector3(Ogre::Vector3 Vec);

		/// @brief Bullet Value Constructor.
        /// @details Constructor that sets all values to match the Bullet vector.
        /// @param Vec The vector to be copied to make this vector
		PhysVector3(btVector3 Vec);

        /// @brief Gets a Bullet vector3.
        /// @details Creates a Bullet vector3 with values equal to this class and returns it.
        btVector3 GetBulletVector3() const;
        /// @brief Copies an existing Bullet vector3.
        /// @details This function will copy the values stored in an existing Bullet vector3
        /// and set the values of this class to be the same.
        /// @param temp The vector3 to be extracted.
        void ExtractBulletVector3(btVector3 temp);

        /// @brief Gets a Ogre vector3.
        /// @details Creates a Ogre vector3 with values equal to this class and returns it.
        Ogre::Vector3 GetOgreVector3() const;
        /// @brief Copies an existing Ogre vector3.
        /// @details This function will copy the values stored in an existing Ogre vector3
        /// and set the values of this class to be the same.
        /// @param Temp The vector3 to be extracted.
        void ExtractOgreVector3(Ogre::Vector3 temp);

        void operator= (const btVector3 &bt3);
        void operator= (const Ogre::Vector3 &OVec3);
};

std::ostream& operator << (std::ostream& stream, const PhysVector3& x);

Ogre::Vector3& operator << (Ogre::Vector3& VecTo, const PhysVector3& VecFrom);
Ogre::Vector3& operator << (Ogre::Vector3& VecTo, const btVector3& VecFrom);

btVector3& operator << (btVector3& VecTo, const Ogre::Vector3& VecFrom);
btVector3& operator << (btVector3& VecTo, const PhysVector3& VecFrom);

PhysVector3& operator << (PhysVector3& VecTo, const Ogre::Vector3& VecFrom);
PhysVector3& operator << (PhysVector3& VecTo, const btVector3& VecFrom);

#endif
