/// @file
/// @version 2.7
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
		inline void __loadVariableCompatible(hlxml::Node* node, Variable* variable, Type::Value loadType)
		{
			// these are predefined compatible types only
			switch (loadType)
			{
			case Type::INT8:	variable->setValue(node->pint8(VALUE));		break;
			case Type::UINT8:	variable->setValue(node->puint8(VALUE));	break;
			case Type::INT16:	variable->setValue(node->pint16(VALUE));	break;
			case Type::UINT16:	variable->setValue(node->puint16(VALUE));	break;
			case Type::INT32:	variable->setValue(node->pint32(VALUE));	break;
			case Type::UINT32:	variable->setValue(node->puint32(VALUE));	break;
			case Type::INT64:	variable->setValue(node->pint64(VALUE));	break;
			case Type::UINT64:	variable->setValue(node->puint64(VALUE));	break;
			case Type::FLOAT:	variable->setValue(node->pfloat(VALUE));	break;
			case Type::DOUBLE:	variable->setValue(node->pdouble(VALUE));	break;
			default:														break;
			}
		}

		inline void __loadVariable(hlxml::Node* node, Variable* variable, Type::Value loadType)
		{
			switch (loadType)
			{
			case Type::INT8:		*variable->value<char>()			= node->pint8(VALUE);	break;
			case Type::UINT8:		*variable->value<unsigned char>()	= node->puint8(VALUE);	break;
			case Type::INT16:		*variable->value<short>()			= node->pint16(VALUE);	break;
			case Type::UINT16:		*variable->value<unsigned short>()	= node->puint16(VALUE);	break;
			case Type::INT32:		*variable->value<int>()				= node->pint32(VALUE);	break;
			case Type::UINT32:		*variable->value<unsigned int>()	= node->puint32(VALUE);	break;
			case Type::INT64:		*variable->value<int64_t>()			= node->pint64(VALUE);	break;
			case Type::UINT64:		*variable->value<uint64_t>()		= node->puint64(VALUE);	break;
			case Type::FLOAT:		*variable->value<float>()			= node->pfloat(VALUE);	break;
			case Type::DOUBLE:		*variable->value<double>()			= node->pdouble(VALUE);	break;
			case Type::BOOL:		*variable->value<bool>()			= node->pbool(VALUE);	break;
			case Type::HSTR:		_load(node, variable->value<hstr>());						break;
			case Type::HVERSION:	_load(node, variable->value<hversion>());					break;
			case Type::HENUM:		_load(node, variable->value<henum>());						break;
			case Type::GRECT:		_load(node, variable->value<grect>());						break;
			case Type::GVEC2:		_load(node, variable->value<gvec2>());						break;
			case Type::GVEC3:		_load(node, variable->value<gvec3>());						break;
			case Type::OBJECT:		_load(node, variable->value<Serializable>());				break;
			case Type::OBJPTR:		_load(node, variable->value<Serializable*>());				break;
			case Type::HARRAY:		__loadContainer(node, variable, loadType);					break;
			case Type::HMAP:		__loadContainer(node, variable, loadType);					break;
			default:																			break;
			}
		}

		inline bool __skipVariable(hlxml::Node* node, Type::Value loadType)
		{
			switch (loadType)
			{
			case Type::OBJECT:	return __skipObject(node);
			case Type::OBJPTR:	return __skipObject(node);
			case Type::HARRAY:	return __skipContainer(node, loadType);
			case Type::HMAP:	return __skipContainer(node, loadType);
			default:			break;
			}
			return false;
		}

		void __loadContainer(hlxml::Node* node, Variable* variable, Type::Value type)
		{
			variable->containerSize = node->getChildCount();
			if (variable->containerSize > 0)
			{
				harray<Type::Value> loadTypes;
				harray<hstr> subTypes = node->pstr(SUB_TYPES).split(SEPARATOR, -1, true);
				foreach (hstr, it, subTypes)
				{
					loadTypes += (Type::Value)(*it).unhex();
				}
				if (loadTypes.size() != variable->type->subTypes.size())
				{
					throw Exception(hsprintf("Number of types for container does not match. Expected: %d, Got: %d", variable->type->subTypes.size(), loadTypes.size()));
				}
				for_iter (i, 0, loadTypes.size())
				{
					if (loadTypes[i] == Type::HARRAY || loadTypes[i] == Type::HMAP)
					{
						throw Exception(hsprintf("Template container within a template container detected, not supported: %02X", loadTypes[i]));
					}
				}
				hlxml::Node* child = node->iterChildren();
				variable->createSubVariables(type);
				if (loadTypes.size() > 1) // if more than one load-type, the sub-variables contain the actual data
				{
					for_iter (i, 0, variable->subVariables.size())
					{
						__loadVariable(child, variable->subVariables[i], variable->subVariables[i]->type->value);
						child = child->next();
					}
				}
				else if (variable->subVariables.size() > 0)
				{
					if (variable->type->subTypes[0]->value == loadTypes[0])
					{
						for_iter (i, 0, variable->subVariables.size())
						{
							__loadVariable(child, variable->subVariables[i], loadTypes[0]);
							child = child->next();
						}
					}
					else
					{
						if (!_isCompatibleType(variable->type->subTypes[0]->value, loadTypes[0]))
						{
							throw Exception(hsprintf("Variable subtype of '%s' has changed. Expected: %02X, Got: %02X", variable->name.cStr(), variable->type->subTypes[0]->value, loadTypes[0]));
						}
						hlog::warn(logTag, "Using compatible subtype for: " + variable->subVariables[0]->name);
						for_iter (i, 0, variable->subVariables.size())
						{
							__loadVariableCompatible(child, variable->subVariables[i], loadTypes[0]);
							child = child->next();
						}
					}
				}
				if (type == Type::HMAP)
				{
					variable->applyHmapSubVariables(type);
				}
			}
		}

		bool __skipContainer(hlxml::Node* node, Type::Value type)
		{
			bool result = true;
			int containerSize = node->getChildCount();
			if (containerSize > 0)
			{
				int subTypesSize = (type == Type::HMAP ? 2 : 1);
				harray<Type::Value> subTypes;
				harray<hstr> types = node->pstr(SUB_TYPES).split(SEPARATOR, -1, true);
				foreach (hstr, it, types)
				{
					subTypes += (Type::Value)(*it).unhex();
				}
				if (subTypes.size() != subTypesSize)
				{
					throw Exception(hsprintf("Number of types for container does not match. Expected: %d, Got: %d", subTypesSize, subTypes.size()));
				}
				if (subTypes.size() == 1)
				{
					foreach_xmlnode (child, node)
					{
						result &= __skipVariable(child, subTypes.first());
					}
				}
				else
				{
					hlxml::Node* container = node->iterChildren();
					foreach (Type::Value, it, subTypes)
					{
						foreach_xmlnode (child, container)
						{
							result &= __skipVariable(child, (*it));
						}
						container = container->next();
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

		void _load(hlxml::Node* node, grect* value)
		{
			harray<hstr> data = node->pstr(VALUE).split(SEPARATOR);
			value->x = (float)data[0];
			value->y = (float)data[1];
			value->w = (float)data[2];
			value->h = (float)data[3];
		}

		void _load(hlxml::Node* node, gvec2* value)
		{
			harray<hstr> data = node->pstr(VALUE).split(SEPARATOR);
			value->x = (float)data[0];
			value->y = (float)data[1];
		}

		void _load(hlxml::Node* node, gvec3* value)
		{
			harray<hstr> data = node->pstr(VALUE).split(SEPARATOR);
			value->x = (float)data[0];
			value->y = (float)data[1];
			value->z = (float)data[2];
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
			if (node->name != "Object")
			{
				node = node->iterChildren();
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
				harray<Variable*> variables = (*value)->_lsVars();
				Variable* variable = NULL;
				int variableIndex = -1;
				hstr variableName;
				Type::Value loadType;
				hlxml::Node* child = node->iterChildren();
				while (child != NULL && variables.size() > 0)
				{
					variableName = child->pstr("name");
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
					loadType = (Type::Value)child->pstr("type").unhex();
					if (variable != NULL)
					{
						if (variable->type->value == loadType)
						{
							__loadVariable(child, variable, loadType);
						}
						else
						{
							if (!_isCompatibleType(variable->type->value, loadType))
							{
								throw Exception(hsprintf("Variable type of '%s' has changed. Expected: %02X, Got: %02X", variable->name.cStr(), variable->type->value, loadType));
							}
							hlog::warn(logTag, "Using compatible type for: " + variable->name);
							__loadVariableCompatible(child, variable, loadType);
						}
						delete variables.removeAt(variableIndex);
					}
					else
					{
						hlog::warn(logTag, "Could not find variable with name: " + variableName);
						__skipVariable(child, loadType);
					}
					child = child->next();
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

		bool __skipObject(hlxml::Node* node)
		{
			if (node->name != "Object")
			{
				node = node->iterChildren();
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
					__skipVariable(child, (Type::Value)child->pstr("type").unhex());
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
				__loadObject(child, &object);
				value->add(object);
			}
		}

#define DEFINE_LOAD_HARRAY(type, loadType) \
		void _loadHarray(hlxml::Node* node, harray<type>* value) \
		{ \
			foreach_xmlnode (child, node) \
			{ \
				value->add(child->p ## loadType(VALUE)); \
			} \
		}

#define DEFINE_LOAD_HARRAY_F(type) \
		void _loadHarray(hlxml::Node* node, harray<type>* value) \
		{ \
			type object; \
			foreach_xmlnode (child, node) \
			{ \
				_load(child, &object); \
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
		DEFINE_LOAD_HARRAY_F(grect);
		DEFINE_LOAD_HARRAY_F(gvec2);
		DEFINE_LOAD_HARRAY_F(gvec3);

	}

}
