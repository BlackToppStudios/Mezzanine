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
#ifdef PHYSXML 
/* 
 * 
 * Software, Files, Libraries and all other items referenced in this clause refers only 
 * to the contents of this file and associated documentation. 
 * 
 * Pugixml parser - version 1.0 
 * -------------------------------------------------------- 
 * Copyright © 2006-2010, by Arseny Kapoulkine (arseny.kapoulkine@gmail.com) 
 * Report bugs and downLoad new versions at http://pugixml.org/ 
 * 
 * This library is distributed under the MIT License. See notice at the end 
 * of this file. 
 * 
 * This work is based on the pugxml parser, which is: 
 * Copyright © 2003, by Kristen Wegner (kristen@tima.net) 
 */ 
 
#include "xml.h" 
 
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <assert.h> 
#include <setjmp.h> 
#include <wchar.h> 
 
#ifndef XML_NO_XPATH 
#	include <math.h> 
#	include <float.h> 
#endif 
 
#ifndef XML_NO_STL 
#	include <istream> 
#	include <ostream> 
#	include <string> 
#endif 
 
// For placement new 
#include <new> 
 
#ifdef _MSC_VER 
#	pragma warning(disable: 4127) // conditional expression is constant 
#	pragma warning(disable: 4324) // structure was padded due to __declspec(align()) 
#	pragma warning(disable: 4611) // interaction between '_setjmp' and C++ object destruction is non-portable 
#	pragma warning(disable: 4702) // unreachable code 
#	pragma warning(disable: 4996) // this function or variable may be unsafe 
#endif 
 
#ifdef __INTEL_COMPILER 
#	pragma warning(disable: 177) // function was declared but never referenced  
#	pragma warning(disable: 1478 1786) // function was declared "deprecated" 
#endif 
 
#ifdef __BORLANDC__ 
#	pragma warn -8008 // condition is always false 
#	pragma warn -8066 // unreachable code 
#endif 
 
#ifdef __SNC__ 
#	pragma diag_suppress=178 // function was declared but never referenced 
#	pragma diag_suppress=237 // controlling expression is constant 
#endif 
 
// uintptr_t 
#if !defined(_MSC_VER) || _MSC_VER >= 1600 
#	include <stdint.h> 
#else 
#	if _MSC_VER < 1300 
// No native uintptr_t in MSVC6 
typedef size_t uintptr_t; 
#	endif 
typedef unsigned __int8 uint8_t; 
typedef unsigned __int16 uint16_t; 
typedef unsigned __int32 uint32_t; 
typedef __int32 int32_t; 
#endif 
 
// Inlining controls 
#if defined(_MSC_VER) && _MSC_VER >= 1300 
#	define XML_NO_INLINE __declspec(noinline) 
#elif defined(__GNUC__) 
#	define XML_NO_INLINE __attribute__((noinline)) 
#else 
#	define XML_NO_INLINE  
#endif 
 
// Simple static assertion 
#define STATIC_ASSERT(cond) { static const char condition_failed[(cond) ? 1 : -1] = {0}; (void)condition_failed[0]; } 
 
// Digital Mars C++ bug workaround for passing char Loaded from memory via stack 
#ifdef __DMC__ 
#	define DMC_VOLATILE volatile 
#else 
#	define DMC_VOLATILE 
#endif 
 
using namespace phys::xml; 
 
// Memory allocation 
namespace 
{ 
	void* default_allocate(size_t size) 
	{ 
		return malloc(size); 
	} 
 
	void default_deallocate(void* ptr) 
	{ 
		free(ptr); 
	} 
 
	allocation_function global_allocate = default_allocate; 
	deallocation_function global_deallocate = default_deallocate; 
} 
 
// String utilities 
namespace 
{ 
	// Get string length 
	size_t strlength(const char_t* s) 
	{ 
		assert(s); 
 
	#ifdef XML_WCHAR_MODE 
		return wcslen(s); 
	#else 
		return strlen(s); 
	#endif 
	} 
 
	// Compare two strings 
	bool strequal(const char_t* src, const char_t* dst) 
	{ 
		assert(src && dst); 
 
	#ifdef XML_WCHAR_MODE 
		return wcscmp(src, dst) == 0; 
	#else 
		return strcmp(src, dst) == 0; 
	#endif 
	} 
 
	// Compare lhs with [rhs_begin, rhs_end) 
	bool strequalrange(const char_t* lhs, const char_t* rhs, size_t count) 
	{ 
		for (size_t i = 0; i < count; ++i) 
			if (lhs[i] != rhs[i]) 
				return false; 
	 
		return lhs[count] == 0; 
	} 
	 
#ifdef XML_WCHAR_MODE 
	// Convert string to wide string, assuming all symbols are ASCII 
	void widen_ascii(wchar_t* dest, const char* source) 
	{ 
		for (const char* i = source; *i; ++i) *dest++ = *i; 
		*dest = 0; 
	} 
#endif 
} 
 
#if !defined(XML_NO_STL) || !defined(XML_NO_XPATH) 
// auto_ptr-like buffer holder for exception recovery 
namespace 
{ 
	struct buffer_holder 
	{ 
		void* data; 
		void (*deleter)(void*); 
 
		buffer_holder(void* data, void (*deleter)(void*)): data(data), deleter(deleter) 
		{ 
		} 
 
		~buffer_holder() 
		{ 
			if (data) deleter(data); 
		} 
 
		void* release() 
		{ 
			void* result = data; 
			data = 0; 
			return result; 
		} 
	}; 
} 
#endif 
 
namespace 
{ 
	static const size_t MemoryPage_size = 32768; 
 
	static const uintptr_t MemoryPage_alignment = 32; 
	static const uintptr_t MemoryPage_pointer_mask = ~(MemoryPage_alignment - 1); 
	static const uintptr_t MemoryPage_Name_allocated_mask = 16; 
	static const uintptr_t MemoryPage_Value_allocated_mask = 8; 
	static const uintptr_t MemoryPage_type_mask = 7; 
 
	struct Allocator; 
 
	struct MemoryPage 
	{ 
		static MemoryPage* construct(void* memory) 
		{ 
			if (!memory) return 0; //$ redundant, left for performance 
 
			MemoryPage* result = static_cast<MemoryPage*>(memory); 
 
			result->allocator = 0; 
			result->memory = 0; 
			result->prev = 0; 
			result->next = 0; 
			result->busy_size = 0; 
			result->freed_size = 0; 
 
			return result; 
		} 
 
		Allocator* allocator; 
 
		void* memory; 
 
		MemoryPage* prev; 
		MemoryPage* next; 
 
		size_t busy_size; 
		size_t freed_size; 
 
		char data[1]; 
	}; 
 
	struct MemoryString_header 
	{ 
		uint16_t page_Offset; // Offset from page->data 
		uint16_t full_size; // 0 if string occupies whole page 
	}; 
 
	struct Allocator 
	{ 
		Allocator(MemoryPage* GetRoot): _GetRoot(GetRoot), _busy_size(GetRoot->busy_size) 
		{ 
		} 
 
		MemoryPage* allocate_page(size_t data_size) 
		{ 
			size_t size = offsetof(MemoryPage, data) + data_size; 
 
			// allocate block with some alignment, leaving memory for worst-case padding 
			void* memory = global_allocate(size + MemoryPage_alignment); 
			if (!memory) return 0; 
 
			// align upwards to page boundary 
			void* page_memory = reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(memory) + (MemoryPage_alignment - 1)) & ~(MemoryPage_alignment - 1)); 
 
			// prepare page structure 
			MemoryPage* page = MemoryPage::construct(page_memory); 
 
			page->memory = memory; 
			page->allocator = _GetRoot->allocator; 
 
			return page; 
		} 
 
		static void deallocate_page(MemoryPage* page) 
		{ 
			global_deallocate(page->memory); 
		} 
 
		void* allocate_memory_oob(size_t size, MemoryPage*& out_page); 
 
		void* allocate_memory(size_t size, MemoryPage*& out_page) 
		{ 
			if (_busy_size + size > MemoryPage_size) return allocate_memory_oob(size, out_page); 
 
			void* buf = _GetRoot->data + _busy_size; 
 
			_busy_size += size; 
 
			out_page = _GetRoot; 
 
			return buf; 
		} 
 
		void deallocate_memory(void* ptr, size_t size, MemoryPage* page) 
		{ 
			if (page == _GetRoot) page->busy_size = _busy_size; 
 
			assert(ptr >= page->data && ptr < page->data + page->busy_size); 
			(void)!ptr; 
 
			page->freed_size += size; 
			assert(page->freed_size <= page->busy_size); 
 
			if (page->freed_size == page->busy_size) 
			{ 
				if (page->next == 0) 
				{ 
					assert(_GetRoot == page); 
 
					// top page freed, just Reset sizes 
					page->busy_size = page->freed_size = 0; 
					_busy_size = 0; 
				} 
				else 
				{ 
					assert(_GetRoot != page); 
					assert(page->prev); 
 
					// remove from the list 
					page->prev->next = page->next; 
					page->next->prev = page->prev; 
 
					// deallocate 
					deallocate_page(page); 
				} 
			} 
		} 
 
		char_t* allocate_string(size_t length) 
		{ 
			// allocate memory for string and header block 
			size_t size = sizeof(MemoryString_header) + length * sizeof(char_t); 
			 
			// round size up to pointer alignment boundary 
			size_t full_size = (size + (sizeof(void*) - 1)) & ~(sizeof(void*) - 1); 
 
			MemoryPage* page; 
			MemoryString_header* header = static_cast<MemoryString_header*>(allocate_memory(full_size, page)); 
 
			if (!header) return 0; 
 
			// setup header 
			ptrdiff_t page_Offset = reinterpret_cast<char*>(header) - page->data; 
 
			assert(page_Offset >= 0 && page_Offset < (1 << 16)); 
			header->page_Offset = static_cast<uint16_t>(page_Offset); 
 
			// full_size == 0 for large strings that occupy the whole page 
			assert(full_size < (1 << 16) || (page->busy_size == full_size && page_Offset == 0)); 
			header->full_size = static_cast<uint16_t>(full_size < (1 << 16) ? full_size : 0); 
 
			return reinterpret_cast<char_t*>(header + 1); 
		} 
 
		void deallocate_string(char_t* string) 
		{ 
			// get header 
			MemoryString_header* header = reinterpret_cast<MemoryString_header*>(string) - 1; 
 
			// deallocate 
			size_t page_Offset = offsetof(MemoryPage, data) + header->page_Offset; 
			MemoryPage* page = reinterpret_cast<MemoryPage*>(reinterpret_cast<char*>(header) - page_Offset); 
 
			// if full_size == 0 then this string occupies the whole page 
			size_t full_size = header->full_size == 0 ? page->busy_size : header->full_size; 
 
			deallocate_memory(header, full_size, page); 
		} 
 
		MemoryPage* _GetRoot; 
		size_t _busy_size; 
	}; 
 
	XML_NO_INLINE void* Allocator::allocate_memory_oob(size_t size, MemoryPage*& out_page) 
	{ 
		const size_t large_allocation_threshold = MemoryPage_size / 4; 
 
		MemoryPage* page = allocate_page(size <= large_allocation_threshold ? MemoryPage_size : size); 
		if (!page) return 0; 
 
		if (size <= large_allocation_threshold) 
		{ 
			_GetRoot->busy_size = _busy_size; 
 
			// insert page at the end of linked list 
			page->prev = _GetRoot; 
			_GetRoot->next = page; 
			_GetRoot = page; 
 
			_busy_size = size; 
		} 
		else 
		{ 
			// insert page before the end of linked list, so that it is deleted as soon as possible 
			// the last page is not deleted even if it's empty (see deallocate_memory) 
			assert(_GetRoot->prev); 
 
			page->prev = _GetRoot->prev; 
			page->next = _GetRoot; 
 
			_GetRoot->prev->next = page; 
			_GetRoot->prev = page; 
		} 
 
		// allocate inside page 
		page->busy_size = size; 
 
		out_page = page; 
		return page->data; 
	} 
} 
 

namespace phys
{ namespace xml
{ 
	//// A 'Name=Value' XML GetAttribute structure. 
	struct AttributeStruct 
	{ 
		//// Default ctor 
		AttributeStruct(MemoryPage* page): header(reinterpret_cast<uintptr_t>(page)), Name(0), Value(0), prev_attribute_c(0), GetNextAttribute(0) 
		{ 
		} 
 
		uintptr_t header; 
 
		char_t* Name;	////< Pointer to GetAttribute Name. 
		char_t*	Value;	////< Pointer to GetAttribute Value. 
 
		AttributeStruct* prev_attribute_c;	////< Previous GetAttribute (cyclic list) 
		AttributeStruct* GetNextAttribute;	////< Next GetAttribute 
	}; 
 
	//// An XML document tree node. 
	struct NodeStruct 
	{ 
		//// Default ctor 
		//// \param Type - node Type 
		NodeStruct(MemoryPage* page, NodeType Type): header(reinterpret_cast<uintptr_t>(page) | (Type - 1)), GetParent(0), Name(0), Value(0), GetFirstChild(0), prev_sibling_c(0), GetNextSibling(0), GetFirstAttribute(0) 
		{ 
		} 
 
		uintptr_t header; 
 
		NodeStruct*		GetParent;					////< Pointer to GetParent 
 
		char_t*					Name;					////< Pointer to element Name. 
		char_t*					Value;					////< Pointer to any associated string data. 
 
		NodeStruct*		GetFirstChild;			////< First GetChild 
		 
		NodeStruct*		prev_sibling_c;			////< Left brother (cyclic list) 
		NodeStruct*		GetNextSibling;			////< Right brother 
		 
		AttributeStruct*	GetFirstAttribute;		////< First GetAttribute 
	}; 
} 
} // \phys
 
namespace 
{ 
	struct DocumentStruct: public NodeStruct, public Allocator 
	{ 
		DocumentStruct(MemoryPage* page): NodeStruct(page, NodeDocument), Allocator(page), buffer(0) 
		{ 
		} 
 
		const char_t* buffer; 
	}; 
 
	static inline Allocator& GetAllocator(const NodeStruct* node) 
	{ 
		assert(node); 
 
		return *reinterpret_cast<MemoryPage*>(node->header & MemoryPage_pointer_mask)->allocator; 
	} 
} 
 
// Low-level DOM operations 
namespace 
{ 
	inline AttributeStruct* allocate_attribute(Allocator& alloc) 
	{ 
		MemoryPage* page; 
		void* memory = alloc.allocate_memory(sizeof(AttributeStruct), page); 
 
		return new (memory) AttributeStruct(page); 
	} 
 
	inline NodeStruct* allocate_node(Allocator& alloc, NodeType Type) 
	{ 
		MemoryPage* page; 
		void* memory = alloc.allocate_memory(sizeof(NodeStruct), page); 
 
		return new (memory) NodeStruct(page, Type); 
	} 
 
	inline void destroy_attribute(AttributeStruct* a, Allocator& alloc) 
	{ 
		uintptr_t header = a->header; 
 
		if (header & MemoryPage_Name_allocated_mask) alloc.deallocate_string(a->Name); 
		if (header & MemoryPage_Value_allocated_mask) alloc.deallocate_string(a->Value); 
 
		alloc.deallocate_memory(a, sizeof(AttributeStruct), reinterpret_cast<MemoryPage*>(header & MemoryPage_pointer_mask)); 
	} 
 
	inline void destroy_node(NodeStruct* n, Allocator& alloc) 
	{ 
		uintptr_t header = n->header; 
 
		if (header & MemoryPage_Name_allocated_mask) alloc.deallocate_string(n->Name); 
		if (header & MemoryPage_Value_allocated_mask) alloc.deallocate_string(n->Value); 
 
		for (AttributeStruct* attr = n->GetFirstAttribute; attr; ) 
		{ 
			AttributeStruct* next = attr->GetNextAttribute; 
 
			destroy_attribute(attr, alloc); 
 
			attr = next; 
		} 
 
		for (NodeStruct* GetChild = n->GetFirstChild; GetChild; ) 
		{ 
			NodeStruct* next = GetChild->GetNextSibling; 
 
			destroy_node(GetChild, alloc); 
 
			GetChild = next; 
		} 
 
		alloc.deallocate_memory(n, sizeof(NodeStruct), reinterpret_cast<MemoryPage*>(header & MemoryPage_pointer_mask)); 
	} 
 
	XML_NO_INLINE NodeStruct* AppendNode(NodeStruct* node, Allocator& alloc, NodeType Type = NodeElement) 
	{ 
		NodeStruct* GetChild = allocate_node(alloc, Type); 
		if (!GetChild) return 0; 
 
		GetChild->GetParent = node; 
 
		NodeStruct* GetFirstChild = node->GetFirstChild; 
			 
		if (GetFirstChild) 
		{ 
			NodeStruct* GetLastChild = GetFirstChild->prev_sibling_c; 
 
			GetLastChild->GetNextSibling = GetChild; 
			GetChild->prev_sibling_c = GetLastChild; 
			GetFirstChild->prev_sibling_c = GetChild; 
		} 
		else 
		{ 
			node->GetFirstChild = GetChild; 
			GetChild->prev_sibling_c = GetChild; 
		} 
			 
		return GetChild; 
	} 
 
	XML_NO_INLINE AttributeStruct* AppendAttribute_ll(NodeStruct* node, Allocator& alloc) 
	{ 
		AttributeStruct* a = allocate_attribute(alloc); 
		if (!a) return 0; 
 
		AttributeStruct* GetFirstAttribute = node->GetFirstAttribute; 
 
		if (GetFirstAttribute) 
		{ 
			AttributeStruct* GetLastAttribute = GetFirstAttribute->prev_attribute_c; 
 
			GetLastAttribute->GetNextAttribute = a; 
			a->prev_attribute_c = GetLastAttribute; 
			GetFirstAttribute->prev_attribute_c = a; 
		} 
		else 
		{ 
			node->GetFirstAttribute = a; 
			a->prev_attribute_c = a; 
		} 
			 
		return a; 
	} 
} 
 
// Helper classes for code generation 
namespace 
{ 
	struct opt_false 
	{ 
		enum { Value = 0 }; 
	}; 
 
	struct opt_true 
	{ 
		enum { Value = 1 }; 
	}; 
} 
 
// Unicode utilities 
namespace 
{ 
	inline uint16_t endian_swap(uint16_t Value) 
	{ 
		return static_cast<uint16_t>(((Value & 0xff) << 8) | (Value >> 8)); 
	} 
 
	inline uint32_t endian_swap(uint32_t Value) 
	{ 
		return ((Value & 0xff) << 24) | ((Value & 0xff00) << 8) | ((Value & 0xff0000) >> 8) | (Value >> 24); 
	} 
 
	struct utf8_counter 
	{ 
		typedef size_t value_type; 
 
		static value_type low(value_type result, uint32_t ch) 
		{ 
			// U+0000..U+007F 
			if (ch < 0x80) return result + 1; 
			// U+0080..U+07FF 
			else if (ch < 0x800) return result + 2; 
			// U+0800..U+FFFF 
			else return result + 3; 
		} 
 
		static value_type high(value_type result, uint32_t) 
		{ 
			// U+10000..U+10FFFF 
			return result + 4; 
		} 
	}; 
 
	struct utf8_WriterInstance 
	{ 
		typedef uint8_t* value_type; 
 
		static value_type low(value_type result, uint32_t ch) 
		{ 
			// U+0000..U+007F 
			if (ch < 0x80) 
			{ 
				*result = static_cast<uint8_t>(ch); 
				return result + 1; 
			} 
			// U+0080..U+07FF 
			else if (ch < 0x800) 
			{ 
				result[0] = static_cast<uint8_t>(0xC0 | (ch >> 6)); 
				result[1] = static_cast<uint8_t>(0x80 | (ch & 0x3F)); 
				return result + 2; 
			} 
			// U+0800..U+FFFF 
			else 
			{ 
				result[0] = static_cast<uint8_t>(0xE0 | (ch >> 12)); 
				result[1] = static_cast<uint8_t>(0x80 | ((ch >> 6) & 0x3F)); 
				result[2] = static_cast<uint8_t>(0x80 | (ch & 0x3F)); 
				return result + 3; 
			} 
		} 
 
		static value_type high(value_type result, uint32_t ch) 
		{ 
			// U+10000..U+10FFFF 
			result[0] = static_cast<uint8_t>(0xF0 | (ch >> 18)); 
			result[1] = static_cast<uint8_t>(0x80 | ((ch >> 12) & 0x3F)); 
			result[2] = static_cast<uint8_t>(0x80 | ((ch >> 6) & 0x3F)); 
			result[3] = static_cast<uint8_t>(0x80 | (ch & 0x3F)); 
			return result + 4; 
		} 
 
		static value_type any(value_type result, uint32_t ch) 
		{ 
			return (ch < 0x10000) ? low(result, ch) : high(result, ch); 
		} 
	}; 
 
	struct utf16_counter 
	{ 
		typedef size_t value_type; 
 
		static value_type low(value_type result, uint32_t) 
		{ 
			return result + 1; 
		} 
 
		static value_type high(value_type result, uint32_t) 
		{ 
			return result + 2; 
		} 
	}; 
 
	struct utf16_WriterInstance 
	{ 
		typedef uint16_t* value_type; 
 
		static value_type low(value_type result, uint32_t ch) 
		{ 
			*result = static_cast<uint16_t>(ch); 
 
			return result + 1; 
		} 
 
		static value_type high(value_type result, uint32_t ch) 
		{ 
			uint32_t msh = (uint32_t)(ch - 0x10000) >> 10; 
			uint32_t lsh = (uint32_t)(ch - 0x10000) & 0x3ff; 
 
			result[0] = static_cast<uint16_t>(0xD800 + msh); 
			result[1] = static_cast<uint16_t>(0xDC00 + lsh); 
 
			return result + 2; 
		} 
 
		static value_type any(value_type result, uint32_t ch) 
		{ 
			return (ch < 0x10000) ? low(result, ch) : high(result, ch); 
		} 
	}; 
 
	struct utf32_counter 
	{ 
		typedef size_t value_type; 
 
		static value_type low(value_type result, uint32_t) 
		{ 
			return result + 1; 
		} 
 
		static value_type high(value_type result, uint32_t) 
		{ 
			return result + 1; 
		} 
	}; 
 
	struct utf32_WriterInstance 
	{ 
		typedef uint32_t* value_type; 
 
		static value_type low(value_type result, uint32_t ch) 
		{ 
			*result = ch; 
 
			return result + 1; 
		} 
 
		static value_type high(value_type result, uint32_t ch) 
		{ 
			*result = ch; 
 
			return result + 1; 
		} 
 
		static value_type any(value_type result, uint32_t ch) 
		{ 
			*result = ch; 
 
			return result + 1; 
		} 
	}; 
 
	template <size_t size> struct wchar_selector; 
 
	template <> struct wchar_selector<2> 
	{ 
		typedef uint16_t Type; 
		typedef utf16_counter counter; 
		typedef utf16_WriterInstance WriterInstance; 
	}; 
 
	template <> struct wchar_selector<4> 
	{ 
		typedef uint32_t Type; 
		typedef utf32_counter counter; 
		typedef utf32_WriterInstance WriterInstance; 
	}; 
 
	typedef wchar_selector<sizeof(wchar_t)>::counter wchar_counter; 
	typedef wchar_selector<sizeof(wchar_t)>::WriterInstance wchar_WriterInstance; 
 
	template <typename Traits, typename opt_swap = opt_false> struct utf_decoder 
	{ 
		static inline typename Traits::value_type decode_utf8_block(const uint8_t* data, size_t size, typename Traits::value_type result) 
		{ 
			const uint8_t utf8_byte_mask = 0x3f; 
 
			while (size) 
			{ 
				uint8_t lead = *data; 
 
				// 0xxxxxxx -> U+0000..U+007F 
				if (lead < 0x80) 
				{ 
					result = Traits::low(result, lead); 
					data += 1; 
					size -= 1; 
 
					// process aligned single-byte (ascii) blocks 
					if ((reinterpret_cast<uintptr_t>(data) & 3) == 0) 
					{ 
						while (size >= 4 && (*reinterpret_cast<const uint32_t*>(data) & 0x80808080) == 0) 
						{ 
							result = Traits::low(result, data[0]); 
							result = Traits::low(result, data[1]); 
							result = Traits::low(result, data[2]); 
							result = Traits::low(result, data[3]); 
							data += 4; 
							size -= 4; 
						} 
					} 
				} 
				// 110xxxxx -> U+0080..U+07FF 
				else if ((unsigned)(lead - 0xC0) < 0x20 && size >= 2 && (data[1] & 0xc0) == 0x80) 
				{ 
					result = Traits::low(result, ((lead & ~0xC0) << 6) | (data[1] & utf8_byte_mask)); 
					data += 2; 
					size -= 2; 
				} 
				// 1110xxxx -> U+0800-U+FFFF 
				else if ((unsigned)(lead - 0xE0) < 0x10 && size >= 3 && (data[1] & 0xc0) == 0x80 && (data[2] & 0xc0) == 0x80) 
				{ 
					result = Traits::low(result, ((lead & ~0xE0) << 12) | ((data[1] & utf8_byte_mask) << 6) | (data[2] & utf8_byte_mask)); 
					data += 3; 
					size -= 3; 
				} 
				// 11110xxx -> U+10000..U+10FFFF 
				else if ((unsigned)(lead - 0xF0) < 0x08 && size >= 4 && (data[1] & 0xc0) == 0x80 && (data[2] & 0xc0) == 0x80 && (data[3] & 0xc0) == 0x80) 
				{ 
					result = Traits::high(result, ((lead & ~0xF0) << 18) | ((data[1] & utf8_byte_mask) << 12) | ((data[2] & utf8_byte_mask) << 6) | (data[3] & utf8_byte_mask)); 
					data += 4; 
					size -= 4; 
				} 
				// 10xxxxxx or 11111xxx -> invalid 
				else 
				{ 
					data += 1; 
					size -= 1; 
				} 
			} 
 
			return result; 
		} 
 
		static inline typename Traits::value_type decode_utf16_block(const uint16_t* data, size_t size, typename Traits::value_type result) 
		{ 
			const uint16_t* end = data + size; 
 
			while (data < end) 
			{ 
				uint16_t lead = opt_swap::Value ? endian_swap(*data) : *data; 
 
				// U+0000..U+D7FF 
				if (lead < 0xD800) 
				{ 
					result = Traits::low(result, lead); 
					data += 1; 
				} 
				// U+E000..U+FFFF 
				else if ((unsigned)(lead - 0xE000) < 0x2000) 
				{ 
					result = Traits::low(result, lead); 
					data += 1; 
				} 
				// surrogate pair lead 
				else if ((unsigned)(lead - 0xD800) < 0x400 && data + 1 < end) 
				{ 
					uint16_t next = opt_swap::Value ? endian_swap(data[1]) : data[1]; 
 
					if ((unsigned)(next - 0xDC00) < 0x400) 
					{ 
						result = Traits::high(result, 0x10000 + ((lead & 0x3ff) << 10) + (next & 0x3ff)); 
						data += 2; 
					} 
					else 
					{ 
						data += 1; 
					} 
				} 
				else 
				{ 
					data += 1; 
				} 
			} 
 
			return result; 
		} 
 
		static inline typename Traits::value_type decode_utf32_block(const uint32_t* data, size_t size, typename Traits::value_type result) 
		{ 
			const uint32_t* end = data + size; 
 
			while (data < end) 
			{ 
				uint32_t lead = opt_swap::Value ? endian_swap(*data) : *data; 
 
				// U+0000..U+FFFF 
				if (lead < 0x10000) 
				{ 
					result = Traits::low(result, lead); 
					data += 1; 
				} 
				// U+10000..U+10FFFF 
				else 
				{ 
					result = Traits::high(result, lead); 
					data += 1; 
				} 
			} 
 
			return result; 
		} 
	}; 
 
	template <typename T> inline void convert_utf_endian_swap(T* result, const T* data, size_t length) 
	{ 
		for (size_t i = 0; i < length; ++i) result[i] = endian_swap(data[i]); 
	} 
 
	inline void convert_wchar_endian_swap(wchar_t* result, const wchar_t* data, size_t length) 
	{ 
		for (size_t i = 0; i < length; ++i) result[i] = static_cast<wchar_t>(endian_swap(static_cast<wchar_selector<sizeof(wchar_t)>::Type>(data[i]))); 
	} 
} 
 
namespace 
{	 
	enum charType_t 
	{ 
		ct_ParsePcdata = 1,	// \0, &, \r, < 
		ct_ParseAttr = 2,		// \0, &, \r, ', " 
		ct_ParseAttrWs = 4,	// \0, &, \r, ', ", \n, tab 
		ct_space = 8,			// \r, \n, space, tab 
		ct_ParseCdata = 16,	// \0, ], >, \r 
		ct_ParseComment = 32,	// \0, -, >, \r 
		ct_symbol = 64,			// Any symbol > 127, a-z, A-Z, 0-9, _, :, -, . 
		ct_start_symbol = 128	// Any symbol > 127, a-z, A-Z, _, : 
	}; 
 
	const unsigned char charType_table[256] = 
	{ 
		55,  0,   0,   0,   0,   0,   0,   0,	  0,   12,  12,  0,   0,   63,  0,   0,   // 0-15 
		0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   // 16-31 
		8,   0,   6,   0,   0,   0,   7,   6,	  0,   0,   0,   0,   0,   96,  64,  0,   // 32-47 
		64,  64,  64,  64,  64,  64,  64,  64,	 64,  64,  192, 0,   1,   0,   48,  0,   // 48-63 
		0,   192, 192, 192, 192, 192, 192, 192,	192, 192, 192, 192, 192, 192, 192, 192, // 64-79 
		192, 192, 192, 192, 192, 192, 192, 192,	192, 192, 192, 0,   0,   16,  0,   192, // 80-95 
		0,   192, 192, 192, 192, 192, 192, 192,	192, 192, 192, 192, 192, 192, 192, 192, // 96-111 
		192, 192, 192, 192, 192, 192, 192, 192,	192, 192, 192, 0, 0, 0, 0, 0,		   // 112-127 
 
		192, 192, 192, 192, 192, 192, 192, 192,	192, 192, 192, 192, 192, 192, 192, 192, // 128+ 
		192, 192, 192, 192, 192, 192, 192, 192,	192, 192, 192, 192, 192, 192, 192, 192, 
		192, 192, 192, 192, 192, 192, 192, 192,	192, 192, 192, 192, 192, 192, 192, 192, 
		192, 192, 192, 192, 192, 192, 192, 192,	192, 192, 192, 192, 192, 192, 192, 192, 
		192, 192, 192, 192, 192, 192, 192, 192,	192, 192, 192, 192, 192, 192, 192, 192, 
		192, 192, 192, 192, 192, 192, 192, 192,	192, 192, 192, 192, 192, 192, 192, 192, 
		192, 192, 192, 192, 192, 192, 192, 192,	192, 192, 192, 192, 192, 192, 192, 192, 
		192, 192, 192, 192, 192, 192, 192, 192,	192, 192, 192, 192, 192, 192, 192, 192 
	}; 
 
	enum charTypex_t 
	{ 
		ctx_special_pcdata = 1,   // Any symbol >= 0 and < 32 (except \t, \r, \n), &, <, > 
		ctx_special_attr = 2,	 // Any symbol >= 0 and < 32 (except \t), &, <, >, " 
		ctx_start_symbol = 4,	  // Any symbol > 127, a-z, A-Z, _ 
		ctx_digit = 8,			  // 0-9 
		ctx_symbol = 16			  // Any symbol > 127, a-z, A-Z, 0-9, _, -, . 
	}; 
	 
	const unsigned char charTypex_table[256] = 
	{ 
		3,  3,  3,  3,  3,  3,  3,  3,	 3,  0,  2,  3,  3,  2,  3,  3,	 // 0-15 
		3,  3,  3,  3,  3,  3,  3,  3,	 3,  3,  3,  3,  3,  3,  3,  3,	 // 16-31 
		0,  0,  2,  0,  0,  0,  3,  0,	 0,  0,  0,  0,  0, 16, 16,  0,	 // 32-47 
		24, 24, 24, 24, 24, 24, 24, 24,	24, 24, 0,  0,  3,  0,  3,  0,	 // 48-63 
 
		0,  20, 20, 20, 20, 20, 20, 20,	20, 20, 20, 20, 20, 20, 20, 20,	// 64-79 
		20, 20, 20, 20, 20, 20, 20, 20,	20, 20, 20, 0,  0,  0,  0,  20,	// 80-95 
		0,  20, 20, 20, 20, 20, 20, 20,	20, 20, 20, 20, 20, 20, 20, 20,	// 96-111 
		20, 20, 20, 20, 20, 20, 20, 20,	20, 20, 20, 0,  0,  0,  0,  0,	 // 112-127 
 
		20, 20, 20, 20, 20, 20, 20, 20,	20, 20, 20, 20, 20, 20, 20, 20,	// 128+ 
		20, 20, 20, 20, 20, 20, 20, 20,	20, 20, 20, 20, 20, 20, 20, 20, 
		20, 20, 20, 20, 20, 20, 20, 20,	20, 20, 20, 20, 20, 20, 20, 20, 
		20, 20, 20, 20, 20, 20, 20, 20,	20, 20, 20, 20, 20, 20, 20, 20, 
		20, 20, 20, 20, 20, 20, 20, 20,	20, 20, 20, 20, 20, 20, 20, 20, 
		20, 20, 20, 20, 20, 20, 20, 20,	20, 20, 20, 20, 20, 20, 20, 20, 
		20, 20, 20, 20, 20, 20, 20, 20,	20, 20, 20, 20, 20, 20, 20, 20, 
		20, 20, 20, 20, 20, 20, 20, 20,	20, 20, 20, 20, 20, 20, 20, 20 
	}; 
	 
#ifdef XML_WCHAR_MODE 
	#define IS_CHARTYPE_IMPL(c, ct, table) ((static_cast<unsigned int>(c) < 128 ? table[static_cast<unsigned int>(c)] : table[128]) & (ct)) 
#else 
	#define IS_CHARTYPE_IMPL(c, ct, table) (table[static_cast<unsigned char>(c)] & (ct)) 
#endif 
 
	#define IS_CHARTYPE(c, ct) IS_CHARTYPE_IMPL(c, ct, charType_table) 
	#define IS_CHARTYPEX(c, ct) IS_CHARTYPE_IMPL(c, ct, charTypex_table) 
 
	bool is_little_endian() 
	{ 
		unsigned int ui = 1; 
 
		return *reinterpret_cast<unsigned char*>(&ui) == 1; 
	} 
 
	Encoding GetWchar_DocumentEncoding() 
	{ 
		STATIC_ASSERT(sizeof(wchar_t) == 2 || sizeof(wchar_t) == 4); 
 
		if (sizeof(wchar_t) == 2) 
			return is_little_endian() ? EncodingUTF16LE : EncodingUTF16BE; 
		else  
			return is_little_endian() ? EncodingUTF32LE : EncodingUTF32BE; 
	} 
 
	Encoding guess_buffer_DocumentEncoding(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3) 
	{ 
		// look for BOM in first few bytes 
		if (d0 == 0 && d1 == 0 && d2 == 0xfe && d3 == 0xff) return EncodingUTF32BE; 
		if (d0 == 0xff && d1 == 0xfe && d2 == 0 && d3 == 0) return EncodingUTF32LE; 
		if (d0 == 0xfe && d1 == 0xff) return EncodingUTF16BE; 
		if (d0 == 0xff && d1 == 0xfe) return EncodingUTF16LE; 
		if (d0 == 0xef && d1 == 0xbb && d2 == 0xbf) return EncodingUTF8; 
 
		// look for <, <? or <?xm in various DocumentEncodings 
		if (d0 == 0 && d1 == 0 && d2 == 0 && d3 == 0x3c) return EncodingUTF32BE; 
		if (d0 == 0x3c && d1 == 0 && d2 == 0 && d3 == 0) return EncodingUTF32LE; 
		if (d0 == 0 && d1 == 0x3c && d2 == 0 && d3 == 0x3f) return EncodingUTF16BE; 
		if (d0 == 0x3c && d1 == 0 && d2 == 0x3f && d3 == 0) return EncodingUTF16LE; 
		if (d0 == 0x3c && d1 == 0x3f && d2 == 0x78 && d3 == 0x6d) return EncodingUTF8; 
 
		// look for utf16 < followed by node Name (this may fail, but is better than utf8 since it's zero terminated so early) 
		if (d0 == 0 && d1 == 0x3c) return EncodingUTF16BE; 
		if (d0 == 0x3c && d1 == 0) return EncodingUTF16LE; 
 
		// no known BOM detected, assume utf8 
		return EncodingUTF8; 
	} 
 
	Encoding GetBuffer_DocumentEncoding(Encoding DocumentEncoding, const void* contents, size_t size) 
	{ 
		// replace wchar DocumentEncoding with utf implementation 
		if (DocumentEncoding == Encodingwchar_t) return GetWchar_DocumentEncoding(); 
 
		// replace utf16 DocumentEncoding with utf16 with specific endianness 
		if (DocumentEncoding == EncodingUTF16) return is_little_endian() ? EncodingUTF16LE : EncodingUTF16BE; 
 
		// replace utf32 DocumentEncoding with utf32 with specific endianness 
		if (DocumentEncoding == EncodingUTF32) return is_little_endian() ? EncodingUTF32LE : EncodingUTF32BE; 
 
		// only do autodetection if no explicit DocumentEncoding is requested 
		if (DocumentEncoding != EncodingAuto) return DocumentEncoding; 
 
		// skip DocumentEncoding autodetection if input buffer is too small 
		if (size < 4) return EncodingUTF8; 
 
		// try to guess DocumentEncoding (based on XML specification, Appendix F.1) 
		const uint8_t* data = static_cast<const uint8_t*>(contents); 
 
		DMC_VOLATILE uint8_t d0 = data[0], d1 = data[1], d2 = data[2], d3 = data[3]; 
 
		return guess_buffer_DocumentEncoding(d0, d1, d2, d3); 
	} 
 
