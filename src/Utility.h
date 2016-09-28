/// @file
/// @version 2.7
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines utility functions.

#ifndef LITESER_UTILITY_H
#define LITESER_UTILITY_H

#include <hltypes/hsbase.h>

#include "Variable.h"
#include "Type.h"

namespace liteser
{
	class Serializable;

	extern hsbase* stream;
	extern hstr _indent;
	
	bool __tryGetObject(unsigned int id, Serializable** object);
	bool __tryMapObject(unsigned int* id, Serializable* object);
	bool __tryGetString(unsigned int id, hstr* string);
	bool __tryMapString(unsigned int* id, chstr string);
	void __forceMapEmptyObject();
	void _start(hsbase* stream);
	void _finish(hsbase* stream);
	bool _isActive();
	bool _isCompatibleType(const Type::Value& variableType, const Type::Value& loadedType);
	void _checkVersion(unsigned char major, unsigned char minor);

}
#endif
