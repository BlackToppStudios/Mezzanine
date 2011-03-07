//© Copyright 2010 Joseph Toppi and John Blackwood
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
/*
 *
 * Software, Files, Libraries and all other items referenced in this clause refers only
 * to the contents of this file and associated documentation.
 *
 * Pugixml parser - version 1.0
 * --------------------------------------------------------
 * Copyright © 2006-2010, by Arseny Kapoulkine (arseny.kapoulkine@gmail.com)
 * Report bugs and download new versions at http://pugixml.org/
 *
 * This library is distributed under the MIT License. See notice at the end
 * of this file.
 *
 * This work is based on the pugxml parser, which is:
 * Copyright © 2003, by Kristen Wegner (kristen@tima.net)
 */

// To provide compilation option via cmake
#ifdef PHYSXML

#include "datatypes.h"

#ifndef _xmldoc_h
#define _xmldoc_h
namespace phys
{

    ///////////////////////////////////////////////////////////////////////////////
    /// @namespace phys::xml
    /// @brief This is where bulk of the XML subsystem is declare, there are numerous class that are all tighlty integrated so one file seemed appropriate.
    /// @details Our XML Parser IS a copy of PugiXML. It is simply the fastest,
    /// most stable, most reliable, feature rich, and robust XML parser we were
    /// aware of. We are using it with permission per it's included license. See the
    /// licenses folder, in either the docs folder, or the data/common folder. \n \n
    /// This XML parser uses an 'in place' design. It attmempts to load all of an XML document to one contiguous location in
    /// in memory, then create a tree of meta data that reference the already loaded document text. In testing this has been shown to be extremely fast.
    /// See pugixml.org for more details on the performace. \n \n
    /// The one drawback PugiXML had was that it has a completely different naming
    /// convention from our code. So we created a script ( docs/tools/PugiConversion)
    /// to covert version 1.1 and hopefully future versions to out naming convention.
    ///////////////////////////////////////
    namespace xml
    {

        class Document;

        ///////////////////////////////////////////////////////////////////////////////
        /// @file xmldoc.h
        /// @brief The Doxygen documentation and minor tools to assist with the XML system
        /// @details The integration process for PugiXML requires that our changes all be stored in shell script.
        /// This is not condusive to good clean documentation  and is why the doxygen documention for the XML parser exist here
        /// instead of xml.h.
        ///////////////////////////////////////

        ///////////////////////////////////////////////////////////////////////////////
        /// @file xml.h
        /// @brief This is where bulk of the XML subsystem is declare, there are numerous class that are all tighlty integrated so one file seemed appropriate.
        /// @details See xmldoc.h for additional details.
        ///////////////////////////////////////

        ///////////////////////////////////////////////////////////////////////////////
        /// @enum ParseStatus
        /// @brief These statuses are used to help determine what issues, if any the parser had. the XML parser had.

        /// @var StatusOk
        /// @brief This is returned to indicated there where no issues parsing the XML document

        /// @var StatusFileNotFound
        /// @brief File was not found during a loading from filename attempt.

        /// @var StatusIOError
        /// @brief Error reading from file or stream.

        /// @var StatusOutOfMemory
        /// @brief Could not allocate memory.

        /// @var StatusInternalError
        /// @brief An unkown error, currently nothing should be able to return this status

        /// @var StatusUnrecognizedTag
        /// @brief The parser could not determine type of tag.

        /// @var StatusBadPi
        /// @brief Parsing error occurred while parsing document declaration/processing instruction.

        /// @var StatusBadComment
        /// @brief Parsing error occurred while parsing comment.

        /// @var StatusBadCdata
        /// @brief Parsing error occurred while parsing CDATA section.

        /// @var StatusBadDocType
        /// @brief Parsing error occurred while parsing document type declaration.

        /// @var StatusBadPcdata
        /// @brief Parsing error occurred while parsing PCDATA section.

        /// @var StatusBadStartElement
        /// @brief Parsing error occurred while parsing start element tag .

        /// @var StatusBadAttribute
        /// @brief Parsing error occurred while parsing element attribute.

        /// @var StatusBadEndElement
        /// @brief Parsing error occurred while parsing end element tag.

        /// @var StatusEndElementMismatch
        /// @brief There was a mismatch of start-end tags (closing tag had incorrect name, some tag was not closed or there was an excessive closing tag).

        ///////////////////////////////////////////////////////////////////////////////
        /// @struct ParseResult
        /// @brief All the information needed to troubleshoot a Parsing error.

        ///////////////////////////////////////////////////////////////////////////////
        /// @enum Encoding
        /// @brief These flags determine the encoding of input data for an XML document.

        /// @var EncodingAuto
        /// @brief Auto-detect input document encoding using Byte Order Mark (BOM) or < / <? detection; use UTF8 if BOM is not found .

        /// @var EncodingUTF8
        /// @brief UTF8 document encoding

        /// @var EncodingUTF16LE
        /// @brief Little-endian UTF16

        /// @var EncodingUTF16BE
        /// @brief Big-endian UTF16

        /// @var EncodingUTF16
        /// @brief UTF16 with native endianness

        /// @var EncodingUTF32LE
        /// @brief Little-endian UTF32

        /// @var EncodingUTF32BE
        /// @brief Big-endian UTF32

        /// @var EncodingUTF32
        /// @brief UTF32 with native endianness

        /// @var Encodingwchar_t
        /// @brief The same document encoding wchar_t has (usually either UTF16 or UTF32)

        ///////////////////////////////////////////////////////////////////////////////
        /// @enum NodeType
        /// @brief The types of nodes.

        /// @var NodeNull
        /// @brief Empty (null) node handle.

        /// @var NodeDocument
        /// @brief A document tree's absolute root.

        /// @var NodeElement
        /// @brief Element tag, i.e. '<node/>'.

        /// @var NodePcdata
        /// @brief Plain character data, i.e. 'text'.

        /// @var NodeCdata
        /// @brief Character data, i.e. '<![CDATA[text]]>'.

        /// @var NodeComment
        /// @brief Comment tag, i.e. '<!-- text -->'.

        /// @var NodePi
        /// @brief Processing instructions to the XML parser, i.e. '<?name?>'.

        /// @var NodeDeclaration
        /// @brief Document declaration, i.e. '<?xml version="1.0"?>'.

        /// @var NodeDocType
        /// @brief Document type declaration, i.e. '<!DOCTYPE doc>'.

        ///////////////////////////////////////////////////////////////////////////////
        /// @var phys::xml::ParseMinimal
        /// @brief Minimal parsing mode (equivalent to turning all other flags off).
        /// @details Only elements and PCDATA sections are added to the DOM tree, no text conversions are performed.

        /// @var ParsePi
        /// @brief This flag determines if processing instructions (NodePi) are added to the DOM tree. This flag is off by default.

        /// @var ParseComments
        /// @brief This flag determines if comments (NodeComment) are added to the DOM tree. This flag is off by default.

        /// @var ParseCdata
        /// @brief This flag determines if CDATA sections (NodeCdata) are added to the DOM tree. This flag is on by default.

        /// @var ParseWsPcdata
        /// @brief This flag determines if plain character data (NodePcdata) that consist only of whitespace are added to the DOM tree.
        /// @details This flag is off by default; turning it on usually results in slower parsing and more memory consumption.

        /// @var ParseEscapes
        /// @brief This flag determines if character and entity references are expanded during parsing. This flag is on by default.

        /// @var ParseEol
        /// @brief This flag determines if EOL characters are normalized (converted to #xA) during parsing. This flag is on by default.

        /// @var ParseWconvAttribute
        /// @brief This flag determines if attribute values are normalized using CDATA normalization rules during parsing. This flag is on by default.

        /// @var ParseWnormAttribute
        /// @brief This flag determines if attribute values are normalized using NMTOKENS normalization rules during parsing. This flag is off by default.

        /// @var ParseDeclaration
        /// @brief This flag determines if document declaration (NodeDeclaration) is added to the DOM tree. This flag is off by default.

        /// @var ParseDocType
        /// @brief This flag determines if document type declaration (NodeDoctype) is added to the DOM tree. This flag is off by default.

        /// @var ParseDefault
        /// @brief The default parsing mode.
        /// @details Elements, PCDATA and CDATA sections are added to the DOM tree, character/reference entities are expanded,
        /// End-of-Line characters are normalized, attribute values are normalized using CDATA normalization rules.

        /// @var ParseFull
        /// @brief The full parsing mode.
        /// @details Nodes of all types are added to the DOM tree, character/reference entities are expanded,
        /// End-of-Line characters are normalized, attribute values are normalized using CDATA normalization rules.

        ///////////////////////////////////////////////////////////////////////////////
        // @typedef string_t
        // @brief A string class used internally to store data in the XML parser.

        ///////////////////////////////////////////////////////////////////////////////
        /// @var FormatIndent
        /// @brief Indent the nodes that are written to output stream with as many indentation strings as deep the node is in DOM tree. This flag is on by default.

