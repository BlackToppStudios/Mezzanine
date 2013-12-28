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
#ifndef _xpathvariable_h
#define _xpathvariable_h

/// @file
/// @brief The XPathVariable class definition

#include "datatypes.h"
#include "XML/xmlenumerations.h"



namespace Mezzanine
{
    namespace XML
    {
        class XPathNodeSet;

        /// @brief A single XPath variable
        /// @details This is intended to be used as a single member of an @ref Mezzanine::XML::XPathVariableSet, and for moving data into and out of @ref Mezzanine::XML::XPathQuery.
        class MEZZ_LIB XPathVariable
        {
                friend class XPathVariableSet;

            protected:

                /// @internal
                /// @brief What kind of data does this variable store
                XPathValueType ValueType;

                /// @internal
                /// @brief The Next Variable in a linked structure of XPathVariables.
                XPathVariable* NextVariable;

                /// @brief Protected Default constructor to prevent default constrution.
                XPathVariable();

                /// @brief Protected Copy Constructor, used to force noncopyable semantics
                XPathVariable(const XPathVariable&);

                /// @brief Protected assignment operator, used to force noncopyable semantics
                /// @return Shouldn't be used, not supported.
                XPathVariable& operator=(const XPathVariable&);

            public:
                /// @brief Get the variable name
                /// @returns The name of this variable or a null pointer if this this @ref XPathVariable is of an invalid type/
                const Char8* Name() const;

                /// @brief Get variable type
                /// @return The current Type of this @ref XPathVariable.
                XPathValueType Type() const;

                // Get variable Value; no Type conversion is performed, default Value (false, NaN, empty string, empty node set) is returned on Type mismatch error
                /// @brief Get this as a bool.
                /// @details Get variable Value; Minimal Type conversion is performed, default Value (false, NaN, empty string, empty node set) is returned on Type mismatch error
                /// @return This as a bool, with any appropriate downcasting.
                bool GetBoolean() const;

                /// @details Get variable Value; default Value (false, NaN, empty string, empty node set) is returned on Type mismatch error
                /// @return This as a double.
                /// @brief Get this as a double.
                double GetNumber() const;

                /// @brief Get this as a c-string.
                /// @details Get variable Value; default Value (false, NaN, empty string, empty node set) is returned on Type mismatch error
                /// @return This as a c-string of char_t, without conversion.
                const Char8* GetString() const;

                /// @brief Get this as a XPathNodeSet.
                /// @details Get variable Value; no Type conversion is performed, if type of variable is not a XPathNodeSet then an XPathException is thrown
                /// @throw XPathException on type mismatch or allocation error
                /// @return A This as an XPathNodeSet, without conversion.
                const XPathNodeSet& GetNodeSet() const;

                /// @brief Set variable Value; no Type conversion is performed.
                /// @return True is returned on Success, false is returned on Type mismatch error.
                /// @param Value The value to attempt to put into this.
                bool Set(bool Value);

                /// @brief Set variable Value; no Type conversion is performed.
                /// @param Value The value as a double to attempt to put into this.
                /// @return True is return, false is returned on Type mismatch error.
                bool Set(double Value);

                /// @brief Set variable Value; no Type conversion is performed.
                /// @param Value The value as a char_t array to attempt to put into this.
                /// @return True is return, false is returned on Type mismatch error.
                bool Set(const Char8* Value);

                /// @brief Set variable Value; no Type conversion is performed.
                /// @param Value The value to attempt to put into this.
                /// @return True is return, false is returned on Type mismatch error.
                bool Set(const XPathNodeSet& Value);
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
