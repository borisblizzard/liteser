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

#define LITESER_LS3_EXTENSION ".ls3"
#define LITESER_LSX_EXTENSION ".lsx"

namespace liteser
{
	liteserExport extern hstr logTag;
	liteserExport extern hversion version;

	liteserFnExport bool serialize(hsbase* stream, Serializable* object, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, const harray<Serializable*>& object, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, const harray<char>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, const harray<unsigned char>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, const harray<short>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, const harray<unsigned short>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, const harray<int>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, const harray<unsigned int>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, const harray<int64_t>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, const harray<uint64_t>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, const harray<float>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, const harray<double>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, const harray<hstr>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, const harray<hversion>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, const harray<henum>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, const harray<grectf>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, const harray<gvec2f>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, const harray<gvec3f>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, const harray<grecti>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, const harray<gvec2i>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, const harray<gvec3i>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, const harray<grectd>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, const harray<gvec2d>& value, bool allowMultiReferencing = true, bool stringPooling = true);
	liteserFnExport bool serialize(hsbase* stream, const harray<gvec3d>& value, bool allowMultiReferencing = true, bool stringPooling = true);

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
		liteserFnExport bool serialize(hsbase* stream, const harray<Serializable*>& object, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, const harray<char>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, const harray<unsigned char>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, const harray<short>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, const harray<unsigned short>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, const harray<int>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, const harray<unsigned int>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, const harray<int64_t>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, const harray<uint64_t>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, const harray<float>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, const harray<double>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, const harray<hstr>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, const harray<hversion>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, const harray<henum>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, const harray<grectf>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, const harray<gvec2f>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, const harray<gvec3f>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, const harray<grecti>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, const harray<gvec2i>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, const harray<gvec3i>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, const harray<grectd>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, const harray<gvec2d>& value, bool allowMultiReferencing = true);
		liteserFnExport bool serialize(hsbase* stream, const harray<gvec3d>& value, bool allowMultiReferencing = true);

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

	liteserFnExport bool loadArrayFromFile(chstr path, harray<liteser::Serializable*>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromFile(chstr path, harray<char>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromFile(chstr path, harray<unsigned char>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromFile(chstr path, harray<short>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromFile(chstr path, harray<unsigned short>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromFile(chstr path, harray<int>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromFile(chstr path, harray<unsigned int>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromFile(chstr path, harray<int64_t>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromFile(chstr path, harray<uint64_t>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromFile(chstr path, harray<float>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromFile(chstr path, harray<double>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromFile(chstr path, harray<hstr>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromFile(chstr path, harray<hversion>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromFile(chstr path, harray<henum>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromFile(chstr path, harray<grectf>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromFile(chstr path, harray<gvec2f>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromFile(chstr path, harray<gvec3f>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromFile(chstr path, harray<grecti>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromFile(chstr path, harray<gvec2i>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromFile(chstr path, harray<gvec3i>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromFile(chstr path, harray<grectd>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromFile(chstr path, harray<gvec2d>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromFile(chstr path, harray<gvec3d>* dataArray, bool warn = true);

	liteserFnExport bool loadArrayFromResource(chstr path, harray<liteser::Serializable*>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromResource(chstr path, harray<char>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromResource(chstr path, harray<unsigned char>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromResource(chstr path, harray<short>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromResource(chstr path, harray<unsigned short>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromResource(chstr path, harray<int>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromResource(chstr path, harray<unsigned int>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromResource(chstr path, harray<int64_t>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromResource(chstr path, harray<uint64_t>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromResource(chstr path, harray<float>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromResource(chstr path, harray<double>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromResource(chstr path, harray<hstr>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromResource(chstr path, harray<hversion>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromResource(chstr path, harray<henum>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromResource(chstr path, harray<grectf>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromResource(chstr path, harray<gvec2f>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromResource(chstr path, harray<gvec3f>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromResource(chstr path, harray<grecti>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromResource(chstr path, harray<gvec2i>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromResource(chstr path, harray<gvec3i>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromResource(chstr path, harray<grectd>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromResource(chstr path, harray<gvec2d>* dataArray, bool warn = true);
	liteserFnExport bool loadArrayFromResource(chstr path, harray<gvec3d>* dataArray, bool warn = true);

	/// @param[out] object Output object.
	/// @return True if successful.
	liteserFnExport bool loadObjectFromFile(chstr path, liteser::Serializable** object, bool warn = true);
	/// @param[out] object Output object.
	/// @return True if successful.
	liteserFnExport bool loadObjectFromResource(chstr path, liteser::Serializable** object, bool warn = true);
	/// @param[out] dataArray Output object array.
	/// @return True if successful.
	liteserFnExport bool loadArrayFromDirectory(chstr path, harray<liteser::Serializable*>* dataArray, bool warn = true);
	/// @param[out] dataArray Output object array.
	/// @return True if successful.
	liteserFnExport bool loadArrayFromResourceDirectory(chstr path, harray<liteser::Serializable*>* dataArray, bool warn = true);

	/// @brief Creates a deep copy of an object.
	/// @note The cloning only affects variables that were declared as serializable.
	liteserFnExport bool clone(Serializable* input, Serializable** output);

}
#endif