        /// @var FormatWriteBom
        /// @brief Write encoding-specific Byte Order Mark (BOM) to the output stream. This flag is off by default.

        /// @var FormatRaw
        /// @brief Use raw output mode (no indentation and no line breaks are written). This flag is off by default.

        /// @var FormatNoDeclaration
        /// @brief Omit default XML declaration even if there is no declaration in the document. This flag is off by default.

        /// @var FormatDefault
        /// @brief The default set of formatting flags.
        /// @details Nodes are indented depending on their depth in DOM tree, a default declaration is output if document has none.

        ///////////////////////////////////////////////////////////////////////////////
        /// @class Writer
        /// @brief Interface for node printing (see @ref Node::print)

        ///////////////////////////////////////////////////////////////////////////////
        /// @class WriterFile
        /// @brief An implementation of @ref Writer intended for writing to FILEs as defined in stdio

        /// @var WriterFile::file
        /// @internal
        /// @brief A pointer to a FILE as defined in stdio

        /// @fn WriterFile::WriterFile(void* file)
        /// @brief Construct a Writer from a FILE* object.
        /// @param file The FILE to be written to. The FILE can be a File handle as per stdio or the standard input, output or even error. The use of void* was intended to avoid a depedency on the stdio header, in the original PugiXML. After a review for compatibility this may change to promote better type safety.

        ///////////////////////////////////////////////////////////////////////////////
        /// @class WriterStream
        /// @brief An implementation of @ref Writer intended for writing std::ostreams

		/// @var WriterStream::narrow_stream
		/// @internal

        /// @var WriterStream::wide_stream
		/// @internal

        ///////////////////////////////////////////////////////////////////////////////
        /// @def XML_DEPRECATED
        /// @brief Used to mark XML internals as deprecated at the compiler level.

        ///////////////////////////////////////////////////////////////////////////////
        /// @def _XML_H
        /// @internal
        /// @brief Prevents accidental loading of the file xml.h multiple times.

        ///////////////////////////////////////////////////////////////////////////////
        /// @def XML_TEXT
        /// @internal
        /// @brief Used before the inclusion of string literal in the Pugi XML code, to allow the determination of the character type at compile time.

        ///////////////////////////////////////////////////////////////////////////////
        /// @def XML_CHAR
        /// @internal
        /// @brief Links the  Pugi character datatype to match the Physgame phys::Characterdatatype

        ///////////////////////////////////////////////////////////////////////////////
        /// @class Attribute
        /// @brief A light-weight handle for manipulating attributes in DOM tree

        /// @var Attribute::_attr
        /// @internal
        /// @brief Stores pointers to the attribute data and some metadata about the attribute.

        /// @fn Attribute::Attribute()
        /// @brief Constructs an empty Attribute.

        /// @fn Attribute::Attribute(AttributeStruct* attr)
        /// @brief Constructs attribute from internal pointer.
        /// @param attr An internal AttributeStruct pointer containing all the data to create an attribute.

        /// @fn Attribute::operator!() const
        /// @brief Used to convert this attribute the opposite of it's normal boolean value
        /// @details This is described in the PugiXML source a a workaround for a borland c++ issue.
        /// @return Returns false if the internal pointer AttributeStruct is set and true otherwise.

        /// @fn Attribute::operator==(const Attribute& r) const;
        /// @brief Compares the internal values to check equality.
        /// @param r The other @ref Attribute this is being compared to.
        /// @details Many of the internal values are pointers, and it is the addresses of these that are being compared.
        /// @return Returns true if all the internal values match between this and the other Attribute.

        /// @fn Attribute::operator!=(const Attribute& r) const;
        /// @brief Compares the internal values to check inequality.
        /// @param r The other @ref Attribute this is being compared to.
        /// @details Many of the internal values are pointers, and it is the addresses of these that are being compared.
        /// @return Returns true if any of the internal values don't match between this and the other @ref Attribute.

        /// @fn Attribute::operator<(const Attribute& r) const;
        /// @brief Compares the internal values to check for inequality.
        /// @param r The other @ref Attribute this is being compared to.
        /// @details Many of the internal values are pointers, and it is the addresses of these that are being compared.
        /// @return Returns True if the other @ref Attribute is greater than this one as per sequential comparison of internal pointers.

        /// @fn Attribute::operator>(const Attribute& r) const;
        /// @brief Compares the internal values to check for inequality.
        /// @param r The other @ref Attribute this is being compared to.
        /// @details Many of the internal values are pointers, and it is the addresses of these that are being compared.
        /// @return Returns True if the other @ref Attribute is less than this one as per sequential comparison of internal pointers.

        /// @fn Attribute::operator<=(const Attribute& r) const;
        /// @brief Compares the internal values to check for inequality.
        /// @param r The other @ref Attribute this is being compared to.
        /// @details Many of the internal values are pointers, and it is the addresses of these that are being compared.
        /// @return Returns True if the other @ref Attribute is greater than or equal to this one as per sequential comparison of internal pointers.

        /// @fn Attribute::operator>=(const Attribute& r) const;
        /// @brief Compares the internal values to check for inequality.
        /// @param r The other @ref Attribute this is being compared to.
        /// @details Many of the internal values are pointers, and it is the addresses of these that are being compared.
        /// @return Returns True if the other @ref Attribute is less than or equal to this one as per sequential comparison of internal pointers.

        /// @fn Attribute::Empty() const;
        /// @brief Is this storing anything at all?
        /// @return Returns True if this @ref Attribute is storing nothing. False if it is storing anything.

        /// @fn Attribute::Name() const;
        /// @brief Get the name of this @ref Attribute.
        /// @return Returns A pointer to a const c-style array of the the character type (usually char or wchar_t) containing the name.
        /// @warning returns "" if attribute is empty.

        /// @fn Attribute::Value() const;
        /// @brief Get the Value of this @ref Attribute.
        /// @return Returns A pointer to a const c-style array of the the character type (usually char or wchar_t) containing the value.
        /// @warning returns "" if attribute is empty.

        /// @fn Attribute::AsInt() const;
        /// @brief Attempts to convert the value of the attribute to an int and returns the results.
        /// @return If the value of this attribute can be convert to an int by reading the character and interpretting them a number, that numberis returned. Returns 0 on failure.
        /// @todo Update Attribute::AsInt() to check errno and throw exceptions were appropriate, and throw a exception on failure instead of producing a valid return value.
        /// @warning This may silently fail if the value of the attribute exceeds the maximum value that can be stored in and int. Check "errno" and see if it is set to "ERANGE" to test for this condition.

        /// @fn Attribute::AsUint() const;
        /// @brief Attempts to convert the value of the attribute to an unsigned int and returns the results.
        /// @return If the value of this attribute can be convert to an unsigned int by reading the character and interpretting them a number, that numberis returned. Returns 0 on failure.
        /// @todo Update Attribute::AsUint() to check errno and throw exceptions were appropriate, and throw a exception on failure instead of producing a valid return value.
        /// @warning This may silently fail if the value of the attribute exceeds the maximum value that can be stored in and int. Check "errno" and see if it is set to "ERANGE" to test for this condition.

        /// @fn Attribute::AsDouble() const;
        /// @brief Attempts to convert the value of the attribute to a double and returns the results.
        /// @return If the value of this attribute can be convert to a double by reading the character and interpretting them a number, that numberis returned. Returns 0 on failure.
        /// @todo Update Attribute::AsDouble() to check errno and throw exceptions were appropriate, and throw a exception on failure instead of producing a valid return value.
        /// @warning This may silently fail if the value of the attribute exceeds the maximum value that can be stored in and double. Check "errno" and see if it is set to "ERANGE" to test for this condition.

        /// @fn Attribute::AsFloat() const;
        /// @brief Attempts to convert the value of the attribute to a float and returns the results.
        /// @return If the value of this attribute can be convert to a float by reading the character and interpretting them a number, that numberis returned. Returns 0 on failure.
        /// @todo Update Attribute::AsFloat() to check errno and throw exceptions were appropriate, and throw a exception on failure instead of producing a valid return value.
        /// @warning This may silently fail if the value of the attribute exceeds the maximum value that can be stored in and float. Check "errno" and see if it is set to "ERANGE" to test for this condition.

        /// @fn Attribute::AsReal() const;
        /// @brief Attempts to convert the value of the attribute to a Real and returns the results.
        /// @return If the value of this attribute can be converted to a Real by reading the character
        /// and interpretting them a number, that number is returned. Returns 0 on failure.
        /// @exception This can throw exception in certain overflow conditions

        /// @fn Attribute::AsWhole() const;
        /// @brief Attempts to convert the value of the attribute to a Whole and returns the results.
        /// @return If the value of this attribute can be converted to a Whole by reading the character
        /// and interpretting them a number, that number is returned. Returns 0 on failure.
        /// @exception This can throw exception in certain overflow conditions

