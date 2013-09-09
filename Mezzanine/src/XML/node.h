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

#ifndef _xmlnode_h
#define _xmlnode_h

/// @file
/// @brief This defines the @ref Mezzanine::XML::Node one of the central XML classes.

#include "datatypes.h"
#include "XML/xmlenumerations.h"
#include "XML/attribute.h"
#include "XML/objectrange.h"



namespace Mezzanine
{
    namespace XML
    {
        class NodeStruct;
        class NodeText;
        class TreeWalker;
        class XPathNode;
        class XPathNodeSet;
        class XPathVariableSet;
        class XPathQuery;
        class Writer;
        class NodeIterator;
        class AttributeIterator;
        class NamedNodeIterator;



        /// @brief A light-weight handle for manipulating nodes in DOM tree.
        class MEZZ_LIB Node
        {
                friend class AttributeIterator;
                friend class NodeIterator;
                friend class NamedNodeIterator;

            protected:
                /// @internal
                /// @brief Stores pointers to the Node data and some metadata.
                NodeStruct* NodeData;

                #ifndef SWIG
                /// @brief Used to prevent casting to numerical types acccidentally.
                /// @note Not available in scripting languages because conversion is handled on a per langauge basis.
                typedef void (*unspecified_bool_type)(Node***);
                #endif

            public:
                /// @brief Default constructor. Constructs an empty node.
                Node();

                /// @brief Virtual deconstructor.
                ~Node();

                /// @brief Constructs node from internal pointer
                /// @param p An internal node pointer.
                explicit Node(NodeStruct* p);

                #ifndef SWIG
                /// @brief Used to convert this to a boolean value in a safe way
                /// @return Returns true if the internal data is set and false otherwise.
                /// @note Not available in scripting languages because conversion is handled on a per langauge basis.
                operator unspecified_bool_type() const;
                #endif

                /// @brief Used to convert this node the opposite of it's normal boolean value
                /// @details This is described in the PugiXML source a a workaround for a borland c++ issue.
                /// @return Returns false if the internal pointer NodeStruct is set and true otherwise.
                bool operator!() const;

                #ifndef SWIG
                /// @brief Compares the internal values to check equality.
                /// @param r The other @ref Node this is being compared to.
                /// @details Many of the internal values are pointers, and it is the addresses of these that are being compared.
                /// @return Returns true if all the internal values match between this and the other Node.
                /// @note Not available in scripting languages
                bool operator==(const Node& r) const;
                #endif

                /// @brief Compares the internal values to check inequality.
                /// @param r The other @ref Node this is being compared to.
                /// @details Many of the internal values are pointers, and it is the addresses of these that are being compared.
                /// @return Returns true if any of the internal values don't match between this and the other @ref Node.
                bool operator!=(const Node& r) const;

                /// @brief Compares the internal values to check for lessthanness.
                /// @param r The other @ref Node this is being compared to.
                /// @details Many of the internal values are pointers, and it is the addresses of these that are being compared.
                /// @return Returns True if the other @ref Node is greater than this one as per sequential comparison of internal pointers.
                bool operator<(const Node& r) const;

                /// @brief Compares the internal values to check for greaterthanness.
                /// @param r The other @ref Node this is being compared to.
                /// @details Many of the internal values are pointers, and it is the addresses of these that are being compared.
                /// @return Returns True if the other @ref Node is less than this one as per sequential comparison of internal pointers.
                bool operator>(const Node& r) const;

                /// @brief Compares the internal values to check for inequality and lessthanness.
                /// @param r The other @ref Node this is being compared to.
                /// @details Many of the internal values are pointers, and it is the addresses of these that are being compared.
                /// @return Returns True if the other @ref Node is greater than or equal to this one as per sequential comparison of internal pointers.
                bool operator<=(const Node& r) const;

                /// @brief Compares the internal values to check for inequality and greaterthanness.
                /// @param r The other @ref Node this is being compared to.
                /// @details Many of the internal values are pointers, and it is the addresses of these that are being compared.
                /// @return Returns True if the other @ref Node is less than or equal to this one as per sequential comparison of internal pointers.
                bool operator>=(const Node& r) const;

