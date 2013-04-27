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
#ifndef _worldproxy_h
#define _worldproxy_h

#include "transformableobject.h"

namespace Mezzanine
{
    class WorldObject;
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is the base class for proxy objects belonging to the various 3D subsystems.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB WorldProxy : public TransformableObject
    {
        protected:
            /// @internal
            /// @brief Pointer to the Object this proxy belongs to.
            WorldObject* ParentObject;
        public:
            /// @brief Class constructor.
            WorldProxy();
            /// @brief Class destructor.
            virtual ~WorldProxy();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets a pointer to the parent object that owns this proxy.
            /// @return Returns a pointer to the WorldObject this proxy belongs to, or NULL if this proxy isn't bound to a WorldObject.
            WorldObject* GetParentObject() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Transform Methods

            /// @copydoc TransformableObject::SetLocation(const Vector3& Location)
            /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
            virtual void SetLocation(const Vector3& Location) = 0;
            /// @copydoc TransformableObject::SetLocation(const Real X, const Real Y, const Real Z)
            /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
            virtual void SetLocation(const Real X, const Real Y, const Real Z) = 0;
            /// @copydoc TransformableObject::GetLocation() const
            /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
            virtual Vector3 GetLocation() const = 0;
            /// @copydoc TransformableObject::SetOrientation(const Quaternion& Orientation)
            /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
            virtual void SetOrientation(const Quaternion& Orientation) = 0;
            /// @copydoc TransformableObject::SetOrientation(const Real X, const Real Y, const Real Z, const Real W)
            /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
            virtual void SetOrientation(const Real X, const Real Y, const Real Z, const Real W) = 0;
            /// @copydoc TransformableObject::GetOrientation() const
            /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
            virtual Quaternion GetOrientation() const = 0;
            /// @copydoc TransformableObject::SetScale(const Vector3& Scale)
            /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
            virtual void SetScale(const Vector3& Scale) = 0;
            /// @copydoc TransformableObject::SetScale(const Real X, const Real Y, const Real Z)
            /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
            virtual void SetScale(const Real X, const Real Y, const Real Z) = 0;
            /// @copydoc TransformableObject::GetScale() const
            /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
            virtual Vector3 GetScale() const = 0;

            /// @copydoc TransformableObject::Translate(const Vector3& Trans)
            /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
            virtual void Translate(const Vector3& Trans) = 0;
            /// @copydoc TransformableObject::Translate(const Real X, const Real Y, const Real Z)
            /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
            virtual void Translate(const Real X, const Real Y, const Real Z) = 0;
            /// @copydoc TransformableObject::Yaw(const Real Angle)
            /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
            virtual void Yaw(const Real Angle) = 0;
            /// @copydoc TransformableObject::Pitch(const Real Angle)
            /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
            virtual void Pitch(const Real Angle) = 0;
            /// @copydoc TransformableObject::Roll(const Real Angle)
            /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
            virtual void Roll(const Real Angle) = 0;
            /// @copydoc TransformableObject::Rotate(const Vector3& Axis, const Real Angle)
            /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
            virtual void Rotate(const Vector3& Axis, const Real Angle) = 0;
            /// @copydoc TransformableObject::Rotate(const Quaternion& Rotation)
            /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
            virtual void Rotate(const Quaternion& Rotation) = 0;
            /// @copydoc TransformableObject::Scale(const Vector3& Scale)
            /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
            virtual void Scale(const Vector3& Scale) = 0;
            /// @copydoc TransformableObject::Scale(const Real X, const Real Y, const Real Z)
            /// @warning Calling this method directly can de-sync a WorldObject.  Do NOT do this unless you know exactly what you are doing.
            virtual void Scale(const Real X, const Real Y, const Real Z) = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Binds this proxy to a WorldObject.
            /// @param NewParent A pointer to the WorldObject taking possession of this proxy.
            void _Bind(WorldObject* NewParent);
    };//WorldProxy
}//Mezzanine

#endif
