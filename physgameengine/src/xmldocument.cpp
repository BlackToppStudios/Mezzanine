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

#ifndef _xmldocument_cpp
#define _xmldocument_cpp

#include "xmldocument.h"
#include "exception.h"


#define TIXML_USE_TICPP
#include <ticpp.h>

namespace phys
{
    namespace xml
    {
        Document::Document()
        {
            this->Wrapped=new ticpp::Document();
            this->TakeOwnerOfWrapped();
        }

        Document::Document (ticpp::Document* Meta, bool FirstTimeUsed)
        {
            this->Wrapped = Meta;
            if (FirstTimeUsed)
                { this->TakeOwnerOfWrapped(); }
        }

        Document::Document(const String& DocumentName)
        {
            this->Wrapped=new ticpp::Document(DocumentName);
            this->TakeOwnerOfWrapped();
        }

        Document* Document::GetPointerFromWrapped(ticpp::Document* Meta)
        {
            Document* Other;
            try {
                //Most likely cause of failure is ticpp::Node::GetBasePointer() returns 0
                Other = static_cast<Document*>( Meta->GetBasePointer()->GetUserData() );
            } catch (ticpp::Exception e) {
                std::stringstream temp;
                temp << "Could not Create phys::xml::Document from invalid pointer." << std::endl << e.what() << std::endl << "Details: " << e.m_details;
                throw Exception(temp.str());
            }

            //If there is no pointer inside TinyXML to our node, then it doesn't exist, so make it Otherwise use what is there
            if(0 == Other)
                { Other = new Document(Meta, true); }
            return Other;
        }

        Base::XMLComponentType Document::GetType() const
        {
            return Base::isDocument;
        }

        std::istream& operator>> (std::istream &In, xml::Document &Doc)
            { return In >> *(static_cast <ticpp::Document*>(Doc.Wrapped)); }

        std::ostream& operator<< (std::ostream &Out, xml::Document &Doc)
            { return Out << *(static_cast <ticpp::Document*>(Doc.Wrapped)); }

    }// /xml
}// /phys

#endif
