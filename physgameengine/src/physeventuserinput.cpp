#ifndef PHYSEVENTUSERINPUT_CPP
#define PHYSEVENTUSERINPUT_CPP
///////////////////////////////////////////////////////////////////////////////
// This will expose all keyboard and mouse, joystick and other userinput events
// to developers, we are using the SDL keymap to get us started, large portions
// are directly copy/pasta'd, so we included their license too
///////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Includes
#include "physeventuserinput.h"
#include "physevent.h"
#include <stdarg.h>
#include <vector>

#include "SDL.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// MetaCode

MetaCode::MetaCode()
{}

MetaCode::MetaCode(int MetaValue_, short unsigned int ID_, InputCode Code_)
{
    SetMetaValue(MetaValue_);
    SetID(ID_);
    SetCode(Code_);
}

MetaCode::MetaCode(RawEvent _RawEvent)
{
    //TODO: Actually process each event
    //Make a metacode for each event and return it
    switch(_RawEvent.type)
    {
        case SDL_KEYDOWN:
            break;
        case SDL_KEYUP:
            break;
        case SDL_MOUSEMOTION:
            break;
        case SDL_MOUSEBUTTONDOWN:
            break;
        case SDL_MOUSEBUTTONUP:
            break;
        case SDL_JOYAXISMOTION:
            break;
        case SDL_JOYBUTTONDOWN:
            break;
        case SDL_JOYBUTTONUP:
            break;
        case SDL_JOYBALLMOTION:
            break;
        case SDL_JOYHATMOTION:
            break;
        default:
            throw ("Unknown SDL Event Inserted");
            break;
    }
}

int MetaCode::GetMetaValue()
{
    return this->MetaValue;
}

InputCode MetaCode::GetCode()
{
    return this->Code;
}

short unsigned int MetaCode::GetID()
{
    return this->ID;
}

void MetaCode::SetMetaValue(int MetaValue_)
{
    this->MetaValue=MetaValue_;
}

void MetaCode::SetCode(InputCode Code_)
{
    this->Code=Code_;
}

void MetaCode::SetID(short unsigned int ID_)
{
    this->ID=ID_;
}

bool MetaCode::operator==(const MetaCode &other) const
{
    if(this->Code == other.Code && this->MetaValue == other.MetaValue)
    {
        return true;
    }else{
        return false;
    }
}


///////////////////////////////////////////////////////////////////////////////
// PhysEventUserInput
PhysEventUserInput::PhysEventUserInput()
{

}

PhysEventUserInput::PhysEventUserInput(MetaCode Code_)
{
    Code.push_back(Code_);
}

PhysEventUserInput::PhysEventUserInput(vector<MetaCode> Code_)
{
    for(unsigned int c=0; Code_.size()>c ; c++)
    {
        Code.push_back(Code_.at(c));
    }
}

PhysEventUserInput::~PhysEventUserInput()
{

}

MetaCode PhysEventUserInput::GetCode(unsigned int Index)
{
    return Code.at(Index);
}

unsigned int PhysEventUserInput::GetCodeCount()
{
    return Code.size();
}

void PhysEventUserInput::AddCode(MetaCode _Code)
{
    Code.push_back(_Code);
}

void PhysEventUserInput::ToggleCode(MetaCode _Code)
{
    vector<MetaCode>::iterator iter;

    for(iter=Code.begin(); Code.end()!=iter ; iter++)
    {
        if(*iter == _Code)
        {
            Code.erase(iter);
        }else{
            Code.push_back(_Code);
        }
    }
}

void PhysEventUserInput::ToggleCode(unsigned int Index)
{
    Code.erase(Code.begin()+Index);
}

EventType PhysEventUserInput::getEventType()
{
    return UserInput;
}



#endif

