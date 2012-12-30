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

namespace liteser
{
	class Serializable;
	class Variable;

	bool _tryMapObject(unsigned int* id, Serializable* object);
	Serializable* _getObject(unsigned int id);
	void _start();
	void _finish();
	bool _isActive();
	void _checkVersion(unsigned char major, unsigned char minor);

	void _dump(hsbase* stream, char* value);
	void _dump(hsbase* stream, unsigned char* value);
	void _dump(hsbase* stream, int16_t* value);
	void _dump(hsbase* stream, uint16_t* value);
	void _dump(hsbase* stream, int32_t* value);
	void _dump(hsbase* stream, uint32_t* value);
	void _dump(hsbase* stream, float* value);
	void _dump(hsbase* stream, double* value);
	void _dump(hsbase* stream, bool* value);
	void _dump(hsbase* stream, Serializable* value);
	void _dump(hsbase* stream, Serializable** value);
	void _dump(hsbase* stream, hstr* value);

	void _dump(hsbase* stream, Variable* variable);

}
#endif