                /// @brief Is this storing anything at all?
                /// @return Returns True if this @ref Node is storing nothing. False if it is storing anything.
                bool Empty() const;

                /// @brief Identify what kind of Node this is.
                /// @return A @ref NodeType identifying this Node, or o/NULL if this Node is empty.
                NodeType Type() const;

                /// @brief ptrdiff_tGet the name of this @ref Node.
                /// @return Returns A pointer to a const c-style array of the the character type (usually char or wchar_t) containing the name.
                /// @warning returns "" if Node is empty.
                const Char8* Name() const;

                /// @brief Get the Value of this @ref Node.
                /// @return Returns A pointer to a const c-style array of the the character type (usually char or wchar_t) containing the value.
                /// @warning returns "" if Node is empty.
                const Char8* Value() const;

                /// @brief Get the First Attribute in this Node.
                /// @return This attempts to return the First @ref Attribute in this node, if it cannot it returns an empty @ref Attribute.
                Attribute GetFirstAttribute() const;

                /// @brief Get the Last Attribute in this Node.
                /// @return This attempts to return the Last @ref Attribute in this node, if it cannot it returns an empty @ref Attribute.
                Attribute GetLastAttribute() const;

                /// @brief Get the first child Node of this Node.
                /// @return Returns the First child node if it exists, otherwise it return an empty node.
                Node GetFirstChild() const;

                /// @brief Get the last child Node of this Node.
                /// @return Returns the last child node if it exists, otherwise it return an empty node.
                Node GetLastChild() const;

                /// @brief Attempt to retrieve the next sibling of this Node.
                /// @details A sibling of a Node is another Node that shares the same parent. If this is and the sibling nodes are valid, this retrieves that Node, otherwise this return an empty Node.
                /// @return A Node that represents a sibling, or an empty Node on failure.
                Node GetNextSibling() const;

                /// @brief Attempt to retrieve the prvious sibling of this Node.
                /// @details A sibling of a Node is another Node that shares the same parent. If this is and the sibling nodes are valid, this retrieves that Node, otherwise this return an empty Node.
                /// @return A Node that represents a sibling, or an empty Node on failure.
                Node GetPreviousSibling() const;

                /// @brief Attempt to retrieve the parent of this Node.
                /// @return A Node that represents the parent Node, or an empty Node on failure.
                Node GetParent() const;

                /// @brief Attempt to retrieve the root Node, or the most base Node containing this Node.
                /// @return A Node that represents the root of the XML document, or an empty Node on failure. If there are multiple roots this attempts to retrieve the appropriate one.
                Node GetRoot() const;

                /// @brief Get text object for the current node
                /// @return An @ref Mezzanine::XML::NodeText which represents the PCData of this node.
                NodeText GetText() const;

                #ifndef SWIG
                /// @brief Attempt to get a child Node with a given name.
                /// @param Name The name of the desired child Node.
                /// @return A Node that represents the first desired child, or an empty Node on failure.
                /// @note Not available in scripting languages in favor of the overload of this function that accepts String as a parameter.
                Node GetChild(const Char8* Name) const;
                #endif

                /// @brief Attempt to get a child Node with a given name.
                /// @param Name The name of the desired child Node.
                /// @return A Node that represents the first desired child, or an empty Node on failure.
                Node GetChild(const String& Name) const
                    { return GetChild(Name.c_str()); }

                /// @brief Attempt to get an Attribute on this Node with a given name.
                /// @param Name The name of the desired Attribute.
                /// @return An Attribute that represents the first matching Attribute, or an empty Attribute on failure.
                Attribute GetAttribute(const Char8* Name) const;

                /// @brief Like @ref GetNextSibling except that the return will be a null Node or have a matching name.
                /// @param Name if possible this will return the next sibling with this name.
                /// @return The next sibling with a matching name, or a null/empty node.
                Node GetNextSibling(const Char8* Name) const;

