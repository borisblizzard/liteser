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
/// Defines special utility templates.

#ifndef LITESER_TEMPLATES_H
#define LITESER_TEMPLATES_H

template <bool T>
struct __LS_BOOL
{
	static const bool value = T;
	operator bool() const { return value; }
};

// type is undefined for assumed !_Test
template <bool _Test, class T>
struct __LS_ENABLE_IF
{
};
// type is T is true for _Test
template <class T>
struct __LS_ENABLE_IF<true, T>
{
	using type = T;
};

template <bool _Test, class T>
using __LS_ENABLE_IF_T = typename __LS_ENABLE_IF<_Test, T>::type;

// determine whether _Base is a base of or the same as _Der
template <class T, class S>
struct __LS_IS_BASE_OF : __LS_BOOL<__is_base_of(S, T)>
{
};

#endif