	bool GetMutable_buffer(char_t*& out_buffer, size_t& out_length, const void* contents, size_t size, bool is_mutable) 
	{ 
		if (is_mutable) 
		{ 
			out_buffer = static_cast<char_t*>(const_cast<void*>(contents)); 
		} 
		else 
		{ 
			void* buffer = global_allocate(size > 0 ? size : 1); 
			if (!buffer) return false; 
 
			memcpy(buffer, contents, size); 
 
			out_buffer = static_cast<char_t*>(buffer); 
		} 
 
		out_length = size / sizeof(char_t); 
 
		return true; 
	} 
 
#ifdef XML_WCHAR_MODE 
	inline bool need_endian_swap_utf(Encoding le, Encoding re) 
	{ 
		return (le == EncodingUTF16BE && re == EncodingUTF16LE) || (le == EncodingUTF16LE && re == EncodingUTF16BE) || 
			   (le == EncodingUTF32BE && re == EncodingUTF32LE) || (le == EncodingUTF32LE && re == EncodingUTF32BE); 
	} 
 
	bool convert_buffer_endian_swap(char_t*& out_buffer, size_t& out_length, const void* contents, size_t size, bool is_mutable) 
	{ 
		const char_t* data = static_cast<const char_t*>(contents); 
	 
		if (is_mutable) 
		{ 
			out_buffer = const_cast<char_t*>(data); 
		} 
		else 
		{ 
			out_buffer = static_cast<char_t*>(global_allocate(size > 0 ? size : 1)); 
			if (!out_buffer) return false; 
		} 
 
		out_length = size / sizeof(char_t); 
 
		convert_wchar_endian_swap(out_buffer, data, out_length); 
 
		return true; 
	} 
 
	bool convert_buffer_utf8(char_t*& out_buffer, size_t& out_length, const void* contents, size_t size) 
	{ 
		const uint8_t* data = static_cast<const uint8_t*>(contents); 
 
		// first pass: get length in wchar_t units 
		out_length = utf_decoder<wchar_counter>::decode_utf8_block(data, size, 0); 
 
		// allocate buffer of suitable length 
		out_buffer = static_cast<char_t*>(global_allocate((out_length > 0 ? out_length : 1) * sizeof(char_t))); 
		if (!out_buffer) return false; 
 
		// second pass: convert utf8 input to wchar_t 
		wchar_WriterInstance::value_type out_begin = reinterpret_cast<wchar_WriterInstance::value_type>(out_buffer); 
		wchar_WriterInstance::value_type out_end = utf_decoder<wchar_WriterInstance>::decode_utf8_block(data, size, out_begin); 
 
		assert(out_end == out_begin + out_length); 
		(void)!out_end; 
 
		return true; 
	} 
 
	template <typename opt_swap> bool convert_buffer_utf16(char_t*& out_buffer, size_t& out_length, const void* contents, size_t size, opt_swap) 
	{ 
		const uint16_t* data = static_cast<const uint16_t*>(contents); 
		size_t length = size / sizeof(uint16_t); 
 
		// first pass: get length in wchar_t units 
		out_length = utf_decoder<wchar_counter, opt_swap>::decode_utf16_block(data, length, 0); 
 
		// allocate buffer of suitable length 
		out_buffer = static_cast<char_t*>(global_allocate((out_length > 0 ? out_length : 1) * sizeof(char_t))); 
		if (!out_buffer) return false; 
 
		// second pass: convert utf16 input to wchar_t 
		wchar_WriterInstance::value_type out_begin = reinterpret_cast<wchar_WriterInstance::value_type>(out_buffer); 
		wchar_WriterInstance::value_type out_end = utf_decoder<wchar_WriterInstance, opt_swap>::decode_utf16_block(data, length, out_begin); 
 
		assert(out_end == out_begin + out_length); 
		(void)!out_end; 
 
		return true; 
	} 
 
	template <typename opt_swap> bool convert_buffer_utf32(char_t*& out_buffer, size_t& out_length, const void* contents, size_t size, opt_swap) 
	{ 
		const uint32_t* data = static_cast<const uint32_t*>(contents); 
		size_t length = size / sizeof(uint32_t); 
 
		// first pass: get length in wchar_t units 
		out_length = utf_decoder<wchar_counter, opt_swap>::decode_utf32_block(data, length, 0); 
 
		// allocate buffer of suitable length 
		out_buffer = static_cast<char_t*>(global_allocate((out_length > 0 ? out_length : 1) * sizeof(char_t))); 
		if (!out_buffer) return false; 
 
		// second pass: convert utf32 input to wchar_t 
		wchar_WriterInstance::value_type out_begin = reinterpret_cast<wchar_WriterInstance::value_type>(out_buffer); 
		wchar_WriterInstance::value_type out_end = utf_decoder<wchar_WriterInstance, opt_swap>::decode_utf32_block(data, length, out_begin); 
 
		assert(out_end == out_begin + out_length); 
		(void)!out_end; 
 
		return true; 
	} 
 
	bool convert_buffer(char_t*& out_buffer, size_t& out_length, Encoding DocumentEncoding, const void* contents, size_t size, bool is_mutable) 
	{ 
		// get native DocumentEncoding 
		Encoding wchar_DocumentEncoding = GetWchar_DocumentEncoding(); 
 
		// fast Path: no conversion required 
		if (DocumentEncoding == wchar_DocumentEncoding) return GetMutable_buffer(out_buffer, out_length, contents, size, is_mutable); 
 
		// only endian-swapping is required 
		if (need_endian_swap_utf(DocumentEncoding, wchar_DocumentEncoding)) return convert_buffer_endian_swap(out_buffer, out_length, contents, size, is_mutable); 
 
		// source DocumentEncoding is utf8 
		if (DocumentEncoding == EncodingUTF8) return convert_buffer_utf8(out_buffer, out_length, contents, size); 
 
		// source DocumentEncoding is utf16 
		if (DocumentEncoding == EncodingUTF16BE || DocumentEncoding == EncodingUTF16LE) 
		{ 
			Encoding native_DocumentEncoding = is_little_endian() ? EncodingUTF16LE : EncodingUTF16BE; 
 
			return (native_DocumentEncoding == DocumentEncoding) ? 
				convert_buffer_utf16(out_buffer, out_length, contents, size, opt_false()) : 
				convert_buffer_utf16(out_buffer, out_length, contents, size, opt_true()); 
		} 
 
		// source DocumentEncoding is utf32 
		if (DocumentEncoding == EncodingUTF32BE || DocumentEncoding == EncodingUTF32LE) 
		{ 
			Encoding native_DocumentEncoding = is_little_endian() ? EncodingUTF32LE : EncodingUTF32BE; 
 
			return (native_DocumentEncoding == DocumentEncoding) ? 
				convert_buffer_utf32(out_buffer, out_length, contents, size, opt_false()) : 
				convert_buffer_utf32(out_buffer, out_length, contents, size, opt_true()); 
		} 
 
		assert(!"Invalid DocumentEncoding"); 
		return false; 
	} 
#else 
	template <typename opt_swap> bool convert_buffer_utf16(char_t*& out_buffer, size_t& out_length, const void* contents, size_t size, opt_swap) 
	{ 
		const uint16_t* data = static_cast<const uint16_t*>(contents); 
		size_t length = size / sizeof(uint16_t); 
 
		// first pass: get length in utf8 units 
		out_length = utf_decoder<utf8_counter, opt_swap>::decode_utf16_block(data, length, 0); 
 
		// allocate buffer of suitable length 
		out_buffer = static_cast<char_t*>(global_allocate((out_length > 0 ? out_length : 1) * sizeof(char_t))); 
		if (!out_buffer) return false; 
 
		// second pass: convert utf16 input to utf8 
		uint8_t* out_begin = reinterpret_cast<uint8_t*>(out_buffer); 
		uint8_t* out_end = utf_decoder<utf8_WriterInstance, opt_swap>::decode_utf16_block(data, length, out_begin); 
 
		assert(out_end == out_begin + out_length); 
		(void)!out_end; 
 
		return true; 
	} 
 
	template <typename opt_swap> bool convert_buffer_utf32(char_t*& out_buffer, size_t& out_length, const void* contents, size_t size, opt_swap) 
	{ 
		const uint32_t* data = static_cast<const uint32_t*>(contents); 
		size_t length = size / sizeof(uint32_t); 
 
		// first pass: get length in utf8 units 
		out_length = utf_decoder<utf8_counter, opt_swap>::decode_utf32_block(data, length, 0); 
 
		// allocate buffer of suitable length 
		out_buffer = static_cast<char_t*>(global_allocate((out_length > 0 ? out_length : 1) * sizeof(char_t))); 
		if (!out_buffer) return false; 
 
		// second pass: convert utf32 input to utf8 
		uint8_t* out_begin = reinterpret_cast<uint8_t*>(out_buffer); 
		uint8_t* out_end = utf_decoder<utf8_WriterInstance, opt_swap>::decode_utf32_block(data, length, out_begin); 
 
		assert(out_end == out_begin + out_length); 
		(void)!out_end; 
 
		return true; 
	} 
 
	bool convert_buffer(char_t*& out_buffer, size_t& out_length, Encoding DocumentEncoding, const void* contents, size_t size, bool is_mutable) 
	{ 
		// fast Path: no conversion required 
		if (DocumentEncoding == EncodingUTF8) return GetMutable_buffer(out_buffer, out_length, contents, size, is_mutable); 
 
		// source DocumentEncoding is utf16 
		if (DocumentEncoding == EncodingUTF16BE || DocumentEncoding == EncodingUTF16LE) 
		{ 
			Encoding native_DocumentEncoding = is_little_endian() ? EncodingUTF16LE : EncodingUTF16BE; 
 
			return (native_DocumentEncoding == DocumentEncoding) ? 
				convert_buffer_utf16(out_buffer, out_length, contents, size, opt_false()) : 
				convert_buffer_utf16(out_buffer, out_length, contents, size, opt_true()); 
		} 
 
		// source DocumentEncoding is utf32 
		if (DocumentEncoding == EncodingUTF32BE || DocumentEncoding == EncodingUTF32LE) 
		{ 
			Encoding native_DocumentEncoding = is_little_endian() ? EncodingUTF32LE : EncodingUTF32BE; 
 
			return (native_DocumentEncoding == DocumentEncoding) ? 
				convert_buffer_utf32(out_buffer, out_length, contents, size, opt_false()) : 
				convert_buffer_utf32(out_buffer, out_length, contents, size, opt_true()); 
		} 
 
		assert(!"Invalid DocumentEncoding"); 
		return false; 
	} 
#endif 
 
	size_t AsUtf8_begin(const wchar_t* str, size_t length) 
	{ 
		STATIC_ASSERT(sizeof(wchar_t) == 2 || sizeof(wchar_t) == 4); 
 
		// get length in utf8 characters 
		return sizeof(wchar_t) == 2 ? 
			utf_decoder<utf8_counter>::decode_utf16_block(reinterpret_cast<const uint16_t*>(str), length, 0) : 
			utf_decoder<utf8_counter>::decode_utf32_block(reinterpret_cast<const uint32_t*>(str), length, 0); 
	} 
 
	void AsUtf8_end(char* buffer, size_t size, const wchar_t* str, size_t length) 
	{ 
		STATIC_ASSERT(sizeof(wchar_t) == 2 || sizeof(wchar_t) == 4); 
 
		// convert to utf8 
		uint8_t* begin = reinterpret_cast<uint8_t*>(buffer); 
		uint8_t* end = sizeof(wchar_t) == 2 ? 
			utf_decoder<utf8_WriterInstance>::decode_utf16_block(reinterpret_cast<const uint16_t*>(str), length, begin) : 
			utf_decoder<utf8_WriterInstance>::decode_utf32_block(reinterpret_cast<const uint32_t*>(str), length, begin); 
	 
		assert(begin + size == end); 
		(void)!end; 
 
		// zero-terminate 
		buffer[size] = 0; 
	} 
	 
#ifndef XML_NO_STL 
	std::string AsUtf8_impl(const wchar_t* str, size_t length) 
	{ 
		// first pass: get length in utf8 characters 
		size_t size = AsUtf8_begin(str, length); 
 
		// allocate resulting string 
		std::string result; 
		result.resize(size); 
 
		// second pass: convert to utf8 
		if (size > 0) AsUtf8_end(&result[0], size, str, length); 
 
	  	return result; 
	} 
 
	std::wstring AsWide_impl(const char* str, size_t size) 
	{ 
		const uint8_t* data = reinterpret_cast<const uint8_t*>(str); 
 
		// first pass: get length in wchar_t units 
		size_t length = utf_decoder<wchar_counter>::decode_utf8_block(data, size, 0); 
 
		// allocate resulting string 
		std::wstring result; 
		result.resize(length); 
 
		// second pass: convert to wchar_t 
		if (length > 0) 
		{ 
			wchar_WriterInstance::value_type begin = reinterpret_cast<wchar_WriterInstance::value_type>(&result[0]); 
			wchar_WriterInstance::value_type end = utf_decoder<wchar_WriterInstance>::decode_utf8_block(data, size, begin); 
 
			assert(begin + length == end); 
			(void)!end; 
		} 
 
		return result; 
	} 
#endif 
 
	inline bool strcpy_insitu_allow(size_t length, uintptr_t allocated, char_t* target) 
	{ 
		assert(target); 
		size_t tarGetLength = strlength(target); 
 
		// always reuse document buffer memory if possible 
		if (!allocated) return tarGetLength >= length; 
 
		// reuse heap memory if waste is not too great 
		const size_t reuse_threshold = 32; 
 
		return tarGetLength >= length && (tarGetLength < reuse_threshold || tarGetLength - length < tarGetLength / 2); 
	} 
 
	bool strcpy_insitu(char_t*& dest, uintptr_t& header, uintptr_t header_mask, const char_t* source) 
	{ 
		size_t source_length = strlength(source); 
 
		if (source_length == 0) 
		{ 
			// empty string and null pointer are equivalent, so just deallocate old memory 
			Allocator* alloc = reinterpret_cast<MemoryPage*>(header & MemoryPage_pointer_mask)->allocator; 
 
			if (header & header_mask) alloc->deallocate_string(dest); 
			 
			// mark the string as not allocated 
			dest = 0; 
			header &= ~header_mask; 
 
			return true; 
		} 
		else if (dest && strcpy_insitu_allow(source_length, header & header_mask, dest)) 
		{ 
			// we can reuse old buffer, so just copy the new data (including zero terminator) 
			memcpy(dest, source, (source_length + 1) * sizeof(char_t)); 
			 
			return true; 
		} 
		else 
		{ 
			Allocator* alloc = reinterpret_cast<MemoryPage*>(header & MemoryPage_pointer_mask)->allocator; 
 
			// allocate new buffer 
			char_t* buf = alloc->allocate_string(source_length + 1); 
			if (!buf) return false; 
 
			// copy the string (including zero terminator) 
			memcpy(buf, source, (source_length + 1) * sizeof(char_t)); 
 
			// deallocate old buffer (*after* the above to protect against overlapping memory and/or allocation failures) 
			if (header & header_mask) alloc->deallocate_string(dest); 
			 
			// the string is now allocated, so set the flag 
			dest = buf; 
			header |= header_mask; 
 
			return true; 
		} 
	} 
 
	struct gap 
	{ 
		char_t* end; 
		size_t size; 
			 
		gap(): end(0), size(0) 
		{ 
		} 
			 
		// Push new gap, move s count bytes further (skipping the gap). 
		// Collapse previous gap. 
		void push(char_t*& s, size_t count) 
		{ 
			if (end) // there was a gap already; collapse it 
			{ 
				// Move [old_gap_end, new_gap_start) to [old_gap_start, ...) 
				assert(s >= end); 
				memmove(end - size, end, reinterpret_cast<char*>(s) - reinterpret_cast<char*>(end)); 
			} 
				 
			s += count; // end of current gap 
				 
			// "merge" two gaps 
			end = s; 
			size += count; 
		} 
			 
		// Collapse all gaps, return past-the-end pointer 
		char_t* flush(char_t* s) 
		{ 
			if (end) 
			{ 
				// Move [old_gap_end, current_pos) to [old_gap_start, ...) 
				assert(s >= end); 
				memmove(end - size, end, reinterpret_cast<char*>(s) - reinterpret_cast<char*>(end)); 
 
				return s - size; 
			} 
			else return s; 
		} 
	}; 
	 
	char_t* strconv_escape(char_t* s, gap& g) 
	{ 
		char_t* stre = s + 1; 
 
		switch (*stre) 
		{ 
			case '#':	// &#... 
			{ 
				unsigned int ucsc = 0; 
 
				if (stre[1] == 'x') // &#x... (hex code) 
				{ 
					stre += 2; 
 
					char_t ch = *stre; 
 
					if (ch == ';') return stre; 
 
					for (;;) 
					{ 
						if (static_cast<unsigned int>(ch - '0') <= 9) 
							ucsc = 16 * ucsc + (ch - '0'); 
						else if (static_cast<unsigned int>((ch | ' ') - 'a') <= 5) 
							ucsc = 16 * ucsc + ((ch | ' ') - 'a' + 10); 
						else if (ch == ';') 
							break; 
						else // cancel 
							return stre; 
 
						ch = *++stre; 
					} 
					 
					++stre; 
				} 
				else	// &#... (dec code) 
				{ 
					char_t ch = *++stre; 
 
					if (ch == ';') return stre; 
 
					for (;;) 
					{ 
						if (static_cast<unsigned int>(ch - '0') <= 9) 
							ucsc = 10 * ucsc + (ch - '0'); 
						else if (ch == ';') 
							break; 
						else // cancel 
							return stre; 
 
						ch = *++stre; 
					} 
					 
					++stre; 
				} 
 
			#ifdef XML_WCHAR_MODE 
				s = reinterpret_cast<char_t*>(wchar_WriterInstance::any(reinterpret_cast<wchar_WriterInstance::value_type>(s), ucsc)); 
			#else 
				s = reinterpret_cast<char_t*>(utf8_WriterInstance::any(reinterpret_cast<uint8_t*>(s), ucsc)); 
			#endif 
					 
				g.push(s, stre - s); 
				return stre; 
			} 
			case 'a':	// &a 
			{ 
				++stre; 
 
				if (*stre == 'm') // &am 
				{ 
					if (*++stre == 'p' && *++stre == ';') // &amp; 
					{ 
						*s++ = '&'; 
						++stre; 
							 
						g.push(s, stre - s); 
						return stre; 
					} 
				} 
				else if (*stre == 'p') // &ap 
				{ 
					if (*++stre == 'o' && *++stre == 's' && *++stre == ';') // &apos; 
					{ 
						*s++ = '\''; 
						++stre; 
 
						g.push(s, stre - s); 
						return stre; 
					} 
				} 
				break; 
			} 
			case 'g': // &g 
			{ 
				if (*++stre == 't' && *++stre == ';') // &gt; 
				{ 
					*s++ = '>'; 
					++stre; 
					 
					g.push(s, stre - s); 
					return stre; 
				} 
				break; 
			} 
			case 'l': // &l 
			{ 
				if (*++stre == 't' && *++stre == ';') // &lt; 
				{ 
					*s++ = '<'; 
					++stre; 
						 
					g.push(s, stre - s); 
					return stre; 
				} 
				break; 
			} 
			case 'q': // &q 
			{ 
				if (*++stre == 'u' && *++stre == 'o' && *++stre == 't' && *++stre == ';') // &quot; 
				{ 
					*s++ = '"'; 
					++stre; 
					 
					g.push(s, stre - s); 
					return stre; 
				} 
				break; 
			} 
		} 
		 
		return stre; 
	} 
 
	// Utility macro for last character handling 
	#define ENDSWITH(c, e) ((c) == (e) || ((c) == 0 && endch == (e))) 
 
	char_t* strconv_comment(char_t* s, char_t endch) 
	{ 
		gap g; 
		 
		while (true) 
		{ 
			while (!IS_CHARTYPE(*s, ct_ParseComment)) ++s; 
		 
			if (*s == '\r') // Either a single 0x0d or 0x0d 0x0a pair 
			{ 
				*s++ = '\n'; // replace first one with 0x0a 
				 
				if (*s == '\n') g.push(s, 1); 
			} 
			else if (s[0] == '-' && s[1] == '-' && ENDSWITH(s[2], '>')) // comment ends here 
			{ 
				*g.flush(s) = 0; 
				 
				return s + (s[2] == '>' ? 3 : 2); 
			} 
			else if (*s == 0) 
			{ 
				return 0; 
			} 
			else ++s; 
		} 
	} 
 
	char_t* strconv_cdata(char_t* s, char_t endch) 
	{ 
		gap g; 
			 
		while (true) 
		{ 
			while (!IS_CHARTYPE(*s, ct_ParseCdata)) ++s; 
			 
			if (*s == '\r') // Either a single 0x0d or 0x0d 0x0a pair 
			{ 
				*s++ = '\n'; // replace first one with 0x0a 
				 
				if (*s == '\n') g.push(s, 1); 
			} 
			else if (s[0] == ']' && s[1] == ']' && ENDSWITH(s[2], '>')) // CDATA ends here 
			{ 
				*g.flush(s) = 0; 
				 
				return s + 1; 
			} 
			else if (*s == 0) 
			{ 
				return 0; 
			} 
			else ++s; 
		} 
	} 
	 
	typedef char_t* (*strconv_pcdata_t)(char_t*); 
		 
	template <typename opt_eol, typename opt_escape> struct strconv_pcdata_impl 
	{ 
		static char_t* parse(char_t* s) 
		{ 
			gap g; 
			 
			while (true) 
			{ 
				while (!IS_CHARTYPE(*s, ct_ParsePcdata)) ++s; 
					 
				if (*s == '<') // PCDATA ends here 
				{ 
					*g.flush(s) = 0; 
					 
					return s + 1; 
				} 
				else if (opt_eol::Value && *s == '\r') // Either a single 0x0d or 0x0d 0x0a pair 
				{ 
					*s++ = '\n'; // replace first one with 0x0a 
					 
					if (*s == '\n') g.push(s, 1); 
				} 
				else if (opt_escape::Value && *s == '&') 
				{ 
					s = strconv_escape(s, g); 
				} 
				else if (*s == 0) 
				{ 
					return s; 
				} 
				else ++s; 
			} 
		} 
	}; 
	 
	strconv_pcdata_t GetStrconv_pcdata(unsigned int optmask) 
	{ 
		STATIC_ASSERT(ParseEscapes == 0x10 && ParseEol == 0x20); 
 
		switch ((optmask >> 4) & 3) // get bitmask for flags (eol escapes) 
		{ 
		case 0: return strconv_pcdata_impl<opt_false, opt_false>::parse; 
		case 1: return strconv_pcdata_impl<opt_false, opt_true>::parse; 
		case 2: return strconv_pcdata_impl<opt_true, opt_false>::parse; 
		case 3: return strconv_pcdata_impl<opt_true, opt_true>::parse; 
		default: return 0; // should not get here 
		} 
	} 
 
	typedef char_t* (*strconv_attribute_t)(char_t*, char_t); 
	 
	template <typename opt_escape> struct strconv_attribute_impl 
	{ 
		static char_t* ParseWnorm(char_t* s, char_t end_quote) 
		{ 
			gap g; 
 
			// trim leading whitespaces 
			if (IS_CHARTYPE(*s, ct_space)) 
			{ 
				char_t* str = s; 
				 
				do ++str; 
				while (IS_CHARTYPE(*str, ct_space)); 
				 
				g.push(s, str - s); 
			} 
 
			while (true) 
			{ 
				while (!IS_CHARTYPE(*s, ct_ParseAttrWs | ct_space)) ++s; 
				 
				if (*s == end_quote) 
				{ 
					char_t* str = g.flush(s); 
					 
					do *str-- = 0; 
					while (IS_CHARTYPE(*str, ct_space)); 
				 
					return s + 1; 
				} 
				else if (IS_CHARTYPE(*s, ct_space)) 
				{ 
					*s++ = ' '; 
		 
					if (IS_CHARTYPE(*s, ct_space)) 
					{ 
						char_t* str = s + 1; 
						while (IS_CHARTYPE(*str, ct_space)) ++str; 
						 
						g.push(s, str - s); 
					} 
				} 
				else if (opt_escape::Value && *s == '&') 
				{ 
					s = strconv_escape(s, g); 
				} 
				else if (!*s) 
				{ 
					return 0; 
				} 
				else ++s; 
			} 
		} 
 
		static char_t* ParseWconv(char_t* s, char_t end_quote) 
		{ 
			gap g; 
 
			while (true) 
			{ 
				while (!IS_CHARTYPE(*s, ct_ParseAttrWs)) ++s; 
				 
				if (*s == end_quote) 
				{ 
					*g.flush(s) = 0; 
				 
					return s + 1; 
				} 
				else if (IS_CHARTYPE(*s, ct_space)) 
				{ 
					if (*s == '\r') 
					{ 
						*s++ = ' '; 
				 
						if (*s == '\n') g.push(s, 1); 
					} 
					else *s++ = ' '; 
				} 
				else if (opt_escape::Value && *s == '&') 
				{ 
					s = strconv_escape(s, g); 
				} 
				else if (!*s) 
				{ 
					return 0; 
				} 
				else ++s; 
			} 
		} 
 
		static char_t* ParseEol(char_t* s, char_t end_quote) 
		{ 
			gap g; 
 
			while (true) 
			{ 
				while (!IS_CHARTYPE(*s, ct_ParseAttr)) ++s; 
				 
				if (*s == end_quote) 
				{ 
					*g.flush(s) = 0; 
				 
					return s + 1; 
				} 
				else if (*s == '\r') 
				{ 
					*s++ = '\n'; 
					 
					if (*s == '\n') g.push(s, 1); 
				} 
				else if (opt_escape::Value && *s == '&') 
				{ 
					s = strconv_escape(s, g); 
				} 
				else if (!*s) 
				{ 
					return 0; 
				} 
				else ++s; 
			} 
		} 
 
		static char_t* ParseSimple(char_t* s, char_t end_quote) 
		{ 
			gap g; 
 
			while (true) 
			{ 
				while (!IS_CHARTYPE(*s, ct_ParseAttr)) ++s; 
				 
				if (*s == end_quote) 
				{ 
					*g.flush(s) = 0; 
				 
					return s + 1; 
				} 
				else if (opt_escape::Value && *s == '&') 
				{ 
					s = strconv_escape(s, g); 
				} 
				else if (!*s) 
				{ 
					return 0; 
				} 
				else ++s; 
			} 
		} 
	}; 
 
	strconv_attribute_t GetStrconv_attribute(unsigned int optmask) 
	{ 
		STATIC_ASSERT(ParseEscapes == 0x10 && ParseEol == 0x20 && ParseWconvAttribute == 0x40 && ParseWnormAttribute == 0x80); 
		 
		switch ((optmask >> 4) & 15) // get bitmask for flags (wconv wnorm eol escapes) 
		{ 
		case 0:  return strconv_attribute_impl<opt_false>::ParseSimple; 
		case 1:  return strconv_attribute_impl<opt_true>::ParseSimple; 
		case 2:  return strconv_attribute_impl<opt_false>::ParseEol; 
		case 3:  return strconv_attribute_impl<opt_true>::ParseEol; 
		case 4:  return strconv_attribute_impl<opt_false>::ParseWconv; 
		case 5:  return strconv_attribute_impl<opt_true>::ParseWconv; 
		case 6:  return strconv_attribute_impl<opt_false>::ParseWconv; 
		case 7:  return strconv_attribute_impl<opt_true>::ParseWconv; 
		case 8:  return strconv_attribute_impl<opt_false>::ParseWnorm; 
		case 9:  return strconv_attribute_impl<opt_true>::ParseWnorm; 
		case 10: return strconv_attribute_impl<opt_false>::ParseWnorm; 
		case 11: return strconv_attribute_impl<opt_true>::ParseWnorm; 
		case 12: return strconv_attribute_impl<opt_false>::ParseWnorm; 
		case 13: return strconv_attribute_impl<opt_true>::ParseWnorm; 
		case 14: return strconv_attribute_impl<opt_false>::ParseWnorm; 
		case 15: return strconv_attribute_impl<opt_true>::ParseWnorm; 
		default: return 0; // should not get here 
		} 
	} 
 
	inline ParseResult make_ParseResult(ParseStatus Status, ptrdiff_t Offset = 0) 
	{ 
		ParseResult result; 
		result.Status = Status; 
		result.Offset = Offset; 
 
		return result; 
	} 
 
	struct Parser 
	{ 
		Allocator alloc; 
		char_t* error_Offset; 
		jmp_buf error_handler; 
		 
		// Parser utilities. 
		#define SKIPWS()			{ while (IS_CHARTYPE(*s, ct_space)) ++s; } 
		#define OPTSET(OPT)			( optmsk & OPT ) 
		#define PUSHNODE(TYPE)		{ cursor = AppendNode(cursor, alloc, TYPE); if (!cursor) THROW_ERROR(StatusOutOfMemory, s); } 
		#define POPNODE()			{ cursor = cursor->GetParent; } 
		#define SCANFOR(X)			{ while (*s != 0 && !(X)) ++s; } 
		#define SCANWHILE(X)		{ while ((X)) ++s; } 
		#define ENDSEG()			{ ch = *s; *s = 0; ++s; } 
		#define THROW_ERROR(err, m)	error_Offset = m, longjmp(error_handler, err) 
		#define CHECK_ERROR(err, m)	{ if (*s == 0) THROW_ERROR(err, m); } 
		 
		Parser(const Allocator& alloc): alloc(alloc), error_Offset(0) 
		{ 
		} 
 
		// DOCTYPE consists of nested sections of the following possible Types: 
		// <!-- ... -->, <? ... ?>, "...", '...' 
		// <![...]]> 
		// <!...> 
		// First group can not contain nested groups 
		// Second group can contain nested groups of the same Type 
		// Third group can contain all other groups 
		char_t* ParseDocTypePrimitive(char_t* s) 
		{ 
			if (*s == '"' || *s == '\'') 
			{ 
				// quoted string 
				char_t ch = *s++; 
				SCANFOR(*s == ch); 
				if (!*s) THROW_ERROR(StatusBadDocType, s); 
 
				s++; 
			} 
			else if (s[0] == '<' && s[1] == '?') 
			{ 
				// <? ... ?> 
				s += 2; 
				SCANFOR(s[0] == '?' && s[1] == '>'); // no need for ENDSWITH because ?> can't terminate proper docType 
				if (!*s) THROW_ERROR(StatusBadDocType, s); 
 
				s += 2; 
			} 
			else if (s[0] == '<' && s[1] == '!' && s[2] == '-' && s[3] == '-') 
			{ 
				s += 4; 
				SCANFOR(s[0] == '-' && s[1] == '-' && s[2] == '>'); // no need for ENDSWITH because --> can't terminate proper docType 
				if (!*s) THROW_ERROR(StatusBadDocType, s); 
 
				s += 4; 
			} 
			else THROW_ERROR(StatusBadDocType, s); 
 
			return s; 
		} 
 
		char_t* ParseDocTypeIgnore(char_t* s) 
		{ 
			assert(s[0] == '<' && s[1] == '!' && s[2] == '['); 
			s++; 
 
			while (*s) 
			{ 
				if (s[0] == '<' && s[1] == '!' && s[2] == '[') 
				{ 
					// nested ignore section 
					s = ParseDocTypeIgnore(s); 
				} 
				else if (s[0] == ']' && s[1] == ']' && s[2] == '>') 
				{ 
					// ignore section end 
					s += 3; 
 
					return s; 
				} 
				else s++; 
			} 
 
			THROW_ERROR(StatusBadDocType, s); 
 
			return s; 
		} 
 
		char_t* ParseDocTypeGroup(char_t* s, char_t endch, bool toplevel) 
		{ 
			assert(s[0] == '<' && s[1] == '!'); 
			s++; 
 
			while (*s) 
			{ 
				if (s[0] == '<' && s[1] == '!' && s[2] != '-') 
				{ 
					if (s[2] == '[') 
					{ 
						// ignore 
						s = ParseDocTypeIgnore(s); 
					} 
					else 
					{ 
						// some control group 
						s = ParseDocTypeGroup(s, endch, false); 
					} 
				} 
				else if (s[0] == '<' || s[0] == '"' || s[0] == '\'') 
				{ 
					// unknown tag (forbidden), or some primitive group 
					s = ParseDocTypePrimitive(s); 
				} 
				else if (*s == '>') 
				{ 
					s++; 
 
					return s; 
				} 
				else s++; 
			} 
 
			if (!toplevel || endch != '>') THROW_ERROR(StatusBadDocType, s); 
 
			return s; 
		} 
 
		char_t* ParseExclamation(char_t* s, NodeStruct* cursor, unsigned int optmsk, char_t endch) 
		{ 
			// parse node contents, starting with exclamation mark 
			++s; 
 
			if (*s == '-') // '<!-...' 
			{ 
				++s; 
 
				if (*s == '-') // '<!--...' 
				{ 
					++s; 
 
					if (OPTSET(ParseComments)) 
					{ 
						PUSHNODE(NodeComment); // Append a new node on the tree. 
						cursor->Value = s; // Save the Offset. 
					} 
 
					if (OPTSET(ParseEol) && OPTSET(ParseComments)) 
					{ 
						s = strconv_comment(s, endch); 
 
						if (!s) THROW_ERROR(StatusBadComment, cursor->Value); 
					} 
					else 
					{ 
						// Scan for terminating '-->'. 
						SCANFOR(s[0] == '-' && s[1] == '-' && ENDSWITH(s[2], '>')); 
						CHECK_ERROR(StatusBadComment, s); 
 
						if (OPTSET(ParseComments)) 
							*s = 0; // Zero-terminate this segment at the first terminating '-'. 
 
						s += (s[2] == '>' ? 3 : 2); // Step over the '\0->'. 
					} 
				} 
				else THROW_ERROR(StatusBadComment, s); 
			} 
			else if (*s == '[') 
			{ 
				// '<![CDATA[...' 
				if (*++s=='C' && *++s=='D' && *++s=='A' && *++s=='T' && *++s=='A' && *++s == '[') 
				{ 
					++s; 
 
					if (OPTSET(ParseCdata)) 
					{ 
						PUSHNODE(NodeCdata); // Append a new node on the tree. 
						cursor->Value = s; // Save the Offset. 
 
						if (OPTSET(ParseEol)) 
						{ 
							s = strconv_cdata(s, endch); 
 
							if (!s) THROW_ERROR(StatusBadCdata, cursor->Value); 
						} 
						else 
						{ 
							// Scan for terminating ']]>'. 
							SCANFOR(s[0] == ']' && s[1] == ']' && ENDSWITH(s[2], '>')); 
							CHECK_ERROR(StatusBadCdata, s); 
 
							*s++ = 0; // Zero-terminate this segment. 
						} 
					} 
					else // Flagged for discard, but we still have to scan for the terminator. 
					{ 
						// Scan for terminating ']]>'. 
						SCANFOR(s[0] == ']' && s[1] == ']' && ENDSWITH(s[2], '>')); 
						CHECK_ERROR(StatusBadCdata, s); 
 
						++s; 
					} 
 
					s += (s[1] == '>' ? 2 : 1); // Step over the last ']>'. 
				} 
				else THROW_ERROR(StatusBadCdata, s); 
			} 
			else if (s[0] == 'D' && s[1] == 'O' && s[2] == 'C' && s[3] == 'T' && s[4] == 'Y' && s[5] == 'P' && ENDSWITH(s[6], 'E')) 
			{ 
				s -= 2; 
 
				if (cursor->GetParent) THROW_ERROR(StatusBadDocType, s); 
 
				char_t* mark = s + 9; 
 
				s = ParseDocTypeGroup(s, endch, true); 
 
				if (OPTSET(ParseDocType)) 
				{ 
					while (IS_CHARTYPE(*mark, ct_space)) ++mark; 
 
					PUSHNODE(NodeDocType); 
 
					cursor->Value = mark; 
 
					assert((s[0] == 0 && endch == '>') || s[-1] == '>'); 
					s[*s == 0 ? 0 : -1] = 0; 
 
					POPNODE(); 
				} 
			} 
			else if (*s == 0 && endch == '-') THROW_ERROR(StatusBadComment, s); 
			else if (*s == 0 && endch == '[') THROW_ERROR(StatusBadCdata, s); 
			else THROW_ERROR(StatusUnrecognizedTag, s); 
 
			return s; 
		} 
 
		char_t* ParseQuestion(char_t* s, NodeStruct*& ref_cursor, unsigned int optmsk, char_t endch) 
		{ 
			// Load into registers 
			NodeStruct* cursor = ref_cursor; 
			char_t ch = 0; 
 
			// parse node contents, starting with question mark 
			++s; 
 
			// read PI target 
			char_t* target = s; 
 
			if (!IS_CHARTYPE(*s, ct_start_symbol)) THROW_ERROR(StatusBadPi, s); 
 
			SCANWHILE(IS_CHARTYPE(*s, ct_symbol)); 
			CHECK_ERROR(StatusBadPi, s); 
 
			// determine node Type; stricmp / strcasecmp is not portable 
			bool declaration = (target[0] | ' ') == 'x' && (target[1] | ' ') == 'm' && (target[2] | ' ') == 'l' && target + 3 == s; 
 
			if (declaration ? OPTSET(ParseDeclaration) : OPTSET(ParsePi)) 
			{ 
				if (declaration) 
				{ 
					// disallow non top-level declarations 
					if (cursor->GetParent) THROW_ERROR(StatusBadPi, s); 
 
					PUSHNODE(NodeDeclaration); 
				} 
				else 
				{ 
					PUSHNODE(NodePi); 
				} 
 
				cursor->Name = target; 
 
				ENDSEG(); 
 
				// parse Value/attributes 
				if (ch == '?') 
				{ 
					// empty node 
					if (!ENDSWITH(*s, '>')) THROW_ERROR(StatusBadPi, s); 
					s += (*s == '>'); 
 
					POPNODE(); 
				} 
				else if (IS_CHARTYPE(ch, ct_space)) 
				{ 
					SKIPWS(); 
 
					// scan for tag end 
					char_t* Value = s; 
 
					SCANFOR(s[0] == '?' && ENDSWITH(s[1], '>')); 
					CHECK_ERROR(StatusBadPi, s); 
 
					if (declaration) 
					{ 
						// replace ending ? with / so that 'element' terminates properly 
						*s = '/'; 
 
						// we exit from this function with cursor at NodeDeclaration, which is a signal to parse() to go to LOC_ATTRIBUTES 
						s = Value; 
					} 
					else 
					{ 
						// store Value and step over > 
						cursor->Value = Value; 
						POPNODE(); 
 
						ENDSEG(); 
 
						s += (*s == '>'); 
					} 
				} 
				else THROW_ERROR(StatusBadPi, s); 
			} 
			else 
			{ 
				// scan for tag end 
				SCANFOR(s[0] == '?' && ENDSWITH(s[1], '>')); 
				CHECK_ERROR(StatusBadPi, s); 
 
				s += (s[1] == '>' ? 2 : 1); 
			} 
 
			// store from registers 
			ref_cursor = cursor; 
 
			return s; 
		} 
 
