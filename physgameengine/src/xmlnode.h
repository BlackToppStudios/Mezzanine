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

#ifndef _xmlnode_h
#define _xmlnode_h

#include "xmlbase.h"

namespace ticpp
{
    class Node;
}

namespace phys
{
    namespace xml
    {
        /// @class Node
        /// @brief This represents Node that represent any element in an XML tree like the document, any element, ....
        /// @file xmlnode.h
        /// @details This wraps ticpp::Node tightly to help make the interface cleaner and easier to use
        class Node : public Base
        {
            protected:
                /// @brief Construct an attribute using meta data from a TiCPP pointer
                /// @param Meta A pointer to a ticpp::Node that this class will wrap.
                /// @param FirstTimeWrapped Set this to true if you are instantiating this for the first time, false if Meta is used in and phys::xml::base
                Node (ticpp::Node* Meta, bool FirstTimeUsed = false);

            public:
                /// @brief Construct an empty Node.
                Node ();

                /// @brief Tears down an Node and removes the node from the DOM
                virtual ~Node ();

                /// @brief Copy Constructor
                /// @param CopiedNode The Node we are copying.
                Node (const Node &CopiedNode);


                /// @brief Get the value of this Node.
                /// @return this returns a String containing the value of this attribute.
                virtual String GetValueAsString() const;

                /// @brief Get the value of this Node converted to a Whole Number.
                /// @return this returns a Whole containing the value of this attribute.
                virtual Whole GetValueAsWhole() const;

                /// @brief Get the value of this Node converted to a Real Number.
                /// @return this returns a Real containing the value of this attribute.
                virtual Real GetValueAsReal() const;

                /// @brief Set the value of this XML Object.
                /// @param Value A Whole number to be inserted into the XML document
                virtual void SetValue(const Whole &Value);

                /// @brief Set the value of this XML Object.
                /// @param Value A Real number to be inserted into the XML document
                virtual void SetValue(const Real &Value);

                /// @brief Set the value of this XML Object.
                /// @param Value A String number to be inserted into the XML document
                virtual void SetValue(const String &Value);

                /// @brief Deletes all the nodes stored below this one
                /// @details Leaves this node intact, but deletes all lower in the XML tree
                void Clear();

                /// @brief This identifies what kind of child of xml::base this is
                /// @return This returns Base::isNode
                virtual XMLComponentType GetType();

                /// @brief Returns the Parent Node of this Node Throws an exception if none exists
                /// @return This returns a pointer to a phys::xml::node. Specifically the node that considers this Node it's child.
                virtual Node* Parent() const = 0;

                /// @brief Returns the First Child of this node
                /// @return This returns a pointer to a phys::xml::node. Specifically the child node suitable to begin iterations from.
                virtual Node* FirstChild () const = 0;

                /// @brief Returns the First Child of this node with a matching value
                /// @param Value The returned child will have a value matching this one
                /// @return This returns a pointer to a phys::xml::node. Specifically the node closest to the beginning that has a matching value.
                virtual Node* FirstChild (const std::string &Value) const = 0;

                /// @brief Returns the Last Child of this node
                /// @return This returns a pointer to a phys::xml::node. Specifically the child node suitable to end iterations at.
                virtual Node* LastChild () const = 0;

                /// @brief Returns the Last Child of this node
                /// @param Value The returned child will have a value matching this one
                /// @return This returns a pointer to a phys::xml::node. Specifically the node closest to the end that has a matching value.
                virtual Node* LastChild (const std::string &Value) const = 0;

                // I skipped these as they uneeded and confusing. They have been supplanted with the clean node iteration methods listed above and by iterators
                //Node * 	IterateChildren (Node *previous) const
                // 	An alternate way to walk the children of a node.
                //Node * 	IterateChildren (const std::string &value, Node *previous) const
                // 	This flavor of IterateChildren searches for children with a particular value.

                /// @brief Adds a child node to the end of the list
                /// @param AddThis The Node to be added
                /// @warning Throws an error if you try to insert a Document
                void AppendChild (Node &AddThis);

                /// @brief Adds a child node after a specified node
                /// @param AfterThis A Pointer to a node already a child, that is to preceed the added node
                /// @param AddThis The Node to be added.
                /// @warning Throws an error if you try to insert a Document
                void InsertAfterChild (Node *AfterThis, Node &AddThis);

                /// @brief Adds a child node before a specified node
                /// @param BeforeThis A Pointer to a node already a child, that is to follow the added node.
                /// @param AddThis The Node to be added.
                /// @warning Throws an error if you try to insert a Document.
                void InsertBeforeChild (Node *BeforeThis, Node &AddThis);

                /// @brief Remove a child node
                /// @param RemoveThis A Pointer to the child node to be removed.
                void RemoveChild (Node *RemoveThis);

                //not implementing
                //Node * 	ReplaceChild (Node *replaceThis, Node &withThis)
                //int 	Type () const
                //Query the type (as TiXmlNode::NodeType ) of this node.

                /// @brief

/*

Node * 	PreviousSibling (bool throwIfNoSiblings=true) const
 	Navigate to a sibling node.
Node * 	PreviousSibling (const std::string &value, bool throwIfNoSiblings=true) const
 	Navigate to a sibling node with the given value.
Node * 	NextSibling (bool throwIfNoSiblings=true) const
 	Navigate to a sibling node.
Node * 	NextSibling (const std::string &value, bool throwIfNoSiblings=true) const
 	Navigate to a sibling node with the given value.

Element * 	NextSiblingElement (bool throwIfNoSiblings=true) const
 	Navigate to a sibling element.
Element * 	NextSiblingElement (const std::string &value, bool throwIfNoSiblings=true) const
 	Navigate to a sibling element with the given value.
Element * 	FirstChildElement (bool throwIfNoChildren=true) const
 	The first child element of this node.
Element * 	FirstChildElement (const std::string &value, bool throwIfNoChildren=true) const
 	The first child element of this node with the matching value.

Document * 	GetDocument (bool throwIfNoDocument=true) const
 	Return a pointer to the Document this node lives in.
bool 	NoChildren () const
 	Check if this node has no children.
template<class T>  T * 	To () const
 	Pointer conversion ( NOT OBJECT CONVERSION ) - replaces TiXmlNode::ToElement, TiXmlNode::ToDocument, TiXmlNode::ToComment, etc.
Document * 	ToDocument () const
 	Pointer conversion - replaces TiXmlNode::ToDocument.
Element * 	ToElement () const
 	Pointer conversion - replaces TiXmlNo
Kirk millerde::ToElement.

Comment * 	ToComment () const
 	Pointer conversion - replaces TiXmlNode::ToComment.
Text * 	ToText () const
 	Pointer conversion - replaces TiXmlNode::ToText.
Declaration * 	ToDeclaration () const
 	Pointer conversion - replaces TiXmlNode::ToDeclaration.
StylesheetReference * 	ToStylesheetReference () const
 	Pointer conversion - replaces TiXmlNode::ToStylesheetReference.
std::auto_ptr< Node > 	Clone () const
 	Create an exact duplicate of this node and return it.
bool 	Accept (TiXmlVisitor *visitor) const
 	Accept a hierchical visit the nodes in the TinyXML DOM.

Friends
std::istream & 	operator>> (std::istream &in, Node &base)
 	Stream input operator.
std::ostream & 	operator<< (std::ostream &out, const Node &base)
 	Stream output operator.
*/



        };// \Node
    } // \xml
}//\phys

#endif
