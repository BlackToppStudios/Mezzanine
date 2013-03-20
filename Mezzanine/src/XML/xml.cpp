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

/// @cond 0

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

#ifndef SOURCE_XML_CPP
#define SOURCE_XML_CPP

#include "XML/xml.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <wchar.h>


#include <math.h>
#include <float.h>


#include <istream>
#include <ostream>
#include <string>


// For placement new
#include <new>

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable: 4127) // conditional expression is constant
#	pragma warning(disable: 4324) // structure was padded due to __declspec(align())
#	pragma warning(disable: 4611) // interaction between '_setjmp' and C++ object destruction is non-portable
#	pragma warning(disable: 4702) // unreachable code
#	pragma warning(disable: 4996) // this function or variable may be unsafe
#	pragma warning(disable: 4793) // function compiled as native: presence of '_setjmp' makes a function unmanaged
#endif

#ifdef __INTEL_COMPILER
#	pragma warning(disable: 177) // function was declared but never referenced
#	pragma warning(disable: 279) // controlling expression is constant
#	pragma warning(disable: 1478 1786) // function was declared "deprecated"
#	pragma warning(disable: 1684) // conversion from pointer to same-sized integral type
#endif

#ifdef __SNC__
// Using diag_push/diag_pop does not disable the warnings inside templates due to a compiler bug
#	pragma diag_suppress=178 // function was declared but never referenced
#	pragma diag_suppress=237 // controlling expression is constant
#endif

// Inlining controls
#if defined(_MSC_VER) && _MSC_VER >= 1300
#	define PUGI__NO_INLINE __declspec(noinline)
#elif defined(__GNUC__)
#	define PUGI__NO_INLINE __attribute__((noinline))
#else
#	define PUGI__NO_INLINE
#endif

// Simple static assertion
#define PUGI__STATIC_ASSERT(cond) { static const char condition_failed[(cond) ? 1 : -1] = {0}; (void)condition_failed[0]; }

// Digital Mars C++ bug workaround for passing char Loaded from memory via stack
#ifdef __DMC__
#	define PUGI__DMC_VOLATILE volatile
#else
#	define PUGI__DMC_VOLATILE
#endif

// In some environments MSVC is a compiler but the CRT lacks certain MSVC-specific features
#if defined(_MSC_VER) && !defined(__S3E__)
#	define PUGI__MSVC_CRT_VERSION _MSC_VER
#endif

#ifdef XML_HEADER_ONLY
#	define PUGI__NS_BEGIN namespace XML { namespace internal {
#	define PUGI__NS_END } }
#	define PUGI__FN inline
#	define PUGI__FN_NO_INLINE inline
#else
#	if defined(_MSC_VER) && _MSC_VER < 1300 // MSVC6 seems to have an amusing bug with anonymous namespaces inside namespaces
#		define PUGI__NS_BEGIN namespace XML { namespace internal {
#		define PUGI__NS_END } }
#	else
#		define PUGI__NS_BEGIN namespace XML { namespace internal { namespace {
#		define PUGI__NS_END } } }
#	endif
#	define PUGI__FN
#	define PUGI__FN_NO_INLINE PUGI__NO_INLINE
#endif

// uintptr_t
#if !defined(_MSC_VER) || _MSC_VER >= 1600
#	include <stdint.h>
#else
#	ifndef _UINTPTR_T_DEFINED
// No native uintptr_t in MSVC6 and in some WinCE versions
typedef size_t uintptr_t;
#define _UINTPTR_T_DEFINED
#	endif
PUGI__NS_BEGIN
	typedef unsigned __int8 uint8_t;
	typedef unsigned __int16 uint16_t;
	typedef unsigned __int32 uint32_t;
PUGI__NS_END
#endif

namespace Mezzanine {
// Memory allocation

PUGI__NS_BEGIN
	PUGI__FN void* default_allocate(size_t size)
	{
		return malloc(size);
	}

	PUGI__FN void default_deallocate(void* ptr)
	{
		free(ptr);
	}

	template <typename T>
	struct MemoryManagement_function_storage
	{
		static AllocationFunction allocate;
		static deAllocationFunction deallocate;
	};

	template <typename T> AllocationFunction MemoryManagement_function_storage<T>::allocate = default_allocate;
	template <typename T> deAllocationFunction MemoryManagement_function_storage<T>::deallocate = default_deallocate;

	typedef MemoryManagement_function_storage<int> Memory;
PUGI__NS_END

// String utilities
PUGI__NS_BEGIN
	// Get string length
    PUGI__FN size_t strlength(const Char8* s)
	{
		assert(s);

		return strlen(s);
	}

	// Compare two strings
    PUGI__FN bool strequal(const Char8* src, const Char8* dst)
	{
		assert(src && dst);

		return strcmp(src, dst) == 0;

	}

	// Compare lhs with [rhs_begin, rhs_end)
    PUGI__FN bool strequalrange(const Char8* lhs, const Char8* rhs, size_t count)
	{
		for (size_t i = 0; i < count; ++i)
			if (lhs[i] != rhs[i])
				return false;

		return lhs[count] == 0;
	}

PUGI__NS_END

// auto_ptr-like buffer holder for exception recovery
PUGI__NS_BEGIN
	struct buffer_holder
	{
		void* data;
		void (*deleter)(void*);

		buffer_holder(void* data_, void (*deleter_)(void*)): data(data_), deleter(deleter_)
		{
		}

		~buffer_holder()
		{
			if (data) deleter(data);
		}

		void* release()
		{
			void* Result = data;
			data = 0;
			return Result;
		}
	};
PUGI__NS_END


PUGI__NS_BEGIN
	static const size_t MemoryPage_size =
	#ifdef XML_MEMORY_PAGE_SIZE
		XML_MEMORY_PAGE_SIZE
	#else
		32768
	#endif
		;

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

			MemoryPage* Result = static_cast<MemoryPage*>(memory);

			Result->allocator = 0;
			Result->memory = 0;
			Result->prev = 0;
			Result->next = 0;
			Result->busy_size = 0;
			Result->freed_size = 0;

			return Result;
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

	struct Allocator	{
		Allocator(MemoryPage* GetRoot): _GetRoot(GetRoot), _busy_size(GetRoot->busy_size)
		{
		}

		MemoryPage* allocate_page(size_t data_size)
		{
			size_t size = offsetof(MemoryPage, data) + data_size;

			// allocate block with some alignment, leaving memory for worst-case padding
			void* memory = Memory::allocate(size + MemoryPage_alignment);
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
			Memory::deallocate(page->memory);
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

        Char8* allocate_string(size_t length)
		{
			// allocate memory for string and header block
            size_t size = sizeof(MemoryString_header) + length * sizeof(Char8);

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

			// round-trip through void* to avoid 'cast increases required alignment of target Type' warning
			// header is guaranteed a pointer-sized alignment, which should be enough for char_t
            return static_cast<Char8*>(static_cast<void*>(header + 1));
		}

        void deallocate_string(Char8* string)
		{
			// this function casts pointers through void* to avoid 'cast increases required alignment of target Type' warnings
			// we're guaranteed the proper (pointer-sized) alignment on the input string if it was allocated via allocate_string

			// get header
			MemoryString_header* header = static_cast<MemoryString_header*>(static_cast<void*>(string)) - 1;

			// deallocate
			size_t page_Offset = offsetof(MemoryPage, data) + header->page_Offset;
			MemoryPage* page = reinterpret_cast<MemoryPage*>(static_cast<void*>(reinterpret_cast<char*>(header) - page_Offset));

			// if full_size == 0 then this string occupies the whole page
			size_t full_size = header->full_size == 0 ? page->busy_size : header->full_size;

			deallocate_memory(header, full_size, page);
		}

		MemoryPage* _GetRoot;
		size_t _busy_size;
	};

	PUGI__FN_NO_INLINE void* Allocator::allocate_memory_oob(size_t size, MemoryPage*& out_page)
	{
		const size_t large_allocation_threshold = MemoryPage_size / 4;

		MemoryPage* page = allocate_page(size <= large_allocation_threshold ? MemoryPage_size : size);
		out_page = page;

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

		return page->data;
	}
PUGI__NS_END

namespace XML
{
	//// A 'Name=Value' XML GetAttribute structure.
	struct AttributeStruct
	{
		//// Default ctor
		AttributeStruct(internal::MemoryPage* page): header(reinterpret_cast<uintptr_t>(page)), Name(0), Value(0), prev_attribute_c(0), GetNextAttribute(0)
		{
		}

		uintptr_t header;

        Char8* Name;	////< Pointer to GetAttribute Name.
        Char8*	Value;	////< Pointer to GetAttribute Value.

		AttributeStruct* prev_attribute_c;	////< Previous GetAttribute (cyclic list)
		AttributeStruct* GetNextAttribute;	////< Next attribute
	};

	//// An XML document tree node.
	struct NodeStruct
	{
		//// Default ctor
		//// \param Type - node type
		NodeStruct(internal::MemoryPage* page, NodeType Type): header(reinterpret_cast<uintptr_t>(page) | (Type - 1)), GetParent(0), Name(0), Value(0), GetFirstChild(0), prev_sibling_c(0), GetNextSibling(0), GetFirstAttribute(0)
		{
		}

		uintptr_t header;

		NodeStruct*		GetParent;					////< Pointer to GetParent

        Char8*					Name;					////< Pointer to element Name.
        Char8*					Value;					////< Pointer to any associated string data.

		NodeStruct*		GetFirstChild;			////< First GetChild

		NodeStruct*		prev_sibling_c;			////< Left brother (cyclic list)
		NodeStruct*		GetNextSibling;			////< Right brother

		AttributeStruct*	GetFirstAttribute;		////< First attribute
	};
}

PUGI__NS_BEGIN
	struct DocumentStruct: public NodeStruct, public Allocator
	{
		DocumentStruct(MemoryPage* page): NodeStruct(page, NodeDocument), Allocator(page), buffer(0)
		{
		}

        const Char8* buffer;
	};

	inline Allocator& GetAllocator(const NodeStruct* node)
	{
		assert(node);

		return *reinterpret_cast<MemoryPage*>(node->header & MemoryPage_pointer_mask)->allocator;
	}
PUGI__NS_END

// Low-level DOM operations
PUGI__NS_BEGIN
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

		if (header & internal::MemoryPage_Name_allocated_mask) alloc.deallocate_string(a->Name);
		if (header & internal::MemoryPage_Value_allocated_mask) alloc.deallocate_string(a->Value);

		alloc.deallocate_memory(a, sizeof(AttributeStruct), reinterpret_cast<MemoryPage*>(header & MemoryPage_pointer_mask));
	}

	inline void destroy_node(NodeStruct* n, Allocator& alloc)
	{
		uintptr_t header = n->header;

		if (header & internal::MemoryPage_Name_allocated_mask) alloc.deallocate_string(n->Name);
		if (header & internal::MemoryPage_Value_allocated_mask) alloc.deallocate_string(n->Value);

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

	PUGI__FN_NO_INLINE NodeStruct* AppendNode(NodeStruct* node, Allocator& alloc, NodeType Type = NodeElement)
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

	PUGI__FN_NO_INLINE AttributeStruct* AppendAttribute_ll(NodeStruct* node, Allocator& alloc)
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
PUGI__NS_END

// Helper classes for code generation
PUGI__NS_BEGIN
	struct opt_false
	{
		enum { Value = 0 };
	};

	struct opt_true
	{
		enum { Value = 1 };
	};
PUGI__NS_END

// Unicode utilities
PUGI__NS_BEGIN
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

		static value_type low(value_type Result, uint32_t ch)
		{
			// U+0000..U+007F
			if (ch < 0x80) return Result + 1;
			// U+0080..U+07FF
			else if (ch < 0x800) return Result + 2;
			// U+0800..U+FFFF
			else return Result + 3;
		}

		static value_type high(value_type Result, uint32_t)
		{
			// U+10000..U+10FFFF
			return Result + 4;
		}
	};

	struct utf8_WriterInstance
	{
		typedef uint8_t* value_type;

		static value_type low(value_type Result, uint32_t ch)
		{
			// U+0000..U+007F
			if (ch < 0x80)
			{
				*Result = static_cast<uint8_t>(ch);
				return Result + 1;
			}
			// U+0080..U+07FF
			else if (ch < 0x800)
			{
				Result[0] = static_cast<uint8_t>(0xC0 | (ch >> 6));
				Result[1] = static_cast<uint8_t>(0x80 | (ch & 0x3F));
				return Result + 2;
			}
			// U+0800..U+FFFF
			else
			{
				Result[0] = static_cast<uint8_t>(0xE0 | (ch >> 12));
				Result[1] = static_cast<uint8_t>(0x80 | ((ch >> 6) & 0x3F));
				Result[2] = static_cast<uint8_t>(0x80 | (ch & 0x3F));
				return Result + 3;
			}
		}

		static value_type high(value_type Result, uint32_t ch)
		{
			// U+10000..U+10FFFF
			Result[0] = static_cast<uint8_t>(0xF0 | (ch >> 18));
			Result[1] = static_cast<uint8_t>(0x80 | ((ch >> 12) & 0x3F));
			Result[2] = static_cast<uint8_t>(0x80 | ((ch >> 6) & 0x3F));
			Result[3] = static_cast<uint8_t>(0x80 | (ch & 0x3F));
			return Result + 4;
		}

		static value_type any(value_type Result, uint32_t ch)
		{
			return (ch < 0x10000) ? low(Result, ch) : high(Result, ch);
		}
	};

	struct utf16_counter
	{
		typedef size_t value_type;

		static value_type low(value_type Result, uint32_t)
		{
			return Result + 1;
		}

		static value_type high(value_type Result, uint32_t)
		{
			return Result + 2;
		}
	};

	struct utf16_WriterInstance
	{
		typedef uint16_t* value_type;

		static value_type low(value_type Result, uint32_t ch)
		{
			*Result = static_cast<uint16_t>(ch);

			return Result + 1;
		}

		static value_type high(value_type Result, uint32_t ch)
		{
			uint32_t msh = static_cast<uint32_t>(ch - 0x10000) >> 10;
			uint32_t lsh = static_cast<uint32_t>(ch - 0x10000) & 0x3ff;

			Result[0] = static_cast<uint16_t>(0xD800 + msh);
			Result[1] = static_cast<uint16_t>(0xDC00 + lsh);

			return Result + 2;
		}

		static value_type any(value_type Result, uint32_t ch)
		{
			return (ch < 0x10000) ? low(Result, ch) : high(Result, ch);
		}
	};

	struct utf32_counter
	{
		typedef size_t value_type;

		static value_type low(value_type Result, uint32_t)
		{
			return Result + 1;
		}

		static value_type high(value_type Result, uint32_t)
		{
			return Result + 1;
		}
	};

	struct utf32_WriterInstance
	{
		typedef uint32_t* value_type;

		static value_type low(value_type Result, uint32_t ch)
		{
			*Result = ch;

			return Result + 1;
		}

		static value_type high(value_type Result, uint32_t ch)
		{
			*Result = ch;

			return Result + 1;
		}

		static value_type any(value_type Result, uint32_t ch)
		{
			*Result = ch;

			return Result + 1;
		}
	};

	struct latin1_WriterInstance
	{
		typedef uint8_t* value_type;

		static value_type low(value_type Result, uint32_t ch)
		{
			*Result = static_cast<uint8_t>(ch > 255 ? '?' : ch);

			return Result + 1;
		}

