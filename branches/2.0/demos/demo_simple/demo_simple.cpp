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
	LS_SERIALIZABLE_CLASS;
	Type1() : varInt(1), varFloat(2.0f), varBool(false) { }
	~Type1() { }

	HL_DEFINE_GETSET(int, varInt, VarInt);
	HL_DEFINE_GETSET(float, varFloat, VarFloat);
	HL_DEFINE_ISSET(bool, varBool, VarBool);

	//*/
	/*
    template<int N, class T>
    static typename T::template _lsField<N, T> _lsVar2(T& x)
    {
        return typename T::template _lsField<N, T>(x);
    }
	//*/

//protected:
	LS_SERIALIZABLE
	(
		(int) varInt,
		(float) varFloat,
		(bool) varBool
	)

	/*
	template <class Self>
	struct _lsField<0, Self>
	{
		Self& self;
		_lsField(Self& self) : self(self) { }
		chstr name() { return "varInt"; }
		int& value() { return self.varInt; }
	};
	*/

public:
	//*
    template<int N, class T>
    _lsField<N, T> _lsVar()
    {
        return template _lsField<N, T>(*this);
    }
	//*/

	/*
	struct reflector
	{
	public:
		//Get field_data at index N
		template <int N, class T>
		static typename T::template _lsField<N, T> _lsVar(T& x)
		{
			return typename T::template _lsField<N, T>(x);
		}

		// Get the number of fields
		template <class T>
		struct fields
		{
			static const int n = T::fields_n;
		};
	};
	//*/

};

int main(int argc, char **argv)
{
	Type1 type1;
	//type1._lsVar<0, Type1>();
	//Type1::_lsVar2<0, Type1>(type1);
	///*
	int& value = type1._lsVar<3, Type1>().value();
	printf("%d\n", value);
	/*
	printf("%f\n", type1.getVarFloat());
	printf("%d\n", type1.getVarBool());
	*/
	system("pause");
	return 0;
}
