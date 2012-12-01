/// @file
/// @author  Boris Mikic
/// @version 1.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents a base for a serializable object.

#ifndef LITESER_SERIALIZABLE_H
#define LITESER_SERIALIZABLE_H

#include <hltypes/hsbase.h>

#include "liteserExport.h"

namespace liteser
{
	class liteserExport Serializable
	{
	public:
		Serializable();
		virtual ~Serializable();

		virtual bool serialize(hsbase* stream);
		virtual bool deserialize(hsbase* stream);
		
	};

}
#endif
