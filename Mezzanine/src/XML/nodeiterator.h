// © Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _xmlnodeiterator_h
#define _xmlnodeiterator_h

/// @file
/// @brief Contains the defintion of the @ref XML::NodeIterator and @ref XML::NamedNodeIterator class.

#include "datatypes.h"
#include "node.h"
#include "swig.h"

SWIG_INFO_BEGINCLASS

namespace Mezzanine
{
    namespace XML
    {

        /// @brief Child node iterator (a bidirectional iterator over a collection of @ref Node)
        /// @details Node::begin() and Node::attributes_begin() return iterators that point to the first node/attribute, respectively; Node::end() and Node::attributes_end() return past-the-end iterator for node/attribute list, respectively - this iterator can't be dereferenced, but decrementing it results in an iterator pointing to the last element in the list (except for empty lists, where decrementing past-the-end iterator results in undefined behavior). Past-the-end iterator is commonly used as a termination value for iteration loops. If you want to get an iterator that points to an existing handle, you can construct the iterator with the handle as a single constructor argument, like so: xml_node_iterator(node). For xml_attribute_iterator, you'll have to provide both an attribute and its parent node.\n\n
        /// Node::begin() and Node::end() return equal iterators if called on null Node; such iterators can't be dereferenced. Node::attributes_begin() and Node::attributes_end() behave the same way. For correct iterator usage this means that child node/attribute collections of null nodes appear to be empty.\n\n
        /// Both types of iterators have bidirectional iterator semantics (i.e. they can be incremented and decremented, but efficient random access is not supported) and support all usual iterator operations - comparison, dereference, etc. The iterators are invalidated if the node/attribute objects they're pointing to are removed from the tree; adding nodes/attributes does not invalidate any iterators.
        class MEZZ_LIB NodeIterator
        {
                friend class Node;

            private:

                /// @brief internal
                /// @brief The current @ref Node this iterator points to.
                mutable Node TargetNode;

                /// @internal
                /// @brief The node that contains this collection of nodes. Used in error situations.
                Node ParentNode;

                /// @brief Internal Constructor, almost a copy construtor
                /// @param ref Some of the internal data from another @ref NodeIterator specifically the Node it points to.
                /// @param ParentNode Some of the interal data from another @ref NodeIterator specifically the parent of the Node it points to.
                NodeIterator(NodeStruct* ref, NodeStruct* ParentNode);

            public:
                /// @brief An Iterator trait
                typedef ptrdiff_t difference_type;

                /// @brief An Iterator trait
                typedef Node value_type;

                /// @brief An Iterator trait
                typedef Node* pointer;

                /// @brief An Iterator trait
                typedef Node& reference;

                /// @brief An Iterator trait
                typedef std::bidirectional_iterator_tag iterator_category;

                /// @brief Default Constructor, makes a blank iterator
                NodeIterator();

                /// @brief Construct an iterator which points to the specified node
                /// @param node A Node that this iterator will point to.
                NodeIterator(const Node& node);

                /// @brief Compares this NodeIterator to another NodeIterator for equality
                /// @param rhs The Right Hand Side NodeIterator
                /// @return True if the internal data stored in Node this NodeIterator refers to is the same as the metadata in the other NodeIterator's Node, false otherwise.
                bool operator==(const NodeIterator& rhs) const;

                /// @brief Compares this NodeIterator to another NodeIterator for inequality
                /// @param rhs The Right Hand Side NodeIterator.
                /// @return False if the internal data stored in Node this NodeIterator refers to is the same as the metadata in the other NodeIterator's Node, True otherwise.
                bool operator!=(const NodeIterator& rhs) const;

                /// @brief Deferences this Iterator
                /// @return a Node reference to the node pointed at by this NodeIterator.
                Node& operator*() const;

                /// @brief Get the pointer the Node this points to.
                /// @return A pointer to the Node this NodeIterator references.
                Node* operator->() const;

                /// @brief Increment the iterator to the next member of the container.
                /// @return Returns a const NodeIterator.
                const NodeIterator& operator++();

