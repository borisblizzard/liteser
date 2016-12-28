/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Represents data about a variable definition and value.

#ifndef LITESER_VARIABLE_H
#define LITESER_VARIABLE_H

#include <stdint.h>

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hexception.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>
#include <hltypes/hversion.h>

#include "liteser.h"
#include "liteserExport.h"
#include "Ptr.h"
#include "templates.h"
#include "Type.h"

namespace liteser
{
	class Serializable;

	class liteserExport Variable
	{
	public:
		hstr name;
		Type* type;
		Ptr* ptr;
		harray<Variable*> subVariables;
		unsigned int containerSize;

		Variable(chstr name = "");
		~Variable();

		Variable* assign(VPtr<char>* ptr); // char is always 8 bits
		Variable* assign(VPtr<unsigned char>* ptr); // unsigned char is always 8 bits
		Variable* assign(VPtr<short>* ptr);
		Variable* assign(VPtr<unsigned short>* ptr);
		Variable* assign(VPtr<int>* ptr);
		Variable* assign(VPtr<unsigned int>* ptr);
		Variable* assign(VPtr<int64_t>* ptr);
		Variable* assign(VPtr<uint64_t>* ptr);
		Variable* assign(VPtr<float>* ptr);
		Variable* assign(VPtr<double>* ptr);
		Variable* assign(VPtr<bool>* ptr);
		Variable* assign(VPtr<hstr>* ptr);
		Variable* assign(VPtr<hversion>* ptr);
		Variable* assign(VPtr<grect>* ptr);
		Variable* assign(VPtr<gvec2>* ptr);
		Variable* assign(VPtr<gvec3>* ptr);
		Variable* assign(VPtr<harray<char> >* ptr);
		Variable* assign(VPtr<harray<unsigned char> >* ptr);
		Variable* assign(VPtr<harray<short> >* ptr);
		Variable* assign(VPtr<harray<unsigned short> >* ptr);
		Variable* assign(VPtr<harray<int> >* ptr);
		Variable* assign(VPtr<harray<unsigned int> >* ptr);
		Variable* assign(VPtr<harray<int64_t> >* ptr);
		Variable* assign(VPtr<harray<uint64_t> >* ptr);
		Variable* assign(VPtr<harray<float> >* ptr);
		Variable* assign(VPtr<harray<double> >* ptr);
		Variable* assign(VPtr<harray<hstr> >* ptr);
		Variable* assign(VPtr<harray<hversion> >* ptr);
		Variable* assign(VPtr<harray<grect> >* ptr);
		Variable* assign(VPtr<harray<gvec2> >* ptr);
		Variable* assign(VPtr<harray<gvec3> >* ptr);
		template <typename T>
		inline typename __LS_ENABLE_IF<__LS_IS_BASE_OF<T, henum>::value, Variable*>::type assign(VPtr<T>* ptr)
		{
			this->type->assign((VPtr<henum>*)NULL);
			this->ptr = ptr;
			return this;
		}
		template <class T>
		inline typename __LS_ENABLE_IF<__LS_IS_BASE_OF<T, Serializable>::value, Variable*>::type assign(VPtr<T>* ptr)
		{
			this->type->assign((VPtr<Serializable>*)NULL);
			this->ptr = ptr;
			return this;
		}
		template <typename T>
		inline typename __LS_ENABLE_IF<__LS_IS_BASE_OF<T, Serializable>::value, Variable*>::type assign(VPtr<T*>* ptr)
		{
			this->type->assign((VPtr<Serializable*>*)NULL);
			this->ptr = ptr;
			return this;
		}
		template <typename T>
		inline typename __LS_ENABLE_IF<__LS_IS_BASE_OF<T, henum>::value, Variable*>::type assign(VPtr<harray<T> >* ptr)
		{
			this->type->assign((VPtr<harray<henum> >*)NULL);
			this->ptr = ptr;
			this->containerSize = ptr->value->size();
			harray<henum>* obj = (harray<henum>*)ptr->value;
			foreach (henum, it, *obj)
			{
				this->subVariables += (new Variable())->assign(new VPtr<henum>(&(*it)));
			}
			return this;
		}
		template <typename T>
		inline typename __LS_ENABLE_IF<__LS_IS_BASE_OF<T, Serializable>::value, Variable*>::type assign(VPtr<harray<T*> >* ptr)
		{
			this->type->assign((VPtr<harray<Serializable*> >*)NULL);
			this->ptr = ptr;
			this->containerSize = ptr->value->size();
			harray<Serializable*>* obj = (harray<Serializable*>*)ptr->value;
			foreach (Serializable*, it, *obj)
			{
				this->subVariables += (new Variable())->assign(new VPtr<Serializable*>(&(*it)));
			}
			return this;
		}
		template <typename K, typename V>
		inline typename __LS_ENABLE_IF<__LS_IS_BASE_OF<K, Serializable>::value, Variable*>::type assign(VPtr<hmap<K*, V> >* ptr)
		{
			this->type->assign((VPtr<harray<Serializable*> >*)NULL);
			this->ptr = ptr;
			this->containerSize = ptr->value->size();
			harray<K*> originalKeys = ptr->value->keys();
			harray<Serializable*>* keys = new harray<Serializable*>(originalKeys.template cast<Serializable*>());
			harray<V>* values = new harray<V>();
			for_iter (i, 0, originalKeys.size()) // cannot use foreach here because GCC can't compile it properly
			{
				values->add(ptr->value->operator[](originalKeys[i]));
			}
			this->ptrKeys = new CPtr<Serializable*>(keys);
			this->ptrValues = new CPtr<V>(values);
			this->subVariables += (new Variable())->assign(new VPtr<harray<Serializable*> >(keys));
			this->subVariables += (new Variable())->assign(new VPtr<harray<V> >(values));
			return this;
		}
		template <typename K, typename V>
		inline Variable* assign(VPtr<hmap<K, V> >* ptr)
		{
			this->type->assign(ptr);
			this->ptr = ptr;
			this->containerSize = ptr->value->size();
			harray<K>* keys = new harray<K>(ptr->value->keys());
			harray<V>* values = new harray<V>();
			for_iter (i, 0, keys->size()) // cannot use foreach here because GCC can't compile it properly
			{
				values->add(ptr->value->operator[](keys->operator[](i)));
			}
			this->ptrKeys = new CPtr<K>(keys);
			this->ptrValues = new CPtr<V>(values);
			this->subVariables += (new Variable())->assign(new VPtr<harray<K> >(keys));
			this->subVariables += (new Variable())->assign(new VPtr<harray<V> >(values));
			return this;
		}

