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
#ifndef _xmlxpathnodeset_h
#define _xmlxpathnodeset_h

/// @file
/// @brief This file contains the defintion of the @ref Mezzanine::XML::XPathNodeSet

#include "datatypes.h"
#include "XML/xpathnode.h"



namespace Mezzanine
{
    namespace XML
    {
        /// @brief A fixed sized collection of nodes that an XPathQuery can work on.
        class MEZZ_LIB XPathNodeSet
        {
            public:
                /// @brief The different ways a collection may or may not be ordered.
                enum CollectionType
                {
                    TypeUnsorted,			///< Not ordered
                    TypeSorted,             ///< Sorted by document order (ascending)
                    TypeSortedReverse		///< Sorted by document order (descending)
                };

                /// @brief An iterator trait. Const iterator for XPathNodes.
                typedef const XPathNode* const_iterator;

                /// @brief Default constructor. Constructs empty set.
                XPathNodeSet();

                /// @param begin A const XPathNode iterator at the beginning of the set of nodes.
                /// @param end A const XPathNode iterator at the end of the set of nodes.
                /// @param Type What XPathNodeSet::CollectionType is being used, this defaults to XPathNodeSet::TypeUnsorted
                /// @brief Constructs a set from iterator range.
                /// @details Data is not checked for duplicates and is not sorted according to provided Type, so be careful.
                XPathNodeSet(const_iterator begin, const_iterator end, CollectionType Type = TypeUnsorted);

                /// @brief Destructor
                ~XPathNodeSet();

                /// @brief Copy constructor/assignment operator
                /// @param ns The Target to copy.
                XPathNodeSet(const XPathNodeSet& ns);

                /// @brief Assignment Operator.
                /// @return A reference to the freshly assigned XPathNodeSet.
                /// @param ns The XPathNodeSet to copy.
                XPathNodeSet& operator=(const XPathNodeSet& ns);

                /// @brief Get collection Type.
                /// @return CollectionType
                CollectionType Type() const;

                // Get collection size
                /// @brief Get collection size.
                /// @return The number of XPathNode instances as a size_t in the set
                size_t size() const;

                /// @brief Indexing operator.
                /// @param index A size_t indicating which XPathNode you would like to retrieve
                /// @return A const reference to the XPathNode you requested.
                /// @warning Out of bounds errors are checked using assert. Exceptions will not be thrown, during debugging out of bounds access will abort the termination and in production code out of bounds accesses will cause undefined behavior.
                const XPathNode& operator[](size_t index) const;

                /// @brief Get Beginning iterator.
                /// @return A XPathNodeSet::const_iterator to the beginning of the collection.
                const_iterator begin() const;

                /// @brief Get Ending iterator.
                /// @return A XPathNodeSet::const_iterator to the end of the collection.
                const_iterator end() const;

                /// @brief Sort the collection in ascending/descending order by document order.
                /// @param reverse If true this sorts the collection in the opposite of document order.
                void sort(bool reverse = false);

                /// @brief Get first node in the collection by document order.
                /// @return The first node of the, in document order as an XPathNode.
                XPathNode first() const;

                /// @brief Check if collection is empty.
                /// @return True if the document is empty, false otherwise.
                bool Empty() const;

            private:
                /// @brief Current ordering of this set
                CollectionType TypeOrder;

                /// @internal
                /// @brief Used to initialize the begining of the collections structure.
                XPathNode Storage;

                /// @internal
                /// @brief The beginning of the collection
                XPathNode* Begin;

                /// @internal
                /// @brief The End of the collection
                XPathNode* End;

                void _assign(const_iterator begin, const_iterator end);
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
