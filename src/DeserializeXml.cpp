/// @file
/// @version 3.2
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
#include <hlxml/Node.h>

#include "DeserializeXml.h"
#include "Factory.h"
#include "liteser.h"
#include "Serializable.h"
#include "Type.h"
#include "Utility.h"
#include "Variable.h"

#define VALUE "value"
#define SUB_TYPES "sub_types"
#define SEPARATOR ','
#define OBJECT_ID "id"

namespace liteser
{
	namespace xml
	{
		inline void __loadVariableCompatible(hlxml::Node* node, Variable* variable, Type::Identifier loadedIdentifier)
		{
			// these are predefined compatible types only
			if (loadedIdentifier == Type::Identifier::Int8)			variable->setValue(node->pint8(VALUE));
			else if (loadedIdentifier == Type::Identifier::Uint8)	variable->setValue(node->puint8(VALUE));
			else if (loadedIdentifier == Type::Identifier::Int16)	variable->setValue(node->pint16(VALUE));
			else if (loadedIdentifier == Type::Identifier::Uint16)	variable->setValue(node->puint16(VALUE));
			else if (loadedIdentifier == Type::Identifier::Int32)	variable->setValue(node->pint32(VALUE));
			else if (loadedIdentifier == Type::Identifier::Uint32)	variable->setValue(node->puint32(VALUE));
			else if (loadedIdentifier == Type::Identifier::Int64)	variable->setValue(node->pint64(VALUE));
			else if (loadedIdentifier == Type::Identifier::Uint64)	variable->setValue(node->puint64(VALUE));
			else if (loadedIdentifier == Type::Identifier::Float)	variable->setValue(node->pfloat(VALUE));
			else if (loadedIdentifier == Type::Identifier::Double)	variable->setValue(node->pdouble(VALUE));
		}

		inline void __loadVariable(hlxml::Node* node, Variable* variable, Type::Identifier loadedIdentifier)
		{
			if (loadedIdentifier == Type::Identifier::Int8)				*variable->value<char>()			= node->pint8(VALUE);
			else if (loadedIdentifier == Type::Identifier::Uint8)		*variable->value<unsigned char>()	= node->puint8(VALUE);
			else if (loadedIdentifier == Type::Identifier::Int16)		*variable->value<short>()			= node->pint16(VALUE);
			else if (loadedIdentifier == Type::Identifier::Uint16)		*variable->value<unsigned short>()	= node->puint16(VALUE);
			else if (loadedIdentifier == Type::Identifier::Int32)		*variable->value<int>()				= node->pint32(VALUE);
			else if (loadedIdentifier == Type::Identifier::Uint32)		*variable->value<unsigned int>()	= node->puint32(VALUE);
			else if (loadedIdentifier == Type::Identifier::Int64)		*variable->value<int64_t>()			= node->pint64(VALUE);
			else if (loadedIdentifier == Type::Identifier::Uint64)		*variable->value<uint64_t>()		= node->puint64(VALUE);
			else if (loadedIdentifier == Type::Identifier::Float)		*variable->value<float>()			= node->pfloat(VALUE);
			else if (loadedIdentifier == Type::Identifier::Double)		*variable->value<double>()			= node->pdouble(VALUE);
			else if (loadedIdentifier == Type::Identifier::Bool)		*variable->value<bool>()			= node->pbool(VALUE);
			else if (loadedIdentifier == Type::Identifier::Hstr)		_load(node, variable->value<hstr>());
			else if (loadedIdentifier == Type::Identifier::Hversion)	_load(node, variable->value<hversion>());
			else if (loadedIdentifier == Type::Identifier::Henum)		_load(node, variable->value<henum>());
			else if (loadedIdentifier == Type::Identifier::Grectf)		_load(node, variable->value<grectf>());
			else if (loadedIdentifier == Type::Identifier::Gvec2f)		_load(node, variable->value<gvec2f>());
			else if (loadedIdentifier == Type::Identifier::Gvec3f)		_load(node, variable->value<gvec3f>());
			else if (loadedIdentifier == Type::Identifier::Grecti)		_load(node, variable->value<grecti>());
			else if (loadedIdentifier == Type::Identifier::Gvec2i)		_load(node, variable->value<gvec2i>());
			else if (loadedIdentifier == Type::Identifier::Gvec3i)		_load(node, variable->value<gvec3i>());
			else if (loadedIdentifier == Type::Identifier::Grectd)		_load(node, variable->value<grectd>());
			else if (loadedIdentifier == Type::Identifier::Gvec2d)		_load(node, variable->value<gvec2d>());
			else if (loadedIdentifier == Type::Identifier::Gvec3d)		_load(node, variable->value<gvec3d>());
			else if (loadedIdentifier == Type::Identifier::ValueObject)	_load(node, variable->value<Serializable>());
			else if (loadedIdentifier == Type::Identifier::Object)		_load(node, variable->value<Serializable*>());
			else if (loadedIdentifier == Type::Identifier::Harray)		__loadContainer(node, variable, loadedIdentifier);
			else if (loadedIdentifier == Type::Identifier::Hmap)		__loadContainer(node, variable, loadedIdentifier);
		}

