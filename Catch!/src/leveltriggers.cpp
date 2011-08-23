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
        Motor->EnableMotor(true,1500.0,4500.0);
        return true;
    }else{
        Motor->EnableMotor(false,0.0,0.0);
        Wind->SetFieldStrength(0);
        return false;
    }
}

void BNS_Fan::ApplyTrigger()
{
    Real RotationSpeed = Fan->GetPhysicsSettings()->GetAngularVelocity().Y;
    // We want the max force applied to be 1000.0.  The rotation speed is on a scale from 0 to 12.
    // So we have to multiply the rotation by ~85 to get the proper force we want to apply for the motor.
    Real FieldStrength = (-RotationSpeed) * 12.0;
    Wind->SetFieldStrength(FieldStrength);

    /*std::stringstream stream;
    stream << "Fan rotation is: " << RotationSpeed << "." << endl;
    stream << "Wind Force is: " << FieldStrength << "." <<endl;
    World::GetWorldPointer()->LogString(stream.str());
    World::GetWorldPointer()->DoMainLoopLogging();// */
}

#endif
