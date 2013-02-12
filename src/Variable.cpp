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
#include "Ptr.h"
#include "Type.h"
#include "Variable.h"

namespace liteser
{
	Variable::Variable(chstr name, Ptr<char>* ptr)
	{
		this->name = name;
		this->type = new Type(ptr);
		this->ptr = (void*)ptr;
	}

	Variable::Variable(chstr name, Ptr<unsigned char>* ptr)
	{
		this->name = name;
		this->type = new Type(ptr);
		this->ptr = (void*)ptr;
	}

	Variable::Variable(chstr name, Ptr<int16_t>* ptr)
	{
		this->name = name;
		this->type = new Type(ptr);
		this->ptr = (void*)ptr;
	}

	Variable::Variable(chstr name, Ptr<uint16_t>* ptr)
	{
		this->name = name;
		this->type = new Type(ptr);
		this->ptr = (void*)ptr;
	}

	Variable::Variable(chstr name, Ptr<int32_t>* ptr)
	{
		this->name = name;
		this->type = new Type(ptr);
		this->ptr = (void*)ptr;
	}

	Variable::Variable(chstr name, Ptr<uint32_t>* ptr)
	{
		this->name = name;
		this->type = new Type(ptr);
		this->ptr = (void*)ptr;
	}

	Variable::Variable(chstr name, Ptr<float>* ptr)
	{
		this->name = name;
		this->type = new Type(ptr);
		this->ptr = (void*)ptr;
	}

	Variable::Variable(chstr name, Ptr<double>* ptr)
	{
		this->name = name;
		this->type = new Type(ptr);
		this->ptr = (void*)ptr;
	}

	Variable::Variable(chstr name, Ptr<bool>* ptr)
	{
		this->name = name;
		this->type = new Type(ptr);
		this->ptr = (void*)ptr;
	}

	Variable::Variable(chstr name, Ptr<hstr>* ptr)
	{
		this->name = name;
		this->type = new Type(ptr);
		this->ptr = (void*)ptr;
	}
	
	Variable::~Variable()
	{
		if (this->type != NULL)
		{
			delete this->type;
		}
		if (this->ptr != NULL)
		{
			delete this->ptr;
		}
		foreach (Variable*, it, this->subVariables)
		{
			delete (*it);
		}
	}

	Variable* Variable::createSubVariable()
	{
		if (this->type->subType == NULL)
		{
			throw hl_exception(hsprintf("Variable type does not contain a subtype anymore: %s", this->name.c_str()));
		}
		Variable* variable = this->type->createSubVariable();
		if (variable == NULL)
		{
			throw hl_exception(hsprintf("Subtype of variable does not exist: %s; type: %02X", this->name.c_str(), this->type->subType->value));
		}
		return variable;
	}

}
