/// @file
/// @version 3.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <stdint.h>

#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>
#include <hltypes/hversion.h>

#include "liteser.h"
#include "Type.h"

namespace liteser
{
	HL_ENUM_CLASS_DEFINE(Type::Identifier,
	(
		HL_ENUM_DEFINE_VALUE(Type::Identifier, None, 0x00);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Int8, 0x01);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Uint8, 0x02);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Int16, 0x03);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Uint16, 0x04);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Int32, 0x05);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Uint32, 0x06);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Int64, 0x07);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Uint64, 0x08);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Float, 0x21);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Double, 0x22);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Bool, 0x41);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, ValueObject, 0x61);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Object, 0x62);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Hstr, 0x81);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Hversion, 0x82);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Henum, 0x83);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Grectf, 0x91);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Gvec2f, 0x92);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Gvec3f, 0x93);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Grecti, 0x94);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Gvec2i, 0x95);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Gvec3i, 0x96);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Grectd, 0x97);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Gvec2d, 0x98);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Gvec3d, 0x99);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Harray, 0xA1);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Hmap, 0xC1);
	));

	Type::Type() :
		identifier(Identifier::None)
	{
	}

	Type::~Type()
	{
		foreach (Type*, it, this->subTypes)
		{
			delete (*it);
		}
	}

	void Type::assign(VPtr<char>* arg)
	{
		this->identifier = Identifier::Int8;
	}

	void Type::assign(VPtr<unsigned char>* arg)
	{
		this->identifier = Identifier::Uint8;
	}

	void Type::assign(VPtr<short>* arg)
	{
		this->identifier = Identifier::Int16;
	}

	void Type::assign(VPtr<unsigned short>* arg)
	{
		this->identifier = Identifier::Uint16;
	}

	void Type::assign(VPtr<int>* arg)
	{
		this->identifier = Identifier::Int32;
	}

	void Type::assign(VPtr<unsigned int>* arg)
	{
		this->identifier = Identifier::Uint32;
	}

	void Type::assign(VPtr<int64_t>* arg)
	{
		this->identifier = Identifier::Int64;
	}

	void Type::assign(VPtr<uint64_t>* arg)
	{
		this->identifier = Identifier::Uint64;
	}

	void Type::assign(VPtr<float>* arg)
	{
		this->identifier = Identifier::Float;
	}

	void Type::assign(VPtr<double>* arg)
	{
		this->identifier = Identifier::Double;
	}

	void Type::assign(VPtr<bool>* arg)
	{
		this->identifier = Identifier::Bool;
	}

	void Type::assign(VPtr<hstr>* arg)
	{
		this->identifier = Identifier::Hstr;
	}

	void Type::assign(VPtr<hversion>* arg)
	{
		this->identifier = Identifier::Hversion;
	}

	void Type::assign(VPtr<henum>* arg)
	{
		this->identifier = Identifier::Henum;
	}

	void Type::assign(VPtr<grectf>* arg)
	{
		this->identifier = Identifier::Grectf;
	}

	void Type::assign(VPtr<gvec2f>* arg)
	{
		this->identifier = Identifier::Gvec2f;
	}

	void Type::assign(VPtr<gvec3f>* arg)
	{
		this->identifier = Identifier::Gvec3f;
	}

	void Type::assign(VPtr<grecti>* arg)
	{
		this->identifier = Identifier::Grecti;
	}

	void Type::assign(VPtr<gvec2i>* arg)
	{
		this->identifier = Identifier::Gvec2i;
	}

	void Type::assign(VPtr<gvec3i>* arg)
	{
		this->identifier = Identifier::Gvec3i;
	}

	void Type::assign(VPtr<grectd>* arg)
	{
		this->identifier = Identifier::Grectd;
	}

	void Type::assign(VPtr<gvec2d>* arg)
	{
		this->identifier = Identifier::Gvec2d;
	}

	void Type::assign(VPtr<gvec3d>* arg)
	{
		this->identifier = Identifier::Gvec3d;
	}

	void Type::assign(VPtr<Serializable>* arg)
	{
		this->identifier = Identifier::ValueObject;
	}

	void Type::assign(VPtr<Serializable*>* arg)
	{
		this->identifier = Identifier::Object;
	}

}
