/// @file
/// @version 3.0
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

#include "Header.h"
#include "Type.h"
#include "Variable.h"

namespace liteser
{
	class Serializable;

	extern unsigned char fileDescriptor[4]; // Lite Serializer Binary Data
	extern harray<harray<Type::Value> > compatibleTypes;
	extern hsbase* stream;
	extern Header _currentHeader;
	extern harray<Serializable*> objects;
	extern harray<hstr> strings;
	extern hmap<Serializable*, unsigned int> objectIds;
	extern hmap<hstr, unsigned int> stringIds;
	extern hstr _indent;

	void _checkVersion();

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

	inline void _setup(hsbase* stream, const Header& header)
	{
		_currentHeader = header;
	}

	inline void _readHeader(hsbase* stream, Header& header)
	{
		unsigned char readFileDescriptor[sizeof(fileDescriptor)];
		stream->readRaw(readFileDescriptor, sizeof(fileDescriptor));
		if (readFileDescriptor[0] != fileDescriptor[0] || readFileDescriptor[1] != fileDescriptor[1])
		{
			throw Exception("Invalid file descriptor!");
		}
		unsigned char compatibilityVersionMajor = readFileDescriptor[2];
		unsigned char compatibilityVersionMinor = readFileDescriptor[3];
		if (compatibilityVersionMajor == fileDescriptor[2] && compatibilityVersionMinor == fileDescriptor[3])
		{
			unsigned int headerSize = stream->loadUint32();
			// this is for version 3.0, header size should be 4 * 4 + 1 (no checks currently performed)
			header.version.major = stream->loadUint32();
			header.version.minor = stream->loadUint32();
			header.version.revision = stream->loadUint32();
			header.version.build = stream->loadUint32();
			header.allowCircularReferences = stream->loadBool();
		}
		else // backwards compatibility with 2.x versions
		{
			header.version.set(compatibilityVersionMajor, compatibilityVersionMinor);
			header.allowCircularReferences = true;
		}
	}

	inline void _writeHeader(hsbase* stream, Header& header)
	{
		stream->writeRaw(fileDescriptor, sizeof(fileDescriptor));
		stream->dump(sizeof(unsigned int) * 4 + 1); // sizeof(unsigned int) * 4 + 1 bool
		stream->dump(header.version.major);
		stream->dump(header.version.minor);
		stream->dump(header.version.revision);
		stream->dump(header.version.build);
		stream->dump(header.allowCircularReferences);
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
