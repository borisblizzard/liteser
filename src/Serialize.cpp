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
#include "Type.h"
#include "Utility.h"
#include "Variable.h"

namespace liteser
{
	void __dumpVariable(Variable* variable)
	{
		switch (variable->type->value)
		{
		case Type::INT8:	_dump(variable->value<char>());				break;
		case Type::UINT8:	_dump(variable->value<unsigned char>());	break;
		case Type::INT16:	_dump(variable->value<int16_t>());			break;
		case Type::UINT16:	_dump(variable->value<uint16_t>());			break;
		case Type::INT32:	_dump(variable->value<int32_t>());			break;
		case Type::UINT32:	_dump(variable->value<uint32_t>());			break;
		case Type::FLOAT:	_dump(variable->value<float>());			break;
		case Type::DOUBLE:	_dump(variable->value<double>());			break;
		case Type::BOOL:	_dump(variable->value<bool>());				break;
		case Type::HSTR:	_dump(variable->value<hstr>());				break;
		case Type::OBJECT:	_dump(variable->value<Serializable>());		break;
		case Type::OBJPTR:	_dump(variable->value<Serializable*>());	break;
		case Type::HARRAY:	__dumpHarray(variable);						break;
		/*
		case Type::HDEQUE:	_dump(*(hdeque*)(variable->value));			break;
		case Type::HLIST:	_dump(*(hlist*)(variable->value));			break;
		case Type::HMAP:	_dump(*(hmap*)(variable->value));			break;
		*/
		}
	}

	void __dumpVariableData(Variable* variable)
	{
		_dump(&variable->name);
		_dump((unsigned char*)(&variable->type));
	}

	void __dumpHarray(Variable* variable)
	{
		uint32_t size = (uint32_t)variable->subVariables.size();
		_dump(&size);
		if (size > 0)
		{
			_dump((unsigned char*)(&variable->subVariables.first()->type));
			foreach (Variable*, it, variable->subVariables)
			{
				__dumpVariable(*it);
			}
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
				__dumpVariableData(*it);
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
