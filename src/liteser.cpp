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

#include "Deserialize.h"
#include "liteser.h"
#include "Serializable.h"
#include "Serialize.h"
#include "Utility.h"
#include "Variable.h"

namespace liteser
{
	char header[4] = {'L', 'S', (char)_LS_VERSION_MAJOR, (char)_LS_VERSION_MINOR};

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
		// TODO - add exception handling
		_start(stream);
		unsigned char readHeader[4];
		stream->read_raw(readHeader, 4);
		unsigned char major = readHeader[2];
		unsigned char minor = readHeader[3];
		_checkVersion(major, minor);
		/*
		int id = stream->load_uint();
		ids[id] = (*object);
		*/
		//(*object)->deserialize(stream);
		_finish(stream);
		return true;
	}

}
