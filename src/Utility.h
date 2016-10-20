/// @file
/// @version 2.7
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines utility functions.

#ifndef LITESER_UTILITY_H
#define LITESER_UTILITY_H

#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hsbase.h>
#include <hltypes/hstring.h>

#include "Variable.h"
#include "Type.h"

namespace liteser
{
	class Serializable;

	extern harray<Serializable*> objects;
	extern harray<hstr> strings;
	extern hmap<Serializable*, unsigned int> objectIds;
	extern hmap<hstr, unsigned int> stringIds;
	extern hsbase* stream;
	extern hstr _indent;
	extern harray<harray<Type::Value> > compatibleTypes;

	void _checkVersion(unsigned char major, unsigned char minor);

	inline bool __tryGetObject(unsigned int id, Serializable** object)
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

	inline bool __tryMapObject(unsigned int* id, Serializable* object)
	{
		if (object == NULL)
		{
			*id = 0;
			return false;
		}
		int index = objectIds.tryGet(object, -1);
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

	inline bool __tryGetString(unsigned int id, hstr* string)
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
	
	inline bool __tryMapString(unsigned int* id, chstr string)
	{
		if (string == "")
		{
			*id = 0;
			return false;
		}
		int index = stringIds.tryGet(string, -1);
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

	inline void __forceMapEmptyObject()
	{
		objects += NULL;
	}

	inline void _start(hsbase* stream)
	{
		objects.clear();
		strings.clear();
		objectIds.clear();
		stringIds.clear();
		liteser::stream = stream;
		_indent = "\t";
	}

	inline void _finish(hsbase* stream)
	{
		objects.clear();
		strings.clear();
		objectIds.clear();
		stringIds.clear();
		liteser::stream = NULL;
		_indent = "\t";
	}

	inline bool _isActive()
	{
		return (stream != NULL);
	}

	inline void __setupCompatibleTypes()
	{
		harray<Type::Value> values;
		values += Type::INT8;
		values += Type::UINT8;
		values += Type::INT16;
		values += Type::UINT16;
		values += Type::INT32;
		values += Type::UINT32;
		values += Type::INT64;
		values += Type::UINT64;
		compatibleTypes += values;
		values.clear();
		values += Type::FLOAT;
		values += Type::DOUBLE;
		compatibleTypes += values;
	}

	inline bool _isCompatibleType(const Type::Value& variableType, const Type::Value& loadedType)
	{
		if (compatibleTypes.size() == 0)
		{
			__setupCompatibleTypes();
		}
		foreach (harray<Type::Value>, it, compatibleTypes)
		{
			if ((*it).has(variableType) && (*it).has(loadedType))
			{
				return true;
			}
		}
		return false;
	}

}
#endif