                /// @brief Like @ref GetPreviousSibling except that the return will be a null Node or have a matching name.
                /// @param Name if possible this will return the previous sibling with this name.
                /// @return The previous sibling with a matching name, or a null/empty node.
                Node GetPreviousSibling(const Char8* Name) const;

                /// @brief Retrieve the value of this(or a child's) Nodes PCDATA child Node
                /// @details If this node represents "<node>Some text in the PCDATA field, that is actually represent by a node</node>", then this would return "Some text in the PCDATA field, that is actually represent by a node". This will iterate through child Nodes from until it finds a PCDATA node or fails
                /// @return This will return the Value of the first available PCDATA node.
                const Char8* GetChildValue() const;

                /// @brief Get the PCDATA of a given child. The same a calling "GetChild(Name).ChildValue()".
                /// @param Name The Name of the desired child node.
                /// @return This will return the Value of the first available matching PCDATA node.
                const Char8* GetChildValue(const Char8* Name) const;

                #ifndef SWIG
                /// @brief Set the name of .
                /// @param rhs The desired name.
                /// @return True if successful, returns false if the name cannot be stored or there is not enough memory.
                /// @note Not available in scripting languages in favor of the overload of this function that accepts String as a parameter.
                bool SetName(const Char8* rhs);
                #endif

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

                #ifndef SWIG
                /// @brief Creates an Attribute and puts it at the end of this Nodes attributes.
                /// @param Name The name of the New attribute to be created
                /// @details This attempts to create an Attribute and stick it at the end of the list of attribute on the current
                /// Node. This will fail and return an Empty Attribute if this Node is neither an Element nor a Declaration. This will
                /// fail and return an empty attribute if this Node is empty.
                /// @return The created Attribute or an empty Attribute on Failure.
                /// @note Not available in scripting languages in favor of the overload of this function that accepts String as a parameter.
                Attribute AppendAttribute(const Char8* Name);
                #endif

                /// @brief Creates an Attribute and puts it at the end of this Nodes attributes.
                /// @param Name The name of the New attribute to be created
                /// @return The created Attribute or an empty Attribute on Failure.
                Attribute AppendAttribute(const String& Name)
                    { return AppendAttribute(Name.c_str()); }

                /// @brief Creates an Attribute and puts it at the begining of this Nodes attributes
                /// @param Name The name of the New attribute to be created
                /// @details This attempts to create an Attribute and stick it at the beginning of the list of attributes on the current
                /// Node. This will fail and return an Empty Attribute if this Node is neither an Element nor a Declaration. This will
                /// fail and return an empty attribute if this Node is empty.
                /// @return The created Attribute or an empty Attribute on Failure.
                Attribute PrependAttribute(const Char8* Name);

                /// @brief Creates an Attribute and puts it into the list of this Nodes attributes.
                /// @param Name The name of the New attribute to be created
                /// @param attr An Attribute that represents an Attribute on this Node, and is just before where you want the new Attribute.
                /// @details This attempts to create an Attribute and stick it in the list of attributes, Just after another Attribute, on the current
                /// Node. This will fail and return an Empty Attribute if this Node is neither an Element nor a Declaration. This will
                /// fail and return an empty attribute if this Node is empty.
                /// @return The created Attribute or an empty Attribute on Failure.
                Attribute InsertAttributeAfter(const Char8* Name, const Attribute& attr);

                /// @brief Creates an Attribute and puts it into the list of this Nodes attributes.
                /// @param Name The name of the New attribute to be created
                /// @param attr An Attribute that represents an Attribute on this Node, and is just after where you want the new Attribute.
                /// @details This attempts to create an Attribute and stick it in the list of attributes, Just before another Attribute, on the current
                /// Node. This will fail and return an Empty Attribute if this Node is neither an Element nor a Declaration. This will
                /// fail and return an empty attribute if this Node is empty.
                /// @return The created Attribute or an empty Attribute on Failure.
                Attribute InsertAttributeBefore(const Char8* Name, const Attribute& attr);

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
                /// Node. Tptrdiff_this will fail and return an Empty Attribute if this Node is neither an Element nor a Declaration. This will
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

