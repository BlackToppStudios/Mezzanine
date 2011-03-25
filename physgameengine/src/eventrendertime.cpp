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
#ifndef EVENTRENDERTIME_CPP
#define EVENTRENDERTIME_CPP
///////////////////////////////////////////////////////////////////////////////
// This will store the amount of time  since key rendering events have occurred
///////////////////////////////////////

#include "datatypes.h"
#include "eventrendertime.h"

#include <memory>

using namespace std;

namespace phys
{
    EventRenderTime::EventRenderTime (Whole Milliseconds)
    {
        Rendertime=Milliseconds;
    }

    EventBase::EventType EventRenderTime::GetType() const
    {
        return EventBase::RenderTime;
    }

    Whole EventRenderTime::getMilliSecondsSinceLastFrame() const
    {
        return Rendertime;
    }

    void EventRenderTime::operator= (const EventRenderTime& rhs)
    {
        this->Rendertime=rhs.Rendertime;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef PHYSXML
std::ostream& operator << (std::ostream& stream, const phys::EventRenderTime& Ev)
{
    stream << "<EventRenderTime Version=\"1\" Rendertime=\"" << Ev.getMilliSecondsSinceLastFrame() << "\" />";
    return stream;
}

std::istream& PHYS_LIB operator >> (std::istream& stream, phys::EventRenderTime& Ev)
{
    phys::String OneTag( phys::xml::GetOneTag(stream) );
    std::auto_ptr<phys::xml::Document> Doc( phys::xml::PreParseClassFromSingleTag("phys::", "EventRenderTime", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}

phys::xml::Node& operator >> (const phys::xml::Node& OneNode, phys::EventRenderTime& Ev)
{
    if ( phys::String(OneNode.Name())==phys::String("EventRenderTime") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            Ev = phys::EventRenderTime( OneNode.GetAttribute("Rendertime").AsInt() );
        }else{
            throw( phys::Exception("Incompatible XML Version for EventRenderTime: Not Version 1"));
        }
    }else{
        throw( phys::Exception(phys::StringCat("Attempting to deserialize a EventRenderTime, found a ", OneNode.Name())));
    }
}
#endif // \PHYSXML

#endif
