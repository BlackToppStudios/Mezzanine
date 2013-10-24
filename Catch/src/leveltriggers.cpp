#ifndef _leveltriggers_cpp
#define _leveltriggers_cpp

#include "leveltriggers.h"

BNS_Fan::BNS_Fan(const String& name, Physics::HingeConstraint* Motor, RigidDebris* Button, RigidDebris* Fan, FieldOfForce* Wind, Graphics::ParticleSystemProxy* WindClutter) :
    WorldTrigger(name)
{
    this->Button = Button;
    this->Wind = Wind;
    this->Fan = Fan;
    this->Motor = Motor;
    this->WindClutter = WindClutter;
}

BNS_Fan::~BNS_Fan()
{
}

bool BNS_Fan::ConditionsAreMet()
{
    if(-107.5 > this->Button->GetLocation().Y)// -123 to -107, due to the 15 units of spring
    {
        this->Motor->EnableMotor(true,20.0,9000.0);
        return true;
    }else{
        this->Motor->EnableMotor(false,0.0,0.0);
        this->WindClutter->RemoveFromWorld();
        this->Wind->SetFieldStrength(0);
        return false;
    }
}

void BNS_Fan::ApplyTrigger()
{
    Real RotationSpeed = this->Fan->GetRigidProxy()->GetAngularVelocity().Y;
    // We want the max force applied to be ~120.0.  The rotation speed is on a scale from 0 to 12.
    // So we have to multiply the rotation by ~12 to get the proper force we want to apply for the motor.
    Real FieldStrength = (-RotationSpeed) * 12.0;
    this->Wind->SetFieldStrength(FieldStrength);
    if(FieldStrength > 60.0) this->WindClutter->AddToWorld();
}

Roll_Roll::Roll_Roll(const String& name, std::vector<Physics::HingeConstraint*>& TheRollers) :
    WorldTrigger(name)
{
    this->Rollers.swap(TheRollers);
    for( Whole X = 0 ; X < Rollers.size() ; ++X )
        this->Rollers[X]->GetProxyA()->SetActivationState(Mezzanine::Physics::AS_DisableDeactivation);
}

Roll_Roll::~Roll_Roll()
{
}

void Roll_Roll::Rotate(Physics::HingeConstraint* RollerMotor)
{
    Real DeltaTime = Entresol::GetSingletonPtr()->GetLastFrameTimeMilliseconds();
    Real DeltaAngle = 1.25 * DeltaTime;
    Real CurrentAngle = RollerMotor->GetHingeAngle();
    Real ActualAngle = DeltaAngle + ( CurrentAngle > 0 ? CurrentAngle : -CurrentAngle );
    RollerMotor->SetMotorTarget(-ActualAngle,DeltaTime);
}

bool Roll_Roll::ConditionsAreMet()
{
    return true;
}

void Roll_Roll::ApplyTrigger()
{
    for( Whole X = 0 ; X < Rollers.size() ; ++X )
        this->Rotate(Rollers[X]);
}

#endif
