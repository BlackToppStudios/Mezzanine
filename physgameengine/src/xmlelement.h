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

#ifndef _xmlelement_h
#define _xmlelement_h

#include "xmlnode.h"

namespace ticpp
{
    class Element;
}

namespace phys
{
    namespace xml
    {
        /// @class Element
        /// @brief This represents an xml element.
        /// @file xmlelemeny.h
        /// @details This wraps ticpp::Element to help make the interface cleaner and easier to use
        class Element : public Node
        {
            protected:
                /// @brief Construct an Element using meta data from a TiCPP pointer
                /// @param Meta A pointer to a ticpp::Element that this class will wrap.
                /// @param FirstTimeWrapped Set this to true if you are instantiating this for the first time, false if Meta is used in another phys::xml::someclass
                Element (ticpp::Element* Meta, bool FirstTimeUsed = false);

            public:
                /// @internal
                /// @brief This will find or create a pointer to the Element
                /// @param Meta A pointer to a ticpp::Element that returned Element will wrap
                /// @return A pointer to the phys::xml::Element that wraps Meta
                static Element* GetPointerFromWrapped(ticpp::Element* Meta);

                /// @brief Construct an empty attribute.
             	Element();

                /// @brief Construct an Element with value.
                /// @param Value What the Attribute is called for example in " \<div style="14"\>Content\</div\> " the word "div" is the Element Value
             	Element (const std::string &Value);

                /// @brief Construct an Element with Value and Text.
                /// @param Value What the Attribute is called for example in " \<div style="14"\>Content\</div\> " the word "div" is the Element Value
                /// @param Text What the Attribute is called for example in " \<div style="14"\>Content\</div\> " the word "Content" is the attribute Text
             	Element (const std::string &Value, const std::string &Text);

                /// @brief Deconstructor
                virtual ~Element();

                /// @brief This identifies what kind of child of xml::base this is
                /// @return This returns Base::isElement
                virtual Base::XMLComponentType GetType() const;

                /// @brief Stream XML data into a phys::xml hierearchy
                /// @param In This is the stream the data comes from when you use the >> operator
                /// @return This returns a stream containing all the data that was streamed in, to allow for chaining >> calls
                /// @details Inherited phys::xml::Node
                virtual std::istream& operator>> (std::istream &In);

                /// @brief Stream XML data out of a phys::xml hierearchy
                /// @param Out This is the stream the data goes to from when you use the << operator
                /// @return This returns a stream containing all the data that was streamed out, to allow for chaining << calls and retrieval of the data
                /// @details Inherited phys::xml::Node
                virtual std::ostream& operator<< (std::ostream &Out);

/*
 	Element ()
 	Default Constructor.
 	Element (const std::string &value)
 	Default Constructor.
 	Element (const char *value)
 	Default Constructor.
 	Element (TiXmlElement *element)
 	Constructor.
template<class T>
 	Element (const std::string &value, const T &text)
 	Constructor that allows you to set the element text.

Attribute * 	FirstAttribute (bool throwIfNoAttributes=true) const
 	Access the first attribute in this element.
Attribute * 	LastAttribute (bool throwIfNoAttributes=true) const
 	Access the last attribute in this element.
template<class T>
void 	SetAttribute (const std::string &name, const T &value)
 	Sets an attribute of name to a given value.
std::string 	GetText (bool throwIfNotFound=true) const
 	Gets the text of an Element.
std::string 	GetTextOrDefault (const std::string &defaultValue) const
 	Gets the text of an Element, if it doesn't exist it will return the defaultValue.
template<class T, class DefaultT>
void 	GetTextOrDefault (T *value, const DefaultT &defaultValue) const
 	Gets the text value of an Element, if it doesn't exist it will return the defaultValue.
template<class T>
void 	GetText (T *value, bool throwIfNotFound=true) const
 	Gets the text of an Element.
template<class T>
void 	SetText (const T &value)
 	Convenience function to set the text of an element.
template<class T, class DefaulT>
void 	GetAttributeOrDefault (const std::string &name, T *value, const DefaulT &defaultValue) const
 	Gets an attribute of name from an element, if it doesn't exist it will return the defaultValue.
std::string 	GetAttributeOrDefault (const std::string &name, const std::string &defaultValue) const
 	Gets an attribute of name from an element, if it doesn't exist it will return the defaultValue.
template<class T>
T 	GetAttribute (const std::string &name, bool throwIfNotFound=true) const
 	Returns an attribute of name from an element.
template<class T>
void 	GetAttribute (const std::string &name, T *value, bool throwIfNotFound=true) const
 	Gets an attribute of name from an element.
std::string 	GetAttribute (const std::string &name) const
 	Gets an attribute of name from an element.
bool 	HasAttribute (const std::string &name) const
 	Returns true, if attribute exists.
void 	RemoveAttribute (const std::string &name)
 	Removes attribute from element.


*/


        }; // \Element
    }// \xml
}// \phys

#endif // \_xmlelement_h
