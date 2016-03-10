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

#include "Type.h"
#include "Variable.h"

namespace liteser
{
	bool Variable::_createSubVariablesHmap()
	{
		switch (this->type->subTypes[0]->value)
		{
		case Type::INT8:		this->_addSubVariablesHmapKey<char>(this->type->subTypes[1]->value);			return true;
		case Type::UINT8:		this->_addSubVariablesHmapKey<unsigned char>(this->type->subTypes[1]->value);	return true;
		case Type::INT16:		this->_addSubVariablesHmapKey<short>(this->type->subTypes[1]->value);			return true;
		case Type::UINT16:		this->_addSubVariablesHmapKey<unsigned short>(this->type->subTypes[1]->value);	return true;
		case Type::INT32:		this->_addSubVariablesHmapKey<int>(this->type->subTypes[1]->value);				return true;
		case Type::UINT32:		this->_addSubVariablesHmapKey<unsigned int>(this->type->subTypes[1]->value);	return true;
		case Type::INT64:		this->_addSubVariablesHmapKey<int64_t>(this->type->subTypes[1]->value);			return true;
		case Type::UINT64:		this->_addSubVariablesHmapKey<uint64_t>(this->type->subTypes[1]->value);		return true;
		case Type::FLOAT:		this->_addSubVariablesHmapKey<float>(this->type->subTypes[1]->value);			return true;
		case Type::DOUBLE:		this->_addSubVariablesHmapKey<double>(this->type->subTypes[1]->value);			return true;
		case Type::HSTR:		this->_addSubVariablesHmapKey<hstr>(this->type->subTypes[1]->value);			return true;
		case Type::HVERSION:	this->_addSubVariablesHmapKey<hversion>(this->type->subTypes[1]->value);		return true;
		case Type::HENUM:		this->_addSubVariablesHmapKey<henum>(this->type->subTypes[1]->value);			return true;
		default:																								break;
		}
		return false;
	}

}
