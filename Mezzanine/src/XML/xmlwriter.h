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
#include "resource.h"

#ifndef _xmlwriter_h
#define _xmlwriter_h
namespace Mezzanine
{

    namespace XML
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief Interface for node printing (see @ref Node::Print)
        class MEZZ_LIB Writer
        {
        public:
            /// @brief Safe to dervied doconstructor
            virtual ~Writer() {}

            /// @brief Write memory chunk into stream/file/whatever
            /// @param data A void pointer that could be anything, so that it does not interfere with inheritance
            /// @param size How much to write in bytes.
            virtual void Write(const void* data, size_t size) = 0;
        };


        ///////////////////////////////////////////////////////////////////////////////
        /// @brief An implementation of @ref Writer intended for writing to FILEs as defined in stdio
        class MEZZ_LIB WriterFile: public Writer
        {
            private:
                /// @internal
                /// @brief A pointer to a FILE as defined in stdio
                void* TargetFile;

            public:
                /// @brief Construct WriterInstance from a FILE* object; void* is used to avoid header dependencies on stdio
                /// @param FilePtr the c  style FILE* to contruct this from.
                WriterFile(void* FilePtr);

                /// @brief Construct a Writer from a FILE* object.
                /// @param data Data To be written to the FILE.
                /// @param size How much data to write.
                virtual void Write(const void* data, size_t size);
        };


        ///////////////////////////////////////////////////////////////////////////////
        /// @class WriterStream
        /// @brief An implementation of @ref Writer intended for writing std::ostreams
        class MEZZ_LIB WriterStream: public Writer
        {
        public:
                /// @brief A constructor that accepts a stream of characters
                /// @param stream A stream to send stuff to.
            WriterStream(std::basic_ostream<char, std::char_traits<char> >& stream);

            /// @brief A constructor that accepts a stream of wide characters
            /// @param stream A stream to send stuff to.
            WriterStream(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& stream);

            /// @brief Actually issues the write commands.
            /// @param
            virtual void Write(const void* data, size_t size);

        private:
            /// @internal
            std::basic_ostream<char, std::char_traits<char> >* narrow_stream;
            /// @internal
            std::basic_ostream<wchar_t, std::char_traits<wchar_t> >* wide_stream;
        };


        ///////////////////////////////////////////////////////////////////////////////
        /// @class XMLStreamWrapper
        /// @headerfile resourcedatastream.h
        /// @brief This represents a simple wrapper that makes data streams compatible with the XML API.
        /// @details
        /// @todo Ideally this class should not exist, and the XML system should be made to use the resource system instead of doing its own IO.
        ///////////////////////////////////////
        class MEZZ_LIB XMLStreamWrapper : public Writer
        {
            protected:

                Resource::DataStream* WrappedStream;
            public:
                /// @brief Class constructor.
                /// @param Stream The stream to be wrapped.
                XMLStreamWrapper(Resource::DataStream* Stream) : WrappedStream(Stream) {}

                /// @brief Class destructor.
                virtual ~XMLStreamWrapper() {}

                /// @brief Writes data to the stream.
                /// @param data The data to be written.
                /// @param size The number of bytes to be written.
                void Write(const void* data, size_t size) { WrappedStream->Write(data,size); }
        };//XMLStreamWrapper


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
