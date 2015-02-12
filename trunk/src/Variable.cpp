/// @file
/// @version 2.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <stdint.h>

#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

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
	DEFINE_ASSIGNERS(grect);
	DEFINE_ASSIGNERS(gvec2);
	DEFINE_ASSIGNERS(gvec3);
	DEFINE_ASSIGNER(bool);

	Variable::Variable(chstr name) : ptr(NULL), ptrKeys(NULL), ptrValues(NULL), containerSize(0)
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
			switch (this->type->subTypes[0]->value)
			{
			case Type::INT8:	this->_addSubVariablesHarray<char>();			return;
			case Type::UINT8:	this->_addSubVariablesHarray<unsigned char>();	return;
			case Type::INT16:	this->_addSubVariablesHarray<short>();			return;
			case Type::UINT16:	this->_addSubVariablesHarray<unsigned short>();	return;
			case Type::INT32:	this->_addSubVariablesHarray<int>();			return;
			case Type::UINT32:	this->_addSubVariablesHarray<unsigned int>();	return;
			case Type::INT64:	this->_addSubVariablesHarray<int64_t>();		return;
			case Type::UINT64:	this->_addSubVariablesHarray<uint64_t>();		return;
			case Type::FLOAT:	this->_addSubVariablesHarray<float>();			return;
			case Type::DOUBLE:	this->_addSubVariablesHarray<double>();			return;
			case Type::HSTR:	this->_addSubVariablesHarray<hstr>();			return;
			case Type::GRECT:	this->_addSubVariablesHarray<grect>();			return;
			case Type::GVEC2:	this->_addSubVariablesHarray<gvec2>();			return;
			case Type::GVEC3:	this->_addSubVariablesHarray<gvec3>();			return;
			case Type::OBJECT:	this->_addSubVariablesHarray<Serializable>();	return;
			case Type::OBJPTR:	this->_addSubVariablesHarray<Serializable*>();	return;
			}
			throw Exception(hsprintf("Subtype is not supported within harray: %s; type: %02X",
				this->name.cStr(), this->type->subTypes[0]->value));
			break;
		case Type::HMAP:
			switch (this->type->subTypes[0]->value)
			{
			case Type::INT8:	this->_addSubVariablesHmapKey<char>(this->type->subTypes[1]->value);			return;
			case Type::UINT8:	this->_addSubVariablesHmapKey<unsigned char>(this->type->subTypes[1]->value);	return;
			case Type::INT16:	this->_addSubVariablesHmapKey<short>(this->type->subTypes[1]->value);			return;
			case Type::UINT16:	this->_addSubVariablesHmapKey<unsigned short>(this->type->subTypes[1]->value);	return;
			case Type::INT32:	this->_addSubVariablesHmapKey<int>(this->type->subTypes[1]->value);				return;
			case Type::UINT32:	this->_addSubVariablesHmapKey<unsigned int>(this->type->subTypes[1]->value);	return;
			case Type::INT64:	this->_addSubVariablesHmapKey<int64_t>(this->type->subTypes[1]->value);			return;
			case Type::UINT64:	this->_addSubVariablesHmapKey<uint64_t>(this->type->subTypes[1]->value);		return;
			case Type::FLOAT:	this->_addSubVariablesHmapKey<float>(this->type->subTypes[1]->value);			return;
			case Type::DOUBLE:	this->_addSubVariablesHmapKey<double>(this->type->subTypes[1]->value);			return;
			case Type::HSTR:	this->_addSubVariablesHmapKey<hstr>(this->type->subTypes[1]->value);			return;
			}
			throw Exception(hsprintf("Subtype is not supported within hmap: %s; types: %02X %02X",
				this->name.cStr(), this->type->subTypes[0]->value, this->type->subTypes[1]->value));
			break;
		}
		throw Exception(hsprintf("Type is not supported for: %s; type: %02X", this->name.cStr(), type));
	}

	void Variable::applyHmapSubVariables(Type::Value type)
	{
		if (type != Type::HMAP)
		{
			throw Exception("Variable type is not hmap!");
		}
		switch (this->type->subTypes[0]->value)
		{
		case Type::INT8:	this->_applyHmapSubVariablesKey<char>(this->type->subTypes[1]->value);				return;
		case Type::UINT8:	this->_applyHmapSubVariablesKey<unsigned char>(this->type->subTypes[1]->value);		return;
		case Type::INT16:	this->_applyHmapSubVariablesKey<short>(this->type->subTypes[1]->value);				return;
		case Type::UINT16:	this->_applyHmapSubVariablesKey<unsigned short>(this->type->subTypes[1]->value);	return;
		case Type::INT32:	this->_applyHmapSubVariablesKey<int>(this->type->subTypes[1]->value);				return;
		case Type::UINT32:	this->_applyHmapSubVariablesKey<unsigned int>(this->type->subTypes[1]->value);		return;
		case Type::INT64:	this->_applyHmapSubVariablesKey<int64_t>(this->type->subTypes[1]->value);			return;
		case Type::UINT64:	this->_applyHmapSubVariablesKey<uint64_t>(this->type->subTypes[1]->value);			return;
		case Type::FLOAT:	this->_applyHmapSubVariablesKey<float>(this->type->subTypes[1]->value);				return;
		case Type::DOUBLE:	this->_applyHmapSubVariablesKey<double>(this->type->subTypes[1]->value);			return;
		case Type::HSTR:	this->_applyHmapSubVariablesKey<hstr>(this->type->subTypes[1]->value);				return;
		}
		throw Exception(hsprintf("Subtype is not supported within hmap: %s; types: %02X %02X",
			this->name.cStr(), this->type->subTypes[0]->value, this->type->subTypes[1]->value));
	}

}