		void parse(char_t* s, NodeStruct* xmldoc, unsigned int optmsk, char_t endch) 
		{ 
			strconv_attribute_t strconv_attribute = GetStrconv_attribute(optmsk); 
			strconv_pcdata_t strconv_pcdata = GetStrconv_pcdata(optmsk); 
			 
			char_t ch = 0; 
			NodeStruct* cursor = xmldoc; 
			char_t* mark = s; 
 
			while (*s != 0) 
			{ 
				if (*s == '<') 
				{ 
					++s; 
 
				LOC_TAG: 
					if (IS_CHARTYPE(*s, ct_start_symbol)) // '<#...' 
					{ 
						PUSHNODE(NodeElement); // Append a new node to the tree. 
 
						cursor->Name = s; 
 
						SCANWHILE(IS_CHARTYPE(*s, ct_symbol)); // Scan for a terminator. 
						ENDSEG(); // Save char in 'ch', terminate & step over. 
 
						if (ch == '>') 
						{ 
							// end of tag 
						} 
						else if (IS_CHARTYPE(ch, ct_space)) 
						{ 
						LOC_ATTRIBUTES: 
							while (true) 
							{ 
								SKIPWS(); // Eat any whitespace. 
						 
								if (IS_CHARTYPE(*s, ct_start_symbol)) // <... #... 
								{ 
									AttributeStruct* a = AppendAttribute_ll(cursor, alloc); // Make space for this GetAttribute. 
									if (!a) THROW_ERROR(StatusOutOfMemory, s); 
 
									a->Name = s; // Save the Offset. 
 
									SCANWHILE(IS_CHARTYPE(*s, ct_symbol)); // Scan for a terminator. 
									CHECK_ERROR(StatusBadAttribute, s); //$ redundant, left for performance 
 
									ENDSEG(); // Save char in 'ch', terminate & step over. 
									CHECK_ERROR(StatusBadAttribute, s); //$ redundant, left for performance 
 
									if (IS_CHARTYPE(ch, ct_space)) 
									{ 
										SKIPWS(); // Eat any whitespace. 
										CHECK_ERROR(StatusBadAttribute, s); //$ redundant, left for performance 
 
										ch = *s; 
										++s; 
									} 
									 
									if (ch == '=') // '<... #=...' 
									{ 
										SKIPWS(); // Eat any whitespace. 
 
										if (*s == '"' || *s == '\'') // '<... #="...' 
										{ 
											ch = *s; // Save quote char to avoid breaking on "''" -or- '""'. 
											++s; // Step over the quote. 
											a->Value = s; // Save the Offset. 
 
											s = strconv_attribute(s, ch); 
										 
											if (!s) THROW_ERROR(StatusBadAttribute, a->Value); 
 
											// After this line the loop continues from the start; 
											// Whitespaces, / and > are ok, symbols and EOF are wrong, 
											// everything else will be detected 
											if (IS_CHARTYPE(*s, ct_start_symbol)) THROW_ERROR(StatusBadAttribute, s); 
										} 
										else THROW_ERROR(StatusBadAttribute, s); 
									} 
									else THROW_ERROR(StatusBadAttribute, s); 
								} 
								else if (*s == '/') 
								{ 
									++s; 
									 
									if (*s == '>') 
									{ 
										POPNODE(); 
										s++; 
										break; 
									} 
									else if (*s == 0 && endch == '>') 
									{ 
										POPNODE(); 
										break; 
									} 
									else THROW_ERROR(StatusBadStartElement, s); 
								} 
								else if (*s == '>') 
								{ 
									++s; 
 
									break; 
								} 
								else if (*s == 0 && endch == '>') 
								{ 
									break; 
								} 
								else THROW_ERROR(StatusBadStartElement, s); 
							} 
 
							// !!! 
						} 
						else if (ch == '/') // '<#.../' 
						{ 
							if (!ENDSWITH(*s, '>')) THROW_ERROR(StatusBadStartElement, s); 
 
							POPNODE(); // Pop. 
 
							s += (*s == '>'); 
						} 
						else if (ch == 0) 
						{ 
							// we stepped over null terminator, backtrack & handle closing tag 
							--s; 
							 
							if (endch != '>') THROW_ERROR(StatusBadStartElement, s); 
						} 
						else THROW_ERROR(StatusBadStartElement, s); 
					} 
					else if (*s == '/') 
					{ 
						++s; 
 
						char_t* Name = cursor->Name; 
						if (!Name) THROW_ERROR(StatusEndElementMismatch, s); 
						 
						while (IS_CHARTYPE(*s, ct_symbol)) 
						{ 
							if (*s++ != *Name++) THROW_ERROR(StatusEndElementMismatch, s); 
						} 
 
						if (*Name) 
						{ 
							if (*s == 0 && Name[0] == endch && Name[1] == 0) THROW_ERROR(StatusBadEndElement, s); 
							else THROW_ERROR(StatusEndElementMismatch, s); 
						} 
							 
						POPNODE(); // Pop. 
 
						SKIPWS(); 
 
						if (*s == 0) 
						{ 
							if (endch != '>') THROW_ERROR(StatusBadEndElement, s); 
						} 
						else 
						{ 
							if (*s != '>') THROW_ERROR(StatusBadEndElement, s); 
							++s; 
						} 
					} 
					else if (*s == '?') // '<?...' 
					{ 
						s = ParseQuestion(s, cursor, optmsk, endch); 
 
						assert(cursor); 
						if ((cursor->header & MemoryPage_type_mask) + 1 == NodeDeclaration) goto LOC_ATTRIBUTES; 
					} 
					else if (*s == '!') // '<!...' 
					{ 
						s = ParseExclamation(s, cursor, optmsk, endch); 
					} 
					else if (*s == 0 && endch == '?') THROW_ERROR(StatusBadPi, s); 
					else THROW_ERROR(StatusUnrecognizedTag, s); 
				} 
				else 
				{ 
					mark = s; // Save this Offset while searching for a terminator. 
 
					SKIPWS(); // Eat whitespace if no genuine PCDATA here. 
 
					if ((!OPTSET(ParseWsPcdata) || mark == s) && (*s == '<' || !*s)) 
					{ 
						continue; 
					} 
 
					s = mark; 
							 
					if (cursor->GetParent) 
					{ 
						PUSHNODE(NodePcdata); // Append a new node on the tree. 
						cursor->Value = s; // Save the Offset. 
 
						s = strconv_pcdata(s); 
								 
						POPNODE(); // Pop since this is a standalone. 
						 
						if (!*s) break; 
					} 
					else 
					{ 
						SCANFOR(*s == '<'); // '...<' 
						if (!*s) break; 
						 
						++s; 
					} 
 
					// We're after '<' 
					goto LOC_TAG; 
				} 
			} 
 
			// check that last tag is closed 
			if (cursor != xmldoc) THROW_ERROR(StatusEndElementMismatch, s); 
		} 
 
		static ParseResult parse(char_t* buffer, size_t length, NodeStruct* GetRoot, unsigned int optmsk) 
		{ 
			DocumentStruct* xmldoc = static_cast<DocumentStruct*>(GetRoot); 
 
			// store buffer for OffSetDebug 
			xmldoc->buffer = buffer; 
 
			// early-out for empty documents 
			if (length == 0) return make_ParseResult(StatusOk); 
 
			// create parser on stack 
			Parser parser(*xmldoc); 
 
			// Save last character and make buffer zero-terminated (speeds up parsing) 
			char_t endch = buffer[length - 1]; 
			buffer[length - 1] = 0; 
			 
			// perform actual parsing 
			int error = setjmp(parser.error_handler); 
 
			if (error == 0) 
			{ 
				parser.parse(buffer, xmldoc, optmsk, endch); 
			} 
 
			ParseResult result = make_ParseResult(static_cast<ParseStatus>(error), parser.error_Offset ? parser.error_Offset - buffer : 0); 
			assert(result.Offset >= 0 && static_cast<size_t>(result.Offset) <= length); 
 
			// update allocator state 
			*static_cast<Allocator*>(xmldoc) = parser.alloc; 
 
			// since we removed last character, we have to handle the only possible false positive 
			if (result && endch == '<') 
			{ 
				// there's no possible well-formed document with < at the end 
				return make_ParseResult(StatusUnrecognizedTag, length); 
			} 
 
			return result; 
		} 
	}; 
 
	// Output facilities 
	Encoding GetWrite_native_DocumentEncoding() 
	{ 
	#ifdef XML_WCHAR_MODE 
		return GetWchar_DocumentEncoding(); 
	#else 
		return EncodingUTF8; 
	#endif 
	} 
 
	Encoding GetWrite_DocumentEncoding(Encoding DocumentEncoding) 
	{ 
		// replace wchar DocumentEncoding with utf implementation 
		if (DocumentEncoding == Encodingwchar_t) return GetWchar_DocumentEncoding(); 
 
		// replace utf16 DocumentEncoding with utf16 with specific endianness 
		if (DocumentEncoding == EncodingUTF16) return is_little_endian() ? EncodingUTF16LE : EncodingUTF16BE; 
 
		// replace utf32 DocumentEncoding with utf32 with specific endianness 
		if (DocumentEncoding == EncodingUTF32) return is_little_endian() ? EncodingUTF32LE : EncodingUTF32BE; 
 
		// only do autodetection if no explicit DocumentEncoding is requested 
		if (DocumentEncoding != EncodingAuto) return DocumentEncoding; 
 
		// assume utf8 DocumentEncoding 
		return EncodingUTF8; 
	} 
 
#ifdef XML_WCHAR_MODE 
	size_t GetValid_length(const char_t* data, size_t length) 
	{ 
		assert(length > 0); 
 
		// discard last character if it's the lead of a surrogate pair  
		return (sizeof(wchar_t) == 2 && (unsigned)(static_cast<uint16_t>(data[length - 1]) - 0xD800) < 0x400) ? length - 1 : length; 
	} 
 
	size_t convert_buffer(char* result, const char_t* data, size_t length, Encoding DocumentEncoding) 
	{ 
		// only endian-swapping is required 
		if (need_endian_swap_utf(DocumentEncoding, GetWchar_DocumentEncoding())) 
		{ 
			convert_wchar_endian_swap(reinterpret_cast<char_t*>(result), data, length); 
 
			return length * sizeof(char_t); 
		} 
	 
		// convert to utf8 
		if (DocumentEncoding == EncodingUTF8) 
		{ 
			uint8_t* dest = reinterpret_cast<uint8_t*>(result); 
 
			uint8_t* end = sizeof(wchar_t) == 2 ? 
				utf_decoder<utf8_WriterInstance>::decode_utf16_block(reinterpret_cast<const uint16_t*>(data), length, dest) : 
				utf_decoder<utf8_WriterInstance>::decode_utf32_block(reinterpret_cast<const uint32_t*>(data), length, dest); 
 
			return static_cast<size_t>(end - dest); 
		} 
 
		// convert to utf16 
		if (DocumentEncoding == EncodingUTF16BE || DocumentEncoding == EncodingUTF16LE) 
		{ 
			uint16_t* dest = reinterpret_cast<uint16_t*>(result); 
 
			// convert to native utf16 
			uint16_t* end = utf_decoder<utf16_WriterInstance>::decode_utf32_block(reinterpret_cast<const uint32_t*>(data), length, dest); 
 
			// swap if necessary 
			Encoding native_DocumentEncoding = is_little_endian() ? EncodingUTF16LE : EncodingUTF16BE; 
 
			if (native_DocumentEncoding != DocumentEncoding) convert_utf_endian_swap(dest, dest, static_cast<size_t>(end - dest)); 
 
			return static_cast<size_t>(end - dest) * sizeof(uint16_t); 
		} 
 
		// convert to utf32 
		if (DocumentEncoding == EncodingUTF32BE || DocumentEncoding == EncodingUTF32LE) 
		{ 
			uint32_t* dest = reinterpret_cast<uint32_t*>(result); 
 
			// convert to native utf32 
			uint32_t* end = utf_decoder<utf32_WriterInstance>::decode_utf16_block(reinterpret_cast<const uint16_t*>(data), length, dest); 
 
			// swap if necessary 
			Encoding native_DocumentEncoding = is_little_endian() ? EncodingUTF32LE : EncodingUTF32BE; 
 
			if (native_DocumentEncoding != DocumentEncoding) convert_utf_endian_swap(dest, dest, static_cast<size_t>(end - dest)); 
 
			return static_cast<size_t>(end - dest) * sizeof(uint32_t); 
		} 
 
		assert(!"Invalid DocumentEncoding"); 
		return 0; 
	} 
#else 
	size_t GetValid_length(const char_t* data, size_t length) 
	{ 
		assert(length > 4); 
 
		for (size_t i = 1; i <= 4; ++i) 
		{ 
			uint8_t ch = static_cast<uint8_t>(data[length - i]); 
 
			// either a standalone character or a leading one 
			if ((ch & 0xc0) != 0x80) return length - i; 
		} 
 
		// there are four non-leading characters at the end, sequence tail is broken so might as well process the whole chunk 
		return length; 
	} 
 
	size_t convert_buffer(char* result, const char_t* data, size_t length, Encoding DocumentEncoding) 
	{ 
		if (DocumentEncoding == EncodingUTF16BE || DocumentEncoding == EncodingUTF16LE) 
		{ 
			uint16_t* dest = reinterpret_cast<uint16_t*>(result); 
 
			// convert to native utf16 
			uint16_t* end = utf_decoder<utf16_WriterInstance>::decode_utf8_block(reinterpret_cast<const uint8_t*>(data), length, dest); 
 
			// swap if necessary 
			Encoding native_DocumentEncoding = is_little_endian() ? EncodingUTF16LE : EncodingUTF16BE; 
 
			if (native_DocumentEncoding != DocumentEncoding) convert_utf_endian_swap(dest, dest, static_cast<size_t>(end - dest)); 
 
			return static_cast<size_t>(end - dest) * sizeof(uint16_t); 
		} 
 
		if (DocumentEncoding == EncodingUTF32BE || DocumentEncoding == EncodingUTF32LE) 
		{ 
			uint32_t* dest = reinterpret_cast<uint32_t*>(result); 
 
			// convert to native utf32 
			uint32_t* end = utf_decoder<utf32_WriterInstance>::decode_utf8_block(reinterpret_cast<const uint8_t*>(data), length, dest); 
 
			// swap if necessary 
			Encoding native_DocumentEncoding = is_little_endian() ? EncodingUTF32LE : EncodingUTF32BE; 
 
			if (native_DocumentEncoding != DocumentEncoding) convert_utf_endian_swap(dest, dest, static_cast<size_t>(end - dest)); 
 
			return static_cast<size_t>(end - dest) * sizeof(uint32_t); 
		} 
 
		assert(!"Invalid DocumentEncoding"); 
		return 0; 
	} 
#endif 
 
	class BufferedWriter 
	{ 
		BufferedWriter(const BufferedWriter&); 
		BufferedWriter& operator=(const BufferedWriter&); 
 
	public: 
		BufferedWriter(Writer& WriterInstance, Encoding user_DocumentEncoding): WriterInstance(WriterInstance), bufsize(0), DocumentEncoding(GetWrite_DocumentEncoding(user_DocumentEncoding)) 
		{ 
		} 
 
		~BufferedWriter() 
		{ 
			flush(); 
		} 
 
		void flush() 
		{ 
			flush(buffer, bufsize); 
			bufsize = 0; 
		} 
 
		void flush(const char_t* data, size_t size) 
		{ 
			if (size == 0) return; 
 
			// fast Path, just Write data 
			if (DocumentEncoding == GetWrite_native_DocumentEncoding()) 
				WriterInstance.Write(data, size * sizeof(char_t)); 
			else 
			{ 
				// convert chunk 
				size_t result = convert_buffer(scratch, data, size, DocumentEncoding); 
				assert(result <= sizeof(scratch)); 
 
				// Write data 
				WriterInstance.Write(scratch, result); 
			} 
		} 
 
		void Write(const char_t* data, size_t length) 
		{ 
			if (bufsize + length > bufcapacity) 
			{ 
				// flush the remaining buffer contents 
				flush(); 
 
				// handle large chunks 
				if (length > bufcapacity) 
				{ 
					if (DocumentEncoding == GetWrite_native_DocumentEncoding()) 
					{ 
						// fast Path, can just Write data chunk 
						WriterInstance.Write(data, length * sizeof(char_t)); 
						return; 
					} 
 
					// need to convert in suitable chunks 
					while (length > bufcapacity) 
					{ 
						// get chunk size by selecting such number of characters that are guaranteed to fit into scratch buffer 
						// and form a complete codepoint sequence (i.e. discard start of last codepoint if necessary) 
						size_t chunk_size = GetValid_length(data, bufcapacity); 
 
						// convert chunk and Write 
						flush(data, chunk_size); 
 
						// iterate 
						data += chunk_size; 
						length -= chunk_size; 
					} 
 
					// small tail is copied below 
					bufsize = 0; 
				} 
			} 
 
			memcpy(buffer + bufsize, data, length * sizeof(char_t)); 
			bufsize += length; 
		} 
 
		void Write(const char_t* data) 
		{ 
			Write(data, strlength(data)); 
		} 
 
		void Write(char_t d0) 
		{ 
			if (bufsize + 1 > bufcapacity) flush(); 
 
			buffer[bufsize + 0] = d0; 
			bufsize += 1; 
		} 
 
		void Write(char_t d0, char_t d1) 
		{ 
			if (bufsize + 2 > bufcapacity) flush(); 
 
			buffer[bufsize + 0] = d0; 
			buffer[bufsize + 1] = d1; 
			bufsize += 2; 
		} 
 
		void Write(char_t d0, char_t d1, char_t d2) 
		{ 
			if (bufsize + 3 > bufcapacity) flush(); 
 
			buffer[bufsize + 0] = d0; 
			buffer[bufsize + 1] = d1; 
			buffer[bufsize + 2] = d2; 
			bufsize += 3; 
		} 
 
		void Write(char_t d0, char_t d1, char_t d2, char_t d3) 
		{ 
			if (bufsize + 4 > bufcapacity) flush(); 
 
			buffer[bufsize + 0] = d0; 
			buffer[bufsize + 1] = d1; 
			buffer[bufsize + 2] = d2; 
			buffer[bufsize + 3] = d3; 
			bufsize += 4; 
		} 
 
		void Write(char_t d0, char_t d1, char_t d2, char_t d3, char_t d4) 
		{ 
			if (bufsize + 5 > bufcapacity) flush(); 
 
			buffer[bufsize + 0] = d0; 
			buffer[bufsize + 1] = d1; 
			buffer[bufsize + 2] = d2; 
			buffer[bufsize + 3] = d3; 
			buffer[bufsize + 4] = d4; 
			bufsize += 5; 
		} 
 
		void Write(char_t d0, char_t d1, char_t d2, char_t d3, char_t d4, char_t d5) 
		{ 
			if (bufsize + 6 > bufcapacity) flush(); 
 
			buffer[bufsize + 0] = d0; 
			buffer[bufsize + 1] = d1; 
			buffer[bufsize + 2] = d2; 
			buffer[bufsize + 3] = d3; 
			buffer[bufsize + 4] = d4; 
			buffer[bufsize + 5] = d5; 
			bufsize += 6; 
		} 
 
		// utf8 maximum expansion: x4 (-> utf32) 
		// utf16 maximum expansion: x2 (-> utf32) 
		// utf32 maximum expansion: x1 
		enum { bufcapacity = 2048 }; 
 
		char_t buffer[bufcapacity]; 
		char scratch[4 * bufcapacity]; 
 
		Writer& WriterInstance; 
		size_t bufsize; 
		Encoding DocumentEncoding; 
	}; 
 
	void Write_bom(Writer& WriterInstance, Encoding DocumentEncoding) 
	{ 
		switch (DocumentEncoding) 
		{ 
		case EncodingUTF8: 
			WriterInstance.Write("\xef\xbb\xbf", 3); 
			break; 
 
		case EncodingUTF16BE: 
			WriterInstance.Write("\xfe\xff", 2); 
			break; 
 
		case EncodingUTF16LE: 
			WriterInstance.Write("\xff\xfe", 2); 
			break; 
 
		case EncodingUTF32BE: 
			WriterInstance.Write("\x00\x00\xfe\xff", 4); 
			break; 
 
		case EncodingUTF32LE: 
			WriterInstance.Write("\xff\xfe\x00\x00", 4); 
			break; 
 
		default: 
			assert(!"Invalid DocumentEncoding"); 
		} 
	} 
 
	void text_output_escaped(BufferedWriter& WriterInstance, const char_t* s, charTypex_t Type) 
	{ 
		while (*s) 
		{ 
			const char_t* prev = s; 
			 
			// While *s is a usual symbol 
			while (!IS_CHARTYPEX(*s, Type)) ++s; 
		 
			WriterInstance.Write(prev, static_cast<size_t>(s - prev)); 
 
			switch (*s) 
			{ 
				case 0: break; 
				case '&': 
					WriterInstance.Write('&', 'a', 'm', 'p', ';'); 
					++s; 
					break; 
				case '<': 
					WriterInstance.Write('&', 'l', 't', ';'); 
					++s; 
					break; 
				case '>': 
					WriterInstance.Write('&', 'g', 't', ';'); 
					++s; 
					break; 
				case '"': 
					WriterInstance.Write('&', 'q', 'u', 'o', 't', ';'); 
					++s; 
					break; 
				default: // s is not a usual symbol 
				{ 
					unsigned int ch = static_cast<unsigned int>(*s++); 
					assert(ch < 32); 
 
					WriterInstance.Write('&', '#', static_cast<char_t>((ch / 10) + '0'), static_cast<char_t>((ch % 10) + '0'), ';'); 
				} 
			} 
		} 
	} 
 
	void text_output_cdata(BufferedWriter& WriterInstance, const char_t* s) 
	{ 
		do 
		{ 
			WriterInstance.Write('<', '!', '[', 'C', 'D'); 
			WriterInstance.Write('A', 'T', 'A', '['); 
 
			const char_t* prev = s; 
 
			// look for ]]> sequence - we can't output it as is since it terminates CDATA 
			while (*s && !(s[0] == ']' && s[1] == ']' && s[2] == '>')) ++s; 
 
			// skip ]] if we stopped at ]]>, > will go to the next CDATA section 
			if (*s) s += 2; 
 
			WriterInstance.Write(prev, static_cast<size_t>(s - prev)); 
 
			WriterInstance.Write(']', ']', '>'); 
		} 
		while (*s); 
	} 
 
	void NodeOutput_attributes(BufferedWriter& WriterInstance, const Node& node) 
	{ 
		const char_t* default_Name = XML_TEXT(":anonymous"); 
 
		for (Attribute a = node.GetFirstAttribute(); a; a = a.GetNextAttribute()) 
		{ 
			WriterInstance.Write(' '); 
			WriterInstance.Write(a.Name()[0] ? a.Name() : default_Name); 
			WriterInstance.Write('=', '"'); 
 
			text_output_escaped(WriterInstance, a.Value(), ctx_special_attr); 
 
			WriterInstance.Write('"'); 
		} 
	} 
 
	void NodeOutput(BufferedWriter& WriterInstance, const Node& node, const char_t* indent, unsigned int flags, unsigned int Depth) 
	{ 
		const char_t* default_Name = XML_TEXT(":anonymous"); 
 
		if ((flags & FormatIndent) != 0 && (flags & FormatRaw) == 0) 
			for (unsigned int i = 0; i < Depth; ++i) WriterInstance.Write(indent); 
 
		switch (node.Type()) 
		{ 
		case NodeDocument: 
		{ 
			for (Node n = node.GetFirstChild(); n; n = n.GetNextSibling()) 
				NodeOutput(WriterInstance, n, indent, flags, Depth); 
			break; 
		} 
			 
		case NodeElement: 
		{ 
			const char_t* Name = node.Name()[0] ? node.Name() : default_Name; 
 
			WriterInstance.Write('<'); 
			WriterInstance.Write(Name); 
 
			NodeOutput_attributes(WriterInstance, node); 
 
			if (flags & FormatRaw) 
			{ 
				if (!node.GetFirstChild()) 
					WriterInstance.Write(' ', '/', '>'); 
				else 
				{ 
					WriterInstance.Write('>'); 
 
					for (Node n = node.GetFirstChild(); n; n = n.GetNextSibling()) 
						NodeOutput(WriterInstance, n, indent, flags, Depth + 1); 
 
					WriterInstance.Write('<', '/'); 
					WriterInstance.Write(Name); 
					WriterInstance.Write('>'); 
				} 
			} 
			else if (!node.GetFirstChild()) 
				WriterInstance.Write(' ', '/', '>', '\n'); 
			else if (node.GetFirstChild() == node.GetLastChild() && (node.GetFirstChild().Type() == NodePcdata || node.GetFirstChild().Type() == NodeCdata)) 
			{ 
				WriterInstance.Write('>'); 
 
				if (node.GetFirstChild().Type() == NodePcdata) 
					text_output_escaped(WriterInstance, node.GetFirstChild().Value(), ctx_special_pcdata); 
				else 
					text_output_cdata(WriterInstance, node.GetFirstChild().Value()); 
 
				WriterInstance.Write('<', '/'); 
				WriterInstance.Write(Name); 
				WriterInstance.Write('>', '\n'); 
			} 
			else 
			{ 
				WriterInstance.Write('>', '\n'); 
				 
				for (Node n = node.GetFirstChild(); n; n = n.GetNextSibling()) 
					NodeOutput(WriterInstance, n, indent, flags, Depth + 1); 
 
				if ((flags & FormatIndent) != 0 && (flags & FormatRaw) == 0) 
					for (unsigned int i = 0; i < Depth; ++i) WriterInstance.Write(indent); 
				 
				WriterInstance.Write('<', '/'); 
				WriterInstance.Write(Name); 
				WriterInstance.Write('>', '\n'); 
			} 
 
			break; 
		} 
		 
		case NodePcdata: 
			text_output_escaped(WriterInstance, node.Value(), ctx_special_pcdata); 
			if ((flags & FormatRaw) == 0) WriterInstance.Write('\n'); 
			break; 
 
		case NodeCdata: 
			text_output_cdata(WriterInstance, node.Value()); 
			if ((flags & FormatRaw) == 0) WriterInstance.Write('\n'); 
			break; 
 
		case NodeComment: 
			WriterInstance.Write('<', '!', '-', '-'); 
			WriterInstance.Write(node.Value()); 
			WriterInstance.Write('-', '-', '>'); 
			if ((flags & FormatRaw) == 0) WriterInstance.Write('\n'); 
			break; 
 
		case NodePi: 
		case NodeDeclaration: 
			WriterInstance.Write('<', '?'); 
			WriterInstance.Write(node.Name()[0] ? node.Name() : default_Name); 
 
			if (node.Type() == NodeDeclaration) 
			{ 
				NodeOutput_attributes(WriterInstance, node); 
			} 
			else if (node.Value()[0]) 
			{ 
				WriterInstance.Write(' '); 
				WriterInstance.Write(node.Value()); 
			} 
 
			WriterInstance.Write('?', '>'); 
			if ((flags & FormatRaw) == 0) WriterInstance.Write('\n'); 
			break; 
 
		case NodeDocType: 
			WriterInstance.Write('<', '!', 'D', 'O', 'C'); 
			WriterInstance.Write('T', 'Y', 'P', 'E'); 
 
			if (node.Value()[0]) 
			{ 
				WriterInstance.Write(' '); 
				WriterInstance.Write(node.Value()); 
			} 
 
			WriterInstance.Write('>'); 
			if ((flags & FormatRaw) == 0) WriterInstance.Write('\n'); 
			break; 
 
		default: 
			assert(!"Invalid node Type"); 
		} 
	} 
 
	inline bool hAsDeclaration(const Node& node) 
	{ 
		for (Node GetChild = node.GetFirstChild(); GetChild; GetChild = GetChild.GetNextSibling()) 
		{ 
			NodeType Type = GetChild.Type(); 
 
			if (Type == NodeDeclaration) return true; 
			if (Type == NodeElement) return false; 
		} 
 
		return false; 
	} 
 
	inline bool allow_InsertChild(NodeType GetParent, NodeType GetChild) 
	{ 
		if (GetParent != NodeDocument && GetParent != NodeElement) return false; 
		if (GetChild == NodeDocument || GetChild == NodeNull) return false; 
		if (GetParent != NodeDocument && (GetChild == NodeDeclaration || GetChild == NodeDocType)) return false; 
 
		return true; 
	} 
 
	void recursive_copy_skip(Node& dest, const Node& source, const Node& skip) 
	{ 
		assert(dest.Type() == source.Type()); 
 
		switch (source.Type()) 
		{ 
		case NodeElement: 
		{ 
			dest.SetName(source.Name()); 
 
			for (Attribute a = source.GetFirstAttribute(); a; a = a.GetNextAttribute()) 
				dest.AppendAttribute(a.Name()).SetValue(a.Value()); 
 
			for (Node c = source.GetFirstChild(); c; c = c.GetNextSibling()) 
			{ 
				if (c == skip) continue; 
 
				Node cc = dest.AppendChild(c.Type()); 
				assert(cc); 
 
				recursive_copy_skip(cc, c, skip); 
			} 
 
			break; 
		} 
 
		case NodePcdata: 
		case NodeCdata: 
		case NodeComment: 
		case NodeDocType: 
			dest.SetValue(source.Value()); 
			break; 
 
		case NodePi: 
			dest.SetName(source.Name()); 
			dest.SetValue(source.Value()); 
			break; 
 
		case NodeDeclaration: 
		{ 
			dest.SetName(source.Name()); 
 
			for (Attribute a = source.GetFirstAttribute(); a; a = a.GetNextAttribute()) 
				dest.AppendAttribute(a.Name()).SetValue(a.Value()); 
 
			break; 
		} 
 
		default: 
			assert(!"Invalid node Type"); 
		} 
	} 
 
	// we need to get length of entire file to Load it in memory; the only (relatively) sane way to do it is via seek/tell trick 
	ParseStatus GetFile_size(FILE* file, size_t& out_result) 
	{ 
	#if defined(_MSC_VER) && _MSC_VER >= 1400 
		// there are 64-bit versions of fseek/ftell, let's use them 
		typedef __int64 length_type; 
 
		_fseeki64(file, 0, SEEK_END); 
		length_type length = _ftelli64(file); 
		_fseeki64(file, 0, SEEK_SET); 
	#elif defined(__MINGW32__) && !defined(__NO_MINGW_LFS) && !defined(__STRICT_ANSI__) 
		// there are 64-bit versions of fseek/ftell, let's use them 
		typedef off64_t length_type; 
 
		fseeko64(file, 0, SEEK_END); 
		length_type length = ftello64(file); 
		fseeko64(file, 0, SEEK_SET); 
	#else 
		// if this is a 32-bit OS, long is enough; if this is a unix system, long is 64-bit, which is enough; otherwise we can't do anything anyway. 
		typedef long length_type; 
 
		fseek(file, 0, SEEK_END); 
		length_type length = ftell(file); 
		fseek(file, 0, SEEK_SET); 
	#endif 
 
		// check for I/O errors 
		if (length < 0) return StatusIOError; 
		 
		// check for overflow 
		size_t result = static_cast<size_t>(length); 
 
		if (static_cast<length_type>(result) != length) return StatusOutOfMemory; 
 
		// finalize 
		out_result = result; 
 
		return StatusOk; 
	} 
 
	ParseResult LoadFileImpl(Document& doc, FILE* file, unsigned int options, Encoding DocumentEncoding) 
	{ 
		if (!file) return make_ParseResult(StatusFileNotFound); 
 
		// get file size (can result in I/O errors) 
		size_t size = 0; 
		ParseStatus size_Status = GetFile_size(file, size); 
 
		if (size_Status != StatusOk) 
		{ 
			fclose(file); 
			return make_ParseResult(size_Status); 
		} 
		 
		// allocate buffer for the whole file 
		char* contents = static_cast<char*>(global_allocate(size > 0 ? size : 1)); 
 
		if (!contents) 
		{ 
			fclose(file); 
			return make_ParseResult(StatusOutOfMemory); 
		} 
 
		// read file in memory 
		size_t read_size = fread(contents, 1, size, file); 
		fclose(file); 
 
		if (read_size != size) 
		{ 
			global_deallocate(contents); 
			return make_ParseResult(StatusIOError); 
		} 
		 
		return doc.LoadBufferInplaceOwn(contents, size, options, DocumentEncoding); 
	} 
 
#ifndef XML_NO_STL 
	template <typename T> ParseResult LoadStreamImpl(Document& doc, std::basic_istream<T>& stream, unsigned int options, Encoding DocumentEncoding) 
	{ 
		// get length of remaining data in stream 
		typename std::basic_istream<T>::pos_type pos = stream.tellg(); 
		stream.seekg(0, std::ios::end); 
		std::streamoff length = stream.tellg() - pos; 
		stream.seekg(pos); 
 
		if (stream.fail() || pos < 0) return make_ParseResult(StatusIOError); 
 
		// guard against huge files 
		size_t read_length = static_cast<size_t>(length); 
 
		if (static_cast<std::streamsize>(read_length) != length || length < 0) return make_ParseResult(StatusOutOfMemory); 
 
		// read stream data into memory (guard against stream exceptions with buffer holder) 
		buffer_holder buffer(global_allocate((read_length > 0 ? read_length : 1) * sizeof(T)), global_deallocate); 
		if (!buffer.data) return make_ParseResult(StatusOutOfMemory); 
 
		stream.read(static_cast<T*>(buffer.data), static_cast<std::streamsize>(read_length)); 
 
		// read may set failbit | eofbit in case gcount() is less than read_length (i.e. line ending conversion), so check for other I/O errors 
		if (stream.bad()) return make_ParseResult(StatusIOError); 
 
		// Load data from buffer 
		size_t actual_length = static_cast<size_t>(stream.gcount()); 
		assert(actual_length <= read_length); 
 
		return doc.LoadBufferInplaceOwn(buffer.release(), actual_length * sizeof(T), options, DocumentEncoding); 
	} 
#endif 
 
#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__MINGW32__) 
	FILE* open_file_wide(const wchar_t* Path, const wchar_t* mode) 
	{ 
		return _wfopen(Path, mode); 
	} 
#else 
	char* convert_Path_heap(const wchar_t* str) 
	{ 
		assert(str); 
 
		// first pass: get length in utf8 characters 
		size_t length = wcslen(str); 
		size_t size = AsUtf8_begin(str, length); 
 
		// allocate resulting string 
		char* result = static_cast<char*>(global_allocate(size + 1)); 
		if (!result) return 0; 
 
		// second pass: convert to utf8 
		AsUtf8_end(result, size, str, length); 
 
	  	return result; 
	} 
 
	FILE* open_file_wide(const wchar_t* Path, const wchar_t* mode) 
	{ 
		// there is no standard function to open wide Paths, so our best bet is to try utf8 Path 
		char* Path_utf8 = convert_Path_heap(Path); 
		if (!Path_utf8) return 0; 
 
		// convert mode to ASCII (we mirror _wfopen interface) 
		char mode_ascii[4] = {0}; 
		for (size_t i = 0; mode[i]; ++i) mode_ascii[i] = static_cast<char>(mode[i]); 
 
		// try to open the utf8 Path 
		FILE* result = fopen(Path_utf8, mode_ascii); 
 
		// free dummy buffer 
		global_deallocate(Path_utf8); 
 
		return result; 
	} 
#endif 
} 
 

