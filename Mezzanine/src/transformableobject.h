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
#ifndef _transformableobject_h
#define _transformableobject_h

#include "enumerations.h"
#include "vector3.h"
#include "quaternion.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is an interface for all 3D objects that can have their full transforms manipulated.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB TransformableObject
    {
    public:
        /// @brief Sets the location of this object in parent space.
        /// @param Location A Vector3 representing the location this object is to be set to.
        virtual void SetLocation(const Vector3& Loc) = 0;
        /// @brief Sets the location of this object in parent space via Reals.
        /// @param X The position on the X axis where this object is to be set.
        /// @param Y The position on the Y axis where this object is to be set.
        /// @param Z The position on the Z axis where this object is to be set.
        virtual void SetLocation(const Real X, const Real Y, const Real Z) = 0;
        /// @brief Gets this objects current location.
        /// @return Returns Vector3 representing the current rotation of this object in parent space.
        virtual Vector3 GetLocation() const = 0;
        /// @brief Sets the orientation of this object in parent space.
        /// @param Orientation A Quaternion representing the rotation this object is to be set to.
        virtual void SetOrientation(const Quaternion& Ori) = 0;
        /// @brief Sets the orientation of this object in parent space via Reals.
        /// @param X The X component of the Axis.
        /// @param Y The Y component of the Axis.
        /// @param Z The Z component of the Axis.
        /// @param W Rotation on the Axis X, Y and Z defined.
        virtual void SetOrientation(const Real X, const Real Y, const Real Z, const Real W) = 0;
        /// @brief Gets this objects current orientation.
        /// @return Returns a Quaternion representing the current orientation of this object in parent space.
        virtual Quaternion GetOrientation() const = 0;
        /// @brief Sets the scaling to be applied to this object.
        /// @param Scale A Vector3 representing the amount of scaling to apply to this object.
        virtual void SetScale(const Vector3& Sc) = 0;
        /// @brief Sets the scaling to be applied to this object via Reals.
        /// @param X The scaling factor to apply on the X axis.
        /// @param Y The scaling factor to apply on the Y axis.
        /// @param Z The scaling factor to apply on the Z axis.
        virtual void SetScale(const Real X, const Real Y, const Real Z) = 0;
        /// @brief Gets the scaling currently being applied to this object.
        /// @return Returns a Vector3 representing the amount this object is being scaled.
        virtual Vector3 GetScale() const = 0;

        /// @brief Moves this object from it's current location.
        /// @note This method has an additive effect with the objects current location.  Unlike "SetLocation" this method does not replace
        /// the existing location with what you provide.
        /// @param Trans A Vector3 representing the amount of movement to apply to this object.
        virtual void Translate(const Vector3& Trans) = 0;
        /// @brief Moves this object from it's current location.
        /// @note This method has an additive effect with the objects current location.  Unlike "SetLocation" this method does not replace
        /// the existing location with what you provide.
        /// @param X The amount of movement to apply on the X axis.
        /// @param Y The amount of movement to apply on the Y axis.
        /// @param Z The amount of movement to apply on the Z axis.
        virtual void Translate(const Real X, const Real Y, const Real Z) = 0;
        /// @brief Rotate the object around the Y axis.
        /// @param Angle The angle to rotate this object in radians.
        virtual void Yaw(const Real Angle) = 0;
        /// @brief Rotate the object around the X axis.
        /// @param Angle The angle to rotate this object in radians.
        virtual void Pitch(const Real Angle) = 0;
        /// @brief Rotate the object around the Z axis.
        /// @param Angle The angle to rotate this object in radians.
        virtual void Roll(const Real Angle) = 0;
        /// @brief Rotates the object from it's existing rotation.
        /// @note This method has an additive effect with the objects current rotation.  Unlike "SetOrientation" this method does not replace
        /// the existing orientation with what you provide.
        /// @param Axis The axis on which this object will be rotated.
        /// @param Angle The angle to rotate this object in radians.
        virtual void Rotate(const Vector3& Axis, const Real Angle) = 0;
        /// @brief Rotates the object from it's existing rotation.
        /// @note This method has an additive effect with the objects current rotation.  Unlike "SetOrientation" this method does not replace
        /// the existing orientation with what you provide.
        /// @param Rotation The rotation to apply to this object.
        virtual void Rotate(const Quaternion& Rotation) = 0;
        /// @brief Scales the object from it's current size.
        /// @note This method has an additive effect with the objects' current scaling.  Unlike "SetScale" this method does not replace
        /// the existing scale with what you provide.
        /// @param Scale A Vector3 representing the scaling to apply to this object.
        virtual void Scale(const Vector3& Scale) = 0;
        /// @brief Scales the object from it's current size.
        /// @note This method has an additive effect with the objects' current scaling.  Unlike "SetScale" this method does not replace
        /// the existing scale with what you provide.
        /// @param X The scaling factor to apply on the X axis.
        /// @param Y The scaling factor to apply on the Y axis.
        /// @param Z The scaling factor to apply on the Z axis.
        virtual void Scale(const Real X, const Real Y, const Real Z) = 0;

        ///////////////////////////////////////////////////////////////////////////////
        // Conversion Functions

        /// @brief Converts a point in local space to the same point in global space.
        /// @param Location The point in local space to be converted.
        /// @return Returns a Vector3 representing the point in global space corresponding to the provided local space point.
        virtual Vector3 ConvertLocalToGlobal(const Vector3& Location) const;
        /// @brief Converts a point in global space to the same point in local space.
        /// @param Location The point in global space to be converted.
        /// @return Returns a Vector3 representing the point in local space corresponding to the provided global space point.
        virtual Vector3 ConvertGlobalToLocal(const Vector3& Location) const;
        /// @brief Converts a rotation in local space to the same rotation in global space.
        /// @param Orientation The rotation in local space to be converted.
        /// @return Returns a Quaternion representing the rotation in global space corresponding to the provided local space rotation.
        virtual Quaternion ConvertLocalToGlobal(const Quaternion& Orientation) const;
        /// @brief Converts a rotation in global space to the same rotation in local space.
        /// @param Orientation The rotation in global space to be converted.
        /// @return Returns a Quaternion representing the rotation in local space corresponding to the provided global space rotation.
        virtual Quaternion ConvertGlobalToLocal(const Quaternion& Orientation) const;
    };//TransformableObject

    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This is an interface for all child 3D objects that can have their full transforms manipulated.
    /// @details Unlike TransformableObject, this interface has added parameters to it's methods for defining
    /// which transform space the operation is to occur in.
    ///////////////////////////////////////
    class MEZZ_LIB TransformableChildObject
    {
    public:
        /// @brief Sets the location of this object in parent space.
        /// @param Location A Vector3 representing the location this object is to be set to.
        virtual void SetLocation(const Vector3& Loc) = 0;
        /// @brief Sets the location of this object in parent space via Reals.
        /// @param X The position on the X axis where this object is to be set.
        /// @param Y The position on the Y axis where this object is to be set.
        /// @param Z The position on the Z axis where this object is to be set.
        virtual void SetLocation(const Real X, const Real Y, const Real Z) = 0;
        /// @brief Gets this objects current location.
        /// @return Returns Vector3 representing the current rotation of this object in parent space.
        virtual Vector3 GetLocation() const = 0;
        /// @brief Sets the orientation of this object in parent space.
        /// @param Orientation A Quaternion representing the rotation this object is to be set to.
        virtual void SetOrientation(const Quaternion& Ori) = 0;
        /// @brief Sets the orientation of this object in parent space via Reals.
        /// @param X The X component of the Axis.
        /// @param Y The Y component of the Axis.
        /// @param Z The Z component of the Axis.
        /// @param W Rotation on the Axis X, Y and Z defined.
        virtual void SetOrientation(const Real X, const Real Y, const Real Z, const Real W) = 0;
        /// @brief Gets this objects current orientation.
        /// @return Returns a Quaternion representing the current orientation of this object in parent space.
        virtual Quaternion GetOrientation() const = 0;
        /// @brief Sets the scaling to be applied to this object.
        /// @param Scale A Vector3 representing the amount of scaling to apply to this object.
        virtual void SetScale(const Vector3& Sc) = 0;
        /// @brief Sets the scaling to be applied to this object via Reals.
        /// @param X The scaling factor to apply on the X axis.
        /// @param Y The scaling factor to apply on the Y axis.
        /// @param Z The scaling factor to apply on the Z axis.
        virtual void SetScale(const Real X, const Real Y, const Real Z) = 0;
        /// @brief Gets the scaling currently being applied to this object.
        /// @return Returns a Vector3 representing the amount this object is being scaled.
        virtual Vector3 GetScale() const = 0;

        /// @brief Moves this object from it's current location.
        /// @note This method has an additive effect with the objects current location.  Unlike "SetLocation" this method does not replace
        /// the existing location with what you provide.
        /// @param Trans A Vector3 representing the amount of movement to apply to this object.
        /// @param Space The transform space in which to apply this translation.
        virtual void Translate(const Vector3& Trans, const Mezzanine::TransformSpace Space = Mezzanine::TS_Parent) = 0;
        /// @brief Moves this object from it's current location.
        /// @note This method has an additive effect with the objects current location.  Unlike "SetLocation" this method does not replace
        /// the existing location with what you provide.
        /// @param X The amount of movement to apply on the X axis.
        /// @param Y The amount of movement to apply on the Y axis.
        /// @param Z The amount of movement to apply on the Z axis.
        /// @param Space The transform space in which to apply this translation.
        virtual void Translate(const Real X, const Real Y, const Real Z, const Mezzanine::TransformSpace Space = Mezzanine::TS_Parent) = 0;
        /// @brief Rotate the object around the Y axis.
        /// @param Angle The angle to rotate this object in radians.
        /// @param Space The transform space in which to apply this rotation.
        virtual void Yaw(const Real Angle, const Mezzanine::TransformSpace Space = Mezzanine::TS_Parent) = 0;
        /// @brief Rotate the object around the X axis.
        /// @param Angle The angle to rotate this object in radians.
        /// @param Space The transform space in which to apply this rotation.
        virtual void Pitch(const Real Angle, const Mezzanine::TransformSpace Space = Mezzanine::TS_Parent) = 0;
        /// @brief Rotate the object around the Z axis.
        /// @param Angle The angle to rotate this object in radians.
        /// @param Space The transform space in which to apply this rotation.
        virtual void Roll(const Real Angle, const Mezzanine::TransformSpace Space = Mezzanine::TS_Parent) = 0;
        /// @brief Rotates the object from it's existing rotation.
        /// @note This method has an additive effect with the objects current rotation.  Unlike "SetOrientation" this method does not replace
        /// the existing orientation with what you provide.
        /// @param Axis The axis on which this object will be rotated.
        /// @param Angle The angle to rotate this object in radians.
        /// @param Space The transform space in which to apply this rotation.
        virtual void Rotate(const Vector3& Axis, const Real Angle, const Mezzanine::TransformSpace Space = Mezzanine::TS_Parent) = 0;
        /// @brief Rotates the object from it's existing rotation.
        /// @note This method has an additive effect with the objects current rotation.  Unlike "SetOrientation" this method does not replace
        /// the existing orientation with what you provide.
        /// @param Rotation The rotation to apply to this object.
        /// @param Space The transform space in which to apply this rotation.
        virtual void Rotate(const Quaternion& Rotation, const Mezzanine::TransformSpace Space = Mezzanine::TS_Parent) = 0;
        /// @brief Scales the object from it's current size.
        /// @note This method has an additive effect with the objects' current scaling.  Unlike "SetScale" this method does not replace
        /// the existing scale with what you provide.
        /// @param Scale A Vector3 representing the scaling to apply to this object.
        virtual void Scale(const Vector3& Scale) = 0;
        /// @brief Scales the object from it's current size.
        /// @note This method has an additive effect with the objects' current scaling.  Unlike "SetScale" this method does not replace
        /// the existing scale with what you provide.
        /// @param X The scaling factor to apply on the X axis.
        /// @param Y The scaling factor to apply on the Y axis.
        /// @param Z The scaling factor to apply on the Z axis.
        virtual void Scale(const Real X, const Real Y, const Real Z) = 0;
    };//TransformableChildObject
}//Mezzanine

#endif
