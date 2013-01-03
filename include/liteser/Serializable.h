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
	hstr _lsName() { return #classe; } \
	static liteser::Factory::Register<classe> _lsRegister;
#define LS_CLASS_DEFINE(classe) \
	liteser::Factory::Register<classe> classe::_lsRegister(#classe);
#define LS_VARS(superclass, ...) \
	__LS_FOREACH(__LS_VAR, __VA_ARGS__) \
	harray<liteser::Variable*> _lsVars() \
	{ \
		harray<liteser::Variable*> variables = superclass::_lsVars(); \
		__LS_FOREACH(__LS_REF, __VA_ARGS__) \
		return variables; \
	}

#define __LS_VAR(i, x) \
	__LS_PAIR(x);
#define __LS_REF(i, x) \
	variables += new liteser::Variable(hstr(__LS_STRINGIFY(__LS_STRIP x)).trim(), \
		new liteser::Variable::Ptr<__LS_TYPEOF x>(&this->__LS_STRIP x));

namespace liteser
{
	class liteserExport Serializable
	{
	public:
		Serializable();
		virtual ~Serializable();

		virtual hstr _lsName() { return "liteser::Serializer"; }
		virtual harray<Variable*> _lsVars() { return harray<Variable*>(); }

	};

}
#endif
