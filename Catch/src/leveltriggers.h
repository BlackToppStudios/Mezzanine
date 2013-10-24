#ifndef _leveltriggers_h
#define _leveltriggers_h

#include <mezzanine.h>

using namespace Mezzanine;

class BNS_Fan : public WorldTrigger
{
protected:
    RigidDebris* Fan;
    RigidDebris* Button;
    FieldOfForce* Wind;
    Graphics::ParticleSystemProxy* WindClutter;
    Physics::HingeConstraint* Motor;
public:
    BNS_Fan(const String& name, Physics::HingeConstraint* Motor, RigidDebris* Button, RigidDebris* Fan, FieldOfForce* Wind, Graphics::ParticleSystemProxy* WindClutter);
    virtual ~BNS_Fan();
    virtual bool ConditionsAreMet();
    virtual void ApplyTrigger();
};//BNS_Fan

class Roll_Roll : public WorldTrigger
{
protected:
    std::vector<Physics::HingeConstraint*> Rollers;
    virtual void Rotate(Physics::HingeConstraint* RollerMotor);
public:
    Roll_Roll(const String& name, std::vector<Physics::HingeConstraint*>& TheRollers);
    virtual ~Roll_Roll();
    virtual bool ConditionsAreMet();
    virtual void ApplyTrigger();
};//Roll_Roll

#endif
