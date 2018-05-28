/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <stdint.h>

#include <hltypes/harray.h>
#include <hltypes/henum.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>
#include <hltypes/hversion.h>

#include "liteser.h"
#include "Ptr.h"
#include "Type.h"
#include "Variable.h"

#define DEFINE_ASSIGNER(typeName) \
	Variable* Variable::assign(VPtr<typeName>* ptr) \
	{ \
		this->type->assign(ptr); \
		this->ptr = ptr; \
		return this; \
	}
#define DEFINE_ASSIGNER_HARRAY(typeName) \
	Variable* Variable::assign(VPtr<harray<typeName> >* ptr) \
	{ \
		this->type->assign(ptr); \
		this->ptr = ptr; \
		this->containerSize = ptr->value->size(); \
		foreach (typeName, it, *ptr->value) \
		{ \
			this->subVariables += (new Variable())->assign(new VPtr<typeName>(&(*it))); \
		} \
		return this; \
	}
#define DEFINE_ASSIGNERS(typeName) \
	DEFINE_ASSIGNER(typeName); \
	DEFINE_ASSIGNER_HARRAY(typeName);

namespace liteser
{
	DEFINE_ASSIGNERS(char);
	DEFINE_ASSIGNERS(unsigned char);
	DEFINE_ASSIGNERS(short);
	DEFINE_ASSIGNERS(unsigned short);
	DEFINE_ASSIGNERS(int);
	DEFINE_ASSIGNERS(unsigned int);
	DEFINE_ASSIGNERS(int64_t);
	DEFINE_ASSIGNERS(uint64_t);
	DEFINE_ASSIGNERS(float);
	DEFINE_ASSIGNERS(double);
	DEFINE_ASSIGNERS(hstr);
	DEFINE_ASSIGNERS(hversion);
	DEFINE_ASSIGNERS(grectf);
	DEFINE_ASSIGNERS(gvec2f);
	DEFINE_ASSIGNERS(gvec3f);
	DEFINE_ASSIGNER(bool);

	Variable::Variable(chstr name) : ptr(NULL), containerSize(0), ptrKeys(NULL), ptrValues(NULL)
	{
		this->name = name;
		this->type = new Type();
	}

	Variable::~Variable()
	{
		foreach (Variable*, it, this->subVariables)
		{
			delete (*it);
		}
		delete this->type;
		if (this->ptr != NULL)
		{
			delete this->ptr;
		}
		if (this->ptrKeys != NULL)
		{
			delete this->ptrKeys;
		}
		if (this->ptrValues != NULL)
		{
			delete this->ptrValues;
		}
	}

	void Variable::createSubVariables(Type::Identifier identifier)
	{
		if (this->type->subTypes.size() == 0)
		{
			throw Exception("Variable type does not contain subtypes: " + this->name);
		}
		if (identifier == Type::Identifier::Harray)
		{
			if (this->type->subTypes[0]->identifier == Type::Identifier::Int8)				this->_addSubVariablesHarray<char>();
			else if (this->type->subTypes[0]->identifier == Type::Identifier::UInt8)		this->_addSubVariablesHarray<unsigned char>();
			else if (this->type->subTypes[0]->identifier == Type::Identifier::Int16)		this->_addSubVariablesHarray<short>();
			else if (this->type->subTypes[0]->identifier == Type::Identifier::UInt16)		this->_addSubVariablesHarray<unsigned short>();
			else if (this->type->subTypes[0]->identifier == Type::Identifier::Int32)		this->_addSubVariablesHarray<int>();
			else if (this->type->subTypes[0]->identifier == Type::Identifier::UInt32)		this->_addSubVariablesHarray<unsigned int>();
			else if (this->type->subTypes[0]->identifier == Type::Identifier::Int64)		this->_addSubVariablesHarray<int64_t>();
			else if (this->type->subTypes[0]->identifier == Type::Identifier::UInt64)		this->_addSubVariablesHarray<uint64_t>();
			else if (this->type->subTypes[0]->identifier == Type::Identifier::Float)		this->_addSubVariablesHarray<float>();
			else if (this->type->subTypes[0]->identifier == Type::Identifier::Double)		this->_addSubVariablesHarray<double>();
			else if (this->type->subTypes[0]->identifier == Type::Identifier::Hstr)			this->_addSubVariablesHarray<hstr>();
			else if (this->type->subTypes[0]->identifier == Type::Identifier::Hversion)		this->_addSubVariablesHarray<hversion>();
			else if (this->type->subTypes[0]->identifier == Type::Identifier::Henum)		this->_addSubVariablesHarray<henum>();
			else if (this->type->subTypes[0]->identifier == Type::Identifier::Grectf)		this->_addSubVariablesHarray<grectf>();
			else if (this->type->subTypes[0]->identifier == Type::Identifier::Gvec2f)		this->_addSubVariablesHarray<gvec2f>();
			else if (this->type->subTypes[0]->identifier == Type::Identifier::Gvec3f)		this->_addSubVariablesHarray<gvec3f>();
			else if (this->type->subTypes[0]->identifier == Type::Identifier::ValueObject)	this->_addSubVariablesHarray<Serializable>();
			else if (this->type->subTypes[0]->identifier == Type::Identifier::Object)		this->_addSubVariablesHarray<Serializable*>();
			else throw Exception(hsprintf("Subtype is not supported within harray: %s; type: %02X", this->name.cStr(), this->type->subTypes[0]->identifier.value));
		}
		else if (identifier == Type::Identifier::Hmap)
		{
			if (this->type->subTypes[0]->identifier == Type::Identifier::Int8)			this->_addSubVariablesHmapKey<char>(this->type->subTypes[1]->identifier);
			else if (this->type->subTypes[0]->identifier == Type::Identifier::UInt8)	this->_addSubVariablesHmapKey<unsigned char>(this->type->subTypes[1]->identifier);
			else if (this->type->subTypes[0]->identifier == Type::Identifier::Int16)	this->_addSubVariablesHmapKey<short>(this->type->subTypes[1]->identifier);
			else if (this->type->subTypes[0]->identifier == Type::Identifier::UInt16)	this->_addSubVariablesHmapKey<unsigned short>(this->type->subTypes[1]->identifier);
			else if (this->type->subTypes[0]->identifier == Type::Identifier::Int32)	this->_addSubVariablesHmapKey<int>(this->type->subTypes[1]->identifier);
			else if (this->type->subTypes[0]->identifier == Type::Identifier::UInt32)	this->_addSubVariablesHmapKey<unsigned int>(this->type->subTypes[1]->identifier);
			else if (this->type->subTypes[0]->identifier == Type::Identifier::Int64)	this->_addSubVariablesHmapKey<int64_t>(this->type->subTypes[1]->identifier);
			else if (this->type->subTypes[0]->identifier == Type::Identifier::UInt64)	this->_addSubVariablesHmapKey<uint64_t>(this->type->subTypes[1]->identifier);
			else if (this->type->subTypes[0]->identifier == Type::Identifier::Float)	this->_addSubVariablesHmapKey<float>(this->type->subTypes[1]->identifier);
			else if (this->type->subTypes[0]->identifier == Type::Identifier::Double)	this->_addSubVariablesHmapKey<double>(this->type->subTypes[1]->identifier);
			else if (this->type->subTypes[0]->identifier == Type::Identifier::Hstr)		this->_addSubVariablesHmapKey<hstr>(this->type->subTypes[1]->identifier);
			else if (this->type->subTypes[0]->identifier == Type::Identifier::Hversion)	this->_addSubVariablesHmapKey<hversion>(this->type->subTypes[1]->identifier);
			else if (this->type->subTypes[0]->identifier == Type::Identifier::Henum)	this->_addSubVariablesHmapKey<henum>(this->type->subTypes[1]->identifier);
			else throw Exception(hsprintf("Subtype is not supported within hmap: %s; types: %02X %02X", this->name.cStr(), this->type->subTypes[0]->identifier.value, this->type->subTypes[1]->identifier.value));
		}
		else
		{
			throw Exception(hsprintf("Type is not supported for: %s; type: %02X", this->name.cStr(), type));
		}
	}

