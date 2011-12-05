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
        Motor->EnableMotor(true,20.0,9000.0);
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

Roll_Roll::Roll_Roll(const String& name, std::vector<HingeConstraint*>& TheRollers)
    : WorldTrigger(name)
{
    Rollers.swap(TheRollers);
    for( Whole X = 0 ; X < Rollers.size() ; ++X )
        Rollers[X]->GetActorA()->GetPhysicsSettings()->SetActivationState(Mezzanine::WOAS_DisableDeactivation);
}

Roll_Roll::~Roll_Roll()
{
}

void Roll_Roll::Rotate(HingeConstraint* RollerMotor)
{
    Real DeltaTime = World::GetWorldPointer()->GetFrameTime();
    Real DeltaAngle = 1.25 * DeltaTime;
    Real CurrentAngle = RollerMotor->GetHingeAngle();
    Real ActualAngle = DeltaAngle + ( CurrentAngle>0 ? CurrentAngle : -CurrentAngle );
    RollerMotor->SetMotorTarget(-ActualAngle,DeltaTime);
}

bool Roll_Roll::ConditionsAreMet()
{
    return true;
}

void Roll_Roll::ApplyTrigger()
{
    for( Whole X = 0 ; X < Rollers.size() ; ++X )
        Rotate(Rollers[X]);
}

#endif
