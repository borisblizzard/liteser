/// @file
/// @author  Boris Mikic
/// @version 2.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
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
		Ptr() { }
		virtual ~Ptr() { }
	};

	template<class T>
	class VPtr : public Ptr
	{
	public:
		T* value;
		VPtr(T* value) { this->value = value; }
		~VPtr() { }

	};

	template<class T>
	class DPtr : public Ptr
	{
	public:
		harray<T>* data;
		DPtr(harray<T>* data) { this->data = data; }
		~DPtr() { delete this->data; }

	};

}
#endif
