/// @file
/// @version 2.5
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
#include <hltypes/hversion.h>

#include "Deserialize.h"
#include "Factory.h"
#include "liteser.h"
#include "Serializable.h"
#include "Type.h"
#include "Utility.h"
#include "Variable.h"

namespace liteser
{
	Type::Value _loadType()
	{
		return (Type::Value)stream->loadUint8();
	}

	inline void __loadVariable(Variable* variable, Type::Value loadType)
	{
		switch (loadType)
		{
		case Type::INT8:		*variable->value<char>()			= stream->loadInt8();		break;
		case Type::UINT8:		*variable->value<unsigned char>()	= stream->loadUint8();		break;
		case Type::INT16:		*variable->value<short>()			= stream->loadInt16();		break;
		case Type::UINT16:		*variable->value<unsigned short>()	= stream->loadUint16();		break;
		case Type::INT32:		*variable->value<int>()				= stream->loadInt32();		break;
		case Type::UINT32:		*variable->value<unsigned int>()	= stream->loadUint32();		break;
		case Type::INT64:		*variable->value<int64_t>()			= stream->loadInt64();		break;
		case Type::UINT64:		*variable->value<uint64_t>()		= stream->loadUint64();		break;
		case Type::FLOAT:		*variable->value<float>()			= stream->loadFloat();		break;
		case Type::DOUBLE:		*variable->value<double>()			= stream->loadDouble();		break;
		case Type::BOOL:		*variable->value<bool>()			= stream->loadBool();		break;
		case Type::HSTR:		_load(variable->value<hstr>());									break;
		case Type::HVERSION:	_load(variable->value<hversion>());								break;
		case Type::HENUM:		_load(variable->value<henum>());								break;
		case Type::GRECT:		_load(variable->value<grect>());								break;
		case Type::GVEC2:		_load(variable->value<gvec2>());								break;
		case Type::GVEC3:		_load(variable->value<gvec3>());								break;
		case Type::OBJECT:		_load(variable->value<Serializable>());							break;
		case Type::OBJPTR:		_load(variable->value<Serializable*>());						break;
		case Type::HARRAY:		__loadContainer(variable, loadType);							break;
		case Type::HMAP:		__loadContainer(variable, loadType);							break;
		default:																				break;
		}
	}

	inline bool __skipVariable(Type::Value loadType)
	{
		switch (loadType)
		{
		case Type::INT8:		stream->loadInt8();				return true;
		case Type::UINT8:		stream->loadUint8();			return true;
		case Type::INT16:		stream->loadInt16();			return true;
		case Type::UINT16:		stream->loadUint16();			return true;
		case Type::INT32:		stream->loadInt32();			return true;
		case Type::UINT32:		stream->loadUint32();			return true;
		case Type::INT64:		stream->loadInt64();			return true;
		case Type::UINT64:		stream->loadUint64();			return true;
		case Type::FLOAT:		stream->loadFloat();			return true;
		case Type::DOUBLE:		stream->loadDouble();			return true;
		case Type::BOOL:		stream->loadBool();				return true;
		case Type::HSTR:		{ hstr var;		_load(&var);	return true; }
		case Type::HVERSION:	{ hversion var;	_load(&var);	return true; }
		case Type::HENUM:		{ henum var;	_load(&var);	return true; }
		case Type::GRECT:		{ grect var;	_load(&var);	return true; }
		case Type::GVEC2:		{ gvec2 var;	_load(&var);	return true; }
		case Type::GVEC3:		{ gvec3 var;	_load(&var);	return true; }
		case Type::OBJECT:		return __skipObject();
		case Type::OBJPTR:		return __skipObject();
		case Type::HARRAY:		return __skipContainer(loadType);
		case Type::HMAP:		return __skipContainer(loadType);
		default:				break;
		}
		return false;
	}

