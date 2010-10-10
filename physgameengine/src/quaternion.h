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

#include "datatypes.h"

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
    class Quaternion
    {
        private:
            /// @brief Rotation on the X Axis.
            Real X;
            /// @brief Rotation on the Y Axis.
            Real Y;
            /// @brief Rotation on the Z Axis.
            Real Z;
            /// @brief Rotation on the W Axis.
            Real W;

        public:
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

            /// @brief Gets a Bullet quaternion.
            /// @details Creates a Bullet quaternion with values equal to this class and returns it.
            /// @param normalize Whether or not you want this function to normalize the quaternion for you.
            btQuaternion GetBulletQuaternion(bool normalize=false);

            /// @brief Copies an existing Bullet quaternion.
            /// @details This function will copy the values stored in an existing Bullet quaternion
            /// and set the values of this class to be the same.
            /// @param Ours The quaternion to be extracted.
            void ExtractBulletQuaternion(const btQuaternion &Ours);

            /// @brief Gets a Ogre quaternion.
            /// @details Creates a Ogre quaternion with values equal to this class and returns it.
            /// @param normalize Whether or not you want this function to normalize the quaternion for you.
            Ogre::Quaternion GetOgreQuaternion(bool normalize=false);

            /// @brief Copies an existing Ogre quaternion.
            /// @details This function will copy the values stored in an existing Ogre quaternion
            /// and set the values of this class to be the same.
            /// @param Ours The quaternion to be extracted.
            void ExtractOgreQuaternion(const Ogre::Quaternion &Ours);
    };
}

#endif