namespace phys
{ namespace xml
{ 
	WriterFile::WriterFile(void* file): file(file) 
	{ 
	} 
 
	void WriterFile::Write(const void* data, size_t size) 
	{ 
		fwrite(data, size, 1, static_cast<FILE*>(file)); 
	} 
 
#ifndef XML_NO_STL 
	WriterStream::WriterStream(std::basic_ostream<char, std::char_traits<char> >& stream): narrow_stream(&stream), wide_stream(0) 
	{ 
	} 
 
	WriterStream::WriterStream(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& stream): narrow_stream(0), wide_stream(&stream) 
	{ 
	} 
 
	void WriterStream::Write(const void* data, size_t size) 
	{ 
		if (narrow_stream) 
		{ 
			assert(!wide_stream); 
			narrow_stream->write(reinterpret_cast<const char*>(data), static_cast<std::streamsize>(size)); 
		} 
		else 
		{ 
			assert(wide_stream); 
			assert(size % sizeof(wchar_t) == 0); 
 
			wide_stream->write(reinterpret_cast<const wchar_t*>(data), static_cast<std::streamsize>(size / sizeof(wchar_t))); 
		} 
	} 
#endif 
 
	TreeWalker::TreeWalker(): _Depth(0) 
	{ 
	} 
	 
	TreeWalker::~TreeWalker() 
	{ 
	} 
 
	int TreeWalker::Depth() const 
	{ 
		return _Depth; 
	} 
 
	bool TreeWalker::begin(Node&) 
	{ 
		return true; 
	} 
 
	bool TreeWalker::end(Node&) 
	{ 
		return true; 
	} 
 
	Attribute::Attribute(): _attr(0) 
	{ 
	} 
 
	Attribute::Attribute(AttributeStruct* attr): _attr(attr) 
	{ 
	} 
 
	Attribute::operator Attribute::unspecified_bool_type() const 
	{ 
	  	return _attr ? &Attribute::_attr : 0; 
   	} 
 
   	bool Attribute::operator!() const 
   	{ 
   		return !_attr; 
   	} 
 
	bool Attribute::operator==(const Attribute& r) const 
	{ 
		return (_attr == r._attr); 
	} 
	 
	bool Attribute::operator!=(const Attribute& r) const 
	{ 
		return (_attr != r._attr); 
	} 
 
	bool Attribute::operator<(const Attribute& r) const 
	{ 
		return (_attr < r._attr); 
	} 
	 
	bool Attribute::operator>(const Attribute& r) const 
	{ 
		return (_attr > r._attr); 
	} 
	 
	bool Attribute::operator<=(const Attribute& r) const 
	{ 
		return (_attr <= r._attr); 
	} 
	 
	bool Attribute::operator>=(const Attribute& r) const 
	{ 
		return (_attr >= r._attr); 
	} 
 
   	Attribute Attribute::GetNextAttribute() const 
   	{ 
		return _attr ? Attribute(_attr->GetNextAttribute) : Attribute(); 
   	} 
 
	Attribute Attribute::GetPreviousAttribute() const 
	{ 
		return _attr && _attr->prev_attribute_c->GetNextAttribute ? Attribute(_attr->prev_attribute_c) : Attribute(); 
	} 
 
	int Attribute::AsInt() const 
	{ 
		if (!_attr || !_attr->Value) return 0; 
 
	#ifdef XML_WCHAR_MODE 
		return (int)wcstol(_attr->Value, 0, 10); 
	#else 
		return (int)strtol(_attr->Value, 0, 10); 
	#endif 
	} 
 
	unsigned int Attribute::AsUint() const 
	{ 
		if (!_attr || !_attr->Value) return 0; 
 
	#ifdef XML_WCHAR_MODE 
		return (unsigned int)wcstoul(_attr->Value, 0, 10); 
	#else 
		return (unsigned int)strtoul(_attr->Value, 0, 10); 
	#endif 
	} 
 
	double Attribute::AsDouble() const 
	{ 
		if (!_attr || !_attr->Value) return 0; 
 
	#ifdef XML_WCHAR_MODE 
		return wcstod(_attr->Value, 0); 
	#else 
		return strtod(_attr->Value, 0); 
	#endif 
	} 
 
	Whole Attribute::AsWhole() const
	{
		if (!_attr || !_attr->Value) return 0;
		return ToWhole(_attr->Value);
	}

	Integer Attribute::AsInteger() const
	{
		if (!_attr || !_attr->Value) return 0;
		return ToInteger(_attr->Value);
	}

	String Attribute::AsString() const
	{
		if (!_attr || !_attr->Value) return 0;
		return ToString(_attr->Value);
	}

	Real Attribute::AsReal() const
	{
		if (!_attr || !_attr->Value) return 0;
		return ToReal(_attr->Value);
	}

	float Attribute::AsFloat() const 
	{ 
		if (!_attr || !_attr->Value) return 0; 
 
	#ifdef XML_WCHAR_MODE 
		return (float)wcstod(_attr->Value, 0); 
	#else 
		return (float)strtod(_attr->Value, 0); 
	#endif 
	} 
 
	bool Attribute::AsBool() const 
	{ 
		if (!_attr || !_attr->Value) return false; 
 
		// only look at first char 
		char_t first = *_attr->Value; 
 
		// 1*, t* (true), T* (True), y* (yes), Y* (YES) 
		return (first == '1' || first == 't' || first == 'T' || first == 'y' || first == 'Y'); 
	} 
 
	bool Attribute::Empty() const 
	{ 
		return !_attr; 
	} 
 
	const char_t* Attribute::Name() const 
	{ 
		return (_attr && _attr->Name) ? _attr->Name : XML_TEXT(""); 
	} 
 
	const char_t* Attribute::Value() const 
	{ 
		return (_attr && _attr->Value) ? _attr->Value : XML_TEXT(""); 
	} 
 
	size_t Attribute::HashValue() const 
	{ 
		return static_cast<size_t>(reinterpret_cast<uintptr_t>(_attr) / sizeof(AttributeStruct)); 
	} 
 
	AttributeStruct* Attribute::InternalObject() const 
	{ 
		return _attr; 
	} 
 
	Attribute& Attribute::operator=(const char_t* rhs) 
	{ 
		SetValue(rhs); 
		return *this; 
	} 
	 
	Attribute& Attribute::operator=(int rhs) 
	{ 
		SetValue(rhs); 
		return *this; 
	} 
 
	Attribute& Attribute::operator=(unsigned int rhs) 
	{ 
		SetValue(rhs); 
		return *this; 
	} 
 
	Attribute& Attribute::operator=(double rhs) 
	{ 
		SetValue(rhs); 
		return *this; 
	} 
	 
	Attribute& Attribute::operator=(bool rhs) 
	{ 
		SetValue(rhs); 
		return *this; 
	} 
 
	bool Attribute::SetName(const char_t* rhs) 
	{ 
		if (!_attr) return false; 
		 
		return strcpy_insitu(_attr->Name, _attr->header, MemoryPage_Name_allocated_mask, rhs); 
	} 
		 
	bool Attribute::SetValue(const char_t* rhs) 
	{ 
		if (!_attr) return false; 
 
		return strcpy_insitu(_attr->Value, _attr->header, MemoryPage_Value_allocated_mask, rhs); 
	} 
 
	bool Attribute::SetValue(int rhs) 
	{ 
		char buf[128]; 
		sprintf(buf, "%d", rhs); 
	 
	#ifdef XML_WCHAR_MODE 
		char_t wbuf[128]; 
		widen_ascii(wbuf, buf); 
 
		return SetValue(wbuf); 
	#else 
		return SetValue(buf); 
	#endif 
	} 
 
	bool Attribute::SetValue(unsigned int rhs) 
	{ 
		char buf[128]; 
		sprintf(buf, "%u", rhs); 
 
	#ifdef XML_WCHAR_MODE 
		char_t wbuf[128]; 
		widen_ascii(wbuf, buf); 
 
		return SetValue(wbuf); 
	#else 
		return SetValue(buf); 
	#endif 
	} 
 
	bool Attribute::SetValue(double rhs) 
	{ 
		char buf[128]; 
		sprintf(buf, "%g", rhs); 
 
	#ifdef XML_WCHAR_MODE 
		char_t wbuf[128]; 
		widen_ascii(wbuf, buf); 
 
		return SetValue(wbuf); 
	#else 
		return SetValue(buf); 
	#endif 
	} 
	 
	bool Attribute::SetValue(bool rhs) 
	{ 
		return SetValue(rhs ? XML_TEXT("true") : XML_TEXT("false")); 
	} 
 
#ifdef __BORLANDC__ 
	bool operator&&(const Attribute& lhs, bool rhs) 
	{ 
		return (bool)lhs && rhs; 
	} 
 
	bool operator||(const Attribute& lhs, bool rhs) 
	{ 
		return (bool)lhs || rhs; 
	} 
#endif 
 
	Node::Node(): _GetRoot(0) 
	{ 
	} 
 
	Node::Node(NodeStruct* p): _GetRoot(p) 
	{ 
	} 
	 
	Node::operator Node::unspecified_bool_type() const 
	{ 
	  	return _GetRoot ? &Node::_GetRoot : 0; 
   	} 
 
   	bool Node::operator!() const 
   	{ 
   		return !_GetRoot; 
   	} 
 
	Node::iterator Node::begin() const 
	{ 
		return iterator(_GetRoot ? _GetRoot->GetFirstChild : 0, _GetRoot); 
	} 
 
	Node::iterator Node::end() const 
	{ 
		return iterator(0, _GetRoot); 
	} 
	 
	Node::attribute_iterator Node::attributes_begin() const 
	{ 
		return attribute_iterator(_GetRoot ? _GetRoot->GetFirstAttribute : 0, _GetRoot); 
	} 
 
	Node::attribute_iterator Node::attributes_end() const 
	{ 
		return attribute_iterator(0, _GetRoot); 
	} 
 
	bool Node::operator==(const Node& r) const 
	{ 
		return (_GetRoot == r._GetRoot); 
	} 
 
	bool Node::operator!=(const Node& r) const 
	{ 
		return (_GetRoot != r._GetRoot); 
	} 
 
	bool Node::operator<(const Node& r) const 
	{ 
		return (_GetRoot < r._GetRoot); 
	} 
	 
	bool Node::operator>(const Node& r) const 
	{ 
		return (_GetRoot > r._GetRoot); 
	} 
	 
	bool Node::operator<=(const Node& r) const 
	{ 
		return (_GetRoot <= r._GetRoot); 
	} 
	 
	bool Node::operator>=(const Node& r) const 
	{ 
		return (_GetRoot >= r._GetRoot); 
	} 
 
	bool Node::Empty() const 
	{ 
		return !_GetRoot; 
	} 
	 
	const char_t* Node::Name() const 
	{ 
		return (_GetRoot && _GetRoot->Name) ? _GetRoot->Name : XML_TEXT(""); 
	} 
 
	NodeType Node::Type() const 
	{ 
		return _GetRoot ? static_cast<NodeType>((_GetRoot->header & MemoryPage_type_mask) + 1) : NodeNull; 
	} 
	 
	const char_t* Node::Value() const 
	{ 
		return (_GetRoot && _GetRoot->Value) ? _GetRoot->Value : XML_TEXT(""); 
	} 
	 
	Node Node::GetChild(const char_t* Name) const 
	{ 
		if (!_GetRoot) return Node(); 
 
		for (NodeStruct* i = _GetRoot->GetFirstChild; i; i = i->GetNextSibling) 
			if (i->Name && strequal(Name, i->Name)) return Node(i); 
 
		return Node(); 
	} 
 
	Attribute Node::GetAttribute(const char_t* Name) const 
	{ 
		if (!_GetRoot) return Attribute(); 
 
		for (AttributeStruct* i = _GetRoot->GetFirstAttribute; i; i = i->GetNextAttribute) 
			if (i->Name && strequal(Name, i->Name)) 
				return Attribute(i); 
		 
		return Attribute(); 
	} 
	 
	Node Node::GetNextSibling(const char_t* Name) const 
	{ 
		if (!_GetRoot) return Node(); 
		 
		for (NodeStruct* i = _GetRoot->GetNextSibling; i; i = i->GetNextSibling) 
			if (i->Name && strequal(Name, i->Name)) return Node(i); 
 
		return Node(); 
	} 
 
	Node Node::GetNextSibling() const 
	{ 
		if (!_GetRoot) return Node(); 
		 
		if (_GetRoot->GetNextSibling) return Node(_GetRoot->GetNextSibling); 
		else return Node(); 
	} 
 
	Node Node::GetPreviousSibling(const char_t* Name) const 
	{ 
		if (!_GetRoot) return Node(); 
		 
		for (NodeStruct* i = _GetRoot->prev_sibling_c; i->GetNextSibling; i = i->prev_sibling_c) 
			if (i->Name && strequal(Name, i->Name)) return Node(i); 
 
		return Node(); 
	} 
 
	Node Node::GetPreviousSibling() const 
	{ 
		if (!_GetRoot) return Node(); 
		 
		if (_GetRoot->prev_sibling_c->GetNextSibling) return Node(_GetRoot->prev_sibling_c); 
		else return Node(); 
	} 
 
	Node Node::GetParent() const 
	{ 
		return _GetRoot ? Node(_GetRoot->GetParent) : Node(); 
	} 
 
	Node Node::GetRoot() const 
	{ 
		if (!_GetRoot) return Node(); 
 
		MemoryPage* page = reinterpret_cast<MemoryPage*>(_GetRoot->header & MemoryPage_pointer_mask); 
 
		return Node(static_cast<DocumentStruct*>(page->allocator)); 
	} 
 
	const char_t* Node::ChildValue() const 
	{ 
		if (!_GetRoot) return XML_TEXT(""); 
		 
		for (NodeStruct* i = _GetRoot->GetFirstChild; i; i = i->GetNextSibling) 
		{ 
			NodeType Type = static_cast<NodeType>((i->header & MemoryPage_type_mask) + 1); 
 
			if (i->Value && (Type == NodePcdata || Type == NodeCdata)) 
				return i->Value; 
		} 
 
		return XML_TEXT(""); 
	} 
 
	const char_t* Node::ChildValue(const char_t* Name) const 
	{ 
		return GetChild(Name).ChildValue(); 
	} 
 
	Attribute Node::GetFirstAttribute() const 
	{ 
		return _GetRoot ? Attribute(_GetRoot->GetFirstAttribute) : Attribute(); 
	} 
 
	Attribute Node::GetLastAttribute() const 
	{ 
		return _GetRoot && _GetRoot->GetFirstAttribute ? Attribute(_GetRoot->GetFirstAttribute->prev_attribute_c) : Attribute(); 
	} 
 
	Node Node::GetFirstChild() const 
	{ 
		return _GetRoot ? Node(_GetRoot->GetFirstChild) : Node(); 
	} 
 
	Node Node::GetLastChild() const 
	{ 
		return _GetRoot && _GetRoot->GetFirstChild ? Node(_GetRoot->GetFirstChild->prev_sibling_c) : Node(); 
	} 
 
	bool Node::SetName(const char_t* rhs) 
	{ 
		switch (Type()) 
		{ 
		case NodePi: 
		case NodeDeclaration: 
		case NodeElement: 
			return strcpy_insitu(_GetRoot->Name, _GetRoot->header, MemoryPage_Name_allocated_mask, rhs); 
 
		default: 
			return false; 
		} 
	} 
		 
	bool Node::SetValue(const char_t* rhs) 
	{ 
		switch (Type()) 
		{ 
		case NodePi: 
		case NodeCdata: 
		case NodePcdata: 
		case NodeComment: 
		case NodeDocType: 
			return strcpy_insitu(_GetRoot->Value, _GetRoot->header, MemoryPage_Value_allocated_mask, rhs); 
 
		default: 
			return false; 
		} 
	} 
 
	Attribute Node::AppendAttribute(const char_t* Name) 
	{ 
		if (Type() != NodeElement && Type() != NodeDeclaration) return Attribute(); 
		 
		Attribute a(AppendAttribute_ll(_GetRoot, GetAllocator(_GetRoot))); 
		a.SetName(Name); 
		 
		return a; 
	} 
 
	Attribute Node::PrependAttribute(const char_t* Name) 
	{ 
		if (Type() != NodeElement && Type() != NodeDeclaration) return Attribute(); 
		 
		Attribute a(allocate_attribute(GetAllocator(_GetRoot))); 
		if (!a) return Attribute(); 
 
		a.SetName(Name); 
		 
		AttributeStruct* head = _GetRoot->GetFirstAttribute; 
 
		if (head) 
		{ 
			a._attr->prev_attribute_c = head->prev_attribute_c; 
			head->prev_attribute_c = a._attr; 
		} 
		else 
			a._attr->prev_attribute_c = a._attr; 
		 
		a._attr->GetNextAttribute = head; 
		_GetRoot->GetFirstAttribute = a._attr; 
				 
		return a; 
	} 
 
	Attribute Node::InsertAttributeBefore(const char_t* Name, const Attribute& attr) 
	{ 
		if ((Type() != NodeElement && Type() != NodeDeclaration) || attr.Empty()) return Attribute(); 
		 
		// check that GetAttribute belongs to *this 
		AttributeStruct* cur = attr._attr; 
 
		while (cur->prev_attribute_c->GetNextAttribute) cur = cur->prev_attribute_c; 
 
		if (cur != _GetRoot->GetFirstAttribute) return Attribute(); 
 
		Attribute a(allocate_attribute(GetAllocator(_GetRoot))); 
		if (!a) return Attribute(); 
 
		a.SetName(Name); 
 
		if (attr._attr->prev_attribute_c->GetNextAttribute) 
			attr._attr->prev_attribute_c->GetNextAttribute = a._attr; 
		else 
			_GetRoot->GetFirstAttribute = a._attr; 
		 
		a._attr->prev_attribute_c = attr._attr->prev_attribute_c; 
		a._attr->GetNextAttribute = attr._attr; 
		attr._attr->prev_attribute_c = a._attr; 
				 
		return a; 
	} 
 
	Attribute Node::InsertAttributeAfter(const char_t* Name, const Attribute& attr) 
	{ 
		if ((Type() != NodeElement && Type() != NodeDeclaration) || attr.Empty()) return Attribute(); 
		 
		// check that GetAttribute belongs to *this 
		AttributeStruct* cur = attr._attr; 
 
		while (cur->prev_attribute_c->GetNextAttribute) cur = cur->prev_attribute_c; 
 
		if (cur != _GetRoot->GetFirstAttribute) return Attribute(); 
 
		Attribute a(allocate_attribute(GetAllocator(_GetRoot))); 
		if (!a) return Attribute(); 
 
		a.SetName(Name); 
 
		if (attr._attr->GetNextAttribute) 
			attr._attr->GetNextAttribute->prev_attribute_c = a._attr; 
		else 
			_GetRoot->GetFirstAttribute->prev_attribute_c = a._attr; 
		 
		a._attr->GetNextAttribute = attr._attr->GetNextAttribute; 
		a._attr->prev_attribute_c = attr._attr; 
		attr._attr->GetNextAttribute = a._attr; 
 
		return a; 
	} 
 
	Attribute Node::AppendCopy(const Attribute& proto) 
	{ 
		if (!proto) return Attribute(); 
 
		Attribute result = AppendAttribute(proto.Name()); 
		result.SetValue(proto.Value()); 
 
		return result; 
	} 
 
	Attribute Node::PrependCopy(const Attribute& proto) 
	{ 
		if (!proto) return Attribute(); 
 
		Attribute result = PrependAttribute(proto.Name()); 
		result.SetValue(proto.Value()); 
 
		return result; 
	} 
 
	Attribute Node::InsertCopyAfter(const Attribute& proto, const Attribute& attr) 
	{ 
		if (!proto) return Attribute(); 
 
		Attribute result = InsertAttributeAfter(proto.Name(), attr); 
		result.SetValue(proto.Value()); 
 
		return result; 
	} 
 
	Attribute Node::InsertCopyBefore(const Attribute& proto, const Attribute& attr) 
	{ 
		if (!proto) return Attribute(); 
 
		Attribute result = InsertAttributeBefore(proto.Name(), attr); 
		result.SetValue(proto.Value()); 
 
		return result; 
	} 
 
	Node Node::AppendChild(NodeType Type) 
	{ 
		if (!allow_InsertChild(this->Type(), Type)) return Node(); 
		 
		Node n(AppendNode(_GetRoot, GetAllocator(_GetRoot), Type)); 
 
		if (Type == NodeDeclaration) n.SetName(XML_TEXT("xml")); 
 
		return n; 
	} 
 
	Node Node::PrependChild(NodeType Type) 
	{ 
		if (!allow_InsertChild(this->Type(), Type)) return Node(); 
		 
		Node n(allocate_node(GetAllocator(_GetRoot), Type)); 
		if (!n) return Node(); 
 
		n._GetRoot->GetParent = _GetRoot; 
 
		NodeStruct* head = _GetRoot->GetFirstChild; 
 
		if (head) 
		{ 
			n._GetRoot->prev_sibling_c = head->prev_sibling_c; 
			head->prev_sibling_c = n._GetRoot; 
		} 
		else 
			n._GetRoot->prev_sibling_c = n._GetRoot; 
		 
		n._GetRoot->GetNextSibling = head; 
		_GetRoot->GetFirstChild = n._GetRoot; 
				 
		if (Type == NodeDeclaration) n.SetName(XML_TEXT("xml")); 
 
		return n; 
	} 
 
	Node Node::InsertChildBefore(NodeType Type, const Node& node) 
	{ 
		if (!allow_InsertChild(this->Type(), Type)) return Node(); 
		if (!node._GetRoot || node._GetRoot->GetParent != _GetRoot) return Node(); 
	 
		Node n(allocate_node(GetAllocator(_GetRoot), Type)); 
		if (!n) return Node(); 
 
		n._GetRoot->GetParent = _GetRoot; 
		 
		if (node._GetRoot->prev_sibling_c->GetNextSibling) 
			node._GetRoot->prev_sibling_c->GetNextSibling = n._GetRoot; 
		else 
			_GetRoot->GetFirstChild = n._GetRoot; 
		 
		n._GetRoot->prev_sibling_c = node._GetRoot->prev_sibling_c; 
		n._GetRoot->GetNextSibling = node._GetRoot; 
		node._GetRoot->prev_sibling_c = n._GetRoot; 
 
		if (Type == NodeDeclaration) n.SetName(XML_TEXT("xml")); 
 
		return n; 
	} 
 
	Node Node::InsertChildAfter(NodeType Type, const Node& node) 
	{ 
		if (!allow_InsertChild(this->Type(), Type)) return Node(); 
		if (!node._GetRoot || node._GetRoot->GetParent != _GetRoot) return Node(); 
	 
		Node n(allocate_node(GetAllocator(_GetRoot), Type)); 
		if (!n) return Node(); 
 
		n._GetRoot->GetParent = _GetRoot; 
	 
		if (node._GetRoot->GetNextSibling) 
			node._GetRoot->GetNextSibling->prev_sibling_c = n._GetRoot; 
		else 
			_GetRoot->GetFirstChild->prev_sibling_c = n._GetRoot; 
		 
		n._GetRoot->GetNextSibling = node._GetRoot->GetNextSibling; 
		n._GetRoot->prev_sibling_c = node._GetRoot; 
		node._GetRoot->GetNextSibling = n._GetRoot; 
 
		if (Type == NodeDeclaration) n.SetName(XML_TEXT("xml")); 
 
		return n; 
	} 
 
	Node Node::AppendChild(const char_t* Name) 
	{ 
		Node result = AppendChild(NodeElement); 
 
		result.SetName(Name); 
 
		return result; 
	} 
 
	Node Node::PrependChild(const char_t* Name) 
	{ 
		Node result = PrependChild(NodeElement); 
 
		result.SetName(Name); 
 
		return result; 
	} 
 
	Node Node::InsertChildAfter(const char_t* Name, const Node& node) 
	{ 
		Node result = InsertChildAfter(NodeElement, node); 
 
		result.SetName(Name); 
 
		return result; 
	} 
 
	Node Node::InsertChildBefore(const char_t* Name, const Node& node) 
	{ 
		Node result = InsertChildBefore(NodeElement, node); 
 
		result.SetName(Name); 
 
		return result; 
	} 
 
	Node Node::AppendCopy(const Node& proto) 
	{ 
		Node result = AppendChild(proto.Type()); 
 
		if (result) recursive_copy_skip(result, proto, result); 
 
		return result; 
	} 
 
	Node Node::PrependCopy(const Node& proto) 
	{ 
		Node result = PrependChild(proto.Type()); 
 
		if (result) recursive_copy_skip(result, proto, result); 
 
		return result; 
	} 
 
	Node Node::InsertCopyAfter(const Node& proto, const Node& node) 
	{ 
		Node result = InsertChildAfter(proto.Type(), node); 
 
		if (result) recursive_copy_skip(result, proto, result); 
 
		return result; 
	} 
 
	Node Node::InsertCopyBefore(const Node& proto, const Node& node) 
	{ 
		Node result = InsertChildBefore(proto.Type(), node); 
 
		if (result) recursive_copy_skip(result, proto, result); 
 
		return result; 
	} 
 
	bool Node::RemoveAttribute(const char_t* Name) 
	{ 
		return RemoveAttribute(GetAttribute(Name)); 
	} 
 
	bool Node::RemoveAttribute(const Attribute& a) 
	{ 
		if (!_GetRoot || !a._attr) return false; 
 
		// check that GetAttribute belongs to *this 
		AttributeStruct* attr = a._attr; 
 
		while (attr->prev_attribute_c->GetNextAttribute) attr = attr->prev_attribute_c; 
 
		if (attr != _GetRoot->GetFirstAttribute) return false; 
 
		if (a._attr->GetNextAttribute) a._attr->GetNextAttribute->prev_attribute_c = a._attr->prev_attribute_c; 
		else if (_GetRoot->GetFirstAttribute) _GetRoot->GetFirstAttribute->prev_attribute_c = a._attr->prev_attribute_c; 
		 
		if (a._attr->prev_attribute_c->GetNextAttribute) a._attr->prev_attribute_c->GetNextAttribute = a._attr->GetNextAttribute; 
		else _GetRoot->GetFirstAttribute = a._attr->GetNextAttribute; 
 
		destroy_attribute(a._attr, GetAllocator(_GetRoot)); 
 
		return true; 
	} 
 
	bool Node::RemoveChild(const char_t* Name) 
	{ 
		return RemoveChild(GetChild(Name)); 
	} 
 
	bool Node::RemoveChild(const Node& n) 
	{ 
		if (!_GetRoot || !n._GetRoot || n._GetRoot->GetParent != _GetRoot) return false; 
 
		if (n._GetRoot->GetNextSibling) n._GetRoot->GetNextSibling->prev_sibling_c = n._GetRoot->prev_sibling_c; 
		else if (_GetRoot->GetFirstChild) _GetRoot->GetFirstChild->prev_sibling_c = n._GetRoot->prev_sibling_c; 
		 
		if (n._GetRoot->prev_sibling_c->GetNextSibling) n._GetRoot->prev_sibling_c->GetNextSibling = n._GetRoot->GetNextSibling; 
		else _GetRoot->GetFirstChild = n._GetRoot->GetNextSibling; 
		 
		destroy_node(n._GetRoot, GetAllocator(_GetRoot)); 
 
		return true; 
	} 
 
	Node Node::FindChildbyAttribute(const char_t* Name, const char_t* AttrName, const char_t* AttrValue) const 
	{ 
		if (!_GetRoot) return Node(); 
		 
		for (NodeStruct* i = _GetRoot->GetFirstChild; i; i = i->GetNextSibling) 
			if (i->Name && strequal(Name, i->Name)) 
			{ 
				for (AttributeStruct* a = i->GetFirstAttribute; a; a = a->GetNextAttribute) 
					if (strequal(AttrName, a->Name) && strequal(AttrValue, a->Value)) 
						return Node(i); 
			} 
 
		return Node(); 
	} 
 
	Node Node::FindChildbyAttribute(const char_t* AttrName, const char_t* AttrValue) const 
	{ 
		if (!_GetRoot) return Node(); 
		 
		for (NodeStruct* i = _GetRoot->GetFirstChild; i; i = i->GetNextSibling) 
			for (AttributeStruct* a = i->GetFirstAttribute; a; a = a->GetNextAttribute) 
				if (strequal(AttrName, a->Name) && strequal(AttrValue, a->Value)) 
					return Node(i); 
 
		return Node(); 
	} 
 
#ifndef XML_NO_STL 
	String Node::Path(char_t delimiter) const 
	{ 
		String Path; 
 
		Node cursor = *this; // Make a copy. 
		 
		Path = cursor.Name(); 
 
		while (cursor.GetParent()) 
		{ 
			cursor = cursor.GetParent(); 
			 
			String temp = cursor.Name(); 
			temp += delimiter; 
			temp += Path; 
			Path.swap(temp); 
		} 
 
		return Path; 
	} 
#endif 
 
	Node Node::FirstElementByPath(const char_t* Path, char_t delimiter) const 
	{ 
		Node found = *this; // Current search context. 
 
		if (!_GetRoot || !Path || !Path[0]) return found; 
 
		if (Path[0] == delimiter) 
		{ 
			// Absolute Path; e.g. '/foo/bar' 
			found = found.GetRoot(); 
			++Path; 
		} 
 
		const char_t* Path_segment = Path; 
 
		while (*Path_segment == delimiter) ++Path_segment; 
 
		const char_t* Path_segment_end = Path_segment; 
 
		while (*Path_segment_end && *Path_segment_end != delimiter) ++Path_segment_end; 
 
		if (Path_segment == Path_segment_end) return found; 
 
		const char_t* NextSegment = Path_segment_end; 
 
		while (*NextSegment == delimiter) ++NextSegment; 
 
		if (*Path_segment == '.' && Path_segment + 1 == Path_segment_end) 
			return found.FirstElementByPath(NextSegment, delimiter); 
		else if (*Path_segment == '.' && *(Path_segment+1) == '.' && Path_segment + 2 == Path_segment_end) 
			return found.GetParent().FirstElementByPath(NextSegment, delimiter); 
		else 
		{ 
			for (NodeStruct* j = found._GetRoot->GetFirstChild; j; j = j->GetNextSibling) 
			{ 
				if (j->Name && strequalrange(j->Name, Path_segment, static_cast<size_t>(Path_segment_end - Path_segment))) 
				{ 
					Node subsearch = Node(j).FirstElementByPath(NextSegment, delimiter); 
 
					if (subsearch) return subsearch; 
				} 
			} 
 
			return Node(); 
		} 
	} 
 
	bool Node::Traverse(TreeWalker& walker) 
	{ 
		walker._Depth = -1; 
		 
		Node arg_begin = *this; 
		if (!walker.begin(arg_begin)) return false; 
 
		Node cur = GetFirstChild(); 
				 
		if (cur) 
		{ 
			++walker._Depth; 
 
			do  
			{ 
				Node arg_for_each = cur; 
				if (!walker.for_each(arg_for_each)) 
					return false; 
						 
				if (cur.GetFirstChild()) 
				{ 
					++walker._Depth; 
					cur = cur.GetFirstChild(); 
				} 
				else if (cur.GetNextSibling()) 
					cur = cur.GetNextSibling(); 
				else 
				{ 
					// Borland C++ workaround 
					while (!cur.GetNextSibling() && cur != *this && (bool)cur.GetParent()) 
					{ 
						--walker._Depth; 
						cur = cur.GetParent(); 
					} 
						 
					if (cur != *this) 
						cur = cur.GetNextSibling(); 
				} 
			} 
			while (cur && cur != *this); 
		} 
 
		assert(walker._Depth == -1); 
 
		Node arg_end = *this; 
		return walker.end(arg_end); 
	} 
 
	size_t Node::HashValue() const 
	{ 
		return static_cast<size_t>(reinterpret_cast<uintptr_t>(_GetRoot) / sizeof(NodeStruct)); 
	} 
 
	NodeStruct* Node::InternalObject() const 
	{ 
		return _GetRoot; 
	} 
 
	void Node::Print(Writer& WriterInstance, const char_t* indent, unsigned int flags, Encoding DocumentEncoding, unsigned int Depth) const 
	{ 
		if (!_GetRoot) return; 
 
		BufferedWriter buffered_WriterInstance(WriterInstance, DocumentEncoding); 
 
		NodeOutput(buffered_WriterInstance, *this, indent, flags, Depth); 
	} 
 
#ifndef XML_NO_STL 
	void Node::Print(std::basic_ostream<char, std::char_traits<char> >& stream, const char_t* indent, unsigned int flags, Encoding DocumentEncoding, unsigned int Depth) const 
	{ 
		WriterStream WriterInstance(stream); 
 
		Print(WriterInstance, indent, flags, DocumentEncoding, Depth); 
	} 
 
	void Node::Print(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& stream, const char_t* indent, unsigned int flags, unsigned int Depth) const 
	{ 
		WriterStream WriterInstance(stream); 
 
		Print(WriterInstance, indent, flags, Encodingwchar_t, Depth); 
	} 
#endif 
 
	ptrdiff_t Node::OffSetDebug() const 
	{ 
		NodeStruct* r = GetRoot()._GetRoot; 
 
		if (!r) return -1; 
 
		const char_t* buffer = static_cast<DocumentStruct*>(r)->buffer; 
 
		if (!buffer) return -1; 
 
		switch (Type()) 
		{ 
		case NodeDocument: 
			return 0; 
 
		case NodeElement: 
		case NodeDeclaration: 
		case NodePi: 
			return (_GetRoot->header & MemoryPage_Name_allocated_mask) ? -1 : _GetRoot->Name - buffer; 
 
		case NodePcdata: 
		case NodeCdata: 
		case NodeComment: 
		case NodeDocType: 
			return (_GetRoot->header & MemoryPage_Value_allocated_mask) ? -1 : _GetRoot->Value - buffer; 
 
		default: 
			return -1; 
		} 
	} 
 
#ifdef __BORLANDC__ 
	bool operator&&(const Node& lhs, bool rhs) 
	{ 
		return (bool)lhs && rhs; 
	} 
 
	bool operator||(const Node& lhs, bool rhs) 
	{ 
		return (bool)lhs || rhs; 
	} 
#endif 
 
	NodeIterator::NodeIterator() 
	{ 
	} 
 
	NodeIterator::NodeIterator(const Node& node): _wrap(node), _GetParent(node.GetParent()) 
	{ 
	} 
 
	NodeIterator::NodeIterator(NodeStruct* ref, NodeStruct* GetParent): _wrap(ref), _GetParent(GetParent) 
	{ 
	} 
 
	bool NodeIterator::operator==(const NodeIterator& rhs) const 
	{ 
		return _wrap._GetRoot == rhs._wrap._GetRoot && _GetParent._GetRoot == rhs._GetParent._GetRoot; 
	} 
	 
	bool NodeIterator::operator!=(const NodeIterator& rhs) const 
	{ 
		return _wrap._GetRoot != rhs._wrap._GetRoot || _GetParent._GetRoot != rhs._GetParent._GetRoot; 
	} 
 
	Node& NodeIterator::operator*() 
	{ 
		assert(_wrap._GetRoot); 
		return _wrap; 
	} 
 
	Node* NodeIterator::operator->() 
	{ 
		assert(_wrap._GetRoot); 
		return &_wrap; 
	} 
 
	const NodeIterator& NodeIterator::operator++() 
	{ 
		assert(_wrap._GetRoot); 
		_wrap._GetRoot = _wrap._GetRoot->GetNextSibling; 
		return *this; 
	} 
 
	NodeIterator NodeIterator::operator++(int) 
	{ 
		NodeIterator temp = *this; 
		++*this; 
		return temp; 
	} 
 
	const NodeIterator& NodeIterator::operator--() 
	{ 
		_wrap = _wrap._GetRoot ? _wrap.GetPreviousSibling() : _GetParent.GetLastChild(); 
		return *this; 
	} 
 
	NodeIterator NodeIterator::operator--(int) 
	{ 
		NodeIterator temp = *this; 
		--*this; 
		return temp; 
	} 
 
	AttributeIterator::AttributeIterator() 
	{ 
	} 
 
	AttributeIterator::AttributeIterator(const Attribute& attr, const Node& GetParent): _wrap(attr), _GetParent(GetParent) 
	{ 
	} 
 
	AttributeIterator::AttributeIterator(AttributeStruct* ref, NodeStruct* GetParent): _wrap(ref), _GetParent(GetParent) 
	{ 
	} 
 
	bool AttributeIterator::operator==(const AttributeIterator& rhs) const 
	{ 
		return _wrap._attr == rhs._wrap._attr && _GetParent._GetRoot == rhs._GetParent._GetRoot; 
	} 
	 
	bool AttributeIterator::operator!=(const AttributeIterator& rhs) const 
	{ 
		return _wrap._attr != rhs._wrap._attr || _GetParent._GetRoot != rhs._GetParent._GetRoot; 
	} 
 
	Attribute& AttributeIterator::operator*() 
	{ 
		assert(_wrap._attr); 
		return _wrap; 
	} 
 
	Attribute* AttributeIterator::operator->() 
	{ 
		assert(_wrap._attr); 
		return &_wrap; 
	} 
 
	const AttributeIterator& AttributeIterator::operator++() 
	{ 
		assert(_wrap._attr); 
		_wrap._attr = _wrap._attr->GetNextAttribute; 
		return *this; 
	} 
 
	AttributeIterator AttributeIterator::operator++(int) 
	{ 
		AttributeIterator temp = *this; 
		++*this; 
		return temp; 
	} 
 
	const AttributeIterator& AttributeIterator::operator--() 
	{ 
		_wrap = _wrap._attr ? _wrap.GetPreviousAttribute() : _GetParent.GetLastAttribute(); 
		return *this; 
	} 
 
	AttributeIterator AttributeIterator::operator--(int) 
	{ 
		AttributeIterator temp = *this; 
		--*this; 
		return temp; 
	} 
 
	ParseResult::ParseResult(): Status(StatusInternalError), Offset(0), DocumentEncoding(EncodingAuto) 
	{ 
	} 
 
	ParseResult::operator bool() const 
	{ 
		return Status == StatusOk; 
	} 
 
	const char* ParseResult::Description() const 
	{ 
		switch (Status) 
		{ 
		case StatusOk: return "No error"; 
 
		case StatusFileNotFound: return "File was not found"; 
		case StatusIOError: return "Error reading from file/stream"; 
		case StatusOutOfMemory: return "Could not allocate memory"; 
		case StatusInternalError: return "Internal error occurred"; 
 
		case StatusUnrecognizedTag: return "Could not determine tag Type"; 
 
		case StatusBadPi: return "Error parsing document declaration/processing instruction"; 
		case StatusBadComment: return "Error parsing comment"; 
		case StatusBadCdata: return "Error parsing CDATA section"; 
		case StatusBadDocType: return "Error parsing document Type declaration"; 
		case StatusBadPcdata: return "Error parsing PCDATA section"; 
		case StatusBadStartElement: return "Error parsing start element tag"; 
		case StatusBadAttribute: return "Error parsing element GetAttribute"; 
		case StatusBadEndElement: return "Error parsing end element tag"; 
		case StatusEndElementMismatch: return "Start-end tags mismatch"; 
 
		default: return "Unknown error"; 
		} 
	} 
 
	Document::Document(): _buffer(0) 
	{ 
		create(); 
	} 
 
	Document::~Document() 
	{ 
		destroy(); 
	} 
 
	void Document::ReSet() 
	{ 
		destroy(); 
		create(); 
	} 
 
	void Document::ReSet(const Document& proto) 
	{ 
		ReSet(); 
 
		for (Node cur = proto.GetFirstChild(); cur; cur = cur.GetNextSibling()) 
			AppendCopy(cur); 
	} 
 
	void Document::create() 
	{ 
		// initialize sentinel page 
		STATIC_ASSERT(offsetof(MemoryPage, data) + sizeof(DocumentStruct) + MemoryPage_alignment <= sizeof(_memory)); 
 
		// align upwards to page boundary 
		void* page_memory = reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(_memory) + (MemoryPage_alignment - 1)) & ~(MemoryPage_alignment - 1)); 
 
