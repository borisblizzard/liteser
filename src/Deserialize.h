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
/// Defines all deserialization functions.

#ifndef LITESER_DESERIALIZE_H
#define LITESER_DESERIALIZE_H

#include <stdint.h>

#include <hltypes/hsbase.h>

#include "Type.h"

namespace liteser
{
	class Serializable;
	class Variable;

	void __loadVariable(Variable* variable);
	void __loadVariableValue(Variable* variable, Type::Value loadType);

	void __loadContainer(Variable* variable, Type::Value type);

	void _load(char* value);
	void _load(unsigned char* value);
	void _load(int16_t* value);
	void _load(uint16_t* value);
	void _load(int32_t* value);
	void _load(uint32_t* value);
	void _load(float* value);
	void _load(double* value);
	void _load(bool* value);
	void _load(hstr* value);
	void _load(Serializable* value);
	void _load(Serializable** value);
	void __loadObject(Serializable** value);

	void _loadHarray(harray<char>* value);
	void _loadHarray(harray<unsigned char>* value);
	void _loadHarray(harray<int16_t>* value);
	void _loadHarray(harray<uint16_t>* value);
	void _loadHarray(harray<int32_t>* value);
	void _loadHarray(harray<uint32_t>* value);
	void _loadHarray(harray<float>* value);
	void _loadHarray(harray<double>* value);
	void _loadHarray(harray<hstr>* value);
	void _loadHarray(harray<Serializable*>* value);

}
#endif
