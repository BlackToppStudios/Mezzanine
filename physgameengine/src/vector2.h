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
#ifndef _vector2_h
#define _vector2_h

#include "datatypes.h"

namespace Ogre
{
    class Vector2;
}

namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class Vector2
    /// @headerfile vector2.h
    /// @brief This is used to represent a point on a 2 dimentional area, such as a screen.
    /// @details This contains an X and Y value used to represent coordinates.
    /// This also has a number of facilities to make converting to graphics subsystems
    /// as easy as possible.
    ///////////////////////////////////////
    class Vector2
    {
        public:
            /// @brief Coordinate on the X vector.
            Real X;
            /// @brief Coordinate on the Y vector.
            Real Y;
            /// @brief Default Constructor.
            /// @details Basic all zero initialization constructor.
            Vector2();
            /// @brief Real value Constructor.
            /// @details Constructor that sets both vectors.
            /// @param X Coordinate on the X vector.
            /// @param Y Coordinate on the Y vector.
            Vector2(Real x, Real y);
            /// @brief Ogre Value Constructor.
            /// @details Constructor that sets all values to match the Ogre vector.
            /// @param Vec The vector to be copied to make this vector.
            Vector2(Ogre::Vector2 Vec);
            /// @brief Gets a Ogre vector2.
            /// @details Creates a Ogre vector2 with values equal to this class and returns it.
            Ogre::Vector2 GetOgreVector2() const;
            /// @brief Copies an existing Ogre vector2.
            /// @details This function will copy the values stored in an existing Ogre vector2
            /// and set the values of this class to be the same.
            /// @param temp The vector2 to be extracted.
            void ExtractOgreVector2(Ogre::Vector2 Ours);

            ///////////////////////////////////////////////////////////////////////////////
            // Vector2 Arithmetic with Real
            /// @brief Scaling by multiplication
            /// @details This Multiplies X, Y and Z by scalar
            /// @return This returns a Vector2 that has been scaled
            /// @param scalar This is the amount to scale the Vector2 by
            Vector2 operator* (const Real &scalar) const;

            /// @brief Scaling by Division
            /// @details This Diisionn X, Y and Z by scalar
            /// @return This returns a Vector2 that has been scaled
            /// @param scalar This is the amount to scale the Vector2 by
            Vector2 operator/ (const Real &scalar) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Vector2 Arithmetic and assignment with Real
            /// @brief Scaling by multiplication.
            /// @details This Multiplies X, Y and Z by scalar and stores the changes in this Vector2.
            /// @param scalar This is the amount to scale the Vector2 by.
            void operator*= (const Real &scalar);

            /// @brief Scaling by Division
            /// @details This Division X, Y and Z by scalar and and stores the changes in this Vector2.
            /// @param scalar This is the amount to scale the Vector2 by
            void operator/= (const Real &scalar);

            ///////////////////////////////////////////////////////////////////////////////
            // Arithmetic Operators
            /// @brief Addition Operator
            /// @details Allows for addition from a phys::Vector2
            /// @param Vec2 This is the other phys::Vector2
            Vector2 operator+ (const Vector2 &Vec2) const;

            /// @brief Subraction Operator
            /// @details Allows for subtraction from a phys::Vector2
            /// @param Vec2 This is the other phys::Vector2
            Vector2 operator- (const Vector2 &Vec2) const;

            /// @brief Multiplaction Operator
            /// @details Allows for multiplaction from a phys::Vector2
            /// @param Vec2 This is the other phys::Vector2
            Vector2 operator* (const Vector2 &Vec2) const;

            /// @brief Division Operator
            /// @details Allows for division from a phys::Vector2
            /// @param Vec2 This is the other phys::Vector2
            Vector2 operator/ (const Vector2 &Vec2) const;
    };
}

#endif