                /// @brief Copies an Attribute and puts the copy into the list of this Nodes attributes.
                /// @param proto The attribute to be copied.
                /// @param attr An Attribute that represents an Attribute on this Node, and is just after where you want the new copy of proto.
                /// @details This attempts to create a copy of an attribute Attribute and stick it in the middle of the list of attributes, just before a selected attribute, on the current
                /// Node. This will fail and return an Empty Attribute if this Node is neither an Element nor a Declaration. This will
                /// fail and return an empty attribute if this Node is empty.
                /// @return The created Attribute or an empty Attribute on Failure.
                Attribute InsertCopyBefore(const Attribute& proto, const Attribute& attr);

                /// @brief Creates a Node and makes it a child of this one.
                /// @param Type The NodeType of the Node to be added to list of child Nodes.
                /// @return A Node representing the freshly added Node, or an empty Node if there was an error.
                Node AppendChild(NodeType Type = NodeElement);

                /// @brief Creates a Node and makes it a child of this one, and puts at the beginning of the Child Nodes.
                /// @param Type The NodeType of the Node to be added to the beginning list of child Nodes.
                /// @return A Node representing the freshly added Node, or an empty Node if there was an error.
                /// @todo Not all nodes can be added to other nodes, we need to figure it out and put it here.
                Node PrependChild(NodeType Type = NodeElement);

                /// @brief Creates a Node and makes it a child of this one, and puts at the middle of the Child Nodes.
                /// @param Type The NodeType of the Node to be added, just after another specific node.
                /// @param node The specific node to add the new one after.
                /// @todo Not all nodes can be added to other nodes, we need to figure it out and put it here.
                /// @return A Node representing the freshly added Node, or an empty Node if there was an error.
                Node InsertChildAfter(NodeType Type, const Node& node);

                /// @brief Creates a Node and makes it a child of this one, and puts at the middle of the Child Nodes.
                /// @param Type The NodeType of the Node to be added, just before another specific node.
                /// @param node The specific node to add the new one before.
                /// @return A Node representing the freshly added Node, or an empty Node if there was an error.
                /// @todo Not all nodes can be added to other nodes, we need to figure it out and put it here.
                Node InsertChildBefore(NodeType Type, const Node& node);

                #ifndef SWIG
                /// @brief Creates an element Node as a child of this Node, with the given name.
                /// @param Name The name of the Node to be created.
                /// @details Calls @ref Node::AppendChild(NodeType); using NodeElement as the NodeType.
                /// @return The desired Node on success, an empty Node on failure.
                /// @note Not available in scripting languages in favor of the overload of this function that accepts String as a parameter.
                Node AppendChild(const Char8* Name);
                #endif

                /// @brief Creates an element Node as a child of this Node, with the given name.
                /// @param Name The name of the Node to be created.
                /// @details Calls @ref Node::AppendChild
                /// @return The desired Node on success, an empty Node on failure.
                Node AppendChild(const String& Name)
                    { return AppendChild(Name.c_str()); }

                #ifndef SWIG
                /// @brief Creates an element Node as a child of this Node, with the given name at the beginning of the children
                /// @param Name The name of the Node to be created.
                /// @details Calls @ref Node::PrependChild(NodeType); using NodeElement as the NodeType.
                /// @return The desired Node on success, an empty Node on failure.
                /// @note Not available in scripting languages in favor of the overload of this function that accepts String as a parameter.
                Node PrependChild(const Char8* Name);
                #endif

                /// @brief Creates an element Node as a child of this Node, with the given name at the beginning of the children
                /// @param Name The name of the Node to be created.
                /// @details Calls @ref Node::PrependChild
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

                /// @brief Remove specified Attribute.
                /// @param a The Attribute to look for. If the given Attribute doesn't belong to this Node then this will fail
                /// @return True if the removal was successful, false otherwise.
                bool RemoveAttribute(const Attribute& a);

