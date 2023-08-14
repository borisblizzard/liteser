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
/// Represents data about a variable's type.

#ifndef LITESER_TYPE_H
#define LITESER_TYPE_H

#include <stdint.h>

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/henum.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>
#include <hltypes/hversion.h>

#include "liteserExport.h"
#include "Ptr.h"

namespace liteser
{
	class Serializable;
	class Variable;

	class liteserExport Type
	{
	public:
		HL_ENUM_CLASS_PREFIX_DECLARE(liteserExport, Identifier,
		(
			HL_ENUM_DECLARE(Identifier, None);
			HL_ENUM_DECLARE(Identifier, Int8);
			HL_ENUM_DECLARE(Identifier, Uint8);
			HL_ENUM_DECLARE(Identifier, Int16);
			HL_ENUM_DECLARE(Identifier, Uint16);
			HL_ENUM_DECLARE(Identifier, Int32);
			HL_ENUM_DECLARE(Identifier, Uint32);
			HL_ENUM_DECLARE(Identifier, Int64);
			HL_ENUM_DECLARE(Identifier, Uint64);
			HL_ENUM_DECLARE(Identifier, Float);
			HL_ENUM_DECLARE(Identifier, Double);
			HL_ENUM_DECLARE(Identifier, Bool);
			HL_ENUM_DECLARE(Identifier, ValueObject);
			HL_ENUM_DECLARE(Identifier, Object);
			HL_ENUM_DECLARE(Identifier, Hstr);
			HL_ENUM_DECLARE(Identifier, Hversion);
			HL_ENUM_DECLARE(Identifier, Henum);
			HL_ENUM_DECLARE(Identifier, Grectf);
			HL_ENUM_DECLARE(Identifier, Gvec2f);
			HL_ENUM_DECLARE(Identifier, Gvec3f);
			HL_ENUM_DECLARE(Identifier, Grecti);
			HL_ENUM_DECLARE(Identifier, Gvec2i);
			HL_ENUM_DECLARE(Identifier, Gvec3i);
			HL_ENUM_DECLARE(Identifier, Grectd);
			HL_ENUM_DECLARE(Identifier, Gvec2d);
			HL_ENUM_DECLARE(Identifier, Gvec3d);
			HL_ENUM_DECLARE(Identifier, Harray);
			HL_ENUM_DECLARE(Identifier, Hmap);
		));

		Identifier identifier;
		harray<Type*> subTypes;

		Type();
		~Type();

		void assign(VPtr<char>* arg);
		void assign(VPtr<unsigned char>* arg);
		void assign(VPtr<short>* arg);
		void assign(VPtr<unsigned short>* arg);
		void assign(VPtr<int>* arg);
		void assign(VPtr<unsigned int>* arg);
		void assign(VPtr<int64_t>* arg);
		void assign(VPtr<uint64_t>* arg);
		void assign(VPtr<float>* arg);
		void assign(VPtr<double>* arg);
		void assign(VPtr<bool>* arg);
		void assign(VPtr<hstr>* arg);
		void assign(VPtr<hversion>* arg);
		void assign(VPtr<henum>* arg);
		void assign(VPtr<grectf>* arg);
		void assign(VPtr<gvec2f>* arg);
		void assign(VPtr<gvec3f>* arg);
		void assign(VPtr<grecti>* arg);
		void assign(VPtr<gvec2i>* arg);
		void assign(VPtr<gvec3i>* arg);
		void assign(VPtr<grectd>* arg);
		void assign(VPtr<gvec2d>* arg);
		void assign(VPtr<gvec3d>* arg);
		void assign(VPtr<Serializable>* arg);
		void assign(VPtr<Serializable*>* arg);
		template <typename T>
		inline void assign(VPtr<harray<T> >* arg)
		{
			this->identifier = Identifier::Harray;
			Type* type = new Type();
			type->assign((VPtr<T>*)NULL);
			this->subTypes += type;
		}
		template <typename K, typename V>
		inline void assign(VPtr<hmap<K, V> >* arg)
		{
			this->identifier = Identifier::Hmap;
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
