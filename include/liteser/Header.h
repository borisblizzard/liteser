/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Represents a header for the liteser file.

#ifndef LITESER_HEADER_H
#define LITESER_HEADER_H

#include <hltypes/hversion.h>

#include "liteserExport.h"

namespace liteser
{
	class liteserExport Header
	{
	public:
		hversion version;
		bool allowCircularReferences;

		Header(bool allowCircularReferences = true);
		~Header();

	};

}
#endif
