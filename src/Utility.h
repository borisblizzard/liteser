/// @file
/// @version 3.1
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
#include <hlxml/Node.h>

#include "Header.h"
#include "Type.h"
#include "Variable.h"

namespace liteser
{
	class Serializable;

	extern unsigned char fileDescriptor[4]; // Lite Serializer Binary Data
	extern harray<harray<Type::Identifier> > compatibleIdentifiers;
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
			stream->loadUint32();
			// this is for version 3.0, header size should be sizeof(unsigned int) * 2 + 2 bools (no checks currently performed)
			header.version.set(0);
			header.version.major = stream->loadUint32();
			header.version.minor = stream->loadUint32();
			header.allowMultiReferencing = stream->loadBool();
			header.stringPooling = stream->loadBool();
		}
		else // backwards compatibility with 2.x versions
		{
			header.version.set(compatibilityVersionMajor, compatibilityVersionMinor);
			header.allowMultiReferencing = true;
			header.stringPooling = true;
		}
	}

	inline void _writeHeader(hsbase* stream, Header& header)
	{
		stream->writeRaw(fileDescriptor, sizeof(fileDescriptor));
		stream->dump((unsigned int)(sizeof(unsigned int) * 2 + 2)); // sizeof(unsigned int) * 2 + 2 bools
		stream->dump(header.version.major);
		stream->dump(header.version.minor);
		stream->dump(header.allowMultiReferencing);
		stream->dump(header.stringPooling);
	}

	inline void _readXmlHeader(hlxml::Node* root, Header& header)
	{
		if (root->name != "Liteser")
		{
			throw Exception("Invalid XML header!");
		}
		hstr versionString = root->pstr("version", "");
		if (versionString.count(".") != 1)
		{
			throw Exception("Invalid XML header!");
		}
		hstr majorString;
		hstr minorString;
		if (!versionString.split('.', majorString, minorString))
		{
			throw Exception("Invalid XML header!");
		}
		header.version.set((unsigned char)(int)majorString, (unsigned char)(int)minorString);
		header.allowMultiReferencing = true;
		header.stringPooling = false;
	}

	inline bool _isActive()
	{
		return (stream != NULL);
	}

	inline Type::Identifier _loadIdentifier()
	{
		return Type::Identifier::fromUint((unsigned int)stream->loadUint8());
	}

	inline void __setupCompatibleIdentifiers()
	{
		harray<Type::Identifier> identifiers;
		identifiers += Type::Identifier::Int8;
		identifiers += Type::Identifier::UInt8;
		identifiers += Type::Identifier::Int16;
		identifiers += Type::Identifier::UInt16;
		identifiers += Type::Identifier::Int32;
		identifiers += Type::Identifier::UInt32;
		identifiers += Type::Identifier::Int64;
		identifiers += Type::Identifier::UInt64;
		compatibleIdentifiers += identifiers;
		identifiers.clear();
		identifiers += Type::Identifier::Float;
		identifiers += Type::Identifier::Double;
		compatibleIdentifiers += identifiers;
		identifiers.clear();
		identifiers += Type::Identifier::Gvec2f;
		identifiers += Type::Identifier::Gvec2i;
		identifiers += Type::Identifier::Gvec2d;
		compatibleIdentifiers += identifiers;
		identifiers.clear();
		identifiers += Type::Identifier::Gvec3f;
		identifiers += Type::Identifier::Gvec3i;
		identifiers += Type::Identifier::Gvec3d;
		compatibleIdentifiers += identifiers;
		identifiers.clear();
		identifiers += Type::Identifier::Grectf;
		identifiers += Type::Identifier::Grecti;
		identifiers += Type::Identifier::Grectd;
		compatibleIdentifiers += identifiers;
	}

	inline bool _isCompatibleType(const Type::Identifier& variableIdentifier, const Type::Identifier& loadedIdentifier)
	{
		if (compatibleIdentifiers.size() == 0)
		{
			__setupCompatibleIdentifiers();
		}
		foreach (harray<Type::Identifier>, it, compatibleIdentifiers)
		{
			if ((*it).has(variableIdentifier) && (*it).has(loadedIdentifier))
			{
				return true;
			}
		}
		return false;
	}

}
#endif
