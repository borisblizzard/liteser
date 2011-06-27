/// @file
/// @author  Boris Mikic
/// @version 1.0
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

#include "liteserExport.h"

namespace liteser
{
	class liteserExport Serializable
	{
	public:
		Serializable();
		virtual ~Serializable();

		virtual void serialize(hfile* file);
		virtual void deserialize(hfile* file);
		
	};

}
#endif
