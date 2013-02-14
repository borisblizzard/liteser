/// @file
/// @author  Boris Mikic
/// @version 2.0
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
	Type::Type(Ptr<char>* arg)
	{
		this->value = INT8;
	}

	Type::Type(Ptr<unsigned char>* arg)
	{
		this->value = UINT8;
	}

	Type::Type(Ptr<int16_t>* arg)
	{
		this->value = INT16;
	}

	Type::Type(Ptr<uint16_t>* arg)
	{
		this->value = UINT16;
	}

	Type::Type(Ptr<int32_t>* arg)
	{
		this->value = INT32;
	}

	Type::Type(Ptr<uint32_t>* arg)
	{
		this->value = UINT32;
	}

	Type::Type(Ptr<float>* arg)
	{
		this->value = FLOAT;
	}

	Type::Type(Ptr<double>* arg)
	{
		this->value = DOUBLE;
	}

	Type::Type(Ptr<bool>* arg)
	{
		this->value = BOOL;
	}

	Type::Type(Ptr<hstr>* arg)
	{
		this->value = HSTR;
	}

	Type::Type(Ptr<Serializable>* arg)
	{
		this->value = OBJECT;
	}

	Type::Type(Ptr<Serializable*>* arg)
	{
		this->value = OBJPTR;
	}

	Type::~Type()
	{
		foreach (Type*, it, this->subTypes)
		{
			delete (*it);
		}
	}

}
