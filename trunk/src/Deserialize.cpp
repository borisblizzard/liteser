/// @file
/// @version 2.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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
		case Type::INT8:	*variable->value<char>()			= stream->load_int8();		break;
		case Type::UINT8:	*variable->value<unsigned char>()	= stream->load_uint8();		break;
		case Type::INT16:	*variable->value<short>()			= stream->load_int16();		break;
		case Type::UINT16:	*variable->value<unsigned short>()	= stream->load_uint16();	break;
		case Type::INT32:	*variable->value<int>()				= stream->load_int32();		break;
		case Type::UINT32:	*variable->value<unsigned int>()	= stream->load_uint32();	break;
		case Type::INT64:	*variable->value<int64_t>()			= stream->load_int64();		break;
		case Type::UINT64:	*variable->value<uint64_t>()		= stream->load_uint64();	break;
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
		case Type::INT8:	stream->load_int8();			return true;
		case Type::UINT8:	stream->load_uint8();			return true;
		case Type::INT16:	stream->load_int16();			return true;
		case Type::UINT16:	stream->load_uint16();			return true;
		case Type::INT32:	stream->load_int32();			return true;
		case Type::UINT32:	stream->load_uint32();			return true;
		case Type::INT64:	stream->load_int64();			return true;
		case Type::UINT64:	stream->load_uint64();			return true;
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
		variable->containerSize = stream->load_uint32();
		if (variable->containerSize > 0)
		{
			Type::Value loadType = Type::NONE;
			unsigned int typeSize = stream->load_uint32();
			if (typeSize != variable->type->subTypes.size())
			{
				throw hl_exception(hsprintf("Number of types for container does not match. Expected: %d, Got: %d", variable->type->subTypes.size(), typeSize));
			}
			for_itert (unsigned int, i, 0, typeSize)
			{
				loadType = (Type::Value)stream->load_uint8();
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
		unsigned int containerSize = stream->load_uint32();
		if (containerSize > 0)
		{
			harray<Type::Value> subTypes;
			int subTypesSize = (type == Type::HMAP ? 2 : 1);
			unsigned int typeSize = stream->load_uint32();
			if (typeSize != subTypesSize)
			{
				throw hl_exception(hsprintf("Number of types for container does not match. Expected: %d, Got: %d", subTypesSize, typeSize));
			}
			for_itert (unsigned int, i, 0, typeSize)
			{
				subTypes += (Type::Value)stream->load_uint8();
			}
			foreach (Type::Value, it, subTypes)
			{
				for_itert (unsigned int, i, 0, containerSize)
				{
					result &= __skipVariable(*it);
				}
			}
		}
		return result;
	}

	void _load(hstr* value)
	{
		unsigned int id = stream->load_uint32();
		if (!__tryGetString(id, value))
		{
			*value = stream->load_string();
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
		__loadObject(value);
	}

	void __loadObject(Serializable** value)
	{
		unsigned int id = stream->load_uint32();
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
			unsigned int size = stream->load_uint32();
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
				loadType = (Type::Value)stream->load_uint8();
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
		unsigned int id = stream->load_uint32();
		if (!__tryGetObject(id, &dummy))
		{
			hstr className;
			_load(&className);
			__forceMapEmptyObject(); // required for proper indexing of later variables
			unsigned int size = stream->load_uint32();
			hstr variableName;
			for_itert (unsigned int, i, 0, size)
			{
				_load(&variableName);
				__skipVariable((Type::Value)stream->load_uint8());
			}
		}
		return true;
	}

	void _loadHarray(harray<Serializable*>* value)
	{
		unsigned int size = stream->load_uint32();
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

#define DEFINE_LOAD_HARRAY(type, loadType) \
	void _loadHarray(harray<type>* value) \
	{ \
		unsigned int size = stream->load_uint32(); \
		for_itert (unsigned int, i, 0, size) \
		{ \
			value->add(stream->load_ ## loadType()); \
		} \
	}

#define DEFINE_LOAD_HARRAY_F(type) \
	void _loadHarray(harray<type>* value) \
	{ \
		unsigned int size = stream->load_uint32(); \
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

	DEFINE_LOAD_HARRAY(char, int8);
	DEFINE_LOAD_HARRAY(unsigned char, uint8);
	DEFINE_LOAD_HARRAY(short, int16);
	DEFINE_LOAD_HARRAY(unsigned short, uint16);
	DEFINE_LOAD_HARRAY(int, int32);
	DEFINE_LOAD_HARRAY(unsigned int, uint32);
	DEFINE_LOAD_HARRAY(int64_t, int64);
	DEFINE_LOAD_HARRAY(uint64_t, uint64);
	DEFINE_LOAD_HARRAY(float, float);
	DEFINE_LOAD_HARRAY(double, double);
	DEFINE_LOAD_HARRAY_F(hstr);
	DEFINE_LOAD_HARRAY_F(grect);
	DEFINE_LOAD_HARRAY_F(gvec2);
	DEFINE_LOAD_HARRAY_F(gvec3);

}
