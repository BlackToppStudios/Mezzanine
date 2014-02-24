// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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

#ifndef _xmlenums_h
#define _xmlenums_h

/// @file
/// @brief Enumerations and constant values used primarily in the XML system but useful for interacting with it in other places.

#include "datatypes.h"



namespace Mezzanine
{
    namespace XML
    {
        // Parsing options

        /// @brief Minimal parsing mode (equivalent to turning all other flags off).
        /// @details Only elements and PCDATA sections are added to the DOM tree, no text conversions are performed.
        const unsigned int ParseMinimal = 0x0000;
        /// @brief This flag determines if processing instructions (NodePi) are added to the DOM tree. This flag is off by default.
        const unsigned int ParsePi = 0x0001;
        /// @brief This flag determines if comments (NodeComment) are added to the DOM tree. This flag is off by default.
        const unsigned int ParseComments = 0x0002;
        /// @brief This flag determines if CDATA sections (NodeCdata) are added to the DOM tree. This flag is on by default.
        const unsigned int ParseCdata = 0x0004;
        /// @brief This flag determines if plain character data (NodePcdata) that consist only of whitespace are added to the DOM tree.
        /// @details This flag is off by default; turning it on usually results in slower parsing and more memory consumption.
        const unsigned int ParseWsPcdata = 0x0008;
        /// @brief This flag determines if character and entity references are expanded during parsing. This flag is on by default.
        const unsigned int ParseEscapes = 0x0010;
        /// @brief This flag determines if EOL characters are normalized (converted to \#xA) during parsing. This flag is on by default.
        const unsigned int ParseEol = 0x0020;
        /// @brief This flag determines if attribute values are normalized using CDATA normalization rules during parsing. This flag is on by default.
        const unsigned int ParseWconvAttribute = 0x0040;
        /// @brief This flag determines if attribute values are normalized using NMTOKENS normalization rules during parsing. This flag is off by default.
        const unsigned int ParseWnormAttribute = 0x0080;
        /// @brief This flag determines if document declaration (NodeDeclaration) is added to the DOM tree. This flag is off by default.
        const unsigned int ParseDeclaration = 0x0100;
        /// @brief This flag determines if document type declaration (NodeDoctype) is added to the DOM tree. This flag is off by default.
        const unsigned int ParseDocType = 0x0200;
        /// @brief This flag determines if plain character data (NodePcdata) that is the only child of the parent node and that consists only of whitespace is added to the DOM tree.
        /// @details This flag is off by default; turning it on may Result in slower parsing and more memory consumption.
        const unsigned int ParseWsPcdata_single = 0x0400;

        /// @brief The default parsing mode.
        /// @details Elements, PCDATA and CDATA sections are added to the DOM tree, character/reference entities are expanded,
        /// End-of-Line characters are normalized, attribute values are normalized using CDATA normalization rules.
        const unsigned int ParseDefault = ParseCdata | ParseEscapes | ParseWconvAttribute | ParseEol;
        /// @brief The full parsing mode.
        /// @details Nodes of all types are added to the DOM tree, character/reference entities are expanded,
        /// End-of-Line characters are normalized, attribute values are normalized using CDATA normalization rules.
        const unsigned int ParseFull = ParseDefault | ParsePi | ParseComments | ParseDeclaration | ParseDocType;

        /// @brief These flags determine the encoding of input data for an XML document.
        enum Encoding
        {
            EncodingAuto,           ///< Auto-detect input DocumentEncoding using BOM or < / <? detection; use UTF8 if BOM is not found
            EncodingUTF8,           ///< UTF8 DocumentEncoding
            EncodingUTF16LE,        ///< Little-endian UTF16
            EncodingUTF16BE,        ///< Big-endian UTF16n
            EncodingUTF16,          ///< UTF16 with native endianness
            EncodingUTF32LE,        ///< Little-endian UTF32
            EncodingUTF32BE,        ///< Big-endian UTF32
            EncodingUTF32,          ///< UTF32 with native endianness
            Encodingwchar_t,		///< The same document encoding wchar_t has (usually either UTF16 or UTF32)
            EncodingLatin1          ///< Also called IEC_8859-1 a common encoding on windows, see http://en.wikipedia.org/wiki/ISO/IEC_8859-1 for furhter
        };

