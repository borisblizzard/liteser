/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#define LOG_TAG "demo_simple"

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

class Type1 : public liteser::Serializable
{
public:
	Type1() : varInt(1), varFloat(2.0f), varBool(false) { }
	~Type1() { }

	HL_DEFINE_GETSET(int, varInt, VarInt);
	HL_DEFINE_GETSET(float, varFloat, VarFloat);
	HL_DEFINE_ISSET(bool, varBool, VarBool);

protected:
	LS_SERIALIZABLE
	(
		(int) varInt,
		(float) varFloat,
		(bool) varBool
	)

};

int main(int argc, char **argv)
{
	Type1 type1;
	/*
	printf("%d\n", type1.getVarInt());
	printf("%f\n", type1.getVarFloat());
	printf("%d\n", type1.getVarBool());
	*/
	system("pause");
	return 0;
}
