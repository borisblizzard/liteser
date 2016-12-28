/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "liteser.h"
#include "Header.h"

namespace liteser
{
	Header::Header(bool allowCircularReferences)
	{
		this->version = liteser::version;
		this->allowCircularReferences = allowCircularReferences;
	}

	Header::~Header()
	{
	}
	
}
