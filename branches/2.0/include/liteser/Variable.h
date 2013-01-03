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

namespace liteser
{
	class Serializable;

	class liteserExport Variable
	{
	public:
		enum Type
		{
			TYPE_INT8	= 0x01,
			TYPE_UINT8	= 0x02,
			TYPE_INT16	= 0x03,
			TYPE_UINT16	= 0x04,
			TYPE_INT32	= 0x05,
			TYPE_UINT32	= 0x06,
			TYPE_FLOAT	= 0x21,
			TYPE_DOUBLE	= 0x22,
			TYPE_BOOL	= 0x41,
			TYPE_OBJECT	= 0x61,
			TYPE_OBJPTR	= 0x62,
			TYPE_HSTR	= 0x81,
			TYPE_HARRAY	= 0xA1,
			TYPE_HLIST	= 0xA2,
			TYPE_HDEQUE	= 0xA3,
			TYPE_HMAP	= 0xC1
		};

		template<class T>
		struct Ptr
		{
		public:
			T* value;
			Ptr(T* value) { this->value = value; }
			~Ptr() { }
		};

		hstr name;
		void* ptr;
		Type type;

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
		//template <class T>
		//Variable(chstr name, harray<T> value);
		template <class T>
		Variable(chstr name, Ptr<T*>* ptr)
		{
			Serializable* obj = dynamic_cast<Serializable*>(*ptr->value);
			this->name = name;
			this->ptr = (void*)(new Ptr<Serializable*>((Serializable**)ptr->value));
			this->type = TYPE_OBJPTR;
			// IMPORTANT NOTE: If you get C2440 on the line below, it means that the class does not inherit liteser::Serializable.
			Ptr<Serializable>(*ptr->value);
			delete ptr;
		}
		template <class T>
		Variable(chstr name, Ptr<T>* ptr)
		{
			this->name = name;
			// IMPORTANT NOTE: If you get C2440 on the line below, it means that the class does not inherit liteser::Serializable.
			this->ptr = (void*)(new Ptr<Serializable>(ptr->value));
			this->type = TYPE_OBJECT;
			delete ptr;
		}
		~Variable();

		template <class T>
		T* value()
		{
			return ((Variable::Ptr<T>*)this->ptr)->value;
		}

	};

}
#endif
