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
        class Document;
        class Element;
        class Text;
        class Comment;
        class StylesheetReference;
        class Declaration;

        /// @class Node
        /// @brief This represents Node that represent any element in an XML tree like the document, any element, ....
        /// @file xmlnode.h
        /// @details This wraps ticpp::Node tightly to help make the interface cleaner and easier to use
        class Node : public Base
        {
            protected:
                /// \internal
                /// @brief This throws a detailed formatted exception if the Node is not of the desired type.
                /// @param TypeItShouldBe is an XMLComponentType that will be compared to the type on the node
                /// @param NodeToCheck The Node to check
                /// @throw This throws a phys::Exception containing the message "The phys::xml::Node is not of type phys::xml::[TypeItShouldBe], it is a: [NodeToCheck's actual Type]";
                void ThrowIfMismatchingType(XMLComponentType TypeItShouldBe, Node* NodeToCheck);

            public:

                /// @brief Tears down an Node and removes the node from the DOM
                virtual ~Node ();

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

                /// @brief This identifies what kind of child of xml::base this is
                /// @return This returns the appropriate value for what this class is.
                virtual XMLComponentType GetType() const = 0;

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

                //not implementing
                //Node * 	ReplaceChild (Node *replaceThis, Node &withThis)
                //int 	Type () const
                //Query the type (as TiXmlNode::NodeType ) of this node.

                /// @brief Navigate to previous sibling node.
                /// @return A pointer to the previous sibling
                virtual Node * 	PreviousSibling () const = 0;

                /// @brief Navigate to the closest previous sibling node with matching Data.
                /// @param Value The returned sibling will have a value matching this one
                /// @return A pointer to the closest previous sibling with a matching value
                virtual Node * 	PreviousSibling (const std::string &Value) const = 0;

                /// @brief Navigate to next sibling node.
                /// @return A pointer to the next sibling
                virtual Node * 	NextSibling () const = 0;

                /// @brief Navigate to the closest next sibling node with matching Data.
                /// @param Value The returned sibling will have a value matching this one
                /// @return A pointer to the closest next sibling with a matching value
                virtual Node * 	NextSibling (const std::string &Value) const = 0;



                /* These with be implemented on the Element class, they are a better fit there.
                Element * 	NextSiblingElement (bool throwIfNoSiblings=true) const
                    Navigate to a sibling element.
                Element * 	NextSiblingElement (const std::string &value, bool throwIfNoSiblings=true) const
                    Navigate to a sibling element with the given value.
                Element * 	FirstChildElement (bool throwIfNoChildren=true) const
                    The first child element of this node.
                Element * 	FirstChildElement (const std::string &value, bool throwIfNoChildren=true) const
                    The first child element of this node with the matching value.
                */

                /// @brief Return a pointer to the Document this node lives in.
                /// @return A Pointer to the phys::xml::Document at the top of the xml Hierarchy
                Document* GetDocument() const;

                /// @brief Are there no child nodes
                /// @return True if this node has no children false otherwise
                bool NoChildren() const;

                /// @brief Convert this phys::xml::Node* to a phys::xml::Document*
                /// @throw If this is not a phys::xml::Document this will throw an phys::Exception with details
                /// @return A pointer this as a phys::xml::Document
                Document* ToDocument();

                /// @brief Convert this phys::xml::Node* to a phys::xml::Element*
                /// @throw If this is not a phys::xml::Element this will throw an phys::Exception with details
                /// @return A pointer this as a phys::xml::Element
                Element* ToElement();

                /// @brief Convert this phys::xml::Node* to a phys::xml::Comment*
                /// @throw If this is not a phys::xml::Comment this will throw an phys::Comment with details
                /// @return A pointer this as a phys::xml::Comment
                Comment* ToComment();

                /// @brief Convert this phys::xml::Node* to a phys::xml::Text*
                /// @throw If this is not a phys::xml::Text this will throw an phys::Exception with details
                /// @return A pointer this as a phys::xml::Text
                Text* ToText();

                /// @brief Convert this phys::xml::Node* to a phys::xml::Declaration*
                /// @throw If this is not a phys::xml::Declaration this will throw an phys::Exception with details
                /// @return A pointer this as a phys::xml::Declaration
                Declaration* ToDeclaration ();

                /// @brief Convert this phys::xml::Node* to a phys::xml::StylesheetReference*
                /// @throw If this is not a phys::xml::StylesheetReference this will throw an phys::Exception with details
                /// @return A pointer this as a phys::xml::StylesheetReference
                StylesheetReference* ToStylesheetReference ();
/*

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
