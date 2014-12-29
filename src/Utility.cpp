/// @file
/// @version 2.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/harray.h>
#include <hltypes/hlog.h>
#include <hltypes/hmap.h>
#include <hltypes/hsbase.h>

#include "liteser.h"
#include "Serializable.h"
#include "Utility.h"

namespace liteser
{
	// this hybrid implementation for indexing gave the best results in benchmarking with larger files
	harray<Serializable*> objects;
	harray<hstr> strings;
	hmap<Serializable*, unsigned int> objectIds;
	hmap<hstr, unsigned int> stringIds;
	hsbase* stream = NULL;

	bool __tryGetObject(unsigned int id, Serializable** object)
	{
		if (id == 0)
		{
			*object = NULL;
			return true;
		}
		if ((int)id - 1 >= objects.size())
		{
			return false;
		}
		*object = objects[id - 1];
		return true;
	}

	bool __tryMapObject(unsigned int* id, Serializable* object)
	{
		if (object == NULL)
		{
			*id = 0;
			return false;
		}
		int index = objectIds.try_get_by_key(object, -1);
		if (index < 0)
		{
			objects += object;
			*id = objects.size();
			objectIds[object] = *id - 1;
			return true;
		}
		*id = index + 1;
		return false;
	}

	bool __tryGetString(unsigned int id, hstr* string)
	{
		if (id == 0)
		{
			*string = "";
			return true;
		}
		if ((int)id - 1 >= strings.size())
		{
			return false;
		}
		*string = strings[id - 1];
		return true;
	}
	
	bool __tryMapString(unsigned int* id, chstr string)
	{
		if (string == "")
		{
			*id = 0;
			return false;
		}
		int index = stringIds.try_get_by_key(string, -1);
		if (index < 0)
		{
			strings += string;
			*id = strings.size();
			stringIds[string] = *id - 1;
			return true;
		}
		*id = index + 1;
		return false;
	}

	void __forceMapEmptyObject()
	{
		objects += NULL;
	}

	void _start(hsbase* stream)
	{
		objects.clear();
		strings.clear();
		objectIds.clear();
		stringIds.clear();
		liteser::stream = stream;
	}

	void _finish(hsbase* stream)
	{
		objects.clear();
		strings.clear();
		objectIds.clear();
		stringIds.clear();
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
			throw Exception(hsprintf("Liteser Read Error! Version mismatch: expected %d.%d, got %d.%d",
				_LS_VERSION_MAJOR, _LS_VERSION_MINOR, major, minor));
		}
		if (minor < _LS_VERSION_MINOR)
		{
			hlog::warnf(liteser::logTag, "Minor version mismatch while loading: expected %d.%d, got %d.%d",
				_LS_VERSION_MAJOR, _LS_VERSION_MINOR, major, minor);
		}
	}

}
