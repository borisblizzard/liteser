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
#include <hltypes/hdeque.h>
#include <hltypes/hlist.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "liteser.h"
#include "Variable.h"

namespace liteser
{
	Variable::Variable(chstr name, char* value)
	{
		this->name = name;
		this->value = (void*)value;
		this->type = TYPE_INT8;
	}

	Variable::Variable(chstr name, unsigned char* value)
	{
		this->name = name;
		this->value = (void*)value;
		this->type = TYPE_UINT8;
	}

	Variable::Variable(chstr name, int16_t* value)
	{
		this->name = name;
		this->value = (void*)value;
		this->type = TYPE_INT16;
	}

	Variable::Variable(chstr name, uint16_t* value)
	{
		this->name = name;
		this->value = (void*)value;
		this->type = TYPE_UINT16;
	}

	Variable::Variable(chstr name, int32_t* value)
	{
		this->name = name;
		this->value = (void*)value;
		this->type = TYPE_INT32;
	}

	Variable::Variable(chstr name, uint32_t* value)
	{
		this->name = name;
		this->value = (void*)value;
		this->type = TYPE_UINT32;
	}

	Variable::Variable(chstr name, float* value)
	{
		this->name = name;
		this->value = (void*)value;
		this->type = TYPE_FLOAT;
	}

	Variable::Variable(chstr name, double* value)
	{
		this->name = name;
		this->value = (void*)value;
		this->type = TYPE_DOUBLE;
	}

	Variable::Variable(chstr name, bool* value)
	{
		this->name = name;
		this->value = (void*)value;
		this->type = TYPE_BOOL;
	}

	Variable::Variable(chstr name, Serializable* value)
	{
		this->name = name;
		this->value = (void*)value;
		this->type = TYPE_OBJECT;
	}

	Variable::Variable(chstr name, void* value)
	{
		this->name = name;
		this->value = (void*)value; // this is the object pointer's address!
		this->type = TYPE_OBJPTR;
	}

	Variable::Variable(chstr name, hstr* value)
	{
		this->name = name;
		this->value = (void*)value;
		this->type = TYPE_HSTR;
	}

	Variable::~Variable()
	{
	}

}
