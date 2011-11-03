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
#ifndef _entity_h
#define _entity_h

#include "attachable.h"

namespace phys
{
    namespace internal
    {
        class EntityInternalData;
    }
    class SceneManager;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class Entity
    /// @headerfile entity.h
    /// @brief This class is useful for representing graphical bodies without a physics representation.
    /// @details An entity is essentially just a graphical mesh floating in 3D space.
    ///////////////////////////////////////
    class PHYS_LIB Entity : public Attachable
    {
        protected:
            internal::EntityInternalData* EID;
        public:
            /// @brief Class Constructor.
            /// @param Name The name of this entity.
            /// @param MeshName The name of the mesh to be applied to this entity.
            /// @param Group The resource group where the mesh can be found.
            /// @param manager The scenemanager to which this entity belongs.
            Entity(const String& Name, const String& MeshName, const String& Group, SceneManager* manager);

            /// @brief Class Destructor.
            virtual ~Entity();

            ///////////////////////////////////////////////////////////////////////////////
            // Inherited From Attachable

            /// @copydoc phys::Attachable::GetName()
            ConstString& GetName() const;

            /// @copydoc phys::Attachable::GetAttachableType()
            virtual Attachable::AttachableElement GetAttachableType() const;

            /// @copydoc phys::Attachable::SetLocation(const Vector3& Vec)
            virtual void SetLocation(const Vector3& Vec);

            /// @copydoc phys::Attachable::GetLocation()
            virtual Vector3 GetLocation() const;

            /// @copydoc phys::Attachable::SetOrientation(const Quaternion& Orientation)
            void SetOrientation(const Quaternion& Orientation);

            /// @copydoc phys::Attachable::GetOrientation()
            Quaternion GetOrientation() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Entity Functionality

            /// @brief Sets the scale to be applied to this entity.
            /// @param Scale A vector3 with the scales that will be applied to each axis.
            void SetScale(const Vector3& Scale);

            /// @brief Gets the scale currently be applied to this entity.
            /// @return Returns a vector3 with the scale being applied to each axis.
            Vector3 GetScale() const;

            /// @brief Adds this entity to the world, allowing it to render.
            /// @details If the entity is already in the world, this does nothing.
            void AddToWorld();

            /// @brief Removes this entity from the world, preventing it from rendering.
            /// @details If the entity is not in the world, this does nothing.
            void RemoveFromWorld();

            /// @brief Gets whether or not this entity is in the world.
            /// @return True if in the world, false otherwise.
            bool IsInWorld() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Functions

            /// @internal
            /// @brief Gets pointers to the internal ogre structures for this attachable.
            /// @return Returns an AttachableData struct with the internal data.
            virtual AttachableData GetAttachableData() const;
    };//Entity
}//phys

#endif
