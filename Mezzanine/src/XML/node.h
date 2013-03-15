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

#include "datatypes.h"
#include "XML/xmlenumerations.h"
#include "XML/attribute.h"
#include "XML/objectrange.h"

#ifndef SWIG
#include <cstddef>
#endif

#ifndef _xmlnode_h
#define _xmlnode_h
namespace Mezzanine
{
    namespace XML
    {
        class NodeStruct;
        class Text;
        class TreeWalker;
        class XPathNode;
        class XPathNodeSet;
        class XPathVariableSet;
        class XPathQuery;
        class Writer;
        class NodeIterator;
        class AttributeIterator;
        class NamedNode_iterator;



        // A light-weight handle for manipulating nodes in DOM tree
        class MEZZ_LIB Node
        {
                friend class AttributeIterator;
                friend class NodeIterator;
                friend class NamedNode_iterator;

            protected:
                NodeStruct* _GetRoot;

                typedef void (*unspecified_bool_type)(Node***);

            public:
                // Default constructor. Constructs an empty node.
                Node();

                /// @brief Constructs node from internal pointer
                /// @param p An internal node pointer.
                explicit Node(NodeStruct* p);

                /// @brief Used to convert this to a boolean value in a safe way
                /// @return Returns true if the internal data is set and false otherwise.
                operator unspecified_bool_type() const;

                // Borland C++ workaround
                bool operator!() const;

                // Comparison operators (compares wrapped node pointers)
                bool operator==(const Node& r) const;
                bool operator!=(const Node& r) const;
                bool operator<(const Node& r) const;
                bool operator>(const Node& r) const;
                bool operator<=(const Node& r) const;
                bool operator>=(const Node& r) const;

                // Check if node is empty.
                bool Empty() const;

                // Get node type
                NodeType Type() const;

                // Get node Name/Value, or "" if node is empty or it has no Name/Value
                const Char8* Name() const;
                const Char8* Value() const;

                // Get GetAttribute list
                Attribute GetFirstAttribute() const;
                Attribute GetLastAttribute() const;

                // Get GetChildren list
                Node GetFirstChild() const;
                Node GetLastChild() const;

                // Get next/previous sibling in the GetChildren list of the GetParent node
                Node GetNextSibling() const;
                Node GetPreviousSibling() const;

                // Get GetParent node
                Node GetParent() const;

                // Get GetRoot of DOM tree this node belongs to
                Node GetRoot() const;

                // Get text object for the current node
                Text text() const;

                // Get GetChild, GetAttribute or next/previous sibling with the specified name
                /// @brief Attempt to get a child Node with a given name.
                /// @param Name The name of the desired child Node.
                /// @return A Node that represents the first desired child, or an empty Node on failure.
                Node GetChild(const Char8* Name) const;
                /// @brief Attempt to get a child Node with a given name.
                /// @param Name The name of the desired child Node.
                /// @return A Node that represents the first desired child, or an empty Node on failure.
                Node GetChild(const String& Name) const
                    { return GetChild(Name.c_str()); }

                Attribute GetAttribute(const Char8* Name) const;
                Node GetNextSibling(const Char8* Name) const;
                Node GetPreviousSibling(const Char8* Name) const;

                // Get GetChild Value of current node; that is, Value of the first GetChild node of Type PCDATA/CDATA
                const Char8* ChildValue() const;

                // Get GetChild Value of GetChild with specified Name. Equivalent to GetChild(Name).ChildValue().
                const Char8* ChildValue(const Char8* Name) const;

                // Set node Name/Value (returns false if node is empty, there is not enough memory, or node can not have Name/Value)
                /// @brief Set the name of .
                /// @param rhs The desired name.
                /// @return True if successful, returns false if the name cannot be stored or there is not enough memory.
                bool SetName(const Char8* rhs);
                /// @brief Set the name of this object
                /// @param rhs The desired name .
                /// @return True if successful, returns false if the name cannot be stored or there is not enough memory.
                bool SetName(const String& rhs)
                    { return SetName(rhs.c_str()); }
                /// @brief Set the value of this.
                /// @param rhs The new Value.
                /// @return True if successful, returns false if this is empty or there is not enough memory.
                /// @todo update this to make the error return code redundant and use an exception instead.
                /// @todo Review for possiblity of buffer overflow.
                bool SetValue(const Char8* rhs);

