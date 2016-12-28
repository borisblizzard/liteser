/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines all deserialization functions.

#ifndef LITESER_DESERIALIZE_H
#define LITESER_DESERIALIZE_H

#include <stdint.h>

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/henum.h>
#include <hltypes/hmap.h>
#include <hltypes/hsbase.h>
#include <hltypes/hstring.h>
#include <hltypes/hversion.h>

#include "Type.h"

namespace liteser
{
	class Serializable;
	class Variable;

	Type::Value _loadType();

	void __loadContainer(Variable* variable, Type::Value type);
	bool __skipContainer(Type::Value loadType);

	void _load(hstr* value);
	void _load(hversion* value);
	void _load(henum* value);
	void _load(grect* value);
	void _load(gvec2* value);
	void _load(gvec3* value);
	void _load(Serializable* value);
	void _load(Serializable** value);
	void __loadObject(Serializable** value);
	bool __skipObject();

	void _loadHarray(harray<char>* value, unsigned int size);
	void _loadHarray(harray<unsigned char>* value, unsigned int size);
	void _loadHarray(harray<short>* value, unsigned int size);
	void _loadHarray(harray<unsigned short>* value, unsigned int size);
	void _loadHarray(harray<int>* value, unsigned int size);
	void _loadHarray(harray<unsigned int>* value, unsigned int size);
	void _loadHarray(harray<int64_t>* value, unsigned int size);
	void _loadHarray(harray<uint64_t>* value, unsigned int size);
	void _loadHarray(harray<float>* value, unsigned int size);
	void _loadHarray(harray<double>* value, unsigned int size);
	void _loadHarray(harray<hstr>* value, unsigned int size);
	void _loadHarray(harray<hversion>* value, unsigned int size);
	void _loadHarray(harray<henum>* value, unsigned int size);
	void _loadHarray(harray<grect>* value, unsigned int size);
	void _loadHarray(harray<gvec2>* value, unsigned int size);
	void _loadHarray(harray<gvec3>* value, unsigned int size);
	void _loadHarray(harray<Serializable*>* value, unsigned int size);

}
#endif
