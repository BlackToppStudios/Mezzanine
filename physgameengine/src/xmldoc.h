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

        /// @var StatusBadDoctype
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


        /// @struct ParseResult
        /// @brief All the information needed to troubleshoot a Parsing error.


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

        /// @var NodeDoctype
        /// @brief Document type declaration, i.e. '<!DOCTYPE doc>'.

    }
}
#endif

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