                /// @brief Remove Attribute as specified by name.
                /// @param Name The name of the Attribute to remove.
                /// @return True if the removal was successful, false otherwise.
                bool RemoveAttribute(const Char8* Name);

                /// @brief Remove specified child element.
                /// @param n The Node to look for. If the given Attribute doesn't belong to this Node then this will fail
                /// @return True if the removal was successful, false otherwise.
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
                    if (!NodeData) return Attribute();

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
                    if (!NodeData) return Node();

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
                    if (!NodeData) return Node();

                    Node cur = GetFirstChild();
                    while (cur.NodeData && cur.NodeData != NodeData)
                    {
                        if (pred(cur)) return cur;

                        if (cur.GetFirstChild()) cur = cur.GetFirstChild();
                        else if (cur.GetNextSibling()) cur = cur.GetNextSibling();
                        else
                        {
                            while (!cur.GetNextSibling() && cur.NodeData != NodeData) cur = cur.GetParent();

                            if (cur.NodeData != NodeData) cur = cur.GetNextSibling();
                        }
                    }

                    return Node();
                }

                /// @brief Find a Node by an Attribute it has.
                /// @param Name The name of the matching Node.
                /// @param AttrName The name of the matching Attribute.
                /// @param AttrValue The value of the matching Attribute.
                /// @details Any Null pointers instead of character arrays passed in will cause undefined behavior. All Matching is Case sensitive.
                /// @return The First matching XML::Node
                Node FindChildbyAttribute(const Char8* Name, const Char8* AttrName, const Char8* AttrValue) const;

                /// @brief Find a Node by an Attribute it has.
                /// @param AttrName The name of the matching Attribute.
                /// @param AttrValue The value of the matching Attribute.
                /// @details Any Null pointers instead of character arrays passed in will cause undefined behavior. All Matching is Case sensitive.
                /// @return The First matching XML::Node
                Node FindChildbyAttribute(const Char8* AttrName, const Char8* AttrValue) const;

                /// @brief Get the absolute path to this Node
                /// @param delimiter The character to use as a pathname separator, this defaults to '/'.
                /// @return A String containing an path
                String Path(Char8 delimiter = '/') const;

                /// @brief Search for a node by Path consisting of node names and . or .. elements.
                /// @todo Investigate this more deeply.
                /// @param Path The path to search for.
                /// @param delimiter The character to use as a pathname separator, this defaults to '/'.
                /// @return The matching Node, of an empty Node on failure.
                Node FirstElementByPath(const Char8* Path, Char8 delimiter = '/') const;

                /// @brief Perform sophisticated (or whatever) algorithms on this and all descendant Nodes in the XML tree.
                /// @param walker Any class that fully implement XML::Treewalker. This is where the algorithm to be run is located.
                /// @return True if every descendant Node of this Node was iterated through, false if it didn't go through every Node.
                /// @see XML::TreeWalker
                bool Traverse(TreeWalker& walker);

                /// @brief Select single node by evaluating an XPath query. Returns first node from the resulting node set.
                /// @param query The XPath query as a c-string to be evaluated.
                /// @param variables undocumented.
                /// @return XPathNode The first matching XPath node.
                XPathNode FindSingleNode(const Char8* query, XPathVariableSet* variables = 0) const;

                /// @brief Select single node by evaluating an XPath query. Returns first node from the resulting node set.
                /// @param query The XPath query XPathQuery class instance.
                /// @return XPathNode The first matching XPath node.
                XPathNode FindSingleNode(const XPathQuery& query) const;

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

                #ifndef SWIG_SAFE
                /// @brief Output the XML document using a Writer.
                /// @param WriterInstance The Writer that will be used to output the xml text.
                /// @param indent The Character(s) used to represent a tab in the output, this defaults to one tab character.
                /// @param flags The output format flags, this is a bitfield that defaults to XML::FormatDefault.
                /// @param DocumentEncoding The XML::Encoding of the document, whichs defaults to EncodingAuto
                /// @param Depth This defaults to 0. The amount of times to prepend the indentation to the beginning of each output line.
                /// @details This will never write a Byte Order Mark(BOM), and will default to not outputing a document declaration.
                /// @note This is not available in the safe libraries in scripting languages
                void Print(Writer& WriterInstance, const Char8* indent = "\t", unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto, unsigned int Depth = 0) const;

