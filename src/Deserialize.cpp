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
#include <hltypes/hlog.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "Deserialize.h"
#include "Factory.h"
#include "liteser.h"
#include "Serializable.h"
#include "Type.h"
#include "Utility.h"
#include "Variable.h"

namespace liteser
{
	void __loadVariable(Variable* variable)
	{
		Type::Value loadType = Type::NONE;
		_load((unsigned char*)&loadType);
		if (loadType != variable->type->value)
		{
			throw hl_exception(hsprintf("Variable type has changed. Expected: %02X, Got: %02X", variable->type->value, loadType));
		}
		__loadVariableValue(variable, loadType);
	}

	void __loadVariableValue(Variable* variable, Type::Value loadType)
	{
		switch (loadType)
		{
		case Type::INT8:	_load(variable->value<char>());				break;
		case Type::UINT8:	_load(variable->value<unsigned char>());	break;
		case Type::INT16:	_load(variable->value<int16_t>());			break;
		case Type::UINT16:	_load(variable->value<uint16_t>());			break;
		case Type::INT32:	_load(variable->value<int32_t>());			break;
		case Type::UINT32:	_load(variable->value<uint32_t>());			break;
		case Type::FLOAT:	_load(variable->value<float>());			break;
		case Type::DOUBLE:	_load(variable->value<double>());			break;
		case Type::BOOL:	_load(variable->value<bool>());				break;
		case Type::HSTR:	_load(variable->value<hstr>());				break;
		case Type::OBJECT:	_load(variable->value<Serializable>());		break;
		case Type::OBJPTR:	_load(variable->value<Serializable*>());	break;
		case Type::HARRAY:	__loadContainer(variable, loadType);		break;
		case Type::HMAP:	__loadContainer(variable, loadType);		break;
		}
	}

	void __loadContainer(Variable* variable, Type::Value type)
	{
		uint32_t size = 0;
		_load(&size);
		if (size > 0)
		{
			Type::Value loadType = Type::NONE;
			uint32_t typeSize = 0;
			_load(&typeSize);
			if (typeSize != variable->type->subTypes.size())
			{
				throw hl_exception(hsprintf("Number of types for container does not match. Expected: %d, Got: %d", variable->type->subTypes.size(), typeSize));
			}
			for_itert (unsigned int, i, 0, typeSize)
			{
				_load((unsigned char*)&loadType);
				if (loadType != variable->type->subTypes[i]->value)
				{
					throw hl_exception(hsprintf("Variable type has changed. Expected: %02X, Got: %02X", variable->type->value, loadType));
				}
				if (loadType == Type::HARRAY || loadType == Type::HMAP)
				{
					throw hl_exception(hsprintf("Template container within a template container detected, not supported: %02X", loadType));
				}
			}
			variable->createSubVariables(type, size);
			foreach (Variable*, it, variable->subVariables)
			{
				__loadVariableValue((*it), (*it)->type->value);
			}
			if (type == Type::HMAP)
			{
				foreach (Variable*, it, variable->subVariables)
				{
					__loadVariableValue((*it), (*it)->type->value);
				}
				variable->applyHmapSubVariables(type);
			}
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

	void _loadHarray(harray<Serializable*>* value)
	{
		uint32_t size = 0;
		_load(&size);
		if (size > 0)
		{
			Serializable* object;
			for_itert (unsigned int, i, 0, size)
			{
				object = NULL;
				__loadObject(&object);
				value->add(object);
			}
		}
	}

#define DEFINE_LOAD_HARRAY(type) \
	void _loadHarray(harray<type>* value) \
	{ \
		uint32_t size = 0; \
		_load(&size); \
		if (size > 0) \
		{ \
			type object; \
			for_itert (unsigned int, i, 0, size) \
			{ \
				_load(&object); \
				value->add(object); \
			} \
		} \
	}

	DEFINE_LOAD_HARRAY(char);
	DEFINE_LOAD_HARRAY(unsigned char);
	DEFINE_LOAD_HARRAY(int16_t);
	DEFINE_LOAD_HARRAY(uint16_t);
	DEFINE_LOAD_HARRAY(int32_t);
	DEFINE_LOAD_HARRAY(uint32_t);
	DEFINE_LOAD_HARRAY(float);
	DEFINE_LOAD_HARRAY(double);
	DEFINE_LOAD_HARRAY(hstr);

}
