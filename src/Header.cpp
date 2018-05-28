/// @file
/// @version 3.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "liteser.h"
#include "Header.h"

namespace liteser
{
	Header::Header()
	{
		this->version = liteser::version;
		this->allowMultiReferencing = true;
		this->stringPooling = true;
	}

	Header::Header(bool allowMultiReferencing, bool stringPooling)
	{
		this->version = liteser::version;
		this->allowMultiReferencing = allowMultiReferencing;
		this->stringPooling = stringPooling;
	}

	Header::~Header()
	{
	}
	
}
