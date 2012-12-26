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

#include "liteser.h"
#include "Serializable.h"

namespace liteser
{
	hmap<unsigned int, Serializable*> _lsIds;

	void serialize(hsbase* stream, Serializable* object)
	{
		_lsIds.clear();
		stream->dump((unsigned char)_LS_VERSION_MAJOR);
		stream->dump((unsigned char)_LS_VERSION_MINOR);
		object->serialize(stream);
	}
	
	void deserialize(hsbase* stream, Serializable** object)
	{
		_lsIds.clear();
		unsigned char major = stream->load_uchar();
		unsigned char minor = stream->load_uchar();
		checkVersion(major, minor);
		int _lsId = stream->load_uint();
		_lsIds[_lsId] = (*object);
		(*object)->deserialize(stream);
	}

	void checkVersion(unsigned char major, unsigned char minor)
	{
		if (major != _LS_VERSION_MAJOR || minor != _LS_VERSION_MINOR)
		{
			throw hl_exception(hsprintf("Liteser Read Error! Version mismatch: expected %d.%d, got %d.%d",
				_LS_VERSION_MAJOR, _LS_VERSION_MINOR, major, minor));
		}
	}
	
}