		static value_type high(value_type Result, uint32_t ch)
		{
			(void)ch;

			*Result = '?';

			return Result + 1;
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
		static inline typename Traits::value_type decode_utf8_block(const uint8_t* data, size_t size, typename Traits::value_type Result)
		{
			const uint8_t utf8_byte_mask = 0x3f;

			while (size)
			{
				uint8_t lead = *data;

				// 0xxxxxxx -> U+0000..U+007F
				if (lead < 0x80)
				{
					Result = Traits::low(Result, lead);
					data += 1;
					size -= 1;

					// process aligned single-byte (ascii) blocks
					if ((reinterpret_cast<uintptr_t>(data) & 3) == 0)
					{
						// round-trip through void* to silence 'cast increases required alignment of target Type' warnings
						while (size >= 4 && (*static_cast<const uint32_t*>(static_cast<const void*>(data)) & 0x80808080) == 0)
						{
							Result = Traits::low(Result, data[0]);
							Result = Traits::low(Result, data[1]);
							Result = Traits::low(Result, data[2]);
							Result = Traits::low(Result, data[3]);
							data += 4;
							size -= 4;
						}
					}
				}
				// 110xxxxx -> U+0080..U+07FF
				else if (static_cast<unsigned int>(lead - 0xC0) < 0x20 && size >= 2 && (data[1] & 0xc0) == 0x80)
				{
					Result = Traits::low(Result, ((lead & ~0xC0) << 6) | (data[1] & utf8_byte_mask));
					data += 2;
					size -= 2;
				}
				// 1110xxxx -> U+0800-U+FFFF
				else if (static_cast<unsigned int>(lead - 0xE0) < 0x10 && size >= 3 && (data[1] & 0xc0) == 0x80 && (data[2] & 0xc0) == 0x80)
				{
					Result = Traits::low(Result, ((lead & ~0xE0) << 12) | ((data[1] & utf8_byte_mask) << 6) | (data[2] & utf8_byte_mask));
					data += 3;
					size -= 3;
				}
				// 11110xxx -> U+10000..U+10FFFF
				else if (static_cast<unsigned int>(lead - 0xF0) < 0x08 && size >= 4 && (data[1] & 0xc0) == 0x80 && (data[2] & 0xc0) == 0x80 && (data[3] & 0xc0) == 0x80)
				{
					Result = Traits::high(Result, ((lead & ~0xF0) << 18) | ((data[1] & utf8_byte_mask) << 12) | ((data[2] & utf8_byte_mask) << 6) | (data[3] & utf8_byte_mask));
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

			return Result;
		}

		static inline typename Traits::value_type decode_utf16_block(const uint16_t* data, size_t size, typename Traits::value_type Result)
		{
			const uint16_t* end = data + size;

			while (data < end)
			{
				uint16_t lead = opt_swap::Value ? endian_swap(*data) : *data;

				// U+0000..U+D7FF
				if (lead < 0xD800)
				{
					Result = Traits::low(Result, lead);
					data += 1;
				}
				// U+E000..U+FFFF
				else if (static_cast<unsigned int>(lead - 0xE000) < 0x2000)
				{
					Result = Traits::low(Result, lead);
					data += 1;
				}
				// surrogate pair lead
				else if (static_cast<unsigned int>(lead - 0xD800) < 0x400 && data + 1 < end)
				{
					uint16_t next = opt_swap::Value ? endian_swap(data[1]) : data[1];

					if (static_cast<unsigned int>(next - 0xDC00) < 0x400)
					{
						Result = Traits::high(Result, 0x10000 + ((lead & 0x3ff) << 10) + (next & 0x3ff));
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

			return Result;
		}

		static inline typename Traits::value_type decode_utf32_block(const uint32_t* data, size_t size, typename Traits::value_type Result)
		{
			const uint32_t* end = data + size;

			while (data < end)
			{
				uint32_t lead = opt_swap::Value ? endian_swap(*data) : *data;

				// U+0000..U+FFFF
				if (lead < 0x10000)
				{
					Result = Traits::low(Result, lead);
					data += 1;
				}
				// U+10000..U+10FFFF
				else
				{
					Result = Traits::high(Result, lead);
					data += 1;
				}
			}

			return Result;
		}

		static inline typename Traits::value_type decode_latin1_block(const uint8_t* data, size_t size, typename Traits::value_type Result)
		{
			for (size_t i = 0; i < size; ++i)
			{
				Result = Traits::low(Result, data[i]);
			}

			return Result;
		}

		static inline typename Traits::value_type decode_wchar_block_impl(const uint16_t* data, size_t size, typename Traits::value_type Result)
		{
			return decode_utf16_block(data, size, Result);
		}

		static inline typename Traits::value_type decode_wchar_block_impl(const uint32_t* data, size_t size, typename Traits::value_type Result)
		{
			return decode_utf32_block(data, size, Result);
		}

		static inline typename Traits::value_type decode_wchar_block(const wchar_t* data, size_t size, typename Traits::value_type Result)
		{
			return decode_wchar_block_impl(reinterpret_cast<const wchar_selector<sizeof(wchar_t)>::Type*>(data), size, Result);
		}
	};

	template <typename T> PUGI__FN void convert_utf_endian_swap(T* Result, const T* data, size_t length)
	{
		for (size_t i = 0; i < length; ++i) Result[i] = endian_swap(data[i]);
	}

PUGI__NS_END

PUGI__NS_BEGIN
	enum charCollectionType
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

	static const unsigned char charCollectionTypeable[256] =
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

	static const unsigned char charTypex_table[256] =
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

	#define PUGI__IS_CHARTYPE_IMPL(c, ct, table) (table[static_cast<unsigned char>(c)] & (ct))

	#define PUGI__IS_CHARTYPE(c, ct) PUGI__IS_CHARTYPE_IMPL(c, ct, charCollectionTypeable)
	#define PUGI__IS_CHARTYPEX(c, ct) PUGI__IS_CHARTYPE_IMPL(c, ct, charTypex_table)

	PUGI__FN bool is_little_endian()
	{
		unsigned int ui = 1;

		return *reinterpret_cast<unsigned char*>(&ui) == 1;
	}

	PUGI__FN Encoding GetWchar_DocumentEncoding()
	{
		PUGI__STATIC_ASSERT(sizeof(wchar_t) == 2 || sizeof(wchar_t) == 4);

		if (sizeof(wchar_t) == 2)
			return is_little_endian() ? EncodingUTF16LE : EncodingUTF16BE;
		else
			return is_little_endian() ? EncodingUTF32LE : EncodingUTF32BE;
	}

	PUGI__FN Encoding guess_buffer_DocumentEncoding(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
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

	PUGI__FN Encoding GetBuffer_DocumentEncoding(Encoding DocumentEncoding, const void* contents, size_t size)
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

		PUGI__DMC_VOLATILE uint8_t d0 = data[0], d1 = data[1], d2 = data[2], d3 = data[3];

		return guess_buffer_DocumentEncoding(d0, d1, d2, d3);
	}

    PUGI__FN bool GetMutable_buffer(Char8*& out_buffer, size_t& out_length, const void* contents, size_t size, bool is_mutable)
	{
		if (is_mutable)
		{
            out_buffer = static_cast<Char8*>(const_cast<void*>(contents));
		}
		else
		{
			void* buffer = Memory::allocate(size > 0 ? size : 1);
			if (!buffer) return false;

			memcpy(buffer, contents, size);

            out_buffer = static_cast<Char8*>(buffer);
		}

        out_length = size / sizeof(Char8);

		return true;
	}

    template <typename opt_swap> PUGI__FN bool convert_buffer_utf16(Char8*& out_buffer, size_t& out_length, const void* contents, size_t size, opt_swap)
	{
		const uint16_t* data = static_cast<const uint16_t*>(contents);
		size_t length = size / sizeof(uint16_t);

		// first pass: get length in utf8 units
		out_length = utf_decoder<utf8_counter, opt_swap>::decode_utf16_block(data, length, 0);

		// allocate buffer of suitable length
        out_buffer = static_cast<Char8*>(Memory::allocate((out_length > 0 ? out_length : 1) * sizeof(Char8)));
		if (!out_buffer) return false;

		// second pass: convert utf16 input to utf8
		uint8_t* out_begin = reinterpret_cast<uint8_t*>(out_buffer);
		uint8_t* out_end = utf_decoder<utf8_WriterInstance, opt_swap>::decode_utf16_block(data, length, out_begin);

		assert(out_end == out_begin + out_length);
		(void)!out_end;

		return true;
	}

    template <typename opt_swap> PUGI__FN bool convert_buffer_utf32(Char8*& out_buffer, size_t& out_length, const void* contents, size_t size, opt_swap)
	{
		const uint32_t* data = static_cast<const uint32_t*>(contents);
		size_t length = size / sizeof(uint32_t);

		// first pass: get length in utf8 units
		out_length = utf_decoder<utf8_counter, opt_swap>::decode_utf32_block(data, length, 0);

		// allocate buffer of suitable length
        out_buffer = static_cast<Char8*>(Memory::allocate((out_length > 0 ? out_length : 1) * sizeof(Char8)));
		if (!out_buffer) return false;

		// second pass: convert utf32 input to utf8
		uint8_t* out_begin = reinterpret_cast<uint8_t*>(out_buffer);
		uint8_t* out_end = utf_decoder<utf8_WriterInstance, opt_swap>::decode_utf32_block(data, length, out_begin);

		assert(out_end == out_begin + out_length);
		(void)!out_end;

		return true;
	}

	PUGI__FN size_t GetLatin1_7bit_prefix_length(const uint8_t* data, size_t size)
	{
		for (size_t i = 0; i < size; ++i)
			if (data[i] > 127)
				return i;

		return size;
	}

    PUGI__FN bool convert_buffer_latin1(Char8*& out_buffer, size_t& out_length, const void* contents, size_t size, bool is_mutable)
	{
		const uint8_t* data = static_cast<const uint8_t*>(contents);

		// get size of prefix that does not need utf8 conversion
		size_t prefix_length = GetLatin1_7bit_prefix_length(data, size);
		assert(prefix_length <= size);

		const uint8_t* postfix = data + prefix_length;
		size_t postfix_length = size - prefix_length;

		// if no conversion is needed, just return the original buffer
		if (postfix_length == 0) return GetMutable_buffer(out_buffer, out_length, contents, size, is_mutable);

		// first pass: get length in utf8 units
		out_length = prefix_length + utf_decoder<utf8_counter>::decode_latin1_block(postfix, postfix_length, 0);

		// allocate buffer of suitable length
        out_buffer = static_cast<Char8*>(Memory::allocate((out_length > 0 ? out_length : 1) * sizeof(Char8)));
		if (!out_buffer) return false;

		// second pass: convert latin1 input to utf8
		memcpy(out_buffer, data, prefix_length);

		uint8_t* out_begin = reinterpret_cast<uint8_t*>(out_buffer);
		uint8_t* out_end = utf_decoder<utf8_WriterInstance>::decode_latin1_block(postfix, postfix_length, out_begin + prefix_length);

		assert(out_end == out_begin + out_length);
		(void)!out_end;

		return true;
	}

    PUGI__FN bool convert_buffer(Char8*& out_buffer, size_t& out_length, Encoding DocumentEncoding, const void* contents, size_t size, bool is_mutable)
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

		// source DocumentEncoding is latin1
        if (DocumentEncoding == EncodingLatin1) return convert_buffer_latin1(out_buffer, out_length, contents, size, is_mutable);

		assert(!"Invalid DocumentEncoding");
		return false;
	}


	PUGI__FN size_t AsUtf8_begin(const wchar_t* str, size_t length)
	{
		// get length in utf8 characters
		return utf_decoder<utf8_counter>::decode_wchar_block(str, length, 0);
	}

	PUGI__FN void AsUtf8_end(char* buffer, size_t size, const wchar_t* str, size_t length)
	{
		// convert to utf8
		uint8_t* begin = reinterpret_cast<uint8_t*>(buffer);
		uint8_t* end = utf_decoder<utf8_WriterInstance>::decode_wchar_block(str, length, begin);

		assert(begin + size == end);
		(void)!end;

		// zero-terminate
		buffer[size] = 0;
	}


	PUGI__FN std::string AsUtf8_impl(const wchar_t* str, size_t length)
	{
		// first pass: get length in utf8 characters
		size_t size = AsUtf8_begin(str, length);

		// allocate Resulting string
		std::string Result;
		Result.resize(size);

		// second pass: convert to utf8
		if (size > 0) AsUtf8_end(&Result[0], size, str, length);

		return Result;
	}

	PUGI__FN std::basic_string<wchar_t> AsWide_impl(const char* str, size_t size)
	{
		const uint8_t* data = reinterpret_cast<const uint8_t*>(str);

		// first pass: get length in wchar_t units
		size_t length = utf_decoder<wchar_counter>::decode_utf8_block(data, size, 0);

		// allocate Resulting string
		std::basic_string<wchar_t> Result;
		Result.resize(length);

		// second pass: convert to wchar_t
		if (length > 0)
		{
			wchar_WriterInstance::value_type begin = reinterpret_cast<wchar_WriterInstance::value_type>(&Result[0]);
			wchar_WriterInstance::value_type end = utf_decoder<wchar_WriterInstance>::decode_utf8_block(data, size, begin);

			assert(begin + length == end);
			(void)!end;
		}

		return Result;
	}


    inline bool strcpy_insitu_allow(size_t length, uintptr_t allocated, Char8* target)
	{
		assert(target);
		size_t tarGetLength = strlength(target);

		// always reuse document buffer memory if possible
		if (!allocated) return tarGetLength >= length;

		// reuse heap memory if waste is not too great
		const size_t reuse_threshold = 32;

		return tarGetLength >= length && (tarGetLength < reuse_threshold || tarGetLength - length < tarGetLength / 2);
	}

    PUGI__FN bool strcpy_insitu(Char8*& dest, uintptr_t& header, uintptr_t header_mask, const Char8* source)
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
            memcpy(dest, source, (source_length + 1) * sizeof(Char8));

			return true;
		}
		else
		{
			Allocator* alloc = reinterpret_cast<MemoryPage*>(header & MemoryPage_pointer_mask)->allocator;

			// allocate new buffer
            Char8* buf = alloc->allocate_string(source_length + 1);
			if (!buf) return false;

			// copy the string (including zero terminator)
            memcpy(buf, source, (source_length + 1) * sizeof(Char8));

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
        Char8* end;
		size_t size;

		gap(): end(0), size(0)
		{
		}

		// Push new gap, move s count bytes further (skipping the gap).
		// Collapse previous gap.
        void push(Char8*& s, size_t count)
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
        Char8* flush(Char8* s)
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

    PUGI__FN Char8* strconv_escape(Char8* s, gap& g)
	{
        Char8* stre = s + 1;

		switch (*stre)
		{
			case '#':	// &#...
			{
				unsigned int ucsc = 0;

				if (stre[1] == 'x') // &#x... (hex code)
				{
					stre += 2;

                    Char8 ch = *stre;

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
                    Char8 ch = *++stre;

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


                s = reinterpret_cast<Char8*>(utf8_WriterInstance::any(reinterpret_cast<uint8_t*>(s), ucsc));


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

			default:
				break;
		}

		return stre;
	}

	// Utility macro for last character handling
	#define ENDSWITH(c, e) ((c) == (e) || ((c) == 0 && endch == (e)))

    PUGI__FN Char8* strconv_comment(Char8* s, Char8 endch)
	{
		gap g;

		while (true)
		{
			while (!PUGI__IS_CHARTYPE(*s, ct_ParseComment)) ++s;

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

    PUGI__FN Char8* strconv_cdata(Char8* s, Char8 endch)
	{
		gap g;

		while (true)
		{
			while (!PUGI__IS_CHARTYPE(*s, ct_ParseCdata)) ++s;

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

    typedef Char8* (*strconv_pcdata_t)(Char8*);

	template <typename opt_eol, typename opt_escape> struct strconv_pcdata_impl
	{
        static Char8* parse(Char8* s)
		{
			gap g;

			while (true)
			{
				while (!PUGI__IS_CHARTYPE(*s, ct_ParsePcdata)) ++s;

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

	PUGI__FN strconv_pcdata_t GetStrconv_pcdata(unsigned int optmask)
	{
		PUGI__STATIC_ASSERT(ParseEscapes == 0x10 && ParseEol == 0x20);

		switch ((optmask >> 4) & 3) // get bitmask for flags (eol escapes)
		{
		case 0: return strconv_pcdata_impl<opt_false, opt_false>::parse;
		case 1: return strconv_pcdata_impl<opt_false, opt_true>::parse;
		case 2: return strconv_pcdata_impl<opt_true, opt_false>::parse;
		case 3: return strconv_pcdata_impl<opt_true, opt_true>::parse;
		default: return 0; // should not get here
		}
	}

    typedef Char8* (*strconv_attribute_t)(Char8*, Char8);

	template <typename opt_escape> struct strconv_attribute_impl
	{
        static Char8* ParseWnorm(Char8* s, Char8 end_quote)
		{
			gap g;

			// trim leading whitespaces
			if (PUGI__IS_CHARTYPE(*s, ct_space))
			{
                Char8* str = s;

				do ++str;
				while (PUGI__IS_CHARTYPE(*str, ct_space));

				g.push(s, str - s);
			}

			while (true)
			{
				while (!PUGI__IS_CHARTYPE(*s, ct_ParseAttrWs | ct_space)) ++s;

				if (*s == end_quote)
				{
                    Char8* str = g.flush(s);

					do *str-- = 0;
					while (PUGI__IS_CHARTYPE(*str, ct_space));

					return s + 1;
				}
				else if (PUGI__IS_CHARTYPE(*s, ct_space))
				{
					*s++ = ' ';

					if (PUGI__IS_CHARTYPE(*s, ct_space))
					{
                        Char8* str = s + 1;
						while (PUGI__IS_CHARTYPE(*str, ct_space)) ++str;

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

        static Char8* ParseWconv(Char8* s, Char8 end_quote)
		{
			gap g;

			while (true)
			{
				while (!PUGI__IS_CHARTYPE(*s, ct_ParseAttrWs)) ++s;

				if (*s == end_quote)
				{
					*g.flush(s) = 0;

					return s + 1;
				}
				else if (PUGI__IS_CHARTYPE(*s, ct_space))
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

        static Char8* ParseEol(Char8* s, Char8 end_quote)
		{
			gap g;

			while (true)
			{
				while (!PUGI__IS_CHARTYPE(*s, ct_ParseAttr)) ++s;

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

        static Char8* ParseSimple(Char8* s, Char8 end_quote)
		{
			gap g;

			while (true)
			{
				while (!PUGI__IS_CHARTYPE(*s, ct_ParseAttr)) ++s;

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

	PUGI__FN strconv_attribute_t GetStrconv_attribute(unsigned int optmask)
	{
		PUGI__STATIC_ASSERT(ParseEscapes == 0x10 && ParseEol == 0x20 && ParseWconvAttribute == 0x40 && ParseWnormAttribute == 0x80);

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
		ParseResult Result;
		Result.Status = Status;
		Result.Offset = Offset;

		return Result;
	}

	struct Parser
	{
		Allocator alloc;
        Char8* error_Offset;
		ParseStatus error_Status;

		// Parser utilities.
		#define PUGI__SKIPWS()			{ while (PUGI__IS_CHARTYPE(*s, ct_space)) ++s; }
		#define PUGI__OPTSET(OPT)			( optmsk & (OPT) )
		#define PUGI__PUSHNODE(TYPE)		{ cursor = AppendNode(cursor, alloc, TYPE); if (!cursor) PUGI__THROW_ERROR(StatusOutOfMemory, s); }
		#define PUGI__POPNODE()			{ cursor = cursor->GetParent; }
		#define PUGI__SCANFOR(X)			{ while (*s != 0 && !(X)) ++s; }
		#define PUGI__SCANWHILE(X)		{ while ((X)) ++s; }
		#define PUGI__ENDSEG()			{ ch = *s; *s = 0; ++s; }
        #define PUGI__THROW_ERROR(err, m)	return error_Offset = m, error_Status = err, static_cast<Char8*>(0)
		#define PUGI__CHECK_ERROR(err, m)	{ if (*s == 0) PUGI__THROW_ERROR(err, m); }

		Parser(const Allocator& alloc_): alloc(alloc_), error_Offset(0), error_Status(StatusOk)
		{
		}

		// DOCTYPE consists of nested sections of the following possible Types:
		// <!-- ... -->, <? ... ?>, "...", '...'
		// <![...]]>
		// <!...>
		// First group can not contain nested groups
		// Second group can contain nested groups of the same type
		// Third group can contain all other groups
        Char8* ParseDocTypePrimitive(Char8* s)
		{
			if (*s == '"' || *s == '\'')
			{
				// quoted string
                Char8 ch = *s++;
				PUGI__SCANFOR(*s == ch);
				if (!*s) PUGI__THROW_ERROR(StatusBadDocType, s);

				s++;
			}
			else if (s[0] == '<' && s[1] == '?')
			{
				// <? ... ?>
				s += 2;
				PUGI__SCANFOR(s[0] == '?' && s[1] == '>'); // no need for ENDSWITH because ?> can't terminate proper doctype
				if (!*s) PUGI__THROW_ERROR(StatusBadDocType, s);

				s += 2;
			}
			else if (s[0] == '<' && s[1] == '!' && s[2] == '-' && s[3] == '-')
			{
				s += 4;
				PUGI__SCANFOR(s[0] == '-' && s[1] == '-' && s[2] == '>'); // no need for ENDSWITH because --> can't terminate proper doctype
				if (!*s) PUGI__THROW_ERROR(StatusBadDocType, s);

				s += 4;
			}
			else PUGI__THROW_ERROR(StatusBadDocType, s);

			return s;
		}

        Char8* ParseDocTypeIgnore(Char8* s)
		{
			assert(s[0] == '<' && s[1] == '!' && s[2] == '[');
			s++;

			while (*s)
			{
				if (s[0] == '<' && s[1] == '!' && s[2] == '[')
				{
					// nested ignore section
					s = ParseDocTypeIgnore(s);
					if (!s) return s;
				}
				else if (s[0] == ']' && s[1] == ']' && s[2] == '>')
				{
					// ignore section end
					s += 3;

					return s;
				}
				else s++;
			}

			PUGI__THROW_ERROR(StatusBadDocType, s);
		}

        Char8* ParseDocTypeGroup(Char8* s, Char8 endch, bool toplevel)
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
						if (!s) return s;
					}
					else
					{
						// some control group
						s = ParseDocTypeGroup(s, endch, false);
						if (!s) return s;
					}
				}
				else if (s[0] == '<' || s[0] == '"' || s[0] == '\'')
				{
					// unknown tag (forbidden), or some primitive group
					s = ParseDocTypePrimitive(s);
					if (!s) return s;
				}
				else if (*s == '>')
				{
					s++;

					return s;
				}
				else s++;
			}

			if (!toplevel || endch != '>') PUGI__THROW_ERROR(StatusBadDocType, s);

			return s;
		}

        Char8* ParseExclamation(Char8* s, NodeStruct* cursor, unsigned int optmsk, Char8 endch)
		{
			// parse node contents, starting with exclamation mark
			++s;

			if (*s == '-') // '<!-...'
			{
				++s;

				if (*s == '-') // '<!--...'
				{
					++s;

					if (PUGI__OPTSET(ParseComments))
					{
						PUGI__PUSHNODE(NodeComment); // Append a new node on the tree.
						cursor->Value = s; // Save the Offset.
					}

					if (PUGI__OPTSET(ParseEol) && PUGI__OPTSET(ParseComments))
					{
						s = strconv_comment(s, endch);

						if (!s) PUGI__THROW_ERROR(StatusBadComment, cursor->Value);
					}
					else
					{
						// Scan for terminating '-->'.
						PUGI__SCANFOR(s[0] == '-' && s[1] == '-' && ENDSWITH(s[2], '>'));
						PUGI__CHECK_ERROR(StatusBadComment, s);

						if (PUGI__OPTSET(ParseComments))
							*s = 0; // Zero-terminate this segment at the first terminating '-'.

						s += (s[2] == '>' ? 3 : 2); // Step over the '\0->'.
					}
				}
				else PUGI__THROW_ERROR(StatusBadComment, s);
			}
			else if (*s == '[')
			{
				// '<![CDATA[...'
				if (*++s=='C' && *++s=='D' && *++s=='A' && *++s=='T' && *++s=='A' && *++s == '[')
				{
					++s;

					if (PUGI__OPTSET(ParseCdata))
					{
						PUGI__PUSHNODE(NodeCdata); // Append a new node on the tree.
						cursor->Value = s; // Save the Offset.

						if (PUGI__OPTSET(ParseEol))
						{
							s = strconv_cdata(s, endch);

							if (!s) PUGI__THROW_ERROR(StatusBadCdata, cursor->Value);
						}
						else
						{
							// Scan for terminating ']]>'.
							PUGI__SCANFOR(s[0] == ']' && s[1] == ']' && ENDSWITH(s[2], '>'));
							PUGI__CHECK_ERROR(StatusBadCdata, s);

							*s++ = 0; // Zero-terminate this segment.
						}
					}
					else // Flagged for discard, but we still have to scan for the terminator.
					{
						// Scan for terminating ']]>'.
						PUGI__SCANFOR(s[0] == ']' && s[1] == ']' && ENDSWITH(s[2], '>'));
						PUGI__CHECK_ERROR(StatusBadCdata, s);

						++s;
					}

					s += (s[1] == '>' ? 2 : 1); // Step over the last ']>'.
				}
				else PUGI__THROW_ERROR(StatusBadCdata, s);
			}
			else if (s[0] == 'D' && s[1] == 'O' && s[2] == 'C' && s[3] == 'T' && s[4] == 'Y' && s[5] == 'P' && ENDSWITH(s[6], 'E'))
			{
				s -= 2;

				if (cursor->GetParent) PUGI__THROW_ERROR(StatusBadDocType, s);

                Char8* mark = s + 9;

				s = ParseDocTypeGroup(s, endch, true);
				if (!s) return s;

				if (PUGI__OPTSET(ParseDocType))
				{
					while (PUGI__IS_CHARTYPE(*mark, ct_space)) ++mark;

					PUGI__PUSHNODE(NodeDocType);

					cursor->Value = mark;

					assert((s[0] == 0 && endch == '>') || s[-1] == '>');
					s[*s == 0 ? 0 : -1] = 0;

					PUGI__POPNODE();
				}
			}
			else if (*s == 0 && endch == '-') PUGI__THROW_ERROR(StatusBadComment, s);
			else if (*s == 0 && endch == '[') PUGI__THROW_ERROR(StatusBadCdata, s);
			else PUGI__THROW_ERROR(StatusUnrecognizedTag, s);

			return s;
		}

        Char8* ParseQuestion(Char8* s, NodeStruct*& ref_cursor, unsigned int optmsk, Char8 endch)
		{
			// Load into registers
			NodeStruct* cursor = ref_cursor;
            Char8 ch = 0;

			// parse node contents, starting with question mark
			++s;

			// read PI target
            Char8* target = s;

            if (!PUGI__IS_CHARTYPE(*s, ct_start_symbol)) PUGI__THROW_ERROR(StatusBadProcessingInstruction, s);

			PUGI__SCANWHILE(PUGI__IS_CHARTYPE(*s, ct_symbol));
            PUGI__CHECK_ERROR(StatusBadProcessingInstruction, s);

			// determine node Type; stricmp / strcasecmp is not portable
			bool declaration = (target[0] | ' ') == 'x' && (target[1] | ' ') == 'm' && (target[2] | ' ') == 'l' && target + 3 == s;

			if (declaration ? PUGI__OPTSET(ParseDeclaration) : PUGI__OPTSET(ParsePi))
			{
				if (declaration)
				{
					// disallow non top-level declarations
                    if (cursor->GetParent) PUGI__THROW_ERROR(StatusBadProcessingInstruction, s);

					PUGI__PUSHNODE(NodeDeclaration);
				}
				else
				{
					PUGI__PUSHNODE(NodePi);
				}

				cursor->Name = target;

				PUGI__ENDSEG();

				// parse Value/attributes
				if (ch == '?')
				{
					// empty node
                    if (!ENDSWITH(*s, '>')) PUGI__THROW_ERROR(StatusBadProcessingInstruction, s);
					s += (*s == '>');

					PUGI__POPNODE();
				}
				else if (PUGI__IS_CHARTYPE(ch, ct_space))
				{
					PUGI__SKIPWS();

					// scan for tag end
                    Char8* Value = s;

					PUGI__SCANFOR(s[0] == '?' && ENDSWITH(s[1], '>'));
                    PUGI__CHECK_ERROR(StatusBadProcessingInstruction, s);

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
						PUGI__POPNODE();

						PUGI__ENDSEG();

						s += (*s == '>');
					}
				}
                else PUGI__THROW_ERROR(StatusBadProcessingInstruction, s);
			}
			else
			{
				// scan for tag end
				PUGI__SCANFOR(s[0] == '?' && ENDSWITH(s[1], '>'));
                PUGI__CHECK_ERROR(StatusBadProcessingInstruction, s);

				s += (s[1] == '>' ? 2 : 1);
			}

			// store from registers
			ref_cursor = cursor;

			return s;
		}

        Char8* parse(Char8* s, NodeStruct* xmldoc, unsigned int optmsk, Char8 endch)
		{
			strconv_attribute_t strconv_attribute = GetStrconv_attribute(optmsk);
			strconv_pcdata_t strconv_pcdata = GetStrconv_pcdata(optmsk);

            Char8 ch = 0;
			NodeStruct* cursor = xmldoc;
            Char8* mark = s;

			while (*s != 0)
			{
				if (*s == '<')
				{
					++s;

				LOC_TAG:
					if (PUGI__IS_CHARTYPE(*s, ct_start_symbol)) // '<#...'
					{
						PUGI__PUSHNODE(NodeElement); // Append a new node to the tree.

						cursor->Name = s;

						PUGI__SCANWHILE(PUGI__IS_CHARTYPE(*s, ct_symbol)); // Scan for a terminator.
						PUGI__ENDSEG(); // Save char in 'ch', terminate & step over.

						if (ch == '>')
						{
							// end of tag
						}
						else if (PUGI__IS_CHARTYPE(ch, ct_space))
						{
						LOC_ATTRIBUTES:
							while (true)
							{
								PUGI__SKIPWS(); // Eat any whitespace.

								if (PUGI__IS_CHARTYPE(*s, ct_start_symbol)) // <... #...
								{
									AttributeStruct* a = AppendAttribute_ll(cursor, alloc); // Make space for this GetAttribute.
									if (!a) PUGI__THROW_ERROR(StatusOutOfMemory, s);

									a->Name = s; // Save the Offset.

									PUGI__SCANWHILE(PUGI__IS_CHARTYPE(*s, ct_symbol)); // Scan for a terminator.
									PUGI__CHECK_ERROR(StatusBadAttribute, s); //$ redundant, left for performance

									PUGI__ENDSEG(); // Save char in 'ch', terminate & step over.
									PUGI__CHECK_ERROR(StatusBadAttribute, s); //$ redundant, left for performance

									if (PUGI__IS_CHARTYPE(ch, ct_space))
									{
										PUGI__SKIPWS(); // Eat any whitespace.
										PUGI__CHECK_ERROR(StatusBadAttribute, s); //$ redundant, left for performance

										ch = *s;
										++s;
									}

									if (ch == '=') // '<... #=...'
									{
										PUGI__SKIPWS(); // Eat any whitespace.

										if (*s == '"' || *s == '\'') // '<... #="...'
										{
											ch = *s; // Save quote char to avoid breaking on "''" -or- '""'.
											++s; // Step over the quote.
											a->Value = s; // Save the Offset.

											s = strconv_attribute(s, ch);

											if (!s) PUGI__THROW_ERROR(StatusBadAttribute, a->Value);

											// After this line the loop continues from the start;
											// Whitespaces, / and > are ok, symbols and EOF are wrong,
											// everything else will be detected
											if (PUGI__IS_CHARTYPE(*s, ct_start_symbol)) PUGI__THROW_ERROR(StatusBadAttribute, s);
										}
										else PUGI__THROW_ERROR(StatusBadAttribute, s);
									}
									else PUGI__THROW_ERROR(StatusBadAttribute, s);
								}
								else if (*s == '/')
								{
									++s;

									if (*s == '>')
									{
										PUGI__POPNODE();
										s++;
										break;
									}
									else if (*s == 0 && endch == '>')
									{
										PUGI__POPNODE();
										break;
									}
									else PUGI__THROW_ERROR(StatusBadStartElement, s);
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
								else PUGI__THROW_ERROR(StatusBadStartElement, s);
							}

							// !!!
						}
						else if (ch == '/') // '<#.../'
						{
							if (!ENDSWITH(*s, '>')) PUGI__THROW_ERROR(StatusBadStartElement, s);

							PUGI__POPNODE(); // Pop.

							s += (*s == '>');
						}
						else if (ch == 0)
						{
							// we stepped over null terminator, backtrack & handle closing tag
							--s;

							if (endch != '>') PUGI__THROW_ERROR(StatusBadStartElement, s);
						}
						else PUGI__THROW_ERROR(StatusBadStartElement, s);
					}
					else if (*s == '/')
					{
						++s;

                        Char8* Name = cursor->Name;
						if (!Name) PUGI__THROW_ERROR(StatusEndElementMismatch, s);

						while (PUGI__IS_CHARTYPE(*s, ct_symbol))
						{
							if (*s++ != *Name++) PUGI__THROW_ERROR(StatusEndElementMismatch, s);
						}

						if (*Name)
						{
							if (*s == 0 && Name[0] == endch && Name[1] == 0) PUGI__THROW_ERROR(StatusBadEndElement, s);
							else PUGI__THROW_ERROR(StatusEndElementMismatch, s);
						}

						PUGI__POPNODE(); // Pop.

						PUGI__SKIPWS();

						if (*s == 0)
						{
							if (endch != '>') PUGI__THROW_ERROR(StatusBadEndElement, s);
						}
						else
						{
							if (*s != '>') PUGI__THROW_ERROR(StatusBadEndElement, s);
							++s;
						}
					}
					else if (*s == '?') // '<?...'
					{
						s = ParseQuestion(s, cursor, optmsk, endch);
						if (!s) return s;

						assert(cursor);
						if ((cursor->header & MemoryPage_type_mask) + 1 == NodeDeclaration) goto LOC_ATTRIBUTES;
					}
					else if (*s == '!') // '<!...'
					{
						s = ParseExclamation(s, cursor, optmsk, endch);
						if (!s) return s;
					}
                    else if (*s == 0 && endch == '?') PUGI__THROW_ERROR(StatusBadProcessingInstruction, s);
					else PUGI__THROW_ERROR(StatusUnrecognizedTag, s);
				}
				else
				{
					mark = s; // Save this Offset while searching for a terminator.

					PUGI__SKIPWS(); // Eat whitespace if no genuine PCDATA here.

					if (*s == '<')
					{
						// We skipped some whitespace characters because otherwise we would take the tag branch instead of PCDATA one
						assert(mark != s);

						if (!PUGI__OPTSET(ParseWsPcdata | ParseWsPcdata_single))
						{
							continue;
						}
						else if (PUGI__OPTSET(ParseWsPcdata_single))
						{
							if (s[1] != '/' || cursor->GetFirstChild) continue;
						}
					}

					s = mark;

					if (cursor->GetParent)
					{
						PUGI__PUSHNODE(NodePcdata); // Append a new node on the tree.
						cursor->Value = s; // Save the Offset.

						s = strconv_pcdata(s);

						PUGI__POPNODE(); // Pop since this is a standalone.

						if (!*s) break;
					}
					else
					{
						PUGI__SCANFOR(*s == '<'); // '...<'
						if (!*s) break;

						++s;
					}

					// We're after '<'
					goto LOC_TAG;
				}
			}

			// check that last tag is closed
			if (cursor != xmldoc) PUGI__THROW_ERROR(StatusEndElementMismatch, s);

			return s;
		}

        static ParseResult parse(Char8* buffer, size_t length, NodeStruct* GetRoot, unsigned int optmsk)
		{
			DocumentStruct* xmldoc = static_cast<DocumentStruct*>(GetRoot);

			// store buffer for OffSetDebug
			xmldoc->buffer = buffer;

			// early-out for empty documents
			if (length == 0) return make_ParseResult(StatusOk);

			// create parser on stack
			Parser parser(*xmldoc);

			// Save last character and make buffer zero-terminated (speeds up parsing)
            Char8 endch = buffer[length - 1];
			buffer[length - 1] = 0;

			// perform actual parsing
			parser.parse(buffer, xmldoc, optmsk, endch);

			ParseResult Result = make_ParseResult(parser.error_Status, parser.error_Offset ? parser.error_Offset - buffer : 0);
			assert(Result.Offset >= 0 && static_cast<size_t>(Result.Offset) <= length);

			// update allocator state
			*static_cast<Allocator*>(xmldoc) = parser.alloc;

			// since we removed last character, we have to handle the only possible false positive
			if (Result && endch == '<')
			{
				// there's no possible well-formed document with < at the end
				return make_ParseResult(StatusUnrecognizedTag, length);
			}

			return Result;
		}
	};

	// Output facilities
	PUGI__FN Encoding GetWrite_native_DocumentEncoding()
	{
		return EncodingUTF8;
	}

	PUGI__FN Encoding GetWrite_DocumentEncoding(Encoding DocumentEncoding)
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

    PUGI__FN size_t GetValid_length(const Char8* data, size_t length)
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

    PUGI__FN size_t convert_buffer(Char8* /* r_char */, uint8_t* r_u8, uint16_t* r_u16, uint32_t* r_u32, const Char8* data, size_t length, Encoding DocumentEncoding)
	{
		if (DocumentEncoding == EncodingUTF16BE || DocumentEncoding == EncodingUTF16LE)
		{
			uint16_t* dest = r_u16;

			// convert to native utf16
			uint16_t* end = utf_decoder<utf16_WriterInstance>::decode_utf8_block(reinterpret_cast<const uint8_t*>(data), length, dest);

			// swap if necessary
			Encoding native_DocumentEncoding = is_little_endian() ? EncodingUTF16LE : EncodingUTF16BE;

			if (native_DocumentEncoding != DocumentEncoding) convert_utf_endian_swap(dest, dest, static_cast<size_t>(end - dest));

			return static_cast<size_t>(end - dest) * sizeof(uint16_t);
		}

		if (DocumentEncoding == EncodingUTF32BE || DocumentEncoding == EncodingUTF32LE)
		{
			uint32_t* dest = r_u32;

			// convert to native utf32
			uint32_t* end = utf_decoder<utf32_WriterInstance>::decode_utf8_block(reinterpret_cast<const uint8_t*>(data), length, dest);

			// swap if necessary
			Encoding native_DocumentEncoding = is_little_endian() ? EncodingUTF32LE : EncodingUTF32BE;

			if (native_DocumentEncoding != DocumentEncoding) convert_utf_endian_swap(dest, dest, static_cast<size_t>(end - dest));

			return static_cast<size_t>(end - dest) * sizeof(uint32_t);
		}

        if (DocumentEncoding == EncodingLatin1)
		{
			uint8_t* dest = r_u8;
			uint8_t* end = utf_decoder<latin1_WriterInstance>::decode_utf8_block(reinterpret_cast<const uint8_t*>(data), length, dest);

			return static_cast<size_t>(end - dest);
		}

		assert(!"Invalid DocumentEncoding");
		return 0;
	}


	class BufferedWriter
	{
		BufferedWriter(const BufferedWriter&);
		BufferedWriter& operator=(const BufferedWriter&);

	public:
		BufferedWriter(Writer& WriterInstance_, Encoding user_DocumentEncoding): WriterInstance(WriterInstance_), bufsize(0), DocumentEncoding(GetWrite_DocumentEncoding(user_DocumentEncoding))
		{
			PUGI__STATIC_ASSERT(bufcapacity >= 8);
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

        void flush(const Char8* data, size_t size)
		{
			if (size == 0) return;

			// fast Path, just Write data
			if (DocumentEncoding == GetWrite_native_DocumentEncoding())
                WriterInstance.Write(data, size * sizeof(Char8));
			else
			{
				// convert chunk
				size_t Result = convert_buffer(scratch.data_char, scratch.data_u8, scratch.data_u16, scratch.data_u32, data, size, DocumentEncoding);
				assert(Result <= sizeof(scratch));

				// Write data
				WriterInstance.Write(scratch.data_u8, Result);
			}
		}

        void Write(const Char8* data, size_t length)
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
                        WriterInstance.Write(data, length * sizeof(Char8));
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

            memcpy(buffer + bufsize, data, length * sizeof(Char8));
			bufsize += length;
		}

        void Write(const Char8* data)
		{
			Write(data, strlength(data));
		}

        void Write(Char8 d0)
		{
			if (bufsize + 1 > bufcapacity) flush();

			buffer[bufsize + 0] = d0;
			bufsize += 1;
		}

        void Write(Char8 d0, Char8 d1)
		{
			if (bufsize + 2 > bufcapacity) flush();

			buffer[bufsize + 0] = d0;
			buffer[bufsize + 1] = d1;
			bufsize += 2;
		}

        void Write(Char8 d0, Char8 d1, Char8 d2)
		{
			if (bufsize + 3 > bufcapacity) flush();

			buffer[bufsize + 0] = d0;
			buffer[bufsize + 1] = d1;
			buffer[bufsize + 2] = d2;
			bufsize += 3;
		}

        void Write(Char8 d0, Char8 d1, Char8 d2, Char8 d3)
		{
			if (bufsize + 4 > bufcapacity) flush();

			buffer[bufsize + 0] = d0;
			buffer[bufsize + 1] = d1;
			buffer[bufsize + 2] = d2;
			buffer[bufsize + 3] = d3;
			bufsize += 4;
		}

        void Write(Char8 d0, Char8 d1, Char8 d2, Char8 d3, Char8 d4)
		{
			if (bufsize + 5 > bufcapacity) flush();

			buffer[bufsize + 0] = d0;
			buffer[bufsize + 1] = d1;
			buffer[bufsize + 2] = d2;
			buffer[bufsize + 3] = d3;
			buffer[bufsize + 4] = d4;
			bufsize += 5;
		}

        void Write(Char8 d0, Char8 d1, Char8 d2, Char8 d3, Char8 d4, Char8 d5)
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
		enum
		{
			bufcapacitybytes =
			#ifdef XML_MEMORY_OUTPUT_STACK
				XML_MEMORY_OUTPUT_STACK
			#else
				10240
			#endif
			,
            bufcapacity = bufcapacitybytes / (sizeof(Char8) + 4)
		};

        Char8 buffer[bufcapacity];

		union
		{
			uint8_t data_u8[4 * bufcapacity];
			uint16_t data_u16[2 * bufcapacity];
			uint32_t data_u32[bufcapacity];
            Char8 data_char[bufcapacity];
		} scratch;

		Writer& WriterInstance;
		size_t bufsize;
		Encoding DocumentEncoding;
	};

    PUGI__FN void text_output_escaped(BufferedWriter& WriterInstance, const Char8* s, charTypex_t Type)
	{
		while (*s)
		{
            const Char8* prev = s;

			// While *s is a usual symbol
			while (!PUGI__IS_CHARTYPEX(*s, Type)) ++s;

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

                    WriterInstance.Write('&', '#', static_cast<Char8>((ch / 10) + '0'), static_cast<Char8>((ch % 10) + '0'), ';');
				}
			}
		}
	}

    PUGI__FN void text_output(BufferedWriter& WriterInstance, const Char8* s, charTypex_t Type, unsigned int flags)
	{
		if (flags & FormatNoEscapes)
			WriterInstance.Write(s);
		else
			text_output_escaped(WriterInstance, s, Type);
	}

    PUGI__FN void text_output_cdata(BufferedWriter& WriterInstance, const Char8* s)
	{
		do
		{
			WriterInstance.Write('<', '!', '[', 'C', 'D');
			WriterInstance.Write('A', 'T', 'A', '[');

            const Char8* prev = s;

			// look for ]]> sequence - we can't output it as is since it terminates CDATA
			while (*s && !(s[0] == ']' && s[1] == ']' && s[2] == '>')) ++s;

			// skip ]] if we stopped at ]]>, > will go to the next CDATA section
			if (*s) s += 2;

			WriterInstance.Write(prev, static_cast<size_t>(s - prev));

			WriterInstance.Write(']', ']', '>');
		}
		while (*s);
	}

	PUGI__FN void NodeOutput_attributes(BufferedWriter& WriterInstance, const Node& node, unsigned int flags)
	{
        const Char8* default_Name = ":anonymous";

		for (Attribute a = node.GetFirstAttribute(); a; a = a.GetNextAttribute())
		{
			WriterInstance.Write(' ');
			WriterInstance.Write(a.Name()[0] ? a.Name() : default_Name);
			WriterInstance.Write('=', '"');

			text_output(WriterInstance, a.Value(), ctx_special_attr, flags);

			WriterInstance.Write('"');
		}
	}

    PUGI__FN void NodeOutput(BufferedWriter& WriterInstance, const Node& node, const Char8* indent, unsigned int flags, unsigned int Depth)
	{
        const Char8* default_Name = ":anonymous";

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
            const Char8* Name = node.Name()[0] ? node.Name() : default_Name;

			WriterInstance.Write('<');
			WriterInstance.Write(Name);

			NodeOutput_attributes(WriterInstance, node, flags);

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
					text_output(WriterInstance, node.GetFirstChild().Value(), ctx_special_pcdata, flags);
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
			text_output(WriterInstance, node.Value(), ctx_special_pcdata, flags);
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
				NodeOutput_attributes(WriterInstance, node, flags);
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

	PUGI__FN void recursive_copy_skip(Node& dest, const Node& source, const Node& skip)
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

	inline bool is_text_node(NodeStruct* node)
	{
		NodeType Type = static_cast<NodeType>((node->header & internal::MemoryPage_type_mask) + 1);

		return Type == NodePcdata || Type == NodeCdata;
	}

	// get Value with conversion functions
    PUGI__FN int GetValue_int(const Char8* Value, int def)
	{
		if (!Value) return def;

		return static_cast<int>(strtol(Value, 0, 10));
	}

    PUGI__FN unsigned int GetValue_uint(const Char8* Value, unsigned int def)
	{
		if (!Value) return def;

		return static_cast<unsigned int>(strtoul(Value, 0, 10));
	}

    PUGI__FN double GetValue_double(const Char8* Value, double def)
	{
		if (!Value) return def;

		return strtod(Value, 0);
	}

    PUGI__FN float GetValue_float(const Char8* Value, float def)
	{
		if (!Value) return def;

        return static_cast<float>(strtod(Value, 0));
	}

    PUGI__FN bool GetValue_bool(const Char8* Value, bool def)
	{
		if (!Value) return def;

		// only look at first char
        Char8 first = *Value;

		// 1*, t* (true), T* (True), y* (yes), Y* (YES)
		return (first == '1' || first == 't' || first == 'T' || first == 'y' || first == 'Y');
	}

	// set Value with conversion functions
    PUGI__FN bool SetValue_buffer(Char8*& dest, uintptr_t& header, uintptr_t header_mask, char (&buf)[128])
    {
        return strcpy_insitu(dest, header, header_mask, buf);
	}

    PUGI__FN bool SetValue_convert(Char8*& dest, uintptr_t& header, uintptr_t header_mask, int Value)
	{
		char buf[128];
		sprintf(buf, "%d", Value);

		return SetValue_buffer(dest, header, header_mask, buf);
	}

    PUGI__FN bool SetValue_convert(Char8*& dest, uintptr_t& header, uintptr_t header_mask, unsigned int Value)
	{
		char buf[128];
		sprintf(buf, "%u", Value);

		return SetValue_buffer(dest, header, header_mask, buf);
	}

    PUGI__FN bool SetValue_convert(Char8*& dest, uintptr_t& header, uintptr_t header_mask, double Value)
	{
		char buf[128];
		sprintf(buf, "%g", Value);

		return SetValue_buffer(dest, header, header_mask, buf);
	}

    PUGI__FN bool SetValue_convert(Char8*& dest, uintptr_t& header, uintptr_t header_mask, bool Value)
	{
        return strcpy_insitu(dest, header, header_mask, Value ? "true" : "false");
	}

	// we need to get length of entire file to Load it in memory; the only (relatively) sane way to do it is via seek/tell trick
	PUGI__FN ParseStatus GetFile_size(FILE* file, size_t& out_Result)
	{
		// if this is a 32-bit OS, long is enough; if this is a unix system, long is 64-bit, which is enough; otherwise we can't do anything anyway.
		typedef long length_type;

		fseek(file, 0, SEEK_END);
		length_type length = ftell(file);
		fseek(file, 0, SEEK_SET);

		// check for I/O errors
		if (length < 0) return StatusIOError;

		// check for overflow
		size_t Result = static_cast<size_t>(length);

		if (static_cast<length_type>(Result) != length) return StatusOutOfMemory;

		// finalize
		out_Result = Result;

		return StatusOk;
	}

	PUGI__FN
	ParseResult LoadDataStreamImpl(Document& doc, Mezzanine::Resource::DataStream& stream, unsigned int options, Encoding DocumentEncoding)
	{
	    // Copying mostly from the function below, a lot of what they try to do is not applicable with data streams since they already do it to some extent.
	    size_t pos = stream.Tell();
	    size_t length = stream.GetSize() - pos;

	    if (pos < 0) return make_ParseResult(StatusIOError);

	    buffer_holder buffer(Memory::allocate(stream.GetSize() > 0 ? length : 1), Memory::deallocate);
		if (!buffer.data) return make_ParseResult(StatusOutOfMemory);

		size_t actual_length = stream.Read(buffer.data, length);
		assert(actual_length <= length);

		return doc.LoadBufferInplaceOwn(buffer.release(), actual_length, options, DocumentEncoding);
    }

 PUGI__FN  	ParseResult LoadFileImpl(Document& doc, FILE* file, unsigned int options, Encoding DocumentEncoding)
	{
		if (!file) return make_ParseResult(StatusFileNotFound);

		// get file size (can Result in I/O errors)
		size_t size = 0;
		ParseStatus size_Status = GetFile_size(file, size);

		if (size_Status != StatusOk)
		{
			fclose(file);
			return make_ParseResult(size_Status);
		}

		// allocate buffer for the whole file
		char* contents = static_cast<char*>(Memory::allocate(size > 0 ? size : 1));

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
			Memory::deallocate(contents);
			return make_ParseResult(StatusIOError);
		}

		return doc.LoadBufferInplaceOwn(contents, size, options, DocumentEncoding);
	}

	template <typename T> struct StreamChunk
	{
		static StreamChunk* create()
		{
			void* memory = Memory::allocate(sizeof(StreamChunk));

			return new (memory) StreamChunk();
		}

		static void destroy(void* ptr)
		{
			StreamChunk* chunk = static_cast<StreamChunk*>(ptr);

			// free chunk chain
			while (chunk)
			{
				StreamChunk* next = chunk->next;
				Memory::deallocate(chunk);
				chunk = next;
			}
		}

		StreamChunk(): next(0), size(0)
		{
		}

		StreamChunk* next;
		size_t size;

		T data[MemoryPage_size / sizeof(T)];
	};

	template <typename T> PUGI__FN ParseStatus LoadStreamDataNoseek(std::basic_istream<T>& stream, void** out_buffer, size_t* out_size)
	{
		buffer_holder chunks(0, StreamChunk<T>::destroy);

		// read file to a chunk list
		size_t total = 0;
		StreamChunk<T>* last = 0;

		while (!stream.eof())
		{
			// allocate new chunk
			StreamChunk<T>* chunk = StreamChunk<T>::create();
			if (!chunk) return StatusOutOfMemory;

			// append chunk to list
			if (last) last = last->next = chunk;
			else chunks.data = last = chunk;

			// read data to chunk
			stream.read(chunk->data, static_cast<std::streamsize>(sizeof(chunk->data) / sizeof(T)));
			chunk->size = static_cast<size_t>(stream.gcount()) * sizeof(T);

			// read may set failbit | eofbit in case gcount() is less than read length, so check for other I/O errors
			if (stream.bad() || (!stream.eof() && stream.fail())) return StatusIOError;

			// guard against huge files (chunk size is small enough to make this overflow check work)
			if (total + chunk->size < total) return StatusOutOfMemory;
			total += chunk->size;
		}

		// copy chunk list to a contiguous buffer
		char* buffer = static_cast<char*>(Memory::allocate(total));
		if (!buffer) return StatusOutOfMemory;

		char* Write = buffer;

		for (StreamChunk<T>* chunk = static_cast<StreamChunk<T>*>(chunks.data); chunk; chunk = chunk->next)
		{
			assert(Write + chunk->size <= buffer + total);
			memcpy(Write, chunk->data, chunk->size);
			Write += chunk->size;
		}

		assert(Write == buffer + total);

		// return buffer
		*out_buffer = buffer;
		*out_size = total;

		return StatusOk;
	}

	template <typename T> PUGI__FN ParseStatus LoadStreamDataSeek(std::basic_istream<T>& stream, void** out_buffer, size_t* out_size)
	{
		// get length of remaining data in stream
		typename std::basic_istream<T>::pos_type pos = stream.tellg();
		stream.seekg(0, std::ios::end);
		std::streamoff length = stream.tellg() - pos;
		stream.seekg(pos);

		if (stream.fail() || pos < 0) return StatusIOError;

		// guard against huge files
		size_t read_length = static_cast<size_t>(length);

		if (static_cast<std::streamsize>(read_length) != length || length < 0) return StatusOutOfMemory;

		// read stream data into memory (guard against stream exceptions with buffer holder)
		buffer_holder buffer(Memory::allocate((read_length > 0 ? read_length : 1) * sizeof(T)), Memory::deallocate);
		if (!buffer.data) return StatusOutOfMemory;

		stream.read(static_cast<T*>(buffer.data), static_cast<std::streamsize>(read_length));

		// read may set failbit | eofbit in case gcount() is less than read_length (i.e. line ending conversion), so check for other I/O errors
		if (stream.bad() || (!stream.eof() && stream.fail())) return StatusIOError;

		// return buffer
		size_t actual_length = static_cast<size_t>(stream.gcount());
		assert(actual_length <= read_length);

		*out_buffer = buffer.release();
		*out_size = actual_length * sizeof(T);

		return StatusOk;
	}

	template <typename T> PUGI__FN ParseResult LoadStreamImpl(Document& doc, std::basic_istream<T>& stream, unsigned int options, Encoding DocumentEncoding)
	{
		void* buffer = 0;
		size_t size = 0;

		// Load stream to memory (using seek-based implementation if possible, since it's faster and takes less memory)
		ParseStatus Status = (stream.tellg() < 0) ? LoadStreamDataNoseek(stream, &buffer, &size) : LoadStreamDataSeek(stream, &buffer, &size);
		if (Status != StatusOk) return make_ParseResult(Status);

		return doc.LoadBufferInplaceOwn(buffer, size, options, DocumentEncoding);
	}


#if defined(PUGI__MSVC_CRT_VERSION) || defined(__BORLANDC__) || (defined(__MINGW32__) && !defined(__STRICT_ANSI__))
	PUGI__FN FILE* open_file_wide(const wchar_t* Path, const wchar_t* mode)
	{
		return _wfopen(Path, mode);
	}
#else
	PUGI__FN char* convert_Path_heap(const wchar_t* str)
	{
		assert(str);

		// first pass: get length in utf8 characters
		size_t length = wcslen(str);
		size_t size = AsUtf8_begin(str, length);

		// allocate Resulting string
		char* Result = static_cast<char*>(Memory::allocate(size + 1));
		if (!Result) return 0;

		// second pass: convert to utf8
		AsUtf8_end(Result, size, str, length);

		return Result;
	}

	PUGI__FN FILE* open_file_wide(const wchar_t* Path, const wchar_t* mode)
	{
		// there is no standard function to open wide Paths, so our best bet is to try utf8 Path
		char* Path_utf8 = convert_Path_heap(Path);
		if (!Path_utf8) return 0;

		// convert mode to ASCII (we mirror _wfopen interface)
		char mode_ascii[4] = {0};
		for (size_t i = 0; mode[i]; ++i) mode_ascii[i] = static_cast<char>(mode[i]);

		// try to open the utf8 Path
		FILE* Result = fopen(Path_utf8, mode_ascii);

		// free dummy buffer
		Memory::deallocate(Path_utf8);

		return Result;
	}
#endif

    PUGI__FN bool SaveFileImpl(const Document& doc, FILE* file, const Char8* indent, unsigned int flags, Encoding DocumentEncoding)
	{
		if (!file) return false;

		WriterFile WriterInstance(file);
		doc.Save(WriterInstance, indent, flags, DocumentEncoding);

		int Result = ferror(file);

		fclose(file);

		return Result == 0;
	}
PUGI__NS_END

namespace XML
{
    #ifndef SWIG_SAFE
    PUGI__FN WriterFile::WriterFile(void* FilePtr): TargetFile(FilePtr)
	{
	}

	PUGI__FN void WriterFile::Write(const void* data, size_t size)
	{
        size_t Result = fwrite(data, 1, size, static_cast<FILE*>(TargetFile));
		(void)!Result; // unfortunately we can't do proper error handling here
	}


	PUGI__FN WriterStream::WriterStream(std::basic_ostream<char, std::char_traits<char> >& stream): narrow_stream(&stream), wide_stream(0)
	{
	}

	PUGI__FN WriterStream::WriterStream(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& stream): narrow_stream(0), wide_stream(&stream)
	{
	}

	PUGI__FN void WriterStream::Write(const void* data, size_t size)
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
    #endif //SWIG_SAFE

	PUGI__FN TreeWalker::TreeWalker(): _Depth(0)
	{
	}

	PUGI__FN TreeWalker::~TreeWalker()
	{
	}

	PUGI__FN int TreeWalker::Depth() const
	{
		return _Depth;
	}

	PUGI__FN bool TreeWalker::begin(Node&)
	{
		return true;
	}

	PUGI__FN bool TreeWalker::end(Node&)
	{
		return true;
	}

	PUGI__FN Attribute::Attribute(): AttributeData(0)
	{
	}

	PUGI__FN Attribute::Attribute(AttributeStruct* attr): AttributeData(attr)
	{
	}

	PUGI__FN static void unspecified_bool_Attribute(Attribute***)
	{
	}

	PUGI__FN Attribute::operator Attribute::unspecified_bool_type() const
	{
		return AttributeData ? unspecified_bool_Attribute : 0;
	}

	PUGI__FN bool Attribute::operator!() const
	{
		return !AttributeData;
	}

	PUGI__FN bool Attribute::operator==(const Attribute& r) const
	{
		return (AttributeData == r.AttributeData);
	}

	PUGI__FN bool Attribute::operator!=(const Attribute& r) const
	{
		return (AttributeData != r.AttributeData);
	}

	PUGI__FN bool Attribute::operator<(const Attribute& r) const
	{
		return (AttributeData < r.AttributeData);
	}

	PUGI__FN bool Attribute::operator>(const Attribute& r) const
	{
		return (AttributeData > r.AttributeData);
	}

	PUGI__FN bool Attribute::operator<=(const Attribute& r) const
	{
		return (AttributeData <= r.AttributeData);
	}

	PUGI__FN bool Attribute::operator>=(const Attribute& r) const
	{
		return (AttributeData >= r.AttributeData);
	}

	PUGI__FN Attribute Attribute::GetNextAttribute() const
	{
		return AttributeData ? Attribute(AttributeData->GetNextAttribute) : Attribute();
	}

	PUGI__FN Attribute Attribute::GetPreviousAttribute() const
	{
		return AttributeData && AttributeData->prev_attribute_c->GetNextAttribute ? Attribute(AttributeData->prev_attribute_c) : Attribute();
	}

    PUGI__FN const Char8* Attribute::AsString(const Char8* def) const
	{
		return (AttributeData && AttributeData->Value) ? AttributeData->Value : def;
	}

	PUGI__FN int Attribute::AsInt(int def) const
	{
		return internal::GetValue_int(AttributeData ? AttributeData->Value : 0, def);
	}

	PUGI__FN unsigned int Attribute::AsUint(unsigned int def) const
	{
		return internal::GetValue_uint(AttributeData ? AttributeData->Value : 0, def);
	}

	PUGI__FN double Attribute::AsDouble(double def) const
	{
		return internal::GetValue_double(AttributeData ? AttributeData->Value : 0, def);
	}

	PUGI__FN Whole Attribute::AsWhole(Whole def) const
		{ return (AttributeData ? ToWhole(AttributeData->Value) : def); }

	PUGI__FN Integer Attribute::AsInteger(Integer def) const
		{ return (AttributeData ? ToInteger(AttributeData->Value) : def); }

	PUGI__FN  Real Attribute::AsReal(Real def) const
		{ return (AttributeData ? ToReal(AttributeData->Value) : def); }

	PUGI__FN float Attribute::AsFloat(float def) const
	{
		return internal::GetValue_float(AttributeData ? AttributeData->Value : 0, def);
	}

	PUGI__FN bool Attribute::AsBool(bool def) const
	{
		return internal::GetValue_bool(AttributeData ? AttributeData->Value : 0, def);
	}

	PUGI__FN bool Attribute::Empty() const
	{
		return !AttributeData;
	}

    PUGI__FN const Char8* Attribute::Name() const
	{
        return (AttributeData && AttributeData->Name) ? AttributeData->Name : "";
	}

    PUGI__FN const Char8* Attribute::Value() const
	{
        return (AttributeData && AttributeData->Value) ? AttributeData->Value : "";
	}

	PUGI__FN size_t Attribute::HashValue() const
	{
		return static_cast<size_t>(reinterpret_cast<uintptr_t>(AttributeData) / sizeof(AttributeStruct));
	}

	PUGI__FN AttributeStruct* Attribute::InternalObject() const
	{
		return AttributeData;
	}

    PUGI__FN Attribute& Attribute::operator=(const Char8* rhs)
	{
		SetValue(rhs);
		return *this;
	}

	PUGI__FN Attribute& Attribute::operator=(int rhs)
	{
		SetValue(rhs);
		return *this;
	}

	PUGI__FN Attribute& Attribute::operator=(unsigned int rhs)
	{
		SetValue(rhs);
		return *this;
	}

	PUGI__FN Attribute& Attribute::operator=(double rhs)
	{
		SetValue(rhs);
		return *this;
	}

	PUGI__FN Attribute& Attribute::operator=(bool rhs)
	{
		SetValue(rhs);
		return *this;
	}

    PUGI__FN bool Attribute::SetName(const Char8* rhs)
	{
		if (!AttributeData) return false;

		return internal::strcpy_insitu(AttributeData->Name, AttributeData->header, internal::MemoryPage_Name_allocated_mask, rhs);
	}

    PUGI__FN bool Attribute::SetValue(const Char8* rhs)
	{
		if (!AttributeData) return false;

		return internal::strcpy_insitu(AttributeData->Value, AttributeData->header, internal::MemoryPage_Value_allocated_mask, rhs);
	}

	PUGI__FN bool Attribute::SetValue(int rhs)
	{
		if (!AttributeData) return false;

		return internal::SetValue_convert(AttributeData->Value, AttributeData->header, internal::MemoryPage_Value_allocated_mask, rhs);
	}

	PUGI__FN bool Attribute::SetValue(unsigned int rhs)
	{
		if (!AttributeData) return false;

		return internal::SetValue_convert(AttributeData->Value, AttributeData->header, internal::MemoryPage_Value_allocated_mask, rhs);
	}

	PUGI__FN bool Attribute::SetValue(double rhs)
	{
		if (!AttributeData) return false;

		return internal::SetValue_convert(AttributeData->Value, AttributeData->header, internal::MemoryPage_Value_allocated_mask, rhs);
	}

	PUGI__FN bool Attribute::SetValue(bool rhs)
	{
		if (!AttributeData) return false;

		return internal::SetValue_convert(AttributeData->Value, AttributeData->header, internal::MemoryPage_Value_allocated_mask, rhs);
	}

#ifdef __BORLANDC__
	PUGI__FN bool operator&&(const Attribute& lhs, bool rhs)
	{
		return (bool)lhs && rhs;
	}

	PUGI__FN bool operator||(const Attribute& lhs, bool rhs)
	{
		return (bool)lhs || rhs;
	}
#endif

	PUGI__FN Node::Node(): NodeData(0)
	{
	}

	PUGI__FN Node::Node(NodeStruct* p): NodeData(p)
	{
	}

	PUGI__FN static void unspecified_bool_Node(Node***)
	{
	}

	PUGI__FN Node::operator Node::unspecified_bool_type() const
	{
		return NodeData ? unspecified_bool_Node : 0;
	}

	PUGI__FN bool Node::operator!() const
	{
		return !NodeData;
	}

	PUGI__FN Node::iterator Node::begin() const
	{
		return iterator(NodeData ? NodeData->GetFirstChild : 0, NodeData);
	}

	PUGI__FN Node::iterator Node::end() const
	{
		return iterator(0, NodeData);
	}

	PUGI__FN Node::attribute_iterator Node::attributes_begin() const
	{
		return attribute_iterator(NodeData ? NodeData->GetFirstAttribute : 0, NodeData);
	}

	PUGI__FN Node::attribute_iterator Node::attributes_end() const
	{
		return attribute_iterator(0, NodeData);
	}

	PUGI__FN ObjectRange<NodeIterator> Node::GetChildren() const
	{
		return ObjectRange<NodeIterator>(begin(), end());
	}

    PUGI__FN ObjectRange<NamedNodeIterator> Node::GetChildren(const Char8* Name_) const
	{
        return ObjectRange<NamedNodeIterator>(NamedNodeIterator(GetChild(Name_), Name_), NamedNodeIterator());
	}

	PUGI__FN ObjectRange<AttributeIterator> Node::attributes() const
	{
		return ObjectRange<AttributeIterator>(attributes_begin(), attributes_end());
	}

	PUGI__FN bool Node::operator==(const Node& r) const
	{
		return (NodeData == r.NodeData);
	}

	PUGI__FN bool Node::operator!=(const Node& r) const
	{
		return (NodeData != r.NodeData);
	}

	PUGI__FN bool Node::operator<(const Node& r) const
	{
		return (NodeData < r.NodeData);
	}

	PUGI__FN bool Node::operator>(const Node& r) const
	{
		return (NodeData > r.NodeData);
	}

	PUGI__FN bool Node::operator<=(const Node& r) const
	{
		return (NodeData <= r.NodeData);
	}

	PUGI__FN bool Node::operator>=(const Node& r) const
	{
		return (NodeData >= r.NodeData);
	}

	PUGI__FN bool Node::Empty() const
	{
		return !NodeData;
	}

    PUGI__FN const Char8* Node::Name() const
	{
        return (NodeData && NodeData->Name) ? NodeData->Name : "";
	}

	PUGI__FN NodeType Node::Type() const
	{
		return NodeData ? static_cast<NodeType>((NodeData->header & internal::MemoryPage_type_mask) + 1) : NodeNull;
	}

    PUGI__FN const Char8* Node::Value() const
	{
        return (NodeData && NodeData->Value) ? NodeData->Value : "";
	}

    PUGI__FN Node Node::GetChild(const Char8* Name_) const
	{
		if (!NodeData) return Node();

		for (NodeStruct* i = NodeData->GetFirstChild; i; i = i->GetNextSibling)
			if (i->Name && internal::strequal(Name_, i->Name)) return Node(i);

		return Node();
	}

    PUGI__FN Attribute Node::GetAttribute(const Char8* Name_) const
	{
		if (!NodeData) return Attribute();

		for (AttributeStruct* i = NodeData->GetFirstAttribute; i; i = i->GetNextAttribute)
			if (i->Name && internal::strequal(Name_, i->Name))
				return Attribute(i);

		return Attribute();
	}

    PUGI__FN Node Node::GetNextSibling(const Char8* Name_) const
	{
		if (!NodeData) return Node();

		for (NodeStruct* i = NodeData->GetNextSibling; i; i = i->GetNextSibling)
			if (i->Name && internal::strequal(Name_, i->Name)) return Node(i);

		return Node();
	}

	PUGI__FN Node Node::GetNextSibling() const
	{
		if (!NodeData) return Node();

		if (NodeData->GetNextSibling) return Node(NodeData->GetNextSibling);
		else return Node();
	}

    PUGI__FN Node Node::GetPreviousSibling(const Char8* Name_) const
	{
		if (!NodeData) return Node();

		for (NodeStruct* i = NodeData->prev_sibling_c; i->GetNextSibling; i = i->prev_sibling_c)
			if (i->Name && internal::strequal(Name_, i->Name)) return Node(i);

		return Node();
	}

	PUGI__FN Node Node::GetPreviousSibling() const
	{
		if (!NodeData) return Node();

		if (NodeData->prev_sibling_c->GetNextSibling) return Node(NodeData->prev_sibling_c);
		else return Node();
	}

	PUGI__FN Node Node::GetParent() const
	{
		return NodeData ? Node(NodeData->GetParent) : Node();
	}

	PUGI__FN Node Node::GetRoot() const
	{
		if (!NodeData) return Node();

		internal::MemoryPage* page = reinterpret_cast<internal::MemoryPage*>(NodeData->header & internal::MemoryPage_pointer_mask);

		return Node(static_cast<internal::DocumentStruct*>(page->allocator));
	}

    PUGI__FN NodeText Node::GetText() const
	{
        return NodeText(NodeData);
	}

    PUGI__FN const Char8* Node::GetChildValue() const
	{
        if (!NodeData) return "";

		for (NodeStruct* i = NodeData->GetFirstChild; i; i = i->GetNextSibling)
			if (i->Value && internal::is_text_node(i))
				return i->Value;

        return "";
	}

    PUGI__FN const Char8* Node::GetChildValue(const Char8* Name_) const
	{
		return GetChild(Name_).GetChildValue();
	}

	PUGI__FN Attribute Node::GetFirstAttribute() const
	{
		return NodeData ? Attribute(NodeData->GetFirstAttribute) : Attribute();
	}

	PUGI__FN Attribute Node::GetLastAttribute() const
	{
		return NodeData && NodeData->GetFirstAttribute ? Attribute(NodeData->GetFirstAttribute->prev_attribute_c) : Attribute();
	}

	PUGI__FN Node Node::GetFirstChild() const
	{
		return NodeData ? Node(NodeData->GetFirstChild) : Node();
	}

	PUGI__FN Node Node::GetLastChild() const
	{
		return NodeData && NodeData->GetFirstChild ? Node(NodeData->GetFirstChild->prev_sibling_c) : Node();
	}

    PUGI__FN bool Node::SetName(const Char8* rhs)
	{
		switch (Type())
		{
		case NodePi:
		case NodeDeclaration:
		case NodeElement:
			return internal::strcpy_insitu(NodeData->Name, NodeData->header, internal::MemoryPage_Name_allocated_mask, rhs);

		default:
			return false;
		}
	}

    PUGI__FN bool Node::SetValue(const Char8* rhs)
	{
		switch (Type())
		{
		case NodePi:
		case NodeCdata:
		case NodePcdata:
		case NodeComment:
		case NodeDocType:
			return internal::strcpy_insitu(NodeData->Value, NodeData->header, internal::MemoryPage_Value_allocated_mask, rhs);

		default:
			return false;
		}
	}

    PUGI__FN Attribute Node::AppendAttribute(const Char8* Name_)
	{
		if (Type() != NodeElement && Type() != NodeDeclaration) return Attribute();

		Attribute a(internal::AppendAttribute_ll(NodeData, internal::GetAllocator(NodeData)));
		a.SetName(Name_);

		return a;
	}

    PUGI__FN Attribute Node::PrependAttribute(const Char8* Name_)
	{
		if (Type() != NodeElement && Type() != NodeDeclaration) return Attribute();

		Attribute a(internal::allocate_attribute(internal::GetAllocator(NodeData)));
		if (!a) return Attribute();

		a.SetName(Name_);

		AttributeStruct* head = NodeData->GetFirstAttribute;

		if (head)
		{
			a.AttributeData->prev_attribute_c = head->prev_attribute_c;
			head->prev_attribute_c = a.AttributeData;
		}
		else
			a.AttributeData->prev_attribute_c = a.AttributeData;

		a.AttributeData->GetNextAttribute = head;
		NodeData->GetFirstAttribute = a.AttributeData;

		return a;
	}

    PUGI__FN Attribute Node::InsertAttributeBefore(const Char8* Name_, const Attribute& attr)
	{
		if ((Type() != NodeElement && Type() != NodeDeclaration) || attr.Empty()) return Attribute();

		// check that GetAttribute belongs to *this
		AttributeStruct* cur = attr.AttributeData;

		while (cur->prev_attribute_c->GetNextAttribute) cur = cur->prev_attribute_c;

		if (cur != NodeData->GetFirstAttribute) return Attribute();

		Attribute a(internal::allocate_attribute(internal::GetAllocator(NodeData)));
		if (!a) return Attribute();

		a.SetName(Name_);

		if (attr.AttributeData->prev_attribute_c->GetNextAttribute)
			attr.AttributeData->prev_attribute_c->GetNextAttribute = a.AttributeData;
		else
			NodeData->GetFirstAttribute = a.AttributeData;

		a.AttributeData->prev_attribute_c = attr.AttributeData->prev_attribute_c;
		a.AttributeData->GetNextAttribute = attr.AttributeData;
		attr.AttributeData->prev_attribute_c = a.AttributeData;

		return a;
	}

    PUGI__FN Attribute Node::InsertAttributeAfter(const Char8* Name_, const Attribute& attr)
	{
		if ((Type() != NodeElement && Type() != NodeDeclaration) || attr.Empty()) return Attribute();

		// check that GetAttribute belongs to *this
		AttributeStruct* cur = attr.AttributeData;

		while (cur->prev_attribute_c->GetNextAttribute) cur = cur->prev_attribute_c;

		if (cur != NodeData->GetFirstAttribute) return Attribute();

		Attribute a(internal::allocate_attribute(internal::GetAllocator(NodeData)));
		if (!a) return Attribute();

		a.SetName(Name_);

		if (attr.AttributeData->GetNextAttribute)
			attr.AttributeData->GetNextAttribute->prev_attribute_c = a.AttributeData;
		else
			NodeData->GetFirstAttribute->prev_attribute_c = a.AttributeData;

		a.AttributeData->GetNextAttribute = attr.AttributeData->GetNextAttribute;
		a.AttributeData->prev_attribute_c = attr.AttributeData;
		attr.AttributeData->GetNextAttribute = a.AttributeData;

		return a;
	}

	PUGI__FN Attribute Node::AppendCopy(const Attribute& proto)
	{
		if (!proto) return Attribute();

		Attribute Result = AppendAttribute(proto.Name());
		Result.SetValue(proto.Value());

		return Result;
	}

	PUGI__FN Attribute Node::PrependCopy(const Attribute& proto)
	{
		if (!proto) return Attribute();

		Attribute Result = PrependAttribute(proto.Name());
		Result.SetValue(proto.Value());

		return Result;
	}

	PUGI__FN Attribute Node::InsertCopyAfter(const Attribute& proto, const Attribute& attr)
	{
		if (!proto) return Attribute();

		Attribute Result = InsertAttributeAfter(proto.Name(), attr);
		Result.SetValue(proto.Value());

		return Result;
	}

	PUGI__FN Attribute Node::InsertCopyBefore(const Attribute& proto, const Attribute& attr)
	{
		if (!proto) return Attribute();

		Attribute Result = InsertAttributeBefore(proto.Name(), attr);
		Result.SetValue(proto.Value());

		return Result;
	}

	PUGI__FN Node Node::AppendChild(NodeType Type_)
	{
		if (!internal::allow_InsertChild(this->Type(), Type_)) return Node();

		Node n(internal::AppendNode(NodeData, internal::GetAllocator(NodeData), Type_));

        if (Type_ == NodeDeclaration) n.SetName("xml");

		return n;
	}

	PUGI__FN Node Node::PrependChild(NodeType Type_)
	{
		if (!internal::allow_InsertChild(this->Type(), Type_)) return Node();

		Node n(internal::allocate_node(internal::GetAllocator(NodeData), Type_));
		if (!n) return Node();

		n.NodeData->GetParent = NodeData;

		NodeStruct* head = NodeData->GetFirstChild;

		if (head)
		{
			n.NodeData->prev_sibling_c = head->prev_sibling_c;
			head->prev_sibling_c = n.NodeData;
		}
		else
			n.NodeData->prev_sibling_c = n.NodeData;

		n.NodeData->GetNextSibling = head;
		NodeData->GetFirstChild = n.NodeData;

        if (Type_ == NodeDeclaration) n.SetName("xml");

		return n;
	}

	PUGI__FN Node Node::InsertChildBefore(NodeType Type_, const Node& node)
	{
		if (!internal::allow_InsertChild(this->Type(), Type_)) return Node();
		if (!node.NodeData || node.NodeData->GetParent != NodeData) return Node();

		Node n(internal::allocate_node(internal::GetAllocator(NodeData), Type_));
		if (!n) return Node();

		n.NodeData->GetParent = NodeData;

		if (node.NodeData->prev_sibling_c->GetNextSibling)
			node.NodeData->prev_sibling_c->GetNextSibling = n.NodeData;
		else
			NodeData->GetFirstChild = n.NodeData;

		n.NodeData->prev_sibling_c = node.NodeData->prev_sibling_c;
		n.NodeData->GetNextSibling = node.NodeData;
		node.NodeData->prev_sibling_c = n.NodeData;

        if (Type_ == NodeDeclaration) n.SetName("xml");

		return n;
	}

	PUGI__FN Node Node::InsertChildAfter(NodeType Type_, const Node& node)
	{
		if (!internal::allow_InsertChild(this->Type(), Type_)) return Node();
		if (!node.NodeData || node.NodeData->GetParent != NodeData) return Node();

		Node n(internal::allocate_node(internal::GetAllocator(NodeData), Type_));
		if (!n) return Node();

		n.NodeData->GetParent = NodeData;

		if (node.NodeData->GetNextSibling)
			node.NodeData->GetNextSibling->prev_sibling_c = n.NodeData;
		else
			NodeData->GetFirstChild->prev_sibling_c = n.NodeData;

		n.NodeData->GetNextSibling = node.NodeData->GetNextSibling;
		n.NodeData->prev_sibling_c = node.NodeData;
		node.NodeData->GetNextSibling = n.NodeData;

        if (Type_ == NodeDeclaration) n.SetName("xml");

		return n;
	}

    PUGI__FN Node Node::AppendChild(const Char8* Name_)
	{
		Node Result = AppendChild(NodeElement);

		Result.SetName(Name_);

		return Result;
	}

    PUGI__FN Node Node::PrependChild(const Char8* Name_)
	{
		Node Result = PrependChild(NodeElement);

		Result.SetName(Name_);

		return Result;
	}

    PUGI__FN Node Node::InsertChildAfter(const Char8* Name_, const Node& node)
	{
		Node Result = InsertChildAfter(NodeElement, node);

		Result.SetName(Name_);

		return Result;
	}

    PUGI__FN Node Node::InsertChildBefore(const Char8* Name_, const Node& node)
	{
		Node Result = InsertChildBefore(NodeElement, node);

		Result.SetName(Name_);

		return Result;
	}

	PUGI__FN Node Node::AppendCopy(const Node& proto)
	{
		Node Result = AppendChild(proto.Type());

		if (Result) internal::recursive_copy_skip(Result, proto, Result);

		return Result;
	}

	PUGI__FN Node Node::PrependCopy(const Node& proto)
	{
		Node Result = PrependChild(proto.Type());

		if (Result) internal::recursive_copy_skip(Result, proto, Result);

		return Result;
	}

	PUGI__FN Node Node::InsertCopyAfter(const Node& proto, const Node& node)
	{
		Node Result = InsertChildAfter(proto.Type(), node);

		if (Result) internal::recursive_copy_skip(Result, proto, Result);

		return Result;
	}

	PUGI__FN Node Node::InsertCopyBefore(const Node& proto, const Node& node)
	{
		Node Result = InsertChildBefore(proto.Type(), node);

		if (Result) internal::recursive_copy_skip(Result, proto, Result);

		return Result;
	}

    PUGI__FN bool Node::RemoveAttribute(const Char8* Name_)
	{
		return RemoveAttribute(GetAttribute(Name_));
	}

	PUGI__FN bool Node::RemoveAttribute(const Attribute& a)
	{
		if (!NodeData || !a.AttributeData) return false;

		// check that GetAttribute belongs to *this
		AttributeStruct* attr = a.AttributeData;

		while (attr->prev_attribute_c->GetNextAttribute) attr = attr->prev_attribute_c;

		if (attr != NodeData->GetFirstAttribute) return false;

		if (a.AttributeData->GetNextAttribute) a.AttributeData->GetNextAttribute->prev_attribute_c = a.AttributeData->prev_attribute_c;
		else if (NodeData->GetFirstAttribute) NodeData->GetFirstAttribute->prev_attribute_c = a.AttributeData->prev_attribute_c;

		if (a.AttributeData->prev_attribute_c->GetNextAttribute) a.AttributeData->prev_attribute_c->GetNextAttribute = a.AttributeData->GetNextAttribute;
		else NodeData->GetFirstAttribute = a.AttributeData->GetNextAttribute;

		internal::destroy_attribute(a.AttributeData, internal::GetAllocator(NodeData));

		return true;
	}

    PUGI__FN bool Node::RemoveChild(const Char8* Name_)
	{
		return RemoveChild(GetChild(Name_));
	}

	PUGI__FN bool Node::RemoveChild(const Node& n)
	{
		if (!NodeData || !n.NodeData || n.NodeData->GetParent != NodeData) return false;

		if (n.NodeData->GetNextSibling) n.NodeData->GetNextSibling->prev_sibling_c = n.NodeData->prev_sibling_c;
		else if (NodeData->GetFirstChild) NodeData->GetFirstChild->prev_sibling_c = n.NodeData->prev_sibling_c;

		if (n.NodeData->prev_sibling_c->GetNextSibling) n.NodeData->prev_sibling_c->GetNextSibling = n.NodeData->GetNextSibling;
		else NodeData->GetFirstChild = n.NodeData->GetNextSibling;

		internal::destroy_node(n.NodeData, internal::GetAllocator(NodeData));

		return true;
	}

    PUGI__FN Node Node::FindChildbyAttribute(const Char8* Name_, const Char8* AttrName, const Char8* AttrValue) const
	{
		if (!NodeData) return Node();

		for (NodeStruct* i = NodeData->GetFirstChild; i; i = i->GetNextSibling)
			if (i->Name && internal::strequal(Name_, i->Name))
			{
				for (AttributeStruct* a = i->GetFirstAttribute; a; a = a->GetNextAttribute)
					if (internal::strequal(AttrName, a->Name) && internal::strequal(AttrValue, a->Value))
						return Node(i);
			}

		return Node();
	}

    PUGI__FN Node Node::FindChildbyAttribute(const Char8* AttrName, const Char8* AttrValue) const
	{
		if (!NodeData) return Node();

		for (NodeStruct* i = NodeData->GetFirstChild; i; i = i->GetNextSibling)
			for (AttributeStruct* a = i->GetFirstAttribute; a; a = a->GetNextAttribute)
				if (internal::strequal(AttrName, a->Name) && internal::strequal(AttrValue, a->Value))
					return Node(i);

		return Node();
	}

    PUGI__FN String Node::Path(Char8 delimiter) const
	{
		Node cursor = *this; // Make a copy.

		String Result = cursor.Name();

		while (cursor.GetParent())
		{
			cursor = cursor.GetParent();

			String temp = cursor.Name();
			temp += delimiter;
			temp += Result;
			Result.swap(temp);
		}

		return Result;
	}

    PUGI__FN Node Node::FirstElementByPath(const Char8* Path_, Char8 delimiter) const
	{
		Node found = *this; // Current search context.

		if (!NodeData || !Path_ || !Path_[0]) return found;

		if (Path_[0] == delimiter)
		{
			// Absolute Path; e.g. '/foo/bar'
			found = found.GetRoot();
			++Path_;
		}

        const Char8* Path_segment = Path_;

		while (*Path_segment == delimiter) ++Path_segment;

        const Char8* Path_segment_end = Path_segment;

		while (*Path_segment_end && *Path_segment_end != delimiter) ++Path_segment_end;

		if (Path_segment == Path_segment_end) return found;

        const Char8* NextSegment = Path_segment_end;

		while (*NextSegment == delimiter) ++NextSegment;

		if (*Path_segment == '.' && Path_segment + 1 == Path_segment_end)
			return found.FirstElementByPath(NextSegment, delimiter);
		else if (*Path_segment == '.' && *(Path_segment+1) == '.' && Path_segment + 2 == Path_segment_end)
			return found.GetParent().FirstElementByPath(NextSegment, delimiter);
		else
		{
			for (NodeStruct* j = found.NodeData->GetFirstChild; j; j = j->GetNextSibling)
			{
				if (j->Name && internal::strequalrange(j->Name, Path_segment, static_cast<size_t>(Path_segment_end - Path_segment)))
				{
					Node subsearch = Node(j).FirstElementByPath(NextSegment, delimiter);

					if (subsearch) return subsearch;
				}
			}

			return Node();
		}
	}

	PUGI__FN bool Node::Traverse(TreeWalker& walker)
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
					while (!cur.GetNextSibling() && cur != *this && !cur.GetParent().Empty())
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

	PUGI__FN size_t Node::HashValue() const
	{
		return static_cast<size_t>(reinterpret_cast<uintptr_t>(NodeData) / sizeof(NodeStruct));
	}

	PUGI__FN NodeStruct* Node::InternalObject() const
	{
		return NodeData;
	}

    PUGI__FN void Node::Print(Writer& WriterInstance, const Char8* indent, unsigned int flags, Encoding DocumentEncoding, unsigned int Depth) const
	{
		if (!NodeData) return;

		internal::BufferedWriter buffered_WriterInstance(WriterInstance, DocumentEncoding);

		internal::NodeOutput(buffered_WriterInstance, *this, indent, flags, Depth);
	}

    PUGI__FN void Node::Print(std::basic_ostream<char, std::char_traits<char> >& stream, const Char8* indent, unsigned int flags, Encoding DocumentEncoding, unsigned int Depth) const
	{
		WriterStream WriterInstance(stream);

		Print(WriterInstance, indent, flags, DocumentEncoding, Depth);
	}

    PUGI__FN void Node::Print(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& stream, const Char8* indent, unsigned int flags, unsigned int Depth) const
	{
		WriterStream WriterInstance(stream);

		Print(WriterInstance, indent, flags, Encodingwchar_t, Depth);
	}

	PUGI__FN ptrdiff_t Node::OffSetDebug() const
	{
		NodeStruct* r = GetRoot().NodeData;

		if (!r) return -1;

        const Char8* buffer = static_cast<internal::DocumentStruct*>(r)->buffer;

		if (!buffer) return -1;

		switch (Type())
		{
		case NodeDocument:
			return 0;

		case NodeElement:
		case NodeDeclaration:
		case NodePi:
			return (NodeData->header & internal::MemoryPage_Name_allocated_mask) ? -1 : NodeData->Name - buffer;

		case NodePcdata:
		case NodeCdata:
		case NodeComment:
		case NodeDocType:
			return (NodeData->header & internal::MemoryPage_Value_allocated_mask) ? -1 : NodeData->Value - buffer;

		default:
			return -1;
		}
	}

#ifdef __BORLANDC__
	PUGI__FN bool operator&&(const Node& lhs, bool rhs)
	{
		return (bool)lhs && rhs;
	}

	PUGI__FN bool operator||(const Node& lhs, bool rhs)
	{
		return (bool)lhs || rhs;
	}
#endif

    PUGI__FN NodeText::NodeText(NodeStruct* OtherRoot): RootNode(OtherRoot)
	{
	}

    PUGI__FN NodeStruct* NodeText::Data() const
	{
        if (!RootNode || internal::is_text_node(RootNode)) return RootNode;

        for (NodeStruct* node = RootNode->GetFirstChild; node; node = node->GetNextSibling)
			if (internal::is_text_node(node))
				return node;

		return 0;
	}

    PUGI__FN NodeStruct* NodeText::DataNew()
	{
        NodeStruct* d = Data();
		if (d) return d;

        return Node(RootNode).AppendChild(NodePcdata).InternalObject();
	}

    PUGI__FN NodeText::NodeText(): RootNode(0)
	{
	}

    PUGI__FN static void unspecified_bool_Text(NodeText***)
	{
	}

    PUGI__FN NodeText::operator NodeText::unspecified_bool_type() const
	{
        return Data() ? unspecified_bool_Text : 0;
	}

    PUGI__FN bool NodeText::operator!() const
	{
        return !Data();
	}

    PUGI__FN bool NodeText::Empty() const
	{
        return Data() == 0;
	}

    PUGI__FN const Char8* NodeText::GetString() const
	{
        NodeStruct* d = Data();

        return (d && d->Value) ? d->Value : "";
	}

    PUGI__FN const Char8* NodeText::AsString(const Char8* def) const
	{
        NodeStruct* d = Data();

		return (d && d->Value) ? d->Value : def;
	}

    PUGI__FN int NodeText::AsInt(int def) const
	{
        NodeStruct* d = Data();

		return internal::GetValue_int(d ? d->Value : 0, def);
	}

    PUGI__FN unsigned int NodeText::AsUint(unsigned int def) const
	{
        NodeStruct* d = Data();

		return internal::GetValue_uint(d ? d->Value : 0, def);
	}

    PUGI__FN double NodeText::AsDouble(double def) const
	{
        NodeStruct* d = Data();

		return internal::GetValue_double(d ? d->Value : 0, def);
	}

    PUGI__FN float NodeText::AsFloat(float def) const
	{
        NodeStruct* d = Data();

		return internal::GetValue_float(d ? d->Value : 0, def);
	}

    PUGI__FN Real NodeText::AsReal(Real def) const
    {
        return AsFloat(def);
    }

    PUGI__FN Whole NodeText::AsWhole(Whole def) const
    {
        return AsUint(def);
    }

    PUGI__FN Integer NodeText::AsInteger(Integer def) const
    {
        return AsInt(def);
    }

    PUGI__FN bool NodeText::AsBool(bool def) const
	{
        NodeStruct* d = Data();

		return internal::GetValue_bool(d ? d->Value : 0, def);
	}

    PUGI__FN bool NodeText::Set(const Char8* rhs)
	{
        NodeStruct* dn = DataNew();

		return dn ? internal::strcpy_insitu(dn->Value, dn->header, internal::MemoryPage_Value_allocated_mask, rhs) : false;
	}

    PUGI__FN bool NodeText::Set(int rhs)
	{
        NodeStruct* dn = DataNew();

		return dn ? internal::SetValue_convert(dn->Value, dn->header, internal::MemoryPage_Value_allocated_mask, rhs) : false;
	}

    PUGI__FN bool NodeText::Set(unsigned int rhs)
	{
        NodeStruct* dn = DataNew();

		return dn ? internal::SetValue_convert(dn->Value, dn->header, internal::MemoryPage_Value_allocated_mask, rhs) : false;
	}

    PUGI__FN bool NodeText::Set(double rhs)
	{
        NodeStruct* dn = DataNew();

		return dn ? internal::SetValue_convert(dn->Value, dn->header, internal::MemoryPage_Value_allocated_mask, rhs) : false;
	}

    PUGI__FN bool NodeText::Set(bool rhs)
	{
        NodeStruct* dn = DataNew();

		return dn ? internal::SetValue_convert(dn->Value, dn->header, internal::MemoryPage_Value_allocated_mask, rhs) : false;
	}

    PUGI__FN NodeText& NodeText::operator=(const Char8* rhs)
	{
		Set(rhs);
		return *this;
	}

    PUGI__FN NodeText& NodeText::operator=(int rhs)
	{
		Set(rhs);
		return *this;
	}

    PUGI__FN NodeText& NodeText::operator=(unsigned int rhs)
	{
		Set(rhs);
		return *this;
	}

    PUGI__FN NodeText& NodeText::operator=(double rhs)
	{
		Set(rhs);
		return *this;
	}

    PUGI__FN NodeText& NodeText::operator=(bool rhs)
	{
		Set(rhs);
		return *this;
	}

    PUGI__FN Node NodeText::data() const
	{
        return Node(Data());
	}

#ifdef __BORLANDC__
	PUGI__FN bool operator&&(const Text& lhs, bool rhs)
	{
		return (bool)lhs && rhs;
	}

	PUGI__FN bool operator||(const Text& lhs, bool rhs)
	{
		return (bool)lhs || rhs;
	}
#endif

	PUGI__FN NodeIterator::NodeIterator()
	{
	}

    PUGI__FN NodeIterator::NodeIterator(const Node& node): TargetNode(node), ParentNode(node.GetParent())
	{
	}

    PUGI__FN NodeIterator::NodeIterator(NodeStruct* ref, NodeStruct* ParentNode): TargetNode(ref), ParentNode(ParentNode)
	{
	}

	PUGI__FN bool NodeIterator::operator==(const NodeIterator& rhs) const
	{
        return TargetNode.NodeData == rhs.TargetNode.NodeData && ParentNode.NodeData == rhs.ParentNode.NodeData;
	}

	PUGI__FN bool NodeIterator::operator!=(const NodeIterator& rhs) const
	{
        return TargetNode.NodeData != rhs.TargetNode.NodeData || ParentNode.NodeData != rhs.ParentNode.NodeData;
	}

	PUGI__FN Node& NodeIterator::operator*() const
	{
        assert(TargetNode.NodeData);
        return TargetNode;
	}

	PUGI__FN Node* NodeIterator::operator->() const
	{
        assert(TargetNode.NodeData);
        return const_cast<Node*>(&TargetNode); // BCC32 workaround
	}

	PUGI__FN const NodeIterator& NodeIterator::operator++()
	{
        assert(TargetNode.NodeData);
        TargetNode.NodeData = TargetNode.NodeData->GetNextSibling;
		return *this;
	}

	PUGI__FN NodeIterator NodeIterator::operator++(int)
	{
		NodeIterator temp = *this;
		++*this;
		return temp;
	}

	PUGI__FN const NodeIterator& NodeIterator::operator--()
	{
        TargetNode = TargetNode.NodeData ? TargetNode.GetPreviousSibling() : ParentNode.GetLastChild();
		return *this;
	}

	PUGI__FN NodeIterator NodeIterator::operator--(int)
	{
		NodeIterator temp = *this;
		--*this;
		return temp;
	}

	PUGI__FN AttributeIterator::AttributeIterator()
	{
	}

	PUGI__FN AttributeIterator::AttributeIterator(const Attribute& attr, const Node& GetParent): TargetAttribute(attr), ParentNode(GetParent)
	{
	}

	PUGI__FN AttributeIterator::AttributeIterator(AttributeStruct* ref, NodeStruct* GetParent): TargetAttribute(ref), ParentNode(GetParent)
	{
	}

	PUGI__FN bool AttributeIterator::operator==(const AttributeIterator& rhs) const
	{
		return TargetAttribute.AttributeData == rhs.TargetAttribute.AttributeData && ParentNode.NodeData == rhs.ParentNode.NodeData;
	}

	PUGI__FN bool AttributeIterator::operator!=(const AttributeIterator& rhs) const
	{
		return TargetAttribute.AttributeData != rhs.TargetAttribute.AttributeData || ParentNode.NodeData != rhs.ParentNode.NodeData;
	}

	PUGI__FN Attribute& AttributeIterator::operator*() const
	{
		assert(TargetAttribute.AttributeData);
		return TargetAttribute;
	}

	PUGI__FN Attribute* AttributeIterator::operator->() const
	{
		assert(TargetAttribute.AttributeData);
		return const_cast<Attribute*>(&TargetAttribute); // BCC32 workaround
	}

	PUGI__FN const AttributeIterator& AttributeIterator::operator++()
	{
		assert(TargetAttribute.AttributeData);
		TargetAttribute.AttributeData = TargetAttribute.AttributeData->GetNextAttribute;
		return *this;
	}

	PUGI__FN AttributeIterator AttributeIterator::operator++(int)
	{
		AttributeIterator temp = *this;
		++*this;
		return temp;
	}

	PUGI__FN const AttributeIterator& AttributeIterator::operator--()
	{
		TargetAttribute = TargetAttribute.AttributeData ? TargetAttribute.GetPreviousAttribute() : ParentNode.GetLastAttribute();
		return *this;
	}

	PUGI__FN AttributeIterator AttributeIterator::operator--(int)
	{
		AttributeIterator temp = *this;
		--*this;
		return temp;
	}

    PUGI__FN NamedNodeIterator::NamedNodeIterator(): TargetName(0)
	{
	}

    PUGI__FN NamedNodeIterator::NamedNodeIterator(const Node& node, const Char8* Name): TargetNode(node), TargetName(Name)
	{
	}

    PUGI__FN bool NamedNodeIterator::operator==(const NamedNodeIterator& rhs) const
	{
        return TargetNode == rhs.TargetNode;
	}

    PUGI__FN bool NamedNodeIterator::operator!=(const NamedNodeIterator& rhs) const
	{
        return TargetNode != rhs.TargetNode;
	}

    PUGI__FN Node& NamedNodeIterator::operator*() const
	{
        assert(TargetNode.NodeData);
        return TargetNode;
	}

    PUGI__FN Node* NamedNodeIterator::operator->() const
	{
        assert(TargetNode.NodeData);
        return const_cast<Node*>(&TargetNode); // BCC32 workaround
	}

    PUGI__FN const NamedNodeIterator& NamedNodeIterator::operator++()
	{
        assert(TargetNode.NodeData);
        TargetNode = TargetNode.GetNextSibling(TargetName);
		return *this;
	}

    PUGI__FN NamedNodeIterator NamedNodeIterator::operator++(int)
	{
        NamedNodeIterator temp = *this;
		++*this;
		return temp;
	}

	PUGI__FN ParseResult::ParseResult(): Status(StatusInternalError), Offset(0), DocumentEncoding(EncodingAuto)
	{
	}

	PUGI__FN ParseResult::operator bool() const
	{
		return Status == StatusOk;
	}

	PUGI__FN const char* ParseResult::Description() const
	{
		switch (Status)
		{
		case StatusOk: return "No error";

		case StatusFileNotFound: return "File was not found";
		case StatusIOError: return "Error reading from file/stream";
		case StatusOutOfMemory: return "Could not allocate memory";
		case StatusInternalError: return "Internal error occurred";

		case StatusUnrecognizedTag: return "Could not determine tag Type";

        case StatusBadProcessingInstruction: return "Error parsing document declaration/processing instruction";
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

	PUGI__FN Document::Document(): _buffer(0)
	{
		create();
	}

	PUGI__FN Document::~Document()
	{
		destroy();
	}

	PUGI__FN void Document::Reset()
	{
		destroy();
		create();
	}

	PUGI__FN void Document::Reset(const Document& proto)
	{
		Reset();

		for (Node cur = proto.GetFirstChild(); cur; cur = cur.GetNextSibling())
			AppendCopy(cur);
	}

	PUGI__FN void Document::create()
	{
		// initialize sentinel page
		PUGI__STATIC_ASSERT(offsetof(internal::MemoryPage, data) + sizeof(internal::DocumentStruct) + internal::MemoryPage_alignment <= sizeof(_memory));

		// align upwards to page boundary
		void* page_memory = reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(_memory) + (internal::MemoryPage_alignment - 1)) & ~(internal::MemoryPage_alignment - 1));

		// prepare page structure
		internal::MemoryPage* page = internal::MemoryPage::construct(page_memory);

		page->busy_size = internal::MemoryPage_size;

		// allocate new GetRoot
		NodeData = new (page->data) internal::DocumentStruct(page);
		NodeData->prev_sibling_c = NodeData;

		// setup sentinel page
		page->allocator = static_cast<internal::DocumentStruct*>(NodeData);
	}

	PUGI__FN ParseResult Document::Load(Resource::DataStream& stream, unsigned int options, Encoding DocumentEncoding)
	{
	    Reset();
	    return internal::LoadDataStreamImpl(*this, stream, options, DocumentEncoding);
	}

    PUGI__FN void Document::Save(Resource::DataStream& stream, const Char8* indent, unsigned int flags, Encoding DocumentEncoding) const
	{
        	XMLStreamWrapper WriterInstance(&stream);
        	Save(WriterInstance, indent, flags, DocumentEncoding);
	}

	PUGI__FN void Document::destroy()
	{
		// destroy static storage
		if (_buffer)
		{
			internal::Memory::deallocate(_buffer);
			_buffer = 0;
		}

		// destroy dynamic storage, leave sentinel page (it's in static memory)
		if (NodeData)
		{
			internal::MemoryPage* GetRoot_page = reinterpret_cast<internal::MemoryPage*>(NodeData->header & internal::MemoryPage_pointer_mask);
			assert(GetRoot_page && !GetRoot_page->prev && !GetRoot_page->memory);

			// destroy all pages
			for (internal::MemoryPage* page = GetRoot_page->next; page; )
			{
				internal::MemoryPage* next = page->next;

				internal::Allocator::deallocate_page(page);

				page = next;
			}

			// cleanup GetRoot page
			GetRoot_page->allocator = 0;
			GetRoot_page->next = 0;
			GetRoot_page->busy_size = GetRoot_page->freed_size = 0;

			NodeData = 0;
		}
	}

	PUGI__FN ParseResult Document::Load(std::basic_istream<char, std::char_traits<char> >& stream, unsigned int options, Encoding DocumentEncoding)
	{
		Reset();

		return internal::LoadStreamImpl(*this, stream, options, DocumentEncoding);
	}

	PUGI__FN ParseResult Document::Load(std::basic_istream<wchar_t, std::char_traits<wchar_t> >& stream, unsigned int options)
	{
		Reset();

		return internal::LoadStreamImpl(*this, stream, options, Encodingwchar_t);
	}

    PUGI__FN ParseResult Document::Load(const Char8* contents, unsigned int options)
	{
        // Force native DocumentEncoding (skip autodetection)
		Encoding DocumentEncoding = EncodingUTF8;

        return LoadBuffer(contents, internal::strlength(contents) * sizeof(Char8), options, DocumentEncoding);
	}

	PUGI__FN ParseResult Document::LoadFile(const char* Path_, unsigned int options, Encoding DocumentEncoding)
	{
		Reset();

		FILE* file = fopen(Path_, "rb");

		return internal::LoadFileImpl(*this, file, options, DocumentEncoding);
	}

	PUGI__FN ParseResult Document::LoadFile(const wchar_t* Path_, unsigned int options, Encoding DocumentEncoding)
	{
		Reset();

		FILE* file = internal::open_file_wide(Path_, L"rb");

		return internal::LoadFileImpl(*this, file, options, DocumentEncoding);
	}

	PUGI__FN ParseResult Document::LoadBufferImpl(void* contents, size_t size, unsigned int options, Encoding DocumentEncoding, bool is_mutable, bool own)
	{
		Reset();

		// check input buffer
		assert(contents || size == 0);

		// get actual DocumentEncoding
		Encoding buffer_DocumentEncoding = internal::GetBuffer_DocumentEncoding(DocumentEncoding, contents, size);

		// get private buffer
        Char8* buffer = 0;
		size_t length = 0;

		if (!internal::convert_buffer(buffer, length, buffer_DocumentEncoding, contents, size, is_mutable)) return internal::make_ParseResult(StatusOutOfMemory);

		// delete original buffer if we performed a conversion
		if (own && buffer != contents && contents) internal::Memory::deallocate(contents);

		// parse
		ParseResult res = internal::Parser::parse(buffer, length, NodeData, options);

		// remember DocumentEncoding
		res.DocumentEncoding = buffer_DocumentEncoding;

		// grab onto buffer if it's our buffer, user is responsible for deallocating contens himself
		if (own || buffer != contents) _buffer = buffer;

		return res;
	}

	PUGI__FN ParseResult Document::LoadBuffer(const void* contents, size_t size, unsigned int options, Encoding DocumentEncoding)
	{
		return LoadBufferImpl(const_cast<void*>(contents), size, options, DocumentEncoding, false, false);
	}

	PUGI__FN ParseResult Document::LoadBufferInplace(void* contents, size_t size, unsigned int options, Encoding DocumentEncoding)
	{
		return LoadBufferImpl(contents, size, options, DocumentEncoding, true, false);
	}

	PUGI__FN ParseResult Document::LoadBufferInplaceOwn(void* contents, size_t size, unsigned int options, Encoding DocumentEncoding)
	{
		return LoadBufferImpl(contents, size, options, DocumentEncoding, true, true);
	}

    PUGI__FN void Document::Save(Writer& WriterInstance, const Char8* indent, unsigned int flags, Encoding DocumentEncoding) const
	{
		internal::BufferedWriter buffered_WriterInstance(WriterInstance, DocumentEncoding);

        if ((flags & FormatWriteBom) && DocumentEncoding != EncodingLatin1)
		{
            // BOM always represents the codepoint U+FEFF, so just Write it in native DocumentEncoding
            buffered_WriterInstance.Write('\xef', '\xbb', '\xbf');
		}

		if (!(flags & FormatNoDeclaration) && !internal::hAsDeclaration(*this))
		{
            buffered_WriterInstance.Write("<?xml version=\"1.0\"");
            if (DocumentEncoding == EncodingLatin1) buffered_WriterInstance.Write(" DocumentEncoding=\"ISO-8859-1\"");
			buffered_WriterInstance.Write('?', '>');
			if (!(flags & FormatRaw)) buffered_WriterInstance.Write('\n');
		}

		internal::NodeOutput(buffered_WriterInstance, *this, indent, flags, 0);
	}

    PUGI__FN void Document::Save(std::basic_ostream<char, std::char_traits<char> >& stream, const Char8* indent, unsigned int flags, Encoding DocumentEncoding) const
	{
		WriterStream WriterInstance(stream);

		Save(WriterInstance, indent, flags, DocumentEncoding);
	}

    PUGI__FN void Document::Save(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& stream, const Char8* indent, unsigned int flags) const
	{
		WriterStream WriterInstance(stream);

		Save(WriterInstance, indent, flags, Encodingwchar_t);
	}

    PUGI__FN bool Document::SaveFile(const char* Path_, const Char8* indent, unsigned int flags, Encoding DocumentEncoding) const
	{
		FILE* file = fopen(Path_, (flags & FormatSaveFileText) ? "w" : "wb");
		return internal::SaveFileImpl(*this, file, indent, flags, DocumentEncoding);
	}

    PUGI__FN bool Document::SaveFile(const wchar_t* Path_, const Char8* indent, unsigned int flags, Encoding DocumentEncoding) const
	{
		FILE* file = internal::open_file_wide(Path_, (flags & FormatSaveFileText) ? L"w" : L"wb");
		return internal::SaveFileImpl(*this, file, indent, flags, DocumentEncoding);
	}

	PUGI__FN Node Document::DocumentElement() const
	{
		for (NodeStruct* i = NodeData->GetFirstChild; i; i = i->GetNextSibling)
			if ((i->header & internal::MemoryPage_type_mask) + 1 == NodeElement)
				return Node(i);

		return Node();
	}

	PUGI__FN std::string MEZZ_LIB AsUtf8(const wchar_t* str)
	{
		assert(str);

		return internal::AsUtf8_impl(str, wcslen(str));
	}

	PUGI__FN std::string MEZZ_LIB AsUtf8(const std::basic_string<wchar_t>& str)
	{
		return internal::AsUtf8_impl(str.c_str(), str.size());
	}

	PUGI__FN std::basic_string<wchar_t> MEZZ_LIB AsWide(const char* str)
	{
		assert(str);

		return internal::AsWide_impl(str, strlen(str));
	}

	PUGI__FN std::basic_string<wchar_t> MEZZ_LIB AsWide(const std::string& str)
	{
		return internal::AsWide_impl(str.c_str(), str.size());
	}


	PUGI__FN void MEZZ_LIB SetMemory_management_functions(AllocationFunction allocate, deAllocationFunction deallocate)
	{
		internal::Memory::allocate = allocate;
		internal::Memory::deallocate = deallocate;
	}

	PUGI__FN AllocationFunction MEZZ_LIB GetMemoryAllocationFunction()
	{
		return internal::Memory::allocate;
	}

	PUGI__FN deAllocationFunction MEZZ_LIB GetMemoryDeallocationFunction()
	{
		return internal::Memory::deallocate;
	}
}



// STL replacements
PUGI__NS_BEGIN
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
		I Result = begin;

		for (I it = begin + 1; it != end; ++it)
			if (pred(*it, *Result))
				Result = it;

		return Result;
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
PUGI__NS_END

// Allocator used for AST and evaluation stacks
PUGI__NS_BEGIN
	struct XPathMemoryBlock
	{
		XPathMemoryBlock* next;

		char data[
	#ifdef XML_MEMORY_XPATH_PAGE_SIZE
			XML_MEMORY_XPATH_PAGE_SIZE
	#else
			4096
	#endif
		];
	};

	class XPathAllocator
	{
		XPathMemoryBlock* _GetRoot;
		size_t _GetRoot_size;

	public:


		XPathAllocator(XPathMemoryBlock* GetRoot, size_t GetRoot_size = 0): _GetRoot(GetRoot), _GetRoot_size(GetRoot_size)
		{

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

				XPathMemoryBlock* block = static_cast<XPathMemoryBlock*>(Memory::allocate(block_size));
				if (!block) return 0;

				block->next = _GetRoot;

				_GetRoot = block;
				_GetRoot_size = size;

				return block->data;
			}
		}

		void* allocate(size_t size)
		{
			void* Result = allocate_nothrow(size);

			if (!Result)
			{
				throw std::bad_alloc();
			}

			return Result;
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
			void* Result = allocate(new_size);
			assert(Result);

			// we have a new block
			if (Result != ptr && ptr)
			{
				// copy old data
				assert(new_size > old_size);
				memcpy(Result, ptr, old_size);

				// free the previous page if it had no other objects
				if (only_object)
				{
					assert(_GetRoot->data == Result);
					assert(_GetRoot->next);

					XPathMemoryBlock* next = _GetRoot->next->next;

					if (next)
					{
						// deallocate the whole page, unless it was the first one
						Memory::deallocate(_GetRoot->next);
						_GetRoot->next = next;
					}
				}
			}

			return Result;
		}

		void revert(const XPathAllocator& state)
		{
			// free all new pages
			XPathMemoryBlock* cur = _GetRoot;

			while (cur != state._GetRoot)
			{
				XPathMemoryBlock* next = cur->next;

				Memory::deallocate(cur);

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

				Memory::deallocate(cur);

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
		XPathAllocator* Result;
		XPathAllocator* temp;
	};

	struct XPathStackData
	{
		XPathMemoryBlock blocks[2];
		XPathAllocator Result;
		XPathAllocator temp;
		XPathStack stack;

		XPathStackData(): Result(blocks + 0), temp(blocks + 1)
		{
			blocks[0].next = blocks[1].next = 0;

			stack.Result = &Result;
			stack.temp = &temp;

		}

		~XPathStackData()
		{
			Result.release();
			temp.release();
		}
	};
PUGI__NS_END

// String class
PUGI__NS_BEGIN
	class XPathString
	{
        const Char8* _buffer;
		bool _uses_heap;

        static Char8* duplicate_string(const Char8* string, size_t length, XPathAllocator* alloc)
		{
            Char8* Result = static_cast<Char8*>(alloc->allocate((length + 1) * sizeof(Char8)));
			assert(Result);

            memcpy(Result, string, length * sizeof(Char8));
			Result[length] = 0;

			return Result;
		}

        static Char8* duplicate_string(const Char8* string, XPathAllocator* alloc)
		{
			return duplicate_string(string, strlength(string), alloc);
		}

	public:
        XPathString(): _buffer(""), _uses_heap(false)
		{
		}

        explicit XPathString(const Char8* str, XPathAllocator* alloc)
		{
			bool empty_ = (*str == 0);

            _buffer = empty_ ? "" : duplicate_string(str, alloc);
			_uses_heap = !empty_;
		}

        explicit XPathString(const Char8* str, bool use_heap): _buffer(str), _uses_heap(use_heap)
		{
		}

        XPathString(const Char8* begin, const Char8* end, XPathAllocator* alloc)
		{
			assert(begin <= end);

			bool empty_ = (begin == end);

            _buffer = empty_ ? "" : duplicate_string(begin, static_cast<size_t>(end - begin), alloc);
			_uses_heap = !empty_;
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
				size_t Result_length = tarGetLength + source_length;

				// allocate new buffer
                Char8* Result = static_cast<Char8*>(alloc->reallocate(_uses_heap ? const_cast<Char8*>(_buffer) : 0, (tarGetLength + 1) * sizeof(Char8), (Result_length + 1) * sizeof(Char8)));
				assert(Result);

				// append first string to the new buffer in case there was no reallocation
                if (!_uses_heap) memcpy(Result, _buffer, tarGetLength * sizeof(Char8));

				// append second string to the new buffer
                memcpy(Result + tarGetLength, o._buffer, source_length * sizeof(Char8));
				Result[Result_length] = 0;

				// finalize
				_buffer = Result;
				_uses_heap = true;
			}
		}

        const Char8* c_str() const
		{
			return _buffer;
		}

		size_t length() const
		{
			return strlength(_buffer);
		}

        Char8* data(XPathAllocator* alloc)
		{
			// make private heap copy
			if (!_uses_heap)
			{
				_buffer = duplicate_string(_buffer, alloc);
				_uses_heap = true;
			}

            return const_cast<Char8*>(_buffer);
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

    PUGI__FN XPathString XPathStringConst(const Char8* str)
	{
		return XPathString(str, false);
	}
PUGI__NS_END

PUGI__NS_BEGIN
    PUGI__FN bool starts_with(const Char8* string, const Char8* pattern)
	{
		while (*pattern && *string == *pattern)
		{
			string++;
			pattern++;
		}

		return *pattern == 0;
	}

    PUGI__FN const Char8* FindChar(const Char8* s, Char8 c)
    {
        return strchr(s, c);
	}

    PUGI__FN const Char8* FindSubstring(const Char8* s, const Char8* p)
    {
        return strstr(s, p);
	}

	// Converts symbol to lower case, if it is an ASCII one
    PUGI__FN Char8 tolower_ascii(Char8 ch)
	{
        return static_cast<unsigned int>(ch - 'A') < 26 ? static_cast<Char8>(ch | ' ') : ch;
	}

	PUGI__FN XPathString string_Value(const XPathNode& na, XPathAllocator* alloc)
	{
		if (na.GetAttribute())
			return XPathStringConst(na.GetAttribute().Value());
		else
		{
			const Node& n = na.GetNode();

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
				XPathString Result;

				Node cur = n.GetFirstChild();

				while (cur && cur != n)
				{
					if (cur.Type() == NodePcdata || cur.Type() == NodeCdata)
						Result.append(XPathStringConst(cur.Value()), alloc);

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

				return Result;
			}

			default:
				return XPathString();
			}
		}
	}

	PUGI__FN unsigned int NodeHeight(Node n)
	{
		unsigned int Result = 0;

		while (n)
		{
			++Result;
			n = n.GetParent();
		}

		return Result;
	}

	PUGI__FN bool NodeIs_before(Node ln, unsigned int lh, Node rn, unsigned int rh)
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

	PUGI__FN bool NodeIs_ancestor(Node GetParent, Node node)
	{
		while (node && node != GetParent) node = node.GetParent();

		return GetParent && node == GetParent;
	}

	PUGI__FN const void* document_order(const XPathNode& xnode)
	{
		NodeStruct* node = xnode.GetNode().InternalObject();

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
			Node ln = lhs.GetNode(), rn = rhs.GetNode();

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
				if (lhs.GetParent() == rhs.GetNode()) return false;

				ln = lhs.GetParent();
			}
			else if (rhs.GetAttribute())
			{
				// attributes go after the GetParent element
				if (rhs.GetParent() == lhs.GetNode()) return true;

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
			else return rhs.GetAttribute() ? false : lhs.GetNode() < rhs.GetNode();
		}
	};

	PUGI__FN double gen_nan()
	{
	#if defined(__STDC_IEC_559__) || ((FLT_RADIX - 0 == 2) && (FLT_MAX_EXP - 0 == 128) && (FLT_MANT_DIG - 0 == 24))
		union { float f; uint32_t i; } u[sizeof(float) == sizeof(uint32_t) ? 1 : -1];
		u[0].i = 0x7fc00000;
		return u[0].f;
	#else
		// fallback
		const volatile double zero = 0.0;
		return zero / zero;
	#endif
	}

	PUGI__FN bool is_nan(double Value)
	{
	#if defined(PUGI__MSVC_CRT_VERSION) || defined(__BORLANDC__)
		return !!_isnan(Value);
	#elif defined(fpclassify) && defined(FP_NAN)
		return fpclassify(Value) == FP_NAN;
	#else
		// fallback
		const volatile double v = Value;
		return v != v;
	#endif
	}

    PUGI__FN const Char8* convert_number_to_string_special(double Value)
	{
	#if defined(PUGI__MSVC_CRT_VERSION) || defined(__BORLANDC__)
        if (_finite(Value)) return (Value == 0) ? "0" : 0;
        if (_isnan(Value)) return "NaN";
        return Value > 0 ? "Infinity" : "-Infinity";
	#elif defined(fpclassify) && defined(FP_NAN) && defined(FP_INFINITE) && defined(FP_ZERO)
		switch (fpclassify(Value))
		{
		case FP_NAN:
            return "NaN";

		case FP_INFINITE:
            return Value > 0 ? "Infinity" : "-Infinity";

		case FP_ZERO:
            return "0";

		default:
			return 0;
		}
	#else
		// fallback
		const volatile double v = Value;

        if (v == 0) return "0";
        if (v != v) return "NaN";
        if (v * 2 == v) return Value > 0 ? "Infinity" : "-Infinity";
		return 0;
	#endif
	}

	PUGI__FN bool convert_number_to_boolean(double Value)
	{
		return (Value != 0 && !is_nan(Value));
	}

	PUGI__FN void truncate_zeros(char* begin, char* end)
	{
		while (begin != end && end[-1] == '0') end--;

		*end = 0;
	}

	// gets mantissa digits in the form of 0.xxxxx with 0. implied and the exponent
#if defined(PUGI__MSVC_CRT_VERSION) && PUGI__MSVC_CRT_VERSION >= 1400 && !defined(_WIN32_WCE)
	PUGI__FN void convert_number_to_mantissa_exponent(double Value, char* buffer, size_t buffer_size, char** out_mantissa, int* out_exponent)
	{
		// get base Values
		int sign, exponent;
		_ecvt_s(buffer, buffer_size, Value, DBL_DIG + 1, &exponent, &sign);

		// truncate redundant zeros
		truncate_zeros(buffer, buffer + strlen(buffer));

		// fill Results
		*out_mantissa = buffer;
		*out_exponent = exponent;
	}
#else
	PUGI__FN void convert_number_to_mantissa_exponent(double Value, char* buffer, size_t buffer_size, char** out_mantissa, int* out_exponent)
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

		// fill Results
		*out_mantissa = mantissa;
		*out_exponent = exponent;
	}
#endif

	PUGI__FN XPathString convert_number_to_string(double Value, XPathAllocator* alloc)
	{
		// try special number conversion
        const Char8* special = convert_number_to_string_special(Value);
		if (special) return XPathStringConst(special);

		// get mantissa + exponent form
		char mantissa_buffer[64];

		char* mantissa;
		int exponent;
		convert_number_to_mantissa_exponent(Value, mantissa_buffer, sizeof(mantissa_buffer), &mantissa, &exponent);

		// make the number!
        Char8 Result[512];
        Char8* s = Result;

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
				assert(*mantissa == 0 || static_cast<unsigned int>(*mantissa - '0') <= 9);
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
				assert(static_cast<unsigned int>(*mantissa - '0') <= 9);
				*s++ = *mantissa++;
			}
		}

		// zero-terminate
		assert(s < Result + sizeof(Result) / sizeof(Result[0]));
		*s = 0;

		return XPathString(Result, alloc);
	}

    PUGI__FN bool check_Stringo_number_format(const Char8* string)
	{
		// parse leading whitespace
		while (PUGI__IS_CHARTYPE(*string, ct_space)) ++string;

		// parse sign
		if (*string == '-') ++string;

		if (!*string) return false;

		// if there is no integer part, there should be a decimal part with at least one digit
		if (!PUGI__IS_CHARTYPEX(string[0], ctx_digit) && (string[0] != '.' || !PUGI__IS_CHARTYPEX(string[1], ctx_digit))) return false;

		// parse integer part
		while (PUGI__IS_CHARTYPEX(*string, ctx_digit)) ++string;

		// parse decimal part
		if (*string == '.')
		{
			++string;

			while (PUGI__IS_CHARTYPEX(*string, ctx_digit)) ++string;
		}

		// parse trailing whitespace
		while (PUGI__IS_CHARTYPE(*string, ct_space)) ++string;

		return *string == 0;
	}

    PUGI__FN double convert_Stringo_number(const Char8* string)
	{
		// check string format
		if (!check_Stringo_number_format(string)) return gen_nan();

        // parse string
        return atof(string);
	}

    PUGI__FN bool convert_Stringo_number(const Char8* begin, const Char8* end, double* out_Result)
	{
        Char8 buffer[32];

		size_t length = static_cast<size_t>(end - begin);
        Char8* scratch = buffer;

		if (length >= sizeof(buffer) / sizeof(buffer[0]))
		{
			// need to make dummy on-heap copy
            scratch = static_cast<Char8*>(Memory::allocate((length + 1) * sizeof(Char8)));
			if (!scratch) return false;
		}

		// copy string to zero-terminated buffer and perform conversion
        memcpy(scratch, begin, length * sizeof(Char8));
		scratch[length] = 0;

		*out_Result = convert_Stringo_number(scratch);

		// free dummy buffer
		if (scratch != buffer) Memory::deallocate(scratch);

		return true;
	}

	PUGI__FN double round_nearest(double Value)
	{
		return floor(Value + 0.5);
	}

	PUGI__FN double round_nearest_nzero(double Value)
	{
		// same as round_nearest, but returns -0 for [-0.5, -0]
		// ceil is used to differentiate between +0 and -0 (we return -0 for [-0.5, -0] and +0 for +0)
		return (Value >= -0.5 && Value <= 0) ? ceil(Value) : floor(Value + 0.5);
	}

    PUGI__FN const Char8* qualified_Name(const XPathNode& node)
	{
		return node.GetAttribute() ? node.GetAttribute().Name() : node.GetNode().Name();
	}

    PUGI__FN const Char8* local_Name(const XPathNode& node)
	{
        const Char8* Name = qualified_Name(node);
        const Char8* p = FindChar(Name, ':');

		return p ? p + 1 : Name;
	}

	struct namespace_uri_predicate
	{
        const Char8* prefix;
		size_t prefix_length;

        namespace_uri_predicate(const Char8* Name)
		{
            const Char8* pos = FindChar(Name, ':');

			prefix = pos ? Name : 0;
			prefix_length = pos ? static_cast<size_t>(pos - Name) : 0;
		}

		bool operator()(const Attribute& a) const
		{
            const Char8* Name = a.Name();

            if (!starts_with(Name, "xmlns")) return false;

			return prefix ? Name[5] == ':' && strequalrange(Name + 6, prefix, prefix_length) : Name[5] == 0;
		}
	};

    PUGI__FN const Char8* namespace_uri(const Node& node)
	{
		namespace_uri_predicate pred = node.Name();

		Node p = node;

		while (p)
		{
			Attribute a = p.FindAttribute(pred);

			if (a) return a.Value();

			p = p.GetParent();
		}

        return "";
	}

    PUGI__FN const Char8* namespace_uri(const Attribute& attr, const Node& GetParent)
	{
		namespace_uri_predicate pred = attr.Name();

		// Default namespace does not apply to attributes
        if (!pred.prefix) return "";

		Node p = GetParent;

		while (p)
		{
			Attribute a = p.FindAttribute(pred);

			if (a) return a.Value();

			p = p.GetParent();
		}

        return "";
	}

    PUGI__FN const Char8* namespace_uri(const XPathNode& node)
	{
		return node.GetAttribute() ? namespace_uri(node.GetAttribute(), node.GetParent()) : namespace_uri(node.GetNode());
	}

    PUGI__FN void normalize_space(Char8* buffer)
	{
        Char8* Write = buffer;

        for (Char8* it = buffer; *it; )
		{
            Char8 ch = *it++;

			if (PUGI__IS_CHARTYPE(ch, ct_space))
			{
				// replace whitespace sequence with single space
				while (PUGI__IS_CHARTYPE(*it, ct_space)) it++;

				// avoid leading spaces
				if (Write != buffer) *Write++ = ' ';
			}
			else *Write++ = ch;
		}

		// remove trailing space
		if (Write != buffer && PUGI__IS_CHARTYPE(Write[-1], ct_space)) Write--;

		// zero-terminate
		*Write = 0;
	}

    PUGI__FN void translate(Char8* buffer, const Char8* from, const Char8* to)
	{
		size_t to_length = strlength(to);

        Char8* Write = buffer;

		while (*buffer)
		{
            PUGI__DMC_VOLATILE Char8 ch = *buffer++;

            const Char8* pos = FindChar(from, ch);

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
        Char8 Name[1];
	};

	struct XPathVariableNumber: XPathVariable
	{
		XPathVariableNumber(): Value(0)
		{
		}

		double Value;
        Char8 Name[1];
	};

	struct XPathVariableString: XPathVariable
	{
		XPathVariableString(): Value(0)
		{
		}

		~XPathVariableString()
		{
			if (Value) Memory::deallocate(Value);
		}

        Char8* Value;
        Char8 Name[1];
	};

	struct XPathVariableNodeSet: XPathVariable
	{
		XPathNodeSet Value;
        Char8 Name[1];
	};

	static const XPathNodeSet dummy_NodeSet;

    PUGI__FN unsigned int hash_string(const Char8* str)
	{
		// Jenkins one-at-a-time hash (http://en.wikipedia.org/wiki/Jenkins_hash_function#one-at-a-time)
		unsigned int Result = 0;

		while (*str)
		{
			Result += static_cast<unsigned int>(*str++);
			Result += Result << 10;
			Result ^= Result >> 6;
		}

		Result += Result << 3;
		Result ^= Result >> 11;
		Result += Result << 15;

		return Result;
	}

    template <typename T> PUGI__FN T* new_XPathVariable(const Char8* Name)
	{
		size_t length = strlength(Name);
		if (length == 0) return 0; // empty variable names are invalid

		// $$ we can't use offsetof(T, Name) because T is non-POD, so we just allocate additional length characters
        void* memory = Memory::allocate(sizeof(T) + length * sizeof(Char8));
		if (!memory) return 0;

		T* Result = new (memory) T();

        memcpy(Result->Name, Name, (length + 1) * sizeof(Char8));

		return Result;
	}

    PUGI__FN XPathVariable* new_XPathVariable(XPathValueType Type, const Char8* Name)
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

	template <typename T> PUGI__FN void delete_XPathVariable(T* var)
	{
		var->~T();
		Memory::deallocate(var);
	}

	PUGI__FN void delete_XPathVariable(XPathValueType Type, XPathVariable* var)
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

    PUGI__FN XPathVariable* GetVariable(XPathVariableSet* set, const Char8* begin, const Char8* end)
	{
        Char8 buffer[32];

		size_t length = static_cast<size_t>(end - begin);
        Char8* scratch = buffer;

		if (length >= sizeof(buffer) / sizeof(buffer[0]))
		{
			// need to make dummy on-heap copy
            scratch = static_cast<Char8*>(Memory::allocate((length + 1) * sizeof(Char8)));
			if (!scratch) return 0;
		}

		// copy string to zero-terminated buffer and perform lookup
        memcpy(scratch, begin, length * sizeof(Char8));
		scratch[length] = 0;

		XPathVariable* Result = set->Get(scratch);

		// free dummy buffer
		if (scratch != buffer) Memory::deallocate(scratch);

		return Result;
	}
PUGI__NS_END

// Internal node set class
PUGI__NS_BEGIN
	PUGI__FN XPathNodeSet::CollectionType XPathSort(XPathNode* begin, XPathNode* end, XPathNodeSet::CollectionType Type, bool rev)
	{
		XPathNodeSet::CollectionType order = rev ? XPathNodeSet::TypeSortedReverse : XPathNodeSet::TypeSorted;

		if (Type == XPathNodeSet::TypeUnsorted)
		{
			sort(begin, end, document_order_comparator());

			Type = XPathNodeSet::TypeSorted;
		}

		if (Type != order) reverse(begin, end);

		return order;
	}

	PUGI__FN XPathNode XPathFirst(const XPathNode* begin, const XPathNode* end, XPathNodeSet::CollectionType Type)
	{
		if (begin == end) return XPathNode();

		switch (Type)
		{
		case XPathNodeSet::TypeSorted:
			return *begin;

		case XPathNodeSet::TypeSortedReverse:
			return *(end - 1);

		case XPathNodeSet::TypeUnsorted:
			return *min_element(begin, end, document_order_comparator());

		default:
			assert(!"Invalid node set Type");
			return XPathNode();
		}
	}

	class XPathNodeSet_raw
	{
		XPathNodeSet::CollectionType _type;

		XPathNode* _begin;
		XPathNode* _end;
		XPathNode* _eos;

	public:
		XPathNodeSet_raw(): _type(XPathNodeSet::TypeUnsorted), _begin(0), _end(0), _eos(0)
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

		void append(const XPathNode* begin_, const XPathNode* end_, XPathAllocator* alloc)
		{
			size_t size_ = static_cast<size_t>(_end - _begin);
			size_t capacity = static_cast<size_t>(_eos - _begin);
			size_t count = static_cast<size_t>(end_ - begin_);

			if (size_ + count > capacity)
			{
				// reallocate the old array or allocate a new one
				XPathNode* data = static_cast<XPathNode*>(alloc->reallocate(_begin, capacity * sizeof(XPathNode), (size_ + count) * sizeof(XPathNode)));
				assert(data);

				// finalize
				_begin = data;
				_end = data + size_;
				_eos = data + size_ + count;
			}

			memcpy(_end, begin_, count * sizeof(XPathNode));
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
			if (_type == XPathNodeSet::TypeUnsorted)
				sort(_begin, _end, duplicate_comparator());

			_end = unique(_begin, _end);
		}

		XPathNodeSet::CollectionType Type() const
		{
			return _type;
		}

		void SetType(XPathNodeSet::CollectionType Value)
		{
			_type = Value;
		}
	};
PUGI__NS_END

PUGI__NS_BEGIN
	struct XPathContext
	{
		XPathNode n;
		size_t position, size;

		XPathContext(const XPathNode& n_, size_t position_, size_t size_): n(n_), position(position_), size(size_)
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
        const Char8* begin;
        const Char8* end;

		XPathLexerString(): begin(0), end(0)
		{
		}

        bool operator==(const Char8* other) const
		{
			size_t length = static_cast<size_t>(end - begin);

			return strequalrange(other, begin, length);
		}
	};

	class XPathLexer
	{
        const Char8* _cur;
        const Char8* _cur_lexeme_pos;
		XPathLexerString _cur_lexeme_contents;

		lexeme_t _cur_lexeme;

	public:
        explicit XPathLexer(const Char8* query): _cur(query)
		{
			next();
		}

        const Char8* state() const
		{
			return _cur;
		}

		void next()
		{
            const Char8* cur = _cur;

			while (PUGI__IS_CHARTYPE(*cur, ct_space)) ++cur;

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

				if (PUGI__IS_CHARTYPEX(*cur, ctx_start_symbol))
				{
					_cur_lexeme_contents.begin = cur;

					while (PUGI__IS_CHARTYPEX(*cur, ctx_symbol)) cur++;

					if (cur[0] == ':' && PUGI__IS_CHARTYPEX(cur[1], ctx_symbol)) // qname
					{
						cur++; // :

						while (PUGI__IS_CHARTYPEX(*cur, ctx_symbol)) cur++;
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
				else if (PUGI__IS_CHARTYPEX(*(cur+1), ctx_digit))
				{
					_cur_lexeme_contents.begin = cur; // .

					++cur;

					while (PUGI__IS_CHARTYPEX(*cur, ctx_digit)) cur++;

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
                Char8 terminator = *cur;

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
				if (PUGI__IS_CHARTYPEX(*cur, ctx_digit))
				{
					_cur_lexeme_contents.begin = cur;

					while (PUGI__IS_CHARTYPEX(*cur, ctx_digit)) cur++;

					if (*cur == '.')
					{
						cur++;

						while (PUGI__IS_CHARTYPEX(*cur, ctx_digit)) cur++;
					}

					_cur_lexeme_contents.end = cur;

					_cur_lexeme = lex_number;
				}
				else if (PUGI__IS_CHARTYPEX(*cur, ctx_start_symbol))
				{
					_cur_lexeme_contents.begin = cur;

					while (PUGI__IS_CHARTYPEX(*cur, ctx_symbol)) cur++;

					if (cur[0] == ':')
					{
						if (cur[1] == '*') // namespace test ncName:*
						{
							cur += 2; // :*
						}
						else if (PUGI__IS_CHARTYPEX(cur[1], ctx_symbol)) // namespace test qname
						{
							cur++; // :

							while (PUGI__IS_CHARTYPEX(*cur, ctx_symbol)) cur++;
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

        const Char8* current_pos() const
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
		ast_op_not_equal,				// left != right
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
		// node type
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
            const Char8* string;
			// Value for ast_number_constant
			double number;
			// variable for ast_variable
			XPathVariable* variable;
			// node test for ast_step (node Name/namespace/node Type/pi target)
            const Char8* nodetest;
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
					XPathAllocatorCapture cr(stack.Result);

					XPathString ls = lhs->eval_string(c, stack);
					XPathString rs = rhs->eval_string(c, stack);

					return comp(ls, rs);
				}
			}
			else if (lt == XPathTypeNodeSet && rt == XPathTypeNodeSet)
			{
				XPathAllocatorCapture cr(stack.Result);

				XPathNodeSet_raw ls = lhs->eval_NodeSet(c, stack);
				XPathNodeSet_raw rs = rhs->eval_NodeSet(c, stack);

				for (const XPathNode* li = ls.begin(); li != ls.end(); ++li)
					for (const XPathNode* ri = rs.begin(); ri != rs.end(); ++ri)
					{
						XPathAllocatorCapture cri(stack.Result);

						if (comp(string_Value(*li, stack.Result), string_Value(*ri, stack.Result)))
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
					XPathAllocatorCapture cr(stack.Result);

					double l = lhs->eval_number(c, stack);
					XPathNodeSet_raw rs = rhs->eval_NodeSet(c, stack);

					for (const XPathNode* ri = rs.begin(); ri != rs.end(); ++ri)
					{
						XPathAllocatorCapture cri(stack.Result);

						if (comp(l, convert_Stringo_number(string_Value(*ri, stack.Result).c_str())))
							return true;
					}

					return false;
				}
				else if (lt == XPathTypeString)
				{
					XPathAllocatorCapture cr(stack.Result);

					XPathString l = lhs->eval_string(c, stack);
					XPathNodeSet_raw rs = rhs->eval_NodeSet(c, stack);

					for (const XPathNode* ri = rs.begin(); ri != rs.end(); ++ri)
					{
						XPathAllocatorCapture cri(stack.Result);

						if (comp(l, string_Value(*ri, stack.Result)))
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
				XPathAllocatorCapture cr(stack.Result);

				XPathNodeSet_raw ls = lhs->eval_NodeSet(c, stack);
				XPathNodeSet_raw rs = rhs->eval_NodeSet(c, stack);

				for (const XPathNode* li = ls.begin(); li != ls.end(); ++li)
				{
					XPathAllocatorCapture cri(stack.Result);

					double l = convert_Stringo_number(string_Value(*li, stack.Result).c_str());

					for (const XPathNode* ri = rs.begin(); ri != rs.end(); ++ri)
					{
						XPathAllocatorCapture crii(stack.Result);

						if (comp(l, convert_Stringo_number(string_Value(*ri, stack.Result).c_str())))
							return true;
					}
				}

				return false;
			}
			else if (lt != XPathTypeNodeSet && rt == XPathTypeNodeSet)
			{
				XPathAllocatorCapture cr(stack.Result);

				double l = lhs->eval_number(c, stack);
				XPathNodeSet_raw rs = rhs->eval_NodeSet(c, stack);

				for (const XPathNode* ri = rs.begin(); ri != rs.end(); ++ri)
				{
					XPathAllocatorCapture cri(stack.Result);

					if (comp(l, convert_Stringo_number(string_Value(*ri, stack.Result).c_str())))
						return true;
				}

				return false;
			}
			else if (lt == XPathTypeNodeSet && rt != XPathTypeNodeSet)
			{
				XPathAllocatorCapture cr(stack.Result);

				XPathNodeSet_raw ls = lhs->eval_NodeSet(c, stack);
				double r = rhs->eval_number(c, stack);

				for (const XPathNode* li = ls.begin(); li != ls.end(); ++li)
				{
					XPathAllocatorCapture cri(stack.Result);

					if (comp(convert_Stringo_number(string_Value(*li, stack.Result).c_str()), r))
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

            const Char8* Name = a.Name();

			// There are no GetAttribute nodes corresponding to attributes that declare namespaces
			// That is, "xmlns:..." or "xmlns"
            if (starts_with(Name, "xmlns") && (Name[5] == 0 || Name[5] == ':')) return;

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
			ns.SetType((axis == axis_ancestor || axis == axis_ancestor_or_self || axis == axis_preceding || axis == axis_preceding_sibling) ? XPathNodeSet::TypeSortedReverse : XPathNodeSet::TypeSorted);

			if (_left)
			{
				XPathNodeSet_raw s = _left->eval_NodeSet(c, stack);

				// self axis preserves the original order
				if (axis == axis_self) ns.SetType(s.Type());

				for (const XPathNode* it = s.begin(); it != s.end(); ++it)
				{
					size_t size = ns.size();

					// in general, all axes generate elements in a particular order, but there is no order guarantee if axis is applied to two nodes
					if (axis != axis_self && size != 0) ns.SetType(XPathNodeSet::TypeUnsorted);

					if (it->GetNode())
						step_fill(ns, it->GetNode(), stack.Result, v);
					else if (attributes)
						step_fill(ns, it->GetAttribute(), it->GetParent(), stack.Result, v);

					apply_predicates(ns, size, stack);
				}
			}
			else
			{
				if (c.n.GetNode())
					step_fill(ns, c.n.GetNode(), stack.Result, v);
				else if (attributes)
					step_fill(ns, c.n.GetAttribute(), c.n.GetParent(), stack.Result, v);

				apply_predicates(ns, 0, stack);
			}

			// GetChild, GetAttribute and self axes always generate unique set of nodes
			// for other axis, if the set stayed sorted, it stayed unique because the traversal algorithms do not visit the same node twice
			if (axis != axis_GetChild && axis != axis_attribute && axis != axis_self && ns.Type() == XPathNodeSet::TypeUnsorted)
				ns.RemoveDuplicates();

			return ns;
		}

	public:
        XPathAstNode(ast_type_t Type, XPathValueType retType_, const Char8* Value):
			_type(static_cast<char>(Type)), _retType(static_cast<char>(retType_)), _axis(0), _test(0), _left(0), _right(0), _next(0)
		{
			assert(Type == ast_string_constant);
			_data.string = Value;
		}

		XPathAstNode(ast_type_t Type, XPathValueType retType_, double Value):
			_type(static_cast<char>(Type)), _retType(static_cast<char>(retType_)), _axis(0), _test(0), _left(0), _right(0), _next(0)
		{
			assert(Type == ast_number_constant);
			_data.number = Value;
		}

		XPathAstNode(ast_type_t Type, XPathValueType retType_, XPathVariable* Value):
			_type(static_cast<char>(Type)), _retType(static_cast<char>(retType_)), _axis(0), _test(0), _left(0), _right(0), _next(0)
		{
			assert(Type == ast_variable);
			_data.variable = Value;
		}

		XPathAstNode(ast_type_t Type, XPathValueType retType_, XPathAstNode* left = 0, XPathAstNode* right = 0):
			_type(static_cast<char>(Type)), _retType(static_cast<char>(retType_)), _axis(0), _test(0), _left(left), _right(right), _next(0)
		{
		}

        XPathAstNode(ast_type_t Type, XPathAstNode* left, axis_t axis, nodetest_t test, const Char8* contents):
			_type(static_cast<char>(Type)), _retType(XPathTypeNodeSet), _axis(static_cast<char>(axis)), _test(static_cast<char>(test)), _left(left), _right(0), _next(0)
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
				XPathAllocatorCapture cr(stack.Result);

				XPathString lr = _left->eval_string(c, stack);
				XPathString rr = _right->eval_string(c, stack);

				return starts_with(lr.c_str(), rr.c_str());
			}

			case ast_func_contains:
			{
				XPathAllocatorCapture cr(stack.Result);

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

				XPathAllocatorCapture cr(stack.Result);

				XPathString lang = _left->eval_string(c, stack);

				for (Node n = c.n.GetNode(); n; n = n.GetParent())
				{
                    Attribute a = n.GetAttribute("xml:lang");

					if (a)
					{
                        const Char8* Value = a.Value();

						// strnicmp / strncasecmp is not portable
                        for (const Char8* lit = lang.c_str(); *lit; ++lit)
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
					XPathAllocatorCapture cr(stack.Result);

					return !eval_string(c, stack).Empty();
				}

				case XPathTypeNodeSet:
				{
					XPathAllocatorCapture cr(stack.Result);

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
				return static_cast<double>(c.size);

			case ast_func_position:
				return static_cast<double>(c.position);

			case ast_func_count:
			{
				XPathAllocatorCapture cr(stack.Result);

				return static_cast<double>(_left->eval_NodeSet(c, stack).size());
			}

			case ast_func_string_length_0:
			{
				XPathAllocatorCapture cr(stack.Result);

				return static_cast<double>(string_Value(c.n, stack.Result).length());
			}

			case ast_func_string_length_1:
			{
				XPathAllocatorCapture cr(stack.Result);

				return static_cast<double>(_left->eval_string(c, stack).length());
			}

			case ast_func_number_0:
			{
				XPathAllocatorCapture cr(stack.Result);

				return convert_Stringo_number(string_Value(c.n, stack.Result).c_str());
			}

			case ast_func_number_1:
				return _left->eval_number(c, stack);

			case ast_func_sum:
			{
				XPathAllocatorCapture cr(stack.Result);

				double r = 0;

				XPathNodeSet_raw ns = _left->eval_NodeSet(c, stack);

				for (const XPathNode* it = ns.begin(); it != ns.end(); ++it)
				{
					XPathAllocatorCapture cri(stack.Result);

					r += convert_Stringo_number(string_Value(*it, stack.Result).c_str());
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
					XPathAllocatorCapture cr(stack.Result);

					return convert_Stringo_number(eval_string(c, stack).c_str());
				}

				case XPathTypeNodeSet:
				{
					XPathAllocatorCapture cr(stack.Result);

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
			XPathStack swapped_stack = {stack.temp, stack.Result};

			buffer[0] = _left->eval_string(c, swapped_stack);

			size_t pos = 1;
			for (XPathAstNode* n = _right; n; n = n->_next, ++pos) buffer[pos] = n->eval_string(c, swapped_stack);
			assert(pos == count);

			// get total length
			size_t length = 0;
			for (size_t i = 0; i < count; ++i) length += buffer[i].length();

			// create final string
            Char8* Result = static_cast<Char8*>(stack.Result->allocate((length + 1) * sizeof(Char8)));
			assert(Result);

            Char8* ri = Result;

			for (size_t j = 0; j < count; ++j)
                for (const Char8* bi = buffer[j].c_str(); *bi; ++bi)
					*ri++ = *bi;

			*ri = 0;

			return XPathString(Result, true);
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
				XPathAllocatorCapture cr(stack.Result);

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
				XPathAllocatorCapture cr(stack.Result);

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
				XPathAllocatorCapture cr(stack.Result);

				XPathNodeSet_raw ns = _left->eval_NodeSet(c, stack);
				XPathNode na = ns.first();

				return XPathStringConst(namespace_uri(na));
			}

			case ast_func_string_0:
				return string_Value(c.n, stack.Result);

			case ast_func_string_1:
				return _left->eval_string(c, stack);

			case ast_func_concat:
				return eval_string_concat(c, stack);

			case ast_func_substring_before:
			{
				XPathAllocatorCapture cr(stack.temp);

				XPathStack swapped_stack = {stack.temp, stack.Result};

				XPathString s = _left->eval_string(c, swapped_stack);
				XPathString p = _right->eval_string(c, swapped_stack);

                const Char8* pos = FindSubstring(s.c_str(), p.c_str());

				return pos ? XPathString(s.c_str(), pos, stack.Result) : XPathString();
			}

			case ast_func_substring_after:
			{
				XPathAllocatorCapture cr(stack.temp);

				XPathStack swapped_stack = {stack.temp, stack.Result};

				XPathString s = _left->eval_string(c, swapped_stack);
				XPathString p = _right->eval_string(c, swapped_stack);

                const Char8* pos = FindSubstring(s.c_str(), p.c_str());
				if (!pos) return XPathString();

                const Char8* Result = pos + p.length();

				return s.uses_heap() ? XPathString(Result, stack.Result) : XPathStringConst(Result);
			}

			case ast_func_substring_2:
			{
				XPathAllocatorCapture cr(stack.temp);

				XPathStack swapped_stack = {stack.temp, stack.Result};

				XPathString s = _left->eval_string(c, swapped_stack);
				size_t s_length = s.length();

				double first = round_nearest(_right->eval_number(c, stack));

				if (is_nan(first)) return XPathString(); // NaN
				else if (first >= s_length + 1) return XPathString();

				size_t pos = first < 1 ? 1 : static_cast<size_t>(first);
				assert(1 <= pos && pos <= s_length + 1);

                const Char8* rbegin = s.c_str() + (pos - 1);

				return s.uses_heap() ? XPathString(rbegin, stack.Result) : XPathStringConst(rbegin);
			}

			case ast_func_substring_3:
			{
				XPathAllocatorCapture cr(stack.temp);

				XPathStack swapped_stack = {stack.temp, stack.Result};

				XPathString s = _left->eval_string(c, swapped_stack);
				size_t s_length = s.length();

				double first = round_nearest(_right->eval_number(c, stack));
				double last = first + round_nearest(_right->_next->eval_number(c, stack));

				if (is_nan(first) || is_nan(last)) return XPathString();
				else if (first >= s_length + 1) return XPathString();
				else if (first >= last) return XPathString();
				else if (last < 1) return XPathString();

				size_t pos = first < 1 ? 1 : static_cast<size_t>(first);
				size_t end = last >= s_length + 1 ? s_length + 1 : static_cast<size_t>(last);

				assert(1 <= pos && pos <= end && end <= s_length + 1);
                const Char8* rbegin = s.c_str() + (pos - 1);
                const Char8* rend = s.c_str() + (end - 1);

				return (end == s_length + 1 && !s.uses_heap()) ? XPathStringConst(rbegin) : XPathString(rbegin, rend, stack.Result);
			}

			case ast_func_normalize_space_0:
			{
				XPathString s = string_Value(c.n, stack.Result);

				normalize_space(s.data(stack.Result));

				return s;
			}

			case ast_func_normalize_space_1:
			{
				XPathString s = _left->eval_string(c, stack);

				normalize_space(s.data(stack.Result));

				return s;
			}

			case ast_func_translate:
			{
				XPathAllocatorCapture cr(stack.temp);

				XPathStack swapped_stack = {stack.temp, stack.Result};

				XPathString s = _left->eval_string(c, stack);
				XPathString from = _right->eval_string(c, swapped_stack);
				XPathString to = _right->_next->eval_string(c, swapped_stack);

				translate(s.data(stack.Result), from.c_str(), to.c_str());

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
                    return XPathStringConst(eval_boolean(c, stack) ? "true" : "false");

				case XPathTypeNumber:
					return convert_number_to_string(eval_number(c, stack), stack.Result);

				case XPathTypeNodeSet:
				{
					XPathAllocatorCapture cr(stack.temp);

					XPathStack swapped_stack = {stack.temp, stack.Result};

					XPathNodeSet_raw ns = eval_NodeSet(c, swapped_stack);
					return ns.Empty() ? XPathString() : string_Value(ns.first(), stack.Result);
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

				XPathStack swapped_stack = {stack.temp, stack.Result};

				XPathNodeSet_raw ls = _left->eval_NodeSet(c, swapped_stack);
				XPathNodeSet_raw rs = _right->eval_NodeSet(c, stack);

				// we can optimize merging two sorted sets, but this is a very rare operation, so don't bother
				rs.SetType(XPathNodeSet::TypeUnsorted);

				rs.append(ls.begin(), ls.end(), stack.Result);
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

				default:
					assert(!"Unknown axis");
					return XPathNodeSet_raw();
				}
			}

			case ast_step_GetRoot:
			{
				assert(!_right); // GetRoot step can't have any predicates

				XPathNodeSet_raw ns;

				ns.SetType(XPathNodeSet::TypeSorted);

				if (c.n.GetNode()) ns.push_back(c.n.GetNode().GetRoot(), stack.Result);
				else if (c.n.GetAttribute()) ns.push_back(c.n.GetParent().GetRoot(), stack.Result);

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
					ns.append(s.begin(), s.end(), stack.Result);

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

        const Char8* _query;
		XPathVariableSet* _variables;

		XPathParseResult* _Result;

		void throw_error(const char* message)
		{
			_Result->error = message;
			_Result->Offset = _lexer.current_pos() - _query;


		}

		void throw_error_oom()
		{
			throw std::bad_alloc();
		}

		void* alloc_node()
		{
			void* Result = _alloc->allocate_nothrow(sizeof(XPathAstNode));

			if (!Result) throw_error_oom();

			return Result;
		}

        const Char8* alloc_string(const XPathLexerString& Value)
		{
			if (Value.begin)
			{
				size_t length = static_cast<size_t>(Value.end - Value.begin);

                Char8* c = static_cast<Char8*>(_alloc->allocate_nothrow((length + 1) * sizeof(Char8)));
				if (!c) throw_error_oom();

                memcpy(c, Value.begin, length * sizeof(Char8));
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
                if (Name == "boolean" && argc == 1)
					return new (alloc_node()) XPathAstNode(ast_func_boolean, XPathTypeBoolean, args[0]);

				break;

			case 'c':
                if (Name == "count" && argc == 1)
				{
					if (args[0]->retType() != XPathTypeNodeSet) throw_error("Function has to be applied to node set");
					return new (alloc_node()) XPathAstNode(ast_func_count, XPathTypeNumber, args[0]);
				}
                else if (Name == "contains" && argc == 2)
					return new (alloc_node()) XPathAstNode(ast_func_contains, XPathTypeString, args[0], args[1]);
                else if (Name == "concat" && argc >= 2)
					return new (alloc_node()) XPathAstNode(ast_func_concat, XPathTypeString, args[0], args[1]);
                else if (Name == "ceiling" && argc == 1)
					return new (alloc_node()) XPathAstNode(ast_func_ceiling, XPathTypeNumber, args[0]);

				break;

			case 'f':
                if (Name == "false" && argc == 0)
					return new (alloc_node()) XPathAstNode(ast_func_false, XPathTypeBoolean);
                else if (Name == "floor" && argc == 1)
					return new (alloc_node()) XPathAstNode(ast_func_floor, XPathTypeNumber, args[0]);

				break;

			case 'i':
                if (Name == "id" && argc == 1)
					return new (alloc_node()) XPathAstNode(ast_func_id, XPathTypeNodeSet, args[0]);

				break;

			case 'l':
                if (Name == "last" && argc == 0)
					return new (alloc_node()) XPathAstNode(ast_func_last, XPathTypeNumber);
                else if (Name == "lang" && argc == 1)
					return new (alloc_node()) XPathAstNode(ast_func_lang, XPathTypeBoolean, args[0]);
                else if (Name == "local-Name" && argc <= 1)
					return ParseFunctionHelper(ast_func_local_Name_0, ast_func_local_Name_1, argc, args);

				break;

			case 'n':
                if (Name == "Name" && argc <= 1)
					return ParseFunctionHelper(ast_func_Name_0, ast_func_Name_1, argc, args);
                else if (Name == "namespace-uri" && argc <= 1)
					return ParseFunctionHelper(ast_func_namespace_uri_0, ast_func_namespace_uri_1, argc, args);
                else if (Name == "normalize-space" && argc <= 1)
					return new (alloc_node()) XPathAstNode(argc == 0 ? ast_func_normalize_space_0 : ast_func_normalize_space_1, XPathTypeString, args[0], args[1]);
                else if (Name == "not" && argc == 1)
					return new (alloc_node()) XPathAstNode(ast_func_not, XPathTypeBoolean, args[0]);
                else if (Name == "number" && argc <= 1)
					return new (alloc_node()) XPathAstNode(argc == 0 ? ast_func_number_0 : ast_func_number_1, XPathTypeNumber, args[0]);

				break;

			case 'p':
                if (Name == "position" && argc == 0)
					return new (alloc_node()) XPathAstNode(ast_func_position, XPathTypeNumber);

				break;

			case 'r':
                if (Name == "round" && argc == 1)
					return new (alloc_node()) XPathAstNode(ast_func_round, XPathTypeNumber, args[0]);

				break;

			case 's':
                if (Name == "string" && argc <= 1)
					return new (alloc_node()) XPathAstNode(argc == 0 ? ast_func_string_0 : ast_func_string_1, XPathTypeString, args[0]);
                else if (Name == "string-length" && argc <= 1)
					return new (alloc_node()) XPathAstNode(argc == 0 ? ast_func_string_length_0 : ast_func_string_length_1, XPathTypeString, args[0]);
                else if (Name == "starts-with" && argc == 2)
					return new (alloc_node()) XPathAstNode(ast_func_starts_with, XPathTypeBoolean, args[0], args[1]);
                else if (Name == "substring-before" && argc == 2)
					return new (alloc_node()) XPathAstNode(ast_func_substring_before, XPathTypeString, args[0], args[1]);
                else if (Name == "substring-after" && argc == 2)
					return new (alloc_node()) XPathAstNode(ast_func_substring_after, XPathTypeString, args[0], args[1]);
                else if (Name == "substring" && (argc == 2 || argc == 3))
					return new (alloc_node()) XPathAstNode(argc == 2 ? ast_func_substring_2 : ast_func_substring_3, XPathTypeString, args[0], args[1]);
                else if (Name == "sum" && argc == 1)
				{
					if (args[0]->retType() != XPathTypeNodeSet) throw_error("Function has to be applied to node set");
					return new (alloc_node()) XPathAstNode(ast_func_sum, XPathTypeNumber, args[0]);
				}

				break;

			case 't':
                if (Name == "translate" && argc == 3)
					return new (alloc_node()) XPathAstNode(ast_func_translate, XPathTypeString, args[0], args[1]);
                else if (Name == "true" && argc == 0)
					return new (alloc_node()) XPathAstNode(ast_func_true, XPathTypeBoolean);

				break;

			default:
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
                if (Name == "ancestor")
					return axis_ancestor;
                else if (Name == "ancestor-or-self")
					return axis_ancestor_or_self;
                else if (Name == "GetAttribute")
					return axis_attribute;

				break;

			case 'c':
                if (Name == "GetChild")
					return axis_GetChild;

				break;

			case 'd':
                if (Name == "descendant")
					return axis_descendant;
                else if (Name == "descendant-or-self")
					return axis_descendant_or_self;

				break;

			case 'f':
                if (Name == "following")
					return axis_following;
                else if (Name == "following-sibling")
					return axis_following_sibling;

				break;

			case 'n':
                if (Name == "namespace")
					return axis_namespace;

				break;

			case 'p':
                if (Name == "GetParent")
					return axis_GetParent;
                else if (Name == "preceding")
					return axis_preceding;
                else if (Name == "preceding-sibling")
					return axis_preceding_sibling;

				break;

			case 's':
                if (Name == "self")
					return axis_self;

				break;

			default:
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
                if (Name == "comment")
					return nodetest_type_comment;

				break;

			case 'n':
                if (Name == "node")
					return nodetest_type_node;

				break;

			case 'p':
                if (Name == "processing-instruction")
					return nodetest_type_pi;

				break;

			case 't':
                if (Name == "text")
					return nodetest_type_text;

				break;

			default:
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
                const Char8* Value = alloc_string(_lexer.contents());

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
					// parse axis name
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
                        else if (nt_Name == "processing-instruction")
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
                    const Char8* state = _lexer.state();

					while (PUGI__IS_CHARTYPE(*state, ct_space)) ++state;

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
                   (_lexer.contents() == "mod" || _lexer.contents() == "div")))
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

            while (_lexer.current() == lex_string && _lexer.contents() == "and")
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

            while (_lexer.current() == lex_string && _lexer.contents() == "or")
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

        XPathParser(const Char8* query, XPathVariableSet* variables, XPathAllocator* alloc, XPathParseResult* Result): _alloc(alloc), _lexer(query), _query(query), _variables(variables), _Result(Result)
		{
		}

		XPathAstNode* parse()
		{
			XPathAstNode* Result = ParseExpression();

			if (_lexer.current() != lex_eof)
			{
				// there are still unparsed tokens left, error
				throw_error("Incorrect query");
			}

			return Result;
		}

        static XPathAstNode* parse(const Char8* query, XPathVariableSet* variables, XPathAllocator* alloc, XPathParseResult* Result)
		{
			XPathParser parser(query, variables, alloc, Result);

            return parser.parse();
		}
	};

	struct XPathQueryImpl
	{
		static XPathQueryImpl* create()
		{
			void* memory = Memory::allocate(sizeof(XPathQueryImpl));

			return new (memory) XPathQueryImpl();
		}

		static void destroy(void* ptr)
		{
			if (!ptr) return;

			// free all allocated pages
			static_cast<XPathQueryImpl*>(ptr)->alloc.release();

			// free allocator memory (with the first page)
			Memory::deallocate(ptr);
		}

		XPathQueryImpl(): GetRoot(0), alloc(&block)
		{
			block.next = 0;
		}

		XPathAstNode* GetRoot;
		XPathAllocator alloc;
		XPathMemoryBlock block;
	};

	PUGI__FN XPathString EvaluateString_impl(XPathQueryImpl* impl, const XPathNode& n, XPathStackData& sd)
	{
		if (!impl) return XPathString();

		XPathContext c(n, 1, 1);

		return impl->GetRoot->eval_string(c, sd.stack);
	}
PUGI__NS_END

namespace XML
{
	PUGI__FN XPathException::XPathException(const XPathParseResult& Result_): Exception("","","","",0), _Result(Result_)
	{
		assert(_Result.error);
	}

	PUGI__FN const char* XPathException::what() const throw()
	{
		return _Result.error;
	}

	PUGI__FN const XPathParseResult& XPathException::Result() const
	{
		return _Result;
	}

	PUGI__FN XPathNode::XPathNode()
	{
	}

	PUGI__FN XPathNode::XPathNode(const Node& Node): _node(Node)
	{
	}

	PUGI__FN XPathNode::XPathNode(const Attribute& attribute_, const Node& GetParent_): _node(attribute_ ? GetParent_ : Node()), _attribute(attribute_)
	{
	}

	PUGI__FN Node XPathNode::GetNode() const
	{
		return _attribute ? Node() : _node;
	}

	PUGI__FN Attribute XPathNode::GetAttribute() const
	{
		return _attribute;
	}

	PUGI__FN Node XPathNode::GetParent() const
	{
		return _attribute ? _node : _node.GetParent();
	}

	PUGI__FN static void unspecified_bool_XPathNode(XPathNode***)
	{
	}

	PUGI__FN XPathNode::operator XPathNode::unspecified_bool_type() const
	{
		return (_node || _attribute) ? unspecified_bool_XPathNode : 0;
	}

	PUGI__FN bool XPathNode::operator!() const
	{
		return !(_node || _attribute);
	}

	PUGI__FN bool XPathNode::operator==(const XPathNode& n) const
	{
		return _node == n._node && _attribute == n._attribute;
	}

	PUGI__FN bool XPathNode::operator!=(const XPathNode& n) const
	{
		return _node != n._node || _attribute != n._attribute;
	}

#ifdef __BORLANDC__
	PUGI__FN bool operator&&(const XPathNode& lhs, bool rhs)
	{
		return (bool)lhs && rhs;
	}

	PUGI__FN bool operator||(const XPathNode& lhs, bool rhs)
	{
		return (bool)lhs || rhs;
	}
#endif

	PUGI__FN void XPathNodeSet::_assign(const_iterator begin_, const_iterator end_)
	{
		assert(begin_ <= end_);

		size_t size_ = static_cast<size_t>(end_ - begin_);

		if (size_ <= 1)
		{
			// deallocate old buffer
			if (_begin != &_storage) internal::Memory::deallocate(_begin);

			// use internal buffer
			if (begin_ != end_) _storage = *begin_;

			_begin = &_storage;
			_end = &_storage + size_;
		}
		else
		{
			// make heap copy
			XPathNode* storage = static_cast<XPathNode*>(internal::Memory::allocate(size_ * sizeof(XPathNode)));

			if (!storage)
			{
				throw std::bad_alloc();
			}

			memcpy(storage, begin_, size_ * sizeof(XPathNode));

			// deallocate old buffer
			if (_begin != &_storage) internal::Memory::deallocate(_begin);

			// finalize
			_begin = storage;
			_end = storage + size_;
		}
	}

	PUGI__FN XPathNodeSet::XPathNodeSet(): _type(TypeUnsorted), _begin(&_storage), _end(&_storage)
	{
	}

	PUGI__FN XPathNodeSet::XPathNodeSet(const_iterator begin_, const_iterator end_, CollectionType Type_): _type(Type_), _begin(&_storage), _end(&_storage)
	{
		_assign(begin_, end_);
	}

	PUGI__FN XPathNodeSet::~XPathNodeSet()
	{
		if (_begin != &_storage) internal::Memory::deallocate(_begin);
	}

	PUGI__FN XPathNodeSet::XPathNodeSet(const XPathNodeSet& ns): _type(ns._type), _begin(&_storage), _end(&_storage)
	{
		_assign(ns._begin, ns._end);
	}

	PUGI__FN XPathNodeSet& XPathNodeSet::operator=(const XPathNodeSet& ns)
	{
		if (this == &ns) return *this;

		_type = ns._type;
		_assign(ns._begin, ns._end);

		return *this;
	}

	PUGI__FN XPathNodeSet::CollectionType XPathNodeSet::Type() const
	{
		return _type;
	}

	PUGI__FN size_t XPathNodeSet::size() const
	{
		return _end - _begin;
	}

	PUGI__FN bool XPathNodeSet::Empty() const
	{
		return _begin == _end;
	}

	PUGI__FN const XPathNode& XPathNodeSet::operator[](size_t index) const
	{
		assert(index < size());
		return _begin[index];
	}

	PUGI__FN XPathNodeSet::const_iterator XPathNodeSet::begin() const
	{
		return _begin;
	}

	PUGI__FN XPathNodeSet::const_iterator XPathNodeSet::end() const
	{
		return _end;
	}

	PUGI__FN void XPathNodeSet::sort(bool reverse)
	{
		_type = internal::XPathSort(_begin, _end, _type, reverse);
	}

	PUGI__FN XPathNode XPathNodeSet::first() const
	{
		return internal::XPathFirst(_begin, _end, _type);
	}

	PUGI__FN XPathParseResult::XPathParseResult(): error("Internal error"), Offset(0)
	{
	}

	PUGI__FN XPathParseResult::operator bool() const
	{
		return error == 0;
	}

	PUGI__FN const char* XPathParseResult::Description() const
	{
		return error ? error : "No error";
	}

	PUGI__FN XPathVariable::XPathVariable()
	{
	}

    PUGI__FN const Char8* XPathVariable::Name() const
	{
		switch (_type)
		{
		case XPathTypeNodeSet:
			return static_cast<const internal::XPathVariableNodeSet*>(this)->Name;

		case XPathTypeNumber:
			return static_cast<const internal::XPathVariableNumber*>(this)->Name;

		case XPathTypeString:
			return static_cast<const internal::XPathVariableString*>(this)->Name;

		case XPathTypeBoolean:
			return static_cast<const internal::XPathVariableBoolean*>(this)->Name;

		default:
			assert(!"Invalid variable Type");
			return 0;
		}
	}

	PUGI__FN XPathValueType XPathVariable::Type() const
	{
		return _type;
	}

	PUGI__FN bool XPathVariable::GetBoolean() const
	{
		return (_type == XPathTypeBoolean) ? static_cast<const internal::XPathVariableBoolean*>(this)->Value : false;
	}

	PUGI__FN double XPathVariable::GetNumber() const
	{
		return (_type == XPathTypeNumber) ? static_cast<const internal::XPathVariableNumber*>(this)->Value : internal::gen_nan();
	}

    PUGI__FN const Char8* XPathVariable::GetString() const
	{
        const Char8* Value = (_type == XPathTypeString) ? static_cast<const internal::XPathVariableString*>(this)->Value : 0;
        return Value ? Value : "";
	}

	PUGI__FN const XPathNodeSet& XPathVariable::GetNodeSet() const
	{
		return (_type == XPathTypeNodeSet) ? static_cast<const internal::XPathVariableNodeSet*>(this)->Value : internal::dummy_NodeSet;
	}

	PUGI__FN bool XPathVariable::Set(bool Value)
	{
		if (_type != XPathTypeBoolean) return false;

		static_cast<internal::XPathVariableBoolean*>(this)->Value = Value;
		return true;
	}

	PUGI__FN bool XPathVariable::Set(double Value)
	{
		if (_type != XPathTypeNumber) return false;

		static_cast<internal::XPathVariableNumber*>(this)->Value = Value;
		return true;
	}

    PUGI__FN bool XPathVariable::Set(const Char8* Value)
	{
		if (_type != XPathTypeString) return false;

		internal::XPathVariableString* var = static_cast<internal::XPathVariableString*>(this);

		// duplicate string
        size_t size = (internal::strlength(Value) + 1) * sizeof(Char8);

        Char8* copy = static_cast<Char8*>(internal::Memory::allocate(size));
		if (!copy) return false;

		memcpy(copy, Value, size);

		// replace old string
		if (var->Value) internal::Memory::deallocate(var->Value);
		var->Value = copy;

		return true;
	}

	PUGI__FN bool XPathVariable::Set(const XPathNodeSet& Value)
	{
		if (_type != XPathTypeNodeSet) return false;

		static_cast<internal::XPathVariableNodeSet*>(this)->Value = Value;
		return true;
	}

	PUGI__FN XPathVariableSet::XPathVariableSet()
	{
		for (size_t i = 0; i < sizeof(_data) / sizeof(_data[0]); ++i) _data[i] = 0;
	}

	PUGI__FN XPathVariableSet::~XPathVariableSet()
	{
		for (size_t i = 0; i < sizeof(_data) / sizeof(_data[0]); ++i)
		{
			XPathVariable* var = _data[i];

			while (var)
			{
				XPathVariable* next = var->_next;

				internal::delete_XPathVariable(var->_type, var);

				var = next;
			}
		}
	}

    PUGI__FN XPathVariable* XPathVariableSet::find(const Char8* Name) const
	{
		const size_t hash_size = sizeof(_data) / sizeof(_data[0]);
		size_t hash = internal::hash_string(Name) % hash_size;

		// look for existing variable
		for (XPathVariable* var = _data[hash]; var; var = var->_next)
			if (internal::strequal(var->Name(), Name))
				return var;

		return 0;
	}

    PUGI__FN XPathVariable* XPathVariableSet::Add(const Char8* Name, XPathValueType Type)
	{
		const size_t hash_size = sizeof(_data) / sizeof(_data[0]);
		size_t hash = internal::hash_string(Name) % hash_size;

		// look for existing variable
		for (XPathVariable* var = _data[hash]; var; var = var->_next)
			if (internal::strequal(var->Name(), Name))
				return var->Type() == Type ? var : 0;

		// add new variable
		XPathVariable* Result = internal::new_XPathVariable(Type, Name);

		if (Result)
		{
			Result->_type = Type;
			Result->_next = _data[hash];

			_data[hash] = Result;
		}

		return Result;
	}

    PUGI__FN bool XPathVariableSet::Set(const Char8* Name, bool Value)
	{
		XPathVariable* var = Add(Name, XPathTypeBoolean);
		return var ? var->Set(Value) : false;
	}

    PUGI__FN bool XPathVariableSet::Set(const Char8* Name, double Value)
	{
		XPathVariable* var = Add(Name, XPathTypeNumber);
		return var ? var->Set(Value) : false;
	}

    PUGI__FN bool XPathVariableSet::Set(const Char8* Name, const Char8* Value)
	{
		XPathVariable* var = Add(Name, XPathTypeString);
		return var ? var->Set(Value) : false;
	}

    PUGI__FN bool XPathVariableSet::Set(const Char8* Name, const XPathNodeSet& Value)
	{
		XPathVariable* var = Add(Name, XPathTypeNodeSet);
		return var ? var->Set(Value) : false;
	}

    PUGI__FN XPathVariable* XPathVariableSet::Get(const Char8* Name)
	{
		return find(Name);
	}

    PUGI__FN const XPathVariable* XPathVariableSet::Get(const Char8* Name) const
	{
		return find(Name);
	}

    PUGI__FN XPathQuery::XPathQuery(const Char8* query, XPathVariableSet* variables): _impl(0)
	{
		internal::XPathQueryImpl* qimpl = internal::XPathQueryImpl::create();

		if (!qimpl)
		{
			throw std::bad_alloc();
		}
		else
		{
			internal::buffer_holder impl_holder(qimpl, internal::XPathQueryImpl::destroy);

			qimpl->GetRoot = internal::XPathParser::parse(query, variables, &qimpl->alloc, &_Result);

			if (qimpl->GetRoot)
			{
				_impl = static_cast<internal::XPathQueryImpl*>(impl_holder.release());
				_Result.error = 0;
			}
		}
	}

	PUGI__FN XPathQuery::~XPathQuery()
	{
		internal::XPathQueryImpl::destroy(_impl);
	}

	PUGI__FN XPathValueType XPathQuery::ReturnType() const
	{
		if (!_impl) return XPathTypeNone;

		return static_cast<internal::XPathQueryImpl*>(_impl)->GetRoot->retType();
	}

	PUGI__FN bool XPathQuery::EvaluateBoolean(const XPathNode& n) const
	{
		if (!_impl) return false;

		internal::XPathContext c(n, 1, 1);
		internal::XPathStackData sd;

		return static_cast<internal::XPathQueryImpl*>(_impl)->GetRoot->eval_boolean(c, sd.stack);
	}

	PUGI__FN double XPathQuery::EvaluateNumber(const XPathNode& n) const
	{
		if (!_impl) return internal::gen_nan();

		internal::XPathContext c(n, 1, 1);
		internal::XPathStackData sd;

		return static_cast<internal::XPathQueryImpl*>(_impl)->GetRoot->eval_number(c, sd.stack);
	}

	PUGI__FN String XPathQuery::EvaluateString(const XPathNode& n) const
	{
		internal::XPathStackData sd;

		return internal::EvaluateString_impl(static_cast<internal::XPathQueryImpl*>(_impl), n, sd).c_str();
	}

    PUGI__FN size_t XPathQuery::EvaluateString(Char8* buffer, size_t capacity, const XPathNode& n) const
	{
		internal::XPathStackData sd;

		internal::XPathString r = internal::EvaluateString_impl(static_cast<internal::XPathQueryImpl*>(_impl), n, sd);

		size_t full_size = r.length() + 1;

		if (capacity > 0)
		{
			size_t size = (full_size < capacity) ? full_size : capacity;
			assert(size > 0);

            memcpy(buffer, r.c_str(), (size - 1) * sizeof(Char8));
			buffer[size - 1] = 0;
		}

		return full_size;
	}

	PUGI__FN XPathNodeSet XPathQuery::EvaluateNodeSet(const XPathNode& n) const
	{
		if (!_impl) return XPathNodeSet();

		internal::XPathAstNode* GetRoot = static_cast<internal::XPathQueryImpl*>(_impl)->GetRoot;

		if (GetRoot->retType() != XPathTypeNodeSet)
		{
			XPathParseResult res;
			res.error = "Expression does not evaluate to node set";

			//throw XPathException(res);

		}

		internal::XPathContext c(n, 1, 1);
		internal::XPathStackData sd;


		internal::XPathNodeSet_raw r = GetRoot->eval_NodeSet(c, sd.stack);

		return XPathNodeSet(r.begin(), r.end(), r.Type());
	}

	PUGI__FN const XPathParseResult& XPathQuery::Result() const
	{
		return _Result;
	}

	PUGI__FN static void unspecified_bool_XPathQuery(XPathQuery***)
	{
	}

	PUGI__FN XPathQuery::operator XPathQuery::unspecified_bool_type() const
	{
		return _impl ? unspecified_bool_XPathQuery : 0;
	}

	PUGI__FN bool XPathQuery::operator!() const
	{
		return !_impl;
	}

    PUGI__FN XPathNode Node::FindSingleNode(const Char8* query, XPathVariableSet* variables) const
	{
		XPathQuery q(query, variables);
		return FindSingleNode(q);
	}

	PUGI__FN XPathNode Node::FindSingleNode(const XPathQuery& query) const
	{
		XPathNodeSet s = query.EvaluateNodeSet(*this);
		return s.Empty() ? XPathNode() : s.first();
	}

    PUGI__FN XPathNodeSet Node::FindNodes(const Char8* query, XPathVariableSet* variables) const
	{
		XPathQuery q(query, variables);
		return FindNodes(q);
	}

	PUGI__FN XPathNodeSet Node::FindNodes(const XPathQuery& query) const
	{
		return query.EvaluateNodeSet(*this);
	}
}

#ifdef __BORLANDC__
#	pragma option pop
#endif

// Intel C++ does not properly keep warning state for function templates,
// so popping warning state at the end of translation unit leads to warnings in the middle.
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
#	pragma warning(pop)
#endif

// Undefine all local macros (makes sure we're not leaking macros in header-only mode)
#undef PUGI__NO_INLINE
#undef PUGI__STATIC_ASSERT
#undef PUGI__DMC_VOLATILE
#undef PUGI__MSVC_CRT_VERSION
#undef PUGI__NS_BEGIN
#undef PUGI__NS_END
#undef PUGI__FN
#undef PUGI__FN_NO_INLINE
#undef PUGI__IS_CHARTYPE_IMPL
#undef PUGI__IS_CHARTYPE
#undef PUGI__IS_CHARTYPEX
#undef PUGI__SKIPWS
#undef PUGI__OPTSET
#undef PUGI__PUSHNODE
#undef PUGI__POPNODE
#undef PUGI__SCANFOR
#undef PUGI__SCANWHILE
#undef PUGI__ENDSEG
#undef PUGI__THROW_ERROR
#undef PUGI__CHECK_ERROR
} // Mezzanine namespace

#endif

/*
 * Copyright (c) 2006-2012 Arseny Kapoulkine
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

/// @endcond

