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
#ifndef _areaeffect_h
#define _areaeffect_h

#include "datatypes.h"
#include "quaternion.h"
#include "vector3.h"

class btCollisionShape;
class btGhostObject;

namespace phys{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class AreaEffect
    /// @headerfile areaeffect.h
    /// @brief This class is used to define area's in the world that have unique effects.
    /// @details Common uses for this class are for gravity fields, and explosions.  But can be made to do more. @n
    /// Note: This is a base class intended to be derived from.  This class cannot be created itself.  To make an
    /// AreaEffect class that does what you want it to, simple inherit from this class with an AE class of your own,
    /// and define the ApplyEffect() function to do what you want your effect to do.
    ///////////////////////////////////////
    class AreaEffect
    {
        friend class PhysicsManager;
        protected:
            /// @brief The name of the Area Effect.
            String Name;
            /// @brief Stores the shape of the AE field.
            btCollisionShape* Shape;
            /// @brief The object representing the AE field itself.
            btGhostObject* Ghost;
            /// @brief Constructor Function.
            /// @param Location The location of the AE field.
            virtual void CreateGhostObject(Vector3 Location);
        public:
            /// @brief Constructor.
            /// @details Basic initialization constructor.
            /// @param Location The location of the AE field.
            AreaEffect(const String &name, Vector3 Location);
            /// @brief Destructor.
            /// @details Class destructor.
            virtual ~AreaEffect();
            /// @brief Defines and applies the effect of the field.
            /// @details When inheriting this class, this function is what defines the effect the field has. @n
            /// This function will be called on by the physics manager and shouldn't be called manually.
            virtual void ApplyEffect() = 0;
            /// @brief Creates a Sphere shape for the field.
            /// @param Radius The radius of the sphere you want to create.
            void CreateSphereShape(Real Radius);
            /// @brief Creates a Cylinder shape for the field.
            /// @details This function assumes the cylinder will be upright, meaning "up" will be on the positive Y axis.  @n
            /// When making the vector to be passed in, remember the layout should be as such: (radius, height*0.5, radius).
            /// @param HalfExtents The vector representing the size of the shape.
            void CreateCylinderShape(Vector3 HalfExtents);
            /// @brief Creates a Box shape for the field.
            /// @details When making the vector to be passed in, remember to pass in only half values of what you want the actual
            /// size to be.
            /// @param HalfExtents The vector representing the size of the shape.
            void CreateBoxShape(Vector3 HalfExtents);
            /// @brief Creates a shape from a .mesh model for the field.
            /// @param Filename The name of the .mesh file to be used.
            /// @param Group The resource group where the mesh can be found.
            void CreateShapeFromMesh(String Filename, String Group);
            /// @brief Gets the Area Effects name.
            /// @return Returns the name of the Area Effect.
            String& GetName();
    };//areaeffect
}//phys

#endif
