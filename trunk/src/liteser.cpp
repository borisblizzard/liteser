/// @file
/// @author  Boris Mikic
/// @version 2.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/exception.h>
#include <hltypes/hmap.h>
#include <hltypes/hsbase.h>
#include <hltypes/hstream.h>
#include <hltypes/hstring.h>

#include "Deserialize.h"
#include "liteser.h"
#include "Serializable.h"
#include "Serialize.h"
#include "Utility.h"
#include "Variable.h"

#define _LS_HEADER_0 'L'
#define _LS_HEADER_1 'S'

#define DECLARE_HARRAY_SERIALIZER(type) \
	bool serialize(hsbase* stream, harray<type> value) \
	{ \
		if (!stream->is_open()) \
		{ \
			throw file_not_open("Liteser Stream"); \
		} \
		_start(stream); \
		stream->write_raw(header, 4); \
		_dumpHarray(&value); \
		_finish(stream); \
		return true; \
	}
	
#define DECLARE_HARRAY_DESERIALIZER(type) \
	bool deserialize(hsbase* stream, harray<type>* value) \
	{ \
		if (!stream->is_open()) \
		{ \
			throw file_not_open("Liteser Stream"); \
		} \
		if (value->size() > 0) \
		{ \
			throw hl_exception("Output harray is not empty!"); \
		} \
		_start(stream); \
		unsigned char readHeader[4]; \
		stream->read_raw(readHeader, 4); \
		if (readHeader[0] != _LS_HEADER_0 || readHeader[1] != _LS_HEADER_1) \
		{ \
			throw hl_exception("Invalid header!"); \
		} \
		unsigned char major = readHeader[2]; \
		unsigned char minor = readHeader[3]; \
		_checkVersion(major, minor); \
		_loadHarray(value); \
		_finish(stream); \
		return true; \
	}

namespace liteser
{
	char header[4] = {_LS_HEADER_0, _LS_HEADER_1, (char)_LS_VERSION_MAJOR, (char)_LS_VERSION_MINOR};

	hstr logTag = "liteser";

	bool serialize(hsbase* stream, Serializable* object)
	{
		if (!stream->is_open())
		{
			throw file_not_open("Liteser Stream");
		}
		// TODO - add exception handling
		_start(stream);
		stream->write_raw(header, 4);
		_dump(&object);
		_finish(stream);
		return true;
	}

	DECLARE_HARRAY_SERIALIZER(Serializable*);
	DECLARE_HARRAY_SERIALIZER(char);
	DECLARE_HARRAY_SERIALIZER(unsigned char);
	DECLARE_HARRAY_SERIALIZER(int16_t);
	DECLARE_HARRAY_SERIALIZER(uint16_t);
	DECLARE_HARRAY_SERIALIZER(int32_t);
	DECLARE_HARRAY_SERIALIZER(uint32_t);
	DECLARE_HARRAY_SERIALIZER(float);
	DECLARE_HARRAY_SERIALIZER(double);
	DECLARE_HARRAY_SERIALIZER(hstr);
	DECLARE_HARRAY_SERIALIZER(grect);
	DECLARE_HARRAY_SERIALIZER(gvec2);
	DECLARE_HARRAY_SERIALIZER(gvec3);
	
	bool deserialize(hsbase* stream, Serializable** object)
	{
		if (!stream->is_open())
		{
			throw file_not_open("Liteser Stream");
		}
		if (*object != NULL)
		{
			throw hl_exception("Given pointer to object for deserialization is not NULL.");
		}
		// TODO - add exception handling
		_start(stream);
		unsigned char readHeader[4];
		stream->read_raw(readHeader, 4);
		if (readHeader[0] != _LS_HEADER_0 || readHeader[1] != _LS_HEADER_1)
		{
			throw hl_exception("Invalid header.");
		}
		unsigned char major = readHeader[2];
		unsigned char minor = readHeader[3];
		_checkVersion(major, minor);
		_load(object);
		_finish(stream);
		return true;
	}

	DECLARE_HARRAY_DESERIALIZER(Serializable*);
	DECLARE_HARRAY_DESERIALIZER(char);
	DECLARE_HARRAY_DESERIALIZER(unsigned char);
	DECLARE_HARRAY_DESERIALIZER(int16_t);
	DECLARE_HARRAY_DESERIALIZER(uint16_t);
	DECLARE_HARRAY_DESERIALIZER(int32_t);
	DECLARE_HARRAY_DESERIALIZER(uint32_t);
	DECLARE_HARRAY_DESERIALIZER(float);
	DECLARE_HARRAY_DESERIALIZER(double);
	DECLARE_HARRAY_DESERIALIZER(hstr);
	DECLARE_HARRAY_DESERIALIZER(grect);
	DECLARE_HARRAY_DESERIALIZER(gvec2);
	DECLARE_HARRAY_DESERIALIZER(gvec3);
	
	bool clone(Serializable* input, Serializable** output)
	{
		hstream stream;
		_start(&stream);
		_dump(&input);
		_finish(&stream);
		stream.rewind();
		_start(&stream);
		_load(output);
		_finish(&stream);
		return true;
	}

}
