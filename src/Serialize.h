/// @file
/// @version 3.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines all serialization functions.

#ifndef LITESER_SERIALIZE_H
#define LITESER_SERIALIZE_H

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

	void _dumpType(const Type::Identifier& identifier);

	void __dumpContainer(Variable* variable);

	void _dump(const hstr* value);
	void _dump(const hversion* value);
	void _dump(const henum* value);
	void _dump(const grectf* value);
	void _dump(const gvec2f* value);
	void _dump(const gvec3f* value);
	void _dump(const grecti* value);
	void _dump(const gvec2i* value);
	void _dump(const gvec3i* value);
	void _dump(const grectd* value);
	void _dump(const gvec2d* value);
	void _dump(const gvec3d* value);
	void _dump(Serializable* value);
	void _dump(Serializable** value);

	void _dumpHarray(const harray<char>* value);
	void _dumpHarray(const harray<unsigned char>* value);
	void _dumpHarray(const harray<short>* value);
	void _dumpHarray(const harray<unsigned short>* value);
	void _dumpHarray(const harray<int>* value);
	void _dumpHarray(const harray<unsigned int>* value);
	void _dumpHarray(const harray<int64_t>* value);
	void _dumpHarray(const harray<uint64_t>* value);
	void _dumpHarray(const harray<float>* value);
	void _dumpHarray(const harray<double>* value);
	void _dumpHarray(const harray<hstr>* value);
	void _dumpHarray(const harray<hversion>* value);
	void _dumpHarray(const harray<henum>* value);
	void _dumpHarray(const harray<grectf>* value);
	void _dumpHarray(const harray<gvec2f>* value);
	void _dumpHarray(const harray<gvec3f>* value);
	void _dumpHarray(const harray<grecti>* value);
	void _dumpHarray(const harray<gvec2i>* value);
	void _dumpHarray(const harray<gvec3i>* value);
	void _dumpHarray(const harray<grectd>* value);
	void _dumpHarray(const harray<gvec2d>* value);
	void _dumpHarray(const harray<gvec3d>* value);
	void _dumpHarray(const harray<Serializable*>* value);

}
#endif
