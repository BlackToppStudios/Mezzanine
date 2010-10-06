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

#ifndef _xmlattribute_h
#define _xmlattribute_h

#include "xmlbase.h"

namespace ticpp
{
    class Attribute;
}

namespace phys
{
    namespace xml
    {
        /// @class Attribute
        /// @brief This represents the attributes that could exist on an XML element
        /// @file xmlattribute.h
        /// @details This wraps ticpp::Attribute tighlty to help make the interface cleaner and easier to use
        class Attribute : public Base
        {
            private:
                /// @internal
                /// @brief Tracks whether or not we are responsible for deleted the Wrapped pointer.
                /// @details Since this is a simple wrapper around a ticpp object it is possible multiple instances of this class wrap one ticpp::attribute.
                /// If this class created it, then this bit will equal true and it will be deleted in the destructor. However, if it was made with the copy
                /// constructor or somehow handed a pointer to the wrapped object, then this assumes no responsibility for deleting it.
                bool ResponsibleForDelete;

            protected:
                /// @brief Construct an attribute using meta data from a TiCPP pointer
                /// @param Meta A pointer to a ticpp::Attribute that this class will wrap.
                /// @param FirstTimeWrapped Set this to true if you are instantiating this for the first time, false if Meta is used in and phys::xml::base
                Attribute (ticpp::Attribute* Meta, bool FirstTimeUsed = false);

            public:

                /// @brief Construct an empty attribute.
                Attribute ();

                /// @brief Tears down an attribute and removes the node from the DOM
                virtual ~Attribute ();

                /// @brief Construct an attribute with name and value.
                /// @param name What the Attribute is called for example in " \<div style="14"\>Content\</div\> " the word "style" is the attribute name
                /// @param value What the Attribute is called for example in " \<div style="14"\>Content\</div\> " the word "14" is the attribute value
                Attribute (const String &name, const String &value);

                /// @brief Copy Constructor
                /// @param CopiedAttribute The attribute we are copying.
                Attribute (const Attribute &CopiedAttribute);

                /// @brief Get the value of this attribute.
                /// @return this returns a String containing the value of this attribute.
                String GetValueAsString() const;

                /// @brief Get the value of this attribute converted to a Whole Number.
                /// @return this returns a Whole containing the value of this attribute.
                Whole GetValueAsWhole() const;

                /// @brief Get the value of this attribute converted to a Real Number.
                /// @return this returns a Real containing the value of this attribute.
                Real GetValueAsReal() const;

                /// @brief Set the value of this node.
                /// @param value A Whole number to be inserted into the XML document
                void SetValue(const Whole &value);
                /// @brief Set the value of this node.
                /// @param value A Real number to be inserted into the XML document
                void SetValue(const Real &value);
                /// @brief Set the value of this node.
                /// @param value A String number to be inserted into the XML document
                void SetValue(const String &value);

                /// @brief Set the Name of this attribute.
                /// @param name A String that is the name the attribute will have.
                void SetName(const String &Name);

                /// @brief Get the Name of this attribute.
                /// @return This returns a String containing the name
                String GetName() const;

                /// @brief Get the next sibling attribute in the DOM.
                /// @warning This will cause memory leaks if the created pointer is not deleted
                /// @param throwIfNo Attribute Should this function throw an exception if it fails, or silently fail. Defaults to true so it throws errors on failure.
                /// @return Returns a pointer to an XML::Attribute that comes after this one, but still on the same element
                Attribute* Next(bool throwIfNoAttribute=true) const;

                /// @brief Get the Previous sibling attribute in the DOM.
                /// @warning This will cause memory leaks if the created pointer is not deleted
                /// @param throwIfNo Attribute Should this function throw an exception if it fails, or silently fail. Defaults to true so it throws errors on failure.
                /// @return Returns a pointer to an XML::Attribute that comes before this one, but still on the same element.
                Attribute* Previous (bool throwIfNoAttribute=true) const;

                /// @brief This identifies what kind of child of xml::base this is
                /// @return This returns Base::isAttribute
                virtual XMLComponentType GetType();


            /*
                //All TinyXml classes can print themselves to a filestream.
                //virtual void 	Print (FILE *file, int depth) const
            */

        };// \Attribute
    }// \xml
}// \phys

#endif
