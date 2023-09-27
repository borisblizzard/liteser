/// @file
/// @version 3.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Represents a generic value pointer.

#ifndef LITESER_PTR_H
#define LITESER_PTR_H

#include <stdint.h>

#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

namespace liteser
{
	class Ptr
	{
	public:
		inline Ptr() { }
		virtual inline ~Ptr() { }

	};

	template <typename T>
	class VPtr : public Ptr
	{
	public:
		T* value;

		inline VPtr(T* value) : Ptr() { this->value = value; }

	};

	template <typename T>
	class CPtr : public Ptr
	{
	public:
		harray<T>* data;

		inline CPtr(harray<T>* data) : Ptr() { this->data = data; }
		inline ~CPtr() { delete this->data; }

	};

}
#endif