        /// @fn Attribute::AsInteger() const;
        /// @brief Attempts to convert the value of the attribute to a Integer and returns the results.
        /// @return If the value of this attribute can be converted to a Integer by reading the character
        /// and interpretting them a number, that number is returned. Returns 0 on failure.
        /// @exception This can throw exception in certain overflow conditions

        /// @fn Attribute::AsString() const;
        /// @brief Attempts to convert the value of the attribute to a String and returns the results.
        /// @return If the value of this attribute can be converted to a Real by reading the character
        /// and interpretting them a number, that number is returned. Returns 0 on failure.
        /// @exception This can throw exception in certain overflow conditions

        /// @fn Attribute::AsBool() const;
        /// @brief Attempts to convert the value of the attribute to a float and returns the results.
        /// @return Value as bool (returns true if first character is in '1tTyY' set), or false if attribute is empty

		/// @fn Attribute::SetName(const char_t* rhs);
		/// @brief Set the name of this attribute.
		/// @param rhs The new name.
		/// @return True if successful, returns false if Attribute is empty or there is not enough memory.
		/// @todo update this to make the error return code redudant and use an exception instead.

		/// @fn Attribute::operator=(const char_t* rhs);
		/// @param rhs The new value as an c-style string.
		/// @brief The same as @ref Attribute::SetValue(const char_t* rhs); without the error return
		/// @return An reference to this attribute.

		/// @fn Attribute::operator=(int rhs);
		/// @param rhs The new value as an int.
		/// @brief The same as @ref Attribute::SetValue(); without the error return.
		/// @return An reference to this attribute.

		/// @fn Attribute::operator=(unsigned int rhs);
		/// @param rhs The new value as an unsigned int.
		/// @brief The same as @ref Attribute::SetValue(); without the error return.
		/// @return An reference to this attribute.

		/// @fn Attribute::operator=(double rhs);
		/// @brief The same as @ref Attribute::SetValue(); without the error return.
		/// @param rhs The new value as a double.
		/// @return An reference to this attribute.

		/// @fn Attribute::operator=(bool rhs);
		/// @brief The same as @ref Attribute::SetValue(); without the error return.
		/// @param rhs This with be interpretted, then converted to "true" or "false" and used as the new value.
		/// @return An reference to this attribute.

		/// @fn Attribute::GetNextAttribute() const;
		/// @brief Get the next attribute.
		/// @details This will get the next sibling attribute. That is, another Attribute on the same node as this attribute. This is internally a circular linked list, so once you reach the end, you simply be given the first node. If this attribute is empty this will return a empty attribute.
		/// @return Either the next attribute or if this attribute is empty an empty attribute.

		/// @fn Attribute::GetPreviousAttribute() const;
		/// @brief Get the previous attribute.
		/// @details This will get the previous sibling attribute. That is, another Attribute on the same node as this attribute. This is internally a circular linked list, so once you reach the beginning, you simply be given the last node. If this attribute is empty this will return a empty attribute.
		/// @return Either the previous attribute or if this attribute is empty an empty attribute.

		// Get hash Value (unique for handles to the same object)
		/// @fn Attribute::HashValue() const;
		/// @brief Get a unique identifying value for the Attribute this represents
		/// @return A size_t that is unique per Attribute that an attribute could represent.

		/// @fn Attribute::InternalObject() const;
		/// @brief Retrieve a pointer to the internal data.
		/// @return A void pointer to the internal data.
		/// @internal

        /// @fn operator&&(const Attribute& lhs, bool rhs);
        /// @brief Used to work around a Borland c++ issue casting @ref Attribute class instances to boolean values.
        /// @param lhs Left Hand Side of the operator.
        /// @param rhs Right Hand Side of the operator.
        /// @return A bool that has the correct value for a && operation.

        /// @fn operator||(const Attribute& lhs, bool rhs);
        /// @brief Used to work around a Borland c++ issue casting @ref Attribute class instances to boolean values.
        /// @param lhs Left Hand Side of the operator.
        /// @param rhs Right Hand Side of the operator.
        /// @return A bool that has the correct value for a || operation.



        ///////////////////////////////////////////////////////////////////////////////
        /// @class Node
        /// @brief A light-weight handle for manipulating nodes in DOM tree

        /// @var Node::_GetRoot
        /// @internal
        /// @brief Stores pointers to the Node data and some metadata.

        /// @fn Node::Node()
        /// @brief Default constructor. Constructs an empty node.

        /// @fn Node::operator!() const
        /// @brief Used to convert this node the opposite of it's normal boolean value
        /// @details This is described in the PugiXML source a a workaround for a borland c++ issue.
        /// @return Returns false if the internal pointer NodeStruct is set and true otherwise.

        /// @fn Node::operator==(const Node& r) const;
        /// @brief Compares the internal values to check equality.
        /// @param r The other @ref Node this is being compared to.
        /// @details Many of the internal values are pointers, and it is the addresses of these that are being compared.
        /// @return Returns true if all the internal values match between this and the other Node.

        /// @fn Node::operator!=(const Node& r) const;
        /// @brief Compares the internal values to check inequality.
        /// @param r The other @ref Node this is being compared to.
        /// @details Many of the internal values are pointers, and it is the addresses of these that are being compared.
        /// @return Returns true if any of the internal values don't match between this and the other @ref Node.

        /// @fn Node::operator<(const Node& r) const;
        /// @brief Compares the internal values to check for inequality.
        /// @param r The other @ref Node this is being compared to.
        /// @details Many of the internal values are pointers, and it is the addresses of these that are being compared.
        /// @return Returns True if the other @ref Node is greater than this one as per sequential comparison of internal pointers.

        /// @fn Node::operator>(const Node& r) const;
        /// @brief Compares the internal values to check for inequality.
        /// @param r The other @ref Node this is being compared to.
        /// @details Many of the internal values are pointers, and it is the addresses of these that are being compared.
        /// @return Returns True if the other @ref Node is less than this one as per sequential comparison of internal pointers.

        /// @fn Node::operator<=(const Node& r) const;
        /// @brief Compares the internal values to check for inequality.
        /// @param r The other @ref Node this is being compared to.
        /// @details Many of the internal values are pointers, and it is the addresses of these that are being compared.
        /// @return Returns True if the other @ref Node is greater than or equal to this one as per sequential comparison of internal pointers.

        /// @fn Node::operator>=(const Node& r) const;
        /// @brief Compares the internal values to check for inequality.
        /// @param r The other @ref Node this is being compared to.
        /// @details Many of the internal values are pointers, and it is the addresses of these that are being compared.
        /// @return Returns True if the other @ref Node is less than or equal to this one as per sequential comparison of internal pointers.

        /// @fn Node::Empty() const;
        /// @brief Is this storing anything at all?
        /// @return Returns True if this @ref Node is storing nothing. False if it is storing anything.

        /// @fn Node::Type() const;
        /// @brief Identify what kind of Node this is.
        /// @return A @ref NodeType identifying this Node, or o/NULL if this Node is empty.

        /// @fn Node::Name() const;
        /// @brief Get the name of this @ref Node.
        /// @return Returns A pointer to a const c-style array of the the character type (usually char or wchar_t) containing the name.
        /// @warning returns "" if Node is empty.

        /// @fn Node::Value() const;
        /// @brief Get the Value of this @ref Node.
        /// @return Returns A pointer to a const c-style array of the the character type (usually char or wchar_t) containing the value.
        /// @warning returns "" if Node is empty.

		/// @fn Node::GetFirstAttribute() const;
		/// @brief Get the First Attribute in this Node.
		/// @return This attempts to return the First @ref Attribute in this node, if it cannot it returns an empty @ref Attribute.

        /// @fn Node::GetLastAttribute() const;
        /// @brief Get the Last Attribute in this Node.
        /// @return This attempts to return the Last @ref Attribute in this node, if it cannot it returns an empty @ref Attribute.

		/// @fn Node::GetFirstChild() const;
		/// @brief Get the first child Node of this Node.
		/// @return Returns the First child node if it exists, otherwise it return an empty node.

		/// @fn Node::GetLastChild() const;
		/// @brief Get the last child Node of this Node.
		/// @return Returns the last child node if it exists, otherwise it return an empty node.

        /// @fn Node::GetNextSibling() const;
        /// @brief Attempt to retrieve the next sibling of this Node.
        /// @details A sibling of a Node is another Node that shares the same parent. If this is and the sibling nodes are valid, this retrieves that Node, otherwise this return an empty Node.
        /// @return A Node that represents a sibling, or an empty Node on failure.

        /// @fn Node::GetParent() const;
        /// @brief Attempt to retrieve the parent of this Node.
        /// @return A Node that represents the parent Node, or an empty Node on failure.

