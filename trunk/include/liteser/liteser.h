/// @file
/// @author  Boris Mikic
/// @version 1.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines global serialization and deserialization procedures as well as serialization macros.

#ifndef LITESER_H
#define LITESER_H

#include "liteserExport.h"

#define LS_SERIALIZE_BEGIN(file, superclass) \
	if (!superclass::serialize(file)) \
	{ \
		return false; \
	} \
	unsigned int _lsId;
#define LS_SERIALIZE_END \
	return true;

#define LS_DESERIALIZE_BEGIN(file, superclass) \
	if (!superclass::deserialize(file)) \
	{ \
		return false; \
	} \
	unsigned int _lsId;
#define LS_DESERIALIZE_END \
	return true;

/// @note "type" can be char, uchar, int, uint, long, ulong, short, ushort, bool, float, double or hstr
#define LS_SERIALIZE(file, name) file->dump(name);
/// @note "type" can be char, uchar, int, uint, long, ulong, short, ushort, bool, float, double or hstr
#define LS_DESERIALIZE(file, name, type) name = file->load_ ## type();

#define LS_SERIALIZE_OBJECT(file, object) object.serialize(file);
#define LS_DESERIALIZE_OBJECT(file, object) \
	_lsId = file->load_uint(); \
	ids[_lsId] = &object; \
	object.deserialize(file);

#define LS_SERIALIZE_OBJECT_PTR(file, object) \
	if (object != NULL) \
	{ \
		object->serialize(file); \
	} \
	else \
	{ \
		file->dump(0); \
	} 
#define LS_DESERIALIZE_OBJECT_PTR(file, classe, object) \
	_lsId = file->load_uint(); \
	if (_lsId != 0) \
	{ \
		if (ids.has_key(_lsId)) \
		{ \
			object = (classe*)ids[_lsId]; \
		} \
		else \
		{ \
			if (object == NULL) \
			{ \
				object = new classe(); \
			} \
			ids[_lsId] = object; \
			object->deserialize(file); \
		} \
	} \
	else if (object != NULL) \
	{ \
		delete object; \
		object = NULL; \
	}

namespace liteser
{
	class Serializable;

	liteserFnExport void serialize(hfile* file, Serializable* object);
	liteserFnExport void deserialize(hfile* file, Serializable* object);

}
#endif