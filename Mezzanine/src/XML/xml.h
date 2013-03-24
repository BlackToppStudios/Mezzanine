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


    // XPath query return type
	enum XPathValueType
	{
		XPathTypeNone,	  // Unknown Type (query failed to compile)
		XPathTypeNodeSet,  // Node set (XPathNodeSet)
		XPathTypeNumber,	  // Number
		XPathTypeString,	  // String
		XPathTypeBoolean	  // Boolean
	};

	// XPath parsing Result
	struct MEZZ_LIB XPathParseResult
	{
		// Error message (0 if no error)
		const char* error;

		// Last parsed Offset (in char_t units from string start)
		ptrdiff_t Offset;

		/// @brief Default constructor, initializes object to failed state.
		XPathParseResult();

		/// @brief Cast to bool operator
		/// @return This returns true if the ParseResult::Status member is set to ParseStatus::StatusOk, otherwise this returns false.
		operator bool() const;

		// Get error Description
		const char* Description() const;
	};

	// A single XPath variable
	class MEZZ_LIB XPathVariable
	{
		friend class XPathVariableSet;

	protected:
		XPathValueType _type;
		XPathVariable* _next;

		XPathVariable();

		// Non-copyable semantics
		XPathVariable(const XPathVariable&);
		XPathVariable& operator=(const XPathVariable&);

	public:
		// Get variable name
        const Char8* Name() const;

		// Get variable type
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

		// Set variable Value; no Type conversion is performed, false is returned on Type mismatch error
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

	// A set of XPath variables
	class MEZZ_LIB XPathVariableSet
	{
	private:
		XPathVariable* _data[64];

		// Non-copyable semantics
		XPathVariableSet(const XPathVariableSet&);
		XPathVariableSet& operator=(const XPathVariableSet&);

        XPathVariable* find(const Char8* Name) const;

	public:
		// Default constructor/destructor
		XPathVariableSet();
		~XPathVariableSet();

		// Add a new variable or get the existing one, if the Types match
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

		// Get existing variable by name
        XPathVariable* Get(const Char8* Name);
        const XPathVariable* Get(const Char8* Name) const;
	};

	// A compiled XPath query object
	class MEZZ_LIB XPathQuery
	{
	private:
		void* _impl;
		XPathParseResult _Result;

		typedef void (*unspecified_bool_type)(XPathQuery***);

		// Non-copyable semantics
		XPathQuery(const XPathQuery&);
		XPathQuery& operator=(const XPathQuery&);

	public:
		// Construct a compiled object from XPath expression.

		/// @brief Construct a compiled object from XPath expression.
		/// @param query The query in the form of a c-string style char_t array.
		/// @param variables Any extra data the query might need, passing a null pointer simply omits passing any arguments.
        /// @throw Throws XPathException on compilation errors.
        explicit XPathQuery(const Char8* query, XPathVariableSet* variables = 0);

		// Destructor
		/// @brief Destructor
		~XPathQuery();

		// Get query expression return type
		XPathValueType ReturnType() const;

		// Evaluate expression as boolean Value in the specified context; performs Type conversion if necessary.
        // throws std::bad_alloc on out of memory errors.
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

        // Get parsing Result
		const XPathParseResult& Result() const;

		/// @brief Used to convert this to a boolean value in a safe way
		/// @return Returns true if the internal data is set and false otherwise.
		operator unspecified_bool_type() const;

		// Borland C++ workaround
		bool operator!() const;
	};

	// XPath node class (either Node or Attribute)
	class MEZZ_LIB XPathNode
	{
	private:
		Node _node;
		Attribute _attribute;

		typedef void (*unspecified_bool_type)(XPathNode***);

	public:
		/// @brief Default constructor; constructs empty XPath node
		XPathNode();

		// Construct XPath node from XML node/attribute
		XPathNode(const Node& node);
		/// @brief Construct From a XML::Attribute.
		/// @param GetAttribute The XML::Attribute this handle should reference.
		/// @param GetParent The XML::Node the XML::Attribute is on.
		XPathNode(const Attribute& GetAttribute, const Node& GetParent);

		// Get node/GetAttribute, if any
		Node GetNode() const;
		Attribute GetAttribute() const;

		// Get GetParent of contained node/attribute
		Node GetParent() const;

		/// @brief Used to convert this to a boolean value in a safe way
		/// @return Returns true if the internal data is set and false otherwise.
		operator unspecified_bool_type() const;

		// Borland C++ workaround
		bool operator!() const;

		// Comparison operators
		/// @brief Called when comparing two XPathNode instances for equality.
		/// @param n The other XML::XPathNode to compare this Node against for equality.
		/// @return True if this and the other XML::XPathNode are referencing the same item, false otherwise;
		bool operator==(const XPathNode& n) const;

		/// @brief Called when comparing two XPathNode instances for inequality.
		/// @param n The other XML::XPathNode to compare this Node against for inequality.
		/// @return True if this and the other XML::XPathNode are referencing the same item, false otherwise;
		bool operator!=(const XPathNode& n) const;
	};

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
	void MEZZ_LIB SetMemory_management_functions(AllocationFunction allocate, deAllocationFunction deallocate);

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
