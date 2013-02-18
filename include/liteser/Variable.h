/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents data about a variable definition and value.

#ifndef LITESER_VARIABLE_H
#define LITESER_VARIABLE_H

#include <stdint.h>

#include <hltypes/exception.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "liteser.h"
#include "liteserExport.h"
#include "Ptr.h"
#include "Type.h"

namespace liteser
{
	class Serializable;

	class liteserExport Variable
	{
	public:
		hstr name;
		Type* type;
		void* ptr;
		harray<Variable*> subVariables;

		Variable(chstr name, Ptr<char>* ptr); // char is always 8 bits
		Variable(chstr name, Ptr<unsigned char>* ptr); // char is always 8 bits
		Variable(chstr name, Ptr<int16_t>* ptr);
		Variable(chstr name, Ptr<uint16_t>* ptr);
		Variable(chstr name, Ptr<int32_t>* ptr);
		Variable(chstr name, Ptr<uint32_t>* ptr);
		Variable(chstr name, Ptr<float>* ptr);
		Variable(chstr name, Ptr<double>* ptr);
		Variable(chstr name, Ptr<bool>* ptr);
		Variable(chstr name, Ptr<hstr>* ptr);
		Variable(chstr name, Ptr<harray<char> >* ptr); // char is always 8 bits
		Variable(chstr name, Ptr<harray<unsigned char> >* ptr); // char is always 8 bits
		Variable(chstr name, Ptr<harray<int16_t> >* ptr);
		Variable(chstr name, Ptr<harray<uint16_t> >* ptr);
		Variable(chstr name, Ptr<harray<int32_t> >* ptr);
		Variable(chstr name, Ptr<harray<uint32_t> >* ptr);
		Variable(chstr name, Ptr<harray<float> >* ptr);
		Variable(chstr name, Ptr<harray<double> >* ptr);
		Variable(chstr name, Ptr<harray<hstr> >* ptr);

		template <class T>
		Variable(chstr name, Ptr<T*>* ptr)
		{
			this->name = name;
			this->type = new Type((Ptr<Serializable*>*)NULL);
			this->ptr = (void*)ptr;
			this->ptrKeys = NULL;
			this->ptrValues = NULL;
			// IMPORTANT NOTE: If you get C2440 on the line below, it means that the class does not inherit liteser::Serializable
			// or there is a forward declaration of the class instead of including the actual header.
			Ptr<Serializable>(*ptr->value);
		}
		template <class T>
		Variable(chstr name, Ptr<T>* ptr)
		{
			this->name = name;
			this->type = new Type((Ptr<Serializable>*)NULL);
			this->ptr = (void*)ptr;
			this->ptrKeys = NULL;
			this->ptrValues = NULL;
			// IMPORTANT NOTE: If you get C2440 on the line below, it means that the class does not inherit liteser::Serializable
			// or there is a forward declaration of the class instead of including the actual header.
			Ptr<Serializable>(ptr->value);
		}
		~Variable();

		template <class T>
		Variable(chstr name, Ptr<harray<T*> >* ptr)
		{
			this->name = name;
			this->type = new Type((Ptr<harray<Serializable*> >*)NULL);
			this->ptr = (void*)ptr;
			this->ptrKeys = NULL;
			this->ptrValues = NULL;
			harray<Serializable*>* obj = (harray<Serializable*>*)ptr->value;
			foreach (Serializable*, it, *obj)
			{
				this->subVariables += new Variable("", new Ptr<Serializable*>(&(*it)));
			}
		}
		template <class K, class V>
		Variable(chstr name, Ptr<hmap<K*, V> >* ptr)
		{
			this->name = name;
			this->type = new Type((Ptr<harray<Serializable*> >*)NULL);
			this->ptr = (void*)ptr;
			harray<K*> originalKeys = ptr->value->keys();
			harray<Serializable*>* keys = new harray<Serializable*>(originalKeys.cast<Serializable*>());
			harray<V>* values = new harray<V>();
			for_iter (i, 0, originalKeys.size()) // cannot use foreach here because GCC can't compile it properly
			{
				values->add(ptr->value->operator[](originalKeys[i]));
			}
			this->ptrKeys = keys;
			this->ptrValues = values;
			this->subVariables += new Variable("", new Ptr<harray<Serializable*> >(keys));
			this->subVariables += new Variable("", new Ptr<harray<V> >(values));
		}
		template <class K, class V>
		Variable(chstr name, Ptr<hmap<K, V> >* ptr)
		{
			this->name = name;
			this->type = new Type(ptr);
			this->ptr = (void*)ptr;
			harray<K>* keys = new harray<K>(ptr->value->keys());
			harray<V>* values = new harray<V>();
			for_iter (i, 0, keys->size()) // cannot use foreach here because GCC can't compile it properly
			{
				values->add(ptr->value->operator[](keys->operator[](i)));
			}
			this->ptrKeys = keys;
			this->ptrValues = values;
			this->subVariables += new Variable("", new Ptr<harray<K> >(keys));
			this->subVariables += new Variable("", new Ptr<harray<V> >(values));
		}

		template <class T>
		T* value()
		{
			return ((Ptr<T>*)this->ptr)->value;
		}

		void createSubVariables(Type::Value type, unsigned int size);
		void applyHmapSubVariables(Type::Value type);

	protected:
		void* ptrKeys;
		void* ptrValues;

		template <class T>
		void _addSubVariablesHarray(unsigned int size)
		{
			harray<T>* container = ((Ptr<harray<T> >*)this->ptr)->value;
			if (container->size() > 0)
			{
				throw hl_exception("Harray in default constructor not empty initially: " + this->name);
			}
			container->add(T(), size); // requires adding first because of possible reallocation of memory to another block
			for_itert (unsigned int, i, 0, size)
			{
				this->subVariables += new Variable("", new Ptr<T>(&container->operator[](i)));
			}
		}
		template <class K, class V>
		void _addSubVariablesHmap(unsigned int size)
		{
			hmap<K, V>* container = ((Ptr<hmap<K, V> >*)this->ptr)->value;
			if (container->size() > 0)
			{
				throw hl_exception("Hmap in default constructor not empty initially: " + this->name);
			}
		}
		template <class K, class V>
		void _applyHmapSubVariables()
		{
			hmap<K, V>* container = ((Ptr<hmap<K, V> >*)this->ptr)->value;
			harray<K>* keys = (harray<K>*)this->ptrKeys;
			harray<V>* values = (harray<V>*)this->ptrValues;
			for_iter (i, 0, keys->size())
			{
				container->operator[](keys->operator[](i)) = values->operator[](i);
			}
		}

	};

}
#endif
