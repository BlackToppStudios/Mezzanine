//© Copyright 2010 Joseph Toppi and John Blackwood 
/* This file is part of The PhysGame Engine. 
 
	The PhysGame Engine is free software: you can redistribute it and/or modify 
	it under the terms of the GNU General Public License as published by 
	the Free Software Foundation, either version 3 of the License, or 
	(at your option) any later version. 
 
	The PhysGame Engine is distributed in the hope that it will be useful, 
	but WITHOUT ANY WARRANTY; without even the implied warranty of 
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
	GNU General Public License for more details. 
 
	You should have received a copy of the GNU General Public License 
	along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.  
*/ 
/* The original authors have included a copy of the license specified above in the 
   'Docs' folder. See 'gpl.txt' 
*/ 
/* We welcome the use of The PhysGame anyone, including companies who wish to  
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
 * Copyright © 2006-2010, by Arseny Kapoulkine (arseny.kapoulkine@gmail.com) 
 * Report bugs and download new versions at http://pugixml.org/ 
 * 
 * This library is distributed under the MIT License. See notice at the end 
 * of this file. 
 * 
 * This work is based on the pugxml parser, which is: 
 * Copyright © 2003, by Kristen Wegner (kristen@tima.net) 
 */ 
 
#ifndef _xmlconfig_h 
#define _xmlconfig_h 
 
// Uncomment this to enable wchar_t mode 
// #define XML_WCHAR_MODE 
 
// Uncomment this to disable XPath 
// #define XML_NO_XPATH 
 
// Uncomment this to disable STL 
// Note: you can't use XPath with XML_NO_STL 
// #define XML_NO_STL 
 
// Uncomment this to disable exceptions 
// Note: you can't use XPath with XML_NO_EXCEPTIONS 
// #define XML_NO_EXCEPTIONS 
 
// Set this to control attributes for public classes/functions, i.e.: 
// #define XML_API __declspec(dllexport) // to export all public symbols from DLL 
// #define XML_CLASS __declspec(dllimport) // to import all classes from DLL 
// #define XML_FUNCTION __fastcall // to set calling conventions to all public functions to fastcall 
// In absence of XML_CLASS/XML_FUNCTION definitions XML_API is used instead 
 
#endif 
 
#ifndef _H 
#define _H 
 
 
 
#ifndef XML_NO_STL 
namespace std 
{ 
	struct bidirectional_iterator_tag; 
 
#ifdef __SUNPRO_CC 
	// Sun C++ compiler has a bug which forces template argument names in forward declarations to be the same as in actual definitions 
	template <class _T> class allocator; 
	template <class _charT> struct char_traits; 
	template <class _charT, class _Traits> class basic_istream; 
	template <class _charT, class _Traits> class basic_ostream; 
	template <class _charT, class _Traits, class _Allocator> class basic_string; 
#else 
	// Borland C++ compiler has a bug which forces template argument names in forward declarations to be the same as in actual definitions 
	template <class _Ty> class allocator; 
	template <class _Ty> struct char_traits; 
	template <class _Elem, class _Traits> class basic_istream; 
	template <class _Elem, class _Traits> class basic_ostream; 
	template <class _Elem, class _Traits, class _Ax> class basic_string; 
#endif 
 
	// Digital Mars compiler has a bug which requires a forward declaration for explicit instantiation (otherwise type selection is messed up later, producing link errors) 
	// Also note that we have to declare char_traits as a class here, since it's defined that way 
#ifdef __DMC__ 
	template <> class char_traits<char>; 
#endif 
} 
#endif 
 
// Macro for deprecated features 
#ifndef XML_DEPRECATED 
#	if defined(__GNUC__) 
#		define XML_DEPRECATED __attribute__((deprecated)) 
#	elif defined(_MSC_VER) && _MSC_VER >= 1300 
#		define XML_DEPRECATED __declspec(deprecated) 
#	else 
#		define XML_DEPRECATED 
#	endif 
#endif 
 
// Include exception header for XPath 
#if !defined(XML_NO_XPATH) && !defined(XML_NO_EXCEPTIONS) 
#	include <exception> 
#endif 
 
// If no API is defined, assume default 
#ifndef XML_API 
#   define XML_API 
#endif 
 
// If no API for classes is defined, assume default 
#ifndef XML_CLASS 
#   define XML_CLASS XML_API 
#endif 
 
// If no API for functions is defined, assume default 
#ifndef XML_FUNCTION 
#   define XML_FUNCTION XML_API 
#endif 
 
#include <stddef.h> 
 
// Character interface macros 
#ifdef XML_WCHAR_MODE 
#	define XML_TEXT(t) L ## t 
#	define XML_CHAR wchar_t 
#else 
#	define XML_TEXT(t) t 
#	define XML_CHAR char 
#endif 
 

namespace phys
{ namespace xml
{ 
	// Character type used for all internal storage and operations; depends on XML_WCHAR_MODE 
	typedef XML_CHAR char_t; 
 
#ifndef XML_NO_STL 
	// String type used for operations that work with STL string; depends on XML_WCHAR_MODE 
	typedef std::basic_string<XML_CHAR, std::char_traits<XML_CHAR>, std::allocator<XML_CHAR> > string_t; 
#endif 
} 
} // \phys
 
// The PugiXML namespace 

