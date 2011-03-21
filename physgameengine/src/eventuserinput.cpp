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
#include "world.h"

#include <memory>
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
        this->push_back(Code_);
    }

    EventUserInput::EventUserInput(const vector<MetaCode> &Code_)
    {
        AddCodes(Code_);
    }

    EventUserInput::~EventUserInput()
    {

    }

    const MetaCode& EventUserInput::GetMetaCode(const unsigned int &Index)
    {
        return this->at(Index);
    }

    size_t EventUserInput::GetMetaCodeCount()
    {
        return this->size();
    }

    MetaCode EventUserInput::AddCode(const MetaCode &Code_)
    {
        this->push_back(Code_);
        return Code_;
    }

    MetaCode EventUserInput::AddCode(const RawEvent &RawEvent_)
    {
        MetaCode CurrentMetaCode( RawEvent_ );
        return this->AddCode(CurrentMetaCode);
    }

    MetaCode EventUserInput::AddCode(const int &MetaValue_, const MetaCode::InputCode &Code_)
    {
        MetaCode CurrentMetaCode( MetaValue_, Code_ );
        return this->AddCode(CurrentMetaCode);
    }

    void EventUserInput::AddCodes(const vector<MetaCode> &Codes)
    {
        for(unsigned int c=0; Codes.size()>c ; c++)
            { this->push_back(Codes.at(c)); }
    }

    void EventUserInput::EraseCode(const MetaCode &Code_)
    {
        vector<MetaCode>::iterator iter;

        for(iter=this->begin(); this->end()!=iter ; iter++)
        {
            if(*iter == Code_)
            {
                this->erase(iter);
            }
        }
    }

    void EventUserInput::EraseCode(const unsigned int &Index)
    {
        this->erase(this->begin()+Index);
    }

    EventBase::EventType EventUserInput::GetType() const
    {
        return UserInput;
    }

    vector<MetaCode> EventUserInput::AddCodesFromRawEvent(const RawEvent &RawEvent_)
    {
        vector<MetaCode> Results;
        switch(RawEvent_.type)
        {
            case SDL_KEYDOWN:   //Only contains one metacode
            case SDL_KEYUP:
                Results.push_back(this->AddCode(RawEvent_));
                break;

            case SDL_MOUSEMOTION:{       //Can contain Multiple Metacodes
                vector<MetaCode> Transport(this->AddCodesFromSDLMouseMotion(RawEvent_));
                Results.insert(Results.end(), Transport.begin(),Transport.end());
                break;}

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:{
                //vector<MetaCode> Transport(this->AddCodesFromSDLMouseButton(RawEvent_));
                //Results.insert(Results.end(), Transport.begin(),Transport.end());
                Results.push_back(this->AddCodeFromSDLMouseButton(RawEvent_));
                break;}

            case SDL_JOYAXISMOTION: {       //Can contain Multiple Metacodes
                vector<MetaCode> Transport(this->AddCodesFromSDLJoyStickMotion(RawEvent_));
                Results.insert(Results.end(), Transport.begin(),Transport.end());
                break;}

                break;
            case SDL_JOYBUTTONDOWN: case SDL_JOYBUTTONUP:{
                Results.push_back(this->AddCodeFromSDLJoyStickButton(RawEvent_));
                break;}

            case SDL_JOYBALLMOTION:

                break;
            case SDL_JOYHATMOTION:

                break;
            default:
                throw ("Unknown SDL Event Inserted");
                break;
        }

        return Results;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // EventUserInput Private Methods
    ///////////////////////////////////////

    vector<MetaCode> EventUserInput::AddCodesFromSDLMouseMotion(const RawEvent &RawEvent_)
    {
        vector<MetaCode> Results;

        Results.push_back(this->AddCode(RawEvent_.motion.x, MetaCode::MOUSEABSOLUTEHORIZONTAL));
        Results.push_back(this->AddCode(RawEvent_.motion.y, MetaCode::MOUSEABSOLUTEVERTICAL));

        if(0 != RawEvent_.motion.xrel)
            { Results.push_back(this->AddCode(RawEvent_.motion.xrel, MetaCode::MOUSEHORIZONTAL));}

        if(0 != RawEvent_.motion.yrel)
            { Results.push_back(this->AddCode(RawEvent_.motion.yrel, MetaCode::MOUSEVERTICAL));}
        return Results;
    }

    vector<MetaCode> EventUserInput::AddCodesFromSDLJoyStickMotion(const RawEvent &RawEvent_)
    {
        vector<MetaCode> Results;

        Results.push_back(this->AddCode(RawEvent_.jaxis.value, MetaCode::GetJoystickAxisCode(RawEvent_.jaxis.axis+1)));

        return Results;
    }

    MetaCode EventUserInput::AddCodeFromSDLMouseButton(const RawEvent &RawEvent_)
    {
        if(RawEvent_.button.state==SDL_PRESSED)
        {
            return this->AddCode(MetaCode::BUTTON_DOWN, MetaCode::GetMouseButtonCode(RawEvent_.button.button));
        }else{
            return this->AddCode(MetaCode::BUTTON_UP, MetaCode::GetMouseButtonCode(RawEvent_.button.button));
        }
    }

    MetaCode EventUserInput::AddCodeFromSDLJoyStickButton(const RawEvent &RawEvent_)
    {
        if(RawEvent_.jbutton.state==SDL_PRESSED)
        {
            return this->AddCode(MetaCode::BUTTON_DOWN, MetaCode::GetJoystickButtonCode(RawEvent_.jbutton.button));
        }else{
            return this->AddCode(MetaCode::BUTTON_UP, MetaCode::GetJoystickButtonCode(RawEvent_.jbutton.button));
        }
    }

} // /phys

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef PHYSXML
std::ostream& operator << (std::ostream& stream, const phys::EventUserInput& Ev)
{
    stream << "<EventUserInput Version=\"1\">";
    for (vector<phys::MetaCode>::const_iterator Iter = Ev.begin(); Iter!=Ev.end(); ++Iter)
    {
        stream << *Iter;
    }
    stream << "</EventUserInput>";

    return stream;
}

std::istream& PHYS_LIB operator >> (std::istream& stream, phys::EventUserInput& Ev)
{
    phys::String OneTag( phys::xml::GetOneTag(stream) );
    std::auto_ptr<phys::xml::Document> Doc( phys::xml::PreParseClassFromSingleTag("phys::", "EventUserInput", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}

void operator >> (const phys::xml::Node& OneNode, phys::EventUserInput& Ev)
{
    if(OneNode.GetAttribute("Version").AsInt() == 1)
    {
        Ev.clear();

        //Ev.Impulse=OneNode.GetAttribute("Impulse").AsReal();
        phys::xml::Node Child = OneNode.GetFirstChild();
        phys::MetaCode ACode;
        while(Child)
        {
            Child >> ACode;
            Ev.AddCode(ACode);
            Child = Child.GetNextSibling();
        }

    }else{
        throw( phys::Exception("Incompatible XML Version for EventUserInput: Not Version 1"));
    }
}
#endif // \PHYSXML


#endif
