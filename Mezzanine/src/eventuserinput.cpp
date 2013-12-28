// © Copyright 2010 - 2013 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
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
#ifndef _EVENTUSERINPUT_CPP
#define _EVENTUSERINPUT_CPP
///////////////////////////////////////////////////////////////////////////////
// This will expose all keyboard and mouse, joystick and other userinput events
// to developers, we are using the SDL keymap to get us started, large portions
// are directly copy/pasta'd, so we included their license too
///////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////
#include "eventuserinput.h"
//#include "eventbase.h"
#include "exception.h"
#include "stringtool.h"

#include <memory>
#include <vector>

#include "SDL.h"

using namespace std;


namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // EventUserInput
    ///////////////////////////////////////
    EventUserInput::EventUserInput()
        {}

    EventUserInput::EventUserInput(const Input::MetaCode& Code_)
        { this->push_back(Code_); }

    EventUserInput::EventUserInput(const vector<Input::MetaCode>& Code_)
        { AddCodes(Code_); }

    EventUserInput::~EventUserInput()
        {}

    const Input::MetaCode& EventUserInput::GetMetaCode(const unsigned int& Index)
        { return this->at(Index); }

    size_t EventUserInput::GetMetaCodeCount()
        { return this->size(); }

    Input::MetaCode EventUserInput::AddCode(const Input::MetaCode& Code_)
    {
        this->push_back(Code_);
        return Code_;
    }

    Input::MetaCode EventUserInput::AddCode(const RawEvent& RawEvent_)
    {
        Input::MetaCode CurrentMetaCode( RawEvent_ );
        return this->AddCode(CurrentMetaCode);
    }

    Input::MetaCode EventUserInput::AddCode(const int& MetaValue_, const Input::InputCode& Code_)
    {
        Input::MetaCode CurrentMetaCode( MetaValue_, Code_ );
        return this->AddCode(CurrentMetaCode);
    }

    Input::MetaCode EventUserInput::AddCode(const int& MetaValue_, const Input::InputCode& Code_, const UInt16& DeviceIndex_)
    {
        Input::MetaCode CurrentMetaCode( MetaValue_, Code_, DeviceIndex_ );
        return this->AddCode(CurrentMetaCode);
    }

    void EventUserInput::AddCodes(const vector<Input::MetaCode>& Codes)
    {
        for(unsigned int c=0; Codes.size()>c ; c++)
            { this->push_back(Codes.at(c)); }
    }

    void EventUserInput::EraseCode(const Input::MetaCode& Code_)
    {
        vector<Input::MetaCode>::iterator iter;

        for(iter=this->begin(); this->end()!=iter ; iter++)
        {
            if(*iter == Code_)
            {
                this->erase(iter);
            }
        }
    }

    void EventUserInput::EraseCode(const unsigned int& Index)
        { this->erase(this->begin()+Index); }

    EventBase::EventType EventUserInput::GetType() const
        { return UserInput; }

    vector<Input::MetaCode> EventUserInput::AddCodesFromRawEvent(const RawEvent& RawEvent_)
    {
        vector<Input::MetaCode> Results;
        switch(RawEvent_.type)
        {
            case SDL_KEYUP:         case SDL_KEYDOWN:
            case SDL_MOUSEBUTTONUP: case SDL_MOUSEBUTTONDOWN:
            case SDL_JOYBUTTONDOWN: case SDL_JOYBUTTONUP:
                Results.push_back(this->AddCode(RawEvent_));
                break;

            case SDL_MOUSEMOTION:{       // ©an contain Multiple Metacodes
                std::vector<Input::MetaCode> Transport(this->AddCodesFromSDLMouseMotion(RawEvent_));
                Results.insert(Results.end(), Transport.begin(),Transport.end());
                break;}

            case SDL_JOYAXISMOTION: {       // ©an contain Multiple Metacodes
                std::vector<Input::MetaCode> Transport(this->AddCodesFromSDLJoyStickMotion(RawEvent_));
                Results.insert(Results.end(), Transport.begin(),Transport.end());
                break;}

            case SDL_JOYBALLMOTION:{
                std::vector<Input::MetaCode> Transport(this->AddCodeFromSDLJoyStickBall(RawEvent_));
                Results.insert(Results.end(), Transport.begin(),Transport.end());
                break;}

            case SDL_JOYHATMOTION:{
                std::vector<Input::MetaCode> Transport(this->AddCodeFromSDLJoyStickHat(RawEvent_));
                Results.insert(Results.end(), Transport.begin(),Transport.end());
                break;}

            default:
                MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Unknown SDL Event Inserted");
                break;
        }

        return Results;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // EventUserInput Private Methods
    ///////////////////////////////////////

    vector<Input::MetaCode> EventUserInput::AddCodesFromSDLMouseMotion(const RawEvent& RawEvent_)
    {
        vector<Input::MetaCode> Results;

        Results.push_back(this->AddCode(RawEvent_.motion.x, Input::MOUSEABSOLUTEHORIZONTAL));
        Results.push_back(this->AddCode(RawEvent_.motion.y, Input::MOUSEABSOLUTEVERTICAL));

        if(0 != RawEvent_.motion.xrel)
            { Results.push_back(this->AddCode(RawEvent_.motion.xrel, Input::MOUSEHORIZONTAL));}

        if(0 != RawEvent_.motion.yrel)
            { Results.push_back(this->AddCode(RawEvent_.motion.yrel, Input::MOUSEVERTICAL));}
        return Results;
    }

    vector<Input::MetaCode> EventUserInput::AddCodesFromSDLJoyStickMotion(const RawEvent& RawEvent_)
    {
        vector<Input::MetaCode> Results;

        Results.push_back(this->AddCode(RawEvent_.jaxis.value, Input::MetaCode::GetControllerAxisCode(RawEvent_.jaxis.axis+1), RawEvent_.jaxis.which));

        return Results;
    }

    vector<Input::MetaCode> EventUserInput::AddCodeFromSDLJoyStickHat(const RawEvent& RawEvent_)
    {
        vector<Input::MetaCode> Results;
        Input::InputCode Hat = (Input::InputCode)(RawEvent_.jhat.hat + Input::CONTROLLERHAT_FIRST);

        if( Input::CONTROLLERHAT_FIRST > Hat || Input::CONTROLLERHAT_LAST < Hat )
        {
            MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"Unsupported Controller Hat Event");
        }

        Results.push_back(this->AddCode( RawEvent_.jhat.value, Hat, RawEvent_.jhat.which ));
        return Results;
    }

    vector<Input::MetaCode> EventUserInput::AddCodeFromSDLJoyStickBall(const RawEvent& RawEvent_)
    {
        vector<Input::MetaCode> Results;

        if( 0 == RawEvent_.jball.ball )
        {
            if( RawEvent_.jball.yrel != 0 )
                { Results.push_back(this->AddCode(RawEvent_.jball.yrel, Input::CONTROLLERBALL_1_VERTICAL, RawEvent_.jball.which)); }
            if( RawEvent_.jball.xrel != 0 )
                { Results.push_back(this->AddCode(RawEvent_.jball.xrel, Input::CONTROLLERBALL_1_HORIZONTAL, RawEvent_.jball.which)); }
        }else if( 1 == RawEvent_.jball.ball ){
            if( RawEvent_.jball.yrel != 0 )
                { Results.push_back(this->AddCode(RawEvent_.jball.yrel, Input::CONTROLLERBALL_2_VERTICAL, RawEvent_.jball.which)); }
            if( RawEvent_.jball.xrel != 0 )
                { Results.push_back(this->AddCode(RawEvent_.jball.xrel, Input::CONTROLLERBALL_2_HORIZONTAL, RawEvent_.jball.which)); }
        }else{
            MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"More then 2 trackballs is currently not supported.  Perhaps we should expand our enum.");
        }

        return Results;
    }

} // /Mezz

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
std::ostream& operator << (std::ostream& stream, const Mezzanine::EventUserInput& Ev)
{
    stream << "<EventUserInput Version=\"1\">";
    for (vector<Mezzanine::Input::MetaCode>::const_iterator Iter = Ev.begin(); Iter!=Ev.end(); ++Iter)
    {
        stream << *Iter;
    }
    stream << "</EventUserInput>";

    return stream;
}

std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::EventUserInput& Ev)
{
    Mezzanine::String OneTag( Mezzanine::XML::GetOneTag(stream) );
    Mezzanine::CountedPtr<Mezzanine::XML::Document> Doc( Mezzanine::XML::PreParseClassFromSingleTag("Mezzanine::", "EventUserInput", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}

void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::EventUserInput& Ev)
{
    if ( Mezzanine::String(OneNode.Name())==Mezzanine::String("EventUserInput") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {            Ev.clear();

            //Ev.Impulse=OneNode.GetAttribute("Impulse").AsReal();
            Mezzanine::XML::Node Child = OneNode.GetFirstChild();
            Mezzanine::Input::MetaCode ACode;
            while(Child)
            {
                Child >> ACode;
                Ev.AddCode(ACode);
                Child = Child.GetNextSibling();
            }

        }else{
            MEZZ_EXCEPTION(Mezzanine::Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for EventUserInput: Not Version 1.");
        }
    }else{
        MEZZ_EXCEPTION(Mezzanine::Exception::II_IDENTITY_INVALID_EXCEPTION,"Attempting to deserialize a EventUserInput, found a " + Mezzanine::String(OneNode.Name()));
    }
}


#endif