	void Variable::applyHmapSubVariables(Type::Identifier identifier)
	{
		if (identifier != Type::Identifier::Hmap)
		{
			throw Exception("Variable type is not hmap!");
		}
		if (this->type->subTypes[0]->identifier == Type::Identifier::Int8)			this->_applyHmapSubVariablesKey<char>(this->type->subTypes[1]->identifier);	
		else if (this->type->subTypes[0]->identifier == Type::Identifier::UInt8)	this->_applyHmapSubVariablesKey<unsigned char>(this->type->subTypes[1]->identifier);
		else if (this->type->subTypes[0]->identifier == Type::Identifier::Int16)	this->_applyHmapSubVariablesKey<short>(this->type->subTypes[1]->identifier);
		else if (this->type->subTypes[0]->identifier == Type::Identifier::UInt16)	this->_applyHmapSubVariablesKey<unsigned short>(this->type->subTypes[1]->identifier);
		else if (this->type->subTypes[0]->identifier == Type::Identifier::Int32)	this->_applyHmapSubVariablesKey<int>(this->type->subTypes[1]->identifier);
		else if (this->type->subTypes[0]->identifier == Type::Identifier::UInt32)	this->_applyHmapSubVariablesKey<unsigned int>(this->type->subTypes[1]->identifier);
		else if (this->type->subTypes[0]->identifier == Type::Identifier::Int64)	this->_applyHmapSubVariablesKey<int64_t>(this->type->subTypes[1]->identifier);
		else if (this->type->subTypes[0]->identifier == Type::Identifier::UInt64)	this->_applyHmapSubVariablesKey<uint64_t>(this->type->subTypes[1]->identifier);
		else if (this->type->subTypes[0]->identifier == Type::Identifier::Float)	this->_applyHmapSubVariablesKey<float>(this->type->subTypes[1]->identifier);
		else if (this->type->subTypes[0]->identifier == Type::Identifier::Double)	this->_applyHmapSubVariablesKey<double>(this->type->subTypes[1]->identifier);
		else if (this->type->subTypes[0]->identifier == Type::Identifier::Hstr)		this->_applyHmapSubVariablesKey<hstr>(this->type->subTypes[1]->identifier);
		else if (this->type->subTypes[0]->identifier == Type::Identifier::Hversion)	this->_applyHmapSubVariablesKey<hversion>(this->type->subTypes[1]->identifier);
		else if (this->type->subTypes[0]->identifier == Type::Identifier::Henum)	this->_applyHmapSubVariablesKey<henum>(this->type->subTypes[1]->identifier);
		else throw Exception(hsprintf("Subtype is not supported within hmap: %s; types: %02X %02X", this->name.cStr(), this->type->subTypes[0]->identifier.value, this->type->subTypes[1]->identifier.value));
	}

}
