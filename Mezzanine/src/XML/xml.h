//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
 * pugixml parser - version 1.2
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

#ifndef _XMLCONFIG_H
#define _XMLCONFIG_H

// Tune these constants to adjust memory-related behavior
// #define XML_MEMORY_PAGE_SIZE 32768
// #define XML_MEMORY_OUTPUT_STACK 10240
// #define XML_MEMORY_XPATH_PAGE_SIZE 4096

#endif

#ifndef XML_VERSION
// Define version macro; evaluates to major * 100 + minor so that it's safe to use in less-than comparisons
/// @def XML_VERSION This correlates to the version of pugixml this uses
/// @brief
#	define XML_VERSION 120
#endif

#include "datatypes.h"
#include "XML/objectrange.h"
#include "XML/attribute.h"
#include "XML/attributeiterator.h"
#include "XML/document.h"
#include "XML/nodeiterator.h"
#include "XML/nodetext.h"
#include "XML/node.h"
#include "XML/parseresult.h"
#include "XML/treewalker.h"
#include "XML/writer.h"
#include "XML/xmldoc.h"
#include "XML/xmlenumerations.h"
#include "XML/xpathnode.h"
#include "XML/xpathparseresult.h"
#include "XML/xpathvariable.h"
#include "XML/xpathvariableset.h"
#include "XML/xpathquery.h"

#include "exception.h"

#ifndef SWIG
#include "Resource/datastream.h"
#endif

#ifndef _XML_H
#define _XML_H

#ifndef SWIG
    // Include stddef.h for size_t and ptrdiff_t
    #include <stddef.h>

    // Include STL headers
    #include <iterator>
    #include <iosfwd>
    #include <string>
#endif

namespace Mezzanine {


// The PugiXML namespace
namespace XML
{




//////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @todo Do something about forward declares in xml.h

    // Forward declarationsxmlboilerplate
	struct AttributeStruct;
	struct NodeStruct;

	class NodeIterator;
	class AttributeIterator;
    class NamedNodeIterator;

	class TreeWalker;

	class Node;

    class NodeText;

	class XPathNode;
	class XPathNodeSet;
	class XPathQuery;
	class XPathVariableSet;

	// Range-based for loop support

////////////////////////////////////////////////////////////////////////////////////////////////////// Here and up is done



	// A fixed-size collection of XPath nodes
	class MEZZ_LIB XPathNodeSet
	{
	public:
		// Collection type
		enum CollectionType
		{
			TypeUnsorted,			// Not ordered
			TypeSorted,			// Sorted by document order (ascending)
			TypeSortedReverse		// Sorted by document order (descending)
		};

		// Constant iterator type
		typedef const XPathNode* const_iterator;

		// Default constructor. Constructs empty set.
		XPathNodeSet();

		// Constructs a set from iterator range; data is not checked for duplicates and is not sorted according to provided Type, so be careful
		XPathNodeSet(const_iterator begin, const_iterator end, CollectionType Type = TypeUnsorted);

		// Destructor
		~XPathNodeSet();

		// Copy constructor/assignment operator
		XPathNodeSet(const XPathNodeSet& ns);
		XPathNodeSet& operator=(const XPathNodeSet& ns);

		// Get collection type
		CollectionType Type() const;

		// Get collection size
		/// @brief Get collection size.
		/// @return The number of XPathNode instances as a size_t in the set
		size_t size() const;

		// Indexing operator
		const XPathNode& operator[](size_t index) const;

		// Collection iterators
		const_iterator begin() const;
		const_iterator end() const;

		// Sort the collection in ascending/descending order by document order
		void sort(bool reverse = false);

		// Get first node in the collection by document order
		XPathNode first() const;

		// Check if collection is empty
		bool Empty() const;

	private:
		CollectionType _type;

		XPathNode _storage;

		XPathNode* _begin;
		XPathNode* _end;

		void _assign(const_iterator begin, const_iterator end);
	};

	// Convert wide string to UTF8
	std::basic_string<char, std::char_traits<char>, std::allocator<char> > MEZZ_LIB AsUtf8(const wchar_t* str);
	std::basic_string<char, std::char_traits<char>, std::allocator<char> > MEZZ_LIB AsUtf8(const std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >& str);

	// Convert UTF8 to wide string
	std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > MEZZ_LIB AsWide(const char* str);
	std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > MEZZ_LIB AsWide(const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& str);


	// Memory allocation function interface; returns pointer to allocated memory or NULL on failure
	typedef void* (*AllocationFunction)(size_t size);

	// Memory deallocation function interface
	typedef void (*deAllocationFunction)(void* ptr);

	// Override default memory management functions. All subsequent allocations/deallocations will be performed via supplied functions.
    void MEZZ_LIB SetMemoryManagementFunctions(AllocationFunction allocate, deAllocationFunction deallocate);

	// Get current memory management functions
	AllocationFunction MEZZ_LIB GetMemoryAllocationFunction();
	deAllocationFunction MEZZ_LIB GetMemoryDeallocationFunction();
}
//Name spaces end here
}

#endif

/*
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