        /// @fn Node::GetRoot() const;
        /// @brief Attempt to retrieve the root Node, or the most base Node containing this Node.
        /// @return A Node that represents the root of the XML document, or an empty Node on failure. If there are multiple roots this attempts to retrieve the appropriate one.

        /// @fn Node::GetChild(const char_t* Name) const;
        /// @brief Attempt to get a child Node with a given name.
        /// @param Name The name of the desired child Node.
        /// @return A Node that represents the first desired child, or an empty Node on failure.

        /// @fn Node::GetAttribute(const char_t* Name) const;
        /// @brief Attempt to get an Attribute on this Node with a given name.
        /// @param Name The name of the desired Attribute.
        /// @return An Attribute that represents the first matching Attribute, or an empty Attribute on failure.

        /// @fn Node::ChildValue() const;
        /// @brief Retrieve the value of this(or a child's) Nodes PCDATA child Node
        /// @details If this node represents "<node>Some text in the PCDATA field, that is actually represent by a node</node>", then this would return "Some text in the PCDATA field, that is actually represent by a node". This will iterate through child Nodes from until it finds a PCDATA node or fails
        /// @return This will return the Value of the first available PCDATA node.

        /// @fn Node::ChildValue(const char_t* Name) const;
        /// @brief Get the PCDATA of a given child. The same a calling "GetChild(Name).ChildValue()".
        /// @param Name The Name of the desired child node.
        /// @return This will return the Value of the first available matching PCDATA node.

        /// @fn Node::SetName(const char_t* rhs);
        /// @brief Set the name of this Node.
        /// @param rhs The desired name of the Node.
        /// @return True if successful, returns false if Node cannot store a name or there is not enough memory.

        /// @fn Node::PrependAttribute(const char_t* Name);
        /// @brief Creates an Attribute and puts it at the begining of this Nodes attributes
        /// @param Name The name of the New attribute to be created
        /// @details This attempts to create an Attribute and stick it at the beginning of the list of attributes on the current
        /// Node. This will fail and return an Empty Attribute if this Node is neither an Element nor a Declaration. This will
        /// fail and return an empty attribute if this Node is empty.
        /// @return The created Attribute or an empty Attribute on Failure.

        /// @fn Node::InsertAttributeAfter(const char_t* Name, const Attribute& attr);
        /// @brief Creates an Attribute and puts it into the list of this Nodes attributes.
        /// @param Name The name of the New attribute to be created
        /// @param attr An Attribute that represents an Attribute on this Node, and is just before where you want the new Attribute.
        /// @details This attempts to create an Attribute and stick it in the list of attributes, Just after another Attribute, on the current
        /// Node. This will fail and return an Empty Attribute if this Node is neither an Element nor a Declaration. This will
        /// fail and return an empty attribute if this Node is empty.
        /// @return The created Attribute or an empty Attribute on Failure.

        /// @fn Node::InsertAttributeBefore(const char_t* Name, const Attribute& attr);
        /// @brief Creates an Attribute and puts it into the list of this Nodes attributes.
        /// @param Name The name of the New attribute to be created
        /// @param attr An Attribute that represents an Attribute on this Node, and is just after where you want the new Attribute.
        /// @details This attempts to create an Attribute and stick it in the list of attributes, Just before another Attribute, on the current
        /// Node. This will fail and return an Empty Attribute if this Node is neither an Element nor a Declaration. This will
        /// fail and return an empty attribute if this Node is empty.
        /// @return The created Attribute or an empty Attribute on Failure.

        /// @fn Node::InsertCopyBefore(const Attribute& proto, const Attribute& attr);
        /// @brief Copies an Attribute and puts the copy into the list of this Nodes attributes.
        /// @param proto The attribute to be copied.
        /// @param attr An Attribute that represents an Attribute on this Node, and is just after where you want the new copy of proto.
        /// @details This attempts to create a copy of an attribute Attribute and stick it in the middle of the list of attributes, just before a selected attribute, on the current
        /// Node. This will fail and return an Empty Attribute if this Node is neither an Element nor a Declaration. This will
        /// fail and return an empty attribute if this Node is empty.
        /// @return The created Attribute or an empty Attribute on Failure.

		/// @fn Node::AppendChild(NodeType Type = NodeElement);
		/// @brief Creates a Node and makes it a child of this one.
		/// @param Type The NodeType of the Node to be added to list of child Nodes.
		/// @return A Node representing the freshly added Node, or an empty Node if there was an error.
		/// @todo Not all nodes can be added to other nodes, we need to figure it out and put it here.

        /// @fn Node::PrependChild(NodeType Type = NodeElement);
        /// @brief Creates a Node and makes it a child of this one, and puts at the beginning of the Child Nodes.
        /// @param Type The NodeType of the Node to be added to the beginning list of child Nodes.
        /// @return A Node representing the freshly added Node, or an empty Node if there was an error.
		/// @todo Not all nodes can be added to other nodes, we need to figure it out and put it here.

        /// @fn Node::InsertChildAfter(NodeType Type, const Node& node);
        /// @brief Creates a Node and makes it a child of this one, and puts at the middle of the Child Nodes.
        /// @param Type The NodeType of the Node to be added, just after another specific node.
        /// @param node The specific node to add the new one after.
        /// @return A Node representing the freshly added Node, or an empty Node if there was an error.
		/// @todo Not all nodes can be added to other nodes, we need to figure it out and put it here.

		/// @fn Node::InsertChildBefore(NodeType Type, const Node& node);
		/// @brief Creates a Node and makes it a child of this one, and puts at the middle of the Child Nodes.
		/// @param Type The NodeType of the Node to be added, just before another specific node.
        /// @param node The specific node to add the new one before.
        /// @return A Node representing the freshly added Node, or an empty Node if there was an error.
		/// @todo Not all nodes can be added to other nodes, we need to figure it out and put it here.

        /// @fn Node::AppendChild(const char_t* Name);
        /// @brief Creates an element Node as a child of this Node, with the given name.
        /// @param Name The name of the Node to be created.
        /// @details Calls @ref Node::AppendChild(NodeType); using NodeElement as the NodeType.
        /// @return The desired Node on success, an empty Node on failure.

		/// @fn Node::PrependChild(const char_t* Name);
		/// @brief Creates an element Node as a child of this Node, with the given name at the beginning of the children
		/// @param Name The name of the Node to be created.
        /// @details Calls @ref Node::PrependChild(NodeType); using NodeElement as the NodeType.
        /// @return The desired Node on success, an empty Node on failure.

		/// @fn Node::InsertChildBefore(const char_t* Name, const Node& node);
		/// @brief Creates an element Node as a child of this Node, with the given name at the middle of the children
		/// @param Name The name of the Node to be created.
		/// @param Node The node just after were the Create node is to be placed.
        /// @details Calls @ref Node::InsertChildBefore(NodeType, Node); using NodeElement as the NodeType.
        /// @return The desired Node on success, an empty Node on failure.

		/// @fn Node::AppendCopy(const Node& proto);
		/// @brief Copies a Node and puts the copy at the end of the list of this Nodes Childrem.
        /// @param proto The Node to be copied. If this is emptry, no work is performed.
        /// @return The copied Node on success, an empty Node on failure.

		/// @fn Node::InsertCopyAfter(const Node& proto, const Node& node);
		/// @brief Copies a Node and puts the copy in the middle the list of this Nodes Childrem.
        /// @param proto The Node to be copied. If this is emptry, no work is performed.
        /// @param node The Node just before the desired place in the list of children to insert the copied node.
        /// @return The copied Node on success, an empty Node on failure.

		/// @fn Node::InsertCopyBefore(const Node& proto, const Node& node);
		/// @brief Copies a Node and puts the copy in the middle the list of this Nodes Childrem.
        /// @param proto The Node to be copied. If this is emptry, no work is performed.
        /// @param node The Node just after the desired place in the list of children to insert the copied node.
        /// @return The copied Node on success, an empty Node on failure.

		/// @fn Node::RemoveAttribute(const Attribute& a);
		/// @brief Remove specified Attribute.
		/// @param a The Attribute to look for. If the given Attribute doesn't belong to this Node then this will fail
		/// @return True if the removal was successful, false otherwise

		/// @fn Node::RemoveAttribute(const char_t* Name);
		/// @brief Remove Attribute as specified by name.
		/// @param Name The name of the Attribute to remove.
		/// @return True if the removal was successful, false otherwise.

		/// @fn Node::RemoveChild(const Node& n);
		/// @brief Remove specified child element.
		/// @param n The Node to look for. If the given Attribute doesn't belong to this Node then this will fail
		/// @return True if the removal was successful, false otherwise

        /// @fn Node::FindAttribute(Predicate pred) const
        /// @brief Search for an Attribute using a function to check each Attribute individually.
        /// @param Predicate a pointer to a function that accepts an Attribute, and returns bool.
        /// @details This iterates through each Attribute on this node, from begining to end and calls the Predicate function passing
        /// an Attribute to it. If the Predicate returns true the Node it was just passed is returned. If Precdicate never returns
        /// true, it is called on every Node and a blank Node is returned. The Predicate is never called with a null value.
        /// @return This returns the first Attribute that causes Predicate to return true.

