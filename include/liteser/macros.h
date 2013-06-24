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
/// Defines special utility macros.

#ifndef LITESER_MACROS_H
#define LITESER_MACROS_H

// a little bit of magic
#define __LS_EXPAND(x) x
#define __LS_FIRSTARG(x, ...) (x)
#define __LS_RESTARGS(x, ...) (__VA_ARGS__)

// macros for messing around with the preprocessor
#define __LS_REM(...) __VA_ARGS__
#define __LS_REM2(x) __LS_REM x
#define __LS_EAT(...)
// stringifying stuff
#define __LS_STRINGIFY(x) __LS_X1STRINGIFY(x)
#define __LS_X1STRINGIFY(x) __LS_X2STRINGIFY(x)
#define __LS_X2STRINGIFY(x) #x

// some more magic for type retrieval
#define __LS_TYPEOF_EAT2(x, y) __LS_REM x
#define __LS_TYPEOF_ARG1(...) ((__VA_ARGS__),
#define __LS_TYPEOF(x) __LS_EXPAND(__LS_TYPEOF_EAT2 __LS_TYPEOF_ARG1 x))
// strip off the type
#define __LS_STRIP(x) __LS_EAT x
// show the argument without parenthesis
#define __LS_PAIR(x) __LS_REM2 x

// reversal indexing
#define __LS_VA_ARGC_INDEX( \
	 _0,  _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, \
	_10, _11, _12, _13, _14, _15, _16, _17, _18, _19, \
	_20, _21, _22, _23, _24, _25, _26, _27, _28, _29, \
	_30, _31, _32, _33, _34, _35, _36, _37, _38, _39, \
	_40, _41, _42, _43, _44, _45, _46, _47, _48, _49, \
	_50, _51, _52, _53, _54, _55, _56, _57, _58, _59, \
	_60, _61, _62, _63, size, ...) size
#define __LS_VA_ARGC(...) __LS_EXPAND(__LS_VA_ARGC_INDEX(__VA_ARGS__, \
							64, 63, 62, 61, \
	60, 59, 58, 57, 56, 55, 54, 53, 52, 51, \
	50, 49, 48, 47, 46, 45, 44, 43, 42, 41, \
	40, 39, 38, 37, 36, 35, 34, 33, 32, 31, \
	30, 29, 28, 27, 26, 25, 24, 23, 22, 21, \
	20, 19, 18, 17, 16, 15, 14, 13, 12, 11, \
	10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0))

// some recursive magic
#define __LS_FOREACH(m, args) __LS_FOREACH_(m, __LS_VA_ARGC args, args)
#define __LS_FOREACH_(m, n, args) __LS_FOREACH__(m, n, args)
#define __LS_FOREACH__(m, n, args) __LS_FOREACH_ ## n(m, 0, args)

#define  __LS_FOREACH_1(m, n, args) m(n, __LS_FIRSTARG args)
#define  __LS_FOREACH_2(m, n, args) m(n, __LS_FIRSTARG args)  __LS_FOREACH_1(m, (n + 1), __LS_RESTARGS args)
#define  __LS_FOREACH_3(m, n, args) m(n, __LS_FIRSTARG args)  __LS_FOREACH_2(m, (n + 1), __LS_RESTARGS args)
#define  __LS_FOREACH_4(m, n, args) m(n, __LS_FIRSTARG args)  __LS_FOREACH_3(m, (n + 1), __LS_RESTARGS args)
#define  __LS_FOREACH_5(m, n, args) m(n, __LS_FIRSTARG args)  __LS_FOREACH_4(m, (n + 1), __LS_RESTARGS args)
#define  __LS_FOREACH_6(m, n, args) m(n, __LS_FIRSTARG args)  __LS_FOREACH_5(m, (n + 1), __LS_RESTARGS args)
#define  __LS_FOREACH_7(m, n, args) m(n, __LS_FIRSTARG args)  __LS_FOREACH_6(m, (n + 1), __LS_RESTARGS args)
#define  __LS_FOREACH_8(m, n, args) m(n, __LS_FIRSTARG args)  __LS_FOREACH_7(m, (n + 1), __LS_RESTARGS args)
#define  __LS_FOREACH_9(m, n, args) m(n, __LS_FIRSTARG args)  __LS_FOREACH_8(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_10(m, n, args) m(n, __LS_FIRSTARG args)  __LS_FOREACH_9(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_11(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_10(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_12(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_11(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_13(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_12(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_14(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_13(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_15(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_14(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_16(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_15(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_17(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_16(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_18(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_17(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_19(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_18(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_20(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_19(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_21(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_20(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_22(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_21(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_23(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_22(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_24(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_23(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_25(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_24(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_26(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_25(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_27(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_26(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_28(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_27(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_29(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_28(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_30(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_29(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_31(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_30(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_32(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_31(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_33(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_32(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_34(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_33(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_35(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_34(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_36(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_35(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_37(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_36(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_38(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_37(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_39(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_38(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_40(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_39(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_41(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_40(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_42(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_41(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_43(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_42(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_44(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_43(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_45(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_44(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_46(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_45(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_47(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_46(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_48(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_47(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_49(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_48(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_50(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_49(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_51(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_50(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_52(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_51(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_53(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_52(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_54(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_53(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_55(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_54(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_56(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_55(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_57(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_56(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_58(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_57(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_59(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_58(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_60(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_59(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_61(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_60(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_62(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_60(m, (n + 1), __LS_RESTARGS args)
#define __LS_FOREACH_63(m, n, args) m(n, __LS_FIRSTARG args) __LS_FOREACH_60(m, (n + 1), __LS_RESTARGS args)


#define __LS_VAR(i, x) \
	__LS_PAIR(x);
#define __LS_REF(i, x) \
	variables += (new liteser::Variable(hstr(__LS_STRINGIFY(__LS_STRIP x)).trim()))->assign( \
		new liteser::VPtr<__LS_TYPEOF x>(&this->__LS_STRIP x));

#endif
