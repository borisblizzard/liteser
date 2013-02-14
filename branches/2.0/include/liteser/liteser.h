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
/// Defines global serialization and deserialization procedures as well as serialization macros.

#ifndef LITESER_H
#define LITESER_H

#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hmap.h>
#include <hltypes/hsbase.h>
#include <hltypes/hstring.h>

#include "liteserExport.h"
#include "macros.h"
#include "Serializable.h"

#define _LS_VERSION_MAJOR 2
#define _LS_VERSION_MINOR 0

namespace liteser
{
	extern hstr logTag;

	liteserFnExport bool serialize(hsbase* stream, Serializable* object);
	liteserFnExport bool serialize(hsbase* stream, harray<Serializable*> object);
	liteserFnExport bool deserialize(hsbase* stream, Serializable** object);
	liteserFnExport bool deserialize(hsbase* stream, harray<Serializable*>* object);

}
#endif