        /// @fn Node::FindChild(Predicate pred) const
        /// @brief Search for an child ( only direct children ) Node using a function to check each Node individually.
        /// @param Predicate a pointer to a function that accepts an Node, and returns bool.
        /// @details This iterates through all immediate children of this Node and calls the Predicate function passing a Node to it. If
        /// the Predicate returns true the Node it was just passed is returned. If Predicate never returns true, it is called
        /// on every Node and a blank Node is returned. The Predicate is never called with a null value.
        /// @return This returns the first Node that causes Predicate to return true.

        /// @fn Node::FindNode(Predicate pred) const
        /// @brief Search for any Node descended from this Node using a function to check each Node individually.
        /// @param Predicate a pointer to a function that accepts an Node, and returns bool.
        /// @details This iterates through all children of this Node, and their children ( and so on), and calls the Predicate function
        /// passing each Node to it. This iterates through all Nodes using a depth first algorithm. If the Predicate returns true the
        /// Node it was just passed is returned. If Predicate never returns true, it is called on every Node and a blank Node is
        /// returned. The Predicate is never called with a null value.
        /// @return This returns the first Node that causes Predicate to return true.

        /// @fn Node::FindChildbyAttribute(const char_t* Name, const char_t* AttrName, const char_t* AttrValue) const;
        /// @brief Find a Node by an Attribute it has.
        /// @param Name The name of the matching Node.
        /// @param AttrName The name of the matching Attribute.
        /// @param AttrValue The value of the matching Attribute.
        /// @details Any Null pointers instead of character arrays passed in will cause undefined behavior. All Matching is Case sensitive.

		/// @fn Node::FindChildbyAttribute(const char_t* AttrName, const char_t* AttrValue) const;
		/// @brief Find a Node by an Attribute it has.
        /// @param AttrName The name of the matching Attribute.
        /// @param AttrValue The value of the matching Attribute.
        /// @details Any Null pointers instead of character arrays passed in will cause undefined behavior. All Matching is Case sensitive.

        /// @fn Node::Path(char_t delimiter = '/') const;
        /// @brief Get the absolute path to this Node
        /// @param delimiter The character to use as a pathname separator, this defaults to '/'.
        /// @return A String containing an path

        /// @fn Node::FirstElementByPath(const char_t* Path, char_t delimiter = '/') const;
        /// @brief Search for a node by Path consisting of node names and . or .. elements.
        /// @todo Investigate this more deeply.
        /// @param Path The path to search for.
        /// @param delimiter The character to use as a pathname separator, this defaults to '/'.
        /// @return The matching Node, of an empty Node on failure.

        /// @fn Node::Traverse(TreeWalker& walker);
        /// @brief Perform sophisticated (or whatever) algorithms on this and all descendant Nodes in the XML tree.
        /// @param walker Any class that fully implement xml::Treewalker. This is where the algorithm to be run is located.
        /// @return True if every descendant Node of this Node was iterated through, false if it didn't go through every Node.
        /// @see xml::TreeWalker

        /// @fn Node::FindSingleNode(const char_t* query, XPathVariableSet* variables = 0) const;
        /// @brief Select single node by evaluating an XPath query. Returns first node from the resulting node set.
        /// @param query The XPath query as a c-string to be evaluated.
        /// @param XPathVariableSet undocumented.
        /// @return XPathNode The first matching XPath node.

        /// @fn Node::FindSingleNode(const XPathQuery& query) const;
        /// @brief Select single node by evaluating an XPath query. Returns first node from the resulting node set.
        /// @param query The XPath query XPathQuery class instance.
        /// @return XPathNode The first matching XPath node.

        /// @typedef Node::iterator
        /// @brief An iterator for child Nodes

        /// @fn Node::begin() const;
        /// @brief Get a Child node iterator that references the first child Node.
        /// @return A Node::Iterator that reference the first child Node.

        /// @fn Node::end() const;
        /// @brief Get a Child node iterator that references one past the last child Node.
        /// @return A Node::Iterator that reference the last child Node.

        /// @typedef Node::attribute_iterator
        /// @brief An iterator for Attribute members on this Node

        /// @fn Node::attributes_begin() const;
        /// @brief Get an Attribute iterator that references the first Attribute on this Node.
        /// @return A Node::Iterator that reference the first child node.

        /// @fn Node::attributes_end() const;
        /// @brief Get an Attribute iterator that references the one past the last Attribute on this Node.
        /// @return A Node::Iterator that reference the last Attribute on this Node.

		/// @fn Node::OffSetDebug() const;
		/// @internal
		/// @brief Get node Offset in parsed file/string (in char_t units) for debugging purposes
        /// @return ptrdiff_t

		/// @fn Node::HashValue() const;
		/// @internal
		/// @brief Get hash Value (unique for handles to the same object)
		/// @return A size_t that uniquely identifies this node.

		/// @fn Node::InternalObject() const;
		/// @internal
		/// @brief Get internal pointer
		/// @return A NodeStruct* that points to the internal data of this Node

        //////////////////////////////////////////////////////////////////////////////
        /// @class NodeStruct
        /// @internal
        /// @brief The internal data storage structure used in a Node.
        /// @warning Not part of the API, subject to change without warning.

        //////////////////////////////////////////////////////////////////////////////
        /// @class AttributeStruct
        /// @internal
        /// @brief The internal data storage structure used in an Attribute.
        /// @warning Not part of the API, subject to change without warning.

        //////////////////////////////////////////////////////////////////////////////
        /// @class NodeIterator
        /// @brief Child node iterator (a bidirectional iterator over a collection of Node)
        /// @details Node::begin() and Node::attributes_begin() return iterators that point to the first node/attribute, respectively; Node::end() and Node::attributes_end() return past-the-end iterator for node/attribute list, respectively - this iterator can't be dereferenced, but decrementing it results in an iterator pointing to the last element in the list (except for empty lists, where decrementing past-the-end iterator results in undefined behavior). Past-the-end iterator is commonly used as a termination value for iteration loops. If you want to get an iterator that points to an existing handle, you can construct the iterator with the handle as a single constructor argument, like so: xml_node_iterator(node). For xml_attribute_iterator, you'll have to provide both an attribute and its parent node.\n\n
        /// Node::begin() and Node::end() return equal iterators if called on null Node; such iterators can't be dereferenced. Node::attributes_begin() and Node::attributes_end() behave the same way. For correct iterator usage this means that child node/attribute collections of null nodes appear to be empty.\n\n
        /// Both types of iterators have bidirectional iterator semantics (i.e. they can be incremented and decremented, but efficient random access is not supported) and support all usual iterator operations - comparison, dereference, etc. The iterators are invalidated if the node/attribute objects they're pointing to are removed from the tree; adding nodes/attributes does not invalidate any iterators.

		/// @typedef NodeIterator::difference_type;
		/// @brief An Iterator trait

		/// @typedef NodeIterator::value_type;
		/// @brief An Iterator trait

		/// @typedef NodeIterator::pointer;
		/// @brief An Iterator trait

		/// @typedef NodeIterator::reference;
		/// @brief An Iterator trait

		/// @typedef NodeIterator::iterator_category;
		/// @brief An Iterator trait

        /// @fn NodeIterator::NodeIterator();
        /// @brief Default Constructor, makes a blank iterator

        /// @fn NodeIterator::NodeIterator(const Node& node);
        /// @brief Construct an iterator which points to the specified node
        /// @param node A Node that this iterator will point to.

        /// @fn NodeIterator::operator==(const NodeIterator& rhs) const;
        /// @brief Compares this NodeIterator to another NodeIterator for equality
        /// @param rhs The Right Hand Side NodeIterator
        /// @return True if the internal data stored in Node this NodeIterator refers to is the same as the metadata in the other NodeIterator's Node, false otherwise.

        /// @fn NodeIterator::operator!=(const NodeIterator& rhs) const;
        /// @brief Compares this NodeIterator to another NodeIterator for inequality
        /// @param rhs The Right Hand Side NodeIterator.
        /// @return False if the internal data stored in Node this NodeIterator refers to is the same as the metadata in the other NodeIterator's Node, True otherwise.

        /// @fn NodeIterator::operator*();
        /// @brief Deferences this Iterator
        /// @return a Node reference to the node pointed at by this NodeIterator.

        /// @fn NodeIterator::operator->();
        /// @brief Get the pointer the Node this points to.
        /// @return A pointer to the Node this NodeIterator references.

		/// @fn NodeIterator::operator++();
		/// @brief Increment the iterator to the next member of the container.
        /// @return Returns a const NodeIterator.

