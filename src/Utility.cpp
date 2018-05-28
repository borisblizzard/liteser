/// @file
/// @version 3.1
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
	unsigned char fileDescriptor[4] = { 'L', 'S', 'B', 'D' }; // Lite Serializer Binary Data
	harray<harray<Type::Identifier> > compatibleIdentifiers;
	hsbase* stream = NULL;
	Header _currentHeader;

	// this hybrid implementation for indexing gave the best results in benchmarking with larger files
	harray<Serializable*> objects;
	harray<hstr> strings;
	hmap<Serializable*, unsigned int> objectIds;
	hmap<hstr, unsigned int> stringIds;
	hstr _indent = "\t";

	void _checkVersion()
	{
		if (_currentHeader.version.major >= 3)
		{
			if (_currentHeader.version.major != liteser::version.major)
			{
				throw Exception(hsprintf("Liteser Read Error! Version mismatch: expected %d.%d, got %d.%d",
					liteser::version.major, liteser::version.minor, _currentHeader.version.major, _currentHeader.version.minor));
			}
			if (_currentHeader.version.minor < liteser::version.minor)
			{
				hlog::warnf(logTag, "Minor version mismatch while loading: expected %d.%d, got %d.%d",
					liteser::version.major, liteser::version.minor, _currentHeader.version.major, _currentHeader.version.minor);
			}
		}
		else
		{
			if (_currentHeader.version.major != 2)
			{
				throw Exception(hsprintf("Liteser Read Error! Version mismatch: expected 2.7, got %d.%d", _currentHeader.version.major, _currentHeader.version.minor));
			}
			if (_currentHeader.version.minor < 7)
			{
				hlog::warnf(logTag, "Minor version mismatch while loading: expected 2.7, got %d.%d", _currentHeader.version.major, _currentHeader.version.minor);
			}
		}
	}

}
