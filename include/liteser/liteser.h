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
/// Defines global serialization and deserialization procedures as well as serialization macros.

#ifndef LITESER_H
#define LITESER_H

#include <stdint.h>

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hmap.h>
#include <hltypes/hsbase.h>
#include <hltypes/hstring.h>

#include "liteserExport.h"
#include "macros.h"
#include "Serializable.h"

#define _LS_VERSION_MAJOR 2
#define _LS_VERSION_MINOR 2

namespace liteser
{
	extern hstr logTag;

	liteserFnExport bool serialize(hsbase* stream, Serializable* object);
	liteserFnExport bool serialize(hsbase* stream, harray<Serializable*>& object);
	liteserFnExport bool serialize(hsbase* stream, harray<char>& value);
	liteserFnExport bool serialize(hsbase* stream, harray<unsigned char>& value);
	liteserFnExport bool serialize(hsbase* stream, harray<int16_t>& value);
	liteserFnExport bool serialize(hsbase* stream, harray<uint16_t>& value);
	liteserFnExport bool serialize(hsbase* stream, harray<int32_t>& value);
	liteserFnExport bool serialize(hsbase* stream, harray<uint32_t>& value);
	liteserFnExport bool serialize(hsbase* stream, harray<float>& value);
	liteserFnExport bool serialize(hsbase* stream, harray<double>& value);
	liteserFnExport bool serialize(hsbase* stream, harray<hstr>& value);
	liteserFnExport bool serialize(hsbase* stream, harray<grect>& value);
	liteserFnExport bool serialize(hsbase* stream, harray<gvec2>& value);
	liteserFnExport bool serialize(hsbase* stream, harray<gvec3>& value);

	liteserFnExport bool deserialize(hsbase* stream, Serializable** object);
	liteserFnExport bool deserialize(hsbase* stream, harray<Serializable*>* object);
	liteserFnExport bool deserialize(hsbase* stream, harray<char>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<unsigned char>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<int16_t>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<uint16_t>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<int32_t>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<uint32_t>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<float>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<double>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<hstr>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<grect>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<gvec2>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<gvec3>* value);

	/// @brief Creates a deep copy of an object.
	/// @note The cloning only affects variables that were declared as serializable.
	liteserFnExport bool clone(Serializable* input, Serializable** output);

}
#endif