		// prepare page structure 
		MemoryPage* page = MemoryPage::construct(page_memory); 
 
		page->busy_size = MemoryPage_size; 
 
		// allocate new GetRoot 
		_GetRoot = new (page->data) DocumentStruct(page); 
		_GetRoot->prev_sibling_c = _GetRoot; 
 
		// setup sentinel page 
		page->allocator = static_cast<DocumentStruct*>(_GetRoot); 
	} 
 
	void Document::destroy() 
	{ 
		// destroy static storage 
		if (_buffer) 
		{ 
			global_deallocate(_buffer); 
			_buffer = 0; 
		} 
 
		// destroy dynamic storage, leave sentinel page (it's in static memory) 
		if (_GetRoot) 
		{ 
			MemoryPage* GetRoot_page = reinterpret_cast<MemoryPage*>(_GetRoot->header & MemoryPage_pointer_mask); 
			assert(GetRoot_page && !GetRoot_page->prev && !GetRoot_page->memory); 
 
			// destroy all pages 
			for (MemoryPage* page = GetRoot_page->next; page; ) 
			{ 
				MemoryPage* next = page->next; 
 
				Allocator::deallocate_page(page); 
 
				page = next; 
			} 
 
			// cleanup GetRoot page 
			GetRoot_page->allocator = 0; 
			GetRoot_page->next = 0; 
			GetRoot_page->busy_size = GetRoot_page->freed_size = 0; 
 
			_GetRoot = 0; 
		} 
	} 
 
#ifndef XML_NO_STL 
	ParseResult Document::Load(std::basic_istream<char, std::char_traits<char> >& stream, unsigned int options, Encoding DocumentEncoding) 
	{ 
		ReSet(); 
 
		return LoadStreamImpl(*this, stream, options, DocumentEncoding); 
	} 
 
	ParseResult Document::Load(std::basic_istream<wchar_t, std::char_traits<wchar_t> >& stream, unsigned int options) 
	{ 
		ReSet(); 
 
		return LoadStreamImpl(*this, stream, options, Encodingwchar_t); 
	} 
#endif 
 
	ParseResult Document::Load(const char_t* contents, unsigned int options) 
	{ 
		// Force native DocumentEncoding (skip autodetection) 
	#ifdef XML_WCHAR_MODE 
		Encoding DocumentEncoding = Encodingwchar_t; 
	#else 
		Encoding DocumentEncoding = EncodingUTF8; 
	#endif 
 
		return LoadBuffer(contents, strlength(contents) * sizeof(char_t), options, DocumentEncoding); 
	} 
 
	ParseResult Document::LoadFile(const char* Path, unsigned int options, Encoding DocumentEncoding) 
	{ 
		ReSet(); 
 
		FILE* file = fopen(Path, "rb"); 
 
		return LoadFileImpl(*this, file, options, DocumentEncoding); 
	} 
 
	ParseResult Document::LoadFile(const wchar_t* Path, unsigned int options, Encoding DocumentEncoding) 
	{ 
		ReSet(); 
 
		FILE* file = open_file_wide(Path, L"rb"); 
 
		return LoadFileImpl(*this, file, options, DocumentEncoding); 
	} 
 
	ParseResult Document::LoadBufferImpl(void* contents, size_t size, unsigned int options, Encoding DocumentEncoding, bool is_mutable, bool own) 
	{ 
		ReSet(); 
 
		// check input buffer 
		assert(contents || size == 0); 
 
		// get actual DocumentEncoding 
		Encoding buffer_DocumentEncoding = GetBuffer_DocumentEncoding(DocumentEncoding, contents, size); 
 
		// get private buffer 
		char_t* buffer = 0; 
		size_t length = 0; 
 
		if (!convert_buffer(buffer, length, buffer_DocumentEncoding, contents, size, is_mutable)) return make_ParseResult(StatusOutOfMemory); 
		 
		// delete original buffer if we performed a conversion 
		if (own && buffer != contents && contents) global_deallocate(contents); 
 
		// parse 
		ParseResult res = Parser::parse(buffer, length, _GetRoot, options); 
 
		// remember DocumentEncoding 
		res.DocumentEncoding = buffer_DocumentEncoding; 
 
		// grab onto buffer if it's our buffer, user is responsible for deallocating contens himself 
		if (own || buffer != contents) _buffer = buffer; 
 
		return res; 
	} 
 
	ParseResult Document::LoadBuffer(const void* contents, size_t size, unsigned int options, Encoding DocumentEncoding) 
	{ 
		return LoadBufferImpl(const_cast<void*>(contents), size, options, DocumentEncoding, false, false); 
	} 
 
	ParseResult Document::LoadBufferInplace(void* contents, size_t size, unsigned int options, Encoding DocumentEncoding) 
	{ 
		return LoadBufferImpl(contents, size, options, DocumentEncoding, true, false); 
	} 
		 
	ParseResult Document::LoadBufferInplaceOwn(void* contents, size_t size, unsigned int options, Encoding DocumentEncoding) 
	{ 
		return LoadBufferImpl(contents, size, options, DocumentEncoding, true, true); 
	} 
 
	void Document::Save(Writer& WriterInstance, const char_t* indent, unsigned int flags, Encoding DocumentEncoding) const 
	{ 
		if (flags & FormatWriteBom) Write_bom(WriterInstance, GetWrite_DocumentEncoding(DocumentEncoding)); 
 
		BufferedWriter buffered_WriterInstance(WriterInstance, DocumentEncoding); 
 
		if (!(flags & FormatNoDeclaration) && !hAsDeclaration(*this)) 
		{ 
			buffered_WriterInstance.Write(XML_TEXT("<?xml version=\"1.0\"?>")); 
			if (!(flags & FormatRaw)) buffered_WriterInstance.Write('\n'); 
		} 
 
		NodeOutput(buffered_WriterInstance, *this, indent, flags, 0); 
	} 
 
#ifndef XML_NO_STL 
	void Document::Save(std::basic_ostream<char, std::char_traits<char> >& stream, const char_t* indent, unsigned int flags, Encoding DocumentEncoding) const 
	{ 
		WriterStream WriterInstance(stream); 
 
		Save(WriterInstance, indent, flags, DocumentEncoding); 
	} 
 
	void Document::Save(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& stream, const char_t* indent, unsigned int flags) const 
	{ 
		WriterStream WriterInstance(stream); 
 
		Save(WriterInstance, indent, flags, Encodingwchar_t); 
	} 
#endif 
 
	bool Document::SaveFile(const char* Path, const char_t* indent, unsigned int flags, Encoding DocumentEncoding) const 
	{ 
		FILE* file = fopen(Path, "wb"); 
		if (!file) return false; 
 
		WriterFile WriterInstance(file); 
		Save(WriterInstance, indent, flags, DocumentEncoding); 
 
		fclose(file); 
 
		return true; 
	} 
 
	bool Document::SaveFile(const wchar_t* Path, const char_t* indent, unsigned int flags, Encoding DocumentEncoding) const 
	{ 
		FILE* file = open_file_wide(Path, L"wb"); 
		if (!file) return false; 
 
		WriterFile WriterInstance(file); 
		Save(WriterInstance, indent, flags, DocumentEncoding); 
 
		fclose(file); 
 
		return true; 
	} 
 
	Node Document::DocumentElement() const 
	{ 
		for (NodeStruct* i = _GetRoot->GetFirstChild; i; i = i->GetNextSibling) 
			if ((i->header & MemoryPage_type_mask) + 1 == NodeElement) 
				return Node(i); 
 
		return Node(); 
	} 
 
#ifndef XML_NO_STL 
	std::string PHYS_LIB AsUtf8(const wchar_t* str) 
	{ 
		assert(str); 
 
		return AsUtf8_impl(str, wcslen(str)); 
	} 
 
	std::string PHYS_LIB AsUtf8(const std::wstring& str) 
	{ 
		return AsUtf8_impl(str.c_str(), str.size()); 
	} 
	 
	std::wstring PHYS_LIB AsWide(const char* str) 
	{ 
		assert(str); 
 
		return AsWide_impl(str, strlen(str)); 
	} 
	 
	std::wstring PHYS_LIB AsWide(const std::string& str) 
	{ 
		return AsWide_impl(str.c_str(), str.size()); 
	} 
#endif 
 
	void PHYS_LIB SetMemory_management_functions(allocation_function allocate, deallocation_function deallocate) 
	{ 
		global_allocate = allocate; 
		global_deallocate = deallocate; 
	} 
 
	allocation_function PHYS_LIB GetMemory_allocation_function() 
	{ 
		return global_allocate; 
	} 
 
	deallocation_function PHYS_LIB GetMemory_deallocation_function() 
	{ 
		return global_deallocate; 
	} 
} 
} // \phys
 
#if !defined(XML_NO_STL) && (defined(_MSC_VER) || defined(__ICC)) 
namespace std 
{ 
	// Workarounds for (non-standard) iterator category detection for older versions (MSVC7/IC8 and earlier) 
	std::bidirectional_iterator_tag _Iter_cat(const NodeIterator&) 
	{ 
		return std::bidirectional_iterator_tag(); 
	} 
 
	std::bidirectional_iterator_tag _Iter_cat(const AttributeIterator&) 
	{ 
		return std::bidirectional_iterator_tag(); 
	} 
} 
#endif 
 
#if !defined(XML_NO_STL) && defined(__SUNPRO_CC) 
namespace std 
{ 
	// Workarounds for (non-standard) iterator category detection 
	std::bidirectional_iterator_tag __iterator_category(const NodeIterator&) 
	{ 
		return std::bidirectional_iterator_tag(); 
	} 
 
	std::bidirectional_iterator_tag __iterator_category(const AttributeIterator&) 
	{ 
		return std::bidirectional_iterator_tag(); 
	} 
} 
#endif 
 
#ifndef XML_NO_XPATH 
 
// STL replacements 
namespace 
{ 
	struct equal_to 
	{ 
		template <typename T> bool operator()(const T& lhs, const T& rhs) const 
		{ 
			return lhs == rhs; 
		} 
	}; 
 
	struct not_equal_to 
	{ 
		template <typename T> bool operator()(const T& lhs, const T& rhs) const 
		{ 
			return lhs != rhs; 
		} 
	}; 
 
	struct less 
	{ 
		template <typename T> bool operator()(const T& lhs, const T& rhs) const 
		{ 
			return lhs < rhs; 
		} 
	}; 
 
	struct less_equal 
	{ 
		template <typename T> bool operator()(const T& lhs, const T& rhs) const 
		{ 
			return lhs <= rhs; 
		} 
	}; 
 
	template <typename T> void swap(T& lhs, T& rhs) 
	{ 
		T temp = lhs; 
		lhs = rhs; 
		rhs = temp; 
	} 
 
	template <typename I, typename Pred> I min_element(I begin, I end, const Pred& pred) 
	{ 
		I result = begin; 
 
		for (I it = begin + 1; it != end; ++it) 
			if (pred(*it, *result)) 
				result = it; 
 
		return result; 
	} 
 
	template <typename I> void reverse(I begin, I end) 
	{ 
		while (begin + 1 < end) swap(*begin++, *--end); 
	} 
 
	template <typename I> I unique(I begin, I end) 
	{ 
		// fast skip head 
		while (begin + 1 < end && *begin != *(begin + 1)) begin++; 
 
		if (begin == end) return begin; 
 
		// last written element 
		I Write = begin++;  
 
		// merge unique elements 
		while (begin != end) 
		{ 
			if (*begin != *Write) 
				*++Write = *begin++; 
			else 
				begin++; 
		} 
 
		// past-the-end (Write points to live element) 
		return Write + 1; 
	} 
 
	template <typename I> void copy_backwards(I begin, I end, I target) 
	{ 
		while (begin != end) *--target = *--end; 
	} 
 
	template <typename I, typename Pred, typename T> void insertion_sort(I begin, I end, const Pred& pred, T*) 
	{ 
		assert(begin != end); 
 
		for (I it = begin + 1; it != end; ++it) 
		{ 
			T val = *it; 
 
			if (pred(val, *begin)) 
			{ 
				// move to front 
				copy_backwards(begin, it, it + 1); 
				*begin = val; 
			} 
			else 
			{ 
				I hole = it; 
 
				// move hole backwards 
				while (pred(val, *(hole - 1))) 
				{ 
					*hole = *(hole - 1); 
					hole--; 
				} 
 
				// fill hole with element 
				*hole = val; 
			} 
		} 
	} 
 
	// std variant for elements with == 
	template <typename I, typename Pred> void partition(I begin, I middle, I end, const Pred& pred, I* out_eqbeg, I* out_eqend) 
	{ 
		I eqbeg = middle, eqend = middle + 1; 
 
		// expand equal range 
		while (eqbeg != begin && *(eqbeg - 1) == *eqbeg) --eqbeg; 
		while (eqend != end && *eqend == *eqbeg) ++eqend; 
 
		// process outer elements 
		I ltend = eqbeg, gtbeg = eqend; 
 
		for (;;) 
		{ 
			// find the element from the right side that belongs to the left one 
			for (; gtbeg != end; ++gtbeg) 
				if (!pred(*eqbeg, *gtbeg)) 
				{ 
					if (*gtbeg == *eqbeg) swap(*gtbeg, *eqend++); 
					else break; 
				} 
 
			// find the element from the left side that belongs to the right one 
			for (; ltend != begin; --ltend) 
				if (!pred(*(ltend - 1), *eqbeg)) 
				{ 
					if (*eqbeg == *(ltend - 1)) swap(*(ltend - 1), *--eqbeg); 
					else break; 
				} 
 
			// scanned all elements 
			if (gtbeg == end && ltend == begin) 
			{ 
				*out_eqbeg = eqbeg; 
				*out_eqend = eqend; 
				return; 
			} 
 
			// make room for elements by moving equal area 
			if (gtbeg == end) 
			{ 
				if (--ltend != --eqbeg) swap(*ltend, *eqbeg); 
				swap(*eqbeg, *--eqend); 
			} 
			else if (ltend == begin) 
			{ 
				if (eqend != gtbeg) swap(*eqbeg, *eqend); 
				++eqend; 
				swap(*gtbeg++, *eqbeg++); 
			} 
			else swap(*gtbeg++, *--ltend); 
		} 
	} 
 
	template <typename I, typename Pred> void median3(I first, I middle, I last, const Pred& pred) 
	{ 
		if (pred(*middle, *first)) swap(*middle, *first); 
		if (pred(*last, *middle)) swap(*last, *middle); 
		if (pred(*middle, *first)) swap(*middle, *first); 
	} 
 
	template <typename I, typename Pred> void median(I first, I middle, I last, const Pred& pred) 
	{ 
		if (last - first <= 40) 
		{ 
			// median of three for small chunks 
			median3(first, middle, last, pred); 
		} 
		else 
		{ 
			// median of nine 
			size_t step = (last - first + 1) / 8; 
 
			median3(first, first + step, first + 2 * step, pred); 
			median3(middle - step, middle, middle + step, pred); 
			median3(last - 2 * step, last - step, last, pred); 
			median3(first + step, middle, last - step, pred); 
		} 
	} 
 
	template <typename I, typename Pred> void sort(I begin, I end, const Pred& pred) 
	{ 
		// sort large chunks 
		while (end - begin > 32) 
		{ 
			// find median element 
			I middle = begin + (end - begin) / 2; 
			median(begin, middle, end - 1, pred); 
 
			// partition in three chunks (< = >) 
			I eqbeg, eqend; 
			partition(begin, middle, end, pred, &eqbeg, &eqend); 
 
			// loop on larger half 
			if (eqbeg - begin > end - eqend) 
			{ 
				sort(eqend, end, pred); 
				end = eqbeg; 
			} 
			else 
			{ 
				sort(begin, eqbeg, pred); 
				begin = eqend; 
			} 
		} 
 
		// insertion sort small chunk 
		if (begin != end) insertion_sort(begin, end, pred, &*begin); 
	} 
} 
 
// Allocator used for AST and evaluation stacks 
namespace 
{ 
	struct XPathMemoryBlock 
	{	 
		XPathMemoryBlock* next; 
 
		char data[4096]; 
	}; 
		 
	class XPathAllocator 
	{ 
		XPathMemoryBlock* _GetRoot; 
		size_t _GetRoot_size; 
 
	public: 
	#ifdef XML_NO_EXCEPTIONS 
		jmp_buf* error_handler; 
	#endif 
 
		XPathAllocator(XPathMemoryBlock* GetRoot, size_t GetRoot_size = 0): _GetRoot(GetRoot), _GetRoot_size(GetRoot_size) 
		{ 
		#ifdef XML_NO_EXCEPTIONS 
			error_handler = 0; 
		#endif 
		} 
		 
		void* allocate_nothrow(size_t size) 
		{ 
			const size_t block_capacity = sizeof(_GetRoot->data); 
 
			// align size so that we're able to store pointers in subsequent blocks 
			size = (size + sizeof(void*) - 1) & ~(sizeof(void*) - 1); 
 
			if (_GetRoot_size + size <= block_capacity) 
			{ 
				void* buf = _GetRoot->data + _GetRoot_size; 
				_GetRoot_size += size; 
				return buf; 
			} 
			else 
			{ 
				size_t block_data_size = (size > block_capacity) ? size : block_capacity; 
				size_t block_size = block_data_size + offsetof(XPathMemoryBlock, data); 
 
				XPathMemoryBlock* block = static_cast<XPathMemoryBlock*>(global_allocate(block_size)); 
				if (!block) return 0; 
				 
				block->next = _GetRoot; 
				 
				_GetRoot = block; 
				_GetRoot_size = size; 
				 
				return block->data; 
			} 
		} 
 
		void* allocate(size_t size) 
		{ 
			void* result = allocate_nothrow(size); 
 
			if (!result) 
			{ 
			#ifdef XML_NO_EXCEPTIONS 
				assert(error_handler); 
				longjmp(*error_handler, 1); 
			#else 
				throw std::bad_alloc(); 
			#endif 
			} 
 
			return result; 
		} 
 
		void* reallocate(void* ptr, size_t old_size, size_t new_size) 
		{ 
			// align size so that we're able to store pointers in subsequent blocks 
			old_size = (old_size + sizeof(void*) - 1) & ~(sizeof(void*) - 1); 
			new_size = (new_size + sizeof(void*) - 1) & ~(sizeof(void*) - 1); 
 
			// we can only reallocate the last object 
			assert(ptr == 0 || static_cast<char*>(ptr) + old_size == _GetRoot->data + _GetRoot_size); 
 
			// adjust GetRoot size so that we have not allocated the object at all 
			bool only_object = (_GetRoot_size == old_size); 
 
			if (ptr) _GetRoot_size -= old_size; 
 
			// allocate a new version (this will obviously reuse the memory if possible) 
			void* result = allocate(new_size); 
			assert(result); 
 
			// we have a new block 
			if (result != ptr && ptr) 
			{ 
				// copy old data 
				assert(new_size > old_size); 
				memcpy(result, ptr, old_size); 
 
				// free the previous page if it had no other objects 
				if (only_object) 
				{ 
					assert(_GetRoot->data == result); 
					assert(_GetRoot->next); 
 
					XPathMemoryBlock* next = _GetRoot->next->next; 
 
					if (next) 
					{ 
						// deallocate the whole page, unless it was the first one 
						global_deallocate(_GetRoot->next); 
						_GetRoot->next = next; 
					} 
				} 
			} 
 
			return result; 
		} 
 
		void revert(const XPathAllocator& state) 
		{ 
			// free all new pages 
			XPathMemoryBlock* cur = _GetRoot; 
 
			while (cur != state._GetRoot) 
			{ 
				XPathMemoryBlock* next = cur->next; 
 
				global_deallocate(cur); 
 
				cur = next; 
			} 
 
			// restore state 
			_GetRoot = state._GetRoot; 
			_GetRoot_size = state._GetRoot_size; 
		} 
 
		void release() 
		{ 
			XPathMemoryBlock* cur = _GetRoot; 
			assert(cur); 
 
			while (cur->next) 
			{ 
				XPathMemoryBlock* next = cur->next; 
 
				global_deallocate(cur); 
 
				cur = next; 
			} 
		} 
	}; 
 
	struct XPathAllocatorCapture 
	{ 
		XPathAllocatorCapture(XPathAllocator* alloc): _target(alloc), _state(*alloc) 
		{ 
		} 
 
		~XPathAllocatorCapture() 
		{ 
			_target->revert(_state); 
		} 
 
		XPathAllocator* _target; 
		XPathAllocator _state; 
	}; 
 
	struct XPathStack 
	{ 
		XPathAllocator* result; 
		XPathAllocator* temp; 
	}; 
 
	struct XPathStackData 
	{ 
		XPathMemoryBlock blocks[2]; 
		XPathAllocator result; 
		XPathAllocator temp; 
		XPathStack stack; 
 
	#ifdef XML_NO_EXCEPTIONS 
		jmp_buf error_handler; 
	#endif 
 
		XPathStackData(): result(blocks + 0), temp(blocks + 1) 
		{ 
			blocks[0].next = blocks[1].next = 0; 
 
			stack.result = &result; 
			stack.temp = &temp; 
 
		#ifdef XML_NO_EXCEPTIONS 
			result.error_handler = temp.error_handler = &error_handler; 
		#endif 
		} 
 
		~XPathStackData() 
		{ 
			result.release(); 
			temp.release(); 
		} 
	}; 
} 
 
// String class 
namespace 
{ 
	class XPathString 
	{ 
		const char_t* _buffer; 
		bool _uses_heap; 
 
		static char_t* duplicate_string(const char_t* string, size_t length, XPathAllocator* alloc) 
		{ 
			char_t* result = static_cast<char_t*>(alloc->allocate((length + 1) * sizeof(char_t))); 
			assert(result); 
 
			memcpy(result, string, length * sizeof(char_t)); 
			result[length] = 0; 
 
			return result; 
		} 
 
		static char_t* duplicate_string(const char_t* string, XPathAllocator* alloc) 
		{ 
			return duplicate_string(string, strlength(string), alloc); 
		} 
 
	public: 
		XPathString(): _buffer(XML_TEXT("")), _uses_heap(false) 
		{ 
		} 
 
		explicit XPathString(const char_t* str, XPathAllocator* alloc) 
		{ 
			bool empty = (*str == 0); 
 
			_buffer = empty ? XML_TEXT("") : duplicate_string(str, alloc); 
			_uses_heap = !empty; 
		} 
 
		explicit XPathString(const char_t* str, bool use_heap): _buffer(str), _uses_heap(use_heap) 
		{ 
		} 
 
		XPathString(const char_t* begin, const char_t* end, XPathAllocator* alloc) 
		{ 
			assert(begin <= end); 
 
			bool empty = (begin == end); 
 
			_buffer = empty ? XML_TEXT("") : duplicate_string(begin, static_cast<size_t>(end - begin), alloc); 
			_uses_heap = !empty; 
		} 
 
		void append(const XPathString& o, XPathAllocator* alloc) 
		{ 
			// skip empty sources 
			if (!*o._buffer) return; 
 
			// fast append for constant empty target and constant source 
			if (!*_buffer && !_uses_heap && !o._uses_heap) 
			{ 
				_buffer = o._buffer; 
			} 
			else 
			{ 
				// need to make heap copy 
				size_t tarGetLength = strlength(_buffer); 
				size_t source_length = strlength(o._buffer); 
				size_t length = tarGetLength + source_length; 
 
				// allocate new buffer 
				char_t* result = static_cast<char_t*>(alloc->reallocate(_uses_heap ? const_cast<char_t*>(_buffer) : 0, (tarGetLength + 1) * sizeof(char_t), (length + 1) * sizeof(char_t))); 
				assert(result); 
 
				// append first string to the new buffer in case there was no reallocation 
				if (!_uses_heap) memcpy(result, _buffer, tarGetLength * sizeof(char_t)); 
 
				// append second string to the new buffer 
				memcpy(result + tarGetLength, o._buffer, source_length * sizeof(char_t)); 
				result[length] = 0; 
 
				// finalize 
				_buffer = result; 
				_uses_heap = true; 
			} 
		} 
 
		const char_t* c_str() const 
		{ 
			return _buffer; 
		} 
 
		size_t length() const 
		{ 
			return strlength(_buffer); 
		} 
		 
		char_t* data(XPathAllocator* alloc) 
		{ 
			// make private heap copy 
			if (!_uses_heap) 
			{ 
				_buffer = duplicate_string(_buffer, alloc); 
				_uses_heap = true; 
			} 
 
			return const_cast<char_t*>(_buffer); 
		} 
 
		bool Empty() const 
		{ 
			return *_buffer == 0; 
		} 
 
		bool operator==(const XPathString& o) const 
		{ 
			return strequal(_buffer, o._buffer); 
		} 
 
		bool operator!=(const XPathString& o) const 
		{ 
			return !strequal(_buffer, o._buffer); 
		} 
 
		bool uses_heap() const 
		{ 
			return _uses_heap; 
		} 
	}; 
 
	XPathString XPathStringConst(const char_t* str) 
	{ 
		return XPathString(str, false); 
	} 
} 
 
namespace 
{ 
	bool starts_with(const char_t* string, const char_t* pattern) 
	{ 
		while (*pattern && *string == *pattern) 
		{ 
			string++; 
			pattern++; 
		} 
 
		return *pattern == 0; 
	} 
 
	const char_t* FindChar(const char_t* s, char_t c) 
	{ 
	#ifdef XML_WCHAR_MODE 
		return wcschr(s, c); 
	#else 
		return strchr(s, c); 
	#endif 
	} 
 
	const char_t* FindSubstring(const char_t* s, const char_t* p) 
	{ 
	#ifdef XML_WCHAR_MODE 
		// MSVC6 wcsstr bug workaround (if s is empty it always returns 0) 
		return (*p == 0) ? s : wcsstr(s, p); 
	#else 
		return strstr(s, p); 
	#endif 
	} 
 
	// Converts symbol to lower case, if it is an ASCII one 
	char_t tolower_ascii(char_t ch) 
	{ 
		return static_cast<unsigned int>(ch - 'A') < 26 ? static_cast<char_t>(ch | ' ') : ch; 
	} 
 
	XPathString string_Value(const XPathNode& na, XPathAllocator* alloc) 
	{ 
		if (na.GetAttribute()) 
			return XPathStringConst(na.GetAttribute().Value()); 
		else 
		{ 
			const Node& n = na.node(); 
 
			switch (n.Type()) 
			{ 
			case NodePcdata: 
			case NodeCdata: 
			case NodeComment: 
			case NodePi: 
				return XPathStringConst(n.Value()); 
			 
			case NodeDocument: 
			case NodeElement: 
			{ 
				XPathString result; 
 
				Node cur = n.GetFirstChild(); 
				 
				while (cur && cur != n) 
				{ 
					if (cur.Type() == NodePcdata || cur.Type() == NodeCdata) 
						result.append(XPathStringConst(cur.Value()), alloc); 
 
					if (cur.GetFirstChild()) 
						cur = cur.GetFirstChild(); 
					else if (cur.GetNextSibling()) 
						cur = cur.GetNextSibling(); 
					else 
					{ 
						while (!cur.GetNextSibling() && cur != n) 
							cur = cur.GetParent(); 
 
						if (cur != n) cur = cur.GetNextSibling(); 
					} 
				} 
				 
				return result; 
			} 
			 
			default: 
				return XPathString(); 
			} 
		} 
	} 
	 
	unsigned int NodeHeight(Node n) 
	{ 
		unsigned int result = 0; 
		 
		while (n) 
		{ 
			++result; 
			n = n.GetParent(); 
		} 
		 
		return result; 
	} 
	 
	bool NodeIs_before(Node ln, unsigned int lh, Node rn, unsigned int rh) 
	{ 
		// normalize heights 
		for (unsigned int i = rh; i < lh; i++) ln = ln.GetParent(); 
		for (unsigned int j = lh; j < rh; j++) rn = rn.GetParent(); 
		 
		// one node is the ancestor of the other 
		if (ln == rn) return lh < rh; 
		 
		// find common ancestor 
		while (ln.GetParent() != rn.GetParent()) 
		{ 
			ln = ln.GetParent(); 
			rn = rn.GetParent(); 
		} 
 
		// there is no common ancestor (the shared GetParent is null), nodes are from different documents 
		if (!ln.GetParent()) return ln < rn; 
 
		// determine sibling order 
		for (; ln; ln = ln.GetNextSibling()) 
			if (ln == rn) 
				return true; 
				 
		return false; 
	} 
 
	bool NodeIs_ancestor(Node GetParent, Node node) 
	{ 
		while (node && node != GetParent) node = node.GetParent(); 
 
		return GetParent && node == GetParent; 
	} 
 
	const void* document_order(const XPathNode& xnode) 
	{ 
		NodeStruct* node = xnode.node().InternalObject(); 
 
		if (node) 
		{ 
			if (node->Name && (node->header & MemoryPage_Name_allocated_mask) == 0) return node->Name; 
			if (node->Value && (node->header & MemoryPage_Value_allocated_mask) == 0) return node->Value; 
			return 0; 
		} 
 
		AttributeStruct* attr = xnode.GetAttribute().InternalObject(); 
 
		if (attr) 
		{ 
			if ((attr->header & MemoryPage_Name_allocated_mask) == 0) return attr->Name; 
			if ((attr->header & MemoryPage_Value_allocated_mask) == 0) return attr->Value; 
			return 0; 
		} 
 
		return 0; 
	} 
	 
	struct document_order_comparator 
	{ 
		bool operator()(const XPathNode& lhs, const XPathNode& rhs) const 
		{ 
			// optimized document order based check 
			const void* lo = document_order(lhs); 
			const void* ro = document_order(rhs); 
 
			if (lo && ro) return lo < ro; 
 
			// slow comparison 
			Node ln = lhs.node(), rn = rhs.node(); 
 
			// compare attributes 
			if (lhs.GetAttribute() && rhs.GetAttribute()) 
			{ 
				// shared GetParent 
				if (lhs.GetParent() == rhs.GetParent()) 
				{ 
					// determine sibling order 
					for (Attribute a = lhs.GetAttribute(); a; a = a.GetNextAttribute()) 
						if (a == rhs.GetAttribute()) 
							return true; 
					 
					return false; 
				} 
				 
				// compare GetAttribute GetParents 
				ln = lhs.GetParent(); 
				rn = rhs.GetParent(); 
			} 
			else if (lhs.GetAttribute()) 
			{ 
				// attributes go after the GetParent element 
				if (lhs.GetParent() == rhs.node()) return false; 
				 
				ln = lhs.GetParent(); 
			} 
			else if (rhs.GetAttribute()) 
			{ 
				// attributes go after the GetParent element 
				if (rhs.GetParent() == lhs.node()) return true; 
				 
				rn = rhs.GetParent(); 
			} 
 
			if (ln == rn) return false; 
			 
			unsigned int lh = NodeHeight(ln); 
			unsigned int rh = NodeHeight(rn); 
			 
			return NodeIs_before(ln, lh, rn, rh); 
		} 
	}; 
 
	struct duplicate_comparator 
	{ 
		bool operator()(const XPathNode& lhs, const XPathNode& rhs) const 
		{ 
			if (lhs.GetAttribute()) return rhs.GetAttribute() ? lhs.GetAttribute() < rhs.GetAttribute() : true; 
			else return rhs.GetAttribute() ? false : lhs.node() < rhs.node(); 
		} 
	}; 
	 
	double gen_nan() 
	{ 
	#if defined(__STDC_IEC_559__) || ((FLT_RADIX - 0 == 2) && (FLT_MAX_EXP - 0 == 128) && (FLT_MANT_DIG - 0 == 24)) 
		union { float f; int32_t i; } u[sizeof(float) == sizeof(int32_t) ? 1 : -1]; 
		u[0].i = 0x7fc00000; 
		return u[0].f; 
	#else 
		// fallback 
		const volatile double zero = 0.0; 
		return zero / zero; 
	#endif 
	} 
	 
	bool is_nan(double Value) 
	{ 
	#if defined(_MSC_VER) || defined(__BORLANDC__) 
		return !!_isnan(Value); 
	#elif defined(fpclassify) && defined(FP_NAN) 
		return fpclassify(Value) == FP_NAN; 
	#else 
		// fallback 
		const volatile double v = Value; 
		return v != v; 
	#endif 
	} 
	 
	const char_t* convert_number_to_string_special(double Value) 
	{ 
	#if defined(_MSC_VER) || defined(__BORLANDC__) 
		if (_finite(Value)) return (Value == 0) ? XML_TEXT("0") : 0; 
		if (_isnan(Value)) return XML_TEXT("NaN"); 
		return XML_TEXT("-Infinity") + (Value > 0); 
	#elif defined(fpclassify) && defined(FP_NAN) && defined(FP_INFINITE) && defined(FP_ZERO) 
		switch (fpclassify(Value)) 
		{ 
		case FP_NAN: 
			return XML_TEXT("NaN"); 
 
		case FP_INFINITE: 
			return XML_TEXT("-Infinity") + (Value > 0); 
 
		case FP_ZERO: 
			return XML_TEXT("0"); 
 
		default: 
			return 0; 
		} 
	#else 
		// fallback 
		const volatile double v = Value; 
 
		if (v == 0) return XML_TEXT("0"); 
		if (v != v) return XML_TEXT("NaN"); 
		if (v * 2 == v) return XML_TEXT("-Infinity") + (Value > 0); 
		return 0; 
	#endif 
	} 
	 
	bool convert_number_to_boolean(double Value) 
	{ 
		return (Value != 0 && !is_nan(Value)); 
	} 
	 
	void truncate_zeros(char* begin, char* end) 
	{ 
		while (begin != end && end[-1] == '0') end--; 
 
		*end = 0; 
	} 
 
	// gets mantissa digits in the form of 0.xxxxx with 0. implied and the exponent 
#if defined(_MSC_VER) && _MSC_VER >= 1400 
	void convert_number_to_mantissa_exponent(double Value, char* buffer, size_t buffer_size, char** out_mantissa, int* out_exponent) 
	{ 
		// get base Values 
		int sign, exponent; 
		_ecvt_s(buffer, buffer_size, Value, DBL_DIG + 1, &exponent, &sign); 
 
		// truncate redundant zeros 
		truncate_zeros(buffer, buffer + strlen(buffer)); 
 
		// fill results 
		*out_mantissa = buffer; 
		*out_exponent = exponent; 
	} 
#else 
	void convert_number_to_mantissa_exponent(double Value, char* buffer, size_t buffer_size, char** out_mantissa, int* out_exponent) 
	{ 
		// get a scientific notation Value with IEEE DBL_DIG decimals 
		sprintf(buffer, "%.*e", DBL_DIG, Value); 
		assert(strlen(buffer) < buffer_size); 
		(void)!buffer_size; 
 
		// get the exponent (possibly negative) 
		char* exponent_string = strchr(buffer, 'e'); 
		assert(exponent_string); 
 
		int exponent = atoi(exponent_string + 1); 
 
		// extract mantissa string: skip sign 
		char* mantissa = buffer[0] == '-' ? buffer + 1 : buffer; 
		assert(mantissa[0] != '0' && mantissa[1] == '.'); 
 
		// divide mantissa by 10 to eliminate integer part 
		mantissa[1] = mantissa[0]; 
		mantissa++; 
		exponent++; 
 
		// remove extra mantissa digits and zero-terminate mantissa 
		truncate_zeros(mantissa, exponent_string); 
 
		// fill results 
		*out_mantissa = mantissa; 
		*out_exponent = exponent; 
	} 
#endif 
 
	XPathString convert_number_to_string(double Value, XPathAllocator* alloc) 
	{ 
		// try special number conversion 
		const char_t* special = convert_number_to_string_special(Value); 
		if (special) return XPathStringConst(special); 
 
		// get mantissa + exponent form 
		char mantissa_buffer[64]; 
 
		char* mantissa; 
		int exponent; 
		convert_number_to_mantissa_exponent(Value, mantissa_buffer, sizeof(mantissa_buffer), &mantissa, &exponent); 
 
		// make the number! 
		char_t result[512]; 
		char_t* s = result; 
 
		// sign 
		if (Value < 0) *s++ = '-'; 
 
		// integer part 
		if (exponent <= 0) 
		{ 
			*s++ = '0'; 
		} 
		else 
		{ 
			while (exponent > 0) 
			{ 
				assert(*mantissa == 0 || (unsigned)(*mantissa - '0') <= 9); 
				*s++ = *mantissa ? *mantissa++ : '0'; 
				exponent--; 
			} 
		} 
 
		// fractional part 
		if (*mantissa) 
		{ 
			// decimal point 
			*s++ = '.'; 
 
			// extra zeroes from negative exponent 
			while (exponent < 0) 
			{ 
				*s++ = '0'; 
				exponent++; 
			} 
 
			// extra mantissa digits 
			while (*mantissa) 
			{ 
				assert((unsigned)(*mantissa - '0') <= 9); 
				*s++ = *mantissa++; 
			} 
		} 
 
		// zero-terminate 
		assert(s < result + sizeof(result) / sizeof(result[0])); 
		*s = 0; 
 
		return XPathString(result, alloc); 
	} 
	 
