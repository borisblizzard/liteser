/// @file
/// @version 2.5
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

	void __dumpContainer(Variable* variable);

	void _dump(hstr* value);
	void _dump(hversion* value);
	void _dump(henum* value);
	void _dump(grect* value);
	void _dump(gvec2* value);
	void _dump(gvec3* value);
	void _dump(Serializable* value);
	void _dump(Serializable** value);

	void _dumpHarray(harray<char>* value);
	void _dumpHarray(harray<unsigned char>* value);
	void _dumpHarray(harray<short>* value);
	void _dumpHarray(harray<unsigned short>* value);
	void _dumpHarray(harray<int>* value);
	void _dumpHarray(harray<unsigned int>* value);
	void _dumpHarray(harray<int64_t>* value);
	void _dumpHarray(harray<uint64_t>* value);
	void _dumpHarray(harray<float>* value);
	void _dumpHarray(harray<double>* value);
	void _dumpHarray(harray<hstr>* value);
	void _dumpHarray(harray<hversion>* value);
	void _dumpHarray(harray<henum>* value);
	void _dumpHarray(harray<grect>* value);
	void _dumpHarray(harray<gvec2>* value);
	void _dumpHarray(harray<gvec3>* value);
	void _dumpHarray(harray<Serializable*>* value);

}
#endif