                /// @brief Increment the iterator to the next member of the container.
                /// @return Returns a NodeIterator.
                NodeIterator operator++(int);

                /// @brief Decrement the iterator to the next member of the container.
                /// @return Returns a const NodeIterator.
                const NodeIterator& operator--();

                /// @brief Decrement the iterator to the next member of the container.
                /// @return Returns a NodeIterator.
                NodeIterator operator--(int);
        };

        // Named node range helper
        /// @brief Child node iterator (a forward iterator over a collection of @ref Node) only iterates over nodes with a given name.
        /// @details Node::begin() and Node::attributes_begin() return iterators that point to the first node/attribute, respectively; Node::end() and Node::attributes_end() return past-the-end iterator for node/attribute list, respectively - this iterator can't be dereferenced, but decrementing it results in an iterator pointing to the last element in the list (except for empty lists, where decrementing past-the-end iterator results in undefined behavior). Past-the-end iterator is commonly used as a termination value for iteration loops. If you want to get an iterator that points to an existing handle, you can construct the iterator with the handle as a single constructor argument, like so: xml_node_iterator(node). For xml_attribute_iterator, you'll have to provide both an attribute and its parent node.\n\n
        /// Node::begin() and Node::end() return equal iterators if called on null Node; such iterators can't be dereferenced. Node::attributes_begin() and Node::attributes_end() behave the same way. For correct iterator usage this means that child node/attribute collections of null nodes appear to be empty.\n\n
        /// Both types of iterators have bidirectional iterator semantics (i.e. they can be incremented and decremented, but efficient random access is not supported) and support all usual iterator operations - comparison, dereference, etc. The iterators are invalidated if the node/attribute objects they're pointing to are removed from the tree; adding nodes/attributes does not invalidate any iterators.
        class MEZZ_LIB NamedNodeIterator
        {
            public:
                /// @brief An Iterator trait
                typedef ptrdiff_t difference_type;

                /// @brief An Iterator trait
                typedef Node value_type;

                /// @brief An Iterator trait
                typedef Node* pointer;

                /// @brief An Iterator trait
                typedef Node& reference;

                /// @brief An Iterator trait
                typedef std::forward_iterator_tag iterator_category;

                /// @brief Default constructor
                NamedNodeIterator();

                /// @brief Construct an iterator which points to the specified node
                NamedNodeIterator(const Node& node, const Char8* Name );

                /// @brief Compares this NamedNodeIterator to another NamedNodeIterator for equality
                /// @param rhs The Right Hand Side NamedNodeIterator.
                /// @return False if the internal data stored in Node this NamedNodeIterator refers to is the same as the metadata in the other NamedNodeIterator's Node, True otherwise.
                bool operator==(const NamedNodeIterator& rhs) const;

                /// @brief Compares this NamedNodeIterator to another NamedNodeIterator for inequality
                /// @param rhs The Right Hand Side NamedNodeIterator.
                /// @return False if the internal data stored in Node this NamedNodeIterator refers to is the same as the metadata in the other NamedNodeIterator's Node, True otherwise.
                bool operator!=(const NamedNodeIterator& rhs) const;

                /// @brief Deferences this Iterator
                /// @return a Node reference to the node pointed at by this NamedNodeIterator.
                Node& operator*() const;

                /// @brief Get the pointer the Node this points to.
                /// @return A pointer to the Node this NamedNodeIterator references.
                Node* operator->() const;

                /// @brief Increment the iterator to the next member of the container.
                /// @return Returns a const NodeIterator.
                const NamedNodeIterator& operator++();

                /// @brief Increment the iterator to the next member of the container.
                /// @return Returns a NodeIterator.
                NamedNodeIterator operator++(int);

            private:
                /// @brief The Current @ref Node being pointed to by this iterator.
                mutable Node TargetNode;

                /// @brief The Name any nodet his points to will assume.
                const Char8* TargetName;
        };

    }
}// /namespace Mezzanine

SWIG_INFO_ENDCLASS

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
