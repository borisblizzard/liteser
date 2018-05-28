/// @file
/// @version 3.0
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
#include <hltypes/hresource.h>
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
	inline void __loadVariableCompatible(Variable* variable, Type::Identifier loadedIdentifier)
	{
		// these are predefined compatible types only
		if (loadedIdentifier == Type::Identifier::Int8)			variable->setValue(stream->loadInt8());
		else if (loadedIdentifier == Type::Identifier::UInt8)	variable->setValue(stream->loadUint8());
		else if (loadedIdentifier == Type::Identifier::Int16)	variable->setValue(stream->loadInt16());
		else if (loadedIdentifier == Type::Identifier::UInt16)	variable->setValue(stream->loadUint16());
		else if (loadedIdentifier == Type::Identifier::Int32)	variable->setValue(stream->loadInt32());
		else if (loadedIdentifier == Type::Identifier::UInt32)	variable->setValue(stream->loadUint32());
		else if (loadedIdentifier == Type::Identifier::Int64)	variable->setValue(stream->loadInt64());
		else if (loadedIdentifier == Type::Identifier::UInt64)	variable->setValue(stream->loadUint64());
		else if (loadedIdentifier == Type::Identifier::Float)	variable->setValue(stream->loadFloat());
		else if (loadedIdentifier == Type::Identifier::Double)	variable->setValue(stream->loadDouble());
	}

	inline void __loadVariable(Variable* variable, Type::Identifier loadedIdentifier)
	{
		if (loadedIdentifier == Type::Identifier::Int8)				*variable->value<char>()			= stream->loadInt8();
		else if (loadedIdentifier == Type::Identifier::UInt8)		*variable->value<unsigned char>()	= stream->loadUint8();
		else if (loadedIdentifier == Type::Identifier::Int16)		*variable->value<short>()			= stream->loadInt16();
		else if (loadedIdentifier == Type::Identifier::UInt16)		*variable->value<unsigned short>()	= stream->loadUint16();
		else if (loadedIdentifier == Type::Identifier::Int32)		*variable->value<int>()				= stream->loadInt32();
		else if (loadedIdentifier == Type::Identifier::UInt32)		*variable->value<unsigned int>()	= stream->loadUint32();
		else if (loadedIdentifier == Type::Identifier::Int64)		*variable->value<int64_t>()			= stream->loadInt64();
		else if (loadedIdentifier == Type::Identifier::UInt64)		*variable->value<uint64_t>()		= stream->loadUint64();
		else if (loadedIdentifier == Type::Identifier::Float)		*variable->value<float>()			= stream->loadFloat();
		else if (loadedIdentifier == Type::Identifier::Double)		*variable->value<double>()			= stream->loadDouble();
		else if (loadedIdentifier == Type::Identifier::Bool)		*variable->value<bool>()			= stream->loadBool();
		else if (loadedIdentifier == Type::Identifier::Hstr)		_load(variable->value<hstr>());
		else if (loadedIdentifier == Type::Identifier::Hversion)	_load(variable->value<hversion>());
		else if (loadedIdentifier == Type::Identifier::Henum)		_load(variable->value<henum>());
		else if (loadedIdentifier == Type::Identifier::Grect)		_load(variable->value<grectf>());
		else if (loadedIdentifier == Type::Identifier::Gvec2)		_load(variable->value<gvec2f>());
		else if (loadedIdentifier == Type::Identifier::Gvec3)		_load(variable->value<gvec3f>());
		else if (loadedIdentifier == Type::Identifier::ValueObject)	_load(variable->value<Serializable>());
		else if (loadedIdentifier == Type::Identifier::Object)		_load(variable->value<Serializable*>());
		else if (loadedIdentifier == Type::Identifier::Harray)		__loadContainer(variable, loadedIdentifier);
		else if (loadedIdentifier == Type::Identifier::Hmap)		__loadContainer(variable, loadedIdentifier);
	}

	inline bool __skipVariable(Type::Identifier loadedIdentifier)
	{
		if (loadedIdentifier == Type::Identifier::Int8)			{ stream->loadInt8();			return true; }
		if (loadedIdentifier == Type::Identifier::UInt8)		{ stream->loadUint8();			return true; }
		if (loadedIdentifier == Type::Identifier::Int16)		{ stream->loadInt16();			return true; }
		if (loadedIdentifier == Type::Identifier::UInt16)		{ stream->loadUint16();			return true; }
		if (loadedIdentifier == Type::Identifier::Int32)		{ stream->loadInt32();			return true; }
		if (loadedIdentifier == Type::Identifier::UInt32)		{ stream->loadUint32();			return true; }
		if (loadedIdentifier == Type::Identifier::Int64)		{ stream->loadInt64();			return true; }
		if (loadedIdentifier == Type::Identifier::UInt64)		{ stream->loadUint64();			return true; }
		if (loadedIdentifier == Type::Identifier::Float)		{ stream->loadFloat();			return true; }
		if (loadedIdentifier == Type::Identifier::Double)		{ stream->loadDouble();			return true; }
		if (loadedIdentifier == Type::Identifier::Bool)			{ stream->loadBool();			return true; }
		if (loadedIdentifier == Type::Identifier::Hstr)			{ hstr var;		_load(&var);	return true; }
		if (loadedIdentifier == Type::Identifier::Hversion)		{ hversion var;	_load(&var);	return true; }
		if (loadedIdentifier == Type::Identifier::Henum)		{ henum var;	_load(&var);	return true; }
		if (loadedIdentifier == Type::Identifier::Grect)		{ grectf var;	_load(&var);	return true; }
		if (loadedIdentifier == Type::Identifier::Gvec2)		{ gvec2f var;	_load(&var);	return true; }
		if (loadedIdentifier == Type::Identifier::Gvec3)		{ gvec3f var;	_load(&var);	return true; }
		if (loadedIdentifier == Type::Identifier::ValueObject)	return __skipObject();
		if (loadedIdentifier == Type::Identifier::Object)		return __skipObject();
		if (loadedIdentifier == Type::Identifier::Harray)		return __skipContainer(loadedIdentifier);
		if (loadedIdentifier == Type::Identifier::Hmap)			return __skipContainer(loadedIdentifier);
		return false;
	}

	void __loadContainer(Variable* variable, Type::Identifier identifier)
	{
		variable->containerSize = stream->loadUint32();
		if (variable->containerSize > 0)
		{
			int typeSize = (int)stream->loadUint32();
			if (typeSize != variable->type->subTypes.size())
			{
				throw Exception(hsprintf("Number of types for container does not match. Expected: %d, Got: %d", variable->type->subTypes.size(), typeSize));
			}
			harray<Type::Identifier> loadedIdentifiers;
			for_iter (i, 0, typeSize)
			{
				loadedIdentifiers += _loadIdentifier();
				if (loadedIdentifiers[i] == Type::Identifier::Harray || loadedIdentifiers[i] == Type::Identifier::Hmap)
				{
					throw Exception(hsprintf("Template container within a template container detected, not supported: %02X", loadedIdentifiers[i].value));
				}
			}
			variable->createSubVariables(identifier);
			if (loadedIdentifiers.size() > 1) // if more than one load-type, the sub-variables contain the actual data
			{
				harray<int> sizes;
				for_iter (i, 0, variable->subVariables.size())
				{
					__loadVariable(variable->subVariables[i], variable->subVariables[i]->type->identifier);
					sizes += variable->subVariables[i]->containerSize;
				}
				if (sizes.size() > 1 && sizes.removedDuplicates().size() > 1)
				{
					throw Exception(hsprintf("Variable '%s' has container sub-variables that have different sizes: %s", variable->name.cStr(), sizes.cast<hstr>().joined(',').cStr()));
				}
			}
			else if (variable->subVariables.size() > 0)
			{
				if (variable->type->subTypes[0]->identifier == loadedIdentifiers[0])
				{
					for_iter (i, 0, variable->subVariables.size())
					{
						__loadVariable(variable->subVariables[i], loadedIdentifiers[0]);
					}
				}
				else
				{
					if (!_isCompatibleType(variable->type->subTypes[0]->identifier, loadedIdentifiers[0]))
					{
						throw Exception(hsprintf("Variable subtype of '%s' has changed. Expected: %02X, Got: %02X", variable->name.cStr(), variable->type->subTypes[0]->identifier.value, loadedIdentifiers[0].value));
					}
					hlog::warn(logTag, "Using compatible subtype for: " + variable->subVariables[0]->name);
					for_iter (i, 0, variable->subVariables.size())
					{
						__loadVariableCompatible(variable->subVariables[i], loadedIdentifiers[0]);
					}
				}
			}
			if (identifier == Type::Identifier::Hmap)
			{
				variable->applyHmapSubVariables(identifier);
			}
		}
	}

	bool __skipContainer(Type::Identifier identifier)
	{
		bool result = true;
		unsigned int containerSize = stream->loadUint32();
		if (containerSize > 0)
		{
			harray<Type::Identifier> subIdentifiers;
			int subTypesSize = (identifier == Type::Identifier::Hmap ? 2 : 1);
			int typeSize = (int)stream->loadUint32();
			if (typeSize != subTypesSize)
			{
				throw Exception(hsprintf("Number of types for container does not match. Expected: %d, Got: %d", subTypesSize, typeSize));
			}
			for_iter (i, 0, typeSize)
			{
				subIdentifiers += _loadIdentifier();
			}
			foreach (Type::Identifier, it, subIdentifiers)
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
		if (!_currentHeader.stringPooling)
		{
			*value = stream->loadString();
		}
		else
		{
			unsigned int id = stream->loadUint32();
			if (!__tryGetString(id, value))
			{
				*value = stream->loadString();
				__tryMapString(&id, *value);
			}
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

	void _load(grectf* value)
	{
		value->x = stream->loadFloat();
		value->y = stream->loadFloat();
		value->w = stream->loadFloat();
		value->h = stream->loadFloat();
	}

	void _load(gvec2f* value)
	{
		value->x = stream->loadFloat();
		value->y = stream->loadFloat();
	}

	void _load(gvec3f* value)
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
		unsigned int id = 0;
		if (_currentHeader.allowMultiReferencing)
		{
			id = stream->loadUint32();
		}
		if (!_currentHeader.allowMultiReferencing || !__tryGetObject(id, value))
		{
			hstr className;
			_load(&className);
			if (*value == NULL)
			{
				*value = Factory::create(className);
			}
			if (_currentHeader.allowMultiReferencing)
			{
				__tryMapObject(&id, *value);
			}
			harray<Variable*> variables = (*value)->_lsVars();
			harray<hstr> missingVariableNames;
			unsigned int size = stream->loadUint32();
			Variable* variable = NULL;
			hstr variableName;
			Type::Identifier loadedIdentifier;
			int variableIndex = -1;
			while (size > 0 && variables.size() > 0)
			{
				_load(&variableName);
				variable = NULL;
				variableIndex = -1;
				for_iter (i, 0, variables.size())
				{
					if (variables[i]->name == variableName)
					{
						variable = variables[i];
						variableIndex = i;
						break;
					}
				}
				loadedIdentifier = _loadIdentifier();
				if (variable != NULL)
				{
					if (variable->type->identifier == loadedIdentifier)
					{
						__loadVariable(variable, loadedIdentifier);
					}
					else
					{
						if (!_isCompatibleType(variable->type->identifier, loadedIdentifier))
						{
							throw Exception(hsprintf("Variable type of '%s' has changed. Expected: %02X, Got: %02X", variable->name.cStr(), variable->type->identifier.value, loadedIdentifier.value));
						}
						hlog::warn(logTag, "Using compatible type for: " + variable->name);
						__loadVariableCompatible(variable, loadedIdentifier);
					}
					delete variables.removeAt(variableIndex);
				}
				else
				{
					missingVariableNames += variableName;
					__skipVariable(loadedIdentifier);
				}
				--size;
			}
			while (size > 0)
			{
				_load(&variableName);
				loadedIdentifier = _loadIdentifier();
				missingVariableNames += variableName;
				__skipVariable(loadedIdentifier);
				--size;
			}
			if (missingVariableNames.size() > 0)
			{
				hlog::warn(logTag, className + " - Variables not part of class definition: " + missingVariableNames.joined(','));
			}
			if (variables.size() > 0)
			{
				harray<hstr> names;
				foreach (Variable*, it, variables)
				{
					names += (*it)->name;
					delete (*it);
				}
				hlog::warn(logTag, className + " - Not all variables were previously saved: " + names.joined(',').cStr());
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
		unsigned int id = 0;
		if (_currentHeader.allowMultiReferencing)
		{
			id = stream->loadUint32();
		}
		if (!_currentHeader.allowMultiReferencing || !__tryGetObject(id, &dummy))
		{
			hstr className;
			_load(&className);
			if (_currentHeader.allowMultiReferencing)
			{
				__forceMapEmptyObject(); // required for proper indexing of later variables
			}
			unsigned int size = stream->loadUint32();
			hstr variableName;
			for_itert (unsigned int, i, 0, size)
			{
				_load(&variableName);
				__skipVariable(_loadIdentifier());
			}
		}
		return true;
	}

	void _loadHarray(harray<Serializable*>* value, unsigned int size)
	{
		Serializable* object = NULL;
		for_itert (unsigned int, i, 0, size)
		{
			object = NULL;
			__loadObject(&object);
			value->add(object);
		}
	}

#define DEFINE_LOAD_HARRAY(type, loadType) \
	void _loadHarray(harray<type>* value, unsigned int size) \
	{ \
		for_itert (unsigned int, i, 0, size) \
		{ \
			value->add(stream->load ## loadType()); \
		} \
	}

#define DEFINE_LOAD_HARRAY_F(type) \
	void _loadHarray(harray<type>* value, unsigned int size) \
	{ \
		type object; \
		for_itert (unsigned int, i, 0, size) \
		{ \
			_load(&object); \
			value->add(object); \
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
	DEFINE_LOAD_HARRAY_F(grectf);
	DEFINE_LOAD_HARRAY_F(gvec2f);
	DEFINE_LOAD_HARRAY_F(gvec3f);

}
