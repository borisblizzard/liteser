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
/// Represents data about a variable definition and value.

#ifndef LITESER_VARIABLE_H
#define LITESER_VARIABLE_H

#include <stdint.h>

#include <hltypes/harray.h>
#include <hltypes/hdeque.h>
#include <hltypes/hlist.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "liteserExport.h"

namespace liteser
{
	class Serializable;

	class liteserExport Variable
	{
	public:
		enum Type
		{
			TYPE_INT8	= 0x01,
			TYPE_UINT8	= 0x02,
			TYPE_INT16	= 0x03,
			TYPE_UINT16	= 0x04,
			TYPE_INT32	= 0x05,
			TYPE_UINT32	= 0x06,
			TYPE_FLOAT	= 0x21,
			TYPE_DOUBLE	= 0x22,
			TYPE_BOOL	= 0x41,
			TYPE_OBJECT	= 0x61,
			TYPE_OBJPTR	= 0x62,
			TYPE_HSTR	= 0x81,
			TYPE_HARRAY	= 0xA1,
			TYPE_HLIST	= 0xA2,
			TYPE_HDEQUE	= 0xA3,
			TYPE_HMAP	= 0xC1
		};

		hstr name;
		void* value;
		Type type;

		Variable(chstr name, char* value); // char is always 8 bits
		Variable(chstr name, unsigned char* value); // char is always 8 bits
		Variable(chstr name, int16_t* value);
		Variable(chstr name, uint16_t* value);
		Variable(chstr name, int32_t* value);
		Variable(chstr name, uint32_t* value);
		Variable(chstr name, float* value);
		Variable(chstr name, double* value);
		Variable(chstr name, bool* value);
		Variable(chstr name, Serializable* value);
		Variable(chstr name, void* value);
		Variable(chstr name, hstr* value);
		//template <class T>
		//Variable(chstr name, harray<T> value);
		~Variable();

	};

}
#endif
