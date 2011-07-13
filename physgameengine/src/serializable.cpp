//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _serializable_cpp
#define _serializable_cpp

#include "serializable.h"
#include <memory>

namespace phys
{
    xml::Node SerializableNode::Serialize() const
    {
        xml::Node Results;
        for(vector<Serializable*>::iterator Iter = ToSerialize.begin(); Iter!=ToSerialize.end(); ++Iter )
        {
            switch((*Iter)->GetType())
            {
                case Serializable::Node:
                    Results.AppendCopy((static_cast<SerializableNode*>(*Iter))->Serialize());
                    break;
                case Serializable::Attribute:
                    Results.AppendCopy((static_cast<SerializableAttribute*>(*Iter))->Serialize());
                    break;
                default:
                    throw phys::Exception("Could not properly deserialize item of unknown type.");
            }
        }
        return Results;
    }

    String SerializableNode::GetValue() const
    {
        GatherRequirements();
        stringstream Results;
        xml::Node ProtoSerialized = Serialize(); //Actually get a DOM of the current state of this and all object that this requires for Serialization

        for (xml::Node Iter = ProtoSerialized.GetFirstChild(); Iter; Iter = Iter.GetNextSibling())
            { Results << Iter; }            // This will Serialize this nodes children, but not this node itself.
        return Results.str();
    }

}


#ifdef PHYSXML
std::ostream& operator << (std::ostream& Stream, const phys::Serializable& Outy)
    { return Outy.Serialize(Stream); }


void operator >> (const phys::xml::Node& OneNode, phys::SerializableNode& Iny)
    { Iny.DeSerialize(OneNode); }

std::istream& operator >> (std::istream& Stream, phys::SerializableNode& Iny)
{
    phys::String OneTag( phys::xml::GetOneTag(Stream) );
    std::auto_ptr<phys::xml::Document> Doc( phys::xml::PreParseClassFromSingleTag("phys::", Iny.GetName(), OneTag) );

    Doc->GetFirstChild() >> Iny;

    return Stream;
}


#endif // \PHYSXML

#endif
