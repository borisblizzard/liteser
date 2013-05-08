/// @file
/// @author  Boris Mikic
/// @version 2.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hlog.h>
#include <hltypes/hmap.h>
#include <hltypes/hsbase.h>

#include "liteser.h"
#include "Serializable.h"
#include "Utility.h"

namespace liteser
{
	hmap<unsigned int, Serializable*> objects;
	hmap<unsigned int, hstr> strings;
	hsbase* stream = NULL;

	bool __tryMapObject(unsigned int* id, Serializable* object)
	{
		if (object == NULL)
		{
			*id = 0;
			return false;
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

	bool __tryGetObject(unsigned int id, Serializable** object)
	{
		if (id == 0)
		{
			*object = NULL;
			return true;
		}
		if (!objects.has_key(id))
		{
			return false;
		}
		*object = objects[id];
		return true;
	}

	bool __tryMapString(unsigned int* id, chstr string)
	{
		if (string == "")
		{
			*id = 0;
			return false;
		}
		if (!strings.has_value(string))
		{
			// necessary to avoid incorrect size() since objects[*id] could be evaluated first
			*id = strings.size() + 1;
			strings[*id] = string;
			return true;
		}
		*id = strings(string);
		return false;
	}

	bool __tryGetString(unsigned int id, hstr* string)
	{
		if (id == 0)
		{
			*string = "";
			return true;
		}
		if (!strings.has_key(id))
		{
			return false;
		}
		*string = strings[id];
		return true;
	}

	void _start(hsbase* stream)
	{
		objects.clear();
		strings.clear();
		liteser::stream = stream;
	}

	void _finish(hsbase* stream)
	{
		objects.clear();
		strings.clear();
		liteser::stream = NULL;
	}

	bool _isActive()
	{
		return (stream != NULL);
	}

	void _checkVersion(unsigned char major, unsigned char minor)
	{
		if (major != _LS_VERSION_MAJOR)
		{
			throw hl_exception(hsprintf("Liteser Read Error! Version mismatch: expected %d.%d, got %d.%d",
				_LS_VERSION_MAJOR, _LS_VERSION_MINOR, major, minor));
		}
		if (minor > _LS_VERSION_MINOR)
		{
			hlog::warnf(liteser::logTag, "Minor version mismatch while loading: expected %d.%d, got %d.%d",
				_LS_VERSION_MAJOR, _LS_VERSION_MINOR, major, minor);
		}
	}

}
