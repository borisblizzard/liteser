/// @file
/// @version 2.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#define LOG_TAG "demo_simple"

#include <hltypes/henum.h>
#include <hltypes/hfile.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include <liteser/liteser.h>
#include <liteser/Serializable.h>

#if defined(_WIN32) && !defined(_WINRT)
#define TEST_FILENAME "../../bin/demo_simple.ls2"
#define XML_FILENAME "../../bin/demo_simple.lsx"
#else
#define TEST_FILENAME "demo_simple.ls2"
#define XML_FILENAME "demo_simple.lsx"
#endif

static int __test_count = 0;

#define CHECK_VALUE(name) \
	if (this->name != other.name) \
	{ \
		hlog::warn(LOG_TAG, "NO MATCH: " #name); \
		__test_count++; \
	}
#define CHECK_VALUE_HARRAY_OBJECTS(name) \
	if (this->name.size() != other.name.size()) \
	{ \
		hlog::warn(LOG_TAG, "NO MATCH: " #name); \
		__test_count++; \
	} \
	else \
	{ \
		for_iter (i, 0, this->name.size()) \
		{ \
			this->name[i]->check(*other.name[i]); \
		} \
	}

class Type4
{
	int var;
	virtual void a() { return; };
};

class Type2 : public liteser::Serializable
{
public:
	LS_CLASS_DECLARE(Type2);
	Type2() : liteser::Serializable()
	{
		this->string = "HELLO WORLD";
		this->version.set(1, 2, 3, 4);
		this->rect.set(0.0f, 5.0f, 10.0f, 20.0f);
		this->vec2.set(100.0f, 200.0f);
		this->vec3.set(-50.0f, -50.0f, -200.0f);
	}
	~Type2() { }

	void check(const Type2& other)
	{
		CHECK_VALUE(string);
		CHECK_VALUE(version);
		CHECK_VALUE(rect);
		CHECK_VALUE(vec2);
		CHECK_VALUE(vec3);
		CHECK_VALUE(moreStrings);
	}

protected:
	LS_VARS
	(
		liteser::Serializable,
		(hstr) string,
		(hversion) version,
		(grect) rect,
		(gvec2) vec2,
		(gvec3) vec3,
		(harray<hstr>) moreStrings
	);

};
LS_CLASS_DEFINE(Type2);

class Type3 : public Type2
{
public:
	LS_CLASS_DECLARE(Type3);
	Type3() : Type2()
	{
		this->string = "NO WAY";
		this->string2 = "MY WORLD IS BETTER";
	}
	~Type3() { }

	void check(const Type3& other)
	{
		CHECK_VALUE(string2);
		Type2::check(other);
	}

protected:
	LS_VARS
	(
		Type2,
		(hstr) string2
	);

};
LS_CLASS_DEFINE(Type3);

HL_ENUM_CLASS_DECLARE(TypeEnum,
(
	HL_ENUM_DECLARE(TypeEnum, Value0);
	HL_ENUM_DECLARE(TypeEnum, Value1);
	HL_ENUM_DECLARE(TypeEnum, Value2);
));
HL_ENUM_CLASS_DEFINE(TypeEnum,
(
	HL_ENUM_DEFINE_NAME(TypeEnum, Value0, "0");
	HL_ENUM_DEFINE_VALUE(TypeEnum, Value1, 1);
	HL_ENUM_DEFINE_VALUE(TypeEnum, Value2, 2);
));

class Type1 : public liteser::Serializable
{
public:
	typedef hmap<hstr, int> hmap_hstr_int;
	LS_CLASS_DECLARE(Type1);
	Type1() : liteser::Serializable()
	{
		this->v_int8 = -8;
		this->v_uint8 = 18;
		this->v_int16 = -16;
		this->v_uint16 = 116;
		this->v_int32 = -32;
		this->v_uint32 = 132;
		this->v_float = 1.0f;
		this->v_double = 2.0f;
		this->v_bool = true;
		this->v_type3 = new Type3();
		this->v_type4 = this->v_type3;
		this->v_enum = TypeEnum::Value0;
	}
	Type1(harray<int> arg1, harray<Type3*> arg2, hmap<hstr, int> arg3) : liteser::Serializable()
	{
		this->v_int8 = -8;
		this->v_uint8 = 18;
		this->v_int16 = -16;
		this->v_uint16 = 116;
		this->v_int32 = -32;
		this->v_uint32 = 132;
		this->v_float = 1.0f;
		this->v_double = 2.0f;
		this->v_bool = true;
		this->v_type3 = new Type3();
		this->v_type4 = this->v_type3;
		this->v_harray_int = arg1;
		this->v_harray_type3 = arg2;
		this->v_hmap_hstr_int2 = arg3;
		this->v_enum = TypeEnum::Value1;
	}
	~Type1()
	{
		delete this->v_type3;
		foreach (Type3*, it, this->v_harray_type3)
		{
			delete (*it);
		}
	}

	void check(const Type1& other)
	{
		CHECK_VALUE(v_int8);
		CHECK_VALUE(v_uint8);
		CHECK_VALUE(v_int16);
		CHECK_VALUE(v_uint16);
		CHECK_VALUE(v_int32);
		CHECK_VALUE(v_uint32);
		CHECK_VALUE(v_float);
		CHECK_VALUE(v_double);
		CHECK_VALUE(v_bool);
		CHECK_VALUE(v_harray_int);
		this->v_type2.check(other.v_type2);
		this->v_type3->check(*other.v_type3);
		this->v_type4->check(*other.v_type4);
		CHECK_VALUE_HARRAY_OBJECTS(v_harray_type3);
		CHECK_VALUE(v_hmap_hstr_int);
		CHECK_VALUE(v_hmap_hstr_int2);
		CHECK_VALUE(v_enum);
	}

protected:

	LS_VARS
	(
		liteser::Serializable,
		(char) v_int8,
		(unsigned char) v_uint8,
		(short) v_int16,
		(unsigned short) v_uint16,
		(int) v_int32,
		(unsigned int) v_uint32,
		(float) v_float,
		(double) v_double,
		(bool) v_bool,
		(Type2) v_type2,
		(Type3*) v_type3,
		(Type3*) v_type4,
		(harray<int>) v_harray_int,
		(harray<Type3*>) v_harray_type3,
		(hmap_hstr_int) v_hmap_hstr_int,
		(hmap_hstr_int) v_hmap_hstr_int2,
		(henum) v_enum
	)

};
LS_CLASS_DEFINE(Type1);

int main(int argc, char **argv)
{
	hfile file;
	harray<int> arg1;
	arg1 += 4;
	arg1 += 8;
	harray<Type3*> arg2;
	arg2 += new Type3();
	arg2 += new Type3();
	hmap<hstr, int> arg3;
	arg3["first"] = 10;
	arg3["second"] = 20;
	Type1 type1(arg1, arg2, arg3);
	Type1* loaded = NULL;
	// binary test
	file.open(TEST_FILENAME, hfile::WRITE);
	liteser::serialize(&file, &type1);
	file.close();
	file.open(TEST_FILENAME);
	liteser::deserialize(&file, (liteser::Serializable**)&loaded);
	file.close();
	__test_count = 0;
	type1.check(*loaded);
	if (__test_count == 0)
	{
		hlog::write(LOG_TAG, "Serialization test was successful.");
	}
	delete loaded;
	loaded = NULL;
	// XML test
	file.open(XML_FILENAME, hfile::WRITE);
	liteser::xml::serialize(&file, &type1);
	file.close();
	file.open(XML_FILENAME);
	liteser::xml::deserialize(&file, (liteser::Serializable**)&loaded);
	file.close();
	__test_count = 0;
	type1.check(*loaded);
	if (__test_count == 0)
	{
		hlog::write(LOG_TAG, "Serialization XML test was successful.");
	}
	delete loaded;
	loaded = NULL;
	// clone test
	Type1* cloned = NULL;
	liteser::clone(&type1, (liteser::Serializable**)&cloned);
	__test_count = 0;
	type1.check(*cloned);
	if (__test_count == 0)
	{
		hlog::write(LOG_TAG, "Cloning test was successful.");
	}
	delete cloned;
	// finished
	system("pause");
	return 0;
}
