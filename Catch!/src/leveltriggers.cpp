#ifndef _leveltriggers_cpp
#define _leveltriggers_cpp

#include "leveltriggers.h"

BNS_Fan::BNS_Fan(const String& name, ActorRigid* Button, ActorRigid* Fan, FieldOfForce* Wind)
    : WorldTrigger(name)
{
    this->Button = Button;
    this->Wind = Wind;
    this->Fan = Fan;
}

BNS_Fan::~BNS_Fan()
{
}

bool BNS_Fan::ConditionsAreMet()
{
    if(-123.0 > Button->GetLocation().Y) return true;
    else return false;
}

void BNS_Fan::ApplyTrigger()
{
    Real RotationSpeed = Fan->GetPhysicsSettings()->GetAngularVelocity().Y;
    std::stringstream stream;
    stream << "Fan rotation is: " << RotationSpeed << "." << endl;
    World::GetWorldPointer()->LogString(stream.str());
    World::GetWorldPointer()->DoMainLoopLogging();
}

#endif
