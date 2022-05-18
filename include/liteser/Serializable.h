/// @file
/// @version 3.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Represents a base for a serializable object.

#ifndef LITESER_SERIALIZABLE_H
#define LITESER_SERIALIZABLE_H

#include <stdint.h>

#include <hltypes/hsbase.h>

#include "Factory.h"
#include "liteserExport.h"
#include "macros.h"
#include "Variable.h"

#define LS_CLASS_DECLARE(classe) \
	LS_CLASS_DECLARE_ABSTRACT(classe); \
	static liteser::Factory::Register<classe> _lsRegister;
#define LS_CLASS_DEFINE(classe) \
	liteser::Factory::Register<classe> classe::_lsRegister(#classe);
/// @brief Use this instead of LS_CLASS_DECLARE if class contains pure virtual methods. In this case LS_CLASS_DEFINE is not needed.
#define LS_CLASS_DECLARE_ABSTRACT(classe) \
	inline hstr _lsName() const override { return #classe; }

#define LS_CLASS_DECLARE_CLONEABLE(classe) \
	LS_CLASS_DECLARE(classe); \
	classe* clone() override;
#define LS_CLASS_DEFINE_CLONEABLE(classe, superclass) \
	LS_CLASS_DEFINE(classe); \
	classe* classe::clone() { return (classe*)superclass::clone(); }

#define LS_VARS(superclass, ...) \
	__LS_FOREACH(__LS_VAR, (__VA_ARGS__)) \
	inline harray<liteser::Variable*> _lsVars() override \
	{ \
		harray<liteser::Variable*> variables = superclass::_lsVars(); \
		__LS_FOREACH(__LS_REF, (__VA_ARGS__)) \
		return variables; \
	}
#define LS_VARS_DECLARE(...) \
	__LS_FOREACH(__LS_VAR, (__VA_ARGS__)) \
	harray<liteser::Variable*> _lsVars() override;
#define LS_VARS_DEFINE(classe, superclass, ...) \
	harray<liteser::Variable*> classe::_lsVars() \
	{ \
		harray<liteser::Variable*> variables = superclass::_lsVars(); \
		__LS_FOREACH(__LS_REF, (__VA_ARGS__)) \
		return variables; \
	}
#define LS_FORCE_REGISTER(classe, name) \
	static classe __lsRegisterObject ## name;
#define LS_REGISTER(classe) \
	{ \
		liteser::Factory::Register<classe> object(#classe); \
	}

namespace liteser
{
	class liteserExport Serializable
	{
	public:
		Serializable();
		virtual ~Serializable();

		virtual inline hstr _lsName() const { return "liteser::Serializer"; }
		virtual inline harray<Variable*> _lsVars() { return harray<Variable*>(); }
		virtual inline void _lsOnSerializing() { }
		virtual inline void _lsOnSerialized() { }
		virtual inline void _lsOnDeserializing() { }
		virtual inline void _lsOnDeserialized() { }
#
	protected:
		virtual Serializable* clone();

	};

}
#endif
