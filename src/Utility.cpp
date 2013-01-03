/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hmap.h>
#include <hltypes/hsbase.h>

#include "liteser.h"
#include "Serializable.h"
#include "Utility.h"

namespace liteser
{
	hmap<unsigned int, Serializable*> objects;
	hsbase* stream = NULL;

	bool __tryMapObject(unsigned int* id, Serializable* object)
	{
		if (object == NULL)
		{
			*id = 0;
			return true;
		}
		if (!objects.has_value(object))
		{
			// necessary to avoid incorrect size() since objects[*id] could be evaluated first
			*id = objects.size() + 1;
			objects[*id] = object;
			return true;
		}
		*id = objects(object);
		return false;
	}

	Serializable* __getObject(unsigned int id)
	{
		return objects.try_get_by_key(id, NULL);
	}

	void _start(hsbase* stream)
	{
		objects.clear();
		liteser::stream = stream;
	}

	void _finish(hsbase* stream)
	{
		objects.clear();
		liteser::stream = NULL;
	}

	bool _isActive()
	{
		return (stream != NULL);
	}

	void _checkVersion(unsigned char major, unsigned char minor)
	{
		if (major != _LS_VERSION_MAJOR || minor != _LS_VERSION_MINOR)
		{
			throw hl_exception(hsprintf("Liteser Read Error! Version mismatch: expected %d.%d, got %d.%d",
				_LS_VERSION_MAJOR, _LS_VERSION_MINOR, major, minor));
		}
	}

}
