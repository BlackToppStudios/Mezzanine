// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef _xmldocument_h
#define _xmldocument_h

/// @file
/// @brief The definition of the XML::Document Class

#include "datatypes.h"

#include "XML/xmlenumerations.h"
#include "XML/parseresult.h"
#include "XML/node.h"
#include "XML/writer.h"


#ifndef SWIG
#include "Resource/datastream.h"
#endif



namespace Mezzanine
{
    namespace XML
    {
        /// @brief The root node of any xml hierarchy is a @ref Document
        /// @details This has all the same features as a Node and include a few features for saving, loading, streaming
        /// and to a limited degree managing the document declaration.
        class MEZZ_LIB Document: public Node
        {
            private:
                /// @internal
                /// @brief Almost all of the XML Text is stored in this.
                Char8* _buffer;

                /// @internal
                /// @brief Used when initializining for storing parts of pages
                char _memory[192];


                /// @brief Private copy constructor to make this non-copyable.
                Document(const Document&);

                /// @brief Private assignment operator enforces non-assignability.
                const Document& operator=(const Document&);

                /// @internal
                /// @brief Performs all required memory allocation for construction.
                void create();

                /// @internal
                /// @brief De-allocates Every allocated durign normal use and the Document::create funciton.
                void destroy();

                /// @internal
                /// @brief The implementation for the different functions that load rom buffers.
                /// @param contents The Buffer
                /// @param size The size of the buffer in bytes
                /// @param options A bitmask of parsing options as declared in @ref xmlenumerations.h
                /// @param DocumentEncoding Value used to determine if we need to guess encoding or use this or a variant of this.
                /// @param is_mutable can we changethe buffer or do we need to write changes elsewhere
                /// @param own Does the XML system own this now?
                ParseResult LoadBufferImpl(void* contents, size_t size, unsigned int options, Encoding DocumentEncoding, bool is_mutable, bool own);

            public:
                /// @brief Creates an empty document with just a root Node
                Document();

                /// @brief Tears down a document, and incidentally invalidates all Node and Attribute handles to this document.
                virtual ~Document();

                /// @brief Removes all nodes, leaving the empty document.
                void Reset();

                /// @brief Removes all nodes, then copies the entire contents of the specified document
                /// @param proto The Document to copy.
                void Reset(const Document& proto);

                #ifndef SWIG_SAFE

                /// @brief Load XML from a stream.
                /// @param stream An std::istream which has xml text in it.
                /// @param options A bitset of parse options that should be set using the Parse variables. This Defaults to ParseDefault.
                /// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto
                /// @return A ParseResult that stores the the outcome of attempting to load the document.
                /// @note Not available in the 'Safe' scripting languages because of the file access it could provide.
                ParseResult Load(std::basic_istream<char, std::char_traits<char> >& stream, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto);

                /// @brief Load XML from a wide stream.
                /// @param stream An std::basic_istream which has xml wide character text in it.
                /// @param options A bitset of parse options that should be set using the Parse variables. This Defaults to ParseDefault.
                /// @return A ParseResult that stores the the outcome of attempting to load the document.
                /// @note Not available in the 'Safe' scripting languages because of the file access it could provide.
                ParseResult Load(std::basic_istream<wchar_t, std::char_traits<wchar_t> >& stream, unsigned int options = ParseDefault);
                #endif

                /// @brief Load XML from a C-style string.
                /// @param contents A pointer to the Null terminated array of Characters.
                /// @param options A bitset of parse options that should be set using the Parse variables. This Defaults to ParseDefault.
                /// @return A ParseResult that stores the the outcome of attempting to load the document.
                /// @note The only Load method available in the 'Safe' scripting languages.
                ParseResult Load(const Char8* contents, unsigned int options = ParseDefault);

                #ifndef SWIG_SAFE
                // Load document from file
                /// @brief Load document from file
                /// @param Path An c-style char array that contains the path and filename of the xml document to load.
                /// @param options A bitset of parse options that should be set using the Parse variables. This Defaults to ParseDefault.
                /// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto
                /// @return A ParseResult that stores the the outcome of attempting to load the document.
                /// @note Not available in the 'Safe' scripting languages because of the file access it could provide.
                ParseResult LoadFile(const char* Path, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto);

                /// @brief Load document from file
                /// @param Path An c-style wide char array that contains the path and filename of the xml document to load.
                /// @param options A bitset of parse options that should be set using the Parse variables. This Defaults to ParseDefault.
                /// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto
                /// @return A ParseResult that stores the the outcome of attempting to load the document.
                /// @note Not available in the 'Safe' scripting languages because of the file access it could provide.
                ParseResult LoadFile(const wchar_t* Path, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto);
                #endif

                /// @brief Load document from buffer. Copies/converts the buffer, so it may be deleted or changed after the function returns.
                /// @param contents A pointer to buffer containing the xml document to be parsed, that will remain unchanged.
                /// @param size The size of the buffer.
                /// @param options A bitset of parse options that should be set using the Parse variables. This Defaults to ParseDefault.
                /// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto
                /// @return A ParseResult that stores the the outcome of attempting to load the document.
                /// @note All buffer loading Methods of the XML::Document are made available to all scripting languages libraries.
                ParseResult LoadBuffer(const void* contents, size_t size, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto);

