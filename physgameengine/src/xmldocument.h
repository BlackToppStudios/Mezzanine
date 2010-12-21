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

#ifndef _xmldocument_h
#define _xmldocument_h

#include <ostream>

#include "crossplatformexport.h"
#include "xmlnode.h"

namespace ticpp
{
    class Document;
}

namespace phys
{
    namespace xml
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Document
        /// @headerfile xmldocument.h
        /// @brief This is a container of nodes and elements and other XML stuff
        /// @details
        class PHYS_LIB Document: public Node
        {
            protected:
                /// @brief Construct a Document using meta data from a TiCPP pointer
                /// @param Meta A pointer to a ticpp::Document that this class will wrap.
                /// @param FirstTimeUsed Set this to true if you are instantiating this for the first time, false if Meta is used in another phys::xml::someclass
                Document(ticpp::Document* Meta, bool FirstTimeUsed = false);

            public:
                /// @internal
                /// @brief This will find or create a pointer to the Document
                /// @param Meta A pointer to a ticpp::Document that returned Attribute will wrap
                /// @return A pointer to the phys::xml::Document that wraps Meta
                static Document* GetPointerFromWrapped(ticpp::Document* Meta);

                /// @brief Descriptive contructor
                /// @param DocumentName The name of the document being created
                Document(const String& DocumentName);

                /// @brief Construct an empty Document
                Document();

                /// @brief This identifies what kind of child of xml::base this is
                /// @return This returns Base::isDocument
                virtual Base::XMLComponentType GetType() const;

                friend std::istream& operator>> (std::istream &In, xml::Document &Doc);
                friend std::ostream& operator<< (std::ostream &Out, xml::Document &Doc);
                /*
                void 	LoadFile (TiXmlEncoding encoding=TIXML_DEFAULT_ENCODING)
                    Load a file using the current document value.
                void 	SaveFile () const
                    Save a file using the current document value.
                void 	LoadFile (const std::string &filename, TiXmlEncoding encoding=TIXML_DEFAULT_ENCODING)
                    Load a file using the given filename.
                void 	LoadFile (const char *filename, TiXmlEncoding encoding=TIXML_DEFAULT_ENCODING)
                void 	SaveFile (const std::string &filename) const
                    Save a file using the given filename.
                void 	Parse (const std::string &xml, bool throwIfParseError=true, TiXmlEncoding encoding=TIXML_DEFAULT_ENCODING)
                    Parse the given xml data.
                */

        }; // /Document

        /// @brief Stream XML data into a phys::xml hierearchy
        /// @param In This is the stream the data comes from when you use the >> operator
        /// @param Doc The xml::Document being streamed to
        /// @return This returns a stream containing all the data that was streamed in, to allow for chaining >> calls
        /// @details Inherited phys::xml::Node
        std::istream& operator>> (std::istream &In, xml::Document &Doc);

        /// @brief Stream XML data out of a phys::xml hierearchy
        /// @param Out This is the stream the data goes to from when you use the << operator
        /// @param Doc The xml::Document being streamed from
        /// @return This returns a stream containing all the data that was streamed out, to allow for chaining << calls and retrieval of the data
        /// @details Inherited phys::xml::Node
        std::ostream& operator<< (std::ostream &Out, xml::Document &Doc);

        ///@brief a function that exists only to test and aid debugging, will be removed soon.
        void TestXML(std::istream* Ins);

    }// /xml
}// /phys



#endif
