/// @file
/// @version 2.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @note This file is split to avoid having to use the /bigobj option as it disables edit-and-continue.

#include <stdint.h>

#include <hltypes/harray.h>
#include <hltypes/henum.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>
#include <hltypes/hversion.h>

#include "liteser.h"
#include "Ptr.h"
#include "Type.h"
#include "Variable.h"

#define DEFINE_ASSIGNER(typeName) \
	Variable* Variable::assign(VPtr<typeName>* ptr) \
	{ \
		this->type->assign(ptr); \
		this->ptr = ptr; \
		return this; \
	}
#define DEFINE_ASSIGNER_HARRAY(typeName) \
	Variable* Variable::assign(VPtr<harray<typeName> >* ptr) \
	{ \
		this->type->assign(ptr); \
		this->ptr = ptr; \
		this->containerSize = ptr->value->size(); \
		foreach (typeName, it, *ptr->value) \
		{ \
			this->subVariables += (new Variable())->assign(new VPtr<typeName>(&(*it))); \
		} \
		return this; \
	}
#define DEFINE_ASSIGNERS(typeName) \
	DEFINE_ASSIGNER(typeName); \
	DEFINE_ASSIGNER_HARRAY(typeName);

namespace liteser
{
	DEFINE_ASSIGNERS(char);
	DEFINE_ASSIGNERS(unsigned char);
	DEFINE_ASSIGNERS(short);
	DEFINE_ASSIGNERS(unsigned short);
	DEFINE_ASSIGNERS(int);
	DEFINE_ASSIGNERS(unsigned int);
	DEFINE_ASSIGNERS(int64_t);
	DEFINE_ASSIGNERS(uint64_t);
	DEFINE_ASSIGNERS(float);
	DEFINE_ASSIGNERS(double);
	DEFINE_ASSIGNERS(hstr);
	DEFINE_ASSIGNERS(hversion);
	DEFINE_ASSIGNERS(grect);
	DEFINE_ASSIGNERS(gvec2);
	DEFINE_ASSIGNERS(gvec3);
	DEFINE_ASSIGNER(bool);

	Variable::Variable(chstr name) : ptr(NULL), containerSize(0), ptrKeys(NULL), ptrValues(NULL)
	{
		this->name = name;
		this->type = new Type();
	}

	Variable::~Variable()
	{
		foreach (Variable*, it, this->subVariables)
		{
			delete (*it);
		}
		delete this->type;
		if (this->ptr != NULL)
		{
			delete this->ptr;
		}
		if (this->ptrKeys != NULL)
		{
			delete this->ptrKeys;
		}
		if (this->ptrValues != NULL)
		{
			delete this->ptrValues;
		}
	}

	void Variable::createSubVariables(Type::Value type)
	{
		if (this->type->subTypes.size() == 0)
		{
			throw Exception("Variable type does not contain subtypes: " + this->name);
		}
		switch (type)
		{
		case Type::HARRAY:
			if (this->_createSubVariablesHarray())
			{
				return;
			}
			throw Exception(hsprintf("Subtype is not supported within harray: %s; type: %02X", this->name.cStr(), this->type->subTypes[0]->value));
			break;
		case Type::HMAP:
			if (this->_createSubVariablesHmap())
			{
				return;
			}
			throw Exception(hsprintf("Subtype is not supported within hmap: %s; types: %02X %02X", this->name.cStr(), this->type->subTypes[0]->value, this->type->subTypes[1]->value));
			break;
		default:
			break;
		}
		throw Exception(hsprintf("Type is not supported for: %s; type: %02X", this->name.cStr(), type));
	}

}
