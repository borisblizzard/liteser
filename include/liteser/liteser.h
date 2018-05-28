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
/// Defines global serialization and deserialization procedures as well as serialization macros.

#ifndef LITESER_H
#define LITESER_H

#include <stdint.h>

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/henum.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hmap.h>
#include <hltypes/hsbase.h>
#include <hltypes/hstring.h>
#include <hltypes/hversion.h>

#include "liteserExport.h"
#include "macros.h"
#include "Serializable.h"

namespace liteser
{
	liteserExport extern hstr logTag;
	liteserExport extern hversion version;

	liteserFnExport bool serialize(hsbase* stream, Serializable* object, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, harray<Serializable*>& object, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, harray<char>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, harray<unsigned char>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, harray<short>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, harray<unsigned short>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, harray<int>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, harray<unsigned int>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, harray<int64_t>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, harray<uint64_t>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, harray<float>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, harray<double>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, harray<hstr>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, harray<hversion>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, harray<henum>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, harray<grectf>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, harray<gvec2f>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, harray<gvec3f>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, harray<grecti>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, harray<gvec2i>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, harray<gvec3i>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, harray<grectd>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, harray<gvec2d>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, harray<gvec3d>& value, bool allowMultiReferencing = true, bool stringPooling = true);

	liteserFnExport bool deserialize(hsbase* stream, Serializable** object);
	liteserFnExport bool deserialize(hsbase* stream, harray<Serializable*>* object);
	liteserFnExport bool deserialize(hsbase* stream, harray<char>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<unsigned char>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<short>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<unsigned short>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<int>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<unsigned int>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<int64_t>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<uint64_t>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<float>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<double>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<hstr>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<hversion>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<henum>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<grectf>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<gvec2f>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<gvec3f>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<grecti>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<gvec2i>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<gvec3i>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<grectd>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<gvec2d>* value);
	liteserFnExport bool deserialize(hsbase* stream, harray<gvec3d>* value);

	namespace xml
	{
		liteserFnExport bool serialize(hsbase* stream, Serializable* object, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, harray<Serializable*>& object, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, harray<char>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, harray<unsigned char>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, harray<short>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, harray<unsigned short>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, harray<int>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, harray<unsigned int>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, harray<int64_t>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, harray<uint64_t>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, harray<float>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, harray<double>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, harray<hstr>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, harray<hversion>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, harray<henum>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, harray<grectf>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, harray<gvec2f>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, harray<gvec3f>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, harray<grecti>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, harray<gvec2i>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, harray<gvec3i>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, harray<grectd>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, harray<gvec2d>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, harray<gvec3d>& value, bool allowMultiReferencing = true);

		liteserFnExport bool deserialize(hsbase* stream, Serializable** object);
		liteserFnExport bool deserialize(hsbase* stream, harray<Serializable*>* object);
		liteserFnExport bool deserialize(hsbase* stream, harray<char>* value);
		liteserFnExport bool deserialize(hsbase* stream, harray<unsigned char>* value);
		liteserFnExport bool deserialize(hsbase* stream, harray<short>* value);
		liteserFnExport bool deserialize(hsbase* stream, harray<unsigned short>* value);
		liteserFnExport bool deserialize(hsbase* stream, harray<int>* value);
		liteserFnExport bool deserialize(hsbase* stream, harray<unsigned int>* value);
		liteserFnExport bool deserialize(hsbase* stream, harray<int64_t>* value);
		liteserFnExport bool deserialize(hsbase* stream, harray<uint64_t>* value);
		liteserFnExport bool deserialize(hsbase* stream, harray<float>* value);
		liteserFnExport bool deserialize(hsbase* stream, harray<double>* value);
		liteserFnExport bool deserialize(hsbase* stream, harray<hstr>* value);
		liteserFnExport bool deserialize(hsbase* stream, harray<hversion>* value);
		liteserFnExport bool deserialize(hsbase* stream, harray<henum>* value);
		liteserFnExport bool deserialize(hsbase* stream, harray<grectf>* value);
		liteserFnExport bool deserialize(hsbase* stream, harray<gvec2f>* value);
		liteserFnExport bool deserialize(hsbase* stream, harray<gvec3f>* value);
		liteserFnExport bool deserialize(hsbase* stream, harray<grecti>* value);
		liteserFnExport bool deserialize(hsbase* stream, harray<gvec2i>* value);
		liteserFnExport bool deserialize(hsbase* stream, harray<gvec3i>* value);
		liteserFnExport bool deserialize(hsbase* stream, harray<grectd>* value);
		liteserFnExport bool deserialize(hsbase* stream, harray<gvec2d>* value);
		liteserFnExport bool deserialize(hsbase* stream, harray<gvec3d>* value);
	}

	/// @brief Creates a deep copy of an object.
	/// @note The cloning only affects variables that were declared as serializable.
	liteserFnExport bool clone(Serializable* input, Serializable** output);

}
#endif
