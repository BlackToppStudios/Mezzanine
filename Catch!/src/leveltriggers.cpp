#ifndef _leveltriggers_cpp
#define _leveltriggers_cpp

#include "leveltriggers.h"

BNS_Fan::BNS_Fan(const String& name, HingeConstraint* Motor, ActorRigid* Button, ActorRigid* Fan, FieldOfForce* Wind)
    : WorldTrigger(name)
{
    this->Button = Button;
    this->Wind = Wind;
    this->Fan = Fan;
    this->Motor = Motor;
}

BNS_Fan::~BNS_Fan()
{
}

bool BNS_Fan::ConditionsAreMet()
{
    if(-107.5 > Button->GetLocation().Y)// -123 to -107, due to the 15 units of spring
    {
        Motor->EnableAngularMotor(true,20.0,9000.0);
        return true;
    }else{
        Motor->EnableAngularMotor(false,0.0,0.0);
        Wind->SetFieldStrength(0);
        return false;
    }
}

void BNS_Fan::ApplyTrigger()
{
    Real RotationSpeed = Fan->GetPhysicsSettings()->GetAngularVelocity().Y;
    // We want the max force applied to be ~120.0.  The rotation speed is on a scale from 0 to 12.
    // So we have to multiply the rotation by ~12 to get the proper force we want to apply for the motor.
    Real FieldStrength = (-RotationSpeed) * 12.0;
    Wind->SetFieldStrength(FieldStrength);

    /*std::stringstream stream;
    stream << "Fan rotation is: " << RotationSpeed << ".";
    stream << "Wind Force is: " << FieldStrength << ".";
    World::GetWorldPointer()->LogString(stream.str());
    World::GetWorldPointer()->DoMainLoopLogging();// */
}

Roll_Roll::Roll_Roll(const String& name, HingeConstraint* RollerMotor1, HingeConstraint* RollerMotor2, HingeConstraint* RollerMotor3)
    : WorldTrigger(name)
{
    this->RollerMotor1 = RollerMotor1;
    this->RollerMotor2 = RollerMotor2;
    this->RollerMotor3 = RollerMotor3;
}

Roll_Roll::~Roll_Roll()
{
}

void Roll_Roll::Rotate(HingeConstraint* RollerMotor)
{
    Real DeltaTime = World::GetWorldPointer()->GetFrameTime();
    // 0.0008 because we want 1 full rotation every 2.5 seconds, and 360 degrees is 2 radians.  2/2500 = 0.0008.
    Real DeltaAngle = 0.0008 * DeltaTime;
}

bool Roll_Roll::ConditionsAreMet()
{
    return true;
}

void Roll_Roll::ApplyTrigger()
{
    Rotate(RollerMotor1);
    Rotate(RollerMotor2);
    Rotate(RollerMotor3);
}

#endif
