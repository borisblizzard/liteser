/// @file
/// @author  Boris Mikic
/// @version 2.2
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
			GRECT	= 0x91,
			GVEC2	= 0x92,
			GVEC3	= 0x93,
			HARRAY	= 0xA1,
			HMAP	= 0xC1
		};

		Value value;
		harray<Type*> subTypes;

		Type();
		~Type();

		void assign(VPtr<char>* arg);
		void assign(VPtr<unsigned char>* arg);
		void assign(VPtr<int16_t>* arg);
		void assign(VPtr<uint16_t>* arg);
		void assign(VPtr<int32_t>* arg);
		void assign(VPtr<uint32_t>* arg);
		void assign(VPtr<float>* arg);
		void assign(VPtr<double>* arg);
		void assign(VPtr<bool>* arg);
		void assign(VPtr<hstr>* arg);
		void assign(VPtr<grect>* arg);
		void assign(VPtr<gvec2>* arg);
		void assign(VPtr<gvec3>* arg);
		void assign(VPtr<Serializable>* arg);
		void assign(VPtr<Serializable*>* arg);
		template <class T>
		void assign(VPtr<harray<T> >* arg)
		{
			this->value = HARRAY;
			Type* type = new Type();
			type->assign((VPtr<T>*)NULL);
			this->subTypes += type;
		}
		template <class K, class V>
		void assign(VPtr<hmap<K, V> >* arg)
		{
			this->value = HMAP;
			Type* type = new Type();
			type->assign((VPtr<K>*)NULL);
			this->subTypes += type;
			type = new Type();
			type->assign((VPtr<V>*)NULL);
			this->subTypes += type;
		}

	};

}
#endif