		template <typename T>
		inline T* value()
		{
			return ((VPtr<T>*)this->ptr)->value;
		}

		template <typename T>
		inline void setValue(const T& value)
		{
			// this is from an internal list of possible compatible types
			switch (this->type->value)
			{
			case Type::INT8:	*((VPtr<char>*)this->ptr)->value			= (char)value;				break;
			case Type::UINT8:	*((VPtr<unsigned char>*)this->ptr)->value	= (unsigned char)value;		break;
			case Type::INT16:	*((VPtr<short>*)this->ptr)->value			= (short)value;				break;
			case Type::UINT16:	*((VPtr<unsigned short>*)this->ptr)->value	= (unsigned short)value;	break;
			case Type::INT32:	*((VPtr<int>*)this->ptr)->value				= (int)value;				break;
			case Type::UINT32:	*((VPtr<unsigned int>*)this->ptr)->value	= (unsigned int)value;		break;
			case Type::INT64:	*((VPtr<int64_t>*)this->ptr)->value			= (int64_t)value;			break;
			case Type::UINT64:	*((VPtr<uint64_t>*)this->ptr)->value		= (uint64_t)value;			break;
			case Type::FLOAT:	*((VPtr<float>*)this->ptr)->value			= (float)value;				break;
			case Type::DOUBLE:	*((VPtr<double>*)this->ptr)->value			= (double)value;			break;
			default:																					break;
			}
		}

		void createSubVariables(Type::Value type);
		void applyHmapSubVariables(Type::Value type);

	protected:
		Ptr* ptrKeys;
		Ptr* ptrValues;

