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
		Ptr* ptr;
		harray<Variable*> subVariables;
		unsigned int containerSize;

		Variable(chstr name = "");
		~Variable();

		Variable* assign(VPtr<char>* ptr); // char is always 8 bits
		Variable* assign(VPtr<unsigned char>* ptr); // char is always 8 bits
		Variable* assign(VPtr<int16_t>* ptr);
		Variable* assign(VPtr<uint16_t>* ptr);
		Variable* assign(VPtr<int32_t>* ptr);
		Variable* assign(VPtr<uint32_t>* ptr);
		Variable* assign(VPtr<float>* ptr);
		Variable* assign(VPtr<double>* ptr);
		Variable* assign(VPtr<bool>* ptr);
		Variable* assign(VPtr<hstr>* ptr);
		Variable* assign(VPtr<grect>* ptr);
		Variable* assign(VPtr<gvec2>* ptr);
		Variable* assign(VPtr<gvec3>* ptr);
		Variable* assign(VPtr<harray<char> >* ptr); // char is always 8 bits
		Variable* assign(VPtr<harray<unsigned char> >* ptr); // char is always 8 bits
		Variable* assign(VPtr<harray<int16_t> >* ptr);
		Variable* assign(VPtr<harray<uint16_t> >* ptr);
		Variable* assign(VPtr<harray<int32_t> >* ptr);
		Variable* assign(VPtr<harray<uint32_t> >* ptr);
		Variable* assign(VPtr<harray<float> >* ptr);
		Variable* assign(VPtr<harray<double> >* ptr);
		Variable* assign(VPtr<harray<hstr> >* ptr);
		Variable* assign(VPtr<harray<grect> >* ptr);
		Variable* assign(VPtr<harray<gvec2> >* ptr);
		Variable* assign(VPtr<harray<gvec3> >* ptr);
		template <class T>
		Variable* assign(VPtr<T*>* ptr)
		{
			this->type->assign((VPtr<Serializable*>*)NULL);
			this->ptr = ptr;
			// IMPORTANT NOTE: If you get C2440 on the line below, it means that the class does not inherit liteser::Serializable
			// or there is a forward declaration of the class instead of including the actual header.
			VPtr<Serializable>(*ptr->value);
			return this;
		}
		template <class T>
		Variable* assign(VPtr<T>* ptr)
		{
			this->type->assign((VPtr<Serializable>*)NULL);
			this->ptr = ptr;
			// IMPORTANT NOTE: If you get C2440 on the line below, it means that the class does not inherit liteser::Serializable
			// or there is a forward declaration of the class instead of including the actual header.
			VPtr<Serializable>(ptr->value);
			return this;
		}
		template <class T>
		Variable* assign(VPtr<harray<T*> >* ptr)
		{
			this->type->assign((VPtr<harray<Serializable*> >*)NULL);
			this->ptr = ptr;
			this->containerSize = ptr->value->size();
			harray<Serializable*>* obj = (harray<Serializable*>*)ptr->value;
			foreach (Serializable*, it, *obj)
			{
				this->subVariables += (new Variable())->assign(new VPtr<Serializable*>(&(*it))); // due to optimization with static, assign has to be called last as it might invalidate the pointer
			}
			return this;
		}
		template <class K, class V>
		Variable* assign(VPtr<hmap<K*, V> >* ptr)
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
			this->ptrKeys = new DPtr(keys);
			this->ptrValues = new DPtr(values);
			this->subVariables += (new Variable())->assign(new VPtr<harray<Serializable*> >(keys)); // due to optimization with static, assign has to be called last as it might invalidate the pointer
			this->subVariables += (new Variable())->assign(new VPtr<harray<V> >(values)); // due to optimization with static, assign has to be called last as it might invalidate the pointer
			return this;
		}
		template <class K, class V>
		Variable* assign(VPtr<hmap<K, V> >* ptr)
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
			this->ptrKeys = new DPtr(keys);
			this->ptrValues = new DPtr(values);
			this->subVariables += (new Variable())->assign(new VPtr<harray<K> >(keys)); // due to optimization with static, assign has to be called last as it might invalidate the pointer
			this->subVariables += (new Variable())->assign(new VPtr<harray<V> >(values)); // due to optimization with static, assign has to be called last as it might invalidate the pointer
			return this;
		}

		template <class T>
		T* value()
		{
			return ((VPtr<T>*)this->ptr)->value;
		}

		void createSubVariables(Type::Value type);
		void applyHmapSubVariables(Type::Value type);

	protected:
		Ptr* ptrKeys;
		Ptr* ptrValues;

		template <class T>
		void _addSubVariablesHarray()
		{
			static harray<T>* container;
			container = ((VPtr<harray<T> >*)this->ptr)->value;
			if (container->size() > 0)
			{
				throw hl_exception("Harray in default constructor not empty initially: " + this->name);
			}
			container->add(T(), this->containerSize); // requires adding first because of possible reallocation of memory to another block
			for_itert (unsigned int, i, 0, this->containerSize)
			{
				this->subVariables += (new Variable())->assign(new VPtr<T>(&container->operator[](i)));
			}
		}
		template <class K, class V>
		void _addSubVariablesHmap()
		{
			if (((VPtr<hmap<K, V> >*)this->ptr)->value->size() > 0)
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
			container = ((VPtr<hmap<K, V> >*)this->ptr)->value;
			keys = ((DPtr<K>*)this->ptrKeys)->data;
			values = ((DPtr<V>*)this->ptrValues)->data;
			for_iter (i, 0, keys->size())
			{
				container->operator[](keys->operator[](i)) = values->operator[](i);
			}
		}

	};

}
#endif
