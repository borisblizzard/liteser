/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <stdint.h>

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hsbase.h>
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
		case Type::GVEC2:	_dump(variable->value<gvec2>());			break;
		case Type::GRECT:	_dump(variable->value<grect>());			break;
		case Type::OBJECT:	_dump(variable->value<Serializable>());		break;
		case Type::OBJPTR:	_dump(variable->value<Serializable*>());	break;
		case Type::HARRAY:	__dumpContainer(variable);					break;
		case Type::HMAP:	__dumpContainer(variable);					break;
		}
	}

	void __dumpVariableData(Variable* variable)
	{
		_dump(&variable->name);
		_dump((unsigned char*)&variable->type->value);
	}

	void __dumpContainer(Variable* variable)
	{
		_dump(&variable->containerSize);
		if (variable->containerSize > 0)
		{
			uint32_t size = (uint32_t)variable->type->subTypes.size();
			_dump(&size);
			foreach (Type*, it, variable->type->subTypes)
			{
				_dump((unsigned char*)(&(*it)->value));
			}
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
		uint32_t id;
		if (__tryMapString(&id, *value))
		{
			_dump(&id);
			stream->dump(*value);
		}
		else
		{
			_dump(&id);
		}
	}

	void _dump(gvec2* value)
	{
		stream->dump(value->x);
		stream->dump(value->y);
	}

	void _dump(grect* value)
	{
		stream->dump(value->x);
		stream->dump(value->y);
		stream->dump(value->w);
		stream->dump(value->h);
	}

	void _dump(Serializable* value)
	{
		uint32_t id;
		if (__tryMapObject(&id, value))
		{
			_dump(&id);
			hstr name = value->_lsName();
			_dump(&name);
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
		else
		{
			_dump(&id);
		}
	}

	void _dump(Serializable** value)
	{
		_dump(*value);
	}

	void _dumpHarray(harray<Serializable*>* value)
	{
		uint32_t size = (uint32_t)value->size();
		_dump(&size);
		foreach (Serializable*, it, *value)
		{
			_dump(*it);
		}
	}

#define DEFINE_DUMP_HARRAY(type) \
	void _dumpHarray(harray<type>* value) \
	{ \
		uint32_t size = (uint32_t)value->size(); \
		_dump(&size); \
		foreach (type, it, *value) \
		{ \
			_dump(&(*it)); \
		} \
	}

	DEFINE_DUMP_HARRAY(char);
	DEFINE_DUMP_HARRAY(unsigned char);
	DEFINE_DUMP_HARRAY(int16_t);
	DEFINE_DUMP_HARRAY(uint16_t);
	DEFINE_DUMP_HARRAY(int32_t);
	DEFINE_DUMP_HARRAY(uint32_t);
	DEFINE_DUMP_HARRAY(float);
	DEFINE_DUMP_HARRAY(double);
	DEFINE_DUMP_HARRAY(hstr);
	DEFINE_DUMP_HARRAY(gvec2);
	DEFINE_DUMP_HARRAY(grect);

}