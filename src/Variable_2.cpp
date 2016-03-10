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
	bool Variable::_createSubVariablesHarray()
	{
		switch (this->type->subTypes[0]->value)
		{
		case Type::INT8:		this->_addSubVariablesHarray<char>();			return true;
		case Type::UINT8:		this->_addSubVariablesHarray<unsigned char>();	return true;
		case Type::INT16:		this->_addSubVariablesHarray<short>();			return true;
		case Type::UINT16:		this->_addSubVariablesHarray<unsigned short>();	return true;
		case Type::INT32:		this->_addSubVariablesHarray<int>();			return true;
		case Type::UINT32:		this->_addSubVariablesHarray<unsigned int>();	return true;
		case Type::INT64:		this->_addSubVariablesHarray<int64_t>();		return true;
		case Type::UINT64:		this->_addSubVariablesHarray<uint64_t>();		return true;
		case Type::FLOAT:		this->_addSubVariablesHarray<float>();			return true;
		case Type::DOUBLE:		this->_addSubVariablesHarray<double>();			return true;
		case Type::HSTR:		this->_addSubVariablesHarray<hstr>();			return true;
		case Type::HVERSION:	this->_addSubVariablesHarray<hversion>();		return true;
		case Type::HENUM:		this->_addSubVariablesHarray<henum>();			return true;
		case Type::GRECT:		this->_addSubVariablesHarray<grect>();			return true;
		case Type::GVEC2:		this->_addSubVariablesHarray<gvec2>();			return true;
		case Type::GVEC3:		this->_addSubVariablesHarray<gvec3>();			return true;
		case Type::OBJECT:		this->_addSubVariablesHarray<Serializable>();	return true;
		case Type::OBJPTR:		this->_addSubVariablesHarray<Serializable*>();	return true;
		default:																break;
		}
		return false;
	}

}
