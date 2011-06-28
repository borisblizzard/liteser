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

#define LS_
#define LS_MAKE_SERIALIZABLE : public liteser::Serializable

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

#define LS_SERIALIZE_UCHAR(file, name) file->dump(name);
#define LS_DESERIALIZE_UCHAR(file, name) name = file->load_uchar();
#define LS_SERIALIZE_INT(file, name) file->dump(name);
#define LS_DESERIALIZE_INT(file, name) name = file->load_int();
#define LS_SERIALIZE_UINT(file, name) file->dump(name);
#define LS_DESERIALIZE_UINT(file, name) name = file->load_uint();
#define LS_SERIALIZE_LONG(file, name) file->dump(name);
#define LS_DESERIALIZE_LONG(file, name) name = file->load_long();
#define LS_SERIALIZE_ULONG(file, name) file->dump(name);
#define LS_DESERIALIZE_ULONG(file, name) name = file->load_ulong();
#define LS_SERIALIZE_SHORT(file, name) file->dump(name);
#define LS_DESERIALIZE_SHORT(file, name) name = file->load_short();
#define LS_SERIALIZE_USHORT(file, name) file->dump(name);
#define LS_DESERIALIZE_USHORT(file, name) name = file->load_ushort();
#define LS_SERIALIZE_BOOL(file, name) file->dump(name);
#define LS_DESERIALIZE_BOOL(file, name) name = file->load_bool();
#define LS_SERIALIZE_FLOAT(file, name) file->dump(name);
#define LS_DESERIALIZE_FLOAT(file, name) name = file->load_float();
#define LS_SERIALIZE_DOUBLE(file, name) file->dump(name);
#define LS_DESERIALIZE_DOUBLE(file, name) name = file->load_double();

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

	void serialize(hfile* file, Serializable* object);
	void deserialize(hfile* file, Serializable* object);

}
#endif
