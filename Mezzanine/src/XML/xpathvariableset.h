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
#ifndef _xpathvariableset_h
#define _xpathvariableset_h

/// @file
/// @brief The definition of the @ref XPathVariableSet

#include "datatypes.h"

#include "XML/xmlenumerations.h"

SWIG_INFO_BEGINCLASS

namespace Mezzanine
{
    namespace XML
    {
        class XPathNodeSet;
        class XPathVariable;

        /// @brief A set of XPath variables.
        class MEZZ_LIB XPathVariableSet
        {
            private:

                /// @internal
                /// @todo Look into this more carefully.
                XPathVariable* _data[64];

                /// @brief Private copy constructor forces non-copyable semantics
                XPathVariableSet(const XPathVariableSet&);

                /// @brief Private copy assignment operator forces non-copyable semantics
                XPathVariableSet& operator=(const XPathVariableSet&);

                /// @internal
                /// @brief Find a variable of a given Name
                /// @param Name The name of the Node
                /// @details Performs a linear search.
                XPathVariable* Find(const Char8* Name) const;

            public:
                /// @brief Default Constructor, Blanks any XPathVariable it contains.
                XPathVariableSet();

                /// @brief Default Deconstructor, Deletes any XPathVariable it contains.
                ~XPathVariableSet();

                /// @brief Add a new variable or get the existing one, if the Types match
                /// @return A pointer to the XPathVariable you referenced or just created.
                /// @param Name The name of variable to add.
                /// @param Type The Type of the new value to add as an XPathValueType.
                XPathVariable* Add(const Char8* Name, XPathValueType Type);

                /// @brief Set contained variable Value; no Type conversion is performed.
                /// @param Name The name of variable to change.
                /// @param Value The value to attempt to put into the named variable.
                /// @return True is return, false is returned if there is no such variable or on Type mismatch error.
                bool Set(const Char8* Name, bool Value);

                /// @brief Set contained variable Value; no Type conversion is performed.
                /// @param Name The name of variable to change.
                /// @param Value The value to attempt to put into the named variable.
                /// @return True is return, false is returned if there is no such variable or on Type mismatch error.
                bool Set(const Char8* Name, double Value);

                /// @brief Set contained variable Value; no Type conversion is performed.
                /// @param Name The name of variable to change.
                /// @param Value The value to attempt to put into the named variable.
                /// @return True is return, false is returned if there is no such variable or on Type mismatch error.
                bool Set(const Char8* Name, const Char8* Value);

                /// @brief Set contained variable Value; no Type conversion is performed.
                /// @param Name The name of variable to change.
                /// @param Value The value to attempt to put into the named variable.
                /// @return True is return, false is returned if there is no such variable or on Type mismatch error.
                bool Set(const Char8* Name, const XPathNodeSet& Value);

                /// @brief Get the named XPathVariable.
                /// @return A pointer to the specified XPathVariable.
                /// @param Name The name of the XPathVariable you want.
                XPathVariable* Get(const Char8* Name);

                #ifndef SWIG
                /// @brief Get the named XPathVariable.
                /// @param Name The name of the XPathVariable you want.
                /// @return A pointer to the specified XPathVariable.
                /// @note The non-const version is available to scripting languages.
                const XPathVariable* Get(const Char8* Name) const;
                #endif
        };
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
