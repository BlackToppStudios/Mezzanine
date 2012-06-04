//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _conetwistconstraint_h
#define _conetwistconstraint_h

#include "Physics/constraint.h"

class btConeTwistConstraint;

namespace Mezzanine
{
    namespace Physics
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class ConeTwistConstraint
        /// @headerfile constraint.h
        /// @brief
        /// @details This class is currently incomplete
        ///////////////////////////////////////
        class MEZZ_LIB ConeTwistConstraint : public Constraint
        {
            protected:
                /// @brief Bullet constraint that this class encapsulates.
                btConeTwistConstraint* ConeTwist;
            public:
                ConeTwistConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& VectorA, const Vector3& Vectorb, const Quaternion& QuaternionA, const Quaternion& QuaternionB);
                ConeTwistConstraint(ActorRigid* ActorA, const Vector3& VectorA, const Quaternion& QuaternionA);
                /// @brief Class destructor.
                /// @details The class destructor.
                virtual ~ConeTwistConstraint();

                virtual void SetAngularOnly(bool AngularOnly);
                virtual void SetLimit(int LimitIndex, Real LimitValue);
                virtual void SetLimit(Real SwingSpan1, Real SwingSpan2, Real Twistspan, Real Softness=1.0, Real BiasFactor=0.3, Real RelaxationFactor=1.0);
                virtual void SetDamping(Real Damping);
                virtual void SetMaxMotorImpulse(Real MaxMotorImpulse);
                virtual void SetMaxMotorImpulseNormalized(Real MaxMotorImpulse);
                virtual void SetFixThresh(Real FixThresh);
                virtual void SetMotorTarget(const Quaternion& Quat);
                virtual void SetMotorTargetInConstraintSpace(const Quaternion& Quat);
                virtual void EnableMotor(bool Enable);
                virtual bool IsPassedSwingLimit();

                /// @copydoc Constraint::GetConstraintBase() const
                virtual btTypedConstraint* GetConstraintBase() const;
        };//ConeTwistConstraint
    }//Physics
}//Mezzanine

#endif
