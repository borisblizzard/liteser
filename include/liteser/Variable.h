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
			this->ptr = (void*)ptr;
			this->type = new Type((Ptr<Serializable*>*)NULL);
			// IMPORTANT NOTE: If you get C2440 on the line below, it means that the class does not inherit liteser::Serializable.
			Ptr<Serializable>(*ptr->value);
		}
		template <class T>
		Variable(chstr name, Ptr<T>* ptr)
		{
			this->name = name;
			this->ptr = (void*)ptr;
			this->type = new Type((Ptr<Serializable>*)NULL);
			// IMPORTANT NOTE: If you get C2440 on the line below, it means that the class does not inherit liteser::Serializable.
			Ptr<Serializable>(ptr->value);
		}
		~Variable();

		template <class T>
		Variable(chstr name, Ptr<harray<T*> >* ptr)
		{
			this->name = name;
			this->ptr = (void*)ptr;
			this->type = new Type((Ptr<harray<Serializable*> >*)NULL);
			harray<Serializable*>* obj = (harray<Serializable*>*)ptr->value;
			foreach (Serializable*, it, *obj)
			{
				this->subVariables += new Variable("", new Ptr<Serializable*>(&(*it)));
			}
		}
		template <class T>
		Variable(chstr name, Ptr<harray<T> >* ptr)
		{
			throw hl_exception("");
			this->name = name;
			this->ptr = (void*)ptr;
			this->type = new Type((Ptr<harray<Serializable> >*)NULL);
			harray<Serializable>* obj = (harray<Serializable>*)ptr->value;
			foreach (Serializable, it, *obj)
			{
				this->subVariables += new Variable("", new Ptr<Serializable>(&(*it)));
			}
		}
		/*
		template <class K, class V>
		Variable(chstr name, Ptr<hmap<K, V> >* ptr)
		{
			this->name = name;
			this->type = new Type(ptr);
			this->ptr = (void*)ptr;
			foreach_map (K, V, it, *ptr->value)
			{
				this->subVariables += new Variable("", new Ptr<K>(&it->first));
				this->subVariables += new Variable("", new Ptr<V>(&it->second));
			}
		}
		*/

		template <class T>
		T* value()
		{
			return ((Ptr<T>*)this->ptr)->value;
		}

		void createSubVariables(unsigned int size, Type::Value type);

	protected:
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
			/*
		template <class K, class V>
		void _addSubVariablesHmap(unsigned int size)
		{
			std::map<K, V>* container = (std::map<K, V>*)((Ptr<hmap<K, V> >*)this->ptr)->value;
			if (container->size() > 0)
			{
				throw hl_exception("Hmap in default constructor not empty initially: " + this->name);
			}
			for_itert (unsigned int, i, 0, size) // requires adding first because of possible reallocation of memory to another block
			{
				container->insert(std::pair<K, V>(K(), V()));
			}
			for (std::map<K, V>::iterator it = container->begin(); it != container->end(); it++)
			{
				this->subVariables += new Variable("", new Ptr<K>(&it->first));
				this->subVariables += new Variable("", new Ptr<V>(&it->second));
			}
		}
			*/

	};

}
#endif
