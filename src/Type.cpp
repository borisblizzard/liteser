/// @file
/// @version 2.7
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
	Type::Type()
	{
		this->value = NONE;
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
		this->value = INT8;
	}

	void Type::assign(VPtr<unsigned char>* arg)
	{
		this->value = UINT8;
	}

	void Type::assign(VPtr<short>* arg)
	{
		this->value = INT16;
	}

	void Type::assign(VPtr<unsigned short>* arg)
	{
		this->value = UINT16;
	}

	void Type::assign(VPtr<int>* arg)
	{
		this->value = INT32;
	}

	void Type::assign(VPtr<unsigned int>* arg)
	{
		this->value = UINT32;
	}

	void Type::assign(VPtr<int64_t>* arg)
	{
		this->value = INT64;
	}

	void Type::assign(VPtr<uint64_t>* arg)
	{
		this->value = UINT64;
	}

	void Type::assign(VPtr<float>* arg)
	{
		this->value = FLOAT;
	}

	void Type::assign(VPtr<double>* arg)
	{
		this->value = DOUBLE;
	}

	void Type::assign(VPtr<bool>* arg)
	{
		this->value = BOOL;
	}

	void Type::assign(VPtr<hstr>* arg)
	{
		this->value = HSTR;
	}

	void Type::assign(VPtr<hversion>* arg)
	{
		this->value = HVERSION;
	}

	void Type::assign(VPtr<henum>* arg)
	{
		this->value = HENUM;
	}

	void Type::assign(VPtr<grect>* arg)
	{
		this->value = GRECT;
	}

	void Type::assign(VPtr<gvec2>* arg)
	{
		this->value = GVEC2;
	}

	void Type::assign(VPtr<gvec3>* arg)
	{
		this->value = GVEC3;
	}

	void Type::assign(VPtr<Serializable>* arg)
	{
		this->value = OBJECT;
	}

	void Type::assign(VPtr<Serializable*>* arg)
	{
		this->value = OBJPTR;
	}

}
