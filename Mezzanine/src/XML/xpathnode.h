// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
/*
 *
 * Software, Files, Libraries and all other items referenced in this clause refers only
 * to the contents of this file and associated documentation.
 *
 * Pugixml parser - version 1.0
 * --------------------------------------------------------
 * Copyright © 2006-2012, by Arseny Kapoulkine (arseny.kapoulkine@gmail.com)
 * Report bugs and download new versions at http://pugixml.org/
 *
 * This library is distributed under the MIT License. See notice at the end
 * of this file.
 *
 * This work is based on the pugxml parser, which is:
 * Copyright © 2003, by Kristen Wegner (kristen@tima.net)
 */
#ifndef _xmlxpathnode_h
#define _xmlxpathnode_h

/// @file
/// @brief The definition of the @ref Mezzanine::XML::XPathNode
#include "datatypes.h"
#include "XML/node.h"



namespace Mezzanine
{
    namespace XML
    {
        /// @brief An XPath node which can store handles to a XML::Node or an XML::Attribute.
        /// @details Because an XPath node can be either a XML::Node or an XML::Attribute, there is a special type, XPathNode,
        /// which is a discriminated union of these types. A value of this type contains two node handles, one of XML::Node type,
        /// and another one of XML::Attribute type; at most one of them can be non-null. The accessors to get these handles are
        /// available: XPathNode::GetNode() an XPathNode::GetAttribute() . \n \n
        /// XPath nodes can be null, in which case both accessors return null handles.
        class MEZZ_LIB XPathNode
        {
            private:
                /// @internal
                /// @brief The node this refers to.
                Node TargetNode;

                /// @internal
                /// @brief The node if any that this targets.
                Attribute _attribute;

                #ifndef SWIG
                /// @brief Used to convert this to a boolean value in a safe way
                /// @return Returns true if the internal data is set and false otherwise.
                /// @note Not available in scripting languages.
                typedef void (*unspecified_bool_type)(XPathNode***);
                #endif

            public:
                /// @brief Default constructor; constructs empty XPath node
                XPathNode();

                /// @brief Construct From a XML::Node.
                /// @param node The XML::Node this handle should reference.
                XPathNode(const Node& node);

                /// @brief Construct From a XML::Attribute.
                /// @param GetAttribute The XML::Attribute this handle should reference.
                /// @param GetParent The XML::Node the XML::Attribute is on.
                XPathNode(const Attribute& GetAttribute, const Node& GetParent);

                /// @brief Get the XML::Node this is referencing
                /// @return A valid XML::Node, or a null node if this doesn't reference a an XML::Node.
                Node GetNode() const;

                /// @brief Get the XML::Attribute this is referencing
                /// @return A valid XML::Attribute, or a null node if this doesn't reference a an XML::Attribute.
                Attribute GetAttribute() const;

                /// @brief Get the parent of the XML::Node or XML::Attribute this refers to.
                /// @return A valid XML::Node, or a null node if this doesn't reference a an XML::Node.
                Node GetParent() const;

                #ifndef SWIG
                /// @brief Used to convert this to a boolean value in a safe way
                /// @return Returns true if the internal data is set and false otherwise.
                /// @note Not available in scripting languages.
                operator unspecified_bool_type() const;
                #endif

                /// @brief Logical not operator, used a workaround for borland compiler.
                /// @return A bool that is the opposite of evaluatig this as a bool normally.
                bool operator!() const;

                // Comparison operators
                /// @brief Called when comparing two XPathNode instances for equality.
                /// @param n The other XML::XPathNode to compare this Node against for equality.
                /// @return True if this and the other XML::XPathNode are referencing the same item, false otherwise;
                bool operator==(const XPathNode& n) const;

                /// @brief Called when comparing two XPathNode instances for inequality.
                /// @param n The other XML::XPathNode to compare this Node against for inequality.
                /// @return True if this and the other XML::XPathNode are referencing the same item, false otherwise;
                bool operator!=(const XPathNode& n) const;
        };
    }
} // /namespace Mezzanine



#endif // Include guard


/*
 *
 * Software, Files, Libraries and all other items referenced in this clause refers only
 * to the contents of this file and associated documentation.
 *
 * Copyright © 2006-2012 Arseny Kapoulkine
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
