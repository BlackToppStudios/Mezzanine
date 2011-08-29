#ifndef _leveltriggers_h
#define _leveltriggers_h

#include <physgame.h>

using namespace phys;

class BNS_Fan : public WorldTrigger
{
    protected:
        ActorRigid* Fan;
        ActorRigid* Button;
        FieldOfForce* Wind;
        HingeConstraint* Motor;
    public:
        BNS_Fan(const String& name, HingeConstraint* Motor, ActorRigid* Button, ActorRigid* Fan, FieldOfForce* Wind);
        virtual ~BNS_Fan();
        virtual bool ConditionsAreMet();
        virtual void ApplyTrigger();
};

class Roll_Roll : public WorldTrigger
{
    protected:
        HingeConstraint* RollerMotor1;
        HingeConstraint* RollerMotor2;
        HingeConstraint* RollerMotor3;
        virtual void Rotate(HingeConstraint* RollerMotor);
    public:
        Roll_Roll(const String& name, HingeConstraint* RollerMotor1, HingeConstraint* RollerMotor2, HingeConstraint* RollerMotor3);
        virtual ~Roll_Roll();
        virtual bool ConditionsAreMet();
        virtual void ApplyTrigger();
};

#endif
