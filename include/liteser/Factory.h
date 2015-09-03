/// @file
/// @version 2.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Represents a factory for derived object types.

#ifndef LITESER_FACTORY_H
#define LITESER_FACTORY_H

#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "liteserExport.h"

namespace liteser
{
	class Serializable;

	class liteserExport Factory
	{
	public:
		template <typename T>
		struct Register
		{
			inline Register(chstr name)
			{
				Factory::_register(name, &_create<T>);
			}
			inline ~Register()
			{
			}
		};

		~Factory();

		template <typename T>
		static inline Serializable* _create() { return new T(); }

		static Serializable* create(chstr name);

	protected:
		Factory();

		static void _register(chstr name, Serializable* (*constructor)());

	};

}
#endif