		/// @fn NodeIterator::operator++(int);
		/// @brief Increment the iterator to the next member of the container.
        /// @return Returns a NodeIterator.

		/// @fn NodeIterator::operator--();
		/// @brief Decrement the iterator to the next member of the container.
        /// @return Returns a const NodeIterator.

		/// @fn NodeIterator::operator--(int);
		/// @brief Decrement the iterator to the next member of the container.
        /// @return Returns a NodeIterator.

        //////////////////////////////////////////////////////////////////////////////
        /// @class AttributeIterator
        /// @brief Attribute iterator (a bidirectional iterator over a collection of Attribute).
        /// @see This behaves very similar to xml::NodeIterator

		/// @typedef AttributeIterator::difference_type;
		/// @brief An Iterator trait

		/// @typedef AttributeIterator::value_type;
		/// @brief An Iterator trait

		/// @typedef AttributeIterator::pointer;
		/// @brief An Iterator trait

		/// @typedef AttributeIterator::reference;
		/// @brief An Iterator trait

		/// @typedef AttributeIterator::iterator_category;
		/// @brief An Iterator trait

        /// @fn AttributeIterator::AttributeIterator();
        /// @brief Default Constructor, makes a blank iterator

        /// @fn AttributeIterator::AttributeIterator(const Attribute& attr, const Node& GetParent);
        /// @brief Construct an iterator which points to the specified node
        /// @param GetParent A Node that contains the Attribute this iterator will point to.
        /// @param attr The Attribute this iterator points to.

        /// @fn AttributeIterator::operator==(const AttributeIterator& rhs) const;
        /// @brief Compares this AttributeIterator to another AttributeIterator for equality
        /// @param rhs The Right Hand Side AttributeIterator
        /// @return True if the internal data stored in the Attribute this AttributeIterator refers to is the same as the metadata in the other AttributeIterator's Attribute, false otherwise.

        /// @fn AttributeIterator::operator!=(const AttributeIterator& rhs) const;
        /// @brief Compares this AttributeIterator to another AttributeIterator for inequality
        /// @param rhs The Right Hand Side AttributeIterator.
        /// @return False if the internal data stored in Node this AttributeIterator refers to is the same as the metadata in the other AttributeIterator's Attribute, True otherwise.

        /// @fn AttributeIterator::operator*();
        /// @brief Deferences this Iterator
        /// @return a Attribute reference to the Attribute pointed at by this AttributeIterator.

        /// @fn AttributeIterator::operator->();
        /// @brief Get the pointer the Attribute this points to.
        /// @return A pointer to the Attribute this AttributeIterator references.

		/// @fn AttributeIterator::operator++();
		/// @brief Increment the iterator to the next member of the container.
        /// @return Returns a const AttributeIterator.

		/// @fn AttributeIterator::operator++(int);
		/// @brief Increment the iterator to the next member of the container.
        /// @return Returns a AttributeIterator.

		/// @fn AttributeIterator::operator--();
		/// @brief Decrement the iterator to the next member of the container.
        /// @return Returns a const AttributeIterator.

		/// @fn AttributeIterator::operator--(int);
		/// @brief Decrement the iterator to the next member of the container.
        /// @return Returns a AttributeIterator.

        //////////////////////////////////////////////////////////////////////////////
        /// @class TreeWalker
        /// @brief Used to call a function for_each member of the subtree of nodes descended from a specific node.
        /// @details If you want to do a deep tree traversal, you'll either have to do it via a recursive function or some
        /// equivalent method or use a TreeWalker. This provides a helper for depth-first traversal of a subtree. In order
        /// to use it, you have to implement xml::TreeWalker interface and call xml::Node::Traverse() function. \n\n
        ///  * First, TreeWalker::begin() is called with traversal root as its argument.\n
        ///  * Then, TreeWalker::for_each() function is called for all nodes in the traversal subtree in depth first order, excluding the traversal root. Each Node is passed as an argument.\n
        ///  * Finally, TreeWalker::end() function is called with traversal root as its argument.\n\n
        /// If TreeWalker::begin(), TreeWalker::end() or any of the TreeWalker::for_each() calls return false, the traversal
        /// is terminated and false is returned as the traversal result; otherwise, the traversal results in true. Note that
        /// you don't have to override begin or end functions; their default implementations return true.\n\n
        /// You can get the node's depth relative to the traversal root at any point by calling TreeWalker::Depth() function.

        /// @fn TreeWalker::Depth() const;
        /// @brief How many descendants deep are we during traversal.
        /// @return This returns -1 if called from TreeWalker::begin() or TreeWalker::end(), and returns 0-based depth if called from for_each - depth is 0 for all children of the traversal root, 1 for all grandchildren, 2 for great-grandchildren and so on.

        /// @fn TreeWalker::TreeWalker();
        /// @brief Default constructor, initializes depth, and can do little else without a fully implemented treewalker.

        /// @fn TreeWalker::~TreeWalker();
        /// @brief Virtual deconstructor. Tears down a TreeWalker

        /// @fn TreeWalker::begin(Node& node);
        /// @brief Called on the root Node of the xml subtree when traversal begins.
        /// @detail By default this simply returns true, but is expected to be overridden with any desired behavior
        /// @return True by default. If it returns false, then traversal ends and the Node::Traverse() that was called is expected to return false.

        /// @fn TreeWalker::for_each(Node& node);
        /// @brief A Pure Virtual function that is expected to be implemented to create the desired behavior.
        /// @detail This is called on every Node that is traversed except the root node of the traversed subtree. Can be used to perform sophisticated searches
        /// of a portion of the xml document, alter the document on a large scale, gather statistics, or just about any other behavior that requires touching
        /// many nodes.
        /// @return if true Traversal is expected to continue, if false, then traversal ends and the Node::Traverse() that was called is expected to return false.

        /// @fn TreeWalker::end(Node& node);
        /// @brief Called on the root Node of the xml subtree when traversal ends.
        /// @detail By default this simply returns true, but is expected to be overridden with any desired behavior
        /// @return True by default. If it returns false, then traversal ends and the Node::Traverse() that was called is expected to return false.

        //////////////////////////////////////////////////////////////////////////////
        /// @class Document
        /// @brief The root node of any xml hierarchy is a Document Node
        /// @details This has all the same features as a Node and include a few features for saving, loading, streaming
        /// and to a limited degree managing the document declaration.

        /// @fn Document::Document();
        /// @brief Creates an empty document with just a root Node

        /// @fn Document::~Document()
        /// @brief Tears down a document, and incidentally invalidates all Node and Attribute handles to this document.

        /// @fn Document::Reset();
        /// @brief Removes all nodes, leaving the empty document.

        /// @fn Document::Reset(const Document& proto);
        /// @brief Removes all nodes, then copies the entire contents of the specified document
        /// @param proto The Document to copy.

        /// @fn Document::Load(std::basic_istream<char, std::char_traits<char> >& stream, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto);
        /// @brief Load XML from a stream.
        /// @param stream An std::istream which has xml text in it.
        /// @param options A bitset of parse options that should be set using the Parse variables. This Defaults to ParseDefault.
        /// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto
        /// @return A ParseResult that stores the the outcome of attempting to load the document.

        /// @fn Document::Load(std::basic_istream<wchar_t, std::char_traits<wchar_t> >& stream, unsigned int options = ParseDefault);
        /// @brief Load XML from a wide stream.
        /// @param stream An std::basic_istream which has xml wide character text in it.
        /// @param options A bitset of parse options that should be set using the Parse variables. This Defaults to ParseDefault.
        /// @return A ParseResult that stores the the outcome of attempting to load the document.

        /// @fn Document::Load(const char_t* contents, unsigned int options = ParseDefault);
        /// @brief Load XML from a Character array.
        /// @param contents A pointer to the Null terminated array of Characters.
        /// @param options A bitset of parse options that should be set using the Parse variables. This Defaults to ParseDefault.
        /// @return A ParseResult that stores the the outcome of attempting to load the document.


		/// @fn Document::LoadFile(const char* Path, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto);
		/// @brief Load document from file
		/// @param Path An c-style char array that contains the path and filename of the xml document to load.
		/// @param options A bitset of parse options that should be set using the Parse variables. This Defaults to ParseDefault.
        /// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto
        /// @return A ParseResult that stores the the outcome of attempting to load the document.

		/// @fn Document::LoadFile(const wchar_t* Path, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto);
		/// @brief Load document from file
		/// @param Path An c-style wide char array that contains the path and filename of the xml document to load.
        /// @param options A bitset of parse options that should be set using the Parse variables. This Defaults to ParseDefault.
        /// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto
        /// @return A ParseResult that stores the the outcome of attempting to load the document.

