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
#ifndef _physicsconetwistconstraint_cpp
#define _physicsconetwistconstraint_cpp

#include "conetwistconstraint.h"
#include "stringtool.h"
#include "serialization.h"

#include <btBulletDynamicsCommon.h>

namespace Mezzanine
{
    namespace Physics
    {
        /////////////////////////////////////////
        // ConeTwist Constraint Functions

        ConeTwistConstraint::ConeTwistConstraint(ActorRigid* ActorA, ActorRigid* ActorB, const Vector3& VectorA,
                                                  const Vector3& VectorB, const Quaternion& QuaternionA, const Quaternion& QuaternionB)
        {
            SetBodies(ActorA,ActorB);

            btTransform transa(QuaternionA.GetBulletQuaternion(), VectorA.GetBulletVector3());
            btTransform transb(QuaternionB.GetBulletQuaternion(), VectorB.GetBulletVector3());
            ConeTwist = new btConeTwistConstraint (*BodyA, *BodyB, transa, transb);
        }

        ConeTwistConstraint::ConeTwistConstraint(ActorRigid* ActorA, const Vector3& VectorA, const Quaternion& QuaternionA)
        {
            SetBodies(ActorA);

            btTransform transa(QuaternionA.GetBulletQuaternion(), VectorA.GetBulletVector3());
            ConeTwist = new btConeTwistConstraint (*BodyA, transa);
        }

        ConeTwistConstraint::~ConeTwistConstraint()
        {
            delete ConeTwist;
        }

        btTypedConstraint* ConeTwistConstraint::GetConstraintBase() const
                    { return this->ConeTwist; }

        void ConeTwistConstraint::SetAngularOnly(bool AngularOnly)
        {
            this->ConeTwist->setAngularOnly(AngularOnly);
        }

        void ConeTwistConstraint::SetLimit(int LimitIndex, Real LimitValue)
        {
            this->ConeTwist->setLimit(LimitIndex, LimitValue);
        }

        void ConeTwistConstraint::SetLimit(Real SwingSpan1, Real SwingSpan2, Real Twistspan,
                                           Real Softness, Real BiasFactor, Real RelaxationFactor)
        {
            this->ConeTwist->setLimit(SwingSpan1, SwingSpan2, Twistspan, Softness, BiasFactor, RelaxationFactor);
        }

        void ConeTwistConstraint::SetDamping(Real Damping)
        {
            this->ConeTwist->setDamping(Damping);
        }

        void ConeTwistConstraint::SetMaxMotorImpulse(Real MaxMotorImpulse)
        {
            this->ConeTwist->setMaxMotorImpulse(MaxMotorImpulse);
        }

        void ConeTwistConstraint::SetMaxMotorImpulseNormalized(Real MaxMotorImpulse)
        {
            this->ConeTwist->setMaxMotorImpulseNormalized(MaxMotorImpulse);
        }

        void ConeTwistConstraint::SetFixThresh(Real FixThresh)
        {
            this->ConeTwist->setFixThresh(FixThresh);
        }

        void ConeTwistConstraint::SetMotorTarget(const Quaternion& Quat)
        {
            this->ConeTwist->setMotorTarget(Quat.GetBulletQuaternion());
        }

        void ConeTwistConstraint::SetMotorTargetInConstraintSpace(const Quaternion& Quat)
        {
            this->ConeTwist->setMotorTargetInConstraintSpace(Quat.GetBulletQuaternion());
        }

        void ConeTwistConstraint::EnableMotor(bool Enable)
        {
            this->ConeTwist->enableMotor(Enable);
        }

        bool ConeTwistConstraint::IsPassedSwingLimit()
        {
            return this->ConeTwist->isPastSwingLimit();
        }
    }//Physics
}//Mezzanine

#endif
