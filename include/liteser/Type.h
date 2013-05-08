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

		void assign(Ptr<char>* arg);
		void assign(Ptr<unsigned char>* arg);
		void assign(Ptr<int16_t>* arg);
		void assign(Ptr<uint16_t>* arg);
		void assign(Ptr<int32_t>* arg);
		void assign(Ptr<uint32_t>* arg);
		void assign(Ptr<float>* arg);
		void assign(Ptr<double>* arg);
		void assign(Ptr<bool>* arg);
		void assign(Ptr<hstr>* arg);
		void assign(Ptr<grect>* arg);
		void assign(Ptr<gvec2>* arg);
		void assign(Ptr<gvec3>* arg);
		void assign(Ptr<Serializable>* arg);
		void assign(Ptr<Serializable*>* arg);
		template <class T>
		void assign(Ptr<harray<T> >* arg)
		{
			static Type* type;
			this->value = HARRAY;
			type = new Type();
			this->subTypes += type;
			type->assign((Ptr<T>*)NULL); // due to optimization with static, assign has to be called last as it might invalidate the pointer
		}
		template <class K, class V>
		void assign(Ptr<hmap<K, V> >* arg)
		{
			static Type* type;
			this->value = HMAP;
			type = new Type();
			this->subTypes += type;
			type->assign((Ptr<K>*)NULL); // due to optimization with static, assign has to be called last as it might invalidate the pointer
			type = new Type();
			this->subTypes += type;
			type->assign((Ptr<V>*)NULL); // due to optimization with static, assign has to be called last as it might invalidate the pointer
		}

	};

}
#endif
