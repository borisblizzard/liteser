/// @file
/// @author  Boris Mikic
/// @version 2.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <stdint.h>

#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

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

	void Type::assign(Ptr<char>* arg)
	{
		this->value = INT8;
	}

	void Type::assign(Ptr<unsigned char>* arg)
	{
		this->value = UINT8;
	}

	void Type::assign(Ptr<int16_t>* arg)
	{
		this->value = INT16;
	}

	void Type::assign(Ptr<uint16_t>* arg)
	{
		this->value = UINT16;
	}

	void Type::assign(Ptr<int32_t>* arg)
	{
		this->value = INT32;
	}

	void Type::assign(Ptr<uint32_t>* arg)
	{
		this->value = UINT32;
	}

	void Type::assign(Ptr<float>* arg)
	{
		this->value = FLOAT;
	}

	void Type::assign(Ptr<double>* arg)
	{
		this->value = DOUBLE;
	}

	void Type::assign(Ptr<bool>* arg)
	{
		this->value = BOOL;
	}

	void Type::assign(Ptr<hstr>* arg)
	{
		this->value = HSTR;
	}

	void Type::assign(Ptr<grect>* arg)
	{
		this->value = GRECT;
	}

	void Type::assign(Ptr<gvec2>* arg)
	{
		this->value = GVEC2;
	}

	void Type::assign(Ptr<gvec3>* arg)
	{
		this->value = GVEC3;
	}

	void Type::assign(Ptr<Serializable>* arg)
	{
		this->value = OBJECT;
	}

	void Type::assign(Ptr<Serializable*>* arg)
	{
		this->value = OBJPTR;
	}

}
