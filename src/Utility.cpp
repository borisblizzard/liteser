/// @file
/// @version 2.7
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
	hstr _indent = "\t";
	harray<harray<Type::Value> > compatibleTypes;

	void _checkVersion(unsigned char major, unsigned char minor)
	{
		if (major != _LS_VERSION_MAJOR)
		{
			throw Exception(hsprintf("Liteser Read Error! Version mismatch: expected %d.%d, got %d.%d",
				_LS_VERSION_MAJOR, _LS_VERSION_MINOR, major, minor));
		}
		if (minor < _LS_VERSION_MINOR)
		{
			hlog::warnf(logTag, "Minor version mismatch while loading: expected %d.%d, got %d.%d",
				_LS_VERSION_MAJOR, _LS_VERSION_MINOR, major, minor);
		}
	}

}
