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
#ifndef _physicssliderconstraint_h
#define _physicssliderconstraint_h

#include "Physics/dualtransformconstraint.h"

class btSliderConstraint;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class SliderConstraint
        /// @headerfile constraint.h
        /// @brief
        /// @details This class is incomplete
        ///////////////////////////////////////
        class MEZZ_LIB SliderConstraint : public DualTransformConstraint
        {
        protected:
            /// @brief Bullet constraint that this class encapsulates.
            btSliderConstraint* Slider;
        public:
            SliderConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& VectorA, const Vector3& VectorB, const Quaternion& QuaternionA, const Quaternion& QuaternionB, bool UseLinearReferenceA);
            /// @brief Two body Terse constructor
            /// @param ActorA The First body to be bound
            /// @param ActorB  The Second body to be bound
            /// @param TransformA The offset and rotation from ActorA's center of gravity to get to match an offset from ActorB
            /// @param TransformB The offset and rotation from ActorB's center of gravity.
            /// @param UseLinearReferenceA Perform Linear math from ActorA's perspective, default to false.
            SliderConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Transform& TransformA, const Transform& TransformB, bool UseLinearReferenceA = false);
            SliderConstraint(ActorRigid* ActorB, const Vector3& VectorB, const Quaternion& QuaternionB, bool UseLinearReferenceA);
            /// @brief Class destructor.
            /// @details The class destructor.
            virtual ~SliderConstraint();

            ////////////////////////////////////////////////////////////////////////////////
            // SliderConstraint Location and Rotation
            /// @copydoc DualTransformConstraint::SetPivotATransform(const Transform&)
            virtual void SetPivotATransform(const Transform& TranA);
            /// @copydoc DualTransformConstraint::SetPivotBTransform(const Transform&)
            virtual void SetPivotBTransform(const Transform& TranB);
            /// @copydoc DualTransformConstraint::GetPivotATransform()
            virtual Transform GetPivotATransform() const;
            /// @copydoc DualTransformConstraint::GetPivotBTransform()
            virtual Transform GetPivotBTransform() const;

            /// @copydoc DualTransformConstraint::SetPivotALocation(const Vector3&)
            virtual void SetPivotALocation(const Vector3& Location);
            /// @copydoc DualTransformConstraint::SetPivotBLocation(const Vector3&)
            virtual void SetPivotBLocation(const Vector3& Location);
            /// @copydoc DualTransformConstraint::GetPivotALocation()
            virtual Vector3 GetPivotALocation() const;
            /// @copydoc DualTransformConstraint::GetPivotBLocation()
            virtual Vector3 GetPivotBLocation() const;

            /// @copydoc DualTransformConstraint::SetPivotARotation(const Quaternion&)
            virtual void SetPivotARotation(const Quaternion& Rotation);
            /// @copydoc DualTransformConstraint::SetPivotBRotation(const Quaternion&)
            virtual void SetPivotBRotation(const Quaternion& Rotation);
            /// @copydoc DualTransformConstraint::GetPivotARotation()
            virtual Quaternion GetPivotARotation() const;
            /// @copydoc DualTransformConstraint::GetPivotBRotation()
            virtual Quaternion GetPivotBRotation() const;

            virtual void SetUpperLinLimit(const Real& UpperLimit);
            virtual void SetUpperAngLimit(const Real& UpperLimit);
            virtual void SetLowerLinLimit(const Real& LowerLimit);
            virtual void SetLowerAngLimit(const Real& LowerLimit);
            virtual void SetPoweredLinMotor(bool OnOff);
            virtual void SetTargetLinMotorVelocity(const Real& TargetLinMotorVelocity);
            virtual void SetMaxLinMotorForce(const Real& MaxLinMotorForce);
            virtual void SetPoweredAngMotor(bool OnOff);
            virtual void SetTargetAngMotorVelocity(const Real& TargetAngMotorVelocity);
            virtual void SetMaxAngMotorForce(const Real& MaxAngMotorForce);
            virtual void SetUseFrameOffset(bool FrameOffset);
            virtual void SetSoftnessDirLin(const Real& SoftnessDirLin);
            virtual void SetRestitutionDirLin(const Real& RestitutionDirLin);
            virtual void SetDampingDirLin(const Real& DampingDirLin);
            virtual void SetSoftnessDirAng(const Real& SoftnessDirAng);
            virtual void SetRestitutionDirAng(const Real& RestitutionDirAng);
            virtual void SetDampingDirAng(const Real& DampingDirAng);
            virtual void SetSoftnessLimLin(const Real& SoftnessLimLin);
            virtual void SetRestitutionLimLin(const Real& RestitutionLimLin);
            virtual void SetDampingLimLin(const Real& DampingLimLin);
            virtual void SetSoftnessLimAng(const Real& SoftnessLimAng);
            virtual void SetRestitutionLimAng(const Real& RestitutionLimAng);
            virtual void SetDampingLimAng(const Real& DampingLimAng);
            virtual void SetSoftnessOrthoLin(const Real& SoftnessOrthoLin);
            virtual void SetRestitutionOrthoLin(const Real& RestitutionOrthoLin);
            virtual void SetDampingOrthoLin(const Real& DampingOrthoLin);
            virtual void SetSoftnessOrthoAng(const Real& SoftnessOrthoAng);
            virtual void SetRestitutionOrthoAng(const Real& RestitutionOrthoAng);
            virtual void SetDampingOrthoAng(const Real& DampingOrthoAng);

            ////////////////////////////////////////////////////////////////////////////////
            // SliderConstraint Axis, Params and other Details
            /// @copydoc Constraint::ValidParamOnAxis(int) const
            virtual Constraint::ParamList ValidParamOnAxis(int Axis) const;
            /// @copydoc Constraint::ValidLinearAxis() const
            virtual Constraint::AxisList ValidLinearAxis() const;
            /// @copydoc Constraint::ValidAngularAxis() const
            virtual Constraint::AxisList ValidAngularAxis() const;
            /// @copydoc Constraint::ValidAngularAxis(ConstraintParam,int) const
            virtual bool HasParamBeenSet(ConstraintParam Param, int Axis) const;

            /// @copydoc Constraint::GetConstraintBase() const
            virtual btTypedConstraint* GetConstraintBase() const;
        };//SliderConstraint
    }//Physics
}//Mezzanine

#endif
