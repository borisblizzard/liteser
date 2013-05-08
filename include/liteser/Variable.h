/// @file
/// @author  Boris Mikic
/// @version 2.1
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

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
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
		unsigned int containerSize;

		Variable(chstr name = "");
		~Variable();

		Variable* assign(Ptr<char>* ptr); // char is always 8 bits
		Variable* assign(Ptr<unsigned char>* ptr); // char is always 8 bits
		Variable* assign(Ptr<int16_t>* ptr);
		Variable* assign(Ptr<uint16_t>* ptr);
		Variable* assign(Ptr<int32_t>* ptr);
		Variable* assign(Ptr<uint32_t>* ptr);
		Variable* assign(Ptr<float>* ptr);
		Variable* assign(Ptr<double>* ptr);
		Variable* assign(Ptr<bool>* ptr);
		Variable* assign(Ptr<hstr>* ptr);
		Variable* assign(Ptr<grect>* ptr);
		Variable* assign(Ptr<gvec2>* ptr);
		Variable* assign(Ptr<gvec3>* ptr);
		Variable* assign(Ptr<harray<char> >* ptr); // char is always 8 bits
		Variable* assign(Ptr<harray<unsigned char> >* ptr); // char is always 8 bits
		Variable* assign(Ptr<harray<int16_t> >* ptr);
		Variable* assign(Ptr<harray<uint16_t> >* ptr);
		Variable* assign(Ptr<harray<int32_t> >* ptr);
		Variable* assign(Ptr<harray<uint32_t> >* ptr);
		Variable* assign(Ptr<harray<float> >* ptr);
		Variable* assign(Ptr<harray<double> >* ptr);
		Variable* assign(Ptr<harray<hstr> >* ptr);
		Variable* assign(Ptr<harray<grect> >* ptr);
		Variable* assign(Ptr<harray<gvec2> >* ptr);
		Variable* assign(Ptr<harray<gvec3> >* ptr);
		template <class T>
		Variable* assign(Ptr<T*>* ptr)
		{
			this->type->assign((Ptr<Serializable*>*)NULL);
			this->ptr = (void*)ptr;
			// IMPORTANT NOTE: If you get C2440 on the line below, it means that the class does not inherit liteser::Serializable
			// or there is a forward declaration of the class instead of including the actual header.
			Ptr<Serializable>(*ptr->value);
			return this;
		}
		template <class T>
		Variable* assign(Ptr<T>* ptr)
		{
			this->type->assign((Ptr<Serializable>*)NULL);
			this->ptr = (void*)ptr;
			// IMPORTANT NOTE: If you get C2440 on the line below, it means that the class does not inherit liteser::Serializable
			// or there is a forward declaration of the class instead of including the actual header.
			Ptr<Serializable>(ptr->value);
			return this;
		}
		template <class T>
		Variable* assign(Ptr<harray<T*> >* ptr)
		{
			this->type->assign((Ptr<harray<Serializable*> >*)NULL);
			this->ptr = (void*)ptr;
			this->containerSize = ptr->value->size();
			harray<Serializable*>* obj = (harray<Serializable*>*)ptr->value;
			foreach (Serializable*, it, *obj)
			{
				this->subVariables += (new Variable())->assign(new Ptr<Serializable*>(&(*it))); // due to optimization with static, assign has to be called last as it might invalidate the pointer
			}
			return this;
		}
		template <class K, class V>
		Variable* assign(Ptr<hmap<K*, V> >* ptr)
		{
			this->type->assign((Ptr<harray<Serializable*> >*)NULL);
			this->ptr = (void*)ptr;
			this->containerSize = ptr->value->size();
			harray<K*> originalKeys = ptr->value->keys();
			harray<Serializable*>* keys = new harray<Serializable*>(originalKeys.template cast<Serializable*>());
			harray<V>* values = new harray<V>();
			for_iter (i, 0, originalKeys.size()) // cannot use foreach here because GCC can't compile it properly
			{
				values->add(ptr->value->operator[](originalKeys[i]));
			}
			this->ptrKeys = keys;
			this->ptrValues = values;
			this->subVariables += (new Variable())->assign(new Ptr<harray<Serializable*> >(keys)); // due to optimization with static, assign has to be called last as it might invalidate the pointer
			this->subVariables += (new Variable())->assign(new Ptr<harray<V> >(values)); // due to optimization with static, assign has to be called last as it might invalidate the pointer
			return this;
		}
		template <class K, class V>
		Variable* assign(Ptr<hmap<K, V> >* ptr)
		{
			this->type->assign(ptr);
			this->ptr = (void*)ptr;
			this->containerSize = ptr->value->size();
			harray<K>* keys = new harray<K>(ptr->value->keys());
			harray<V>* values = new harray<V>();
			for_iter (i, 0, keys->size()) // cannot use foreach here because GCC can't compile it properly
			{
				values->add(ptr->value->operator[](keys->operator[](i)));
			}
			this->ptrKeys = keys;
			this->ptrValues = values;
			this->subVariables += (new Variable())->assign(new Ptr<harray<K> >(keys)); // due to optimization with static, assign has to be called last as it might invalidate the pointer
			this->subVariables += (new Variable())->assign(new Ptr<harray<V> >(values)); // due to optimization with static, assign has to be called last as it might invalidate the pointer
			return this;
		}

		template <class T>
		T* value()
		{
			return ((Ptr<T>*)this->ptr)->value;
		}

		void createSubVariables(Type::Value type);
		void applyHmapSubVariables(Type::Value type);

	protected:
		void* ptrKeys;
		void* ptrValues;

		template <class T>
		void _addSubVariablesHarray()
		{
			static harray<T>* container;
			container = ((Ptr<harray<T> >*)this->ptr)->value;
			if (container->size() > 0)
			{
				throw hl_exception("Harray in default constructor not empty initially: " + this->name);
			}
			container->add(T(), this->containerSize); // requires adding first because of possible reallocation of memory to another block
			for_itert (unsigned int, i, 0, this->containerSize)
			{
				this->subVariables += (new Variable())->assign(new Ptr<T>(&container->operator[](i)));
			}
		}
		template <class K, class V>
		void _addSubVariablesHmap()
		{
			if (((Ptr<hmap<K, V> >*)this->ptr)->value->size() > 0)
			{
				throw hl_exception("Hmap in default constructor not empty initially: " + this->name);
			}
		}
		template <class K, class V>
		void _applyHmapSubVariables()
		{
			static hmap<K, V>* container;
			static harray<K>* keys;
			static harray<V>* values;
			container = ((Ptr<hmap<K, V> >*)this->ptr)->value;
			keys = (harray<K>*)this->ptrKeys;
			values = (harray<V>*)this->ptrValues;
			for_iter (i, 0, keys->size())
			{
				container->operator[](keys->operator[](i)) = values->operator[](i);
			}
		}

	};

}
#endif
