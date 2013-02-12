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

#include <hltypes/harray.h>
#include <hltypes/hdeque.h>
#include <hltypes/hlist.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

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

		template <class T>
		Variable(chstr name, Ptr<T*>* ptr)
		{
			this->name = name;
			Serializable* obj = dynamic_cast<Serializable*>(*ptr->value);
			Ptr<Serializable*>* tempPtr = new Ptr<Serializable*>((Serializable**)ptr->value);
			this->ptr = (void*)tempPtr;
			this->type = new Type(tempPtr);
			// IMPORTANT NOTE: If you get C2440 on the line below, it means that the class does not inherit liteser::Serializable.
			Ptr<Serializable>(*ptr->value);
			delete ptr;
		}
		template <class T>
		Variable(chstr name, Ptr<T>* ptr)
		{
			this->name = name;
			// IMPORTANT NOTE: If you get C2440 on the line below, it means that the class does not inherit liteser::Serializable.
			Ptr<Serializable>* tempPtr = new Ptr<Serializable>(ptr->value);
			this->ptr = (void*)tempPtr;
			this->type = new Type(tempPtr);
			delete ptr;
		}
		~Variable();

		template <class T>
		Variable(chstr name, Ptr<harray<T> >* ptr)
		{
			this->name = name;
			this->type = new Type(ptr);
			this->ptr = (void*)ptr;
			foreach (T, it, *ptr->value)
			{
				this->subVariables += new Variable("", new Ptr<T>(&(*it)));
			}
		}

		template <class T>
		T* value()
		{
			return ((Ptr<T>*)this->ptr)->value;
		}

		Variable* createSubVariable();

	};

}
#endif
