/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hmap.h>
#include <hltypes/hsbase.h>

#include "liteser.h"
#include "Serializable.h"
#include "Utility.h"

namespace liteser
{
	hmap<unsigned int, Serializable*> objects;
	hsbase* stream = NULL;

	bool __tryMapObject(unsigned int* id, Serializable* object)
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

	Serializable* __getObject(unsigned int id)
	{
		return objects.try_get_by_key(id, NULL);
	}

	void _start(hsbase* stream)
	{
		objects.clear();
		liteser::stream = stream;
	}

	void _finish(hsbase* stream)
	{
		objects.clear();
		liteser::stream = NULL;
	}

	bool _isActive()
	{
		return (stream != NULL);
	}

	void _checkVersion(unsigned char major, unsigned char minor)
	{
		if (major != _LS_VERSION_MAJOR || minor != _LS_VERSION_MINOR)
		{
			throw hl_exception(hsprintf("Liteser Read Error! Version mismatch: expected %d.%d, got %d.%d",
				_LS_VERSION_MAJOR, _LS_VERSION_MINOR, major, minor));
		}
	}

	void __dumpVariableType(Variable::Type type)
	{
		stream->dump((unsigned char)type);
	}
	
	void _dump(Variable* variable)
	{
		_dump(&variable->name);
		unsigned char type = (unsigned char)variable->type;
		_dump(&type);
		switch (variable->type)
		{
		case Variable::TYPE_INT8:	_dump((char*)(variable->value));				break;
		case Variable::TYPE_UINT8:	_dump((unsigned char*)(variable->value));	break;
		case Variable::TYPE_INT16:	_dump((int16_t*)(variable->value));			break;
		case Variable::TYPE_UINT16:	_dump((uint16_t*)(variable->value));			break;
		case Variable::TYPE_INT32:	_dump((int32_t*)(variable->value));			break;
		case Variable::TYPE_UINT32:	_dump((uint32_t*)(variable->value));			break;
		case Variable::TYPE_FLOAT:	_dump((float*)(variable->value));			break;
		case Variable::TYPE_DOUBLE:	_dump((double*)(variable->value));			break;
		case Variable::TYPE_BOOL:	_dump((bool*)(variable->value));				break;
		case Variable::TYPE_OBJECT:	_dump((Serializable*)(variable->value));		break;
		case Variable::TYPE_OBJPTR:	_dump((Serializable**)(variable->value));	break;
		case Variable::TYPE_HSTR:	_dump((hstr*)(variable->value));				break;
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

	void _dump(Serializable* object)
	{
		uint32_t id;
		bool added = __tryMapObject(&id, object);
		_dump(&id);
		if (added)
		{
			_dump(&object->_lsName());
			harray<Variable> variables = object->_lsVars();
			uint32_t size = (uint32_t)variables.size();
			_dump(&size);
			foreach (Variable, it, variables)
			{
				_dump(&(*it));
			}
		}
	}

	void _dump(Serializable** object)
	{
		if (dynamic_cast<Serializable*>(*object) == NULL)
		{
			throw hl_exception("Trying to serialize object that does not inherit liteser::Serializable.");
		}
		_dump(*object);
	}

	void _dump(hstr* value)
	{
		// TODO - implement own way of saving stuff
		// TODO - index variable names and strings
		stream->dump(*value);
	}

}
