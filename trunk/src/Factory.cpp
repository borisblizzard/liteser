/// @file
/// @author  Boris Mikic
/// @version 2.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hmap.h>
#include <hltypes/hstring.h>
#include <hltypes/hlog.h>

#include "Factory.h"
#include "Serializable.h"

namespace liteser
{
	// this approach is used because there is a massive problem with static linking
	static hmap<hstr, Serializable* (*)()>* __lsConstructors = NULL;

	Factory::Factory()
	{
	}

	Factory::~Factory()
	{
	}

	Serializable* Factory::create(chstr name)
	{
		Serializable* (*constructor)() = __lsConstructors->try_get_by_key(name, NULL);
		if (constructor == NULL)
		{
			throw hl_exception("Detected class not registered as a Serializable: " + name);
		}
		return (*constructor)();
	}

	void Factory::_register(chstr name, Serializable* (*constructor)())
	{
		if (__lsConstructors == NULL)
		{
			__lsConstructors = new hmap<hstr, Serializable* (*)()>();
		}
		hlog::debug(liteser::logTag, "Registering class: " + name);
		__lsConstructors->operator[](name) = constructor;
	}
	
}