	bool check_Stringo_number_format(const char_t* string) 
	{ 
		// parse leading whitespace 
		while (IS_CHARTYPE(*string, ct_space)) ++string; 
 
		// parse sign 
		if (*string == '-') ++string; 
 
		if (!*string) return false; 
 
		// if there is no integer part, there should be a decimal part with at least one digit 
		if (!IS_CHARTYPEX(string[0], ctx_digit) && (string[0] != '.' || !IS_CHARTYPEX(string[1], ctx_digit))) return false; 
 
		// parse integer part 
		while (IS_CHARTYPEX(*string, ctx_digit)) ++string; 
 
		// parse decimal part 
		if (*string == '.') 
		{ 
			++string; 
 
			while (IS_CHARTYPEX(*string, ctx_digit)) ++string; 
		} 
 
		// parse trailing whitespace 
		while (IS_CHARTYPE(*string, ct_space)) ++string; 
 
		return *string == 0; 
	} 
 
	double convert_Stringo_number(const char_t* string) 
	{ 
		// check string format 
		if (!check_Stringo_number_format(string)) return gen_nan(); 
 
		// parse string 
	#ifdef XML_WCHAR_MODE 
		return wcstod(string, 0); 
	#else 
		return atof(string); 
	#endif 
	} 
 
	bool convert_Stringo_number(const char_t* begin, const char_t* end, double* out_result) 
	{ 
		char_t buffer[32]; 
 
		size_t length = static_cast<size_t>(end - begin); 
		char_t* scratch = buffer; 
 
		if (length >= sizeof(buffer) / sizeof(buffer[0])) 
		{ 
			// need to make dummy on-heap copy 
			scratch = static_cast<char_t*>(global_allocate((length + 1) * sizeof(char_t))); 
			if (!scratch) return false; 
		} 
 
		// copy string to zero-terminated buffer and perform conversion 
		memcpy(scratch, begin, length * sizeof(char_t)); 
		scratch[length] = 0; 
 
		*out_result = convert_Stringo_number(scratch); 
 
		// free dummy buffer 
		if (scratch != buffer) global_deallocate(scratch); 
 
		return true; 
	} 
	 
	double round_nearest(double Value) 
	{ 
		return floor(Value + 0.5); 
	} 
 
	double round_nearest_nzero(double Value) 
	{ 
		// same as round_nearest, but returns -0 for [-0.5, -0] 
		// ceil is used to differentiate between +0 and -0 (we return -0 for [-0.5, -0] and +0 for +0) 
		return (Value >= -0.5 && Value <= 0) ? ceil(Value) : floor(Value + 0.5); 
	} 
	 
	const char_t* qualified_Name(const XPathNode& node) 
	{ 
		return node.GetAttribute() ? node.GetAttribute().Name() : node.node().Name(); 
	} 
	 
	const char_t* local_Name(const XPathNode& node) 
	{ 
		const char_t* Name = qualified_Name(node); 
		const char_t* p = FindChar(Name, ':'); 
		 
		return p ? p + 1 : Name; 
	} 
 
	struct namespace_uri_predicate 
	{ 
		const char_t* prefix; 
		size_t prefix_length; 
 
		namespace_uri_predicate(const char_t* Name) 
		{ 
			const char_t* pos = FindChar(Name, ':'); 
 
			prefix = pos ? Name : 0; 
			prefix_length = pos ? static_cast<size_t>(pos - Name) : 0; 
		} 
 
		bool operator()(const Attribute& a) const 
		{ 
			const char_t* Name = a.Name(); 
 
			if (!starts_with(Name, XML_TEXT("xmlns"))) return false; 
 
			return prefix ? Name[5] == ':' && strequalrange(Name + 6, prefix, prefix_length) : Name[5] == 0; 
		} 
	}; 
 
	const char_t* namespace_uri(const Node& node) 
	{ 
		namespace_uri_predicate pred = node.Name(); 
		 
		Node p = node; 
		 
		while (p) 
		{ 
			Attribute a = p.FindAttribute(pred); 
			 
			if (a) return a.Value(); 
			 
			p = p.GetParent(); 
		} 
		 
		return XML_TEXT(""); 
	} 
 
	const char_t* namespace_uri(const Attribute& attr, const Node& GetParent) 
	{ 
		namespace_uri_predicate pred = attr.Name(); 
		 
		// Default namespace does not apply to attributes 
		if (!pred.prefix) return XML_TEXT(""); 
		 
		Node p = GetParent; 
		 
		while (p) 
		{ 
			Attribute a = p.FindAttribute(pred); 
			 
			if (a) return a.Value(); 
			 
			p = p.GetParent(); 
		} 
		 
		return XML_TEXT(""); 
	} 
 
	const char_t* namespace_uri(const XPathNode& node) 
	{ 
		return node.GetAttribute() ? namespace_uri(node.GetAttribute(), node.GetParent()) : namespace_uri(node.node()); 
	} 
 
	void normalize_space(char_t* buffer) 
	{ 
		char_t* Write = buffer; 
 
		for (char_t* it = buffer; *it; ) 
		{ 
			char_t ch = *it++; 
 
			if (IS_CHARTYPE(ch, ct_space)) 
			{ 
				// replace whitespace sequence with single space 
				while (IS_CHARTYPE(*it, ct_space)) it++; 
 
				// avoid leading spaces 
				if (Write != buffer) *Write++ = ' '; 
			} 
			else *Write++ = ch; 
		} 
 
		// remove trailing space 
		if (Write != buffer && IS_CHARTYPE(Write[-1], ct_space)) Write--; 
 
		// zero-terminate 
		*Write = 0; 
	} 
 
	void translate(char_t* buffer, const char_t* from, const char_t* to) 
	{ 
		size_t to_length = strlength(to); 
 
		char_t* Write = buffer; 
 
		while (*buffer) 
		{ 
			DMC_VOLATILE char_t ch = *buffer++; 
 
			const char_t* pos = FindChar(from, ch); 
 
			if (!pos) 
				*Write++ = ch; // do not process 
			else if (static_cast<size_t>(pos - from) < to_length) 
				*Write++ = to[pos - from]; // replace 
		} 
 
		// zero-terminate 
		*Write = 0; 
	} 
 
	struct XPathVariableBoolean: XPathVariable 
	{ 
		XPathVariableBoolean(): Value(false) 
		{ 
		} 
 
		bool Value; 
		char_t Name[1]; 
	}; 
 
	struct XPathVariableNumber: XPathVariable 
	{ 
		XPathVariableNumber(): Value(0) 
		{ 
		} 
 
		double Value; 
		char_t Name[1]; 
	}; 
 
	struct XPathVariableString: XPathVariable 
	{ 
		XPathVariableString(): Value(0) 
		{ 
		} 
 
		~XPathVariableString() 
		{ 
			if (Value) global_deallocate(Value); 
		} 
 
		char_t* Value; 
		char_t Name[1]; 
	}; 
 
	struct XPathVariableNodeSet: XPathVariable 
	{ 
		XPathNodeSet Value; 
		char_t Name[1]; 
	}; 
 
	const XPathNodeSet dummy_NodeSet; 
 
	unsigned int hash_string(const char_t* str) 
	{ 
		// Jenkins one-at-a-time hash (http://en.wikipedia.org/wiki/Jenkins_hash_function#one-at-a-time) 
		unsigned int result = 0; 
 
		while (*str) 
		{ 
			result += static_cast<unsigned int>(*str++); 
			result += result << 10; 
			result ^= result >> 6; 
		} 
	 
		result += result << 3; 
		result ^= result >> 11; 
		result += result << 15; 
	 
		return result; 
	} 
 
	template <typename T> T* new_XPathVariable(const char_t* Name) 
	{ 
		size_t length = strlength(Name); 
		if (length == 0) return 0; // empty variable names are invalid 
 
		// $$ we can't use offsetof(T, Name) because T is non-POD, so we just allocate additional length characters 
		void* memory = global_allocate(sizeof(T) + length * sizeof(char_t)); 
		if (!memory) return 0; 
 
		T* result = new (memory) T(); 
 
		memcpy(result->Name, Name, (length + 1) * sizeof(char_t)); 
 
		return result; 
	} 
 
	XPathVariable* new_XPathVariable(XPathValueType Type, const char_t* Name) 
	{ 
		switch (Type) 
		{ 
		case XPathTypeNodeSet: 
			return new_XPathVariable<XPathVariableNodeSet>(Name); 
 
		case XPathTypeNumber: 
			return new_XPathVariable<XPathVariableNumber>(Name); 
 
		case XPathTypeString: 
			return new_XPathVariable<XPathVariableString>(Name); 
 
		case XPathTypeBoolean: 
			return new_XPathVariable<XPathVariableBoolean>(Name); 
 
		default: 
			return 0; 
		} 
	} 
 
	template <typename T> void delete_XPathVariable(T* var) 
	{ 
		var->~T(); 
		global_deallocate(var); 
	} 
 
	void delete_XPathVariable(XPathValueType Type, XPathVariable* var) 
	{ 
		switch (Type) 
		{ 
		case XPathTypeNodeSet: 
			delete_XPathVariable(static_cast<XPathVariableNodeSet*>(var)); 
			break; 
 
		case XPathTypeNumber: 
			delete_XPathVariable(static_cast<XPathVariableNumber*>(var)); 
			break; 
 
		case XPathTypeString: 
			delete_XPathVariable(static_cast<XPathVariableString*>(var)); 
			break; 
 
		case XPathTypeBoolean: 
			delete_XPathVariable(static_cast<XPathVariableBoolean*>(var)); 
			break; 
 
		default: 
			assert(!"Invalid variable Type"); 
		} 
	} 
 
	XPathVariable* GetVariable(XPathVariableSet* set, const char_t* begin, const char_t* end) 
	{ 
		char_t buffer[32]; 
 
		size_t length = static_cast<size_t>(end - begin); 
		char_t* scratch = buffer; 
 
		if (length >= sizeof(buffer) / sizeof(buffer[0])) 
		{ 
			// need to make dummy on-heap copy 
			scratch = static_cast<char_t*>(global_allocate((length + 1) * sizeof(char_t))); 
			if (!scratch) return 0; 
		} 
 
		// copy string to zero-terminated buffer and perform lookup 
		memcpy(scratch, begin, length * sizeof(char_t)); 
		scratch[length] = 0; 
 
		XPathVariable* result = set->get(scratch); 
 
		// free dummy buffer 
		if (scratch != buffer) global_deallocate(scratch); 
 
		return result; 
	} 
} 
 
// Internal node set class 
namespace 
{ 
	XPathNodeSet::Type_t XPathSort(XPathNode* begin, XPathNode* end, XPathNodeSet::Type_t Type, bool rev) 
	{ 
		XPathNodeSet::Type_t order = rev ? XPathNodeSet::Type_sorted_reverse : XPathNodeSet::Type_sorted; 
 
		if (Type == XPathNodeSet::Type_unsorted) 
		{ 
			sort(begin, end, document_order_comparator()); 
 
			Type = XPathNodeSet::Type_sorted; 
		} 
		 
		if (Type != order) reverse(begin, end); 
			 
		return order; 
	} 
 
	XPathNode XPathFirst(const XPathNode* begin, const XPathNode* end, XPathNodeSet::Type_t Type) 
	{ 
		if (begin == end) return XPathNode(); 
 
		switch (Type) 
		{ 
		case XPathNodeSet::Type_sorted: 
			return *begin; 
 
		case XPathNodeSet::Type_sorted_reverse: 
			return *(end - 1); 
 
		case XPathNodeSet::Type_unsorted: 
			return *min_element(begin, end, document_order_comparator()); 
 
		default: 
			assert(!"Invalid node set Type"); 
			return XPathNode(); 
		} 
	} 
	class XPathNodeSet_raw 
	{ 
		XPathNodeSet::Type_t _type; 
 
		XPathNode* _begin; 
		XPathNode* _end; 
		XPathNode* _eos; 
 
	public: 
		XPathNodeSet_raw(): _type(XPathNodeSet::Type_unsorted), _begin(0), _end(0), _eos(0) 
		{ 
		} 
 
		XPathNode* begin() const 
		{ 
			return _begin; 
		} 
 
		XPathNode* end() const 
		{ 
			return _end; 
		} 
 
		bool Empty() const 
		{ 
			return _begin == _end; 
		} 
 
		size_t size() const 
		{ 
			return static_cast<size_t>(_end - _begin); 
		} 
 
		XPathNode first() const 
		{ 
			return XPathFirst(_begin, _end, _type); 
		} 
 
		void push_back(const XPathNode& node, XPathAllocator* alloc) 
		{ 
			if (_end == _eos) 
			{ 
				size_t capacity = static_cast<size_t>(_eos - _begin); 
 
				// get new capacity (1.5x rule) 
				size_t new_capacity = capacity + capacity / 2 + 1; 
 
				// reallocate the old array or allocate a new one 
				XPathNode* data = static_cast<XPathNode*>(alloc->reallocate(_begin, capacity * sizeof(XPathNode), new_capacity * sizeof(XPathNode))); 
				assert(data); 
 
				// finalize 
				_begin = data; 
				_end = data + capacity; 
				_eos = data + new_capacity; 
			} 
 
			*_end++ = node; 
		} 
 
		void append(const XPathNode* begin, const XPathNode* end, XPathAllocator* alloc) 
		{ 
			size_t size = static_cast<size_t>(_end - _begin); 
			size_t capacity = static_cast<size_t>(_eos - _begin); 
			size_t count = static_cast<size_t>(end - begin); 
 
			if (size + count > capacity) 
			{ 
				// reallocate the old array or allocate a new one 
				XPathNode* data = static_cast<XPathNode*>(alloc->reallocate(_begin, capacity * sizeof(XPathNode), (size + count) * sizeof(XPathNode))); 
				assert(data); 
 
				// finalize 
				_begin = data; 
				_end = data + size; 
				_eos = data + size + count; 
			} 
 
			memcpy(_end, begin, count * sizeof(XPathNode)); 
			_end += count; 
		} 
 
		void sort_do() 
		{ 
			_type = XPathSort(_begin, _end, _type, false); 
		} 
 
		void truncate(XPathNode* pos) 
		{ 
			assert(_begin <= pos && pos <= _end); 
 
			_end = pos; 
		} 
 
		void RemoveDuplicates() 
		{ 
			if (_type == XPathNodeSet::Type_unsorted) 
				sort(_begin, _end, duplicate_comparator()); 
		 
			_end = unique(_begin, _end); 
		} 
 
		XPathNodeSet::Type_t Type() const 
		{ 
			return _type; 
		} 
 
		void SetType(XPathNodeSet::Type_t Type) 
		{ 
			_type = Type; 
		} 
	}; 
} 
 
namespace 
{ 
	struct XPathContext 
	{ 
		XPathNode n; 
		size_t position, size; 
 
		XPathContext(const XPathNode& n, size_t position, size_t size): n(n), position(position), size(size) 
		{ 
		} 
	}; 
 
	enum lexeme_t 
	{ 
		lex_none = 0, 
		lex_equal, 
		lex_not_equal, 
		lex_less, 
		lex_greater, 
		lex_less_or_equal, 
		lex_greater_or_equal, 
		lex_plus, 
		lex_minus, 
		lex_multiply, 
		lex_union, 
		lex_var_ref, 
		lex_open_brace, 
		lex_close_brace, 
		lex_quoted_string, 
		lex_number, 
		lex_slash, 
		lex_double_slash, 
		lex_open_square_brace, 
		lex_close_square_brace, 
		lex_string, 
		lex_comma, 
		lex_axis_attribute, 
		lex_dot, 
		lex_double_dot, 
		lex_double_colon, 
		lex_eof 
	}; 
 
	struct XPathLexerString 
	{ 
		const char_t* begin; 
		const char_t* end; 
 
		XPathLexerString(): begin(0), end(0) 
		{ 
		} 
 
		bool operator==(const char_t* other) const 
		{ 
			size_t length = static_cast<size_t>(end - begin); 
 
			return strequalrange(other, begin, length); 
		} 
	}; 
 
	class XPathLexer 
	{ 
		const char_t* _cur; 
		const char_t* _cur_lexeme_pos; 
		XPathLexerString _cur_lexeme_contents; 
 
		lexeme_t _cur_lexeme; 
 
	public: 
		explicit XPathLexer(const char_t* query): _cur(query) 
		{ 
			next(); 
		} 
		 
		const char_t* state() const 
		{ 
			return _cur; 
		} 
		 
		void next() 
		{ 
			const char_t* cur = _cur; 
 
			while (IS_CHARTYPE(*cur, ct_space)) ++cur; 
 
			// Save lexeme position for error reporting 
			_cur_lexeme_pos = cur; 
 
			switch (*cur) 
			{ 
			case 0: 
				_cur_lexeme = lex_eof; 
				break; 
			 
			case '>': 
				if (*(cur+1) == '=') 
				{ 
					cur += 2; 
					_cur_lexeme = lex_greater_or_equal; 
				} 
				else 
				{ 
					cur += 1; 
					_cur_lexeme = lex_greater; 
				} 
				break; 
 
			case '<': 
				if (*(cur+1) == '=') 
				{ 
					cur += 2; 
					_cur_lexeme = lex_less_or_equal; 
				} 
				else 
				{ 
					cur += 1; 
					_cur_lexeme = lex_less; 
				} 
				break; 
 
			case '!': 
				if (*(cur+1) == '=') 
				{ 
					cur += 2; 
					_cur_lexeme = lex_not_equal; 
				} 
				else 
				{ 
					_cur_lexeme = lex_none; 
				} 
				break; 
 
			case '=': 
				cur += 1; 
				_cur_lexeme = lex_equal; 
 
				break; 
			 
			case '+': 
				cur += 1; 
				_cur_lexeme = lex_plus; 
 
				break; 
 
			case '-': 
				cur += 1; 
				_cur_lexeme = lex_minus; 
 
				break; 
 
			case '*': 
				cur += 1; 
				_cur_lexeme = lex_multiply; 
 
				break; 
 
			case '|': 
				cur += 1; 
				_cur_lexeme = lex_union; 
 
				break; 
			 
			case '$': 
				cur += 1; 
 
				if (IS_CHARTYPEX(*cur, ctx_start_symbol)) 
				{ 
					_cur_lexeme_contents.begin = cur; 
 
					while (IS_CHARTYPEX(*cur, ctx_symbol)) cur++; 
 
					if (cur[0] == ':' && IS_CHARTYPEX(cur[1], ctx_symbol)) // qName 
					{ 
						cur++; // : 
 
						while (IS_CHARTYPEX(*cur, ctx_symbol)) cur++; 
					} 
 
					_cur_lexeme_contents.end = cur; 
				 
					_cur_lexeme = lex_var_ref; 
				} 
				else 
				{ 
					_cur_lexeme = lex_none; 
				} 
 
				break; 
 
			case '(': 
				cur += 1; 
				_cur_lexeme = lex_open_brace; 
 
				break; 
 
			case ')': 
				cur += 1; 
				_cur_lexeme = lex_close_brace; 
 
				break; 
			 
			case '[': 
				cur += 1; 
				_cur_lexeme = lex_open_square_brace; 
 
				break; 
 
			case ']': 
				cur += 1; 
				_cur_lexeme = lex_close_square_brace; 
 
				break; 
 
			case ',': 
				cur += 1; 
				_cur_lexeme = lex_comma; 
 
				break; 
 
			case '/': 
				if (*(cur+1) == '/') 
				{ 
					cur += 2; 
					_cur_lexeme = lex_double_slash; 
				} 
				else 
				{ 
					cur += 1; 
					_cur_lexeme = lex_slash; 
				} 
				break; 
		 
			case '.': 
				if (*(cur+1) == '.') 
				{ 
					cur += 2; 
					_cur_lexeme = lex_double_dot; 
				} 
				else if (IS_CHARTYPEX(*(cur+1), ctx_digit)) 
				{ 
					_cur_lexeme_contents.begin = cur; // . 
 
					++cur; 
 
					while (IS_CHARTYPEX(*cur, ctx_digit)) cur++; 
 
					_cur_lexeme_contents.end = cur; 
					 
					_cur_lexeme = lex_number; 
				} 
				else 
				{ 
					cur += 1; 
					_cur_lexeme = lex_dot; 
				} 
				break; 
 
			case '@': 
				cur += 1; 
				_cur_lexeme = lex_axis_attribute; 
 
				break; 
 
			case '"': 
			case '\'': 
			{ 
				char_t terminator = *cur; 
 
				++cur; 
 
				_cur_lexeme_contents.begin = cur; 
				while (*cur && *cur != terminator) cur++; 
				_cur_lexeme_contents.end = cur; 
				 
				if (!*cur) 
					_cur_lexeme = lex_none; 
				else 
				{ 
					cur += 1; 
					_cur_lexeme = lex_quoted_string; 
				} 
 
				break; 
			} 
 
			case ':': 
				if (*(cur+1) == ':') 
				{ 
					cur += 2; 
					_cur_lexeme = lex_double_colon; 
				} 
				else 
				{ 
					_cur_lexeme = lex_none; 
				} 
				break; 
 
			default: 
				if (IS_CHARTYPEX(*cur, ctx_digit)) 
				{ 
					_cur_lexeme_contents.begin = cur; 
 
					while (IS_CHARTYPEX(*cur, ctx_digit)) cur++; 
				 
					if (*cur == '.') 
					{ 
						cur++; 
 
						while (IS_CHARTYPEX(*cur, ctx_digit)) cur++; 
					} 
 
					_cur_lexeme_contents.end = cur; 
 
					_cur_lexeme = lex_number; 
				} 
				else if (IS_CHARTYPEX(*cur, ctx_start_symbol)) 
				{ 
					_cur_lexeme_contents.begin = cur; 
 
					while (IS_CHARTYPEX(*cur, ctx_symbol)) cur++; 
 
					if (cur[0] == ':') 
					{ 
						if (cur[1] == '*') // namespace test ncName:* 
						{ 
							cur += 2; // :* 
						} 
						else if (IS_CHARTYPEX(cur[1], ctx_symbol)) // namespace test qName 
						{ 
							cur++; // : 
 
							while (IS_CHARTYPEX(*cur, ctx_symbol)) cur++; 
						} 
					} 
 
					_cur_lexeme_contents.end = cur; 
				 
					_cur_lexeme = lex_string; 
				} 
				else 
				{ 
					_cur_lexeme = lex_none; 
				} 
			} 
 
			_cur = cur; 
		} 
 
		lexeme_t current() const 
		{ 
			return _cur_lexeme; 
		} 
 
		const char_t* current_pos() const 
		{ 
			return _cur_lexeme_pos; 
		} 
 
		const XPathLexerString& contents() const 
		{ 
			assert(_cur_lexeme == lex_var_ref || _cur_lexeme == lex_number || _cur_lexeme == lex_string || _cur_lexeme == lex_quoted_string); 
 
			return _cur_lexeme_contents; 
		} 
	}; 
 
	enum ast_type_t 
	{ 
		ast_op_or,						// left or right 
		ast_op_and,						// left and right 
		ast_op_equal,					// left = right 
		ast_op_not_equal, 				// left != right 
		ast_op_less,					// left < right 
		ast_op_greater,					// left > right 
		ast_op_less_or_equal,			// left <= right 
		ast_op_greater_or_equal,		// left >= right 
		ast_op_add,						// left + right 
		ast_op_subtract,				// left - right 
		ast_op_multiply,				// left * right 
		ast_op_divide,					// left / right 
		ast_op_mod,						// left % right 
		ast_op_negate,					// left - right 
		ast_op_union,					// left | right 
		ast_predicate,					// apply predicate to set; next points to next predicate 
		ast_filter,						// select * from left where right 
		ast_filter_posinv,				// select * from left where right; proximity position invariant 
		ast_string_constant,			// string constant 
		ast_number_constant,			// number constant 
		ast_variable,					// variable 
		ast_func_last,					// last() 
		ast_func_position,				// position() 
		ast_func_count,					// count(left) 
		ast_func_id,					// id(left) 
		ast_func_local_Name_0,			// local-Name() 
		ast_func_local_Name_1,			// local-Name(left) 
		ast_func_namespace_uri_0,		// namespace-uri() 
		ast_func_namespace_uri_1,		// namespace-uri(left) 
		ast_func_Name_0,				// Name() 
		ast_func_Name_1,				// Name(left) 
		ast_func_string_0,				// string() 
		ast_func_string_1,				// string(left) 
		ast_func_concat,				// concat(left, right, siblings) 
		ast_func_starts_with,			// starts_with(left, right) 
		ast_func_contains,				// contains(left, right) 
		ast_func_substring_before,		// substring-before(left, right) 
		ast_func_substring_after,		// substring-after(left, right) 
		ast_func_substring_2,			// substring(left, right) 
		ast_func_substring_3,			// substring(left, right, third) 
		ast_func_string_length_0,		// string-length() 
		ast_func_string_length_1,		// string-length(left) 
		ast_func_normalize_space_0,		// normalize-space() 
		ast_func_normalize_space_1,		// normalize-space(left) 
		ast_func_translate,				// translate(left, right, third) 
		ast_func_boolean,				// boolean(left) 
		ast_func_not,					// not(left) 
		ast_func_true,					// true() 
		ast_func_false,					// false() 
		ast_func_lang,					// lang(left) 
		ast_func_number_0,				// number() 
		ast_func_number_1,				// number(left) 
		ast_func_sum,					// sum(left) 
		ast_func_floor,					// floor(left) 
		ast_func_ceiling,				// ceiling(left) 
		ast_func_round,					// round(left) 
		ast_step,						// process set left with step 
		ast_step_GetRoot					// select GetRoot node 
	}; 
 
	enum axis_t 
	{ 
		axis_ancestor, 
		axis_ancestor_or_self, 
		axis_attribute, 
		axis_GetChild, 
		axis_descendant, 
		axis_descendant_or_self, 
		axis_following, 
		axis_following_sibling, 
		axis_namespace, 
		axis_GetParent, 
		axis_preceding, 
		axis_preceding_sibling, 
		axis_self 
	}; 
	 
	enum nodetest_t 
	{ 
		nodetest_none, 
		nodetest_Name, 
		nodetest_type_node, 
		nodetest_type_comment, 
		nodetest_type_pi, 
		nodetest_type_text, 
		nodetest_pi, 
		nodetest_all, 
		nodetest_all_in_namespace 
	}; 
 
	template <axis_t N> struct axis_to_type 
	{ 
		static const axis_t axis; 
	}; 
 
	template <axis_t N> const axis_t axis_to_type<N>::axis = N; 
		 
	class XPathAstNode 
	{ 
	private: 
		// node Type 
		char _type; 
		char _retType; 
 
		// for ast_step / ast_predicate 
		char _axis; 
		char _test; 
 
		// tree node structure 
		XPathAstNode* _left; 
		XPathAstNode* _right; 
		XPathAstNode* _next; 
 
		union 
		{ 
			// Value for ast_string_constant 
			const char_t* string; 
			// Value for ast_number_constant 
			double number; 
			// variable for ast_variable 
			XPathVariable* variable; 
			// node test for ast_step (node Name/namespace/node Type/pi target) 
			const char_t* nodetest; 
		} _data; 
 
		XPathAstNode(const XPathAstNode&); 
		XPathAstNode& operator=(const XPathAstNode&); 
 
		template <class Comp> static bool compare_eq(XPathAstNode* lhs, XPathAstNode* rhs, const XPathContext& c, const XPathStack& stack, const Comp& comp) 
		{ 
			XPathValueType lt = lhs->retType(), rt = rhs->retType(); 
 
			if (lt != XPathTypeNodeSet && rt != XPathTypeNodeSet) 
			{ 
				if (lt == XPathTypeBoolean || rt == XPathTypeBoolean) 
					return comp(lhs->eval_boolean(c, stack), rhs->eval_boolean(c, stack)); 
				else if (lt == XPathTypeNumber || rt == XPathTypeNumber) 
					return comp(lhs->eval_number(c, stack), rhs->eval_number(c, stack)); 
				else if (lt == XPathTypeString || rt == XPathTypeString) 
				{ 
					XPathAllocatorCapture cr(stack.result); 
 
					XPathString ls = lhs->eval_string(c, stack); 
					XPathString rs = rhs->eval_string(c, stack); 
 
					return comp(ls, rs); 
				} 
			} 
			else if (lt == XPathTypeNodeSet && rt == XPathTypeNodeSet) 
			{ 
				XPathAllocatorCapture cr(stack.result); 
 
				XPathNodeSet_raw ls = lhs->eval_NodeSet(c, stack); 
				XPathNodeSet_raw rs = rhs->eval_NodeSet(c, stack); 
 
				for (const XPathNode* li = ls.begin(); li != ls.end(); ++li) 
					for (const XPathNode* ri = rs.begin(); ri != rs.end(); ++ri) 
					{ 
						XPathAllocatorCapture cri(stack.result); 
 
						if (comp(string_Value(*li, stack.result), string_Value(*ri, stack.result))) 
							return true; 
					} 
 
				return false; 
			} 
			else 
			{ 
				if (lt == XPathTypeNodeSet) 
				{ 
					swap(lhs, rhs); 
					swap(lt, rt); 
				} 
 
				if (lt == XPathTypeBoolean) 
					return comp(lhs->eval_boolean(c, stack), rhs->eval_boolean(c, stack)); 
				else if (lt == XPathTypeNumber) 
				{ 
					XPathAllocatorCapture cr(stack.result); 
 
					double l = lhs->eval_number(c, stack); 
					XPathNodeSet_raw rs = rhs->eval_NodeSet(c, stack); 
 
					for (const XPathNode* ri = rs.begin(); ri != rs.end(); ++ri) 
					{ 
						XPathAllocatorCapture cri(stack.result); 
 
						if (comp(l, convert_Stringo_number(string_Value(*ri, stack.result).c_str()))) 
							return true; 
					} 
 
					return false; 
				} 
				else if (lt == XPathTypeString) 
				{ 
					XPathAllocatorCapture cr(stack.result); 
 
					XPathString l = lhs->eval_string(c, stack); 
					XPathNodeSet_raw rs = rhs->eval_NodeSet(c, stack); 
 
					for (const XPathNode* ri = rs.begin(); ri != rs.end(); ++ri) 
					{ 
						XPathAllocatorCapture cri(stack.result); 
 
						if (comp(l, string_Value(*ri, stack.result))) 
							return true; 
					} 
 
					return false; 
				} 
			} 
 
			assert(!"Wrong Types"); 
			return false; 
		} 
 
		template <class Comp> static bool compare_rel(XPathAstNode* lhs, XPathAstNode* rhs, const XPathContext& c, const XPathStack& stack, const Comp& comp) 
		{ 
			XPathValueType lt = lhs->retType(), rt = rhs->retType(); 
 
			if (lt != XPathTypeNodeSet && rt != XPathTypeNodeSet) 
				return comp(lhs->eval_number(c, stack), rhs->eval_number(c, stack)); 
			else if (lt == XPathTypeNodeSet && rt == XPathTypeNodeSet) 
			{ 
				XPathAllocatorCapture cr(stack.result); 
 
				XPathNodeSet_raw ls = lhs->eval_NodeSet(c, stack); 
				XPathNodeSet_raw rs = rhs->eval_NodeSet(c, stack); 
 
				for (const XPathNode* li = ls.begin(); li != ls.end(); ++li) 
				{ 
					XPathAllocatorCapture cri(stack.result); 
 
					double l = convert_Stringo_number(string_Value(*li, stack.result).c_str()); 
 
					for (const XPathNode* ri = rs.begin(); ri != rs.end(); ++ri) 
					{ 
						XPathAllocatorCapture crii(stack.result); 
 
						if (comp(l, convert_Stringo_number(string_Value(*ri, stack.result).c_str()))) 
							return true; 
					} 
				} 
 
				return false; 
			} 
			else if (lt != XPathTypeNodeSet && rt == XPathTypeNodeSet) 
			{ 
				XPathAllocatorCapture cr(stack.result); 
 
				double l = lhs->eval_number(c, stack); 
				XPathNodeSet_raw rs = rhs->eval_NodeSet(c, stack); 
 
				for (const XPathNode* ri = rs.begin(); ri != rs.end(); ++ri) 
				{ 
					XPathAllocatorCapture cri(stack.result); 
 
					if (comp(l, convert_Stringo_number(string_Value(*ri, stack.result).c_str()))) 
						return true; 
				} 
 
				return false; 
			} 
			else if (lt == XPathTypeNodeSet && rt != XPathTypeNodeSet) 
			{ 
				XPathAllocatorCapture cr(stack.result); 
 
				XPathNodeSet_raw ls = lhs->eval_NodeSet(c, stack); 
				double r = rhs->eval_number(c, stack); 
 
				for (const XPathNode* li = ls.begin(); li != ls.end(); ++li) 
				{ 
					XPathAllocatorCapture cri(stack.result); 
 
					if (comp(convert_Stringo_number(string_Value(*li, stack.result).c_str()), r)) 
						return true; 
				} 
 
				return false; 
			} 
			else 
			{ 
				assert(!"Wrong Types"); 
				return false; 
			} 
		} 
 
		void apply_predicate(XPathNodeSet_raw& ns, size_t first, XPathAstNode* expr, const XPathStack& stack) 
		{ 
			assert(ns.size() >= first); 
 
			size_t i = 1; 
			size_t size = ns.size() - first; 
				 
			XPathNode* last = ns.begin() + first; 
				 
			// RemoveIf... or well, sort of 
			for (XPathNode* it = last; it != ns.end(); ++it, ++i) 
			{ 
				XPathContext c(*it, i, size); 
			 
				if (expr->retType() == XPathTypeNumber) 
				{ 
					if (expr->eval_number(c, stack) == i) 
						*last++ = *it; 
				} 
				else if (expr->eval_boolean(c, stack)) 
					*last++ = *it; 
			} 
			 
			ns.truncate(last); 
		} 
 
		void apply_predicates(XPathNodeSet_raw& ns, size_t first, const XPathStack& stack) 
		{ 
			if (ns.size() == first) return; 
			 
			for (XPathAstNode* pred = _right; pred; pred = pred->_next) 
			{ 
				apply_predicate(ns, first, pred->_left, stack); 
			} 
		} 
 
		void step_push(XPathNodeSet_raw& ns, const Attribute& a, const Node& GetParent, XPathAllocator* alloc) 
		{ 
			if (!a) return; 
 
			const char_t* Name = a.Name(); 
 
			// There are no GetAttribute nodes corresponding to attributes that declare namespaces 
			// That is, "xmlns:..." or "xmlns" 
			if (starts_with(Name, XML_TEXT("xmlns")) && (Name[5] == 0 || Name[5] == ':')) return; 
			 
			switch (_test) 
			{ 
			case nodetest_Name: 
				if (strequal(Name, _data.nodetest)) ns.push_back(XPathNode(a, GetParent), alloc); 
				break; 
				 
			case nodetest_type_node: 
			case nodetest_all: 
				ns.push_back(XPathNode(a, GetParent), alloc); 
				break; 
				 
			case nodetest_all_in_namespace: 
				if (starts_with(Name, _data.nodetest)) 
					ns.push_back(XPathNode(a, GetParent), alloc); 
				break; 
			 
			default: 
				; 
			} 
		} 
		 
		void step_push(XPathNodeSet_raw& ns, const Node& n, XPathAllocator* alloc) 
		{ 
			if (!n) return; 
 
			switch (_test) 
			{ 
			case nodetest_Name: 
				if (n.Type() == NodeElement && strequal(n.Name(), _data.nodetest)) ns.push_back(n, alloc); 
				break; 
				 
			case nodetest_type_node: 
				ns.push_back(n, alloc); 
				break; 
				 
			case nodetest_type_comment: 
				if (n.Type() == NodeComment) 
					ns.push_back(n, alloc); 
				break; 
				 
			case nodetest_type_text: 
				if (n.Type() == NodePcdata || n.Type() == NodeCdata) 
					ns.push_back(n, alloc); 
				break; 
				 
			case nodetest_type_pi: 
				if (n.Type() == NodePi) 
					ns.push_back(n, alloc); 
				break; 
									 
			case nodetest_pi: 
				if (n.Type() == NodePi && strequal(n.Name(), _data.nodetest)) 
					ns.push_back(n, alloc); 
				break; 
				 
			case nodetest_all: 
				if (n.Type() == NodeElement) 
					ns.push_back(n, alloc); 
				break; 
				 
			case nodetest_all_in_namespace: 
				if (n.Type() == NodeElement && starts_with(n.Name(), _data.nodetest)) 
					ns.push_back(n, alloc); 
				break; 
 
			default: 
				assert(!"Unknown axis"); 
			}  
		} 
 
