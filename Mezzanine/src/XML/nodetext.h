// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _xmlnodetext_h
#define _xmlnodetext_h

/// @file
/// @brief The declaration of the XML::NodeText class

#include "datatypes.h"
#include "node.h"
#include "swig.h"



namespace Mezzanine
{
    namespace XML
    {
        /// @brief A helper for working with text inside PCDATA nodes.
        class MEZZ_LIB NodeText
        {
            friend class Node;

            /// @brief Actually the the data for this NodeText
            NodeStruct* RootNode;

            #ifndef SWIG
            /// @brief Used to prevent casting to numerical types acccidentally.
            /// @note Not available in scripting languages because conversion is handled on a per langauge basis
            typedef void (*unspecified_bool_type)(NodeText***);
            #endif

            /// @internal
            /// @brief Create this from the internal data of another node.
            explicit NodeText(NodeStruct* OtherRoot);

            /// @internal
            /// @brief Get the internal data pointer for the nearest Parent node or allocate one if required.
            NodeStruct* DataNew();

            /// @internal
            /// @brief Get the internal data pointer for the nearest Parent node
            NodeStruct* Data() const;

        public:
            /// @brief Default constructor. Constructs an empty object.
            NodeText();

            #ifndef SWIG
            /// @brief Used to convert this to a boolean value in a safe way
            /// @return Returns true if the internal data is set and false otherwise.
            operator unspecified_bool_type() const;
            #endif

            /// @brief Used to convert this attribute the opposite of it's normal boolean value
            /// @details This is described in the PugiXML source a a workaround for a borland c++ issue.
            /// @return Returns false if the internal pointer AttributeStruct is set and true otherwise.
            bool operator!() const;

            /// @brief Is this storing anything at all?
            /// @return Returns True if this @ref NodeText is storing nothing. False if it is storing anything.
            bool Empty() const;

            /// @brief Get text, or "" if object is empty
            /// @return A pointer to the string or "" if this is empty.
            const Char8* GetString() const;

            /// @brief Get text, or the default Value if object is empty
            /// @param def The value to be returned if there is no valid return
            /// @return This will return The content of this node or def
            const Char8* AsString(const Char8* def = "") const;

            /// @brief Get text as a number, or the default Value if conversion did not succeed or object is empty
            /// @param def The value To be returned if an error occurs in conversion, like a an out of memory condition.
            /// @return The value in this @ref NodeText as an int or def if the text is invalid for conversion or generates an error.
            int AsInt(int def = 0) const;

            /// @brief Get text as a number, or the default Value if conversion did not succeed or object is empty
            /// @param def The value To be returned if an error occurs in conversion, like a an out of memory condition.
            /// @return The value in this @ref NodeText as an unisgned int or def if the text is invalid for conversion or generates an error.
            unsigned int AsUint(unsigned int def = 0) const;

            /// @brief Get text as a number, or the default Value if conversion did not succeed or object is empty
            /// @param def The value To be returned if an error occurs in conversion, like a an out of memory condition.
            /// @return The value in this @ref NodeText as a double or def if the text is invalid for conversion or generates an error.
            double AsDouble(double def = 0) const;

            /// @brief Get text as a number, or the default Value if conversion did not succeed or object is empty
            /// @param def The value To be returned if an error occurs in conversion, like a an out of memory condition.
            /// @return The value in this @ref NodeText as a float or def if the text is invalid for conversion or generates an error.
            float AsFloat(float def = 0) const;

            /// @brief Get text as a number, or the default Value if conversion did not succeed or object is empty
            /// @param def The value To be returned if an error occurs in conversion, like a an out of memory condition.
            /// @return The value in this @ref NodeText as a Mezzanine::Real or def if the text is invalid for conversion or generates an error.
            Real AsReal(Real def = 0) const;

            /// @brief Get text as a number, or the default Value if conversion did not succeed or object is empty
            /// @param def The value To be returned if an error occurs in conversion, like a an out of memory condition.
            /// @return The value in this @ref NodeText as a Mezzanine::Whole or def if the text is invalid for conversion or generates an error.
            Whole AsWhole(Whole def = 0) const;

            /// @brief Get text as a number, or the default Value if conversion did not succeed or object is empty
            /// @param def The value To be returned if an error occurs in conversion, like a an out of memory condition.
            /// @return The value in this @ref NodeText as a Mezzanine::Integer or def if the text is invalid for conversion or generates an error.
            Integer AsInteger(Integer def = 0) const;

            /// @brief Get text as bool
            /// @param def The default value to return if conversion is not possible.
            /// @return True if first character is in '1tTyY' set), or the default Value if object is empty.
            bool AsBool(bool def = false) const;

            /// @brief Set text
            /// @param rhs The value to store in this text.
            /// @return False if object is empty or there is not enough memory.
            bool Set(const Char8* rhs);

#if !(defined(SWIG) && defined(MEZZLUA51)) // Stop Swig from making lua bindings but allow other languages
            /// @brief Set text by lexigraphically converting rhs.
            /// @param rhs The value to store in this text.
            /// @return False if object is empty or there is not enough memory.
            bool Set(int rhs);

            /// @brief Set text by lexigraphically converting rhs.
            /// @param rhs The value to store in this text.
            /// @return False if object is empty or there is not enough memory.
            bool Set(unsigned int rhs);

            /// @brief Set text by lexigraphically converting rhs.
            /// @param rhs The value to store in this text.
            /// @return False if object is empty or there is not enough memory.
            bool Set(double rhs);

            /// @brief Set text by converting value to "true"/"false"
            /// @param rhs The value to store in this text.
            /// @return False if object is empty or there is not enough memory.
            bool Set(bool rhs);
#endif

            /// @brief Set text (equivalent to set without error checking)
            /// @param rhs The value to store in this text.
            /// @return A reference to the modified NodeText to allow operator chaining.
            NodeText& operator=(const Char8* rhs);

            /// @brief Set text (equivalent to set without error checking) by lexigraphically converting rhs.
            /// @param rhs The value to store in this text.
            /// @return A reference to the modified NodeText to allow operator chaining.
            NodeText& operator=(int rhs);

            /// @brief Set text (equivalent to set without error checking) by lexigraphically converting rhs.
            /// @param rhs The value to store in this text.
            /// @return A reference to the modified NodeText to allow operator chaining.
            NodeText& operator=(unsigned int rhs);

            /// @brief Set text (equivalent to set without error checking) by lexigraphically converting rhs.
            /// @param rhs The value to store in this text.
            /// @return A reference to the modified NodeText to allow operator chaining.
            NodeText& operator=(double rhs);

            /// @brief Set text (equivalent to set without error checking)by converting value to "true"/"false"
            /// @param rhs The value to store in this text.
            /// @return A reference to the modified NodeText to allow operator chaining.
            NodeText& operator=(bool rhs);

            /// @brief Get the data node (NodePcdata or NodeCdata) for this object
            /// @return A Node that contains this NodeText.
            Node data() const;
        };
    }
}// /namespace Mezzanine



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