		template <typename S>
		inline void _addSubVariablesHarray()
		{
			harray<S>* container = ((VPtr<harray<S> >*)this->ptr)->value;
			if (container->size() > 0)
			{
				throw Exception("harray in default constructor not empty initially: " + this->name);
			}
			container->add(S(), this->containerSize); // requires adding first because of possible reallocation of memory to another block
			for_itert (unsigned int, i, 0, this->containerSize)
			{
				this->subVariables += (new Variable())->assign(new VPtr<S>(&container->operator[](i)));
			}
		}
		template <typename key>
		inline void _addSubVariablesHmapKey(Type::Value value)
		{
			switch (value)
			{
			case Type::INT8:		this->_addSubVariablesHmap<key, char>();			break;
			case Type::UINT8:		this->_addSubVariablesHmap<key, unsigned char>();	break;
			case Type::INT16:		this->_addSubVariablesHmap<key, short>();			break;
			case Type::UINT16:		this->_addSubVariablesHmap<key, unsigned short>();	break;
			case Type::INT32:		this->_addSubVariablesHmap<key, int>();				break;
			case Type::UINT32:		this->_addSubVariablesHmap<key, unsigned int>();	break;
			case Type::INT64:		this->_addSubVariablesHmap<key, int64_t>();			break;
			case Type::UINT64:		this->_addSubVariablesHmap<key, uint64_t>();		break;
			case Type::FLOAT:		this->_addSubVariablesHmap<key, float>();			break;
			case Type::DOUBLE:		this->_addSubVariablesHmap<key, double>();			break;
			case Type::HSTR:		this->_addSubVariablesHmap<key, hstr>();			break;
			case Type::HVERSION:	this->_addSubVariablesHmap<key, hversion>();		break;
			case Type::HENUM:		this->_addSubVariablesHmap<key, henum>();			break;
			case Type::GRECT:		this->_addSubVariablesHmap<key, grect>();			break;
			case Type::GVEC2:		this->_addSubVariablesHmap<key, gvec2>();			break;
			case Type::GVEC3:		this->_addSubVariablesHmap<key, gvec3>();			break;
			case Type::OBJPTR:		this->_addSubVariablesHmap<key, Serializable*>();	break;
			case Type::NONE:		throw Exception("hmap value cannot be NONE!");		break;
			case Type::BOOL:		throw Exception("hmap value cannot be BOOL!");		break;
			case Type::OBJECT:		throw Exception("hmap value cannot be OBJECT!");	break;
			case Type::HARRAY:		throw Exception("hmap value cannot be HARRAY!");	break;
			case Type::HMAP:		throw Exception("hmap value cannot be HMAP!");		break;
			default:																	break;
			}
		}
		template <typename K, typename V>
		inline void _addSubVariablesHmap()
		{
			if (((VPtr<hmap<K, V> >*)this->ptr)->value->size() > 0)
			{
				throw Exception("hmap in default constructor not empty initially: " + this->name);
			}
		}
		template <typename keyType>
		inline void _applyHmapSubVariablesKey(Type::Value value)
		{
			switch (value)
			{
			case Type::INT8:		this->_applyHmapSubVariables<keyType, char>();				break;
			case Type::UINT8:		this->_applyHmapSubVariables<keyType, unsigned char>();		break;
			case Type::INT16:		this->_applyHmapSubVariables<keyType, short>();				break;
			case Type::UINT16:		this->_applyHmapSubVariables<keyType, unsigned short>();	break;
			case Type::INT32:		this->_applyHmapSubVariables<keyType, int>();				break;
			case Type::UINT32:		this->_applyHmapSubVariables<keyType, unsigned int>();		break;
			case Type::INT64:		this->_applyHmapSubVariables<keyType, int64_t>();			break;
			case Type::UINT64:		this->_applyHmapSubVariables<keyType, uint64_t>();			break;
			case Type::FLOAT:		this->_applyHmapSubVariables<keyType, float>();				break;
			case Type::DOUBLE:		this->_applyHmapSubVariables<keyType, double>();			break;
			case Type::HSTR:		this->_applyHmapSubVariables<keyType, hstr>();				break;
			case Type::HVERSION:	this->_applyHmapSubVariables<keyType, hversion>();			break;
			case Type::HENUM:		this->_applyHmapSubVariables<keyType, henum>();				break;
			case Type::GRECT:		this->_applyHmapSubVariables<keyType, grect>();				break;
			case Type::GVEC2:		this->_applyHmapSubVariables<keyType, gvec2>();				break;
			case Type::GVEC3:		this->_applyHmapSubVariables<keyType, gvec3>();				break;
			case Type::OBJPTR:		this->_applyHmapSubVariables<keyType, Serializable*>();		break;
			case Type::NONE:		throw Exception("hmap value cannot be NONE!");				break;
			case Type::BOOL:		throw Exception("hmap value cannot be BOOL!");				break;
			case Type::OBJECT:		throw Exception("hmap value cannot be OBJECT!");			break;
			case Type::HARRAY:		throw Exception("hmap value cannot be HARRAY!");			break;
			case Type::HMAP:		throw Exception("hmap value cannot be HMAP!");				break;
			default:																			break;
			}
		}
		template <typename K, typename V>
		inline void _applyHmapSubVariables()
		{
			hmap<K, V>* container = ((VPtr<hmap<K, V> >*)this->ptr)->value;
			harray<K>* keys = ((CPtr<K>*)this->ptrKeys)->data;
			harray<V>* values = ((CPtr<V>*)this->ptrValues)->data;
			for_iter (i, 0, keys->size())
			{
				container->operator[](keys->operator[](i)) = values->operator[](i);
			}
		}

	};

}
#endif
