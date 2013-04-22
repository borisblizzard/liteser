/// @file
/// @author  Boris Mikic
/// @version 2.01
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines all serialization functions.

#ifndef LITESER_SERIALIZE_H
#define LITESER_SERIALIZE_H

#include <stdint.h>

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hsbase.h>
#include <hltypes/hstring.h>

#include "Type.h"

namespace liteser
{
	class Serializable;
	class Variable;


	void __dumpContainer(Variable* variable);

	void _dump(hstr* value);
	void _dump(gvec2* value);
	void _dump(grect* value);
	void _dump(Serializable* value);
	void _dump(Serializable** value);

	void _dumpHarray(harray<char>* value);
	void _dumpHarray(harray<unsigned char>* value);
	void _dumpHarray(harray<int16_t>* value);
	void _dumpHarray(harray<uint16_t>* value);
	void _dumpHarray(harray<int32_t>* value);
	void _dumpHarray(harray<uint32_t>* value);
	void _dumpHarray(harray<float>* value);
	void _dumpHarray(harray<double>* value);
	void _dumpHarray(harray<hstr>* value);
	void _dumpHarray(harray<gvec2>* value);
	void _dumpHarray(harray<grect>* value);
	void _dumpHarray(harray<Serializable*>* value);

}
#endif
