// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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
#ifndef _xmlattributeiterator_h
#define _xmlattributeiterator_h

/// @file
/// @brief Contains the defintion of the AttributeIterator class

#include "datatypes.h"
#include "XML/node.h"



namespace Mezzanine
{
    namespace XML
    {

        // Attribute iterator (a bidirectional iterator over a collection of Attribute)
        /// @brief Attribute iterator (a bidirectional iterator over a collection of Attribute).
        /// @see This behaves similar to XML::NodeIterator
        class MEZZ_LIB AttributeIterator
        {
                friend class Node;

            private:

                /// @internal
                /// @brief The @ref XML::Attribute this points to.
                mutable Attribute TargetAttribute;

                /// @internal
                /// @brief The @ref XML::Node that is the parent of the @ref XML::Attribute pointed to by @ref TargetAttribute
                Node ParentNode;

                /// @brief Internal Constructor, almost a copy construtor
                /// @param ref Some of the internal data from another @ref AttributeIterator specifically the Attribute it points to.
                /// @param ParentNode Some of the interal data from another @ref AttributeIterator specifically the parent of the Node it points to.
                AttributeIterator(AttributeStruct* ref, NodeStruct* GetParent);

            public:

                /// @brief An Iterator trait
                typedef ptrdiff_t difference_type;
                /// @brief An Iterator trait
                typedef Attribute value_type;
                /// @brief An Iterator trait
                typedef Attribute* pointer;
                /// @brief An Iterator trait
                typedef Attribute& reference;
                /// @brief An Iterator trait
                typedef std::bidirectional_iterator_tag iterator_category;

                /// @brief Default Constructor, makes a blank iterator
                AttributeIterator();

                // Construct an iterator which points to the specified attribute
                /// @brief Construct an iterator which points to the specified node
                /// @param GetParent A Node that contains the Attribute this iterator will point to.
                /// @param attr The Attribute this iterator points to.
                AttributeIterator(const Attribute& attr, const Node& GetParent);

                /// @brief Compares this AttributeIterator to another AttributeIterator for equality
                /// @param rhs The Right Hand Side AttributeIterator
                /// @return True if the internal data stored in the Attribute this AttributeIterator refers to is the same as the metadata in the other AttributeIterator's Attribute, false otherwise.
                bool operator==(const AttributeIterator& rhs) const;

                /// @brief Compares this AttributeIterator to another AttributeIterator for inequality
                /// @param rhs The Right Hand Side AttributeIterator.
                /// @return False if the internal data stored in Node this AttributeIterator refers to is the same as the metadata in the other AttributeIterator's Attribute, True otherwise.
                bool operator!=(const AttributeIterator& rhs) const;

                /// @brief Deferences this Iterator
                /// @return a Attribute reference to the Attribute pointed at by this AttributeIterator.
                Attribute& operator*() const;

                /// @brief Get the pointer the Attribute this points to.
                /// @return A pointer to the Attribute this AttributeIterator references.
                Attribute* operator->() const;

                /// @brief Increment the iterator to the next member of the container.
                /// @return Returns a const AttributeIterator.
                const AttributeIterator& operator++();

                /// @brief Increment the iterator to the next member of the container.
                /// @return Returns a AttributeIterator.
                AttributeIterator operator++(int);

                /// @brief Decrement the iterator to the next member of the container.
                /// @return Returns a const AttributeIterator.
                const AttributeIterator& operator--();

                /// @brief Decrement the iterator to the next member of the container.
                /// @return Returns a AttributeIterator.
                AttributeIterator operator--(int);
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
