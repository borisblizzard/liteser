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

namespace liteser
{
	void Variable::applyHmapSubVariables(Type::Value type)
	{
		if (type != Type::HMAP)
		{
			throw Exception("Variable type is not hmap!");
		}
		switch (this->type->subTypes[0]->value)
		{
		case Type::INT8:		this->_applyHmapSubVariablesKey<char>(this->type->subTypes[1]->value);				return;
		case Type::UINT8:		this->_applyHmapSubVariablesKey<unsigned char>(this->type->subTypes[1]->value);		return;
		case Type::INT16:		this->_applyHmapSubVariablesKey<short>(this->type->subTypes[1]->value);				return;
		case Type::UINT16:		this->_applyHmapSubVariablesKey<unsigned short>(this->type->subTypes[1]->value);	return;
		case Type::INT32:		this->_applyHmapSubVariablesKey<int>(this->type->subTypes[1]->value);				return;
		case Type::UINT32:		this->_applyHmapSubVariablesKey<unsigned int>(this->type->subTypes[1]->value);		return;
		case Type::INT64:		this->_applyHmapSubVariablesKey<int64_t>(this->type->subTypes[1]->value);			return;
		case Type::UINT64:		this->_applyHmapSubVariablesKey<uint64_t>(this->type->subTypes[1]->value);			return;
		case Type::FLOAT:		this->_applyHmapSubVariablesKey<float>(this->type->subTypes[1]->value);				return;
		case Type::DOUBLE:		this->_applyHmapSubVariablesKey<double>(this->type->subTypes[1]->value);			return;
		case Type::HSTR:		this->_applyHmapSubVariablesKey<hstr>(this->type->subTypes[1]->value);				return;
		case Type::HVERSION:	this->_applyHmapSubVariablesKey<hversion>(this->type->subTypes[1]->value);			return;
		case Type::HENUM:		this->_applyHmapSubVariablesKey<henum>(this->type->subTypes[1]->value);				return;
		default:																									break;
		}
		throw Exception(hsprintf("Subtype is not supported within hmap: %s; types: %02X %02X", this->name.cStr(), this->type->subTypes[0]->value, this->type->subTypes[1]->value));
	}


}
