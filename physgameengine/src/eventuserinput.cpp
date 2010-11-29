//© Copyright 2010 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef PHYSEVENTUSERINPUT_CPP
#define PHYSEVENTUSERINPUT_CPP
///////////////////////////////////////////////////////////////////////////////
// This will expose all keyboard and mouse, joystick and other userinput events
// to developers, we are using the SDL keymap to get us started, large portions
// are directly copy/pasta'd, so we included their license too
///////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////
#include "eventuserinput.h"
#include "eventbase.h"

#include <vector>

#include "SDL.h"

using namespace std;


namespace phys
{
    ///////////////////////////////////////////////////////////////////////////////
    // EventUserInput
    ///////////////////////////////////////
    EventUserInput::EventUserInput()
    {

    }

    EventUserInput::EventUserInput(const MetaCode &Code_)
    {
        Code.push_back(Code_);
    }

    EventUserInput::EventUserInput(const vector<MetaCode> &Code_)
    {
        AddCode(Code_);
    }

    EventUserInput::~EventUserInput()
    {

    }

    const MetaCode& EventUserInput::GetMetaCode(const unsigned int &Index)
    {
        return Code.at(Index);
    }

    unsigned int EventUserInput::GetMetaCodeCount()
    {
        return Code.size();
    }

    void EventUserInput::AddCode(const MetaCode &Code_)
    {
        Code.push_back(Code_);
    }

    void EventUserInput::AddCode(const RawEvent &RawEvent_)
    {
        MetaCode CurrentMetaCode( RawEvent_ );
        this->AddCode(CurrentMetaCode);
    }

    void EventUserInput::AddCode(const int &MetaValue_, const short unsigned int &ID_, const MetaCode::InputCode &Code_)
    {
        MetaCode CurrentMetaCode( MetaValue_, ID_, Code_ );
        this->AddCode(CurrentMetaCode);
    }

    void EventUserInput::AddCode(const vector<MetaCode> &Codes)
    {
        for(unsigned int c=0; Codes.size()>c ; c++)
        {
            Code.push_back(Codes.at(c));
        }
    }


    void EventUserInput::EraseCode(const MetaCode &Code_)
    {
        vector<MetaCode>::iterator iter;

        for(iter=Code.begin(); Code.end()!=iter ; iter++)
        {
            if(*iter == Code_)
            {
                Code.erase(iter);
            }
        }
    }

    void EventUserInput::EraseCode(const unsigned int &Index)
    {
        Code.erase(Code.begin()+Index);
    }

    void EventUserInput::ToggleCode(const MetaCode &Code_)
    {
        vector<MetaCode>::iterator iter;

        bool ErasedOne= false;

        for(iter=Code.begin(); Code.end()!=iter ; iter++)
        {
            if(*iter == Code_)
            {
                Code.erase(iter);
                ErasedOne=true;
            }
        }

        if(!ErasedOne)
            {Code.push_back(Code_);}
    }

    EventBase::EventType EventUserInput::GetType() const
    {
        return UserInput;
    }

    void EventUserInput::AddCodesFromRawEvent(const RawEvent &RawEvent_)
    {
        switch(RawEvent_.type)
        {
            case SDL_KEYDOWN:   //Only contains one metacode
            case SDL_KEYUP:
                this->AddCode(RawEvent_);
                break;

            case SDL_MOUSEMOTION:       //Can contain Multiple Metacodes
                this->AddCodesFromSDLMouseMotion(RawEvent_);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                this->AddCodesFromSDLMouseButton(RawEvent_);
                break;

            case SDL_JOYAXISMOTION: //Incomplete
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

    EventUserInput& EventUserInput::operator += (const EventUserInput& Add)
    {
        for(unsigned int c=0; Add.Code.size()>c ; c++)
        {
            Code.push_back(Add.Code.at(c));
        }
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // EventUserInput Private Methods
    ///////////////////////////////////////

    void EventUserInput::AddCodesFromSDLMouseMotion(const RawEvent &RawEvent_)
    {
        this->AddCode(RawEvent_.motion.x,0,MetaCode::MOUSEABSOLUTEHORIZONTAL);
        this->AddCode(RawEvent_.motion.y,0,MetaCode::MOUSEABSOLUTEVERTICAL);

        if(0 != RawEvent_.motion.xrel)
            {this->AddCode(RawEvent_.motion.xrel,0,MetaCode::MOUSEVERTICAL);}

        if(0 != RawEvent_.motion.yrel)
            {this->AddCode(RawEvent_.motion.yrel,0,MetaCode::MOUSEHORIZONTAL);}
    }

    void EventUserInput::AddCodesFromSDLMouseButton(const RawEvent &RawEvent_)
    {
        //MetaCode::MetaCode(const int &MetaValue_, const short unsigned int &ID_, const MetaCode::InputCode &Code_)
        this->AddCode(RawEvent_.button.x,0,MetaCode::MOUSEABSOLUTEHORIZONTAL);

        this->AddCode(RawEvent_.button.y,0,MetaCode::MOUSEABSOLUTEVERTICAL);

        if ( SDL_BUTTON_WHEELUP==RawEvent_.button.button)
        {
            this->AddCode(MetaCode::MOUSEWHEEL_UP,0,MetaCode::MOUSEWHEELVERTICAL);
        }else if( SDL_BUTTON_WHEELDOWN==RawEvent_.button.button ){
            this->AddCode(MetaCode::MOUSEWHEEL_DOWN,0,MetaCode::MOUSEWHEELVERTICAL);
        }else{
            if(RawEvent_.button.state==SDL_PRESSED)
            {
                this->AddCode(MetaCode::BUTTON_DOWN, RawEvent_.button.button, MetaCode::MOUSEBUTTON);
            }else{
                this->AddCode(MetaCode::BUTTON_UP, RawEvent_.button.button, MetaCode::MOUSEBUTTON);
            }
        }
    }

} // /phys


#endif
