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

#define DEFINE_CONSTRUCTOR(typeName) \
	Variable::Variable(chstr name, Ptr<typeName>* ptr) \
	{ \
		this->name = name; \
		this->type = new Type(ptr); \
		this->ptr = (void*)ptr; \
	}

#define CHECK_MAP_KEY_VALUE_TYPE(keyTypeValue, valueTypeValue) \
	switch (keyTypeValue) \
	{ \
	case Type::INT8:	_CHECK_MAP_VALUE_TYPE(char, valueTypeValue);			break; \
	case Type::UINT8:	_CHECK_MAP_VALUE_TYPE(unsigned char, valueTypeValue);	break; \
	case Type::INT16:	_CHECK_MAP_VALUE_TYPE(int16_t, valueTypeValue);			break; \
	case Type::UINT16:	_CHECK_MAP_VALUE_TYPE(uint16_t, valueTypeValue);		break; \
	case Type::INT32:	_CHECK_MAP_VALUE_TYPE(int32_t, valueTypeValue);			break; \
	case Type::UINT32:	_CHECK_MAP_VALUE_TYPE(uint32_t, valueTypeValue);		break; \
	case Type::FLOAT:	_CHECK_MAP_VALUE_TYPE(float, valueTypeValue);			break; \
	case Type::DOUBLE:	_CHECK_MAP_VALUE_TYPE(double, valueTypeValue);			break; \
	case Type::HSTR:	_CHECK_MAP_VALUE_TYPE(hstr, valueTypeValue);			break; \
	}

#define _CHECK_MAP_VALUE_TYPE(keyType, value) \
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
		case Type::OBJECT:	this->_addSubVariablesHmap<keyType, Serializable>(size);	return; \
		case Type::OBJPTR:	this->_addSubVariablesHmap<keyType, Serializable*>(size);	return; \
		} \
	}

namespace liteser
{
	DEFINE_CONSTRUCTOR(char);
	DEFINE_CONSTRUCTOR(unsigned char);
	DEFINE_CONSTRUCTOR(int16_t);
	DEFINE_CONSTRUCTOR(uint16_t);
	DEFINE_CONSTRUCTOR(int32_t);
	DEFINE_CONSTRUCTOR(uint32_t);
	DEFINE_CONSTRUCTOR(float);
	DEFINE_CONSTRUCTOR(double);
	DEFINE_CONSTRUCTOR(bool);
	DEFINE_CONSTRUCTOR(hstr);

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

	void Variable::createSubVariables(unsigned int size, Type::Value type)
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
		case Type::HLIST:
			switch (this->type->subTypes[0]->value)
			{
			case Type::INT8:	this->_addSubVariablesHlist<char>(size);			return;
			case Type::UINT8:	this->_addSubVariablesHlist<unsigned char>(size);	return;
			case Type::INT16:	this->_addSubVariablesHlist<int16_t>(size);			return;
			case Type::UINT16:	this->_addSubVariablesHlist<uint16_t>(size);		return;
			case Type::INT32:	this->_addSubVariablesHlist<int32_t>(size);			return;
			case Type::UINT32:	this->_addSubVariablesHlist<uint32_t>(size);		return;
			case Type::FLOAT:	this->_addSubVariablesHlist<float>(size);			return;
			case Type::DOUBLE:	this->_addSubVariablesHlist<double>(size);			return;
			case Type::HSTR:	this->_addSubVariablesHlist<hstr>(size);			return;
			case Type::OBJECT:	this->_addSubVariablesHlist<Serializable>(size);	return;
			case Type::OBJPTR:	this->_addSubVariablesHlist<Serializable*>(size);	return;
			}
			throw hl_exception(hsprintf("Subtype is not supported within hlist: %s; type: %02X",
				this->name.c_str(), this->type->subTypes[0]->value));
			break;
		case Type::HDEQUE:
			switch (this->type->subTypes[0]->value)
			{
			case Type::INT8:	this->_addSubVariablesHdeque<char>(size);			return;
			case Type::UINT8:	this->_addSubVariablesHdeque<unsigned char>(size);	return;
			case Type::INT16:	this->_addSubVariablesHdeque<int16_t>(size);		return;
			case Type::UINT16:	this->_addSubVariablesHdeque<uint16_t>(size);		return;
			case Type::INT32:	this->_addSubVariablesHdeque<int32_t>(size);		return;
			case Type::UINT32:	this->_addSubVariablesHdeque<uint32_t>(size);		return;
			case Type::FLOAT:	this->_addSubVariablesHdeque<float>(size);			return;
			case Type::DOUBLE:	this->_addSubVariablesHdeque<double>(size);			return;
			case Type::HSTR:	this->_addSubVariablesHdeque<hstr>(size);			return;
			case Type::OBJECT:	this->_addSubVariablesHdeque<Serializable>(size);	return;
			case Type::OBJPTR:	this->_addSubVariablesHdeque<Serializable*>(size);	return;
			}
			throw hl_exception(hsprintf("Subtype is not supported within hdeque: %s; type: %02X",
				this->name.c_str(), this->type->subTypes[0]->value));
			break;
			/*
		case Type::HMAP:
			CHECK_MAP_KEY_VALUE_TYPE(this->type->subTypes[0]->value, this->type->subTypes[1]->value);
			throw hl_exception(hsprintf("Subtype is not supported within hmap: %s; types: %02X %02X",
				this->name.c_str(), this->type->subTypes[0]->value, this->type->subTypes[1]->value));
			break;
			*/
		}
		throw hl_exception(hsprintf("Type is not supported for: %s; type: %02X", this->name.c_str(), type));
	}

}