                // Add GetAttribute with specified Name. Returns added GetAttribute, or empty GetAttribute on errors.

                /// @brief Creates an Attribute and puts it at the end of this Nodes attributes.
                /// @param Name The name of the New attribute to be created
                /// @details This attempts to create an Attribute and stick it at the end of the list of attribute on the current
                /// Node. This will fail and return an Empty Attribute if this Node is neither an Element nor a Declaration. This will
                /// fail and return an empty attribute if this Node is empty.
                /// @return The created Attribute or an empty Attribute on Failure.
                Attribute AppendAttribute(const Char8* Name);
                /// @brief Creates an Attribute and puts it at the end of this Nodes attributes.
                /// @param Name The name of the New attribute to be created
                /// @return The created Attribute or an empty Attribute on Failure.
                Attribute AppendAttribute(const String& Name)
                    { return AppendAttribute(Name.c_str()); }
                Attribute PrependAttribute(const Char8* Name);
                Attribute InsertAttributeAfter(const Char8* Name, const Attribute& attr);
                Attribute InsertAttributeBefore(const Char8* Name, const Attribute& attr);

                // Add a copy of the specified GetAttribute. Returns added GetAttribute, or empty GetAttribute on errors.

                /// @brief Copies an Attribute and puts the copy at the end of this Nodes attributes.
                /// @param proto The attribute to be copied.
                /// @details This attempts to create a copy of an attribute Attribute and stick it at the end of the list of attribute on the current
                /// Node. This will fail and return an Empty Attribute if this Node is neither an Element nor a Declaration. This will
                /// fail and return an empty attribute if this Node is empty.
                /// @return The created Attribute or an empty Attribute on Failure.
                Attribute AppendCopy(const Attribute& proto);

                /// @brief Copies an Attribute and puts the copy at the beginning of this Nodes attributes.
                /// @param proto The attribute to be copied.
                /// @details This attempts to create a copy of an attribute Attribute and stick it at the beginning of the list of attribute on the current
                /// Node. This will fail and return an Empty Attribute if this Node is neither an Element nor a Declaration. This will
                /// fail and return an empty attribute if this Node is empty.
                /// @return The created Attribute or an empty Attribute on Failure.
                Attribute PrependCopy(const Attribute& proto);

                /// @brief Copies an Attribute and puts the copy into the list of this Nodes attributes.
                /// @param proto The attribute to be copied.
                /// @param attr An Attribute that represents an Attribute on this Node, and is just before where you want the new copy of proto.
                /// @details This attempts to create a copy of an attribute Attribute and stick it in the middle of the list of attributes, just after a selected attribute, on the current
                /// Node. This will fail and return an Empty Attribute if this Node is neither an Element nor a Declaration. This will
                /// fail and return an empty attribute if this Node is empty.
                /// @return The created Attribute or an empty Attribute on Failure.
                Attribute InsertCopyAfter(const Attribute& proto, const Attribute& attr);
                Attribute InsertCopyBefore(const Attribute& proto, const Attribute& attr);

                // Add GetChild node with specified Type. Returns added node, or empty node on errors.
                /// @brief Creates a Node and makes it a child of this one.
                /// @param Type The NodeType of the Node to be added to list of child Nodes.
                /// @return A Node representing the freshly added Node, or an empty Node if there was an error.
                Node AppendChild(NodeType Type = NodeElement);
                Node PrependChild(NodeType Type = NodeElement);
                Node InsertChildAfter(NodeType Type, const Node& node);
                /// @brief Creates a Node and makes it a child of this one, and puts at the middle of the Child Nodes.
                /// @param Type The NodeType of the Node to be added, just before another specific node.
                /// @param node The specific node to add the new one before.
                /// @return A Node representing the freshly added Node, or an empty Node if there was an error.
                /// @todo Not all nodes can be added to other nodes, we need to figure it out and put it here.
                Node InsertChildBefore(NodeType Type, const Node& node);

