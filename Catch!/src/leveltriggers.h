#ifndef _leveltriggers_h
#define _leveltriggers_h

#include <physgame.h>

using namespace phys;

class BNS_Fan : public WorldTrigger
{
    public:
        BNS_Fan(const String& name);
        virtual ~BNS_Fan();
        virtual bool ConditionsAreMet();
        virtual void ApplyTrigger();
};

#endif
