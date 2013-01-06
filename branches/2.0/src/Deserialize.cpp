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
#include <hltypes/hlog.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "Deserialize.h"
#include "Factory.h"
#include "liteser.h"
#include "Serializable.h"
#include "Utility.h"
#include "Variable.h"

namespace liteser
{
	void __loadVariable(Variable* variable)
	{
		unsigned char loadType;
		_load(&loadType);
		if (loadType != variable->type)
		{
			throw hl_exception(hsprintf("Variable type has changed. Expected: %02x, Got: %02x", variable->type, loadType));
		}
		switch (loadType)
		{
		case Variable::TYPE_INT8:	_load(variable->value<char>());				break;
		case Variable::TYPE_UINT8:	_load(variable->value<unsigned char>());		break;
		case Variable::TYPE_INT16:	_load(variable->value<int16_t>());			break;
		case Variable::TYPE_UINT16:	_load(variable->value<uint16_t>());			break;
		case Variable::TYPE_INT32:	_load(variable->value<int32_t>());			break;
		case Variable::TYPE_UINT32:	_load(variable->value<uint32_t>());			break;
		case Variable::TYPE_FLOAT:	_load(variable->value<float>());				break;
		case Variable::TYPE_DOUBLE:	_load(variable->value<double>());			break;
		case Variable::TYPE_BOOL:	_load(variable->value<bool>());				break;
		case Variable::TYPE_HSTR:	_load(variable->value<hstr>());				break;
		case Variable::TYPE_OBJECT:	_load(variable->value<Serializable>());		break;
		case Variable::TYPE_OBJPTR:	_load(variable->value<Serializable*>());		break;
		}
	}

	void _load(char* value)
	{
		*value = stream->load_char();
	}

	void _load(unsigned char* value)
	{
		*value = stream->load_uchar();
	}

	void _load(int16_t* value)
	{
		*value = stream->load_short();
	}

	void _load(uint16_t* value)
	{
		*value = stream->load_ushort();
	}

	void _load(int32_t* value)
	{
		*value = stream->load_int();
	}

	void _load(uint32_t* value)
	{
		*value = stream->load_uint();
	}

	void _load(float* value)
	{
		*value = stream->load_float();
	}

	void _load(double* value)
	{
		*value = stream->load_double();
	}

	void _load(bool* value)
	{
		*value = stream->load_bool();
	}

	void _load(hstr* value)
	{
		*value = stream->load_hstr();
	}

	void _load(Serializable* value)
	{
		__loadObject(&value);
	}

	void _load(Serializable** value)
	{
		if (*value != NULL)
		{
			delete (*value);
			*value = NULL;
		}
		__loadObject(value);
	}

	void __loadObject(Serializable** value)
	{
		uint32_t id;
		_load(&id);
		bool found = __tryGetObject(id, value);
		if (!found)
		{
			hstr className;
			_load(&className);
			if (*value == NULL)
			{
				*value = Factory::create(className);
			}
			__tryMapObject(&id, *value);
			harray<Variable*> variables = (*value)->_lsVars();
			uint32_t size;
			_load(&size);
			Variable* variable = NULL;
			hstr variableName;
			while (size > 0 && variables.size() > 0)
			{
				_load(&variableName);
				variable = NULL;
				foreach (Variable*, it, variables)
				{
					if ((*it)->name == variableName)
					{
						variable = (*it);
						break;
					}
				}
				if (variable != NULL)
				{
					__loadVariable(variable);
					variables.remove(variable);
					delete variable;
				}
				else
				{
					hlog::warn(liteser::logTag, "Could not find variable with name: " + variableName);
				}
				size--;
			}
			if (variables.size() > 0)
			{
				hlog::warn(liteser::logTag, "Not all variables were previously saved in class: " + className);
			}
		}
	}

}
