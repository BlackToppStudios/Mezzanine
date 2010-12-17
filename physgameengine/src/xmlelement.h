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

#include "crossplatformexport.h"
#include "xmlattribute.h"
#include "xmlnode.h"

namespace ticpp
{
    class Element;
    class Attribute;
}

namespace phys
{
    namespace xml
    {
        /// @class Element
        /// @brief This represents an xml element.
        /// @file xmlelement.h
        /// @details This wraps ticpp::Element to help make the interface cleaner and easier to use
        class PHYS_LIB Element : public Node
        {
            protected:
                /// @brief Construct an Element using meta data from a TiCPP pointer
                /// @param Meta A pointer to a ticpp::Element that this class will wrap.
                /// @param FirstTimeUsed Set this to true if you are instantiating this for the first time, false if Meta is used in another phys::xml::someclass
                Element(ticpp::Element* Meta, bool FirstTimeUsed = false);

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
             	Element(const std::string& Value);

             	/// @brief Construct an Element with Value and Text.
                /// @param Value What the Attribute is called for example in " \<div style="14"\>Content\</div\> " the word "div" is the Element Value
                /// @param Text What the Attribute is called for example in " \<div style="14"\>Content\</div\> " the word "Content" is the attribute Text
             	Element (const std::string &Value, const std::string &Text);

                /// @brief Deconstructor
                virtual ~Element();

                 /// @brief This identifies what kind of child of xml::base this is
                /// @return This returns Base::isElement
                virtual Base::XMLComponentType GetType() const;

                friend std::istream& operator>> (std::istream &In, xml::Element &Elem);

                friend std::ostream& operator<< (std::ostream &Out, xml::Element &Elem);

                /// @brief Get the First Child Attribute
                /// @return A pointer to a phys::xml::Attribute
                Attribute* FirstAttribute() const;

                /// @brief Get the Last Child Attribute
                /// @return A pointer to a phys::xml::Attribute
                Attribute* LastAttribute() const;

                /// @brief Sets an attribute
                /// @param Name a String that is the identifies the Attribute
                /// @param Value The String Value that will be assigned to the Attribute
                void SetAttribute(const String &Name, const String& Value);

                /// @brief Sets an attribute
                /// @param Name a String that is the identifies the Attribute
                /// @param Value The Whole number Value that will be assigned to the Attribute
                void SetAttribute(const String &Name, const Whole& Value);

                /// @brief Sets an attribute
                /// @param Name a String that is the identifies the Attribute
                /// @param Value The Real number Value that will be assigned to the Attribute
                void SetAttribute(const String &Name, const Real& Value);

                /// @brief Gets the Text ofthe Element
                /// @return Returns a String
                String GetText() const;

                /// @brief Set the Text of this Element
                /// @param Value A String to assign to the Text of this Element
                void SetText(const String& Value);

                /// @brief Set the Text of this Element
                /// @param Value A Whole number to assign to the Text of this Element
                void SetText(const Whole& Value);

                /// @brief Set the Text of this Element
                /// @param Value A Real number to assign to the Text of this Element
                void SetText(const Real& Value);
/*
            void 	GetTextOrDefault (T *value, const DefaultT &defaultValue) const
            Gets the text value of an Element, if it doesn't exist it will return the defaultValue.
            template<class T>
            void 	GetText (T *value, bool throwIfNotFound=true) const
            Gets the text of an Element.
            std::string 	GetTextOrDefault (const std::string &defaultValue) const
            Gets the text of an Element, if it doesn't exist it will return the defaultValue.
            Gets an attribute of name from an element, if it doesn't exist it will return the defaultValue.
            template<class T>
            T 	GetAttribute (const std::string &name, bool throwIfNotFound=true) const
                Returns an attribute of name from an element.
            template<class T>
            void 	GetAttribute (const std::string &name, T *value, bool throwIfNotFound=true) const
                Gets an attribute of name from an element.
*/

            /// @brief Gets an attribute of name from an element.
            /// @param Name The name of the Attribute you want to retrieve
            /// @return This needs to be documented more accurately
            String GetAttribute (const String& Name) const;

            /// @brief Returns true, if attribute exists.
            /// @param Name a String which is the Attribute To check
            /// @return A bool which is true if the attibute exists
            bool HasAttribute (const String& Name) const;

            /// @brief Removes attribute from element.
            /// @param Name a String which is the Attribute To remove
            void RemoveAttribute (const String& Name);

        }; // \Element

        /// @brief Stream XML data into a phys::xml hierearchy
        /// @param In This is the stream the data comes from when you use the >> operator
        /// @param Elem The xml::Element being streamed to
        /// @return This returns a stream containing all the data that was streamed in, to allow for chaining >> calls
        /// @details Inherited phys::xml::Node
        std::istream& operator>> (std::istream &In, xml::Element &Elem);

        /// @brief Stream XML data out of a phys::xml hierearchy
        /// @param Out This is the stream the data goes to from when you use the << operator
        /// @param Elem The xml::Element being streamed from
        /// @return This returns a stream containing all the data that was streamed out, to allow for chaining << calls and retrieval of the data
        /// @details Inherited phys::xml::Node
        std::ostream& operator<< (std::ostream &Out, xml::Element &Elem);

    }// \xml
}// \phys

#endif
