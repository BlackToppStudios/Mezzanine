#ifndef PHYSEVENTUSERINPUT_CPP
#define PHYSEVENTUSERINPUT_CPP
///////////////////////////////////////////////////////////////////////////////
// This will expose all keyboard and mouse, joystick and other userinput events
// to developers, we are using the SDL keymap to get us started, large portions
// are directly copy/pasta'd, so we included their license too
///////////////////////////////////////

#include "physeventuserinput.h"
#include "physevent.h"
#include <stdarg.h>
#include <vector>

using namespace std;

PhysEventUserInput::PhysEventUserInput()
{

};

PhysEventUserInput::PhysEventUserInput(MetaCode Code_)
{
    Code.push_back(Code_);
};

PhysEventUserInput::PhysEventUserInput(vector<MetaCode> Code_)
{
    for(int c=0; Code_.size()>c ; c++)
    {
        Code.push_back(Code_.at(c));
    }
};

MetaCode PhysEventUserInput::GetCode(unsigned int Index)
{
        return Code.at(Index);
};

unsigned int PhysEventUserInput::GetCodeCount()
{
        return Code.size();
};

void PhysEventUserInput::ToggleCode(MetaCode _Code)
{
        //TODO seach the code list and see if the given meta code is on it, then either remove it or add it
};

void PhysEventUserInput::ToggleCode(unsigned int Index)
{
        //todo REmove the Nth index from the list
};

EventType PhysEventUserInput::getEventType()
{
        return UserInput;
};

#endif

