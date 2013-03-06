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
/// Represents data about a variable's type.

#ifndef LITESER_TYPE_H
#define LITESER_TYPE_H

#include <stdint.h>

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "liteserExport.h"
#include "Ptr.h"

namespace liteser
{
	class Serializable;
	class Variable;

	class liteserExport Type
	{
	public:
		enum Value
		{
			NONE    = 0x00,
			INT8	= 0x01,
			UINT8	= 0x02,
			INT16	= 0x03,
			UINT16	= 0x04,
			INT32	= 0x05,
			UINT32	= 0x06,
			FLOAT	= 0x21,
			DOUBLE	= 0x22,
			BOOL	= 0x41,
			OBJECT	= 0x61,
			OBJPTR	= 0x62,
			HSTR	= 0x81,
			GVEC2	= 0x91,
			GRECT	= 0x92,
			HARRAY	= 0xA1,
			HMAP	= 0xC1
		};

		Value value;
		harray<Type*> subTypes;

		Type(Ptr<char>* arg);
		Type(Ptr<unsigned char>* arg);
		Type(Ptr<int16_t>* arg);
		Type(Ptr<uint16_t>* arg);
		Type(Ptr<int32_t>* arg);
		Type(Ptr<uint32_t>* arg);
		Type(Ptr<float>* arg);
		Type(Ptr<double>* arg);
		Type(Ptr<bool>* arg);
		Type(Ptr<hstr>* arg);
		Type(Ptr<gvec2>* arg);
		Type(Ptr<grect>* arg);
		Type(Ptr<Serializable>* arg);
		Type(Ptr<Serializable*>* arg);
		
		template <class T>
		Type(Ptr<harray<T> >* arg)
		{
			this->value = HARRAY;
			this->subTypes += new Type((Ptr<T>*)NULL);
		}
		template <class K, class V>
		Type(Ptr<hmap<K, V> >* arg)
		{
			this->value = HMAP;
			this->subTypes += new Type((Ptr<K>*)NULL);
			this->subTypes += new Type((Ptr<V>*)NULL);
		}

		~Type();

	};

}
#endif
