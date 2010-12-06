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

#include "crossplatformexport.h"
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
        class PHYS_LIB Attribute : public Base
        {
            protected:
                /// @brief Construct an attribute using meta data from a TiCPP pointer
                /// @param Meta A pointer to a ticpp::Attribute that this class will wrap.
                /// @param FirstTimeUsed Set this to true if you are instantiating this for the first time, false if Meta is used in another phys::xml::someclass
                Attribute (ticpp::Attribute* Meta, bool FirstTimeUsed = false);

            public:
                /// @internal
                /// @brief This will find or create a pointer to the Attribute
                /// @param Meta A pointer to a ticpp::Attribute that returned Attribute will wrap
                /// @return A pointer to the phys::xml::Attribute that wraps Meta
                static Attribute* GetPointerFromWrapped(ticpp::Attribute* Meta);

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
                virtual String GetValueAsString() const;

                /// @brief Get the value of this attribute converted to a Whole Number.
                /// @return this returns a Whole containing the value of this attribute.
                virtual Whole GetValueAsWhole() const;

                /// @brief Get the value of this attribute converted to a Real Number.
                /// @return this returns a Real containing the value of this attribute.
                virtual Real GetValueAsReal() const;

                /// @brief Set the value of this attribute.
                /// @param value A Whole number to be inserted into the XML document
                virtual void SetValue(const Whole &value);

                /// @brief Set the value of this attribute.
                /// @param value A Real number to be inserted into the XML document
                virtual void SetValue(const Real &value);

                /// @brief Set the value of this attribute.
                /// @param value A String number to be inserted into the XML document
                virtual void SetValue(const String &value);

                /// @brief Set the Name of this attribute.
                /// @param Name A String that is the name the attribute will have.
                void SetName(const String &Name);

                /// @brief Get the Name of this attribute.
                /// @return This returns a String containing the name
                String GetName() const;

                /// @brief Get the next sibling attribute in the DOM.
                /// @return Returns a pointer to an XML::Attribute that comes after this one, but still on the same element
                Attribute* Next() const;

                /// @brief Get the Previous sibling attribute in the DOM.
                /// @return Returns a pointer to an XML::Attribute that comes before this one, but still on the same element.
                Attribute* Previous() const;

                /// @brief This identifies what kind of child of xml::base this is
                /// @return This returns Base::isAttribute
                virtual Base::XMLComponentType GetType() const;

            /*
                //All TinyXml classes can print themselves to a filestream.
                //virtual void 	Print (FILE *file, int depth) const
            */

        };// \Attribute
    }// \xml
}// \phys

#endif
