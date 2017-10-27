#ifndef _leveltriggers_h
#define _leveltriggers_h

#include <mezzanine.h>

using namespace Mezzanine;

class BNS_Fan : public WorldTrigger
{
protected:
    RigidDebris* TrigFan;
    RigidDebris* TrigButton;
    FieldOfForce* TrigWind;
    Graphics::ParticleSystemProxy* TrigWindClutter;
    Physics::HingeConstraint* TrigMotor;
    Physics::SixDofSpringConstraint* TrigSpring;
public:
    BNS_Fan(const String& name, Physics::HingeConstraint* Motor, Physics::SixDofSpringConstraint* Spring, RigidDebris* Fan, FieldOfForce* Wind, Graphics::ParticleSystemProxy* WindClutter);
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