                // Add GetChild element with specified Name. Returns added node, or empty node on errors.
                /// @brief Creates an element Node as a child of this Node, with the given name.
                /// @param Name The name of the Node to be created.
                /// @details Calls @ref Node::AppendChild(NodeType); using NodeElement as the NodeType.
                /// @return The desired Node on success, an empty Node on failure.
                Node AppendChild(const Char8* Name);
                /// @brief Creates an element Node as a child of this Node, with the given name.
                /// @param Name The name of the Node to be created.
                /// @details Calls @ref Node::AppendChild(const char_t*)
                /// @return The desired Node on success, an empty Node on failure.
                Node AppendChild(const String& Name)
                    { return AppendChild(Name.c_str()); }
                /// @brief Creates an element Node as a child of this Node, with the given name at the beginning of the children
                /// @param Name The name of the Node to be created.
                /// @details Calls @ref Node::PrependChild(NodeType); using NodeElement as the NodeType.
                /// @return The desired Node on success, an empty Node on failure.
                Node PrependChild(const Char8* Name);
                /// @brief Creates an element Node as a child of this Node, with the given name at the beginning of the children
                /// @param Name The name of the Node to be created.
                /// @details Calls @ref Node::PrependChild(const char_t*);
                /// @return The desired Node on success, an empty Node on failure.
                Node PrependChild(const String& Name)
                    { return PrependChild(Name.c_str()); }

                /// @brief Creates an element Node as a child of this Node, with the given name at the middle of the children
                /// @param Name The name of the Node to be created.
                /// @param node The node just before were the Create node is to be placed.
                /// @details Calls Node::InsertChildAfter(NodeType, Node); using NodeElement as the NodeType.
                /// @return The desired Node on success, an empty Node on failure.
                Node InsertChildAfter(const Char8* Name, const Node& node);
                /// @brief Creates an element Node as a child of this Node, with the given name at the middle of the children
                /// @param Name The name of the Node to be created.
                /// @param node The node just after were the Create node is to be placed.
                /// @details Calls Node::InsertChildBefore(NodeType, Node); using NodeElement as the NodeType.
                /// @return The desired Node on success, an empty Node on failure.
                Node InsertChildBefore(const Char8* Name, const Node& node);

                /// @brief Copies a Node and puts the copy at the end of the list of this Nodes Childrem.
                /// @param proto The Node to be copied. If this is emptry, no work is performed.
                /// @return The copied Node on success, an empty Node on failure.
                Node AppendCopy(const Node& proto);

                /// @brief Copies a Node and puts the copy at the start of the list of this Nodes Childrem.
                /// @param proto The Node to be copied. If this is emptry, no work is performed.
                /// @return The copied Node on success, an empty Node on failure.
                Node PrependCopy(const Node& proto);

                /// @brief Copies a Node and puts the copy in the middle the list of this Nodes Childrem.
                /// @param proto The Node to be copied. If this is emptry, no work is performed.
                /// @param node The Node just before the desired place in the list of children to insert the copied node.
                /// @return The copied Node on success, an empty Node on failure.
                Node InsertCopyAfter(const Node& proto, const Node& node);
                /// @brief Copies a Node and puts the copy in the middle the list of this Nodes Childrem.
                /// @param proto The Node to be copied. If this is emptry, no work is performed.
                /// @param node The Node just after the desired place in the list of children to insert the copied node.
                /// @return The copied Node on success, an empty Node on failure.
                Node InsertCopyBefore(const Node& proto, const Node& node);

                // Remove specified attribute
                bool RemoveAttribute(const Attribute& a);
                bool RemoveAttribute(const Char8* Name);