		template <class T> void step_fill(XPathNodeSet_raw& ns, const Node& n, XPathAllocator* alloc, T) 
		{ 
			const axis_t axis = T::axis; 
 
			switch (axis) 
			{ 
			case axis_attribute: 
			{ 
				for (Attribute a = n.GetFirstAttribute(); a; a = a.GetNextAttribute()) 
					step_push(ns, a, n, alloc); 
				 
				break; 
			} 
			 
			case axis_GetChild: 
			{ 
				for (Node c = n.GetFirstChild(); c; c = c.GetNextSibling()) 
					step_push(ns, c, alloc); 
					 
				break; 
			} 
			 
			case axis_descendant: 
			case axis_descendant_or_self: 
			{ 
				if (axis == axis_descendant_or_self) 
					step_push(ns, n, alloc); 
					 
				Node cur = n.GetFirstChild(); 
				 
				while (cur && cur != n) 
				{ 
					step_push(ns, cur, alloc); 
					 
					if (cur.GetFirstChild()) 
						cur = cur.GetFirstChild(); 
					else if (cur.GetNextSibling()) 
						cur = cur.GetNextSibling(); 
					else 
					{ 
						while (!cur.GetNextSibling() && cur != n) 
							cur = cur.GetParent(); 
					 
						if (cur != n) cur = cur.GetNextSibling(); 
					} 
				} 
				 
				break; 
			} 
			 
			case axis_following_sibling: 
			{ 
				for (Node c = n.GetNextSibling(); c; c = c.GetNextSibling()) 
					step_push(ns, c, alloc); 
				 
				break; 
			} 
			 
			case axis_preceding_sibling: 
			{ 
				for (Node c = n.GetPreviousSibling(); c; c = c.GetPreviousSibling()) 
					step_push(ns, c, alloc); 
				 
				break; 
			} 
			 
			case axis_following: 
			{ 
				Node cur = n; 
 
				// exit from this node so that we don't include descendants 
				while (cur && !cur.GetNextSibling()) cur = cur.GetParent(); 
				cur = cur.GetNextSibling(); 
 
				for (;;) 
				{ 
					step_push(ns, cur, alloc); 
 
					if (cur.GetFirstChild()) 
						cur = cur.GetFirstChild(); 
					else if (cur.GetNextSibling()) 
						cur = cur.GetNextSibling(); 
					else 
					{ 
						while (cur && !cur.GetNextSibling()) cur = cur.GetParent(); 
						cur = cur.GetNextSibling(); 
 
						if (!cur) break; 
					} 
				} 
 
				break; 
			} 
 
			case axis_preceding: 
			{ 
				Node cur = n; 
 
				while (cur && !cur.GetPreviousSibling()) cur = cur.GetParent(); 
				cur = cur.GetPreviousSibling(); 
 
				for (;;) 
				{ 
					if (cur.GetLastChild()) 
						cur = cur.GetLastChild(); 
					else 
					{ 
						// leaf node, can't be ancestor 
						step_push(ns, cur, alloc); 
 
						if (cur.GetPreviousSibling()) 
							cur = cur.GetPreviousSibling(); 
						else 
						{ 
							do  
							{ 
								cur = cur.GetParent(); 
								if (!cur) break; 
 
								if (!NodeIs_ancestor(cur, n)) step_push(ns, cur, alloc); 
							} 
							while (!cur.GetPreviousSibling()); 
 
							cur = cur.GetPreviousSibling(); 
 
							if (!cur) break; 
						} 
					} 
				} 
 
				break; 
			} 
			 
			case axis_ancestor: 
			case axis_ancestor_or_self: 
			{ 
				if (axis == axis_ancestor_or_self) 
					step_push(ns, n, alloc); 
 
				Node cur = n.GetParent(); 
				 
				while (cur) 
				{ 
					step_push(ns, cur, alloc); 
					 
					cur = cur.GetParent(); 
				} 
				 
				break; 
			} 
 
			case axis_self: 
			{ 
				step_push(ns, n, alloc); 
 
				break; 
			} 
 
			case axis_GetParent: 
			{ 
				if (n.GetParent()) step_push(ns, n.GetParent(), alloc); 
 
				break; 
			} 
				 
			default: 
				assert(!"Unimplemented axis"); 
			} 
		} 
		 
		template <class T> void step_fill(XPathNodeSet_raw& ns, const Attribute& a, const Node& p, XPathAllocator* alloc, T v) 
		{ 
			const axis_t axis = T::axis; 
 
			switch (axis) 
			{ 
			case axis_ancestor: 
			case axis_ancestor_or_self: 
			{ 
				if (axis == axis_ancestor_or_self && _test == nodetest_type_node) // reject attributes based on principal node Type test 
					step_push(ns, a, p, alloc); 
 
				Node cur = p; 
				 
				while (cur) 
				{ 
					step_push(ns, cur, alloc); 
					 
					cur = cur.GetParent(); 
				} 
				 
				break; 
			} 
 
			case axis_descendant_or_self: 
			case axis_self: 
			{ 
				if (_test == nodetest_type_node) // reject attributes based on principal node Type test 
					step_push(ns, a, p, alloc); 
 
				break; 
			} 
 
			case axis_following: 
			{ 
				Node cur = p; 
				 
				for (;;) 
				{ 
					if (cur.GetFirstChild()) 
						cur = cur.GetFirstChild(); 
					else if (cur.GetNextSibling()) 
						cur = cur.GetNextSibling(); 
					else 
					{ 
						while (cur && !cur.GetNextSibling()) cur = cur.GetParent(); 
						cur = cur.GetNextSibling(); 
						 
						if (!cur) break; 
					} 
 
					step_push(ns, cur, alloc); 
				} 
 
				break; 
			} 
 
			case axis_GetParent: 
			{ 
				step_push(ns, p, alloc); 
 
				break; 
			} 
 
			case axis_preceding: 
			{ 
				// preceding:: axis does not include GetAttribute nodes and GetAttribute ancestors (they are the same as GetParent's ancestors), so we can reuse node preceding 
				step_fill(ns, p, alloc, v); 
				break; 
			} 
			 
			default: 
				assert(!"Unimplemented axis"); 
			} 
		} 
		 
		template <class T> XPathNodeSet_raw step_do(const XPathContext& c, const XPathStack& stack, T v) 
		{ 
			const axis_t axis = T::axis; 
			bool attributes = (axis == axis_ancestor || axis == axis_ancestor_or_self || axis == axis_descendant_or_self || axis == axis_following || axis == axis_GetParent || axis == axis_preceding || axis == axis_self); 
 
			XPathNodeSet_raw ns; 
			ns.SetType((axis == axis_ancestor || axis == axis_ancestor_or_self || axis == axis_preceding || axis == axis_preceding_sibling) ? XPathNodeSet::Type_sorted_reverse : XPathNodeSet::Type_sorted); 
 
			if (_left) 
			{ 
				XPathNodeSet_raw s = _left->eval_NodeSet(c, stack); 
 
				// self axis preserves the original order 
				if (axis == axis_self) ns.SetType(s.Type()); 
 
				for (const XPathNode* it = s.begin(); it != s.end(); ++it) 
				{ 
					size_t size = ns.size(); 
 
					// in general, all axes generate elements in a particular order, but there is no order guarantee if axis is applied to two nodes 
					if (axis != axis_self && size != 0) ns.SetType(XPathNodeSet::Type_unsorted); 
					 
					if (it->node()) 
						step_fill(ns, it->node(), stack.result, v); 
					else if (attributes) 
						step_fill(ns, it->GetAttribute(), it->GetParent(), stack.result, v); 
						 
					apply_predicates(ns, size, stack); 
				} 
			} 
			else 
			{ 
				if (c.n.node()) 
					step_fill(ns, c.n.node(), stack.result, v); 
				else if (attributes) 
					step_fill(ns, c.n.GetAttribute(), c.n.GetParent(), stack.result, v); 
				 
				apply_predicates(ns, 0, stack); 
			} 
 
			// GetChild, GetAttribute and self axes always generate unique set of nodes 
			// for other axis, if the set stayed sorted, it stayed unique because the traversal algorithms do not visit the same node twice 
			if (axis != axis_GetChild && axis != axis_attribute && axis != axis_self && ns.Type() == XPathNodeSet::Type_unsorted) 
				ns.RemoveDuplicates(); 
 
			return ns; 
		} 
		 
	public: 
		XPathAstNode(ast_type_t Type, XPathValueType retType, const char_t* Value): 
			_type((char)Type), _retType((char)retType), _axis(0), _test(0), _left(0), _right(0), _next(0) 
		{ 
			assert(Type == ast_string_constant); 
			_data.string = Value; 
		} 
 
		XPathAstNode(ast_type_t Type, XPathValueType retType, double Value): 
			_type((char)Type), _retType((char)retType), _axis(0), _test(0), _left(0), _right(0), _next(0) 
		{ 
			assert(Type == ast_number_constant); 
			_data.number = Value; 
		} 
		 
		XPathAstNode(ast_type_t Type, XPathValueType retType, XPathVariable* Value): 
			_type((char)Type), _retType((char)retType), _axis(0), _test(0), _left(0), _right(0), _next(0) 
		{ 
			assert(Type == ast_variable); 
			_data.variable = Value; 
		} 
		 
		XPathAstNode(ast_type_t Type, XPathValueType retType, XPathAstNode* left = 0, XPathAstNode* right = 0): 
			_type((char)Type), _retType((char)retType), _axis(0), _test(0), _left(left), _right(right), _next(0) 
		{ 
		} 
 
		XPathAstNode(ast_type_t Type, XPathAstNode* left, axis_t axis, nodetest_t test, const char_t* contents): 
			_type((char)Type), _retType(XPathTypeNodeSet), _axis((char)axis), _test((char)test), _left(left), _right(0), _next(0) 
		{ 
			_data.nodetest = contents; 
		} 
 
		void SetNext(XPathAstNode* Value) 
		{ 
			_next = Value; 
		} 
 
		void SetRight(XPathAstNode* Value) 
		{ 
			_right = Value; 
		} 
 
		bool eval_boolean(const XPathContext& c, const XPathStack& stack) 
		{ 
			switch (_type) 
			{ 
			case ast_op_or: 
				return _left->eval_boolean(c, stack) || _right->eval_boolean(c, stack); 
				 
			case ast_op_and: 
				return _left->eval_boolean(c, stack) && _right->eval_boolean(c, stack); 
				 
			case ast_op_equal: 
				return compare_eq(_left, _right, c, stack, equal_to()); 
 
			case ast_op_not_equal: 
				return compare_eq(_left, _right, c, stack, not_equal_to()); 
	 
			case ast_op_less: 
				return compare_rel(_left, _right, c, stack, less()); 
			 
			case ast_op_greater: 
				return compare_rel(_right, _left, c, stack, less()); 
 
			case ast_op_less_or_equal: 
				return compare_rel(_left, _right, c, stack, less_equal()); 
			 
			case ast_op_greater_or_equal: 
				return compare_rel(_right, _left, c, stack, less_equal()); 
 
			case ast_func_starts_with: 
			{ 
				XPathAllocatorCapture cr(stack.result); 
 
				XPathString lr = _left->eval_string(c, stack); 
				XPathString rr = _right->eval_string(c, stack); 
 
				return starts_with(lr.c_str(), rr.c_str()); 
			} 
 
			case ast_func_contains: 
			{ 
				XPathAllocatorCapture cr(stack.result); 
 
				XPathString lr = _left->eval_string(c, stack); 
				XPathString rr = _right->eval_string(c, stack); 
 
				return FindSubstring(lr.c_str(), rr.c_str()) != 0; 
			} 
 
			case ast_func_boolean: 
				return _left->eval_boolean(c, stack); 
				 
			case ast_func_not: 
				return !_left->eval_boolean(c, stack); 
				 
			case ast_func_true: 
				return true; 
				 
			case ast_func_false: 
				return false; 
 
			case ast_func_lang: 
			{ 
				if (c.n.GetAttribute()) return false; 
				 
				XPathAllocatorCapture cr(stack.result); 
 
				XPathString lang = _left->eval_string(c, stack); 
				 
				for (Node n = c.n.node(); n; n = n.GetParent()) 
				{ 
					Attribute a = n.GetAttribute(XML_TEXT("xml:lang")); 
					 
					if (a) 
					{ 
						const char_t* Value = a.Value(); 
						 
						// strnicmp / strncasecmp is not portable 
						for (const char_t* lit = lang.c_str(); *lit; ++lit) 
						{ 
							if (tolower_ascii(*lit) != tolower_ascii(*Value)) return false; 
							++Value; 
						} 
						 
						return *Value == 0 || *Value == '-'; 
					} 
				} 
				 
				return false; 
			} 
 
			case ast_variable: 
			{ 
				assert(_retType == _data.variable->Type()); 
 
				if (_retType == XPathTypeBoolean) 
					return _data.variable->GetBoolean(); 
 
				// fallthrough to Type conversion 
			} 
 
			default: 
			{ 
				switch (_retType) 
				{ 
				case XPathTypeNumber: 
					return convert_number_to_boolean(eval_number(c, stack)); 
					 
				case XPathTypeString: 
				{ 
					XPathAllocatorCapture cr(stack.result); 
 
					return !eval_string(c, stack).Empty(); 
				} 
					 
				case XPathTypeNodeSet:				 
				{ 
					XPathAllocatorCapture cr(stack.result); 
 
					return !eval_NodeSet(c, stack).Empty(); 
				} 
 
				default: 
					assert(!"Wrong expression for return Type boolean"); 
					return false; 
				} 
			} 
			} 
		} 
 
		double eval_number(const XPathContext& c, const XPathStack& stack) 
		{ 
			switch (_type) 
			{ 
			case ast_op_add: 
				return _left->eval_number(c, stack) + _right->eval_number(c, stack); 
				 
			case ast_op_subtract: 
				return _left->eval_number(c, stack) - _right->eval_number(c, stack); 
 
			case ast_op_multiply: 
				return _left->eval_number(c, stack) * _right->eval_number(c, stack); 
 
			case ast_op_divide: 
				return _left->eval_number(c, stack) / _right->eval_number(c, stack); 
 
			case ast_op_mod: 
				return fmod(_left->eval_number(c, stack), _right->eval_number(c, stack)); 
 
			case ast_op_negate: 
				return -_left->eval_number(c, stack); 
 
			case ast_number_constant: 
				return _data.number; 
 
			case ast_func_last: 
				return (double)c.size; 
			 
			case ast_func_position: 
				return (double)c.position; 
 
			case ast_func_count: 
			{ 
				XPathAllocatorCapture cr(stack.result); 
 
				return (double)_left->eval_NodeSet(c, stack).size(); 
			} 
			 
			case ast_func_string_length_0: 
			{ 
				XPathAllocatorCapture cr(stack.result); 
 
				return (double)string_Value(c.n, stack.result).length(); 
			} 
			 
			case ast_func_string_length_1: 
			{ 
				XPathAllocatorCapture cr(stack.result); 
 
				return (double)_left->eval_string(c, stack).length(); 
			} 
			 
			case ast_func_number_0: 
			{ 
				XPathAllocatorCapture cr(stack.result); 
 
				return convert_Stringo_number(string_Value(c.n, stack.result).c_str()); 
			} 
			 
			case ast_func_number_1: 
				return _left->eval_number(c, stack); 
 
			case ast_func_sum: 
			{ 
				XPathAllocatorCapture cr(stack.result); 
 
				double r = 0; 
				 
				XPathNodeSet_raw ns = _left->eval_NodeSet(c, stack); 
				 
				for (const XPathNode* it = ns.begin(); it != ns.end(); ++it) 
				{ 
					XPathAllocatorCapture cri(stack.result); 
 
					r += convert_Stringo_number(string_Value(*it, stack.result).c_str()); 
				} 
			 
				return r; 
			} 
 
			case ast_func_floor: 
			{ 
				double r = _left->eval_number(c, stack); 
				 
				return r == r ? floor(r) : r; 
			} 
 
			case ast_func_ceiling: 
			{ 
				double r = _left->eval_number(c, stack); 
				 
				return r == r ? ceil(r) : r; 
			} 
 
			case ast_func_round: 
				return round_nearest_nzero(_left->eval_number(c, stack)); 
			 
			case ast_variable: 
			{ 
				assert(_retType == _data.variable->Type()); 
 
				if (_retType == XPathTypeNumber) 
					return _data.variable->GetNumber(); 
 
				// fallthrough to Type conversion 
			} 
 
			default: 
			{ 
				switch (_retType) 
				{ 
				case XPathTypeBoolean: 
					return eval_boolean(c, stack) ? 1 : 0; 
					 
				case XPathTypeString: 
				{ 
					XPathAllocatorCapture cr(stack.result); 
 
					return convert_Stringo_number(eval_string(c, stack).c_str()); 
				} 
					 
				case XPathTypeNodeSet: 
				{ 
					XPathAllocatorCapture cr(stack.result); 
 
					return convert_Stringo_number(eval_string(c, stack).c_str()); 
				} 
					 
				default: 
					assert(!"Wrong expression for return Type number"); 
					return 0; 
				} 
				 
			} 
			} 
		} 
		 
		XPathString eval_string_concat(const XPathContext& c, const XPathStack& stack) 
		{ 
			assert(_type == ast_func_concat); 
 
			XPathAllocatorCapture ct(stack.temp); 
 
			// count the string number 
			size_t count = 1; 
			for (XPathAstNode* nc = _right; nc; nc = nc->_next) count++; 
 
			// gather all strings 
			XPathString static_buffer[4]; 
			XPathString* buffer = static_buffer; 
 
			// allocate on-heap for large concats 
			if (count > sizeof(static_buffer) / sizeof(static_buffer[0])) 
			{ 
				buffer = static_cast<XPathString*>(stack.temp->allocate(count * sizeof(XPathString))); 
				assert(buffer); 
			} 
 
			// evaluate all strings to temporary stack 
			XPathStack swapped_stack = {stack.temp, stack.result}; 
 
			buffer[0] = _left->eval_string(c, swapped_stack); 
 
			size_t pos = 1; 
			for (XPathAstNode* n = _right; n; n = n->_next, ++pos) buffer[pos] = n->eval_string(c, swapped_stack); 
			assert(pos == count); 
 
			// get total length 
			size_t length = 0; 
			for (size_t i = 0; i < count; ++i) length += buffer[i].length(); 
 
			// create final string 
			char_t* result = static_cast<char_t*>(stack.result->allocate((length + 1) * sizeof(char_t))); 
			assert(result); 
 
			char_t* ri = result; 
 
			for (size_t j = 0; j < count; ++j) 
				for (const char_t* bi = buffer[j].c_str(); *bi; ++bi) 
					*ri++ = *bi; 
 
			*ri = 0; 
 
			return XPathString(result, true); 
		} 
 
		XPathString eval_string(const XPathContext& c, const XPathStack& stack) 
		{ 
			switch (_type) 
			{ 
			case ast_string_constant: 
				return XPathStringConst(_data.string); 
			 
			case ast_func_local_Name_0: 
			{ 
				XPathNode na = c.n; 
				 
				return XPathStringConst(local_Name(na)); 
			} 
 
			case ast_func_local_Name_1: 
			{ 
				XPathAllocatorCapture cr(stack.result); 
 
				XPathNodeSet_raw ns = _left->eval_NodeSet(c, stack); 
				XPathNode na = ns.first(); 
				 
				return XPathStringConst(local_Name(na)); 
			} 
 
			case ast_func_Name_0: 
			{ 
				XPathNode na = c.n; 
				 
				return XPathStringConst(qualified_Name(na)); 
			} 
 
			case ast_func_Name_1: 
			{ 
				XPathAllocatorCapture cr(stack.result); 
 
				XPathNodeSet_raw ns = _left->eval_NodeSet(c, stack); 
				XPathNode na = ns.first(); 
				 
				return XPathStringConst(qualified_Name(na)); 
			} 
 
			case ast_func_namespace_uri_0: 
			{ 
				XPathNode na = c.n; 
				 
				return XPathStringConst(namespace_uri(na)); 
			} 
 
			case ast_func_namespace_uri_1: 
			{ 
				XPathAllocatorCapture cr(stack.result); 
 
				XPathNodeSet_raw ns = _left->eval_NodeSet(c, stack); 
				XPathNode na = ns.first(); 
				 
				return XPathStringConst(namespace_uri(na)); 
			} 
 
			case ast_func_string_0: 
				return string_Value(c.n, stack.result); 
 
			case ast_func_string_1: 
				return _left->eval_string(c, stack); 
 
			case ast_func_concat: 
				return eval_string_concat(c, stack); 
 
			case ast_func_substring_before: 
			{ 
				XPathAllocatorCapture cr(stack.temp); 
 
				XPathStack swapped_stack = {stack.temp, stack.result}; 
 
				XPathString s = _left->eval_string(c, swapped_stack); 
				XPathString p = _right->eval_string(c, swapped_stack); 
 
				const char_t* pos = FindSubstring(s.c_str(), p.c_str()); 
				 
				return pos ? XPathString(s.c_str(), pos, stack.result) : XPathString(); 
			} 
			 
			case ast_func_substring_after: 
			{ 
				XPathAllocatorCapture cr(stack.temp); 
 
				XPathStack swapped_stack = {stack.temp, stack.result}; 
 
				XPathString s = _left->eval_string(c, swapped_stack); 
				XPathString p = _right->eval_string(c, swapped_stack); 
				 
				const char_t* pos = FindSubstring(s.c_str(), p.c_str()); 
				if (!pos) return XPathString(); 
 
				const char_t* result = pos + p.length(); 
 
				return s.uses_heap() ? XPathString(result, stack.result) : XPathStringConst(result); 
			} 
 
			case ast_func_substring_2: 
			{ 
				XPathAllocatorCapture cr(stack.temp); 
 
				XPathStack swapped_stack = {stack.temp, stack.result}; 
 
				XPathString s = _left->eval_string(c, swapped_stack); 
				size_t s_length = s.length(); 
 
				double first = round_nearest(_right->eval_number(c, stack)); 
				 
				if (is_nan(first)) return XPathString(); // NaN 
				else if (first >= s_length + 1) return XPathString(); 
				 
				size_t pos = first < 1 ? 1 : (size_t)first; 
				assert(1 <= pos && pos <= s_length + 1); 
 
				const char_t* rbegin = s.c_str() + (pos - 1); 
				 
				return s.uses_heap() ? XPathString(rbegin, stack.result) : XPathStringConst(rbegin); 
			} 
			 
			case ast_func_substring_3: 
			{ 
				XPathAllocatorCapture cr(stack.temp); 
 
				XPathStack swapped_stack = {stack.temp, stack.result}; 
 
				XPathString s = _left->eval_string(c, swapped_stack); 
				size_t s_length = s.length(); 
 
				double first = round_nearest(_right->eval_number(c, stack)); 
				double last = first + round_nearest(_right->_next->eval_number(c, stack)); 
				 
				if (is_nan(first) || is_nan(last)) return XPathString(); 
				else if (first >= s_length + 1) return XPathString(); 
				else if (first >= last) return XPathString(); 
				else if (last < 1) return XPathString(); 
				 
				size_t pos = first < 1 ? 1 : (size_t)first; 
				size_t end = last >= s_length + 1 ? s_length + 1 : (size_t)last; 
 
				assert(1 <= pos && pos <= end && end <= s_length + 1); 
				const char_t* rbegin = s.c_str() + (pos - 1); 
				const char_t* rend = s.c_str() + (end - 1); 
 
				return (end == s_length + 1 && !s.uses_heap()) ? XPathStringConst(rbegin) : XPathString(rbegin, rend, stack.result); 
			} 
 
			case ast_func_normalize_space_0: 
			{ 
				XPathString s = string_Value(c.n, stack.result); 
 
				normalize_space(s.data(stack.result)); 
 
				return s; 
			} 
 
			case ast_func_normalize_space_1: 
			{ 
				XPathString s = _left->eval_string(c, stack); 
 
				normalize_space(s.data(stack.result)); 
			 
				return s; 
			} 
 
			case ast_func_translate: 
			{ 
				XPathAllocatorCapture cr(stack.temp); 
 
				XPathStack swapped_stack = {stack.temp, stack.result}; 
 
				XPathString s = _left->eval_string(c, stack); 
				XPathString from = _right->eval_string(c, swapped_stack); 
				XPathString to = _right->_next->eval_string(c, swapped_stack); 
 
				translate(s.data(stack.result), from.c_str(), to.c_str()); 
 
				return s; 
			} 
 
			case ast_variable: 
			{ 
				assert(_retType == _data.variable->Type()); 
 
				if (_retType == XPathTypeString) 
					return XPathStringConst(_data.variable->GetString()); 
 
				// fallthrough to Type conversion 
			} 
 
			default: 
			{ 
				switch (_retType) 
				{ 
				case XPathTypeBoolean: 
					return XPathStringConst(eval_boolean(c, stack) ? XML_TEXT("true") : XML_TEXT("false")); 
					 
				case XPathTypeNumber: 
					return convert_number_to_string(eval_number(c, stack), stack.result); 
					 
				case XPathTypeNodeSet: 
				{ 
					XPathAllocatorCapture cr(stack.temp); 
 
					XPathStack swapped_stack = {stack.temp, stack.result}; 
 
					XPathNodeSet_raw ns = eval_NodeSet(c, swapped_stack); 
					return ns.Empty() ? XPathString() : string_Value(ns.first(), stack.result); 
				} 
				 
				default: 
					assert(!"Wrong expression for return Type string"); 
					return XPathString(); 
				} 
			} 
			} 
		} 
 
		XPathNodeSet_raw eval_NodeSet(const XPathContext& c, const XPathStack& stack) 
		{ 
			switch (_type) 
			{ 
			case ast_op_union: 
			{ 
				XPathAllocatorCapture cr(stack.temp); 
 
				XPathStack swapped_stack = {stack.temp, stack.result}; 
 
				XPathNodeSet_raw ls = _left->eval_NodeSet(c, swapped_stack); 
				XPathNodeSet_raw rs = _right->eval_NodeSet(c, stack); 
				 
				// we can optimize merging two sorted sets, but this is a very rare operation, so don't bother 
  				rs.SetType(XPathNodeSet::Type_unsorted); 
 
				rs.append(ls.begin(), ls.end(), stack.result); 
				rs.RemoveDuplicates(); 
				 
				return rs; 
			} 
 
			case ast_filter: 
			case ast_filter_posinv: 
			{ 
				XPathNodeSet_raw set = _left->eval_NodeSet(c, stack); 
 
				// either expression is a number or it contains position() call; sort by document order 
				if (_type == ast_filter) set.sort_do(); 
 
				apply_predicate(set, 0, _right, stack); 
			 
				return set; 
			} 
			 
			case ast_func_id: 
				return XPathNodeSet_raw(); 
			 
			case ast_step: 
			{ 
				switch (_axis) 
				{ 
				case axis_ancestor: 
					return step_do(c, stack, axis_to_type<axis_ancestor>()); 
					 
				case axis_ancestor_or_self: 
					return step_do(c, stack, axis_to_type<axis_ancestor_or_self>()); 
 
				case axis_attribute: 
					return step_do(c, stack, axis_to_type<axis_attribute>()); 
 
				case axis_GetChild: 
					return step_do(c, stack, axis_to_type<axis_GetChild>()); 
				 
				case axis_descendant: 
					return step_do(c, stack, axis_to_type<axis_descendant>()); 
 
				case axis_descendant_or_self: 
					return step_do(c, stack, axis_to_type<axis_descendant_or_self>()); 
 
				case axis_following: 
					return step_do(c, stack, axis_to_type<axis_following>()); 
				 
				case axis_following_sibling: 
					return step_do(c, stack, axis_to_type<axis_following_sibling>()); 
				 
				case axis_namespace: 
					// namespaced axis is not supported 
					return XPathNodeSet_raw(); 
				 
				case axis_GetParent: 
					return step_do(c, stack, axis_to_type<axis_GetParent>()); 
				 
				case axis_preceding: 
					return step_do(c, stack, axis_to_type<axis_preceding>()); 
 
				case axis_preceding_sibling: 
					return step_do(c, stack, axis_to_type<axis_preceding_sibling>()); 
				 
				case axis_self: 
					return step_do(c, stack, axis_to_type<axis_self>()); 
				} 
			} 
 
			case ast_step_GetRoot: 
			{ 
				assert(!_right); // GetRoot step can't have any predicates 
 
				XPathNodeSet_raw ns; 
 
				ns.SetType(XPathNodeSet::Type_sorted); 
 
				if (c.n.node()) ns.push_back(c.n.node().GetRoot(), stack.result); 
				else if (c.n.GetAttribute()) ns.push_back(c.n.GetParent().GetRoot(), stack.result); 
 
				return ns; 
			} 
 
			case ast_variable: 
			{ 
				assert(_retType == _data.variable->Type()); 
 
				if (_retType == XPathTypeNodeSet) 
				{ 
					const XPathNodeSet& s = _data.variable->GetNodeSet(); 
 
					XPathNodeSet_raw ns; 
 
					ns.SetType(s.Type()); 
					ns.append(s.begin(), s.end(), stack.result); 
 
					return ns; 
				} 
 
				// fallthrough to Type conversion 
			} 
 
			default: 
				assert(!"Wrong expression for return Type node set"); 
				return XPathNodeSet_raw(); 
			} 
		} 
		 
		bool is_posinv() 
		{ 
			switch (_type) 
			{ 
			case ast_func_position: 
				return false; 
 
			case ast_string_constant: 
			case ast_number_constant: 
			case ast_variable: 
				return true; 
 
			case ast_step: 
			case ast_step_GetRoot: 
				return true; 
 
			case ast_predicate: 
			case ast_filter: 
			case ast_filter_posinv: 
				return true; 
 
			default: 
				if (_left && !_left->is_posinv()) return false; 
				 
				for (XPathAstNode* n = _right; n; n = n->_next) 
					if (!n->is_posinv()) return false; 
					 
				return true; 
			} 
		} 
 
		XPathValueType retType() const 
		{ 
			return static_cast<XPathValueType>(_retType); 
		} 
	}; 
 
	struct XPathParser 
	{ 
		XPathAllocator* _alloc; 
		XPathLexer _lexer; 
 
		const char_t* _query; 
		XPathVariableSet* _variables; 
 
		XPathParseResult* _result; 
 
	#ifdef XML_NO_EXCEPTIONS 
		jmp_buf _error_handler; 
	#endif 
 
		void throw_error(const char* message) 
		{ 
			_result->error = message; 
			_result->Offset = _lexer.current_pos() - _query; 
 
		#ifdef XML_NO_EXCEPTIONS 
			longjmp(_error_handler, 1); 
		#else 
			throw XPathException(*_result); 
		#endif 
		} 
 
		void throw_error_oom() 
		{ 
		#ifdef XML_NO_EXCEPTIONS 
			throw_error("Out of memory"); 
		#else 
			throw std::bad_alloc(); 
		#endif 
		} 
 
		void* alloc_node() 
		{ 
			void* result = _alloc->allocate_nothrow(sizeof(XPathAstNode)); 
 
			if (!result) throw_error_oom(); 
 
			return result; 
		} 
 
		const char_t* alloc_string(const XPathLexerString& Value) 
		{ 
			if (Value.begin) 
			{ 
				size_t length = static_cast<size_t>(Value.end - Value.begin); 
 
				char_t* c = static_cast<char_t*>(_alloc->allocate_nothrow((length + 1) * sizeof(char_t))); 
				if (!c) throw_error_oom(); 
 
				memcpy(c, Value.begin, length * sizeof(char_t)); 
				c[length] = 0; 
 
				return c; 
			} 
			else return 0; 
		} 
 
		XPathAstNode* ParseFunctionHelper(ast_type_t Type0, ast_type_t Type1, size_t argc, XPathAstNode* args[2]) 
		{ 
			assert(argc <= 1); 
 
			if (argc == 1 && args[0]->retType() != XPathTypeNodeSet) throw_error("Function has to be applied to node set"); 
 
			return new (alloc_node()) XPathAstNode(argc == 0 ? Type0 : Type1, XPathTypeString, args[0]); 
		} 
 
		XPathAstNode* ParseFunction(const XPathLexerString& Name, size_t argc, XPathAstNode* args[2]) 
		{ 
			switch (Name.begin[0]) 
			{ 
			case 'b': 
				if (Name == XML_TEXT("boolean") && argc == 1) 
					return new (alloc_node()) XPathAstNode(ast_func_boolean, XPathTypeBoolean, args[0]); 
					 
				break; 
			 
			case 'c': 
				if (Name == XML_TEXT("count") && argc == 1) 
				{ 
					if (args[0]->retType() != XPathTypeNodeSet) throw_error("Function has to be applied to node set"); 
					return new (alloc_node()) XPathAstNode(ast_func_count, XPathTypeNumber, args[0]); 
				} 
				else if (Name == XML_TEXT("contains") && argc == 2) 
					return new (alloc_node()) XPathAstNode(ast_func_contains, XPathTypeString, args[0], args[1]); 
				else if (Name == XML_TEXT("concat") && argc >= 2) 
					return new (alloc_node()) XPathAstNode(ast_func_concat, XPathTypeString, args[0], args[1]); 
				else if (Name == XML_TEXT("ceiling") && argc == 1) 
					return new (alloc_node()) XPathAstNode(ast_func_ceiling, XPathTypeNumber, args[0]); 
					 
				break; 
			 
			case 'f': 
				if (Name == XML_TEXT("false") && argc == 0) 
					return new (alloc_node()) XPathAstNode(ast_func_false, XPathTypeBoolean); 
				else if (Name == XML_TEXT("floor") && argc == 1) 
					return new (alloc_node()) XPathAstNode(ast_func_floor, XPathTypeNumber, args[0]); 
					 
				break; 
			 
			case 'i': 
				if (Name == XML_TEXT("id") && argc == 1) 
					return new (alloc_node()) XPathAstNode(ast_func_id, XPathTypeNodeSet, args[0]); 
					 
				break; 
			 
			case 'l': 
				if (Name == XML_TEXT("last") && argc == 0) 
					return new (alloc_node()) XPathAstNode(ast_func_last, XPathTypeNumber); 
				else if (Name == XML_TEXT("lang") && argc == 1) 
					return new (alloc_node()) XPathAstNode(ast_func_lang, XPathTypeBoolean, args[0]); 
				else if (Name == XML_TEXT("local-Name") && argc <= 1) 
					return ParseFunctionHelper(ast_func_local_Name_0, ast_func_local_Name_1, argc, args); 
			 
				break; 
			 
			case 'n': 
				if (Name == XML_TEXT("Name") && argc <= 1) 
					return ParseFunctionHelper(ast_func_Name_0, ast_func_Name_1, argc, args); 
				else if (Name == XML_TEXT("namespace-uri") && argc <= 1) 
					return ParseFunctionHelper(ast_func_namespace_uri_0, ast_func_namespace_uri_1, argc, args); 
				else if (Name == XML_TEXT("normalize-space") && argc <= 1) 
					return new (alloc_node()) XPathAstNode(argc == 0 ? ast_func_normalize_space_0 : ast_func_normalize_space_1, XPathTypeString, args[0], args[1]); 
				else if (Name == XML_TEXT("not") && argc == 1) 
					return new (alloc_node()) XPathAstNode(ast_func_not, XPathTypeBoolean, args[0]); 
				else if (Name == XML_TEXT("number") && argc <= 1) 
					return new (alloc_node()) XPathAstNode(argc == 0 ? ast_func_number_0 : ast_func_number_1, XPathTypeNumber, args[0]); 
			 
				break; 
			 
			case 'p': 
				if (Name == XML_TEXT("position") && argc == 0) 
					return new (alloc_node()) XPathAstNode(ast_func_position, XPathTypeNumber); 
				 
				break; 
			 
			case 'r': 
				if (Name == XML_TEXT("round") && argc == 1) 
					return new (alloc_node()) XPathAstNode(ast_func_round, XPathTypeNumber, args[0]); 
 
				break; 
			 
			case 's': 
				if (Name == XML_TEXT("string") && argc <= 1) 
					return new (alloc_node()) XPathAstNode(argc == 0 ? ast_func_string_0 : ast_func_string_1, XPathTypeString, args[0]); 
				else if (Name == XML_TEXT("string-length") && argc <= 1) 
					return new (alloc_node()) XPathAstNode(argc == 0 ? ast_func_string_length_0 : ast_func_string_length_1, XPathTypeString, args[0]); 
				else if (Name == XML_TEXT("starts-with") && argc == 2) 
					return new (alloc_node()) XPathAstNode(ast_func_starts_with, XPathTypeBoolean, args[0], args[1]); 
				else if (Name == XML_TEXT("substring-before") && argc == 2) 
					return new (alloc_node()) XPathAstNode(ast_func_substring_before, XPathTypeString, args[0], args[1]); 
				else if (Name == XML_TEXT("substring-after") && argc == 2) 
					return new (alloc_node()) XPathAstNode(ast_func_substring_after, XPathTypeString, args[0], args[1]); 
				else if (Name == XML_TEXT("substring") && (argc == 2 || argc == 3)) 
					return new (alloc_node()) XPathAstNode(argc == 2 ? ast_func_substring_2 : ast_func_substring_3, XPathTypeString, args[0], args[1]); 
				else if (Name == XML_TEXT("sum") && argc == 1) 
				{ 
					if (args[0]->retType() != XPathTypeNodeSet) throw_error("Function has to be applied to node set"); 
					return new (alloc_node()) XPathAstNode(ast_func_sum, XPathTypeNumber, args[0]); 
				} 
 
				break; 
			 
			case 't': 
				if (Name == XML_TEXT("translate") && argc == 3) 
					return new (alloc_node()) XPathAstNode(ast_func_translate, XPathTypeString, args[0], args[1]); 
				else if (Name == XML_TEXT("true") && argc == 0) 
					return new (alloc_node()) XPathAstNode(ast_func_true, XPathTypeBoolean); 
					 
				break; 
			} 
 
			throw_error("Unrecognized function or wrong parameter count"); 
 
			return 0; 
		} 
 
		axis_t ParseAxisName(const XPathLexerString& Name, bool& specified) 
		{ 
			specified = true; 
 
			switch (Name.begin[0]) 
			{ 
			case 'a': 
				if (Name == XML_TEXT("ancestor")) 
					return axis_ancestor; 
				else if (Name == XML_TEXT("ancestor-or-self")) 
					return axis_ancestor_or_self; 
				else if (Name == XML_TEXT("GetAttribute")) 
					return axis_attribute; 
				 
				break; 
			 
			case 'c': 
				if (Name == XML_TEXT("GetChild")) 
					return axis_GetChild; 
				 
				break; 
			 
			case 'd': 
				if (Name == XML_TEXT("descendant")) 
					return axis_descendant; 
				else if (Name == XML_TEXT("descendant-or-self")) 
					return axis_descendant_or_self; 
				 
				break; 
			 
			case 'f': 
				if (Name == XML_TEXT("following")) 
					return axis_following; 
				else if (Name == XML_TEXT("following-sibling")) 
					return axis_following_sibling; 
				 
				break; 
			 
			case 'n': 
				if (Name == XML_TEXT("namespace")) 
					return axis_namespace; 
				 
				break; 
			 
			case 'p': 
				if (Name == XML_TEXT("GetParent")) 
					return axis_GetParent; 
				else if (Name == XML_TEXT("preceding")) 
					return axis_preceding; 
				else if (Name == XML_TEXT("preceding-sibling")) 
					return axis_preceding_sibling; 
				 
				break; 
			 
			case 's': 
				if (Name == XML_TEXT("self")) 
					return axis_self; 
				 
				break; 
			} 
 
			specified = false; 
			return axis_GetChild; 
		} 
 
