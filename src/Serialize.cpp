/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <stdint.h>

#include <hltypes/harray.h>
#include <hltypes/hdeque.h>
#include <hltypes/hlist.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "liteser.h"
#include "Serializable.h"
#include "Serialize.h"
#include "Utility.h"
#include "Variable.h"

namespace liteser
{
	void __dumpVariable(Variable* variable)
	{
		_dump(&variable->name);
		unsigned char type = (unsigned char)variable->type;
		_dump(&type);
		switch (variable->type)
		{
		case Variable::TYPE_INT8:	_dump(variable->value<char>());				break;
		case Variable::TYPE_UINT8:	_dump(variable->value<unsigned char>());		break;
		case Variable::TYPE_INT16:	_dump(variable->value<int16_t>());			break;
		case Variable::TYPE_UINT16:	_dump(variable->value<uint16_t>());			break;
		case Variable::TYPE_INT32:	_dump(variable->value<int32_t>());			break;
		case Variable::TYPE_UINT32:	_dump(variable->value<uint32_t>());			break;
		case Variable::TYPE_FLOAT:	_dump(variable->value<float>());				break;
		case Variable::TYPE_DOUBLE:	_dump(variable->value<double>());			break;
		case Variable::TYPE_BOOL:	_dump(variable->value<bool>());				break;
		case Variable::TYPE_HSTR:	_dump(variable->value<hstr>());				break;
		case Variable::TYPE_OBJECT:	_dump(variable->value<Serializable>());		break;
		case Variable::TYPE_OBJPTR:	_dump(variable->value<Serializable*>());		break;
		/*
		case Variable::TYPE_HDEQUE:	_dump(*(hdeque*)(variable->value));			break;
		case Variable::TYPE_HARRAY:	_dump(*(harray*)(variable->value));			break;
		case Variable::TYPE_HLIST:	_dump(*(hlist*)(variable->value));			break;
		case Variable::TYPE_HMAP:	_dump(*(hmap*)(variable->value));			break;
		*/
		}
	}

	void _dump(char* value)
	{
		stream->dump(*value);
	}

	void _dump(unsigned char* value)
	{
		stream->dump(*value);
	}

	void _dump(int16_t* value)
	{
		stream->dump(*value);
	}

	void _dump(uint16_t* value)
	{
		stream->dump(*value);
	}

	void _dump(int32_t* value)
	{
		stream->dump(*value);
	}

	void _dump(uint32_t* value)
	{
		stream->dump(*value);
	}

	void _dump(float* value)
	{
		stream->dump(*value);
	}

	void _dump(double* value)
	{
		stream->dump(*value);
	}

	void _dump(bool* value)
	{
		stream->dump(*value);
	}

	void _dump(hstr* value)
	{
		// TODO - implement own way of saving stuff
		// TODO - index variable names and strings
		stream->dump(*value);
	}

	void _dump(Serializable* value)
	{
		uint32_t id;
		bool added = __tryMapObject(&id, value);
		_dump(&id);
		if (added)
		{
			_dump(&value->_lsName());
			harray<Variable*> variables = value->_lsVars();
			uint32_t size = (uint32_t)variables.size();
			_dump(&size);
			foreach (Variable*, it, variables)
			{
				__dumpVariable(*it);
				delete (*it);
			}
		}
	}

	void _dump(Serializable** value)
	{
		_dump(*value);
	}

}