                // Remove specified GetChild
                bool RemoveChild(const Node& n);

                /// @brief Remove child element as specified by name.
                /// @param Name The name of the Node to remove.
                /// @return True if the removal was successful, false otherwise
                bool RemoveChild(const Char8* Name);

                /// @brief Search for an Attribute using a function to check each Attribute individually.
                /// @param pred a pointer to a function that accepts an Attribute, and returns bool.
                /// @details This iterates through each Attribute on this node, from begining to end and calls the Predicate function passing
                /// an Attribute to it. If the Predicate returns true the Node it was just passed is returned. If Precdicate never returns
                /// true, it is called on every Node and a blank Node is returned. The Predicate is never called with a null value.
                /// @return This returns the first Attribute that causes Predicate to return true.
                template <typename Predicate> Attribute FindAttribute(Predicate pred) const
                {
                    if (!_GetRoot) return Attribute();

                    for (Attribute attrib = GetFirstAttribute(); attrib; attrib = attrib.GetNextAttribute())
                        if (pred(attrib))
                            return attrib;

                    return Attribute();
                }


                /// @brief Search for an child ( only direct children ) Node using a function to check each Node individually.
                /// @param pred a pointer to a function that accepts an Node, and returns bool.
                /// @details This iterates through all immediate children of this Node and calls the Predicate function passing a Node to it. If
                /// the Predicate returns true the Node it was just passed is returned. If Predicate never returns true, it is called
                /// on every Node and a blank Node is returned. The Predicate is never called with a null value.
                /// @return This returns the first Node that causes Predicate to return true.
                template <typename Predicate> Node FindChild(Predicate pred) const
                {
                    if (!_GetRoot) return Node();

                    for (Node node = GetFirstChild(); node; node = node.GetNextSibling())
                        if (pred(node))
                            return node;

                    return Node();
                }

                /// @brief Search for any Node descended from this Node using a function to check each Node individually.
                /// @param pred a pointer to a function that accepts an Node, and returns bool.
                /// @details This iterates through all children of this Node, and their children ( and so on), and calls the Predicate function
                /// passing each Node to it. This iterates through all Nodes using a depth first algorithm. If the Predicate returns true the
                /// Node it was just passed is returned. If Predicate never returns true, it is called on every Node and a blank Node is
                /// returned. The Predicate is never called with a null value.
                /// @return This returns the first Node that causes Predicate to return true.
                template <typename Predicate> Node FindNode(Predicate pred) const
                {
                    if (!_GetRoot) return Node();

                    Node cur = GetFirstChild();

                    while (cur._GetRoot && cur._GetRoot != _GetRoot)
                    {
                        if (pred(cur)) return cur;

                        if (cur.GetFirstChild()) cur = cur.GetFirstChild();
                        else if (cur.GetNextSibling()) cur = cur.GetNextSibling();
                        else
                        {
                            while (!cur.GetNextSibling() && cur._GetRoot != _GetRoot) cur = cur.GetParent();

                            if (cur._GetRoot != _GetRoot) cur = cur.GetNextSibling();
                        }
                    }

                    return Node();
                }

                // Find GetChild node by GetAttribute Name/Value
                Node FindChildbyAttribute(const Char8* Name, const Char8* AttrName, const Char8* AttrValue) const;
                /// @brief Find a Node by an Attribute it has.
                /// @param AttrName The name of the matching Attribute.
                /// @param AttrValue The value of the matching Attribute.
                /// @details Any Null pointers instead of character arrays passed in will cause undefined behavior. All Matching is Case sensitive.
                /// @return The First matching XML::Node
                Node FindChildbyAttribute(const Char8* AttrName, const Char8* AttrValue) const;

                // Get the absolute node Path from GetRoot as a text string.
                String Path(Char8 delimiter = '/') const;

                // Search for a node by Path consisting of node names and . or .. elements.
                Node FirstElementByPath(const Char8* Path, Char8 delimiter = '/') const;

