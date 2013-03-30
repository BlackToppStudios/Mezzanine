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
#ifndef _xmlxpathquery_h
#define _xmlxpathquery_h

/// @file
/// @brief The definition of the @ref Mezzanine::XML::XPathQuery .

#include "datatypes.h"
#include "XML/xmlenumerations.h"
#include "XML/xpathparseresult.h"

SWIG_INFO_BEGINCLASS

namespace Mezzanine
{
    namespace XML
    {
        // A compiled XPath query object
        class XPathNode;
        class XPathNodeSet;
        class XPathVariableSet;


        /// @brief A compiled XPath query object
        /// @details When you call @ref XML::Node::FindNodes with an expression string as an argument, a query object is created behind the scenes.
        /// A query object represents a compiled XPath expression. Query objects can be needed in the following circumstances: \n
        /// - You can precompile expressions to query objects to save compilation time if it becomes an issue; \n
        /// - You can use query objects to evaluate XPath expressions which result in booleans, numbers or strings; \n
        /// - You can get the type of expression value via query object. \n \n
        /// Query objects correspond to @ref XML::XPathQuery type. They are immutable and non-copyable: they are bound to the expression at creation
        /// time and can not be cloned. If you want to put query objects in a container, allocate them on heap via new operator and store pointers
        /// to XML::XPathQuery in the container. \n \n
        /// To evaluate an XPath expression there are a few EvaluatedType functions. According to XPath specification, value of any type can be
        /// converted to boolean, number or string value, but no type other than node set can be converted to node set. Because of this,
        /// @ref XPathQuery::EvaluateBoolean(), @ref XPathQuery::EvaluateNumber() and @ref XPathQuery::EvaluateString() always return a result,
        /// but EvaluateNodeSet results in an error if the return type is not node set.
        class MEZZ_LIB XPathQuery
        {
            private:
                /// @internal
                /// @brief Used to keep much of the implementation hidden from item including this.
                void* QueryImplementation;

                /// @internal
                /// @brief A cache for the result of the query
                XPathParseResult ResultCache ;

                #ifndef SWIG
                /// @internal
                /// @brief Used to convert this to a boolean value in a safe way
                typedef void (*unspecified_bool_type)(XPathQuery***);
                #endif

                /// @brief Private copy constructor to ensure non-copyable semantics.
                XPathQuery(const XPathQuery&);

                /// @brief Private assignment operator to ensure non-assignable semantics.
                XPathQuery& operator=(const XPathQuery&);

            public:
                /// @brief Construct a compiled object from XPath expression.
                /// @param query The query in the form of a c-string style char_t array.
                /// @param variables Any extra data the query might need, passing a null pointer simply omits passing any arguments.
                /// @throw Throws XPathException on compilation errors.
                explicit XPathQuery(const Char8* query, XPathVariableSet* variables = 0);

                // Destructor
                /// @brief Destructor
                ~XPathQuery();

                /// @brief Get query expression return Type.
                /// @return A XPathValueType.
                XPathValueType ReturnType() const;

                /// @brief Evaluate expression as boolean value in the specified context; performs Type conversion if necessary.
                /// @return A bool result of evaluating the expression.
                /// @throw If XML_NO_EXCEPTIONS is not defined (by default it is not defined), throws std::bad_alloc on out of memory errors.
                /// @param n The XPathNode that will serve as the context for the query.
                bool EvaluateBoolean(const XPathNode& n) const;

                // Evaluate expression as double Value in the specified context; performs Type conversion if necessary.
                /// @brief Evaluate expression as double value in the specified context; performs Type conversion if necessary.
                /// @param n The XPathNode that will serve as the context for the query.
                /// @throw Throws std::bad_alloc on out of memory errors.
                /// @return A result as a double from evaluating the expression.
                double EvaluateNumber(const XPathNode& n) const;

                // Evaluate expression as string Value in the specified context; performs Type conversion if necessary.
                /// @brief Evaluate expression as string value in the specified context; performs Type conversion if necessary.
                /// @param n The XPathNode that will serve as the context for the query.
                /// @throw Throws std::bad_alloc on out of memory errors.
                /// @return A result as a String from evaluating the expression.
                String EvaluateString(const XPathNode& n) const;

                // Evaluate expression as string Value in the specified context; performs Type conversion if necessary.
                // At most capacity characters are written to the destination buffer, full Result size is returned (includes terminating zero).
                /// @brief Evaluate expression as string value in the specified context; performs Type conversion if necessary.
                /// @param buffer The place to store the c-style Character array
                /// @param capacity At most capacity characters are written to the destination buffer.
                /// @param n The XPathNode that with serve as the context for the query.
                /// @throw  std::bad_alloc on out of memory errors.
                /// @return Full result size is returned (includes terminating zero).
                size_t EvaluateString(Char8* buffer, size_t capacity, const XPathNode& n) const;

                // Evaluate expression as node set in the specified context.
                // throws XPathException on Type mismatch and std::bad_alloc on out of memory errors.

                /// @brief Evaluate expression as node set in the specified context.
                /// @param n The XPathNode that with serve as the context for the query.
                /// @return An XPathNodeSet.
                XPathNodeSet EvaluateNodeSet(const XPathNode& n) const;

                /// @brief Get parsing Result (used to get compilation errors when XML_NO_EXCEPTIONS is enabled)
                /// @return A const reference to an XPathParseResult.
                const XPathParseResult& Result() const;

                #ifndef SWIG
                /// @brief Used to convert this to a boolean value in a safe way
                /// @return Returns true if the internal data is set and false otherwise.
                operator unspecified_bool_type() const;
                #endif

                /// @brief Logical not operator, used a workaround for borland compiler.
                /// @return A bool that is the opposite of evaluatig this as a bool normally.
                bool operator!() const;
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
