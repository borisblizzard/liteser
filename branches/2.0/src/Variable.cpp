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
	Variable::Variable(chstr name, Variable::Ptr<char>* ptr)
	{
		this->name = name;
		this->ptr = (void*)ptr;
		this->type = TYPE_INT8;
	}

	Variable::Variable(chstr name, Variable::Ptr<unsigned char>* ptr)
	{
		this->name = name;
		this->ptr = (void*)ptr;
		this->type = TYPE_UINT8;
	}

	Variable::Variable(chstr name, Variable::Ptr<int16_t>* ptr)
	{
		this->name = name;
		this->ptr = (void*)ptr;
		this->type = TYPE_INT16;
	}

	Variable::Variable(chstr name, Variable::Ptr<uint16_t>* ptr)
	{
		this->name = name;
		this->ptr = (void*)ptr;
		this->type = TYPE_UINT16;
	}

	Variable::Variable(chstr name, Variable::Ptr<int32_t>* ptr)
	{
		this->name = name;
		this->ptr = (void*)ptr;
		this->type = TYPE_INT32;
	}

	Variable::Variable(chstr name, Variable::Ptr<uint32_t>* ptr)
	{
		this->name = name;
		this->ptr = (void*)ptr;
		this->type = TYPE_UINT32;
	}

	Variable::Variable(chstr name, Variable::Ptr<float>* ptr)
	{
		this->name = name;
		this->ptr = (void*)ptr;
		this->type = TYPE_FLOAT;
	}

	Variable::Variable(chstr name, Variable::Ptr<double>* ptr)
	{
		this->name = name;
		this->ptr = (void*)ptr;
		this->type = TYPE_DOUBLE;
	}

	Variable::Variable(chstr name, Variable::Ptr<bool>* ptr)
	{
		this->name = name;
		this->ptr = (void*)ptr;
		this->type = TYPE_BOOL;
	}

	Variable::Variable(chstr name, Variable::Ptr<hstr>* ptr)
	{
		this->name = name;
		this->ptr = (void*)ptr;
		this->type = TYPE_HSTR;
	}
	
	Variable::~Variable()
	{
		delete this->ptr;
	}

}
