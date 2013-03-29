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
#ifndef _xmlutfconversion_h
#define _xmlutfconversion_h

/// @file
/// @brief A few functions to help the XML system perform conversions between UTF8 and the native wide character UTF implementation.

#include "datatypes.h"

SWIG_INFO_BEGINCLASS

namespace Mezzanine
{
    namespace XML
    {
        /// @brief Convert a c-style string of wchar_t to std::string containing UTF8.
        /// @param str The wide string to convert
        /// @return A std::basic_string<char, std::char_traits<char>, std::allocator<char> > containing the converted data
        std::basic_string<char, std::char_traits<char>, std::allocator<char> > MEZZ_LIB AsUtf8(const wchar_t* str);

        /// @brief Convert a std::wstring to a UTF8 std::string
        /// @param str The wide string to convert.
        /// @return A std::basic_string<char, std::char_traits<char>, std::allocator<char> > containing the converted data
        std::basic_string<char, std::char_traits<char>, std::allocator<char> > MEZZ_LIB AsUtf8(const std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >& str);

        /// @brief Convert a Convert a c-style string to std::wstring containing native encoding (Usually UCS2 on windows and UTF32 on Linux/Mac).
        /// @param str The string to convert.
        /// @return A std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > containing the converted data
        std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > MEZZ_LIB AsWide(const char* str);

        /// @brief Convert a Convert a std::string to std::wstring containing native encoding (Usually UCS2 on windows and UTF32 on Linux/Mac).
        /// @param str The string to convert.
        /// @return A std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > containing the converted data
        std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > MEZZ_LIB AsWide(const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& str);
    }
} // /namespace Mezzanine

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