		inline bool __skipVariable(hlxml::Node* node, Type::Identifier loadedIdentifier)
		{
			if (loadedIdentifier == Type::Identifier::ValueObject)	return __skipObject(node);
			if (loadedIdentifier == Type::Identifier::Object)		return __skipObject(node);
			if (loadedIdentifier == Type::Identifier::Harray)		return __skipContainer(node, loadedIdentifier);
			if (loadedIdentifier == Type::Identifier::Hmap)			return __skipContainer(node, loadedIdentifier);
			return false;
		}

		void __loadContainer(hlxml::Node* node, Variable* variable, Type::Identifier identifier)
		{
			variable->containerSize = node->children.size();
			if (variable->containerSize > 0)
			{
				harray<Type::Identifier> loadedIdentifiers;
				harray<hstr> subTypes = node->pstr(SUB_TYPES).split(SEPARATOR, -1, true);
				foreach (hstr, it, subTypes)
				{
					loadedIdentifiers += Type::Identifier::fromUint((*it).unhex());
				}
				if (loadedIdentifiers.size() != variable->type->subTypes.size())
				{
					throw Exception(hsprintf("Number of types for container does not match. Expected: %d, Got: %d", variable->type->subTypes.size(), loadedIdentifiers.size()));
				}
				for_iter (i, 0, loadedIdentifiers.size())
				{
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
						__loadVariable(node->children[i], variable->subVariables[i], variable->subVariables[i]->type->identifier);
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
							__loadVariable(node->children[i], variable->subVariables[i], loadedIdentifiers[0]);
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
							__loadVariableCompatible(node->children[i], variable->subVariables[i], loadedIdentifiers[0]);
						}
					}
				}
				if (identifier == Type::Identifier::Hmap)
				{
					variable->applyHmapSubVariables(identifier);
				}
			}
		}

		bool __skipContainer(hlxml::Node* node, Type::Identifier identifier)
		{
			bool result = true;
			if (node->children.size() > 0)
			{
				int subTypesSize = (identifier == Type::Identifier::Hmap ? 2 : 1);
				harray<Type::Identifier> subIdentifiers;
				harray<hstr> types = node->pstr(SUB_TYPES).split(SEPARATOR, -1, true);
				foreach (hstr, it, types)
				{
					subIdentifiers += Type::Identifier::fromUint((*it).unhex());
				}
				if (subIdentifiers.size() != subTypesSize)
				{
					throw Exception(hsprintf("Number of types for container does not match. Expected: %d, Got: %d", subTypesSize, subIdentifiers.size()));
				}
				if (subIdentifiers.size() == 1)
				{
					foreach_xmlnode (child, node)
					{
						result &= __skipVariable((*child), subIdentifiers.first());
					}
				}
				else
				{
					foreach_xmlnode (container, node)
					{
						foreach(Type::Identifier, it, subIdentifiers)
						{
							foreach_xmlnode(child, (*container))
							{
								result &= __skipVariable((*child), (*it));
							}
						}
					}
				}
			}
			return result;
		}

		void _load(hlxml::Node* node, hstr* value)
		{
			*value = node->pstr(VALUE);
		}

		void _load(hlxml::Node* node, hversion* value)
		{
			value->set(node->pstr(VALUE));
		}

		void _load(hlxml::Node* node, henum* value)
		{
			value->value = node->pint32(VALUE);
		}

		void _load(hlxml::Node* node, grectf* value)
		{
			harray<hstr> data = node->pstr(VALUE).split(SEPARATOR);
			value->x = (float)data[0];
			value->y = (float)data[1];
			value->w = (float)data[2];
			value->h = (float)data[3];
		}

		void _load(hlxml::Node* node, gvec2f* value)
		{
			harray<hstr> data = node->pstr(VALUE).split(SEPARATOR);
			value->x = (float)data[0];
			value->y = (float)data[1];
		}

		void _load(hlxml::Node* node, gvec3f* value)
		{
			harray<hstr> data = node->pstr(VALUE).split(SEPARATOR);
			value->x = (float)data[0];
			value->y = (float)data[1];
			value->z = (float)data[2];
		}

		void _load(hlxml::Node* node, grecti* value)
		{
			harray<hstr> data = node->pstr(VALUE).split(SEPARATOR);
			value->x = (int)data[0];
			value->y = (int)data[1];
			value->w = (int)data[2];
			value->h = (int)data[3];
		}

		void _load(hlxml::Node* node, gvec2i* value)
		{
			harray<hstr> data = node->pstr(VALUE).split(SEPARATOR);
			value->x = (int)data[0];
			value->y = (int)data[1];
		}

		void _load(hlxml::Node* node, gvec3i* value)
		{
			harray<hstr> data = node->pstr(VALUE).split(SEPARATOR);
			value->x = (int)data[0];
			value->y = (int)data[1];
			value->z = (int)data[2];
		}

		void _load(hlxml::Node* node, grectd* value)
		{
			harray<hstr> data = node->pstr(VALUE).split(SEPARATOR);
			value->x = (double)data[0];
			value->y = (double)data[1];
			value->w = (double)data[2];
			value->h = (double)data[3];
		}

		void _load(hlxml::Node* node, gvec2d* value)
		{
			harray<hstr> data = node->pstr(VALUE).split(SEPARATOR);
			value->x = (double)data[0];
			value->y = (double)data[1];
		}

		void _load(hlxml::Node* node, gvec3d* value)
		{
			harray<hstr> data = node->pstr(VALUE).split(SEPARATOR);
			value->x = (double)data[0];
			value->y = (double)data[1];
			value->z = (double)data[2];
		}

		void _load(hlxml::Node* node, Serializable* value)
		{
			__loadObject(node, &value);
		}

		void _load(hlxml::Node* node, Serializable** value)
		{
			__loadObject(node, value);
		}

		void __loadObject(hlxml::Node* node, Serializable** value)
		{
			if (node->name != "Object" && node->name != "Null")
			{
				if (node->children.size() == 0)
				{
					*value = NULL;
					return;
				}
				node = node->children.first();
			}
			if (node->name == "Null")
			{
				*value = NULL;
				return;
			}
			unsigned int id = 0;
			bool idExists = node->pexists(OBJECT_ID);
			if (idExists)
			{
				id = node->puint32(OBJECT_ID);
			}
			if (!idExists || !__tryGetObject(id, value))
			{
				hstr className = node->pstr("name");
				if (*value == NULL)
				{
					*value = Factory::create(className);
				}
				__tryMapObject(&id, *value);
				(*value)->_lsOnDeserializing();
				harray<Variable*> variables = (*value)->_lsVars();
				harray<hstr> missingVariableNames;
				Variable* variable = NULL;
				int variableIndex = -1;
				hstr variableName;
				Type::Identifier loadedIdentifier;
				int j = 0;
				while (j < node->children.size() && variables.size() > 0)
				{
					variableName = node->children[j]->pstr("name");
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
					loadedIdentifier = Type::Identifier::fromUint(node->children[j]->pstr("type").unhex());
					if (variable != NULL)
					{
						if (variable->type->identifier == loadedIdentifier)
						{
							__loadVariable(node->children[j], variable, loadedIdentifier);
						}
						else
						{
							if (!_isCompatibleType(variable->type->identifier, loadedIdentifier))
							{
								throw Exception(hsprintf("Variable type of '%s' has changed. Expected: %02X, Got: %02X", variable->name.cStr(), variable->type->identifier.value, loadedIdentifier.value));
							}
							hlog::warn(logTag, "Using compatible type for: " + variable->name);
							__loadVariableCompatible(node->children[j], variable, loadedIdentifier);
						}
						delete variables.removeAt(variableIndex);
					}
					else
					{
						missingVariableNames += variableName;
						__skipVariable(node->children[j], loadedIdentifier);
					}
					++j;
				}
				while (j < node->children.size())
				{
					variableName = node->children[j]->pstr("name");
					loadedIdentifier = Type::Identifier::fromUint(node->children[j]->pstr("type").unhex());
					missingVariableNames += variableName;
					__skipVariable(node->children[j], loadedIdentifier);
					++j;
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
				(*value)->_lsOnDeserialized();
			}
			else if (id == 0)
			{
				*value = NULL;
			}
		}

		bool __skipObject(hlxml::Node* node)
		{
			if (node->name != "Object")
			{
				node = node->children.first();
			}
			Serializable* dummy = NULL;
			unsigned int id = 0;
			bool idExists = node->pexists(OBJECT_ID);
			if (idExists)
			{
				id = node->puint32(OBJECT_ID);
			}
			if (!idExists || !__tryGetObject(id, &dummy))
			{
				__forceMapEmptyObject(); // required for proper indexing of later variables
				foreach_xmlnode (child, node)
				{
					__skipVariable((*child), Type::Identifier::fromUint((*child)->pstr("type").unhex()));
				}
			}
			return true;
		}

		void _loadHarray(hlxml::Node* node, harray<Serializable*>* value)
		{
			Serializable* object = NULL;
			foreach_xmlnode (child, node)
			{
				object = NULL;
				__loadObject((*child), &object);
				value->add(object);
			}
		}

#define DEFINE_LOAD_HARRAY(type, loadType) \
		void _loadHarray(hlxml::Node* node, harray<type>* value) \
		{ \
			foreach_xmlnode (child, node) \
			{ \
				value->add((*child)->p ## loadType(VALUE)); \
			} \
		}

#define DEFINE_LOAD_HARRAY_F(type) \
		void _loadHarray(hlxml::Node* node, harray<type>* value) \
		{ \
			type object; \
			foreach_xmlnode (child, node) \
			{ \
				_load((*child), &object); \
				value->add(object); \
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
		DEFINE_LOAD_HARRAY_F(hversion);
		DEFINE_LOAD_HARRAY_F(henum);
		DEFINE_LOAD_HARRAY_F(grectf);
		DEFINE_LOAD_HARRAY_F(gvec2f);
		DEFINE_LOAD_HARRAY_F(gvec3f);
		DEFINE_LOAD_HARRAY_F(grecti);
		DEFINE_LOAD_HARRAY_F(gvec2i);
		DEFINE_LOAD_HARRAY_F(gvec3i);
		DEFINE_LOAD_HARRAY_F(grectd);
		DEFINE_LOAD_HARRAY_F(gvec2d);
		DEFINE_LOAD_HARRAY_F(gvec3d);

	}

}
