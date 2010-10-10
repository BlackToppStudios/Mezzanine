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

#ifndef _xmlnode_cpp
#define _xmlnode_cpp

#include "xmlattribute.h"
#include "xmlnode.h"

#define TIXML_USE_TICPP
#include <ticpp.h>

namespace phys
{
    namespace xml
    {
            Node::~Node ()
                { }

            String Node::GetValueAsString() const
            {
                return static_cast<ticpp::Node*> (this->Wrapped)->Value();
            }

            Whole Node::GetValueAsWhole() const
            {
                Whole Temp=0;
                static_cast<ticpp::Node*> (this->Wrapped)->GetValue(&Temp);
                return Temp;
            }

            Real Node::GetValueAsReal() const
            {
                Real Temp=0;
                static_cast<ticpp::Node*> (this->Wrapped)->GetValue(&Temp);
                return Temp;
            }

            void Node::SetValue (const Whole &value)
                { static_cast<ticpp::Node*> (this->Wrapped)->SetValue(value); }

            void Node::SetValue (const Real &value)
                { static_cast<ticpp::Node*> (this->Wrapped)->SetValue(value); }

            void Node::SetValue (const String &value)
                { static_cast<ticpp::Node*> (this->Wrapped)->SetValue(value); }

            /* Attribute::XMLComponentType Node::GetType()
                { return Base::isNode; } */

            void Node::Clear()
            {
                /// @todo TODO actually Code this
                //for ( TravelingPointer=This->FirstChild; TravelingPointer!=this->Lastchild; TravelingPointer=TravelingPointer->NextSibling)
                //{
                //  TravelingPointer->Clear;
                //  delete TravelingPointer;
                //}
            }

            void Node::AppendChild (Node &AddThis)
            {
                static_cast<ticpp::Node*> (this->Wrapped)->LinkEndChild( static_cast<ticpp::Node*>(AddThis.Wrapped) );
                AddThis.TakeOwnerOfWrapped();
            }

            void Node::InsertAfterChild (Node *AfterThis, Node &AddThis)
            {
                static_cast<ticpp::Node*> (this->Wrapped)->InsertAfterChild( static_cast<ticpp::Node*>(AfterThis->Wrapped), *(static_cast<ticpp::Node*>(AddThis.Wrapped)) );
                AddThis.TakeOwnerOfWrapped();
            }

            void Node::InsertBeforeChild (Node *BeforeThis, Node &AddThis)
            {
                static_cast<ticpp::Node*> (this->Wrapped)->InsertBeforeChild( static_cast<ticpp::Node*>(BeforeThis->Wrapped), *(static_cast<ticpp::Node*>(AddThis.Wrapped)) );
                AddThis.TakeOwnerOfWrapped();
            }




    } // \xml
}//\phys

#endif
