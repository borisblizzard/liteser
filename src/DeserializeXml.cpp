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
#include <hlxml/Node.h>

#include "DeserializeXml.h"
#include "Factory.h"
#include "liteser.h"
#include "Serializable.h"
#include "Type.h"
#include "Utility.h"
#include "Variable.h"

namespace liteser
{
	namespace xml
	{
		inline void __loadVariable(hlxml::Node* node, Variable* variable, Type::Value loadType)
		{
			switch (loadType)
			{
			case Type::INT8:		*variable->value<char>()			= node->pint("value");		break;
			case Type::UINT8:		*variable->value<unsigned char>()	= node->pint("value");		break;
			case Type::INT16:		*variable->value<short>()			= node->pint("value");		break;
			case Type::UINT16:		*variable->value<unsigned short>()	= node->pint("value");		break;
			case Type::INT32:		*variable->value<int>()				= node->pint("value");		break;
			case Type::UINT32:		*variable->value<unsigned int>()	= node->pint("value");		break;
			case Type::INT64:		*variable->value<int64_t>()			= node->pint("value");		break;
			case Type::UINT64:		*variable->value<uint64_t>()		= node->pint("value");		break;
			case Type::FLOAT:		*variable->value<float>()			= node->pfloat("value");	break;
			case Type::DOUBLE:		*variable->value<double>()			= node->pfloat("value");	break;
			case Type::BOOL:		*variable->value<bool>()			= node->pbool("value");		break;
			case Type::HSTR:		_load(node, variable->value<hstr>());							break;
			case Type::HVERSION:	_load(node, variable->value<hversion>());						break;
			case Type::HENUM:		_load(node, variable->value<henum>());							break;
			case Type::GRECT:		_load(node, variable->value<grect>());							break;
			case Type::GVEC2:		_load(node, variable->value<gvec2>());							break;
			case Type::GVEC3:		_load(node, variable->value<gvec3>());							break;
			case Type::OBJECT:		_load(node, variable->value<Serializable>());					break;
			case Type::OBJPTR:		_load(node, variable->value<Serializable*>());					break;
			case Type::HARRAY:		__loadContainer(node, variable, loadType);						break;
			case Type::HMAP:		__loadContainer(node, variable, loadType);						break;
			default:																				break;
			}
		}

		inline bool __skipVariable(hlxml::Node* node, Type::Value loadType)
		{
			switch (loadType)
			{
			case Type::OBJECT:		return __skipObject(node);
			case Type::OBJPTR:		return __skipObject(node);
			case Type::HARRAY:		return __skipContainer(node, loadType);
			case Type::HMAP:		return __skipContainer(node, loadType);
			default:				break;
			}
			return false;
		}

		void __loadContainer(hlxml::Node* node, Variable* variable, Type::Value type)
		{
			variable->containerSize = node->getChildCount();
			if (variable->containerSize > 0)
			{
				harray<Type::Value> loadTypes;
				harray<hstr> subTypes = node->pstr("sub_types").split(',', -1, true);
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
					if (loadTypes[i] != variable->type->subTypes[i]->value)
					{
						throw Exception(hsprintf("Variable type of '%s' has changed. Expected: %02X, Got: %02X", variable->name.cStr(), variable->type->value, loadTypes[i]));
					}
					if (loadTypes[i] == Type::HARRAY || loadTypes[i] == Type::HMAP)
					{
						throw Exception(hsprintf("Template container within a template container detected, not supported: %02X", loadTypes[i]));
					}
				}
				hlxml::Node* child = node->iterChildren();
				variable->createSubVariables(type);
				foreach (Variable*, it, variable->subVariables)
				{
					__loadVariable(child, (*it), (*it)->type->value);
					child = child->next();
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
				harray<hstr> types = node->pstr("sub_types").split(',', -1, true);
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
			*value = node->pstr("value");
		}

		void _load(hlxml::Node* node, hversion* value)
		{
			value->set(node->pstr("value"));
		}

		void _load(hlxml::Node* node, henum* value)
		{
			value->value = node->pint("value");
		}

		void _load(hlxml::Node* node, grect* value)
		{
			harray<hstr> data = node->pstr("value").split(',');
			value->x = (float)data[0];
			value->y = (float)data[1];
			value->w = (float)data[2];
			value->h = (float)data[3];
		}

		void _load(hlxml::Node* node, gvec2* value)
		{
			harray<hstr> data = node->pstr("value").split(',');
			value->x = (float)data[0];
			value->y = (float)data[1];
		}

		void _load(hlxml::Node* node, gvec3* value)
		{
			harray<hstr> data = node->pstr("value").split(',');
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
			if (node->value != "Object")
			{
				node = node->iterChildren();
			}
			unsigned int id = 0;
			bool idExists = node->pexists("id");
			if (idExists)
			{
				id = (unsigned int)node->pint("id");
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
				hstr variableName;
				Type::Value loadType;
				hlxml::Node* child = node->iterChildren();
				while (child != NULL && variables.size() > 0)
				{
					variableName = child->pstr("name");
					variable = NULL;
					foreach (Variable*, it, variables)
					{
						if ((*it)->name == variableName)
						{
							variable = (*it);
							break;
						}
					}
					loadType = (Type::Value)child->pstr("type").unhex();
					if (variable != NULL)
					{
						if (loadType != variable->type->value)
						{
							throw Exception(hsprintf("Variable type of '%s' has changed. Expected: %02X, Got: %02X", variable->name.cStr(), variable->type->value, loadType));
						}
						__loadVariable(child, variable, loadType);
						variables.remove(variable);
						delete variable;
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
			if (node->value != "Object")
			{
				node = node->iterChildren();
			}
			Serializable* dummy = NULL;
			unsigned int id = 0;
			bool idExists = node->pexists("id");
			if (idExists)
			{
				id = (unsigned int)node->pint("id");
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

		void _loadHarray(hlxml::Node* node, harray<char>* value)
		{
			foreach_xmlnode (child, node)
			{
				value->add((char)child->pint("value"));
			}
		}

		void _loadHarray(hlxml::Node* node, harray<unsigned char>* value)
		{
			foreach_xmlnode (child, node)
			{
				value->add((unsigned char)child->pint("value"));
			}
		}

#define DEFINE_LOAD_HARRAY(type, loadType) \
		void _loadHarray(hlxml::Node* node, harray<type>* value) \
		{ \
			foreach_xmlnode (child, node) \
			{ \
				value->add((type)child->pstr("value")); \
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

}