		/// @fn Document::LoadBufferInplace(void* contents, size_t size, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto);
		/// @brief Load document from buffer, using the buffer for in-place parsing (the buffer is modified and used for storage of document data).
		/// @details You should ensure that buffer data will persist throughout the document's lifetime, and free the buffer memory manually once document is destroyed.
        /// @param contents A pointer to buffer containing the xml document to be parsed, that must remain for the lifecycle of the xml::Document.
        /// @param size The size of the buffer.
        /// @param options A bitset of parse options that should be set using the Parse variables. This Defaults to ParseDefault.
        /// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto
        /// @return A ParseResult that stores the the outcome of attempting to load the document.

        /// @fn Document::LoadBuffer(const void* contents, size_t size, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto);
        /// @brief Load document from buffer. Copies/converts the buffer, so it may be deleted or changed after the function returns.
        /// @param contents A pointer to buffer containing the xml document to be parsed, that will remain unchanged.
        /// @param size The size of the buffer.
        /// @param options A bitset of parse options that should be set using the Parse variables. This Defaults to ParseDefault.
        /// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto
        /// @return A ParseResult that stores the the outcome of attempting to load the document.

		/// @fn Document::LoadBufferInplaceOwn(void* contents, size_t size, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto);
		/// @brief Load document from buffer, using the buffer for in-place parsing (the buffer is modified and used for storage of document data).
		/// @details You should allocate the buffer with pugixml allocation function; xml::Document will free the buffer when it is no longer needed (you can't use it anymore).
        /// @param contents A pointer to buffer containing the xml document to be parsed.
        /// @param size The size of the buffer.
        /// @param options A bitset of parse options that should be set using the Parse variables. This Defaults to ParseDefault.
        /// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto.
        /// @return A ParseResult that stores the the outcome of attempting to load the document.

        /// @fn Document::Save(Writer& WriterInstance, const char_t* indent = XML_TEXT("\t"), unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto) const;
        /// @brief Save XML document to WriterInstance.
		/// @param WriterInstance The Writer that will be used to output the xml text.
		/// @param indent The Character(s) used to represent a tab in the output, this defaults to one tab character.
		/// @param flags The output format flags, this is a bitfield that defaults to xml::FormatDefault.
        /// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto.

		/// @fn Document::SaveFile(const char* Path, const char_t* indent = XML_TEXT("\t"), unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto) const;
		/// @brief Save XML to file.
		/// @param Path A c-style array of chars that contain the filename (and any path) of the file to be output.
		/// @param indent The Character(s) used to represent a tab in the output, this defaults to one tab character.
		/// @param flags The output format flags, this is a bitfield that defaults to xml::FormatDefault.
        /// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto.
        /// @return False if the target file could not be opened for writing

		/// @fn Document::SaveFile(const wchar_t* Path, const char_t* indent = XML_TEXT("\t"), unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto) const;
		/// @brief Save XML to file.
		/// @param Path A c-style array of wide chars that contain the filename (and any path) of the file to be output.
		/// @param indent The Character(s) used to represent a tab in the output, this defaults to one tab character.
		/// @param flags The output format flags, this is a bitfield that defaults to xml::FormatDefault.
        /// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto.
        /// @return False if the target file could not be opened for writing

        ///////////////////////////////////////////////////////////////////////////////
        /// @enum XPathValueType
        /// @brief XPathQuery return type

		/// @var XPathTypeNone
		/// @brief Unknown Type (query failed to compile)

		/// @var XPathTypeNodeSet
		/// @brief Node set (XPathNodeSet)

		/// @var XPathTypeNumber
		/// @brief Number This corresponds to a double or Real.

		/// @var XPathTypeString
		/// @brief Corresponds to the String type.

		/// @var XPathTypeBoolean
		/// @brief A Boolean value.

		///////////////////////////////////////////////////////////////////////////////
        /// @struct XPathParseResult
        /// @brief XPath parsing result

        /// @var XPathParseResult::Offset
        /// @brief Last parsed Offset (in Character units from string start)

        /// @var XPathParseResult::error
        /// @brief Error message (0 if no error).

        ///////////////////////////////////////////////////////////////////////////////
        /// @class XPathVariable
        /// @brief A single XPath variable
        /// @details This is intended to be used as a single member of an XPathVariableSet, and for moving data into and out of XPathQueries.

        /// @fn XPathVariable::XPathVariable();
        /// @brief Protected Default constructor.

        /// @var XPathVariable::_type
        /// @brief What kind of data does this variable store

        /// @var XPathVariable::_next
        /// @brief The next variable in the variable set. I think, and I am not certain, that this is a circularly linked list.

        /// @fn XPathVariable::XPathVariable(const XPathVariable&);
        /// @brief Protected Copy Constructor, used to force noncopyable semantics

        /// @fn XPathVariable::operator=(const XPathVariable&);
        /// @brief Protected assignment operator, used to force noncopyable semantics
        /// @return Shouldn't be used, not implemented.

		/// @fn XPathVariable::GetBoolean() const;
		/// @brief Get this as a bool.
		/// @details Get variable Value; no Type conversion is performed, default Value (false, NaN, empty string, empty node set) is returned on Type mismatch error
		/// @return A This as a bool, without conversion.

		/// @fn XPathVariable::GetNumber() const;
		/// @brief Get this as a double.
		/// @details Get variable Value; no Type conversion is performed, default Value (false, NaN, empty string, empty node set) is returned on Type mismatch error
        /// @return A This as a double, without conversion.

		/// @fn XPathVariable::GetString() const;
		/// @brief Get this as a c-string.
		/// @details Get variable Value; no Type conversion is performed, default Value (false, NaN, empty string, empty node set) is returned on Type mismatch error
		/// @return A This as a c-string of char_t, without conversion.

		/// @fn XPathVariable::GetNodeSet() const;
		/// @brief Get this as a XPathNodeSet.
		/// @details Get variable Value; no Type conversion is performed, default Value (false, NaN, empty string, empty node set) is returned on Type mismatch error
        /// @return A This as an XPathNodeSet, without conversion.

        /// @fn XPathVariable::Set(bool Value);
        /// @brief Set variable Value; no Type conversion is performed.
        /// @param Value The value to attempt to put into this.
		/// @return True is return, false is returned on Type mismatch error.

        ///////////////////////////////////////////////////////////////////////////////
        /// @class XPathVariableSet
        /// @brief A set of XPath variables.

        /// @fn XPathVariableSet::XPathVariableSet();
        /// @brief Default Constructor, Blanks any XPathVariable it contains.

        /// @fn XPathVariableSet::~XPathVariableSet();
        /// @brief Default Deconstructor, Deletes any XPathVariable it contains.

        /// @fn XPathVariableSet::Add(const char_t* Name, XPathValueType Type);
        /// @brief Add a new variable or get the existing one, if the Types match
        /// @return A pointer to the XPathVariable you referenced or just created.

        /// @fn XPathVariableSet::Set(const char_t* Name, bool Value);
        /// @brief Set contained variable Value; no Type conversion is performed.
        /// @param Name The name of variable to change.
        /// @param Value The value to attempt to put into the named variable.
		/// @return True is return, false is returned if there is no such variable or on Type mismatch error.

   		/// @fn XPathVariableSet::Get(const char_t* Name);
   		/// @brief Get the named XPathVariable.
   		/// @param Name The name of the XPathVariable you want.
        /// @return A pointer to the specified XPathVariable.

		/// @fn XPathVariableSet::Get(const char_t* Name) const;
		/// @brief Get the named XPathVariable.
   		/// @param Name The name of the XPathVariable you want.
        /// @return A pointer to the specified XPathVariable.

        ///////////////////////////////////////////////////////////////////////////////
        /// @class XPathQuery
        /// @brief A compiled XPath query object
        /// @details When you call select_nodes with an expression string as an argument, a query object is created behind the scenes. A query object represents a compiled XPath expression. Query objects can be needed in the following circumstances: \n
        /// - You can precompile expressions to query objects to save compilation time if it becomes an issue; \n
        /// - You can use query objects to evaluate XPath expressions which result in booleans, numbers or strings; \n
        /// - You can get the type of expression value via query object. \n \n
        /// Query objects correspond to xml::XPathQuery type. They are immutable and non-copyable: they are bound to the expression at creation time and can not be cloned. If you want to put query objects in a container, allocate them on heap via new operator and store pointers to xml::XPathQuery in the container. \n \n
        /// To evaluate an XPath expression there are a few EvaluatedType functions. According to XPath specification, value of any type can be converted to boolean, number or string value, but no type other than node set can be converted to node set. Because of this, XPathQuery::EvaluateBoolean(), XPathQuery::EvaluateNumber() and XPathQuery::EvaluateString() always return a result, but EvaluateNodeSet results in an error if the return type is not node set.

        /// @fn XPathQuery::ReturnType() const;
        /// @brief Get query expression return Type.
        /// @return A XPathValueType.

