/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#define LOG_TAG "demo_simple"

#include <hltypes/hfile.h>
#include <hltypes/hstring.h>

/*
#include <stdio.h>

#include <hltypes/hlog.h>
#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>
*/
#include <liteser/liteser.h>
#include <liteser/Serializable.h>

class Type2 : public liteser::Serializable
{
public:
	Type2() : liteser::Serializable()
	{

	}
	~Type2() { }

	LS_SERIALIZABLE
	(
		liteser::Serializable,
		(hstr) string
	)
};

class Type3 : public Type2
{
public:
	Type3() : Type2()
	{
		this->string = "NO WAY";
		this->string2 = "MY WORLD IS BETTER";
	}
	~Type3() { }

	LS_SERIALIZABLE
	(
		Type2,
		(hstr) string2
	)
};

class Type1 : public liteser::Serializable
{
public:
	Type1() : liteser::Serializable()
	{

	}
	~Type1() { }

	LS_SERIALIZABLE
	(
		liteser::Serializable,
		(char) v_int8/*,
		(unsigned char) v_uint8,
		(short) v_int16,
		(unsigned short) v_uint16,
		(int) v_int32,
		(unsigned int) v_uint32,
		(float) v_float,
		(double) v_double,
		(bool) v_bool*/,
		(Type3*) v_type3/*,
		(Type2) v_type2*/
	)

};

int main(int argc, char **argv)
{
	Type1 type1;
	//type1._lsVar<0, Type1>();
	//Type1::_lsVar2<0, Type1>(type1);
	///*
	/*
	int& value = type1._lsVar<0, Type1>().value();
	printf("%d\n", value);
	int vars = type1._lsVarCount();
	for_iter (i, 0, vars)
	{
		printf("%s = %s\n", type1._lsVar<i, Type1>().name().c_str())
	}
	*/
	hfile file("demo_simple.lsb");
	//liteser::serialize(&file, &type1);
	/*
	printf("%f\n", type1.getVarFloat());
	printf("%d\n", type1.getVarBool());
	*/
	system("pause");
	return 0;
}
