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
#include "Ptr.h"
#include "Type.h"
#include "Variable.h"

#define DEFINE_CONSTRUCTOR(typeName) \
	Variable::Variable(chstr name, Ptr<typeName>* ptr) \
	{ \
		this->name = name; \
		this->type = new Type(ptr); \
		this->ptr = (void*)ptr; \
		this->ptrKeys = NULL; \
		this->ptrValues = NULL; \
	}
#define DEFINE_CONSTRUCTOR_HARRAY(typeName) \
	Variable::Variable(chstr name, Ptr<harray<typeName> >* ptr) \
	{ \
		this->name = name; \
		this->type = new Type(ptr); \
		this->ptr = (void*)ptr; \
		this->ptrKeys = NULL; \
		this->ptrValues = NULL; \
		foreach (typeName, it, *ptr->value) \
		{ \
			this->subVariables += new Variable("", new Ptr<typeName>(&(*it))); \
		} \
	}
#define DEFINE_CONSTRUCTORS(typeName) \
	DEFINE_CONSTRUCTOR(typeName); \
	DEFINE_CONSTRUCTOR_HARRAY(typeName);

#define CHECK_HMAP_KEY_VALUE_TYPE(keyTypeValue, valueTypeValue) \
	switch (keyTypeValue) \
	{ \
	case Type::INT8:	_CHECK_HMAP_VALUE_TYPE(char, valueTypeValue);			break; \
	case Type::UINT8:	_CHECK_HMAP_VALUE_TYPE(unsigned char, valueTypeValue);	break; \
	case Type::INT16:	_CHECK_HMAP_VALUE_TYPE(int16_t, valueTypeValue);			break; \
	case Type::UINT16:	_CHECK_HMAP_VALUE_TYPE(uint16_t, valueTypeValue);		break; \
	case Type::INT32:	_CHECK_HMAP_VALUE_TYPE(int32_t, valueTypeValue);			break; \
	case Type::UINT32:	_CHECK_HMAP_VALUE_TYPE(uint32_t, valueTypeValue);		break; \
	case Type::FLOAT:	_CHECK_HMAP_VALUE_TYPE(float, valueTypeValue);			break; \
	case Type::DOUBLE:	_CHECK_HMAP_VALUE_TYPE(double, valueTypeValue);			break; \
	case Type::HSTR:	_CHECK_HMAP_VALUE_TYPE(hstr, valueTypeValue);			break; \
	}
#define _CHECK_HMAP_VALUE_TYPE(keyType, value) \
	{ \
		switch (value) \
		{ \
		case Type::INT8:	this->_addSubVariablesHmap<keyType, char>(size);			return; \
		case Type::UINT8:	this->_addSubVariablesHmap<keyType, unsigned char>(size);	return; \
		case Type::INT16:	this->_addSubVariablesHmap<keyType, int16_t>(size);			return; \
		case Type::UINT16:	this->_addSubVariablesHmap<keyType, uint16_t>(size);		return; \
		case Type::INT32:	this->_addSubVariablesHmap<keyType, int32_t>(size);			return; \
		case Type::UINT32:	this->_addSubVariablesHmap<keyType, uint32_t>(size);		return; \
		case Type::FLOAT:	this->_addSubVariablesHmap<keyType, float>(size);			return; \
		case Type::DOUBLE:	this->_addSubVariablesHmap<keyType, double>(size);			return; \
		case Type::HSTR:	this->_addSubVariablesHmap<keyType, hstr>(size);			return; \
		case Type::OBJPTR:	this->_addSubVariablesHmap<keyType, Serializable*>(size);	return; \
		} \
	}

#define APPLY_HMAP_KEYS_VALUES(keyTypeValue, valueTypeValue) \
	switch (keyTypeValue) \
	{ \
	case Type::INT8:	_APPLY_HMAP_VALUES(char, valueTypeValue);			break; \
	case Type::UINT8:	_APPLY_HMAP_VALUES(unsigned char, valueTypeValue);	break; \
	case Type::INT16:	_APPLY_HMAP_VALUES(int16_t, valueTypeValue);		break; \
	case Type::UINT16:	_APPLY_HMAP_VALUES(uint16_t, valueTypeValue);		break; \
	case Type::INT32:	_APPLY_HMAP_VALUES(int32_t, valueTypeValue);		break; \
	case Type::UINT32:	_APPLY_HMAP_VALUES(uint32_t, valueTypeValue);		break; \
	case Type::FLOAT:	_APPLY_HMAP_VALUES(float, valueTypeValue);			break; \
	case Type::DOUBLE:	_APPLY_HMAP_VALUES(double, valueTypeValue);			break; \
	case Type::HSTR:	_APPLY_HMAP_VALUES(hstr, valueTypeValue);			break; \
	}