namespace phys
{ namespace xml
{ 
	// Tree node types 
	enum Node_Type 
	{ 
		node_null,		  // Empty (null) node handle 
		node_document,		// A document tree's absolute root 
		node_element,		// Element tag, i.e. '<node/>' 
		node_pcdata,		// Plain character data, i.e. 'text' 
		node_cdata,			// Character data, i.e. '<![CDATA[text]]>' 
		node_comment,		// Comment tag, i.e. '<!-- text -->' 
		node_pi,			// Processing instruction, i.e. '<?name?>' 
		node_declaration,	// Document declaration, i.e. '<?xml version="1.0"?>' 
		node_doctype		// Document type declaration, i.e. '<!DOCTYPE doc>' 
	}; 
 
	// Parsing options 
 
	// Minimal parsing mode (equivalent to turning all other flags off). 
	// Only elements and PCDATA sections are added to the DOM tree, no text conversions are performed. 
	const unsigned int parse_minimal = 0x0000; 
 
	// This flag determines if processing instructions (node_pi) are added to the DOM tree. This flag is off by default. 
	const unsigned int parse_pi = 0x0001; 
 
	// This flag determines if comments (node_comment) are added to the DOM tree. This flag is off by default. 
	const unsigned int parse_comments = 0x0002; 
 
	// This flag determines if CDATA sections (node_cdata) are added to the DOM tree. This flag is on by default. 
	const unsigned int parse_cdata = 0x0004; 
 
	// This flag determines if plain character data (node_pcdata) that consist only of whitespace are added to the DOM tree. 
	// This flag is off by default; turning it on usually results in slower parsing and more memory consumption. 
	const unsigned int parse_ws_pcdata = 0x0008; 
 
	// This flag determines if character and entity references are expanded during parsing. This flag is on by default. 
	const unsigned int parse_escapes = 0x0010; 
 
	// This flag determines if EOL characters are normalized (converted to #xA) during parsing. This flag is on by default. 
	const unsigned int parse_eol = 0x0020; 
	 
 	// This flag determines if attribute values are normalized using CDATA normalization rules during parsing. This flag is on by default. 
 	const unsigned int parse_wconv_attribute = 0x0040; 
 
 	// This flag determines if attribute values are normalized using NMTOKENS normalization rules during parsing. This flag is off by default. 
 	const unsigned int parse_wnorm_attribute = 0x0080; 
	 
	// This flag determines if document declaration (node_declaration) is added to the DOM tree. This flag is off by default. 
	const unsigned int parse_declaration = 0x0100; 
 
	// This flag determines if document type declaration (node_doctype) is added to the DOM tree. This flag is off by default. 
	const unsigned int parse_doctype = 0x0200; 
 
	// The default parsing mode. 
	// Elements, PCDATA and CDATA sections are added to the DOM tree, character/reference entities are expanded, 
	// End-of-Line characters are normalized, attribute values are normalized using CDATA normalization rules. 
	const unsigned int parse_default = parse_cdata | parse_escapes | parse_wconv_attribute | parse_eol; 
 
	// The full parsing mode. 
	// Nodes of all types are added to the DOM tree, character/reference entities are expanded, 
	// End-of-Line characters are normalized, attribute values are normalized using CDATA normalization rules. 
	const unsigned int parse_full = parse_default | parse_pi | parse_comments | parse_declaration | parse_doctype; 
 
	// These flags determine the encoding of input data for XML document 
	enum Encoding 
	{ 
		encoding_auto,	  // Auto-detect input encoding using BOM or < / <? detection; use UTF8 if BOM is not found 
		encoding_utf8,	  // UTF8 encoding 
		encoding_utf16_le,  // Little-endian UTF16 
		encoding_utf16_be,  // Big-endian UTF16 
		encoding_utf16,	 // UTF16 with native endianness 
		encoding_utf32_le,  // Little-endian UTF32 
		encoding_utf32_be,  // Big-endian UTF32 
		encoding_utf32,	 // UTF32 with native endianness 
		encoding_wchar	  // The same encoding wchar_t has (either UTF16 or UTF32) 
	}; 
 
	// Formatting flags 
	 
	// Indent the nodes that are written to output stream with as many indentation strings as deep the node is in DOM tree. This flag is on by default. 
	const unsigned int format_indent = 0x01; 
	 
	// Write encoding-specific BOM to the output stream. This flag is off by default. 
	const unsigned int format_write_bom = 0x02; 
 
	// Use raw output mode (no indentation and no line breaks are written). This flag is off by default. 
	const unsigned int format_raw = 0x04; 
	 
	// Omit default XML declaration even if there is no declaration in the document. This flag is off by default. 
	const unsigned int format_no_declaration = 0x08; 
 
	// The default set of formatting flags. 
	// Nodes are indented depending on their depth in DOM tree, a default declaration is output if document has none. 
	const unsigned int format_default = format_indent; 
		 
	// Forward declarations 
	struct Attribute_Struct; 
	struct Node_Struct; 
 
	class Node_Iterator; 
	class Attribute_Iterator; 
 
	class Tree_Walker; 
	 
	class Node; 
 
	#ifndef XML_NO_XPATH 
	class xpath_node; 
	class xpath_node_set; 
	class xpath_query; 
	class xpath_variable_set; 
	#endif 
 
	// Writer interface for node printing (see Node::print) 
	class XML_CLASS Writer 
	{ 
	public: 
		virtual ~Writer() {} 
 
		// Write memory chunk into stream/file/whatever 
		virtual void write(const void* data, size_t size) = 0; 
	}; 
 
	// Writer implementation for FILE* 
	class XML_CLASS Writer_File: public Writer 
	{ 
	public: 
		// Construct writer from a FILE* object; void* is used to avoid header dependencies on stdio 
		Writer_File(void* file); 
 
		virtual void write(const void* data, size_t size); 
 
	private: 
		void* file; 
	}; 
 
	#ifndef XML_NO_STL 
	// Writer implementation for streams 
	class XML_CLASS Writer_Stream: public Writer 
	{ 
	public: 
		// Construct writer from an output stream object 
		Writer_Stream(std::basic_ostream<char, std::char_traits<char> >& stream); 
		Writer_Stream(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& stream); 
 
		virtual void write(const void* data, size_t size); 
 
	private: 
		std::basic_ostream<char, std::char_traits<char> >* narrow_stream; 
		std::basic_ostream<wchar_t, std::char_traits<wchar_t> >* wide_stream; 
	}; 
	#endif 
 
	// A light-weight handle for manipulating attributes in DOM tree 
	class XML_CLASS Attribute 
	{ 
		friend class Attribute_Iterator; 
		friend class Node; 
 
	private: 
		Attribute_Struct* _attr; 
	 
		typedef Attribute_Struct* Attribute::*unspecified_bool_type; 
 
	public: 
		// Default constructor. Constructs an empty attribute. 
		Attribute(); 
		 
		// Constructs attribute from internal pointer 
		explicit Attribute(Attribute_Struct* attr); 
 
		// Safe bool conversion operator 
		operator unspecified_bool_type() const; 
 
		// Borland C++ workaround 
		bool operator!() const; 
 
		// Comparison operators (compares wrapped attribute pointers) 
		bool operator==(const Attribute& r) const; 
		bool operator!=(const Attribute& r) const; 
		bool operator<(const Attribute& r) const; 
		bool operator>(const Attribute& r) const; 
		bool operator<=(const Attribute& r) const; 
		bool operator>=(const Attribute& r) const; 
 
		// Check if attribute is empty 
		bool empty() const; 
 
		// Get attribute name/value, or "" if attribute is empty 
		const char_t* name() const; 
		const char_t* value() const; 
 
		// Get attribute value as a number, or 0 if conversion did not succeed or attribute is empty 
		int as_int() const; 
		unsigned int as_uint() const; 
		double as_double() const; 
		float as_float() const; 
 
		// Get attribute value as bool (returns true if first character is in '1tTyY' set), or false if attribute is empty 
		bool as_bool() const; 
 
		// Set attribute name/value (returns false if attribute is empty or there is not enough memory) 
		bool set_name(const char_t* rhs); 
		bool set_value(const char_t* rhs); 
 
		// Set attribute value with type conversion (numbers are converted to strings, boolean is converted to "true"/"false") 
		bool set_value(int rhs); 
		bool set_value(unsigned int rhs); 
		bool set_value(double rhs); 
		bool set_value(bool rhs); 
 
		// Set attribute value (equivalent to set_value without error checking) 
		Attribute& operator=(const char_t* rhs); 
		Attribute& operator=(int rhs); 
		Attribute& operator=(unsigned int rhs); 
		Attribute& operator=(double rhs); 
		Attribute& operator=(bool rhs); 
 
		// Get next/previous attribute in the attribute list of the parent node 
		Attribute next_attribute() const; 
		Attribute previous_attribute() const; 
 
		// Get hash value (unique for handles to the same object) 
		size_t hash_value() const; 
 
		// Get internal pointer 
		Attribute_Struct* internal_object() const; 
	}; 
 
#ifdef __BORLANDC__ 
	// Borland C++ workaround 
	bool XML_FUNCTION operator&&(const Attribute& lhs, bool rhs); 
	bool XML_FUNCTION operator||(const Attribute& lhs, bool rhs); 
#endif 
 
	// A light-weight handle for manipulating nodes in DOM tree 
	class XML_CLASS Node 
	{ 
		friend class Attribute_Iterator; 
		friend class Node_Iterator; 
 
	protected: 
		Node_Struct* _root; 
 
		typedef Node_Struct* Node::*unspecified_bool_type; 
 
	public: 
		// Default constructor. Constructs an empty node. 
		Node(); 
 
		// Constructs node from internal pointer 
		explicit Node(Node_Struct* p); 
 
		// Safe bool conversion operator 
		operator unspecified_bool_type() const; 
 
		// Borland C++ workaround 
		bool operator!() const; 
	 
		// Comparison operators (compares wrapped node pointers) 
		bool operator==(const Node& r) const; 
		bool operator!=(const Node& r) const; 
		bool operator<(const Node& r) const; 
		bool operator>(const Node& r) const; 
		bool operator<=(const Node& r) const; 
		bool operator>=(const Node& r) const; 
 
		// Check if node is empty. 
		bool empty() const; 
 
		// Get node type 
		Node_Type type() const; 
 
		// Get node name/value, or "" if node is empty or it has no name/value 
		const char_t* name() const; 
		const char_t* value() const; 
	 
		// Get attribute list 
		Attribute first_attribute() const; 
		Attribute last_attribute() const; 
 
		// Get children list 
		Node first_child() const; 
		Node last_child() const; 
 
		// Get next/previous sibling in the children list of the parent node 
		Node next_sibling() const; 
		Node previous_sibling() const; 
		 
		// Get parent node 
		Node parent() const; 
 
		// Get root of DOM tree this node belongs to 
		Node root() const; 
 
		// Get child, attribute or next/previous sibling with the specified name 
		Node child(const char_t* name) const; 
		Attribute attribute(const char_t* name) const; 
		Node next_sibling(const char_t* name) const; 
		Node previous_sibling(const char_t* name) const; 
 
		// Get child value of current node; that is, value of the first child node of type PCDATA/CDATA 
		const char_t* child_value() const; 
 
		// Get child value of child with specified name. Equivalent to child(name).child_value(). 
		const char_t* child_value(const char_t* name) const; 
 
		// Set node name/value (returns false if node is empty, there is not enough memory, or node can not have name/value) 
		bool set_name(const char_t* rhs); 
		bool set_value(const char_t* rhs); 
		 
		// Add attribute with specified name. Returns added attribute, or empty attribute on errors. 
		Attribute append_attribute(const char_t* name); 
		Attribute prepend_attribute(const char_t* name); 
		Attribute insert_attribute_after(const char_t* name, const Attribute& attr); 
		Attribute insert_attribute_before(const char_t* name, const Attribute& attr); 
 
		// Add a copy of the specified attribute. Returns added attribute, or empty attribute on errors. 
		Attribute append_copy(const Attribute& proto); 
		Attribute prepend_copy(const Attribute& proto); 
		Attribute insert_copy_after(const Attribute& proto, const Attribute& attr); 
		Attribute insert_copy_before(const Attribute& proto, const Attribute& attr); 
 
		// Add child node with specified type. Returns added node, or empty node on errors. 
		Node append_child(Node_Type type = node_element); 
		Node prepend_child(Node_Type type = node_element); 
		Node insert_child_after(Node_Type type, const Node& node); 
		Node insert_child_before(Node_Type type, const Node& node); 
 
		// Add child element with specified name. Returns added node, or empty node on errors. 
		Node append_child(const char_t* name); 
		Node prepend_child(const char_t* name); 
		Node insert_child_after(const char_t* name, const Node& node); 
		Node insert_child_before(const char_t* name, const Node& node); 
 
		// Add a copy of the specified node as a child. Returns added node, or empty node on errors. 
		Node append_copy(const Node& proto); 
		Node prepend_copy(const Node& proto); 
		Node insert_copy_after(const Node& proto, const Node& node); 
		Node insert_copy_before(const Node& proto, const Node& node); 
 
		// Remove specified attribute 
		bool remove_attribute(const Attribute& a); 
		bool remove_attribute(const char_t* name); 
 
		// Remove specified child 
		bool remove_child(const Node& n); 
		bool remove_child(const char_t* name); 
 
		// Find attribute using predicate. Returns first attribute for which predicate returned true. 
		template <typename Predicate> Attribute find_attribute(Predicate pred) const 
		{ 
			if (!_root) return Attribute(); 
			 
			for (Attribute attrib = first_attribute(); attrib; attrib = attrib.next_attribute()) 
				if (pred(attrib)) 
					return attrib; 
		 
			return Attribute(); 
		} 
 
		// Find child node using predicate. Returns first child for which predicate returned true. 
		template <typename Predicate> Node find_child(Predicate pred) const 
		{ 
			if (!_root) return Node(); 
	 
			for (Node node = first_child(); node; node = node.next_sibling()) 
				if (pred(node)) 
					return node; 
		 
			return Node(); 
		} 
 
		// Find node from subtree using predicate. Returns first node from subtree (depth-first), for which predicate returned true. 
		template <typename Predicate> Node find_node(Predicate pred) const 
		{ 
			if (!_root) return Node(); 
 
			Node cur = first_child(); 
			 
			while (cur._root && cur._root != _root) 
			{ 
				if (pred(cur)) return cur; 
 
				if (cur.first_child()) cur = cur.first_child(); 
				else if (cur.next_sibling()) cur = cur.next_sibling(); 
				else 
				{ 
					while (!cur.next_sibling() && cur._root != _root) cur = cur.parent(); 
 
					if (cur._root != _root) cur = cur.next_sibling(); 
				} 
			} 
 
			return Node(); 
		} 
 
		// Find child node by attribute name/value 
		Node find_child_by_attribute(const char_t* name, const char_t* attr_name, const char_t* attr_value) const; 
		Node find_child_by_attribute(const char_t* attr_name, const char_t* attr_value) const; 
 
	#ifndef XML_NO_STL 
		// Get the absolute node path from root as a text string. 
		string_t path(char_t delimiter = '/') const; 
	#endif 
 
		// Search for a node by path consisting of node names and . or .. elements. 
		Node first_element_by_path(const char_t* path, char_t delimiter = '/') const; 
 
		// Recursively traverse subtree with Tree_Walker 
		bool traverse(Tree_Walker& walker); 
	 
	#ifndef XML_NO_XPATH 
		// Select single node by evaluating XPath query. Returns first node from the resulting node set. 
		xpath_node select_single_node(const char_t* query, xpath_variable_set* variables = 0) const; 
		xpath_node select_single_node(const xpath_query& query) const; 
 
		// Select node set by evaluating XPath query 
		xpath_node_set select_nodes(const char_t* query, xpath_variable_set* variables = 0) const; 
		xpath_node_set select_nodes(const xpath_query& query) const; 
	#endif 
		 
		// Print subtree using a writer object 
		void print(Writer& writer, const char_t* indent = XML_TEXT("\t"), unsigned int flags = format_default, Encoding encoding = encoding_auto, unsigned int depth = 0) const; 
 
	#ifndef XML_NO_STL 
		// Print subtree to stream 
		void print(std::basic_ostream<char, std::char_traits<char> >& os, const char_t* indent = XML_TEXT("\t"), unsigned int flags = format_default, Encoding encoding = encoding_auto, unsigned int depth = 0) const; 
		void print(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& os, const char_t* indent = XML_TEXT("\t"), unsigned int flags = format_default, unsigned int depth = 0) const; 
	#endif 
 
		// Child nodes iterators 
		typedef Node_Iterator iterator; 
 
		iterator begin() const; 
		iterator end() const; 
 
		// Attribute iterators 
		typedef Attribute_Iterator attribute_iterator; 
 
		attribute_iterator attributes_begin() const; 
		attribute_iterator attributes_end() const; 
 
		// Get node offset in parsed file/string (in char_t units) for debugging purposes 
		ptrdiff_t offset_debug() const; 
 
		// Get hash value (unique for handles to the same object) 
		size_t hash_value() const; 
 
		// Get internal pointer 
		Node_Struct* internal_object() const; 
	}; 
 
#ifdef __BORLANDC__ 
	// Borland C++ workaround 
	bool XML_FUNCTION operator&&(const Node& lhs, bool rhs); 
	bool XML_FUNCTION operator||(const Node& lhs, bool rhs); 
#endif 
 
	// Child node iterator (a bidirectional iterator over a collection of Node) 
	class XML_CLASS Node_Iterator 
	{ 
		friend class Node; 
 
	private: 
		Node _wrap; 
		Node _parent; 
 
		Node_Iterator(Node_Struct* ref, Node_Struct* parent); 
 
	public: 
		// Iterator traits 
		typedef ptrdiff_t difference_type; 
		typedef Node value_type; 
		typedef Node* pointer; 
		typedef Node& reference; 
 
	#ifndef XML_NO_STL 
		typedef std::bidirectional_iterator_tag iterator_category; 
	#endif 
 
		// Default constructor 
		Node_Iterator(); 
 
		// Construct an iterator which points to the specified node 
		Node_Iterator(const Node& node); 
 
		// Iterator operators 
		bool operator==(const Node_Iterator& rhs) const; 
		bool operator!=(const Node_Iterator& rhs) const; 
 
		Node& operator*(); 
		Node* operator->(); 
 
		const Node_Iterator& operator++(); 
		Node_Iterator operator++(int); 
 
		const Node_Iterator& operator--(); 
		Node_Iterator operator--(int); 
	}; 
 
	// Attribute iterator (a bidirectional iterator over a collection of Attribute) 
	class XML_CLASS Attribute_Iterator 
	{ 
		friend class Node; 
 
	private: 
		Attribute _wrap; 
		Node _parent; 
 
		Attribute_Iterator(Attribute_Struct* ref, Node_Struct* parent); 
 
	public: 
		// Iterator traits 
		typedef ptrdiff_t difference_type; 
		typedef Attribute value_type; 
		typedef Attribute* pointer; 
		typedef Attribute& reference; 
 
	#ifndef XML_NO_STL 
		typedef std::bidirectional_iterator_tag iterator_category; 
	#endif 
 
		// Default constructor 
		Attribute_Iterator(); 
 
		// Construct an iterator which points to the specified attribute 
		Attribute_Iterator(const Attribute& attr, const Node& parent); 
 
		// Iterator operators 
		bool operator==(const Attribute_Iterator& rhs) const; 
		bool operator!=(const Attribute_Iterator& rhs) const; 
 
		Attribute& operator*(); 
		Attribute* operator->(); 
 
		const Attribute_Iterator& operator++(); 
		Attribute_Iterator operator++(int); 
 
		const Attribute_Iterator& operator--(); 
		Attribute_Iterator operator--(int); 
	}; 
 
	// Abstract tree walker class (see Node::traverse) 
	class XML_CLASS Tree_Walker 
	{ 
		friend class Node; 
 
	private: 
		int _depth; 
	 
	protected: 
		// Get current traversal depth 
		int depth() const; 
	 
	public: 
		Tree_Walker(); 
		virtual ~Tree_Walker(); 
 
		// Callback that is called when traversal begins 
		virtual bool begin(Node& node); 
 
		// Callback that is called for each node traversed 
		virtual bool for_each(Node& node) = 0; 
 
		// Callback that is called when traversal ends 
		virtual bool end(Node& node); 
	}; 
 
	// Parsing status, returned as part of Parse_Result object 
	enum Parse_Status 
	{ 
		status_ok = 0,			  // No error 
 
		status_file_not_found,	  // File was not found during load_file() 
		status_io_error,			// Error reading from file/stream 
		status_out_of_memory,	   // Could not allocate memory 
		status_internal_error,	  // Internal error occurred 
 
		status_unrecognized_tag,	// Parser could not determine tag type 
 
		status_bad_pi,			  // Parsing error occurred while parsing document declaration/processing instruction 
		status_bad_comment,		 // Parsing error occurred while parsing comment 
		status_bad_cdata,		   // Parsing error occurred while parsing CDATA section 
		status_bad_doctype,		 // Parsing error occurred while parsing document type declaration 
		status_bad_pcdata,		  // Parsing error occurred while parsing PCDATA section 
		status_bad_start_element,   // Parsing error occurred while parsing start element tag 
		status_bad_attribute,	   // Parsing error occurred while parsing element attribute 
		status_bad_end_element,	 // Parsing error occurred while parsing end element tag 
		status_end_element_mismatch // There was a mismatch of start-end tags (closing tag had incorrect name, some tag was not closed or there was an excessive closing tag) 
	}; 
 
	// Parsing result 
	struct XML_CLASS Parse_Result 
	{ 
		// Parsing status (see Parse_Status) 
		Parse_Status status; 
 
		// Last parsed offset (in char_t units from start of input data) 
		ptrdiff_t offset; 
 
		// Source document encoding 
		Encoding encoding; 
 
		// Default constructor, initializes object to failed state 
		Parse_Result(); 
 
		// Cast to bool operator 
		operator bool() const; 
 
		// Get error description 
		const char* description() const; 
	}; 
 
	// Document class (DOM tree root) 
	class XML_CLASS Document: public Node 
	{ 
	private: 
		char_t* _buffer; 
 
		char _memory[192]; 
		 
		// Non-copyable semantics 
		Document(const Document&); 
		const Document& operator=(const Document&); 
 
		void create(); 
		void destroy(); 
 
		Parse_Result load_buffer_impl(void* contents, size_t size, unsigned int options, Encoding encoding, bool is_mutable, bool own); 
 
	public: 
		// Default constructor, makes empty document 
		Document(); 
 
		// Destructor, invalidates all node/attribute handles to this document 
		~Document(); 
 
		// Removes all nodes, leaving the empty document 
		void reset(); 
 
		// Removes all nodes, then copies the entire contents of the specified document 
		void reset(const Document& proto); 
 
	#ifndef XML_NO_STL 
		// Load document from stream. 
		Parse_Result load(std::basic_istream<char, std::char_traits<char> >& stream, unsigned int options = parse_default, Encoding encoding = encoding_auto); 
		Parse_Result load(std::basic_istream<wchar_t, std::char_traits<wchar_t> >& stream, unsigned int options = parse_default); 
	#endif 
 
		// Load document from zero-terminated string. No encoding conversions are applied. 
		Parse_Result load(const char_t* contents, unsigned int options = parse_default); 
 
		// Load document from file 
		Parse_Result load_file(const char* path, unsigned int options = parse_default, Encoding encoding = encoding_auto); 
		Parse_Result load_file(const wchar_t* path, unsigned int options = parse_default, Encoding encoding = encoding_auto); 
 
		// Load document from buffer. Copies/converts the buffer, so it may be deleted or changed after the function returns. 
		Parse_Result load_buffer(const void* contents, size_t size, unsigned int options = parse_default, Encoding encoding = encoding_auto); 
 
		// Load document from buffer, using the buffer for in-place parsing (the buffer is modified and used for storage of document data). 
		// You should ensure that buffer data will persist throughout the document's lifetime, and free the buffer memory manually once document is destroyed. 
		Parse_Result load_buffer_inplace(void* contents, size_t size, unsigned int options = parse_default, Encoding encoding = encoding_auto); 
 
		// Load document from buffer, using the buffer for in-place parsing (the buffer is modified and used for storage of document data). 
		// You should allocate the buffer with pugixml allocation function; document will free the buffer when it is no longer needed (you can't use it anymore). 
		Parse_Result load_buffer_inplace_own(void* contents, size_t size, unsigned int options = parse_default, Encoding encoding = encoding_auto); 
 
		// Save XML document to writer (semantics is slightly different from Node::print, see documentation for details). 
		void save(Writer& writer, const char_t* indent = XML_TEXT("\t"), unsigned int flags = format_default, Encoding encoding = encoding_auto) const; 
 
	#ifndef XML_NO_STL 
		// Save XML document to stream (semantics is slightly different from Node::print, see documentation for details). 
		void save(std::basic_ostream<char, std::char_traits<char> >& stream, const char_t* indent = XML_TEXT("\t"), unsigned int flags = format_default, Encoding encoding = encoding_auto) const; 
		void save(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& stream, const char_t* indent = XML_TEXT("\t"), unsigned int flags = format_default) const; 
	#endif 
 
		// Save XML to file 
		bool save_file(const char* path, const char_t* indent = XML_TEXT("\t"), unsigned int flags = format_default, Encoding encoding = encoding_auto) const; 
		bool save_file(const wchar_t* path, const char_t* indent = XML_TEXT("\t"), unsigned int flags = format_default, Encoding encoding = encoding_auto) const; 
 
		// Get document element 
		Node document_element() const; 
	}; 
 
#ifndef XML_NO_XPATH 
	// XPath query return type 
	enum xpath_value_type 
	{ 
		xpath_type_none,	  // Unknown type (query failed to compile) 
		xpath_type_node_set,  // Node set (xpath_node_set) 
		xpath_type_number,	// Number 
		xpath_type_string,	// String 
		xpath_type_boolean	// Boolean 
	}; 
 
	// XPath parsing result 
	struct XML_CLASS xpath_parse_result 
	{ 
		// Error message (0 if no error) 
		const char* error; 
 
		// Last parsed offset (in char_t units from string start) 
		ptrdiff_t offset; 
 
		// Default constructor, initializes object to failed state 
		xpath_parse_result(); 
 
		// Cast to bool operator 
		operator bool() const; 
 
		// Get error description 
		const char* description() const; 
	}; 
 
	// A single XPath variable 
	class XML_CLASS xpath_variable 
	{ 
		friend class xpath_variable_set; 
 
	protected: 
		xpath_value_type _type; 
		xpath_variable* _next; 
 
		xpath_variable(); 
 
		// Non-copyable semantics 
		xpath_variable(const xpath_variable&); 
		xpath_variable& operator=(const xpath_variable&); 
		 
	public: 
		// Get variable name 
		const char_t* name() const; 
 
		// Get variable type 
		xpath_value_type type() const; 
 
		// Get variable value; no type conversion is performed, default value (false, NaN, empty string, empty node set) is returned on type mismatch error 
		bool get_boolean() const; 
		double get_number() const; 
		const char_t* get_string() const; 
		const xpath_node_set& get_node_set() const; 
 
		// Set variable value; no type conversion is performed, false is returned on type mismatch error 
		bool set(bool value); 
		bool set(double value); 
		bool set(const char_t* value); 
		bool set(const xpath_node_set& value); 
	}; 
 
	// A set of XPath variables 
	class XML_CLASS xpath_variable_set 
	{ 
	private: 
		xpath_variable* _data[64]; 
 
		// Non-copyable semantics 
		xpath_variable_set(const xpath_variable_set&); 
		xpath_variable_set& operator=(const xpath_variable_set&); 
 
		xpath_variable* find(const char_t* name) const; 
 
	public: 
		// Default constructor/destructor 
		xpath_variable_set(); 
		~xpath_variable_set(); 
 
		// Add a new variable or get the existing one, if the types match 
		xpath_variable* add(const char_t* name, xpath_value_type type); 
 
		// Set value of an existing variable; no type conversion is performed, false is returned if there is no such variable or if types mismatch 
		bool set(const char_t* name, bool value); 
		bool set(const char_t* name, double value); 
		bool set(const char_t* name, const char_t* value); 
		bool set(const char_t* name, const xpath_node_set& value); 
 
		// Get existing variable by name 
		xpath_variable* get(const char_t* name); 
		const xpath_variable* get(const char_t* name) const; 
	}; 
 
	// A compiled XPath query object 
	class XML_CLASS xpath_query 
	{ 
	private: 
		void* _impl; 
		xpath_parse_result _result; 
 
		typedef void* xpath_query::*unspecified_bool_type; 
 
		// Non-copyable semantics 
		xpath_query(const xpath_query&); 
		xpath_query& operator=(const xpath_query&); 
 
	public: 
		// Construct a compiled object from XPath expression. 
		// If XML_NO_EXCEPTIONS is not defined, throws xpath_exception on compilation errors. 
		explicit xpath_query(const char_t* query, xpath_variable_set* variables = 0); 
 
		// Destructor 
		~xpath_query(); 
 
		// Get query expression return type 
		xpath_value_type return_type() const; 
		 
		// Evaluate expression as boolean value in the specified context; performs type conversion if necessary. 
		// If XML_NO_EXCEPTIONS is not defined, throws std::bad_alloc on out of memory errors. 
		bool evaluate_boolean(const xpath_node& n) const; 
		 
		// Evaluate expression as double value in the specified context; performs type conversion if necessary. 
		// If XML_NO_EXCEPTIONS is not defined, throws std::bad_alloc on out of memory errors. 
		double evaluate_number(const xpath_node& n) const; 
		 
	#ifndef XML_NO_STL 
		// Evaluate expression as string value in the specified context; performs type conversion if necessary. 
		// If XML_NO_EXCEPTIONS is not defined, throws std::bad_alloc on out of memory errors. 
		string_t evaluate_string(const xpath_node& n) const; 
	#endif 
		 
		// Evaluate expression as string value in the specified context; performs type conversion if necessary. 
		// At most capacity characters are written to the destination buffer, full result size is returned (includes terminating zero). 
		// If XML_NO_EXCEPTIONS is not defined, throws std::bad_alloc on out of memory errors. 
		// If XML_NO_EXCEPTIONS is defined, returns empty  set instead. 
		size_t evaluate_string(char_t* buffer, size_t capacity, const xpath_node& n) const; 
 
		// Evaluate expression as node set in the specified context. 
		// If XML_NO_EXCEPTIONS is not defined, throws xpath_exception on type mismatch and std::bad_alloc on out of memory errors. 
		// If XML_NO_EXCEPTIONS is defined, returns empty node set instead. 
		xpath_node_set evaluate_node_set(const xpath_node& n) const; 
 
		// Get parsing result (used to get compilation errors in XML_NO_EXCEPTIONS mode) 
		const xpath_parse_result& result() const; 
 
		// Safe bool conversion operator 
		operator unspecified_bool_type() const; 
 
		// Borland C++ workaround 
		bool operator!() const; 
	}; 
	 
	#ifndef XML_NO_EXCEPTIONS 
	// XPath exception class 
	class XML_CLASS xpath_exception: public std::exception 
	{ 
	private: 
		xpath_parse_result _result; 
 
	public: 
		// Construct exception from parse result 
		explicit xpath_exception(const xpath_parse_result& result); 
 
		// Get error message 
		virtual const char* what() const throw(); 
 
		// Get parse result 
		const xpath_parse_result& result() const; 
	}; 
	#endif 
	 
	// XPath node class (either Node or Attribute) 
	class XML_CLASS xpath_node 
	{ 
	private: 
		Node _node; 
		Attribute _attribute; 
	 
		typedef Node xpath_node::*unspecified_bool_type; 
 
	public: 
		// Default constructor; constructs empty XPath node 
		xpath_node(); 
		 
		// Construct XPath node from XML node/attribute 
		xpath_node(const Node& node); 
		xpath_node(const Attribute& attribute, const Node& parent); 
 
		// Get node/attribute, if any 
		Node node() const; 
		Attribute attribute() const; 
		 
		// Get parent of contained node/attribute 
		Node parent() const; 
 
		// Safe bool conversion operator 
		operator unspecified_bool_type() const; 
		 
		// Borland C++ workaround 
		bool operator!() const; 
 
		// Comparison operators 
		bool operator==(const xpath_node& n) const; 
		bool operator!=(const xpath_node& n) const; 
	}; 
 
#ifdef __BORLANDC__ 
	// Borland C++ workaround 
	bool XML_FUNCTION operator&&(const xpath_node& lhs, bool rhs); 
	bool XML_FUNCTION operator||(const xpath_node& lhs, bool rhs); 
#endif 
 
	// A fixed-size collection of XPath nodes 
	class XML_CLASS xpath_node_set 
	{ 
	public: 
		// Collection type 
		enum type_t 
		{ 
			type_unsorted,			// Not ordered 
			type_sorted,			// Sorted by document order (ascending) 
			type_sorted_reverse		// Sorted by document order (descending) 
		}; 
		 
		// Constant iterator type 
		typedef const xpath_node* const_iterator; 
	 
		// Default constructor. Constructs empty set. 
		xpath_node_set(); 
 
		// Constructs a set from iterator range; data is not checked for duplicates and is not sorted according to provided type, so be careful 
		xpath_node_set(const_iterator begin, const_iterator end, type_t type = type_unsorted); 
 
		// Destructor 
		~xpath_node_set(); 
		 
		// Copy constructor/assignment operator 
		xpath_node_set(const xpath_node_set& ns); 
		xpath_node_set& operator=(const xpath_node_set& ns); 
 
		// Get collection type 
		type_t type() const; 
		 
		// Get collection size 
		size_t size() const; 
 
		// Indexing operator 
		const xpath_node& operator[](size_t index) const; 
		 
		// Collection iterators 
		const_iterator begin() const; 
		const_iterator end() const; 
 
		// Sort the collection in ascending/descending order by document order 
		void sort(bool reverse = false); 
		 
		// Get first node in the collection by document order 
		xpath_node first() const; 
		 
		// Check if collection is empty 
		bool empty() const; 
	 
	private: 
		type_t _type; 
		 
		xpath_node _storage; 
		 
		xpath_node* _begin; 
		xpath_node* _end; 
 
		void _assign(const_iterator begin, const_iterator end); 
	}; 
#endif 
 
#ifndef XML_NO_STL 
	// Convert wide string to UTF8 
	std::basic_string<char, std::char_traits<char>, std::allocator<char> > XML_FUNCTION as_utf8(const wchar_t* str); 
	std::basic_string<char, std::char_traits<char>, std::allocator<char> > XML_FUNCTION as_utf8(const std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >& str); 
	 
	// Convert UTF8 to wide string 
	std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > XML_FUNCTION as_wide(const char* str); 
	std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > XML_FUNCTION as_wide(const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& str); 
#endif 
 
	// Memory allocation function interface; returns pointer to allocated memory or NULL on failure 
	typedef void* (*allocation_function)(size_t size); 
	 
	// Memory deallocation function interface 
	typedef void (*deallocation_function)(void* ptr); 
 
	// Override default memory management functions. All subsequent allocations/deallocations will be performed via supplied functions. 
	void XML_FUNCTION set_memory_management_functions(allocation_function allocate, deallocation_function deallocate); 
	 
	// Get current memory management functions 
	allocation_function XML_FUNCTION get_memory_allocation_function(); 
	deallocation_function XML_FUNCTION get_memory_deallocation_function(); 
} 
} // \phys
 
#if !defined(XML_NO_STL) && (defined(_MSC_VER) || defined(__ICC)) 
namespace std 
{ 
	// Workarounds for (non-standard) iterator category detection for older versions (MSVC7/IC8 and earlier) 
	std::bidirectional_iterator_tag XML_FUNCTION _Iter_cat(const pugi::Node_Iterator&); 
	std::bidirectional_iterator_tag XML_FUNCTION _Iter_cat(const pugi::Attribute_Iterator&); 
} 
#endif 
 
#if !defined(XML_NO_STL) && defined(__SUNPRO_CC) 
namespace std 
{ 
	// Workarounds for (non-standard) iterator category detection 
	std::bidirectional_iterator_tag XML_FUNCTION __iterator_category(const pugi::Node_Iterator&); 
	std::bidirectional_iterator_tag XML_FUNCTION __iterator_category(const pugi::Attribute_Iterator&); 
} 
#endif 
 
#endif 
 
/* 
 * 
 * Software, Files, Libraries and all other items referenced in this clause refers only 
 * to the contents of this file and associated documentation. 
 * 
 * Copyright © 2006-2010 Arseny Kapoulkine 
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
