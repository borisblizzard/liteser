/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hmap.h>

#include "liteser.h"
#include "Serializable.h"
#include "Utility.h"

namespace liteser
{
	bool active = false;
	hmap<unsigned int, Serializable*> objects;

	bool _tryMapObject(unsigned int* id, Serializable* object)
	{
		if (!objects.has_value(object))
		{
			// necessary to avoid incorrect size() since objects[*id] could be evaluated first
			*id = objects.size() + 1;
			objects[*id] = object;
			return true;
		}
		*id = objects(object);
		return false;
	}

	Serializable* _getObject(unsigned int id)
	{
		return objects.try_get_by_key(id, NULL);
	}

	void _start()
	{
		objects.clear();
		active = true;
	}

	void _finish()
	{
		objects.clear();
		active = false;
	}

	bool _isActive()
	{
		return active;
	}

	void _checkVersion(unsigned char major, unsigned char minor)
	{
		if (major != _LS_VERSION_MAJOR || minor != _LS_VERSION_MINOR)
		{
			throw hl_exception(hsprintf("Liteser Read Error! Version mismatch: expected %d.%d, got %d.%d",
				_LS_VERSION_MAJOR, _LS_VERSION_MINOR, major, minor));
		}
	}
	
	void _dump(hsbase* stream, Variable* variable)
	{
		_dump(stream, variable->name);
		_dump(stream, (unsigned char)variable->type);
		switch (variable->type)
		{
		case Variable::TYPE_INT8:	_dump(stream, *(int8_t*)(variable->value));			break;
		case Variable::TYPE_UINT8:	_dump(stream, *(uint8_t*)(variable->value));			break;
		case Variable::TYPE_INT16:	_dump(stream, *(int16_t*)(variable->value));			break;
		case Variable::TYPE_UINT16:	_dump(stream, *(uint16_t*)(variable->value));		break;
		case Variable::TYPE_INT32:	_dump(stream, *(int32_t*)(variable->value));			break;
		case Variable::TYPE_UINT32:	_dump(stream, *(uint32_t*)(variable->value));		break;
		case Variable::TYPE_FLOAT:	_dump(stream, *(float*)(variable->value));			break;
		case Variable::TYPE_DOUBLE:	_dump(stream, *(double*)(variable->value));			break;
		case Variable::TYPE_BOOL:	_dump(stream, *(bool*)(variable->value));			break;
		case Variable::TYPE_OBJECT:	_dump(stream, *(Serializable*)(variable->value));	break;
		case Variable::TYPE_OBJPTR:	_dump(stream, *(Serializable**)(variable->value));	break;
		case Variable::TYPE_HSTR:	_dump(stream, *(hstr*)(variable->value));			break;
		/*
		case Variable::TYPE_HDEQUE:	_dump(stream, *(hdeque*)(variable->value));			break;
		case Variable::TYPE_HARRAY:	_dump(stream, *(harray*)(variable->value));			break;
		case Variable::TYPE_HLIST:	_dump(stream, *(hlist*)(variable->value));			break;
		case Variable::TYPE_HMAP:	_dump(stream, *(hmap*)(variable->value));			break;
		*/
		}
	}

	void _dump(hsbase* stream, char* value)
	{
		stream->dump(value);
	}

	void _dump(hsbase* stream, unsigned char* value)
	{
		stream->dump(value);
	}

	void _dump(hsbase* stream, int16_t* value)
	{
		stream->dump(value);
	}

	void _dump(hsbase* stream, uint16_t* value)
	{
		stream->dump(value);
	}

	void _dump(hsbase* stream, int32_t* value)
	{
		stream->dump(value);
	}

	void _dump(hsbase* stream, uint32_t* value)
	{
		stream->dump(value);
	}

	void _dump(hsbase* stream, float* value)
	{
		stream->dump(value);
	}

	void _dump(hsbase* stream, double* value)
	{
		stream->dump(value);
	}

	void _dump(hsbase* stream, bool* value)
	{
		stream->dump(value);
	}

	void _dump(hsbase* stream, Serializable* value)
	{
		//stream->dump(value);
	}

	void _dump(hsbase* stream, Serializable** object)
	{
		uint32_t id;
		bool added = _tryMapObject(&id, *object);
		_dump(stream, &id);
		if (added)
		{
			harray<Variable> variables = (*object)->_lsVars();
			uint32_t size = (uint32_t)variables.size();
			_dump(stream, &size);
			foreach (Variable, it, variables)
			{
				_dump(stream, &(*it));
			}
		}
	}

	void _dump(hsbase* stream, hstr* value)
	{
		// TODO - implement own way of saving stuff
		// TODO - index variable names and strings
		stream->dump(value);
	}

}
