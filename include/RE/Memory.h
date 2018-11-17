#pragma once

#include "Hooks.h"  // _HeapAllocAbstraction()


namespace RE
{
#define TES_HEAP_REDEFINE_NEW()												\
	static void* operator new(std::size_t a_size)							\
	{ return Heap_Allocate(a_size); }										\
	static void* operator new(std::size_t a_size, const std::nothrow_t &)	\
	{ return Heap_Allocate(a_size); }										\
	static void* operator new(std::size_t a_size, void* a_ptr)				\
	{ return a_ptr; }														\
	static void operator delete(void* a_ptr)								\
	{ Heap_Free(a_ptr); }													\
	static void operator delete(void* a_ptr, const std::nothrow_t &)		\
	{ Heap_Free(a_ptr); }													\
	static void operator delete(void*, void *)								\
	{ }


#define TES_HEAP_REDEFINE_ABSTRACT_NEW()									\
	static void* operator new(std::size_t a_size)							\
	{ return Hooks::_HeapAllocAbstraction(a_size); }						\
	static void* operator new(std::size_t a_size, const std::nothrow_t&)	\
	{ return Hooks::_HeapAllocAbstraction(a_size); }						\
	static void* operator new(std::size_t a_size, void* a_ptr)				\
	{ return a_ptr; }														\
	static void operator delete(void* a_ptr)								\
	{ Heap_Free(a_ptr); }													\
	static void operator delete(void* a_ptr, const std::nothrow_t&)			\
	{ Heap_Free(a_ptr); }													\
	static void operator delete(void*, void*)								\
	{ }
}
