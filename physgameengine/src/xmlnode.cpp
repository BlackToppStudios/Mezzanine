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
#include "xmlcomment.h"
#include "xmldeclaration.h"
#include "xmldocument.h"
#include "xmlelement.h"
#include "xmlnode.h"
#include "xmltext.h"
#include "xmlstylesheetreference.h"
#include "exception.h"

#define TIXML_USE_TICPP
#include <ticpp.h>

#include <iostream>

namespace phys
{
    namespace xml
    {
        Node::~Node ()
            { }

        ///////////////////////////////////////////////////////////////////////////////
        // Get/Set Value Members
        ///////////////////////////////////////
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

        ///////////////////////////////////////////////////////////////////////////////
        // DOM Tree adjusting
        ///////////////////////////////////////

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

        ///////////////////////////////////////////////////////////////////////////////
        // DOM Tree Navigation
        ///////////////////////////////////////

        Document* Node::GetDocument () const
            { return Document::GetPointerFromWrapped( static_cast<ticpp::Document*>( static_cast<ticpp::Node*> (this->Wrapped)->GetDocument(true))); }

        bool Node::NoChildren () const
            { return (static_cast<ticpp::Node*> (this->Wrapped)->NoChildren()); }

        Node* Node::Parent() const
            { return GetPointerFromWrapped( static_cast<ticpp::Node*>(this->Wrapped)->Parent(true)); }

        Node* Node::FirstChild() const
            { return GetPointerFromWrapped(static_cast<ticpp::Node*> (this->Wrapped)->FirstChild()); }

        Node* Node::FirstChild(const std::string &Value) const
            { return this->GetPointerFromWrapped(static_cast<ticpp::Node*> (this->Wrapped)->FirstChild(Value)); }

        Node* Node::LastChild() const
            { return this->GetPointerFromWrapped(static_cast<ticpp::Node*> (this->Wrapped)->LastChild()); }

        Node* Node::LastChild(const std::string &Value) const
            { return this->GetPointerFromWrapped(static_cast<ticpp::Node*> (this->Wrapped)->LastChild(Value)); }

        Node* Node::PreviousSibling() const
            { return this->GetPointerFromWrapped(static_cast<ticpp::Node*> (this->Wrapped)->LastChild()); }

        Node* Node::PreviousSibling(const std::string &Value) const
            { return this->GetPointerFromWrapped(static_cast<ticpp::Node*> (this->Wrapped)->PreviousSibling(Value)); }

        Node* Node::NextSibling() const
            { return this->GetPointerFromWrapped(static_cast<ticpp::Node*> (this->Wrapped)->LastChild()); }

        Node* Node::NextSibling(const std::string &Value) const
            { return this->GetPointerFromWrapped(static_cast<ticpp::Node*> (this->Wrapped)->NextSibling(Value)); }



        ///////////////////////////////////////////////////////////////////////////////
        // Type verification and Conversion
        ///////////////////////////////////////
        Node* Node::GetPointerFromWrapped(ticpp::Node* Meta)
        {
            //We have no clue what Meta is, but need to wrap it with a legit phys::xml::objectofsomekind
            ticpp::Node* Results;
            /*Results=dynamic_cast<ticpp::Attribute*>(Meta);
            if (Results)
                { Results = Attribute::GetPointerFromWrapped(Results); }*/

            Results = dynamic_cast<ticpp::Comment*>(Meta);
            if (Results)
                { return Comment::GetPointerFromWrapped(static_cast<ticpp::Comment*>(Meta)); }

            Results = dynamic_cast<ticpp::Declaration*>(Meta);
            if (Results)
                { return Declaration::GetPointerFromWrapped(static_cast<ticpp::Declaration*>(Meta)); }

            Results = dynamic_cast<ticpp::Document*>(Meta);
            if (Results)
                { return Document::GetPointerFromWrapped(static_cast<ticpp::Document*>(Meta)); }

            Results = dynamic_cast<ticpp::Element*>(Meta);
            if (Results)
                { return Element::GetPointerFromWrapped(static_cast<ticpp::Element*>(Meta)); }

            Results = dynamic_cast<ticpp::StylesheetReference*>(Meta);
            if (Results)
                { return StylesheetReference::GetPointerFromWrapped(static_cast<ticpp::StylesheetReference*>(Meta)); }

            Results = dynamic_cast<ticpp::Text*>(Meta);
            if (Results)
                { return Text::GetPointerFromWrapped(static_cast<ticpp::Text*>(Meta)); }

            throw Exception("Internal Node is of unknown type, We cannot cast");
            return 0;
        }

        void Node::ThrowIfMismatchingType(XMLComponentType TypeItShouldBe, Node* NodeToCheck)
        {
            if(TypeItShouldBe == NodeToCheck->GetType())
            {
                return;
            }else{
                std::stringstream Temp;
                Temp << "The phys::xml::Node is not of type phys::xml::" << GetTypeAsString(TypeItShouldBe) << ", it is a: " << NodeToCheck->GetTypeAsString();
                throw Exception(Temp.str());
            }
        }

        Document* Node::ToDocument()
        {
            ThrowIfMismatchingType(Base::isDocument, this);
            return static_cast<Document*> (this);
        }

        Element* Node::ToElement()
        {
            ThrowIfMismatchingType(Base::isElement, this);
            return static_cast<Element*> (this);
        }

        Comment* Node::ToComment()
        {
            ThrowIfMismatchingType(Base::isComment, this);
            return static_cast<Comment*> (this);
        }

        Text* Node::ToText()
        {
            ThrowIfMismatchingType(Base::isText, this);
            return static_cast<Text*> (this);
        }

        Declaration* Node::ToDeclaration ()
        {
            ThrowIfMismatchingType(Base::isDeclaration, this);
            return static_cast<Declaration*> (this);
        }

        StylesheetReference* Node::ToStylesheetReference ()
        {
            ThrowIfMismatchingType(Base::isStylesheetReference, this);
            return static_cast<StylesheetReference*> (this);
        }
    } // \xml
}//\phys

#endif
