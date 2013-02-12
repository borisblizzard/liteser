/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <stdint.h>

#include <hltypes/harray.h>
#include <hltypes/hdeque.h>
#include <hltypes/hlist.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "liteser.h"
#include "Type.h"

namespace liteser
{
	Type::Type(Ptr<char>* arg) : subType(NULL)
	{
		this->value = INT8;
	}

	Type::Type(Ptr<unsigned char>* arg) : subType(NULL)
	{
		this->value = UINT8;
	}

	Type::Type(Ptr<int16_t>* arg) : subType(NULL)
	{
		this->value = INT16;
	}

	Type::Type(Ptr<uint16_t>* arg) : subType(NULL)
	{
		this->value = UINT16;
	}

	Type::Type(Ptr<int32_t>* arg) : subType(NULL)
	{
		this->value = INT32;
	}

	Type::Type(Ptr<uint32_t>* arg) : subType(NULL)
	{
		this->value = UINT32;
	}

	Type::Type(Ptr<float>* arg) : subType(NULL)
	{
		this->value = FLOAT;
	}

	Type::Type(Ptr<double>* arg) : subType(NULL)
	{
		this->value = DOUBLE;
	}

	Type::Type(Ptr<bool>* arg) : subType(NULL)
	{
		this->value = BOOL;
	}

	Type::Type(Ptr<hstr>* arg) : subType(NULL)
	{
		this->value = HSTR;
	}

	Type::Type(Ptr<Serializable>* arg) : subType(NULL)
	{
		this->value = OBJECT;
	}

	Type::Type(Ptr<Serializable*>* arg) : subType(NULL)
	{
		this->value = OBJPTR;
	}

	Type::~Type()
	{
		if (this->subType != NULL)
		{
			delete this->subType;
		}
	}

	Variable* Type::createSubVariable()
	{
		switch (this->subType->value)
		{
		case Type::INT8:	return new Variable("", new Ptr<char>(NULL));
		case Type::UINT8:	return new Variable("", new Ptr<unsigned char>(NULL));
		case Type::INT16:	return new Variable("", new Ptr<int16_t>(NULL));
		case Type::UINT16:	return new Variable("", new Ptr<uint16_t>(NULL));
		case Type::INT32:	return new Variable("", new Ptr<int32_t>(NULL));
		case Type::UINT32:	return new Variable("", new Ptr<uint32_t>(NULL));
		case Type::FLOAT:	return new Variable("", new Ptr<float>(NULL));
		case Type::DOUBLE:	return new Variable("", new Ptr<double>(NULL));
		case Type::BOOL:	return new Variable("", new Ptr<bool>(NULL));
		case Type::HSTR:	return new Variable("", new Ptr<hstr>(NULL));
		case Type::OBJECT:	return new Variable("", new Ptr<Serializable>(NULL));
		case Type::OBJPTR:	return new Variable("", new Ptr<Serializable*>(NULL));
		case Type::HARRAY:	return this->subType->createSubVariable();
		}
		return NULL;
	}

}
