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

namespace liteser
{
	Serializable::Serializable()
	{
	}

	Serializable::~Serializable()
	{
	}

	bool Serializable::serialize(hfile* file)
	{
		bool result = false;
		unsigned int id = liteser::_lsIds.size() + 1; // necessary to avoid incorrect size() since ids[this] could be evaluated first
		if (!liteser::_lsIds.has_value(this))
		{
			liteser::_lsIds[id] = this;
			result = true;
		}
		else
		{
			id = liteser::_lsIds(this);
		}
		file->dump(id);
		return result;
	}

	bool Serializable::deserialize(hfile* file)
	{
		return true;
	}
		
}