                // Recursively Traverse subtree with TreeWalker
                bool Traverse(TreeWalker& walker);

                // Select single node by evaluating XPath query. Returns first node from the Resulting node set.
                XPathNode FindSingleNode(const Char8* query, XPathVariableSet* variables = 0) const;
                XPathNode FindSingleNode(const XPathQuery& query) const;

                // Select node set by evaluating XPath query

                /// @brief Select a group of nodes by evaluating an XPath query.
                /// @param query The XPath query as a c-string to be evaluated.
                /// @param variables undocumented.
                /// @return An XPathNodeSet with the Matchs of the XPath query.
                XPathNodeSet FindNodes(const Char8* query, XPathVariableSet* variables = 0) const;

                /// @brief Select a group of nodes by evaluating an XPath query.
                /// @param query The XPath query XPathQuery class instance.
                /// @return An XPathNodeSet with the Matchs of the XPath query.
                /// @param query The XPath query XPathQuery class instance.
                XPathNodeSet FindNodes(const XPathQuery& query) const;


                // Print subtree using a WriterInstance object

                /// @brief Output the XML document using a Writer.
                /// @param WriterInstance The Writer that will be used to output the xml text.
                /// @param indent The Character(s) used to represent a tab in the output, this defaults to one tab character.
                /// @param flags The output format flags, this is a bitfield that defaults to XML::FormatDefault.
                /// @param DocumentEncoding The XML::Encoding of the document, whichs defaults to EncodingAuto
                /// @param Depth This defaults to 0. The amount of times to prepend the indentation to the beginning of each output line.
                /// @details This will never write a Byte Order Mark(BOM), and will default to not outputing a document declaration.
                void Print(Writer& WriterInstance, const Char8* indent = "\t", unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto, unsigned int Depth = 0) const;

                // Print subtree to stream

                /// @brief Output the XML document using a Output Stream.
                /// @param os An output stream to send xml text to.
                /// @param indent The Character(s) used to represent a tab in the outpput, this defaults to one tab character.
                /// @param flags The output format flags, this is a bitfield that defaults to XML::FormatDefault
                /// @param DocumentEncoding The XML::Encoding of the document, whichs defaults to EncodingAuto
                /// @param Depth This defaults to 0. The amount of times to prepend the indentation to the beginning of each output line.
                /// @details This will never write a Byte Order Mark(BOM), and will default to not outputing a document declaration.
                void Print(std::basic_ostream<char, std::char_traits<char> >& os, const Char8* indent = "\t", unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto, unsigned int Depth = 0) const;

                /// @brief Output the XML document using a Output Stream.
                /// @param os An output stream to send xml text to.
                /// @param indent The Character(s) used to represent a tab in the outpput, this defaults to one tab character.
                /// @param flags The output format flags, this is a bitfield that defaults to XML::FormatDefault
                /// @param Depth This defaults to 0. The amount of times to prepend the indentation to the beginning of each output line.
                /// @details This will never write a Byte Order Mark(BOM), and will default to not outputing a document declaration.
                void Print(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& os, const Char8* indent = "\t", unsigned int flags = FormatDefault, unsigned int Depth = 0) const;


                // Child nodes iterators
                typedef NodeIterator iterator;

                iterator begin() const;
                iterator end() const;

                // Attribute iterators
                typedef AttributeIterator attribute_iterator;

                attribute_iterator attributes_begin() const;
                attribute_iterator attributes_end() const;

                // Range-based for support
                ObjectRange<NodeIterator> GetChildren() const;
                ObjectRange<NamedNode_iterator> GetChildren(const Char8* Name) const;
                ObjectRange<AttributeIterator> attributes() const;

                // Get node Offset in parsed file/string (in char_t units) for debugging purposes
                ptrdiff_t OffSetDebug() const;

                // Get hash Value (unique for handles to the same object)
                size_t HashValue() const;

                // Get internal pointer
                NodeStruct* InternalObject() const;
        }; // /Class Node
    }
}
#endif

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
