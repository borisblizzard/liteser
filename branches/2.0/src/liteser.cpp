/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/exception.h>
#include <hltypes/hmap.h>
#include <hltypes/hsbase.h>
#include <hltypes/hstring.h>

#include "Deserialize.h"
#include "liteser.h"
#include "Serializable.h"
#include "Serialize.h"
#include "Utility.h"
#include "Variable.h"

#define _LS_HEADER_0 'L'
#define _LS_HEADER_1 'S'

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

}