#define _APPLY_HMAP_VALUES(keyType, value) \
	{ \
		switch (value) \
		{ \
		case Type::INT8:	this->_applyHmapSubVariables<keyType, char>();			return; \
		case Type::UINT8:	this->_applyHmapSubVariables<keyType, unsigned char>();	return; \
		case Type::INT16:	this->_applyHmapSubVariables<keyType, int16_t>();		return; \
		case Type::UINT16:	this->_applyHmapSubVariables<keyType, uint16_t>();		return; \
		case Type::INT32:	this->_applyHmapSubVariables<keyType, int32_t>();		return; \
		case Type::UINT32:	this->_applyHmapSubVariables<keyType, uint32_t>();		return; \
		case Type::FLOAT:	this->_applyHmapSubVariables<keyType, float>();			return; \
		case Type::DOUBLE:	this->_applyHmapSubVariables<keyType, double>();		return; \
		case Type::HSTR:	this->_applyHmapSubVariables<keyType, hstr>();			return; \
		case Type::OBJPTR:	this->_applyHmapSubVariables<keyType, Serializable*>();	return; \
		} \
	}
	

namespace liteser
{
	DEFINE_CONSTRUCTORS(char);
	DEFINE_CONSTRUCTORS(unsigned char);
	DEFINE_CONSTRUCTORS(int16_t);
	DEFINE_CONSTRUCTORS(uint16_t);
	DEFINE_CONSTRUCTORS(int32_t);
	DEFINE_CONSTRUCTORS(uint32_t);
	DEFINE_CONSTRUCTORS(float);
	DEFINE_CONSTRUCTORS(double);
	DEFINE_CONSTRUCTORS(hstr);
	DEFINE_CONSTRUCTOR(bool);

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
		if (this->ptrKeys != NULL)
		{
			delete this->ptrKeys;
		}
		if (this->ptrValues != NULL)
		{
			delete this->ptrValues;
		}
		foreach (Variable*, it, this->subVariables)
		{
			delete (*it);
		}
	}

	void Variable::createSubVariables(Type::Value type, unsigned int size)
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
			case Type::INT8:	this->_addSubVariablesHarray<char>(size);			return;
			case Type::UINT8:	this->_addSubVariablesHarray<unsigned char>(size);	return;
			case Type::INT16:	this->_addSubVariablesHarray<int16_t>(size);		return;
			case Type::UINT16:	this->_addSubVariablesHarray<uint16_t>(size);		return;
			case Type::INT32:	this->_addSubVariablesHarray<int32_t>(size);		return;
			case Type::UINT32:	this->_addSubVariablesHarray<uint32_t>(size);		return;
			case Type::FLOAT:	this->_addSubVariablesHarray<float>(size);			return;
			case Type::DOUBLE:	this->_addSubVariablesHarray<double>(size);			return;
			case Type::HSTR:	this->_addSubVariablesHarray<hstr>(size);			return;
			case Type::OBJECT:	this->_addSubVariablesHarray<Serializable>(size);	return;
			case Type::OBJPTR:	this->_addSubVariablesHarray<Serializable*>(size);	return;
			}
			throw hl_exception(hsprintf("Subtype is not supported within harray: %s; type: %02X",
				this->name.c_str(), this->type->subTypes[0]->value));
			break;
		case Type::HMAP:
			CHECK_HMAP_KEY_VALUE_TYPE(this->type->subTypes[0]->value, this->type->subTypes[1]->value);
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
		APPLY_HMAP_KEYS_VALUES(this->type->subTypes[0]->value, this->type->subTypes[1]->value);
		throw hl_exception(hsprintf("Subtype is not supported within hmap: %s; types: %02X %02X",
			this->name.c_str(), this->type->subTypes[0]->value, this->type->subTypes[1]->value));
	}

}
