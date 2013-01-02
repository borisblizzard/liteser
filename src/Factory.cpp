/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "Factory.h"
#include "Serializable.h"

namespace liteser
{
	hmap<hstr, Serializable* (*)()> Factory::constructors;

	Factory::Factory()
	{
	}

	Serializable* Factory::create(chstr name)
	{
		Serializable* (*constructor)() = constructors.try_get_by_key(name, NULL);
		if (constructor == NULL)
		{
			throw hl_exception("Detected class not registered as a Serializable: " + name);
		}
		return (*constructor)();
	}
	
}
