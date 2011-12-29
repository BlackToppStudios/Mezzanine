#ifndef _leveltriggers_h
#define _leveltriggers_h

#include <mezzanine.h>

using namespace Mezzanine;

class BNS_Fan : public WorldTrigger
{
    protected:
        ActorRigid* Fan;
        ActorRigid* Button;
        FieldOfForce* Wind;
        ParticleEffect* WindClutter;
        HingeConstraint* Motor;
    public:
        BNS_Fan(const String& name, HingeConstraint* Motor, ActorRigid* Button, ActorRigid* Fan, FieldOfForce* Wind, ParticleEffect* WindClutter);
        virtual ~BNS_Fan();
        virtual bool ConditionsAreMet();
        virtual void ApplyTrigger();
};

class Roll_Roll : public WorldTrigger
{
    protected:
        std::vector<HingeConstraint*> Rollers;
        virtual void Rotate(HingeConstraint* RollerMotor);
    public:
        Roll_Roll(const String& name, std::vector<HingeConstraint*>& TheRollers);
        virtual ~Roll_Roll();
        virtual bool ConditionsAreMet();
        virtual void ApplyTrigger();
};

#endif
