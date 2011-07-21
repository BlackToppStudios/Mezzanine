#ifndef _leveltriggers_cpp
#define _leveltriggers_cpp

#include "leveltriggers.h"

BNS_Fan::BNS_Fan(const String& name)
    : WorldTrigger(name)
{
}

BNS_Fan::~BNS_Fan()
{
}

bool BNS_Fan::ConditionsAreMet()
{
    return false;
}

void BNS_Fan::ApplyTrigger()
{

}

#endif
