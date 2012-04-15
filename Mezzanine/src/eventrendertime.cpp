//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef EVENTRENDERTIME_CPP
#define EVENTRENDERTIME_CPP
///////////////////////////////////////////////////////////////////////////////
// This will store the amount of time  since key rendering events have occurred
///////////////////////////////////////

#include "datatypes.h"
#include "eventrendertime.h"
#include "stringtool.h"

#include <memory>

using namespace std;

namespace Mezzanine
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
#ifdef MEZZXML
std::ostream& operator << (std::ostream& stream, const Mezzanine::EventRenderTime& Ev)
{
    stream << "<EventRenderTime Version=\"1\" Rendertime=\"" << Ev.getMilliSecondsSinceLastFrame() << "\" />";
    return stream;
}

std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::EventRenderTime& Ev)
{
    Mezzanine::String OneTag( Mezzanine::xml::GetOneTag(stream) );
    std::auto_ptr<Mezzanine::xml::Document> Doc( Mezzanine::xml::PreParseClassFromSingleTag("Mezzanine::", "EventRenderTime", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}

Mezzanine::xml::Node& operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::EventRenderTime& Ev)
{
    if ( Mezzanine::String(OneNode.Name())==Mezzanine::String("EventRenderTime") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            Ev = Mezzanine::EventRenderTime( OneNode.GetAttribute("Rendertime").AsInt() );
        }else{
            throw( Mezzanine::Exception("Incompatible XML Version for EventRenderTime: Not Version 1"));
        }
    }else{
        throw( Mezzanine::Exception(Mezzanine::StringTool::StringCat("Attempting to deserialize a EventRenderTime, found a ", OneNode.Name())));
    }
}
#endif // \MEZZXML

#endif