                // Load document from buffer, using the buffer for in-place parsing (the buffer is modified and used for storage of document data).
                // You should ensure that buffer data will persist throughout the document's lifetime, and free the buffer memory manually once document is destroyed.
                /// @brief Load document from buffer, using the buffer for in-place parsing (the buffer is modified and used for storage of document data).
                /// @details You should ensure that buffer data will persist throughout the documents lifetime, and free the buffer memory manually once document is destroyed.
                /// @param contents A pointer to buffer containing the xml document to be parsed, that must remain for the lifecycle of the XML::Document.
                /// @param size The size of the buffer.
                /// @param options A bitset of parse options that should be set using the Parse variables. This Defaults to ParseDefault.
                /// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto
                /// @return A ParseResult that stores the the outcome of attempting to load the document.
                /// @note All buffer loading Methods of the XML::Document are made available to all scripting languages libraries.
                ParseResult LoadBufferInplace(void* contents, size_t size, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto);

                // Load document from buffer, using the buffer for in-place parsing (the buffer is modified and used for storage of document data).
                // You should allocate the buffer with pugixml allocation function; document will free the buffer when it is no longer needed (you can't use it anymore).
                /// @brief Load document from buffer, using the buffer for in-place parsing (the buffer is modified and used for storage of document data).
                /// @details You should allocate the buffer with pugixml allocation function; XML::Document will free the buffer when it is no longer needed (you can not use it anymore).
                /// @param contents A pointer to buffer containing the xml document to be parsed.
                /// @param size The size of the buffer.
                /// @param options A bitset of parse options that should be set using the Parse variables. This Defaults to ParseDefault.
                /// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto.
                /// @return A ParseResult that stores the the outcome of attempting to load the document.
                /// @note All buffer loading Methods of the XML::Document are made available to all scripting languages libraries.
                ParseResult LoadBufferInplaceOwn(void* contents, size_t size, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto);

                #ifndef SWIG_SAFE

                /// @brief Save XML document to WriterInstance.
                /// @param WriterInstance The Writer that will be used to output the xml text.
                /// @param indent The Character(s) used to represent a tab in the output, this defaults to one tab character.
                /// @param flags The output format flags, this is a bitfield that defaults to XML::FormatDefault.
                /// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto.
                /// @note Not available in the 'Safe' scripting languages because of the file access it could provide.
                void Save(Writer& WriterInstance, const Char8* indent = "\t", unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto) const;

                /// @brief Save XML document to a stream of characters.
                /// @param stream The output stream of wide characters to send the XML document to.
                /// @param indent The Character(s) used to represent a tab in the output, this defaults to one tab character.
                /// @param flags The output format flags, this is a bitfield that defaults to XML::FormatDefault.
                /// @param DocumentEncoding The Type of encoding to use when saving the document.
                /// @note Not available in the 'Safe' scripting languages because of the file access it could provide.
                void Save(std::basic_ostream<char, std::char_traits<char> >& stream, const Char8* indent = "\t", unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto) const;

                /// @brief Save XML document to a stream of wide characters.
                /// @param stream The output stream of wide characters to send the XML document to.
                /// @param indent The Character(s) used to represent a tab in the output, this defaults to one tab character.
                /// @param flags The output format flags, this is a bitfield that defaults to XML::FormatDefault.
                /// @note Not available in the 'Safe' scripting languages because of the file access it could provide.
                void Save(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& stream, const Char8* indent = "\t", unsigned int flags = FormatDefault) const;

                /// @brief Save XML to file.
                /// @param Path A c-style array of chars that contain the filename (and any path) of the file to be output.
                /// @param indent The Character(s) used to represent a tab in the output, this defaults to one tab character.
                /// @param flags The output format flags, this is a bitfield that defaults to XML::FormatDefault.
                /// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto.
                /// @return False if the target file could not be opened for writing
                /// @note Not available in the 'Safe' scripting languages because of the file access it could provide.
                bool SaveFile(const char* Path, const Char8* indent = "\t", unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto) const;

                /// @brief Save XML to file.
                /// @param Path A c-style array of wide chars that contain the filename (and any path) of the file to be output.
                /// @param indent The Character(s) used to represent a tab in the output, this defaults to one tab character.
                /// @param flags The output format flags, this is a bitfield that defaults to XML::FormatDefault.
                /// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto.
                /// @return False if the target file could not be opened for writing
                /// @note Not available in the 'Safe' scripting languages because of the file access it could provide.
                bool SaveFile(const wchar_t* Path, const Char8* indent = "\t", unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto) const;
                #endif

                // Get document element
                /// @brief Get document element
                /// @return An XML::Node that is the root element of the xml Document
                Node DocumentElement() const;
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
