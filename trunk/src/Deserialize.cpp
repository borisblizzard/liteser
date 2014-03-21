/// @file
/// @author  Boris Mikic
/// @version 2.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <stdint.h>

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hlog.h>
#include <hltypes/hmap.h>
#include <hltypes/hsbase.h>
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
	inline void __loadVariable(Variable* variable, Type::Value loadType)
	{
		switch (loadType)
		{
		case Type::INT8:	*variable->value<char>()			= stream->load_char();		break;
		case Type::UINT8:	*variable->value<unsigned char>()	= stream->load_uchar();		break;
		case Type::INT16:	*variable->value<int16_t>()			= stream->load_short();		break;
		case Type::UINT16:	*variable->value<uint16_t>()		= stream->load_ushort();	break;
		case Type::INT32:	*variable->value<int32_t>()			= stream->load_int();		break;
		case Type::UINT32:	*variable->value<uint32_t>()		= stream->load_uint();		break;
		case Type::FLOAT:	*variable->value<float>()			= stream->load_float();		break;
		case Type::DOUBLE:	*variable->value<double>()			= stream->load_double();	break;
		case Type::BOOL:	*variable->value<bool>()			= stream->load_bool();		break;
		case Type::HSTR:	_load(variable->value<hstr>());									break;
		case Type::GRECT:	_load(variable->value<grect>());								break;
		case Type::GVEC2:	_load(variable->value<gvec2>());								break;
		case Type::GVEC3:	_load(variable->value<gvec3>());								break;
		case Type::OBJECT:	_load(variable->value<Serializable>());							break;
		case Type::OBJPTR:	_load(variable->value<Serializable*>());						break;
		case Type::HARRAY:	__loadContainer(variable, loadType);							break;
		case Type::HMAP:	__loadContainer(variable, loadType);							break;
		}
	}

	inline bool __skipVariable(Type::Value loadType)
	{
		switch (loadType)
		{
		case Type::INT8:	stream->load_char();			return true;
		case Type::UINT8:	stream->load_uchar();			return true;
		case Type::INT16:	stream->load_short();			return true;
		case Type::UINT16:	stream->load_ushort();			return true;
		case Type::INT32:	stream->load_int();				return true;
		case Type::UINT32:	stream->load_uint();			return true;
		case Type::FLOAT:	stream->load_float();			return true;
		case Type::DOUBLE:	stream->load_double();			return true;
		case Type::BOOL:	stream->load_bool();			return true;
		case Type::HSTR:	{ hstr var;		_load(&var);	return true; }
		case Type::GRECT:	{ grect var;	_load(&var);	return true; }
		case Type::GVEC2:	{ gvec2 var;	_load(&var);	return true; }
		case Type::GVEC3:	{ gvec3 var;	_load(&var);	return true; }
		case Type::OBJECT:	return __skipObject();
		case Type::OBJPTR:	return __skipObject();
		case Type::HARRAY:	return __skipContainer(loadType);
		case Type::HMAP:	return __skipContainer(loadType);
		}
		return false;
	}

	void __loadContainer(Variable* variable, Type::Value type)
	{
		variable->containerSize = stream->load_uint();
		if (variable->containerSize > 0)
		{
			Type::Value loadType = Type::NONE;
			uint32_t typeSize = stream->load_uint();
			if (typeSize != variable->type->subTypes.size())
			{
				throw hl_exception(hsprintf("Number of types for container does not match. Expected: %d, Got: %d", variable->type->subTypes.size(), typeSize));
			}
			for_itert (uint32_t, i, 0, typeSize)
			{
				loadType = (Type::Value)stream->load_uchar();
				if (loadType != variable->type->subTypes[i]->value)
				{
					throw hl_exception(hsprintf("Variable type has changed. Expected: %02X, Got: %02X", variable->type->value, loadType));
				}
				if (loadType == Type::HARRAY || loadType == Type::HMAP)
				{
					throw hl_exception(hsprintf("Template container within a template container detected, not supported: %02X", loadType));
				}
			}
			variable->createSubVariables(type);
			foreach (Variable*, it, variable->subVariables)
			{
				__loadVariable((*it), (*it)->type->value);
			}
			if (type == Type::HMAP)
			{
				variable->applyHmapSubVariables(type);
			}
		}
	}

	bool __skipContainer(Type::Value type)
	{
		bool result = true;
		uint32_t containerSize = stream->load_uint();
		if (containerSize > 0)
		{
			harray<Type::Value> subTypes;
			int subTypesSize = (type == Type::HMAP ? 2 : 1);
			uint32_t typeSize = stream->load_uint();
			if (typeSize != subTypesSize)
			{
				throw hl_exception(hsprintf("Number of types for container does not match. Expected: %d, Got: %d", subTypesSize, typeSize));
			}
			for_itert (uint32_t, i, 0, typeSize)
			{
				subTypes += (Type::Value)stream->load_uchar();
			}
			foreach (Type::Value, it, subTypes)
			{
				for_itert (uint32_t, i, 0, containerSize)
				{
					result &= __skipVariable(*it);
				}
			}
		}
		return result;
	}

	void _load(hstr* value)
	{
		uint32_t id = stream->load_uint();
		if (!__tryGetString(id, value))
		{
			*value = stream->load_hstr();
			__tryMapString(&id, *value);
		}
	}

	void _load(grect* value)
	{
		value->x = stream->load_float();
		value->y = stream->load_float();
		value->w = stream->load_float();
		value->h = stream->load_float();
	}

	void _load(gvec2* value)
	{
		value->x = stream->load_float();
		value->y = stream->load_float();
	}

	void _load(gvec3* value)
	{
		value->x = stream->load_float();
		value->y = stream->load_float();
		value->z = stream->load_float();
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
		uint32_t id = stream->load_uint();
		if (!__tryGetObject(id, value))
		{
			hstr className;
			_load(&className);
			if (*value == NULL)
			{
				*value = Factory::create(className);
			}
			__tryMapObject(&id, *value);
			harray<Variable*> variables = (*value)->_lsVars();
			uint32_t size = stream->load_uint();
			Variable* variable = NULL;
			hstr variableName;
			Type::Value loadType;
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
				loadType = (Type::Value)stream->load_uchar();
				if (variable != NULL)
				{
					if (loadType != variable->type->value)
					{
						throw hl_exception(hsprintf("Variable type has changed. Expected: %02X, Got: %02X", variable->type->value, loadType));
					}
					__loadVariable(variable, loadType);
					variables.remove(variable);
					delete variable;
				}
				else
				{
					hlog::warn(liteser::logTag, "Could not find variable with name: " + variableName);
					__skipVariable(loadType);
				}
				--size;
			}
			if (variables.size() > 0)
			{
				hlog::warn(liteser::logTag, "Not all variables were previously saved in class: " + className);
				foreach (Variable*, it, variables)
				{
					delete (*it);
				}
			}
		}
		else if (id == 0)
		{
			*value = NULL;
		}
	}

	bool __skipObject()
	{
		Serializable* dummy = NULL;
		uint32_t id = stream->load_uint();
		if (!__tryGetObject(id, &dummy))
		{
			hstr className;
			_load(&className);
			__forceMapEmptyObject(); // required for proper indexing of later variables
			uint32_t size = stream->load_uint();
			hstr variableName;
			for_itert (uint32_t, i, 0, size)
			{
				_load(&variableName);
				__skipVariable((Type::Value)stream->load_uchar());
			}
		}
		return true;
	}

	void _loadHarray(harray<Serializable*>* value)
	{
		uint32_t size = stream->load_uint();
		if (size > 0)
		{
			Serializable* object;
			for_itert (uint32_t, i, 0, size)
			{
				object = NULL;
				__loadObject(&object);
				value->add(object);
			}
		}
	}

