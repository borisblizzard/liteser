/// @file
/// @version 3.0
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
		HL_ENUM_DEFINE_VALUE(Type::Identifier, UInt8, 0x02);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Int16, 0x03);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, UInt16, 0x04);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Int32, 0x05);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, UInt32, 0x06);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Int64, 0x07);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, UInt64, 0x08);
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
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Harray, 0xA1);
		HL_ENUM_DEFINE_VALUE(Type::Identifier, Hmap, 0xC1);
	));

	Type::Type()
	{
		this->identifier = Identifier::None;
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
		this->identifier = Identifier::UInt8;
	}

	void Type::assign(VPtr<short>* arg)
	{
		this->identifier = Identifier::Int16;
	}

	void Type::assign(VPtr<unsigned short>* arg)
	{
		this->identifier = Identifier::UInt16;
	}

	void Type::assign(VPtr<int>* arg)
	{
		this->identifier = Identifier::Int32;
	}

	void Type::assign(VPtr<unsigned int>* arg)
	{
		this->identifier = Identifier::UInt32;
	}

	void Type::assign(VPtr<int64_t>* arg)
	{
		this->identifier = Identifier::Int64;
	}

	void Type::assign(VPtr<uint64_t>* arg)
	{
		this->identifier = Identifier::UInt64;
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

	void Type::assign(VPtr<Serializable>* arg)
	{
		this->identifier = Identifier::ValueObject;
	}

	void Type::assign(VPtr<Serializable*>* arg)
	{
		this->identifier = Identifier::Object;
	}

}
