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
#ifndef _circle_h
#define _circle_h

#include "vector2.h"
#include "vector3.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief A generic circle class for geometry calculations.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB Circle
    {
    public:
        ///////////////////////////////////////////////////////////////////////////////
        // Public Data Members

        /// @brief The center point of the circle.
        Vector2 Center;
        /// @brief The radius of the circle.
        Real Radius;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Class constructor.
        Circle();
        /// @brief Copy constructor.
        /// @param Other The other circle to be copied.
        Circle(const Circle& Other) = default;
        /// @brief Move constructor.
        /// @param Other The other circle to be moved.
        Circle(Circle&& Other) = default;
        /// @brief Descriptive constructor.
        /// @param CircleCenter The center point of the circle.
        /// @param CircleRadius The radius of the circle.
        Circle(const Vector2& CircleCenter, const Real CircleRadius);
        /// @brief Vector constructor.
        /// @param Point1 The first point determining the circles bounds.
        /// @param Point2 The second point determining the circles bounds.
        /// @param Point3 The third point determining the circles bounds.
        Circle(const Vector2& Point1, const Vector2& Point2, const Vector2& Point3);
        /// @brief Class destructor.
        ~Circle();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets whether or not a point is inside this circle.
        /// @param Point The point to check against this circle.
        /// @return Returns true if the specified point is inside this circle, false otherwise.
        Boole IsInside(const Vector2& Point) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Assignment Operators

        /// @brief Assignment operator.
        /// @param Other The other Circle to be copied.
        /// @return Returns a reference to this.
        Circle& operator=(const Circle& Other) = default;
        /// @brief Move assignment operator.
        /// @param Other The other Circle to be moved.
        /// @return Returns a reference to this.
        Circle& operator=(Circle&& Other) = default;
    };//Circle
}//Mezzanine

#endif
