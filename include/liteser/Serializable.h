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

#include "liteserExport.h"
#include "macros.h"
#include "Variable.h"

/*

#define REM(...) __VA_ARGS__
#define EAT(...)

// Retrieve the type
#define TYPEOF(x) DETAIL_TYPEOF(DETAIL_TYPEOF_PROBE x,)
#define DETAIL_TYPEOF(...) DETAIL_TYPEOF_HEAD(__VA_ARGS__)
#define DETAIL_TYPEOF_HEAD(x, ...) REM x
#define DETAIL_TYPEOF_PROBE(...) (__VA_ARGS__),
// Strip off the type
#define STRIP(x) EAT x
// Show the type without parenthesis
#define PAIR(x) REM x



// A helper metafunction for adding const to a type
template<class M, class T>
struct make_const
{
    typedef T type;
};

template<class M, class T>
struct make_const<const M, T>
{
    typedef typename boost::add_const<T>::type type;
};


#define REFLECTABLE(...) \
static const int variables_n = BOOST_PP_VARIADIC_SIZE(__VA_ARGS__); \
friend struct reflector; \
template<int N, class Self = void> \
struct field_data {}; \
BOOST_PP_SEQ_FOR_EACH_I(REFLECT_EACH, data, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

#define REFLECT_EACH(r, data, i, x) \
PAIR(x); \
template<class Self> \
struct field_data<i, Self> \
{ \
    Self & self; \
    field_data(Self & self) : self(self) {} \
    \
    typename make_const<Self, TYPEOF(x)>::type & get() \
    { \
        return self.STRIP(x); \
    }\
    typename boost::add_const<TYPEOF(x)>::type & get() const \
    { \
        return self.STRIP(x); \
    }\
    const char * name() const \
    {\
        return BOOST_PP_STRINGIZE(STRIP(x)); \
    } \
};




struct reflector
{
    //Get field_data at index N
    template<int N, class T>
    static typename T::template field_data<N, T> get_field_data(T& x)
    {
        return typename T::template field_data<N, T>(x);
    }

    // Get the number of variables
    template<class T>
    struct variables
    {
        static const int n = T::variables_n;
    };
};





template<class C, class Action>
struct field_visitor
{
    Action a;
    C & c;
    field_visitor(Action a, C& c) : a(a), c(c)
    {
    }

    template<class T>
    void operator()(T)
    {
        a(reflector::get_field_data<T::value>(c));
    }
};

template<class C, class Action>
void visit_each(C & c, Action a)
{
    typedef boost::mpl::range_c<int,0,reflector::variables<C>::n> range;
    field_visitor<C, Action> visitor(a, c);
    boost::mpl::for_each<range>(visitor);
}





struct print_visitor
{
    template<class T>
    void operator()(T x)
    {
        std::cout << x.name() << "=" << x.get() << std::endl;
    }
};

template<class T>
void print_variables(T & x)
{
    visit_each(x, print_visitor());
}
*/



// template voodoo:
/*
#define LS_SERIALIZABLE(x, ...) \
	int _lsVarCount() { return __LS_VA_ARGC(__VA_ARGS__); } \
	template<int I, class Self = void> \
	struct _lsVariable { }; \
	__LS_FOREACH(__LS_VAR, __VA_ARGS__) \
	__LS_FOREACH(__LS_REF, __VA_ARGS__)

// separate variable declaration
#define __LS_VAR(i, x) \
	__LS_PAIR(x);
// template voodoo magic
#define __LS_REF(i, x) \
	template <class Self> \
	struct _lsVariable<i, Self> \
	{ \
	public: \
		Self& self; \
		_lsVariable(Self& self) : self(self) { } \
		chstr name() { return hstr(__LS_STRINGIFY(__LS_STRIP x)).trim(); } \
		__LS_TYPEOF x & value() { return self.__LS_STRIP x; } \
	};
*/

#define LS_SERIALIZABLE(thisclass, superclass, ...) \
	__LS_FOREACH(__LS_VAR, __VA_ARGS__) \
	hstr _lsName() { return #thisclass; } \
	harray<liteser::Variable> _lsVars() \
	{ \
		harray<liteser::Variable> variables = superclass::_lsVars(); \
		__LS_FOREACH(__LS_REF, __VA_ARGS__) \
		return variables; \
	}
#define __LS_VAR(i, x) \
	__LS_PAIR(x);
#define __LS_REF(i, x) \
	variables += liteser::Variable(hstr(__LS_STRINGIFY(__LS_STRIP x)).trim(), &this->__LS_STRIP x);

namespace liteser
{
	class liteserExport Serializable
	{
	public:
		Serializable();
		virtual ~Serializable();

		virtual hstr _lsName() { return "liteser::Serializer"; }
		virtual harray<Variable> _lsVars() { return harray<Variable>(); }

	};

}
#endif