	void __loadContainer(Variable* variable, Type::Value type)
	{
		variable->containerSize = stream->loadUint32();
		if (variable->containerSize > 0)
		{
			Type::Value loadType = Type::NONE;
			int typeSize = (int)stream->loadUint32();
			if (typeSize != variable->type->subTypes.size())
			{
				throw Exception(hsprintf("Number of types for container does not match. Expected: %d, Got: %d", variable->type->subTypes.size(), typeSize));
			}
			for_iter (i, 0, typeSize)
			{
				loadType = _loadType();
				if (loadType != variable->type->subTypes[i]->value)
				{
					throw Exception(hsprintf("Variable type has changed. Expected: %02X, Got: %02X", variable->type->value, loadType));
				}
				if (loadType == Type::HARRAY || loadType == Type::HMAP)
				{
					throw Exception(hsprintf("Template container within a template container detected, not supported: %02X", loadType));
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
		unsigned int containerSize = stream->loadUint32();
		if (containerSize > 0)
		{
			harray<Type::Value> subTypes;
			int subTypesSize = (type == Type::HMAP ? 2 : 1);
			int typeSize = (int)stream->loadUint32();
			if (typeSize != subTypesSize)
			{
				throw Exception(hsprintf("Number of types for container does not match. Expected: %d, Got: %d", subTypesSize, typeSize));
			}
			for_iter (i, 0, typeSize)
			{
				subTypes += _loadType();
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
		unsigned int id = stream->loadUint32();
		if (!__tryGetString(id, value))
		{
			*value = stream->loadString();
			__tryMapString(&id, *value);
		}
	}

	void _load(hversion* value)
	{
		value->major = stream->loadUint32();
		value->minor = stream->loadUint32();
		value->revision = stream->loadUint32();
		value->build = stream->loadUint32();
	}

	void _load(henum* value)
	{
		value->value = stream->loadUint32();
	}

	void _load(grect* value)
	{
		value->x = stream->loadFloat();
		value->y = stream->loadFloat();
		value->w = stream->loadFloat();
		value->h = stream->loadFloat();
	}

	void _load(gvec2* value)
	{
		value->x = stream->loadFloat();
		value->y = stream->loadFloat();
	}

	void _load(gvec3* value)
	{
		value->x = stream->loadFloat();
		value->y = stream->loadFloat();
		value->z = stream->loadFloat();
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
		unsigned int id = stream->loadUint32();
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
			unsigned int size = stream->loadUint32();
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
				loadType = _loadType();
				if (variable != NULL)
				{
					if (loadType != variable->type->value)
					{
						throw Exception(hsprintf("Variable type has changed. Expected: %02X, Got: %02X", variable->type->value, loadType));
					}
					__loadVariable(variable, loadType);
					variables.remove(variable);
					delete variable;
				}
				else
				{
					hlog::warn(logTag, "Could not find variable with name: " + variableName);
					__skipVariable(loadType);
				}
				--size;
			}
			if (variables.size() > 0)
			{
				hlog::warn(logTag, "Not all variables were previously saved in class: " + className);
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
		unsigned int id = stream->loadUint32();
		if (!__tryGetObject(id, &dummy))
		{
			hstr className;
			_load(&className);
			__forceMapEmptyObject(); // required for proper indexing of later variables
			unsigned int size = stream->loadUint32();
			hstr variableName;
			for_itert (unsigned int, i, 0, size)
			{
				_load(&variableName);
				__skipVariable(_loadType());
			}
		}
		return true;
	}

	void _loadHarray(harray<Serializable*>* value)
	{
		unsigned int size = stream->loadUint32();
		if (size > 0)
		{
			Serializable* object = NULL;
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
		unsigned int size = stream->loadUint32(); \
		for_itert (unsigned int, i, 0, size) \
		{ \
			value->add(stream->load ## loadType()); \
		} \
	}

#define DEFINE_LOAD_HARRAY_F(type) \
	void _loadHarray(harray<type>* value) \
	{ \
		unsigned int size = stream->loadUint32(); \
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

	DEFINE_LOAD_HARRAY(char, Int8);
	DEFINE_LOAD_HARRAY(unsigned char, Uint8);
	DEFINE_LOAD_HARRAY(short, Int16);
	DEFINE_LOAD_HARRAY(unsigned short, Uint16);
	DEFINE_LOAD_HARRAY(int, Int32);
	DEFINE_LOAD_HARRAY(unsigned int, Uint32);
	DEFINE_LOAD_HARRAY(int64_t, Int64);
	DEFINE_LOAD_HARRAY(uint64_t, Uint64);
	DEFINE_LOAD_HARRAY(float, Float);
	DEFINE_LOAD_HARRAY(double, Double);
	DEFINE_LOAD_HARRAY_F(hstr);
	DEFINE_LOAD_HARRAY_F(hversion);
	DEFINE_LOAD_HARRAY_F(henum);
	DEFINE_LOAD_HARRAY_F(grect);
	DEFINE_LOAD_HARRAY_F(gvec2);
	DEFINE_LOAD_HARRAY_F(gvec3);

}
