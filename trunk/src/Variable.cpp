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
	DEFINE_ASSIGNERS(int8_t);
	DEFINE_ASSIGNERS(uint8_t);
	DEFINE_ASSIGNERS(int16_t);
	DEFINE_ASSIGNERS(uint16_t);
	DEFINE_ASSIGNERS(int32_t);
	DEFINE_ASSIGNERS(uint32_t);
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
			throw hl_exception("Variable type does not contain subtypes: " + this->name);
		}
		switch (type)
		{
		case Type::HARRAY:
			switch (this->type->subTypes[0]->value)
			{
			case Type::INT8:	this->_addSubVariablesHarray<int8_t>();			return;
			case Type::UINT8:	this->_addSubVariablesHarray<uint8_t>();		return;
			case Type::INT16:	this->_addSubVariablesHarray<int16_t>();		return;
			case Type::UINT16:	this->_addSubVariablesHarray<uint16_t>();		return;
			case Type::INT32:	this->_addSubVariablesHarray<int32_t>();		return;
			case Type::UINT32:	this->_addSubVariablesHarray<uint32_t>();		return;
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
			throw hl_exception(hsprintf("Subtype is not supported within harray: %s; type: %02X",
				this->name.c_str(), this->type->subTypes[0]->value));
			break;
		case Type::HMAP:
			switch (this->type->subTypes[0]->value)
			{
			case Type::INT8:	this->_addSubVariablesHmapKey<int8_t>(this->type->subTypes[1]->value);			return;
			case Type::UINT8:	this->_addSubVariablesHmapKey<uint8_t>(this->type->subTypes[1]->value);			return;
			case Type::INT16:	this->_addSubVariablesHmapKey<int16_t>(this->type->subTypes[1]->value);			return;
			case Type::UINT16:	this->_addSubVariablesHmapKey<uint16_t>(this->type->subTypes[1]->value);		return;
			case Type::INT32:	this->_addSubVariablesHmapKey<int32_t>(this->type->subTypes[1]->value);			return;
			case Type::UINT32:	this->_addSubVariablesHmapKey<uint32_t>(this->type->subTypes[1]->value);		return;
			case Type::INT64:	this->_addSubVariablesHmapKey<int64_t>(this->type->subTypes[1]->value);			return;
			case Type::UINT64:	this->_addSubVariablesHmapKey<uint64_t>(this->type->subTypes[1]->value);		return;
			case Type::FLOAT:	this->_addSubVariablesHmapKey<float>(this->type->subTypes[1]->value);			return;
			case Type::DOUBLE:	this->_addSubVariablesHmapKey<double>(this->type->subTypes[1]->value);			return;
			case Type::HSTR:	this->_addSubVariablesHmapKey<hstr>(this->type->subTypes[1]->value);			return;
			}
			throw hl_exception(hsprintf("Subtype is not supported within hmap: %s; types: %02X %02X",
				this->name.c_str(), this->type->subTypes[0]->value, this->type->subTypes[1]->value));
			break;
		}
		throw hl_exception(hsprintf("Type is not supported for: %s; type: %02X", this->name.c_str(), type));
	}

	void Variable::applyHmapSubVariables(Type::Value type)
	{
		if (type != Type::HMAP)
		{
			throw hl_exception("Variable type is not hmap!");
		}
		switch (this->type->subTypes[0]->value)
		{
		case Type::INT8:	this->_applyHmapSubVariablesKey<int8_t>(this->type->subTypes[1]->value);		return;
		case Type::UINT8:	this->_applyHmapSubVariablesKey<uint8_t>(this->type->subTypes[1]->value);		return;
		case Type::INT16:	this->_applyHmapSubVariablesKey<int16_t>(this->type->subTypes[1]->value);		return;
		case Type::UINT16:	this->_applyHmapSubVariablesKey<uint16_t>(this->type->subTypes[1]->value);		return;
		case Type::INT32:	this->_applyHmapSubVariablesKey<int32_t>(this->type->subTypes[1]->value);		return;
		case Type::UINT32:	this->_applyHmapSubVariablesKey<uint32_t>(this->type->subTypes[1]->value);		return;
		case Type::INT64:	this->_applyHmapSubVariablesKey<int64_t>(this->type->subTypes[1]->value);		return;
		case Type::UINT64:	this->_applyHmapSubVariablesKey<uint64_t>(this->type->subTypes[1]->value);		return;
		case Type::FLOAT:	this->_applyHmapSubVariablesKey<float>(this->type->subTypes[1]->value);			return;
		case Type::DOUBLE:	this->_applyHmapSubVariablesKey<double>(this->type->subTypes[1]->value);		return;
		case Type::HSTR:	this->_applyHmapSubVariablesKey<hstr>(this->type->subTypes[1]->value);			return;
		}
		throw hl_exception(hsprintf("Subtype is not supported within hmap: %s; types: %02X %02X",
			this->name.c_str(), this->type->subTypes[0]->value, this->type->subTypes[1]->value));
	}

}