                /// @brief Output the XML document using a Output Stream.
                /// @param os An output stream to send xml text to.
                /// @param indent The Character(s) used to represent a tab in the outpput, this defaults to one tab character.
                /// @param flags The output format flags, this is a bitfield that defaults to XML::FormatDefault
                /// @param DocumentEncoding The XML::Encoding of the document, whichs defaults to EncodingAuto
                /// @param Depth This defaults to 0. The amount of times to prepend the indentation to the beginning of each output line.
                /// @details This will never write a Byte Order Mark(BOM), and will default to not outputing a document declaration.
                /// @note This is not available in the safe libraries in scripting languages
                void Print(std::basic_ostream<char, std::char_traits<char> >& os, const Char8* indent = "\t", unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto, unsigned int Depth = 0) const;

                /// @brief Output the XML document using a Output Stream.
                /// @param os An output stream to send xml text to.
                /// @param indent The Character(s) used to represent a tab in the outpput, this defaults to one tab character.
                /// @param flags The output format flags, this is a bitfield that defaults to XML::FormatDefault
                /// @param Depth This defaults to 0. The amount of times to prepend the indentation to the beginning of each output line.
                /// @details This will never write a Byte Order Mark(BOM), and will default to not outputing a document declaration.
                /// @note This is not available in the safe libraries in scripting languages
                void Print(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& os, const Char8* indent = "\t", unsigned int flags = FormatDefault, unsigned int Depth = 0) const;
                #endif //SWIG_SAFE

                /// @brief An iterator for child Nodes that will be easier for members of the std namespace to work with.
                typedef NodeIterator iterator;

                /// @brief Get a Child node iterator that references the first child Node.
                /// @return A Node::Iterator that reference the first child Node.
                iterator begin() const;

                /// @brief Get a Child node iterator that references one past the last child Node.
                /// @return A Node::Iterator that reference the last child Node.
                iterator end() const;

                /// @brief An iterator for Attribute members on this Node
                typedef AttributeIterator attribute_iterator;

                /// @brief Get an Attribute iterator that references the first Attribute on this Node.
                /// @return A Node::Iterator that reference the first child node.
                attribute_iterator attributes_begin() const;

                /// @brief Get an Attribute iterator that references the one past the last Attribute on this Node.
                /// @return A Node::Iterator that reference the last Attribute on this Node.
                attribute_iterator attributes_end() const;

                /// @brief Get an iterator range for this node's children nodes.
                /// @return The Begin and end iterators for a collection of all child nodes.
                ObjectRange<NodeIterator> GetChildren() const;

                /// @brief Get an iterator range for this a subset of this node's children nodes.
                /// @param Name All members of the returned range with have this for a name.
                /// @return A begin and end iterator for a range containing only the child nodes with the given name.
                ObjectRange<NamedNodeIterator> GetChildren(const Char8* Name) const;

                /// @brief A range of iterators for just the attributes of this node.
                /// @return A par of iterators suitable for traversing all Attributes directly as children of this noce.
                ObjectRange<AttributeIterator> attributes() const;

                /// @internal
                /// @brief Get node Offset in parsed file/string (in char_t units) for debugging purposes
                /// @return A ptrdiff_t containing how far into the document this node is.
                ptrdiff_t OffSetDebug() const;

                /// @internal
                /// @brief Get hash Value (unique for handles to the same object)
                /// @return A size_t that uniquely identifies this node.
                size_t HashValue() const;

                /// @internal
                /// @brief Get internal pointer
                /// @return A NodeStruct* that points to the internal data of this Node
                NodeStruct* InternalObject() const;
        }; // /Class Node
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