        /// @fn XPathQuery::EvaluateBoolean(const XPathNode& n) const;
        /// @brief Evaluate expression as boolean value in the specified context; performs Type conversion if necessary.
        /// @param n The XPathNode that will serve as the context for the query.
		/// @throw If XML_NO_EXCEPTIONS is not defined (by default it is not defined), throws std::bad_alloc on out of memory errors.
		/// @return A bool result of evaluating the expression.

        /// @fn XPathQuery::Result() const;
        /// @brief Get parsing Result (used to get compilation errors when XML_NO_EXCEPTIONS is enabled)
		/// @return A const reference to an XPathParseResult.

        /// @fn XPathQuery::operator!() const;
        /// @brief Logical not operator, used a workaround for borland compiler.
        /// @return A bool that is the opposite of evaluatig this as a bool normally.

        ///////////////////////////////////////////////////////////////////////////////
        /// @class XPathException
        /// @brief Thrown in a variety of XPath only situations, to indicate type mismatch or other issues.

		/// @fn XPathException::what() const throw();
		/// @brief Get error message.
		/// @return A description of the error message as a c-style string.

		/// @fn XPathException::Result() const;
		/// @brief Get parse Result.
		/// @return The XPathParseResult used to create this exception.

        ///////////////////////////////////////////////////////////////////////////////
        /// @class XPathNode
        /// @brief An XPath node which can store handles to a xml::Node or an xml::Attribute.
        /// @details Because an XPath node can be either a xml::Node or an xml::Attribute, there is a special type, XPathNode,
        /// which is a discriminated union of these types. A value of this type contains two node handles, one of xml::Node type,
        /// and another one of xml::Attribute type; at most one of them can be non-null. The accessors to get these handles are
        /// available: XPathNode::GetNode() an XPathNode::GetAttribute() . \n \n
        /// XPath nodes can be null, in which case both accessors return null handles.

        /// @fn XPathNode::XPathNode();
		/// Default constructor; constructs empty XPath node

        /// @fn XPathNode::XPathNode(const Node& node);
        /// @brief Construct From a xml::Node.
        /// @param node The xml::Node this handle should reference.

		/// @fn XPathNode::GetNode() const;
		/// @brief Get the xml::Node this is referencing
		/// @return A valid xml::Node, or a null node if this doesn't reference a an xml::Node.

		/// @fn XPathNode::GetAttribute() const;
		/// @brief Get the xml::Attribute this is referencing
        /// @return A valid xml::Attribute, or a null node if this doesn't reference a an xml::Attribute.

		/// @fn XPathNode::GetParent() const;
		/// @brief Get the parent of the xml::Node or xml::Attribute this refers to.
        /// @return A valid xml::Node, or a null node if this doesn't reference a an xml::Node.

        /// @fn XPathNode::operator!() const;
        /// @brief Logical not operator, used a workaround for borland compiler.
        /// @return A bool that is the opposite of evaluatig this as a bool normally.

        ///////////////////////////////////////////////////////////////////////////////
        /// @class XPathNodeSet
        /// @brief A collection of nodes that an XPathQuery can work on.

        /// @enum XPathNodeSet::CollectionType
        /// @brief The different ways a collection may or may not be ordered.

        /// @var XPathNodeSet::TypeUnsorted
        /// @brief Not Ordered.

        /// @var XPathNodeSet::TypeSorted
        /// @brief In document Order.

        /// @var XPathNodeSet::TypeSortedReverse
        /// @brief In reverse document Order.

        /// @typedef XPathNodeSet::const_iterator;
        /// @brief An iterator trait. Const iterator for XPathNodes.

        /// @fn XPathNodeSet::XPathNodeSet();
        /// @brief Default constructor. Constructs empty set.

        /// @fn XPathNodeSet::XPathNodeSet(const_iterator begin, const_iterator end, CollectionType Type = TypeUnsorted);
        /// @param begin A const XPathNode iterator at the beginning of the set of nodes.
        /// @param end A const XPathNode iterator at the end of the set of nodes.
        /// @param Type What XPathNodeSet::CollectionType is being used, this defaults to XPathNodeSet::TypeUnsorted
        /// @brief Constructs a set from iterator range.
        /// @details Data is not checked for duplicates and is not sorted according to provided Type, so be careful.

		/// @fn XPathNodeSet::operator=(const XPathNodeSet& ns);
		/// @brief Assignment Operator.
		/// @param ns The XPathNodeSet to copy.
        /// @return A reference to the freshly assigned XPathNodeSet.

		/// @fn XPathNodeSet::Type() const;
		/// @brief Get collection Type.
		/// @return CollectionType

        /// @fn XPathNodeSet::operator[](size_t index) const;
        /// @brief Indexing operator.
        /// @param index A size_t indicating which XPathNode you would like to retrieve
        /// @return A const reference to the XPathNode you requested.
        /// @warning Out of bounds errors are checked using assert. Exceptions will not be thrown, during debugging out of bounds access will abort the termination and in production code out of bounds accesses will cause undefined behavior.

		/// @fn XPathNodeSet::begin() const;
		/// @brief Get Beginning iterator.
		/// @return A XPathNodeSet::const_iterator to the beginning of the collection.

		/// @fn XPathNodeSet::end() const;
		/// @brief Get Ending iterator.
        /// @return A XPathNodeSet::const_iterator to the end of the collection.

        /// @fn XPathNodeSet::sort(bool reverse = false);
        /// @brief Sort the collection in ascending/descending order by document order.
        /// @param reverse If true this sorts the collection in the opposite of document order.

        /// @fn XPathNodeSet::first() const;
        /// @brief Get first node in the collection by document order.
        /// @return The first node of the, in document order as an XPathNode.

        /// @fn XPathNodeSet::Empty() const;
        /// @brief Check if collection is empty.
        /// @return True if the document is empty, false otherwise.

        ///////////////////////////////////////////////////////////////////////////////
        // floaters

        /// @internal
        /// @fn AsUtf8(const wchar_t* str);
        /// @brief Convert a c-style string of wchar_t to std::string containing UTF8.
        /// @param str The string to convert
        /// @return A std::basic_string<char, std::char_traits<char>, std::allocator<char> > containing the converted data

        /// @internal
        /// @fn AsUtf8(const std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >& str);
        /// @brief Convert a std::wstring to a UTF8 std::string
        /// @param str The string to convert.
        /// @return A std::basic_string<char, std::char_traits<char>, std::allocator<char> > containing the converted data

        /// @internal
        /// @fn AsWide(const char* str);
        /// @brief Convert a Convert a c-style string to std::wstring containing native encoding (Usually UCS2 on windows and UTF32 on Linux/Mac).
        /// @param str The string to convert.
        /// @return A std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > containing the converted data

        /// @internal
        /// @fn AsWide(const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& str);
        /// @brief Convert a Convert a std::string to std::wstring containing native encoding (Usually UCS2 on windows and UTF32 on Linux/Mac).
        /// @param str The string to convert.
        /// @return A std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > containing the converted data

        /// @typedef AllocationFunction
        /// @brief Memory allocation function interface; returns pointer to allocated memory or NULL on failure

        /// @typedef deAllocationFunction
        /// @brief Memory deallocation function interface

        /// @internal
        /// @fn SetMemory_management_functions(AllocationFunction allocate, deAllocationFunction deallocate);
        /// @brief Override default memory management functions. All subsequent allocations/deallocations will be performed via supplied functions.
        /// @param allocate The new memory allocator
        /// @param deallocate The new memory deallocator

        /// @internal
        /// @fn GetMemoryAllocationFunction();
        /// @brief Get the current allocation funciton
        /// @return A function to the current allocation function

        /// @internal
        /// @fn GetMemoryDeallocationFunction();
        /// @brief Get the current allocation funciton
        /// @return A function to the current allocation function

        /// @brief Gets the first tag out of the Stream and returns it as a String
        /// @param stream An std::ostream that contains atleast one xml tag
        /// @details This simply returns all the characters from the current point in the stream until the first '>' is reached.
        /// @return This returns from the begining of the stream to the first xml tag.
        String GetOneTag(std::istream& stream);

        /// @internal
        /// @brief Perform a basic series of checks for extracting meaning from a single xml tag.
        /// @param NameSpace Used when throwing exceptions, this is a string containing the namespace and colons of a class to be deserialized for example "phys::"
        /// @param ClassName This will be used to identify the main xml element/tag you are attempting to deserialize. This will also be used in error messages.
        /// @param OneTag One XML tag/elements worth of text to deserialize.
        /// @return An pointer to xml::Document that you are now the owner of and must delete, that has the data parse and ready to access.
        /// @throw This can throw a phys::exception in the event that the xml cannot be parsed.
        Document* PreParseClassFromSingleTag(const String& NameSpace, const String& ClassName, const String& OneTag);

    }
}
#endif

#endif // \PHYSXML

/*
 *
 * Software, Files, Libraries and all other items referenced in this clause refers only
 * to the contents of this file and associated documentation.
 *
 * Copyright © 2006-2010 Arseny Kapoulkine
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
