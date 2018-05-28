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
		Variable* assign(VPtr<grectf>* ptr);
		Variable* assign(VPtr<gvec2f>* ptr);
		Variable* assign(VPtr<gvec3f>* ptr);
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
		Variable* assign(VPtr<harray<grectf> >* ptr);
		Variable* assign(VPtr<harray<gvec2f> >* ptr);
		Variable* assign(VPtr<harray<gvec3f> >* ptr);
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
			this->type->template assign<Serializable*, V>(NULL);
			this->ptr = ptr;
			this->containerSize = ptr->value->size();
			harray<K*>& originalKeys = ptr->value->keys();
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
		inline typename __LS_ENABLE_IF<__LS_IS_BASE_OF<V, Serializable>::value, Variable*>::type assign(VPtr<hmap<K, V*> >* ptr)
		{
			this->type->template assign<K, Serializable*>(NULL);
			this->ptr = ptr;
			this->containerSize = ptr->value->size();
			harray<K>* keys = new harray<K>(ptr->value->keys());
			harray<Serializable*>* values = new harray<Serializable*>();
			for_iter (i, 0, keys->size()) // cannot use foreach here because GCC can't compile it properly
			{
				values->add((Serializable*)ptr->value->operator[](keys->operator[](i)));
			}
			this->ptrKeys = new CPtr<K>(keys);
			this->ptrValues = new CPtr<Serializable*>(values);
			this->subVariables += (new Variable())->assign(new VPtr<harray<K> >(keys));
			this->subVariables += (new Variable())->assign(new VPtr<harray<Serializable*> >(values));
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
			if (this->type->identifier == Type::Identifier::Int8)			*((VPtr<char>*)this->ptr)->value			= (char)value;
			else if (this->type->identifier == Type::Identifier::UInt8)		*((VPtr<unsigned char>*)this->ptr)->value	= (unsigned char)value;
			else if (this->type->identifier == Type::Identifier::Int16)		*((VPtr<short>*)this->ptr)->value			= (short)value;
			else if (this->type->identifier == Type::Identifier::UInt16)	*((VPtr<unsigned short>*)this->ptr)->value	= (unsigned short)value;
			else if (this->type->identifier == Type::Identifier::Int32)		*((VPtr<int>*)this->ptr)->value				= (int)value;
			else if (this->type->identifier == Type::Identifier::UInt32)	*((VPtr<unsigned int>*)this->ptr)->value	= (unsigned int)value;
			else if (this->type->identifier == Type::Identifier::Int64)		*((VPtr<int64_t>*)this->ptr)->value			= (int64_t)value;
			else if (this->type->identifier == Type::Identifier::UInt64)	*((VPtr<uint64_t>*)this->ptr)->value		= (uint64_t)value;
			else if (this->type->identifier == Type::Identifier::Float)		*((VPtr<float>*)this->ptr)->value			= (float)value;
			else if (this->type->identifier == Type::Identifier::Double)	*((VPtr<double>*)this->ptr)->value			= (double)value;
		}

		void createSubVariables(Type::Identifier identifier);
		void applyHmapSubVariables(Type::Identifier identifier);

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
		inline void _addSubVariablesHmapKey(Type::Identifier identifier)
		{
			if (identifier == Type::Identifier::Int8)				this->_addSubVariablesHmap<key, char>();
			else if (identifier == Type::Identifier::UInt8)			this->_addSubVariablesHmap<key, unsigned char>();
			else if (identifier == Type::Identifier::Int16)			this->_addSubVariablesHmap<key, short>();
			else if (identifier == Type::Identifier::UInt16)		this->_addSubVariablesHmap<key, unsigned short>();
			else if (identifier == Type::Identifier::Int32)			this->_addSubVariablesHmap<key, int>();
			else if (identifier == Type::Identifier::UInt32)		this->_addSubVariablesHmap<key, unsigned int>();
			else if (identifier == Type::Identifier::Int64)			this->_addSubVariablesHmap<key, int64_t>();
			else if (identifier == Type::Identifier::UInt64)		this->_addSubVariablesHmap<key, uint64_t>();
			else if (identifier == Type::Identifier::Float)			this->_addSubVariablesHmap<key, float>();
			else if (identifier == Type::Identifier::Double)		this->_addSubVariablesHmap<key, double>();
			else if (identifier == Type::Identifier::Hstr)			this->_addSubVariablesHmap<key, hstr>();
			else if (identifier == Type::Identifier::Hversion)		this->_addSubVariablesHmap<key, hversion>();
			else if (identifier == Type::Identifier::Henum)			this->_addSubVariablesHmap<key, henum>();
			else if (identifier == Type::Identifier::Grect)			this->_addSubVariablesHmap<key, grectf>();
			else if (identifier == Type::Identifier::Gvec2)			this->_addSubVariablesHmap<key, gvec2f>();
			else if (identifier == Type::Identifier::Gvec3)			this->_addSubVariablesHmap<key, gvec3f>();
			else if (identifier == Type::Identifier::Object)		this->_addSubVariablesHmap<key, Serializable*>();
			else if (identifier == Type::Identifier::None)			throw Exception("hmap value cannot be None!");
			else if (identifier == Type::Identifier::Bool)			throw Exception("hmap value cannot be Bool!");
			else if (identifier == Type::Identifier::ValueObject)	throw Exception("hmap value cannot be ValueObject!");
			else if (identifier == Type::Identifier::Harray)		throw Exception("hmap value cannot be Harray!");
			else if (identifier == Type::Identifier::Hmap)			throw Exception("hmap value cannot be Hmap!");
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
		inline void _applyHmapSubVariablesKey(Type::Identifier identifier)
		{
			if (identifier == Type::Identifier::Int8)				this->_applyHmapSubVariables<keyType, char>();
			else if (identifier == Type::Identifier::UInt8)			this->_applyHmapSubVariables<keyType, unsigned char>();
			else if (identifier == Type::Identifier::Int16)			this->_applyHmapSubVariables<keyType, short>();
			else if (identifier == Type::Identifier::UInt16)		this->_applyHmapSubVariables<keyType, unsigned short>();
			else if (identifier == Type::Identifier::Int32)			this->_applyHmapSubVariables<keyType, int>();
			else if (identifier == Type::Identifier::UInt32)		this->_applyHmapSubVariables<keyType, unsigned int>();
			else if (identifier == Type::Identifier::Int64)			this->_applyHmapSubVariables<keyType, int64_t>();
			else if (identifier == Type::Identifier::UInt64)		this->_applyHmapSubVariables<keyType, uint64_t>();
			else if (identifier == Type::Identifier::Float)			this->_applyHmapSubVariables<keyType, float>();
			else if (identifier == Type::Identifier::Double)		this->_applyHmapSubVariables<keyType, double>();
			else if (identifier == Type::Identifier::Hstr)			this->_applyHmapSubVariables<keyType, hstr>();
			else if (identifier == Type::Identifier::Hversion)		this->_applyHmapSubVariables<keyType, hversion>();
			else if (identifier == Type::Identifier::Henum)			this->_applyHmapSubVariables<keyType, henum>();
			else if (identifier == Type::Identifier::Grect)			this->_applyHmapSubVariables<keyType, grectf>();
			else if (identifier == Type::Identifier::Gvec2)			this->_applyHmapSubVariables<keyType, gvec2f>();
			else if (identifier == Type::Identifier::Gvec3)			this->_applyHmapSubVariables<keyType, gvec3f>();
			else if (identifier == Type::Identifier::Object)		this->_applyHmapSubVariables<keyType, Serializable*>();
			else if (identifier == Type::Identifier::None)			throw Exception("hmap value cannot be None!");
			else if (identifier == Type::Identifier::Bool)			throw Exception("hmap value cannot be Bool!");
			else if (identifier == Type::Identifier::ValueObject)	throw Exception("hmap value cannot be ValueObject!");
			else if (identifier == Type::Identifier::Harray)		throw Exception("hmap value cannot be Harray!");
			else if (identifier == Type::Identifier::Hmap)			throw Exception("hmap value cannot be Hmap!");
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
