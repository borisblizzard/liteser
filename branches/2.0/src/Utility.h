/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines utility functions.

#ifndef LITESER_UTILITY_H
#define LITESER_UTILITY_H

#include <hltypes/hsbase.h>

#include "Variable.h"

namespace liteser
{
	class Serializable;

	bool __tryMapObject(unsigned int* id, Serializable* object);
	Serializable* __getObject(unsigned int id);
	void _start(hsbase* stream);
	void _finish(hsbase* stream);
	bool _isActive();
	void _checkVersion(unsigned char major, unsigned char minor);

	void __dumpVariable(Variable* variable);

	void _dump(char* value);
	void _dump(unsigned char* value);
	void _dump(int16_t* value);
	void _dump(uint16_t* value);
	void _dump(int32_t* value);
	void _dump(uint32_t* value);
	void _dump(float* value);
	void _dump(double* value);
	void _dump(bool* value);
	void _dump(Serializable* value);
	void _dump(Serializable** value);
	void _dump(hstr* value);


}
#endif
