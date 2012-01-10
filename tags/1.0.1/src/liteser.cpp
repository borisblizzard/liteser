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

namespace liteser
{
	hmap<unsigned int, Serializable*> _lsIds;
	
	void serialize(hfile* file, Serializable* object)
	{
		_lsIds.clear();
		file->dump((unsigned char)_LS_VERSION_MAJOR);
		file->dump((unsigned char)_LS_VERSION_MINOR);
		object->serialize(file);
	}
	
	void deserialize(hfile* file, Serializable* object)
	{
		_lsIds.clear();
		unsigned char major = file->load_uchar();
		unsigned char minor = file->load_uchar();
		checkVersion(major, minor);
		int _lsId = file->load_uint();
		_lsIds[_lsId] = object;
		object->deserialize(file);
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
