/// @file
/// @author  Boris Mikic
/// @version 1.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hfile.h>
#include <hltypes/hmap.h>

#include "liteser.h"
#include "Serializable.h"

#define VERSION_MAJOR "1"
#define VERSION_MINOR "0"

namespace liteser
{
	hmap<unsigned int, Serializable*> ids;

	void serialize(hfile* file, Serializable* object)
	{
		ids.clear();
		file->dump(VERSION_MAJOR);
		file->dump(VERSION_MINOR);
		LS_SERIALIZE_OBJECT_PTR(file, object);
	}
	
	void deserialize(hfile* file, Serializable* object)
	{
		ids.clear();
		char major = file->load_uchar();
		char minor = file->load_uchar();
		// TODO - check version
	}
	
}
