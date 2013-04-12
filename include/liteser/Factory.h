/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents a factory for derived object types.

#ifndef LITESER_FACTORY_H
#define LITESER_FACTORY_H

#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "liteserExport.h"

namespace liteser
{
	class Serializable;

	class liteserExport Factory
	{
	public:
		template <class T>
		struct Register
		{
			Register(chstr name)
			{
				Factory::constructors[name] = &_create<T>;
			}
		};

		template <class T>
		static Serializable* _create() { return new T(); }

		static Serializable* create(chstr name);

	protected:
		Factory();

		static hmap<hstr, Serializable* (*)()> constructors;

	};

}
#endif
