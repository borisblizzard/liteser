/// @file
/// @version 2.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#define LOG_TAG "demo_simple"

#include <hltypes/hfile.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include <liteser/liteser.h>
#include <liteser/Serializable.h>

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
	}
	~Type2() { }

	void check(const Type2& other)
	{
		CHECK_VALUE(string);
	}

protected:
	LS_VARS
	(
		liteser::Serializable,
		(hstr) string
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
		this->v_harray_int = arg1;
		this->v_harray_type3 = arg2;
		this->v_hmap_hstr_int2 = arg3;
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
		CHECK_VALUE_HARRAY_OBJECTS(v_harray_type3);
		CHECK_VALUE(v_hmap_hstr_int);
		CHECK_VALUE(v_hmap_hstr_int2);
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
		(harray<int>) v_harray_int,
		(harray<Type3*>) v_harray_type3,
		(hmap_hstr_int) v_hmap_hstr_int,
		(hmap_hstr_int) v_hmap_hstr_int2
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
	
	file.open("demo_simple.ls2", hfile::WRITE);
	liteser::serialize(&file, &type1);
	file.close();
	
	Type1* loaded = NULL;
	file.open("demo_simple.ls2");
	liteser::deserialize(&file, (liteser::Serializable**)&loaded);
	file.close();
	
	type1.check(*loaded);
	if (__test_count == 0)
	{
		hlog::write(LOG_TAG, "Serialization test was successful.");
	}

	Type1* cloned = NULL;
	liteser::clone(&type1, (liteser::Serializable**)&cloned);
	__test_count = 0;
	type1.check(*loaded);
	if (__test_count == 0)
	{
		hlog::write(LOG_TAG, "Cloning test was successful.");
	}

	system("pause");
	return 0;
}
