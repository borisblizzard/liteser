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

#define VERSION_MAJOR 1
#define VERSION_MINOR 0

namespace liteser
{
	hmap<unsigned int, Serializable*> ids;

	void serialize(hfile* file, Serializable* object)
	{
		ids.clear();
		file->dump((unsigned char)VERSION_MAJOR);
		file->dump((unsigned char)VERSION_MINOR);
		object->serialize(file);
	}
	
	void deserialize(hfile* file, Serializable* object)
	{
		ids.clear();
		unsigned char major = file->load_uchar();
		unsigned char minor = file->load_uchar();
		// TODO - check version here
		object->deserialize(file);
	}
	
}
