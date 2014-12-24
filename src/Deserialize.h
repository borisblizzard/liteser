/// @file
/// @version 2.2
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
#include <hltypes/hmap.h>
#include <hltypes/hsbase.h>
#include <hltypes/hstring.h>

#include "Type.h"

namespace liteser
{
	class Serializable;
	class Variable;

	inline void __loadVariable(Variable* variable, Type::Value loadType);
	inline bool __skipVariable(Type::Value loadType);
	void __loadContainer(Variable* variable, Type::Value type);
	bool __skipContainer(Type::Value loadType);

	void _load(hstr* value);
	void _load(grect* value);
	void _load(gvec2* value);
	void _load(gvec3* value);
	void _load(Serializable* value);
	void _load(Serializable** value);
	void __loadObject(Serializable** value);
	bool __skipObject();

	void _loadHarray(harray<int8_t>* value);
	void _loadHarray(harray<uint8_t>* value);
	void _loadHarray(harray<int16_t>* value);
	void _loadHarray(harray<uint16_t>* value);
	void _loadHarray(harray<int32_t>* value);
	void _loadHarray(harray<uint32_t>* value);
	void _loadHarray(harray<int64_t>* value);
	void _loadHarray(harray<uint64_t>* value);
	void _loadHarray(harray<float>* value);
	void _loadHarray(harray<double>* value);
	void _loadHarray(harray<hstr>* value);
	void _loadHarray(harray<grect>* value);
	void _loadHarray(harray<gvec2>* value);
	void _loadHarray(harray<gvec3>* value);
	void _loadHarray(harray<Serializable*>* value);

}
#endif
