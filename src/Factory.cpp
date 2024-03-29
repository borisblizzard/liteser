/// @file
/// @version 3.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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

	Serializable* Factory::create(chstr name)
	{
		Serializable* (*constructor)() = __lsConstructors->tryGet(name, NULL);
		if (constructor == NULL)
		{
			hstr error = "Detected class not registered as a Serializable: '" + name + "'";
			harray<hstr> names = __lsConstructors->keys();
			if (names.size() > 0)
			{
				error += "\nCurrently registered classes:\n- " + names.joined("\n- ");
			}
			throw Exception(error);
		}
		return (*constructor)();
	}

	void Factory::_register(chstr name, Serializable* (*constructor)())
	{
		if (__lsConstructors == NULL)
		{
			__lsConstructors = new hmap<hstr, Serializable* (*)()>();
		}
		// because of the order of global var initializations, mutexes get messed up
		printf("[liteser] Registering class: %s\n", name.cStr());
		__lsConstructors->operator[](name) = constructor;
	}
	
}
