/// @file
/// @author  Boris Mikic
/// @version 1.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/exception.h>
#include <hltypes/hfile.h>
#include <hltypes/hmap.h>

#include "liteser.h"
#include "Serializable.h"

#define VERSION_MAJOR 1
#define VERSION_MINOR 0

namespace liteser
{
	hmap<unsigned int, Serializable*> _lsIds;
	
	void serialize(hfile* file, Serializable* object)
	{
		_lsIds.clear();
		file->dump((unsigned char)VERSION_MAJOR);
		file->dump((unsigned char)VERSION_MINOR);
		object->serialize(file);
	}
	
	void deserialize(hfile* file, Serializable* object)
	{
		_lsIds.clear();
		unsigned char major = file->load_uchar();
		unsigned char minor = file->load_uchar();
		checkVersion(major, minor);
		object->deserialize(file);
	}

	void checkVersion(unsigned char major, unsigned char minor)
	{
		if (major != VERSION_MAJOR || minor != VERSION_MINOR)
		{
			throw hl_exception(hsprintf("Liteser Read Error! Version mismatch: expected %d.%d, got %d.%d",
				VERSION_MAJOR, VERSION_MINOR, major, minor));
		}
	}
	
}