#define DEFINE_LOAD_HARRAY(type, loadType) \
	void _loadHarray(harray<type>* value) \
	{ \
		uint32_t size = stream->load_uint(); \
		for_itert (uint32_t, i, 0, size) \
		{ \
			value->add(stream->load_ ## loadType()); \
		} \
	}

#define DEFINE_LOAD_HARRAY_X(type) \
	void _loadHarray(harray<type>* value) \
	{ \
		uint32_t size = stream->load_uint(); \
		if (size > 0) \
		{ \
			type object; \
			for_itert (uint32_t, i, 0, size) \
			{ \
				_load(&object); \
				value->add(object); \
			} \
		} \
	}

	DEFINE_LOAD_HARRAY(char, char);
	DEFINE_LOAD_HARRAY(unsigned char, uchar);
	DEFINE_LOAD_HARRAY(int16_t, short);
	DEFINE_LOAD_HARRAY(uint16_t, ushort);
	DEFINE_LOAD_HARRAY(int32_t, int);
	DEFINE_LOAD_HARRAY(uint32_t, uint);
	DEFINE_LOAD_HARRAY(float, float);
	DEFINE_LOAD_HARRAY(double, double);
	DEFINE_LOAD_HARRAY_X(hstr);
	DEFINE_LOAD_HARRAY_X(grect);
	DEFINE_LOAD_HARRAY_X(gvec2);
	DEFINE_LOAD_HARRAY_X(gvec3);

}