        // Formatting flags

        /// @brief Indent the nodes that are written to output stream with as many indentation strings as deep the node is in DOM tree. This flag is off by default.
        const unsigned int FormatIndent = 0x01;
        /// @brief Write encoding-specific Byte Order Mark (BOM) to the output stream. This flag is off by default.
        const unsigned int FormatWriteBom = 0x02;
        /// @brief Use raw output mode (no indentation and no line breaks are written). This flag is on by default.
        const unsigned int FormatRaw = 0x04;
        /// @brief Omit default XML declaration even if there is no declaration in the document. This flag is off by default.
        const unsigned int FormatNoDeclaration = 0x08;
        /// @brief  Don't escape GetAttribute Values and PCDATA contents. This flag is off by default.
        const unsigned int FormatNoEscapes = 0x10;
        /// @brief Open file using text mode in XML::Document::SaveFile. This enables special character (i.e. new-line) conversions on some systems. This flag is off by default.
        const unsigned int FormatSaveFileText = 0x20;
        /// @brief The default set of formatting flags. Only FormatRaw is enabled.
        /// @note PugiXML defaults to FormatIndent which is not well suited to computer to computer transmission as games commonly do
        const unsigned int FormatDefault = FormatRaw;

        /// @brief The types of nodes that could be in the XML Tree.
        enum NodeType
        {
            NodeNull,			///< Empty (null) node handle
            NodeDocument,		///< A document tree's absolute GetRoot
            NodeElement,		///< Element tag, i.e. '\<node/\>'
            NodePcdata,         ///< Plain character data, i.e. 'text'
            NodeCdata,			///< Character data, i.e. '\<![CDATA[text]]\>'
            NodeComment,		///< Comment tag, i.e. '\<!-- text --\>'
            NodePi,             ///< Processing instruction, i.e. '\<?Name?\>'
            NodeDeclaration,	///< Document declaration, i.e. '\<?xml version="1.0"?\>'
            NodeDocType         ///< Document Type declaration, i.e. '\<!DOCTYPE doc\>'
        };

        /// @brief These statuses are used to help determine what issues, if any the parser had. Returned by @ref Mezzanine::XML::ParseResult instances.
        enum ParseStatus
        {
            StatusOk = 0,                       ///< This is returned to indicated there where no issues parsing the XML document.

            StatusFileNotFound,                 ///< File was not found during a loading from filename attempt.
            StatusIOError,                      ///< Error reading from file or stream.

            StatusOutOfMemory,                  ///< Could not allocate memory.
            StatusInternalError,                ///< An unkown error, currently nothing should be able to return this status.

            StatusUnrecognizedTag,              ///< The parser could not determine type of tag.

            StatusBadProcessingInstruction,		///< Parsing error occurred while parsing document declaration/processing instruction.
            StatusBadComment,                   ///< Parsing error occurred while parsing comment.
            StatusBadCdata,                     ///< Parsing error occurred while parsing CDATA section.
            StatusBadDocType,                   ///< Parsing error occurred while parsing document type declaration.
            StatusBadPcdata,                    ///< Parsing error occurred while parsing PCDATA section.
            StatusBadStartElement,              ///< Parsing error occurred while parsing start element tag.
            StatusBadAttribute,                 ///< Parsing error occurred while parsing element attribute.
            StatusBadEndElement,                ///< Parsing error occurred while parsing end element tag.
            StatusEndElementMismatch            ///< There was a mismatch of start-end tags (closing tag had incorrect name, some tag was not closed or there was an excessive closing tag).
        };

        /// @brief XPathQuery return type
        enum XPathValueType
        {
            XPathTypeNone,          ///< Unknown Type (query failed to compile)
            XPathTypeNodeSet,       ///< Node set (XPathNodeSet)
            XPathTypeNumber,        ///< Number This corresponds to a double or Real.
            XPathTypeString,        ///< Corresponds to the String type.
            XPathTypeBoole        ///< Boole.
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
