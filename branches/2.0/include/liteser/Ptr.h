/// @file
/// @author  Boris Mikic
/// @version 2.0
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
	template<class T>
	struct Ptr
	{
	public:
		T* value;
		Ptr(T* value) { this->value = value; }
		~Ptr() { }

	};

}
#endif