		nodetest_t ParseNodeTest_type(const XPathLexerString& Name) 
		{ 
			switch (Name.begin[0]) 
			{ 
			case 'c': 
				if (Name == XML_TEXT("comment")) 
					return nodetest_type_comment; 
 
				break; 
 
			case 'n': 
				if (Name == XML_TEXT("node")) 
					return nodetest_type_node; 
 
				break; 
 
			case 'p': 
				if (Name == XML_TEXT("processing-instruction")) 
					return nodetest_type_pi; 
 
				break; 
 
			case 't': 
				if (Name == XML_TEXT("text")) 
					return nodetest_type_text; 
 
				break; 
			} 
 
			return nodetest_none; 
		} 
 
		// PrimaryExpr ::= VariableReference | '(' Expr ')' | Literal | Number | FunctionCall 
		XPathAstNode* ParsePrimaryExpression() 
		{ 
			switch (_lexer.current()) 
			{ 
			case lex_var_ref: 
			{ 
				XPathLexerString Name = _lexer.contents(); 
 
				if (!_variables) 
					throw_error("Unknown variable: variable set is not provided"); 
 
				XPathVariable* var = GetVariable(_variables, Name.begin, Name.end); 
 
				if (!var) 
					throw_error("Unknown variable: variable set does not contain the given Name"); 
 
				_lexer.next(); 
 
				return new (alloc_node()) XPathAstNode(ast_variable, var->Type(), var); 
			} 
 
			case lex_open_brace: 
			{ 
				_lexer.next(); 
 
				XPathAstNode* n = ParseExpression(); 
 
				if (_lexer.current() != lex_close_brace) 
					throw_error("Unmatched braces"); 
 
				_lexer.next(); 
 
				return n; 
			} 
 
			case lex_quoted_string: 
			{ 
				const char_t* Value = alloc_string(_lexer.contents()); 
 
				XPathAstNode* n = new (alloc_node()) XPathAstNode(ast_string_constant, XPathTypeString, Value); 
				_lexer.next(); 
 
				return n; 
			} 
 
			case lex_number: 
			{ 
				double Value = 0; 
 
				if (!convert_Stringo_number(_lexer.contents().begin, _lexer.contents().end, &Value)) 
					throw_error_oom(); 
 
				XPathAstNode* n = new (alloc_node()) XPathAstNode(ast_number_constant, XPathTypeNumber, Value); 
				_lexer.next(); 
 
				return n; 
			} 
 
			case lex_string: 
			{ 
				XPathAstNode* args[2] = {0}; 
				size_t argc = 0; 
				 
				XPathLexerString function = _lexer.contents(); 
				_lexer.next(); 
				 
				XPathAstNode* LastArg = 0; 
				 
				if (_lexer.current() != lex_open_brace) 
					throw_error("Unrecognized function call"); 
				_lexer.next(); 
 
				if (_lexer.current() != lex_close_brace) 
					args[argc++] = ParseExpression(); 
 
				while (_lexer.current() != lex_close_brace) 
				{ 
					if (_lexer.current() != lex_comma) 
						throw_error("No comma between function arguments"); 
					_lexer.next(); 
					 
					XPathAstNode* n = ParseExpression(); 
					 
					if (argc < 2) args[argc] = n; 
					else LastArg->SetNext(n); 
 
					argc++; 
					LastArg = n; 
				} 
				 
				_lexer.next(); 
 
				return ParseFunction(function, argc, args); 
			} 
 
			default: 
				throw_error("Unrecognizable primary expression"); 
 
				return 0; 
			} 
		} 
		 
		// FilterExpr ::= PrimaryExpr | FilterExpr Predicate 
		// Predicate ::= '[' PredicateExpr ']' 
		// PredicateExpr ::= Expr 
		XPathAstNode* ParseFilterExpression() 
		{ 
			XPathAstNode* n = ParsePrimaryExpression(); 
 
			while (_lexer.current() == lex_open_square_brace) 
			{ 
				_lexer.next(); 
 
				XPathAstNode* expr = ParseExpression(); 
 
				if (n->retType() != XPathTypeNodeSet) throw_error("Predicate has to be applied to node set"); 
 
				bool posinv = expr->retType() != XPathTypeNumber && expr->is_posinv(); 
 
				n = new (alloc_node()) XPathAstNode(posinv ? ast_filter_posinv : ast_filter, XPathTypeNodeSet, n, expr); 
 
				if (_lexer.current() != lex_close_square_brace) 
					throw_error("Unmatched square brace"); 
			 
				_lexer.next(); 
			} 
			 
			return n; 
		} 
		 
		// Step ::= AxisSpecifier NodeTest Predicate* | AbbreviatedStep 
		// AxisSpecifier ::= AxisName '::' | '@'? 
		// NodeTest ::= NameTest | NodeType '(' ')' | 'processing-instruction' '(' Literal ')' 
		// NameTest ::= '*' | NCName ':' '*' | QName 
		// AbbreviatedStep ::= '.' | '..' 
		XPathAstNode* ParseStep(XPathAstNode* set) 
		{ 
			if (set && set->retType() != XPathTypeNodeSet) 
				throw_error("Step has to be applied to node set"); 
 
			bool axis_specified = false; 
			axis_t axis = axis_GetChild; // implied GetChild axis 
 
			if (_lexer.current() == lex_axis_attribute) 
			{ 
				axis = axis_attribute; 
				axis_specified = true; 
				 
				_lexer.next(); 
			} 
			else if (_lexer.current() == lex_dot) 
			{ 
				_lexer.next(); 
				 
				return new (alloc_node()) XPathAstNode(ast_step, set, axis_self, nodetest_type_node, 0); 
			} 
			else if (_lexer.current() == lex_double_dot) 
			{ 
				_lexer.next(); 
				 
				return new (alloc_node()) XPathAstNode(ast_step, set, axis_GetParent, nodetest_type_node, 0); 
			} 
		 
			nodetest_t nt_type = nodetest_none; 
			XPathLexerString nt_Name; 
			 
			if (_lexer.current() == lex_string) 
			{ 
				// node Name test 
				nt_Name = _lexer.contents(); 
				_lexer.next(); 
 
				// was it an axis Name? 
				if (_lexer.current() == lex_double_colon) 
				{ 
					// parse axis Name 
					if (axis_specified) throw_error("Two axis specifiers in one step"); 
 
					axis = ParseAxisName(nt_Name, axis_specified); 
 
					if (!axis_specified) throw_error("Unknown axis"); 
 
					// read actual node test 
					_lexer.next(); 
 
					if (_lexer.current() == lex_multiply) 
					{ 
						nt_type = nodetest_all; 
						nt_Name = XPathLexerString(); 
						_lexer.next(); 
					} 
					else if (_lexer.current() == lex_string) 
					{ 
						nt_Name = _lexer.contents(); 
						_lexer.next(); 
					} 
					else throw_error("Unrecognized node test"); 
				} 
				 
				if (nt_type == nodetest_none) 
				{ 
					// node Type test or processing-instruction 
					if (_lexer.current() == lex_open_brace) 
					{ 
						_lexer.next(); 
						 
						if (_lexer.current() == lex_close_brace) 
						{ 
							_lexer.next(); 
 
							nt_type = ParseNodeTest_type(nt_Name); 
 
							if (nt_type == nodetest_none) throw_error("Unrecognized node Type"); 
							 
							nt_Name = XPathLexerString(); 
						} 
						else if (nt_Name == XML_TEXT("processing-instruction")) 
						{ 
							if (_lexer.current() != lex_quoted_string) 
								throw_error("Only literals are allowed as arguments to processing-instruction()"); 
						 
							nt_type = nodetest_pi; 
							nt_Name = _lexer.contents(); 
							_lexer.next(); 
							 
							if (_lexer.current() != lex_close_brace) 
								throw_error("Unmatched brace near processing-instruction()"); 
							_lexer.next(); 
						} 
						else 
							throw_error("Unmatched brace near node Type test"); 
 
					} 
					// QName or NCName:* 
					else 
					{ 
						if (nt_Name.end - nt_Name.begin > 2 && nt_Name.end[-2] == ':' && nt_Name.end[-1] == '*') // NCName:* 
						{ 
							nt_Name.end--; // erase * 
							 
							nt_type = nodetest_all_in_namespace; 
						} 
						else nt_type = nodetest_Name; 
					} 
				} 
			} 
			else if (_lexer.current() == lex_multiply) 
			{ 
				nt_type = nodetest_all; 
				_lexer.next(); 
			} 
			else throw_error("Unrecognized node test"); 
			 
			XPathAstNode* n = new (alloc_node()) XPathAstNode(ast_step, set, axis, nt_type, alloc_string(nt_Name)); 
			 
			XPathAstNode* last = 0; 
			 
			while (_lexer.current() == lex_open_square_brace) 
			{ 
				_lexer.next(); 
				 
				XPathAstNode* expr = ParseExpression(); 
 
				XPathAstNode* pred = new (alloc_node()) XPathAstNode(ast_predicate, XPathTypeNodeSet, expr); 
				 
				if (_lexer.current() != lex_close_square_brace) 
					throw_error("Unmatched square brace"); 
				_lexer.next(); 
				 
				if (last) last->SetNext(pred); 
				else n->SetRight(pred); 
				 
				last = pred; 
			} 
			 
			return n; 
		} 
		 
		// RelativeLocationPath ::= Step | RelativeLocationPath '/' Step | RelativeLocationPath '//' Step 
		XPathAstNode* ParseRelativeLocation_Path(XPathAstNode* set) 
		{ 
			XPathAstNode* n = ParseStep(set); 
			 
			while (_lexer.current() == lex_slash || _lexer.current() == lex_double_slash) 
			{ 
				lexeme_t l = _lexer.current(); 
				_lexer.next(); 
 
				if (l == lex_double_slash) 
					n = new (alloc_node()) XPathAstNode(ast_step, n, axis_descendant_or_self, nodetest_type_node, 0); 
				 
				n = ParseStep(n); 
			} 
			 
			return n; 
		} 
		 
		// LocationPath ::= RelativeLocationPath | AbsoluteLocationPath 
		// AbsoluteLocationPath ::= '/' RelativeLocationPath? | '//' RelativeLocationPath 
		XPathAstNode* ParseLocationPath() 
		{ 
			if (_lexer.current() == lex_slash) 
			{ 
				_lexer.next(); 
				 
				XPathAstNode* n = new (alloc_node()) XPathAstNode(ast_step_GetRoot, XPathTypeNodeSet); 
 
				// relative location Path can start from axis_attribute, dot, double_dot, multiply and string lexemes; any other lexeme means standalone GetRoot Path 
				lexeme_t l = _lexer.current(); 
 
				if (l == lex_string || l == lex_axis_attribute || l == lex_dot || l == lex_double_dot || l == lex_multiply) 
					return ParseRelativeLocation_Path(n); 
				else 
					return n; 
			} 
			else if (_lexer.current() == lex_double_slash) 
			{ 
				_lexer.next(); 
				 
				XPathAstNode* n = new (alloc_node()) XPathAstNode(ast_step_GetRoot, XPathTypeNodeSet); 
				n = new (alloc_node()) XPathAstNode(ast_step, n, axis_descendant_or_self, nodetest_type_node, 0); 
				 
				return ParseRelativeLocation_Path(n); 
			} 
 
			// else clause moved outside of if because of bogus warning 'control may reach end of non-void function being inlined' in gcc 4.0.1 
			return ParseRelativeLocation_Path(0); 
		} 
		 
		// PathExpr ::= LocationPath 
		//				| FilterExpr 
		//				| FilterExpr '/' RelativeLocationPath 
		//				| FilterExpr '//' RelativeLocationPath 
		XPathAstNode* ParsePathExpression() 
		{ 
			// Clarification. 
			// PathExpr begins with either LocationPath or FilterExpr. 
			// FilterExpr begins with PrimaryExpr 
			// PrimaryExpr begins with '$' in case of it being a variable reference, 
			// '(' in case of it being an expression, string literal, number constant or 
			// function call. 
 
			if (_lexer.current() == lex_var_ref || _lexer.current() == lex_open_brace ||  
				_lexer.current() == lex_quoted_string || _lexer.current() == lex_number || 
				_lexer.current() == lex_string) 
			{ 
				if (_lexer.current() == lex_string) 
				{ 
					// This is either a function call, or not - if not, we shall proceed with location Path 
					const char_t* state = _lexer.state(); 
					 
					while (IS_CHARTYPE(*state, ct_space)) ++state; 
					 
					if (*state != '(') return ParseLocationPath(); 
 
					// This looks like a function call; however this still can be a node-test. Check it. 
					if (ParseNodeTest_type(_lexer.contents()) != nodetest_none) return ParseLocationPath(); 
				} 
				 
				XPathAstNode* n = ParseFilterExpression(); 
 
				if (_lexer.current() == lex_slash || _lexer.current() == lex_double_slash) 
				{ 
					lexeme_t l = _lexer.current(); 
					_lexer.next(); 
					 
					if (l == lex_double_slash) 
					{ 
						if (n->retType() != XPathTypeNodeSet) throw_error("Step has to be applied to node set"); 
 
						n = new (alloc_node()) XPathAstNode(ast_step, n, axis_descendant_or_self, nodetest_type_node, 0); 
					} 
	 
					// select from location Path 
					return ParseRelativeLocation_Path(n); 
				} 
 
				return n; 
			} 
			else return ParseLocationPath(); 
		} 
 
		// UnionExpr ::= PathExpr | UnionExpr '|' PathExpr 
		XPathAstNode* ParseUnionExpression() 
		{ 
			XPathAstNode* n = ParsePathExpression(); 
 
			while (_lexer.current() == lex_union) 
			{ 
				_lexer.next(); 
 
				XPathAstNode* expr = ParseUnionExpression(); 
 
				if (n->retType() != XPathTypeNodeSet || expr->retType() != XPathTypeNodeSet) 
					throw_error("Union operator has to be applied to node sets"); 
 
				n = new (alloc_node()) XPathAstNode(ast_op_union, XPathTypeNodeSet, n, expr); 
			} 
 
			return n; 
		} 
 
		// UnaryExpr ::= UnionExpr | '-' UnaryExpr 
		XPathAstNode* ParseUnaryExpression() 
		{ 
			if (_lexer.current() == lex_minus) 
			{ 
				_lexer.next(); 
 
				XPathAstNode* expr = ParseUnaryExpression(); 
 
				return new (alloc_node()) XPathAstNode(ast_op_negate, XPathTypeNumber, expr); 
			} 
			else return ParseUnionExpression(); 
		} 
		 
		// MultiplicativeExpr ::= UnaryExpr 
		//						  | MultiplicativeExpr '*' UnaryExpr 
		//						  | MultiplicativeExpr 'div' UnaryExpr 
		//						  | MultiplicativeExpr 'mod' UnaryExpr 
		XPathAstNode* ParseMultiplicativeExpression() 
		{ 
			XPathAstNode* n = ParseUnaryExpression(); 
 
			while (_lexer.current() == lex_multiply || (_lexer.current() == lex_string && 
				   (_lexer.contents() == XML_TEXT("mod") || _lexer.contents() == XML_TEXT("div")))) 
			{ 
				ast_type_t op = _lexer.current() == lex_multiply ? ast_op_multiply : 
					_lexer.contents().begin[0] == 'd' ? ast_op_divide : ast_op_mod; 
				_lexer.next(); 
 
				XPathAstNode* expr = ParseUnaryExpression(); 
 
				n = new (alloc_node()) XPathAstNode(op, XPathTypeNumber, n, expr); 
			} 
 
			return n; 
		} 
 
		// AdditiveExpr ::= MultiplicativeExpr 
		//					| AdditiveExpr '+' MultiplicativeExpr 
		//					| AdditiveExpr '-' MultiplicativeExpr 
		XPathAstNode* ParseAdditiveExpression() 
		{ 
			XPathAstNode* n = ParseMultiplicativeExpression(); 
 
			while (_lexer.current() == lex_plus || _lexer.current() == lex_minus) 
			{ 
				lexeme_t l = _lexer.current(); 
 
				_lexer.next(); 
 
				XPathAstNode* expr = ParseMultiplicativeExpression(); 
 
				n = new (alloc_node()) XPathAstNode(l == lex_plus ? ast_op_add : ast_op_subtract, XPathTypeNumber, n, expr); 
			} 
 
			return n; 
		} 
 
		// RelationalExpr ::= AdditiveExpr 
		//					  | RelationalExpr '<' AdditiveExpr 
		//					  | RelationalExpr '>' AdditiveExpr 
		//					  | RelationalExpr '<=' AdditiveExpr 
		//					  | RelationalExpr '>=' AdditiveExpr 
		XPathAstNode* ParseRelationalExpression() 
		{ 
			XPathAstNode* n = ParseAdditiveExpression(); 
 
			while (_lexer.current() == lex_less || _lexer.current() == lex_less_or_equal ||  
				   _lexer.current() == lex_greater || _lexer.current() == lex_greater_or_equal) 
			{ 
				lexeme_t l = _lexer.current(); 
				_lexer.next(); 
 
				XPathAstNode* expr = ParseAdditiveExpression(); 
 
				n = new (alloc_node()) XPathAstNode(l == lex_less ? ast_op_less : l == lex_greater ? ast_op_greater : 
								l == lex_less_or_equal ? ast_op_less_or_equal : ast_op_greater_or_equal, XPathTypeBoolean, n, expr); 
			} 
 
			return n; 
		} 
		 
		// EqualityExpr ::= RelationalExpr 
		//					| EqualityExpr '=' RelationalExpr 
		//					| EqualityExpr '!=' RelationalExpr 
		XPathAstNode* ParseEqualityExpression() 
		{ 
			XPathAstNode* n = ParseRelationalExpression(); 
 
			while (_lexer.current() == lex_equal || _lexer.current() == lex_not_equal) 
			{ 
				lexeme_t l = _lexer.current(); 
 
				_lexer.next(); 
 
				XPathAstNode* expr = ParseRelationalExpression(); 
 
				n = new (alloc_node()) XPathAstNode(l == lex_equal ? ast_op_equal : ast_op_not_equal, XPathTypeBoolean, n, expr); 
			} 
 
			return n; 
		} 
		 
		// AndExpr ::= EqualityExpr | AndExpr 'and' EqualityExpr 
		XPathAstNode* ParseAndExpression() 
		{ 
			XPathAstNode* n = ParseEqualityExpression(); 
 
			while (_lexer.current() == lex_string && _lexer.contents() == XML_TEXT("and")) 
			{ 
				_lexer.next(); 
 
				XPathAstNode* expr = ParseEqualityExpression(); 
 
				n = new (alloc_node()) XPathAstNode(ast_op_and, XPathTypeBoolean, n, expr); 
			} 
 
			return n; 
		} 
 
		// OrExpr ::= AndExpr | OrExpr 'or' AndExpr 
		XPathAstNode* ParseOrExpression() 
		{ 
			XPathAstNode* n = ParseAndExpression(); 
 
			while (_lexer.current() == lex_string && _lexer.contents() == XML_TEXT("or")) 
			{ 
				_lexer.next(); 
 
				XPathAstNode* expr = ParseAndExpression(); 
 
				n = new (alloc_node()) XPathAstNode(ast_op_or, XPathTypeBoolean, n, expr); 
			} 
 
			return n; 
		} 
		 
		// Expr ::= OrExpr 
		XPathAstNode* ParseExpression() 
		{ 
			return ParseOrExpression(); 
		} 
 
		XPathParser(const char_t* query, XPathVariableSet* variables, XPathAllocator* alloc, XPathParseResult* result): _alloc(alloc), _lexer(query), _query(query), _variables(variables), _result(result) 
		{ 
		} 
 
		XPathAstNode* parse() 
		{ 
			XPathAstNode* result = ParseExpression(); 
			 
			if (_lexer.current() != lex_eof) 
			{ 
				// there are still unparsed tokens left, error 
				throw_error("Incorrect query"); 
			} 
			 
			return result; 
		} 
 
		static XPathAstNode* parse(const char_t* query, XPathVariableSet* variables, XPathAllocator* alloc, XPathParseResult* result) 
		{ 
			XPathParser parser(query, variables, alloc, result); 
 
		#ifdef XML_NO_EXCEPTIONS 
			int error = setjmp(parser._error_handler); 
 
			return (error == 0) ? parser.parse() : 0; 
		#else 
			return parser.parse(); 
		#endif 
		} 
	}; 
 
	struct XPathQueryImpl 
	{ 
		static XPathQueryImpl* create() 
		{ 
			void* memory = global_allocate(sizeof(XPathQueryImpl)); 
 
			return new (memory) XPathQueryImpl(); 
		} 
 
		static void destroy(void* ptr) 
		{ 
			if (!ptr) return; 
			 
			// free all allocated pages 
			static_cast<XPathQueryImpl*>(ptr)->alloc.release(); 
 
			// free allocator memory (with the first page) 
			global_deallocate(ptr); 
		} 
 
		XPathQueryImpl(): GetRoot(0), alloc(&block) 
		{ 
			block.next = 0; 
		} 
 
		XPathAstNode* GetRoot; 
		XPathAllocator alloc; 
		XPathMemoryBlock block; 
	}; 
 
	XPathString evaluate_string_impl(XPathQueryImpl* impl, const XPathNode& n, XPathStackData& sd) 
	{ 
		if (!impl) return XPathString(); 
 
	#ifdef XML_NO_EXCEPTIONS 
		if (setjmp(sd.error_handler)) return XPathString(); 
	#endif 
 
		XPathContext c(n, 1, 1); 
 
		return impl->GetRoot->eval_string(c, sd.stack); 
	} 
} 
 

namespace phys
{ namespace xml
{ 
#ifndef XML_NO_EXCEPTIONS 
	XPathException::XPathException(const XPathParseResult& result): _result(result) 
	{ 
		assert(result.error); 
	} 
	 
	const char* XPathException::what() const throw() 
	{ 
		return _result.error; 
	} 
 
	const XPathParseResult& XPathException::result() const 
	{ 
		return _result; 
	} 
#endif 
	 
	XPathNode::XPathNode() 
	{ 
	} 
		 
	XPathNode::XPathNode(const Node& node): _node(node) 
	{ 
	} 
		 
	XPathNode::XPathNode(const Attribute& GetAttribute, const Node& GetParent): _node(GetAttribute ? GetParent : Node()), _attribute(GetAttribute) 
	{ 
	} 
 
	Node XPathNode::node() const 
	{ 
		return _attribute ? Node() : _node; 
	} 
		 
	Attribute XPathNode::GetAttribute() const 
	{ 
		return _attribute; 
	} 
	 
	Node XPathNode::GetParent() const 
	{ 
		return _attribute ? _node : _node.GetParent(); 
	} 
 
	XPathNode::operator XPathNode::unspecified_bool_type() const 
	{ 
		return (_node || _attribute) ? &XPathNode::_node : 0; 
	} 
	 
	bool XPathNode::operator!() const 
	{ 
		return !(_node || _attribute); 
	} 
 
	bool XPathNode::operator==(const XPathNode& n) const 
	{ 
		return _node == n._node && _attribute == n._attribute; 
	} 
	 
	bool XPathNode::operator!=(const XPathNode& n) const 
	{ 
		return _node != n._node || _attribute != n._attribute; 
	} 
 
#ifdef __BORLANDC__ 
	bool operator&&(const XPathNode& lhs, bool rhs) 
	{ 
		return (bool)lhs && rhs; 
	} 
 
	bool operator||(const XPathNode& lhs, bool rhs) 
	{ 
		return (bool)lhs || rhs; 
	} 
#endif 
 
	void XPathNodeSet::_assign(const_iterator begin, const_iterator end) 
	{ 
		assert(begin <= end); 
 
		size_t size = static_cast<size_t>(end - begin); 
 
		if (size <= 1) 
		{ 
			// deallocate old buffer 
			if (_begin != &_storage) global_deallocate(_begin); 
 
			// use internal buffer 
			if (begin != end) _storage = *begin; 
 
			_begin = &_storage; 
			_end = &_storage + size; 
		} 
		else 
		{ 
			// make heap copy 
			XPathNode* storage = static_cast<XPathNode*>(global_allocate(size * sizeof(XPathNode))); 
 
			if (!storage) 
			{ 
			#ifdef XML_NO_EXCEPTIONS 
				return; 
			#else 
				throw std::bad_alloc(); 
			#endif 
			} 
 
			memcpy(storage, begin, size * sizeof(XPathNode)); 
			 
			// deallocate old buffer 
			if (_begin != &_storage) global_deallocate(_begin); 
 
			// finalize 
			_begin = storage; 
			_end = storage + size; 
		} 
	} 
 
	XPathNodeSet::XPathNodeSet(): _type(Type_unsorted), _begin(&_storage), _end(&_storage) 
	{ 
	} 
 
	XPathNodeSet::XPathNodeSet(const_iterator begin, const_iterator end, Type_t Type): _type(Type), _begin(&_storage), _end(&_storage) 
	{ 
		_assign(begin, end); 
	} 
 
	XPathNodeSet::~XPathNodeSet() 
	{ 
		if (_begin != &_storage) global_deallocate(_begin); 
	} 
		 
	XPathNodeSet::XPathNodeSet(const XPathNodeSet& ns): _type(ns._type), _begin(&_storage), _end(&_storage) 
	{ 
		_assign(ns._begin, ns._end); 
	} 
	 
	XPathNodeSet& XPathNodeSet::operator=(const XPathNodeSet& ns) 
	{ 
		if (this == &ns) return *this; 
		 
		_type = ns._type; 
		_assign(ns._begin, ns._end); 
 
		return *this; 
	} 
 
	XPathNodeSet::Type_t XPathNodeSet::Type() const 
	{ 
		return _type; 
	} 
		 
	size_t XPathNodeSet::size() const 
	{ 
		return _end - _begin; 
	} 
		 
	bool XPathNodeSet::Empty() const 
	{ 
		return _begin == _end; 
	} 
		 
	const XPathNode& XPathNodeSet::operator[](size_t index) const 
	{ 
		assert(index < size()); 
		return _begin[index]; 
	} 
 
	XPathNodeSet::const_iterator XPathNodeSet::begin() const 
	{ 
		return _begin; 
	} 
		 
	XPathNodeSet::const_iterator XPathNodeSet::end() const 
	{ 
		return _end; 
	} 
	 
	void XPathNodeSet::sort(bool reverse) 
	{ 
		_type = XPathSort(_begin, _end, _type, reverse); 
	} 
 
	XPathNode XPathNodeSet::first() const 
	{ 
		return XPathFirst(_begin, _end, _type); 
	} 
 
	XPathParseResult::XPathParseResult(): error("Internal error"), Offset(0) 
	{ 
	} 
 
	XPathParseResult::operator bool() const 
	{ 
		return error == 0; 
	} 
	const char* XPathParseResult::Description() const 
	{ 
		return error ? error : "No error"; 
	} 
 
	XPathVariable::XPathVariable() 
	{ 
	} 
 
	const char_t* XPathVariable::Name() const 
	{ 
		switch (_type) 
		{ 
		case XPathTypeNodeSet: 
			return static_cast<const XPathVariableNodeSet*>(this)->Name; 
 
		case XPathTypeNumber: 
			return static_cast<const XPathVariableNumber*>(this)->Name; 
 
		case XPathTypeString: 
			return static_cast<const XPathVariableString*>(this)->Name; 
 
		case XPathTypeBoolean: 
			return static_cast<const XPathVariableBoolean*>(this)->Name; 
 
		default: 
			assert(!"Invalid variable Type"); 
			return 0; 
		} 
	} 
 
	XPathValueType XPathVariable::Type() const 
	{ 
		return _type; 
	} 
 
	bool XPathVariable::GetBoolean() const 
	{ 
		return (_type == XPathTypeBoolean) ? static_cast<const XPathVariableBoolean*>(this)->Value : false; 
	} 
 
	double XPathVariable::GetNumber() const 
	{ 
		return (_type == XPathTypeNumber) ? static_cast<const XPathVariableNumber*>(this)->Value : gen_nan(); 
	} 
 
	const char_t* XPathVariable::GetString() const 
	{ 
		const char_t* Value = (_type == XPathTypeString) ? static_cast<const XPathVariableString*>(this)->Value : 0; 
		return Value ? Value : XML_TEXT(""); 
	} 
 
	const XPathNodeSet& XPathVariable::GetNodeSet() const 
	{ 
		return (_type == XPathTypeNodeSet) ? static_cast<const XPathVariableNodeSet*>(this)->Value : dummy_NodeSet; 
	} 
 
	bool XPathVariable::Set(bool Value) 
	{ 
		if (_type != XPathTypeBoolean) return false; 
 
		static_cast<XPathVariableBoolean*>(this)->Value = Value; 
		return true; 
	} 
 
	bool XPathVariable::Set(double Value) 
	{ 
		if (_type != XPathTypeNumber) return false; 
 
		static_cast<XPathVariableNumber*>(this)->Value = Value; 
		return true; 
	} 
 
	bool XPathVariable::Set(const char_t* Value) 
	{ 
		if (_type != XPathTypeString) return false; 
 
		XPathVariableString* var = static_cast<XPathVariableString*>(this); 
 
		// duplicate string 
		size_t size = (strlength(Value) + 1) * sizeof(char_t); 
 
		char_t* copy = static_cast<char_t*>(global_allocate(size)); 
		if (!copy) return false; 
 
		memcpy(copy, Value, size); 
 
		// replace old string 
		if (var->Value) global_deallocate(var->Value); 
		var->Value = copy; 
 
		return true; 
	} 
 
	bool XPathVariable::Set(const XPathNodeSet& Value) 
	{ 
		if (_type != XPathTypeNodeSet) return false; 
 
		static_cast<XPathVariableNodeSet*>(this)->Value = Value; 
		return true; 
	} 
 
	XPathVariableSet::XPathVariableSet() 
	{ 
		for (size_t i = 0; i < sizeof(_data) / sizeof(_data[0]); ++i) _data[i] = 0; 
	} 
 
	XPathVariableSet::~XPathVariableSet() 
	{ 
		for (size_t i = 0; i < sizeof(_data) / sizeof(_data[0]); ++i) 
		{ 
			XPathVariable* var = _data[i]; 
 
			while (var) 
			{ 
				XPathVariable* next = var->_next; 
 
				delete_XPathVariable(var->_type, var); 
 
				var = next; 
			} 
		} 
	} 
 
	XPathVariable* XPathVariableSet::find(const char_t* Name) const 
	{ 
		const size_t hash_size = sizeof(_data) / sizeof(_data[0]); 
		size_t hash = hash_string(Name) % hash_size; 
 
		// look for existing variable 
		for (XPathVariable* var = _data[hash]; var; var = var->_next) 
			if (strequal(var->Name(), Name)) 
				return var; 
 
		return 0; 
	} 
 
	XPathVariable* XPathVariableSet::add(const char_t* Name, XPathValueType Type) 
	{ 
		const size_t hash_size = sizeof(_data) / sizeof(_data[0]); 
		size_t hash = hash_string(Name) % hash_size; 
 
		// look for existing variable 
		for (XPathVariable* var = _data[hash]; var; var = var->_next) 
			if (strequal(var->Name(), Name)) 
				return var->Type() == Type ? var : 0; 
 
		// add new variable 
		XPathVariable* result = new_XPathVariable(Type, Name); 
 
		if (result) 
		{ 
			result->_type = Type; 
			result->_next = _data[hash]; 
 
			_data[hash] = result; 
		} 
 
		return result; 
	} 
 
	bool XPathVariableSet::Set(const char_t* Name, bool Value) 
	{ 
		XPathVariable* var = add(Name, XPathTypeBoolean); 
		return var ? var->Set(Value) : false; 
	} 
 
	bool XPathVariableSet::Set(const char_t* Name, double Value) 
	{ 
		XPathVariable* var = add(Name, XPathTypeNumber); 
		return var ? var->Set(Value) : false; 
	} 
 
	bool XPathVariableSet::Set(const char_t* Name, const char_t* Value) 
	{ 
		XPathVariable* var = add(Name, XPathTypeString); 
		return var ? var->Set(Value) : false; 
	} 
 
	bool XPathVariableSet::Set(const char_t* Name, const XPathNodeSet& Value) 
	{ 
		XPathVariable* var = add(Name, XPathTypeNodeSet); 
		return var ? var->Set(Value) : false; 
	} 
 
	XPathVariable* XPathVariableSet::get(const char_t* Name) 
	{ 
		return find(Name); 
	} 
 
	const XPathVariable* XPathVariableSet::get(const char_t* Name) const 
	{ 
		return find(Name); 
	} 
 
	XPathQuery::XPathQuery(const char_t* query, XPathVariableSet* variables): _impl(0) 
	{ 
		XPathQueryImpl* impl = XPathQueryImpl::create(); 
 
		if (!impl) 
		{ 
		#ifdef XML_NO_EXCEPTIONS 
			_result.error = "Out of memory"; 
		#else 
			throw std::bad_alloc(); 
		#endif 
		} 
		else 
		{ 
			buffer_holder impl_holder(impl, XPathQueryImpl::destroy); 
 
			impl->GetRoot = XPathParser::parse(query, variables, &impl->alloc, &_result); 
 
			if (impl->GetRoot) 
			{ 
				_impl = static_cast<XPathQueryImpl*>(impl_holder.release()); 
				_result.error = 0; 
			} 
		} 
	} 
 
	XPathQuery::~XPathQuery() 
	{ 
		XPathQueryImpl::destroy(_impl); 
	} 
 
	XPathValueType XPathQuery::return_type() const 
	{ 
		if (!_impl) return XPathTypeNone; 
 
		return static_cast<XPathQueryImpl*>(_impl)->GetRoot->retType(); 
	} 
 
	bool XPathQuery::evaluate_boolean(const XPathNode& n) const 
	{ 
		if (!_impl) return false; 
		 
		XPathContext c(n, 1, 1); 
		XPathStackData sd; 
 
	#ifdef XML_NO_EXCEPTIONS 
		if (setjmp(sd.error_handler)) return false; 
	#endif 
		 
		return static_cast<XPathQueryImpl*>(_impl)->GetRoot->eval_boolean(c, sd.stack); 
	} 
	 
	double XPathQuery::evaluate_number(const XPathNode& n) const 
	{ 
		if (!_impl) return gen_nan(); 
		 
		XPathContext c(n, 1, 1); 
		XPathStackData sd; 
 
	#ifdef XML_NO_EXCEPTIONS 
		if (setjmp(sd.error_handler)) return gen_nan(); 
	#endif 
 
		return static_cast<XPathQueryImpl*>(_impl)->GetRoot->eval_number(c, sd.stack); 
	} 
 
#ifndef XML_NO_STL 
	String XPathQuery::evaluate_string(const XPathNode& n) const 
	{ 
		XPathStackData sd; 
 
		return evaluate_string_impl(static_cast<XPathQueryImpl*>(_impl), n, sd).c_str(); 
	} 
#endif 
 
	size_t XPathQuery::evaluate_string(char_t* buffer, size_t capacity, const XPathNode& n) const 
	{ 
		XPathStackData sd; 
 
		XPathString r = evaluate_string_impl(static_cast<XPathQueryImpl*>(_impl), n, sd); 
 
		size_t full_size = r.length() + 1; 
		 
		if (capacity > 0) 
		{ 
			size_t size = (full_size < capacity) ? full_size : capacity; 
			assert(size > 0); 
 
			memcpy(buffer, r.c_str(), (size - 1) * sizeof(char_t)); 
			buffer[size - 1] = 0; 
		} 
		 
		return full_size; 
	} 
 
	XPathNodeSet XPathQuery::evaluate_NodeSet(const XPathNode& n) const 
	{ 
		if (!_impl) return XPathNodeSet(); 
 
		XPathAstNode* GetRoot = static_cast<XPathQueryImpl*>(_impl)->GetRoot; 
 
		if (GetRoot->retType() != XPathTypeNodeSet) 
		{ 
		#ifdef XML_NO_EXCEPTIONS 
			return XPathNodeSet(); 
		#else 
			XPathParseResult result; 
			result.error = "Expression does not evaluate to node set"; 
 
			throw XPathException(result); 
		#endif 
		} 
		 
		XPathContext c(n, 1, 1); 
		XPathStackData sd; 
 
	#ifdef XML_NO_EXCEPTIONS 
		if (setjmp(sd.error_handler)) return XPathNodeSet(); 
	#endif 
 
		XPathNodeSet_raw r = GetRoot->eval_NodeSet(c, sd.stack); 
 
		return XPathNodeSet(r.begin(), r.end(), r.Type()); 
	} 
 
	const XPathParseResult& XPathQuery::result() const 
	{ 
		return _result; 
	} 
 
	XPathQuery::operator XPathQuery::unspecified_bool_type() const 
	{ 
		return _impl ? &XPathQuery::_impl : 0; 
	} 
 
	bool XPathQuery::operator!() const 
	{ 
		return !_impl; 
	} 
 
	XPathNode Node::FindSingleNode(const char_t* query, XPathVariableSet* variables) const 
	{ 
		XPathQuery q(query, variables); 
		return FindSingleNode(q); 
	} 
 
	XPathNode Node::FindSingleNode(const XPathQuery& query) const 
	{ 
		XPathNodeSet s = query.evaluate_NodeSet(*this); 
		return s.Empty() ? XPathNode() : s.first(); 
	} 
 
	XPathNodeSet Node::FindNodes(const char_t* query, XPathVariableSet* variables) const 
	{ 
		XPathQuery q(query, variables); 
		return FindNodes(q); 
	} 
 
	XPathNodeSet Node::FindNodes(const XPathQuery& query) const 
	{ 
		return query.evaluate_NodeSet(*this); 
	} 
} 
} // \phys
 
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
#endif // \PHYSXML 
