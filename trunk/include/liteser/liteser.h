/// @file
/// @author  Boris Mikic
/// @version 1.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines global serialization and deserialization procedures as well as serialization macros.

#ifndef LITESER_H
#define LITESER_H

#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hmap.h>
#include <hltypes/hsbase.h>
#include <hltypes/hstring.h>

#include "liteserExport.h"
#include "Serializable.h"

#define _LS_VERSION_MAJOR 1
#define _LS_VERSION_MINOR 0

#define _LS_INIT_MANUAL_SERIALIZATION \
	liteser::_lsIds.clear(); \
	stream->dump((unsigned char)_LS_VERSION_MAJOR); \
	stream->dump((unsigned char)_LS_VERSION_MINOR);
#define _LS_INIT_MANUAL_DESERIALIZATION \
	liteser::_lsIds.clear(); \
	unsigned char major = stream->load_uchar(); \
	unsigned char minor = stream->load_uchar(); \
	liteser::checkVersion(major, minor);
#define _LS_ASSIGN_HMAP_KEYS_VALUES(name) \
	for_iter (_lsI ## name, 0, _lsKeys ## name.size()) \
	{ \
		name[_lsKeys ## name[_lsI ## name]] = _lsValues ## name[_lsI ## name]; \
	}

#define LS_MAKE_SERIALIZABLE \
	bool serialize(hsbase* stream); \
	bool deserialize(hsbase* stream);

#define LS_SER_BEGIN(classe, superclass) \
	bool classe::serialize(hsbase* stream)\
	{ \
		if (!superclass::serialize(stream)) \
		{ \
			return false; \
		}
#define LS_SER_END \
		return true; \
	} 

#define LS_DES_BEGIN(classe, superclass) \
	bool classe::deserialize(hsbase* stream)\
	{ \
		if (!superclass::deserialize(stream)) \
		{ \
			return false; \
		}
#define LS_DES_END \
		return true; \
	}

// single serialization

#define LS_SER(name) stream->dump(name);
#define LS_SER_ENUM(name) stream->dump((int)name);
/// @note "loadType" can be char, uchar, int, uint, long, ulong, short, ushort, bool, float, double or hstr
#define LS_DES(name, loadType) name = stream->load_ ## loadType();
#define LS_DES_ENUM(name, type) name = (type)stream->load_int();

#define LS_SER_OBJ(name) name.serialize(stream);
#define LS_DES_OBJ(name) \
	int _lsId ## name = stream->load_uint(); \
	liteser::_lsIds[_lsId ## name] = &name; \
	name.deserialize(stream);

#define LS_SER_OBJ_PTR(name) \
	if (name != NULL) \
	{ \
		name->serialize(stream); \
	} \
	else \
	{ \
		stream->dump(0); \
	}
#define LS_DES_OBJ_PTR(name, classe) \
	int _lsId ## name = stream->load_uint(); \
	if (_lsId ## name != 0) \
	{ \
		if (liteser::_lsIds.has_key(_lsId ## name)) \
		{ \
			name = (classe*)liteser::_lsIds[_lsId ## name]; \
		} \
		else \
		{ \
			if (name == NULL) \
			{ \
				name = new classe(); \
			} \
			liteser::_lsIds[_lsId ## name] = name; \
			name->deserialize(stream); \
		} \
	} \
	else if (name != NULL) \
	{ \
		delete name; \
		name = NULL; \
	}

// array serialization

#define LS_SER_ARRAY(name, size) \
	stream->dump(size); \
	for_iter (_lsI ## name, 0, size) \
	{ \
		stream->dump(name[_lsI ## name]); \
	}
#define LS_SER_ARRAY_ENUM(name, size) \
	stream->dump(size); \
	for_iter (_lsI ## name, 0, size) \
	{ \
		stream->dump((int)name[_lsI ## name]); \
	}
/// @note "type" can be char, unsigned char, int, unsigned uint, long, unsigned long, short, unsigned short, bool, float, double or hstr
/// @note "loadType" can be char, uchar, int, uint, long, ulong, short, ushort, bool, float, double or hstr and has to correspond with "type"
#define LS_DES_ARRAY(name, type, loadType) \
	int _lsCount ## name = stream->load_int(); \
	if (name != NULL) \
	{ \
		delete [] name; \
		name = new type[_lsCount ## name]; \
	} \
	for_iter (_lsI ## name, 0, _lsCount ## name) \
	{ \
		name[_lsI ## name] = stream->load_ ## loadType(); \
	}
#define LS_DES_ARRAY_ENUM(name, type, loadType) \
	int _lsCount ## name = stream->load_int(); \
	if (name != NULL) \
	{ \
		delete [] name; \
		name = new type[_lsCount ## name]; \
	} \
	for_iter (_lsI ## name, 0, _lsCount ## name) \
	{ \
		name[_lsI ## name] = (type)stream->load_int(); \
	}

#define LS_SER_ARRAY_OBJ(name, size) \
	stream->dump(size); \
	for_iter (_lsI ## name, 0, size) \
	{ \
		LS_SER_OBJ(name[_lsI ## name]); \
	}
#define LS_DES_ARRAY_OBJ(name, classe) \
	int _lsCount ## name = stream->load_int(); \
	if (name != NULL) \
	{ \
		delete [] name; \
		name = new classe[_lsCount ## name]; \
	} \
	for_iter (_lsI ## name, 0, _lsCount ## name) \
	{ \
		classe _lsInstance; \
		LS_DES_OBJ(_lsInstance); \
		name[_lsI ## name] = _lsInstance; \
	}

#define LS_SER_ARRAY_OBJ_PTR(name, size) \
	stream->dump(size); \
	for_iter (_lsI ## name, 0, size) \
	{ \
		LS_SER_OBJ_PTR(name[_lsI ## name]); \
	}
#define LS_DES_ARRAY_OBJ_PTR(name, classe) \
	int _lsCount ## name = stream->load_int(); \
	for_iter (_lsI ## name, 0, _lsCount) \
	{ \
		classe* _lsInstance = NULL; \
		LS_DES_OBJ_PTR(_lsInstance, classe); \
		name[_lsI ## name] = _lsInstance; \
	}

// harray serialization

#define LS_SER_HARRAY(name) \
	stream->dump(name.size()); \
	for_iter (_lsI ## name, 0, name.size()) \
	{ \
		stream->dump(name[_lsI ## name]); \
	}
#define LS_SER_HARRAY_ENUM(name) \
	stream->dump(name.size()); \
	for_iter (_lsI ## name, 0, name.size()) \
	{ \
		stream->dump((int)name[_lsI ## name]); \
	}
/// @note "type" can be char, uchar, int, uint, long, ulong, short, ushort, bool, float, double or hstr
#define LS_DES_HARRAY(name, type) \
	name.clear(); \
	int _lsCount ## name = stream->load_int(); \
	for_iter (_lsI ## name, 0, _lsCount ## name) \
	{ \
		name += stream->load_ ## type(); \
	}
#define LS_DES_HARRAY_ENUM(name, type) \
	name.clear(); \
	int _lsCount ## name = stream->load_int(); \
	for_iter (_lsI ## name, 0, _lsCount ## name) \
	{ \
		name += (type)stream->load_int(); \
	}

#define LS_SER_HARRAY_OBJ(name) \
	stream->dump(name.size()); \
	for_iter (_lsI ## name, 0, name.size()) \
	{ \
		LS_SER_OBJ(name[_lsI ## name]); \
	}
#define LS_DES_HARRAY_OBJ(name, classe) \
	int _lsCount ## name = stream->load_int(); \
	for_iter (_lsI ## name, 0, _lsCount ## name) \
	{ \
		classe _lsInstance; \
		LS_DES_OBJ(_lsInstance); \
		name += _lsInstance; \
	}

#define LS_SER_HARRAY_OBJ_PTR(name) \
	stream->dump(name.size()); \
	for_iter (_lsI ## name, 0, name.size()) \
	{ \
		LS_SER_OBJ_PTR(name[_lsI ## name]); \
	}
#define LS_DES_HARRAY_OBJ_PTR(name, classe) \
	int _lsCount ## name = stream->load_int(); \
	for_iter (_lsI ## name, 0, _lsCount ## name) \
	{ \
		classe* _lsInstance = NULL; \
		LS_DES_OBJ_PTR(_lsInstance, classe); \
		name += _lsInstance; \
	}

// hmap serialization

/// @note "keyType" and "valueType" can be char, unsigned char, int, unsigned uint, long, unsigned long, short, unsigned short, bool, float, double or hstr
#define LS_SER_HMAP(name, keyType, valueType) \
	harray<keyType> _lsKeys ## name = name.keys(); \
	harray<valueType> _lsValues ## name = name.values(_lsKeys ## name); \
	LS_SER_HARRAY(_lsKeys ## name); \
	LS_SER_HARRAY(_lsValues ## name);
/// @note "keyType" and "valueType" can be char, unsigned char, int, unsigned uint, long, unsigned long, short, unsigned short, bool, float, double or hstr
/// @note "keyLoadType" and "valueLoadType" can be char, uchar, int, uint, long, ulong, short, ushort, bool, float, double or hstr and have to correspond with "keyType" and "valueType"
#define LS_DES_HMAP(name, keyType, keyLoadType, valueType, valueLoadType) \
	name.clear(); \
	harray<keyType> _lsKeys ## name; \
	LS_DES_HARRAY(_lsKeys ## name, keyLoadType); \
	harray<valueType> _lsValues ## name; \
	LS_DES_HARRAY(_lsValues ## name, valueLoadType); \
	_LS_ASSIGN_HMAP_KEYS_VALUES(name);

/// @note "keyType" can be char, unsigned char, int, unsigned uint, long, unsigned long, short, unsigned short, bool, float, double or hstr
#define LS_SER_HMAP_V_OBJ(name, keyType, valueClass) \
	harray<keyType> _lsKeys ## name = name.keys(); \
	harray<valueClass> _lsValues ## name = name.values(_lsKeys ## name); \
	LS_SER_HARRAY(_lsKeys ## name); \
	LS_SER_HARRAY_OBJ(_lsValues ## name);
/// @note "keyType" can be char, unsigned char, int, unsigned uint, long, unsigned long, short, unsigned short, bool, float, double or hstr
/// @note "keyLoadType" can be char, uchar, int, uint, long, ulong, short, ushort, bool, float, double or hstr and has to correspond with "keyType"
#define LS_DES_HMAP_V_OBJ(name, keyType, keyLoadType, valueClass) \
	name.clear(); \
	harray<keyType> _lsKeys ## name; \
	LS_DES_HARRAY(_lsKeys ## name, keyLoadType); \
	harray<valueClass> _lsValues ## name; \
	LS_DES_HARRAY_OBJ(_lsValues ## name, valueClass); \
	_LS_ASSIGN_HMAP_KEYS_VALUES(name);

/// @note "keyType" can be char, unsigned char, int, unsigned uint, long, unsigned long, short, unsigned short, bool, float, double or hstr
#define LS_SER_HMAP_V_OBJ_PTR(name, keyType, valueClass) \
	harray<keyType> _lsKeys ## name = name.keys(); \
	harray<valueClass> _lsValues ## name = name.values(_lsKeys ## name); \
	LS_SER_HARRAY(_lsKeys ## name); \
	LS_SER_HARRAY_OBJ_PTR(_lsValues ## name);
/// @note "keyType" can be char, unsigned char, int, unsigned uint, long, unsigned long, short, unsigned short, bool, float, double or hstr
/// @note "keyLoadType" can be char, uchar, int, uint, long, ulong, short, ushort, bool, float, double or hstr and has to correspond with "keyType"
#define LS_DES_HMAP_V_OBJ_PTR(name, keyType, keyLoadType, valueClass) \
	name.clear(); \
	harray<keyType> _lsKeys ## name; \
	LS_DES_HARRAY(_lsKeys ## name, keyLoadType); \
	harray<valueClass> _lsValues ## name; \
	LS_DES_HARRAY_OBJ_PTR(_lsValues ## name, valueClass); \
	_LS_ASSIGN_HMAP_KEYS_VALUES(name);

/// @note "valueType" can be char, unsigned char, int, unsigned uint, long, unsigned long, short, unsigned short, bool, float, double or hstr
#define LS_SER_HMAP_K_OBJ(name, valueType, keyClass) \
	harray<keyClass> _lsKeys ## name = name.keys(); \
	harray<valueType> _lsValues ## name = name.values(_lsKeys ## name); \
	LS_SER_HARRAY_OBJ(_lsKeys ## name); \
	LS_SER_HARRAY(_lsValues ## name);
/// @note "valueType" can be char, unsigned char, int, unsigned uint, long, unsigned long, short, unsigned short, bool, float, double or hstr
/// @note "valueLoadType" can be char, uchar, int, uint, long, ulong, short, ushort, bool, float, double or hstr and has to correspond with "valueType"
#define LS_DES_HMAP_K_OBJ(name, keyClass, valueType, valueLoadType) \
	name.clear(); \
	harray<keyClass> _lsKeys ## name; \
	LS_DES_HARRAY_OBJ(_lsKeys ## name, keyClass); \
	harray<valueType> _lsValues ## name; \
	LS_DES_HARRAY(_lsValues ## name, valueLoadType); \
	_LS_ASSIGN_HMAP_KEYS_VALUES(name);

#define LS_SER_HMAP_K_OBJ_V_OBJ(name, valueType, valueClass) \
	harray<keyClass> _lsKeys ## name = name.keys(); \
	harray<valueClass> _lsValues ## name = name.values(_lsKeys ## name); \
	LS_SER_HARRAY_OBJ(_lsKeys ## name); \
	LS_SER_HARRAY_OBJ(_lsValues ## name);
#define LS_DES_HMAP_K_OBJ_V_OBJ(name, keyClass, valueClass) \
	name.clear(); \
	harray<keyClass> _lsKeys ## name; \
	LS_DES_HARRAY_OBJ(_lsKeys ## name, keyClass); \
	harray<valueClass> _lsValues ## name; \
	LS_DES_HARRAY_OBJ(_lsValues ## name, valueClass); \
	_LS_ASSIGN_HMAP_KEYS_VALUES(name);

#define LS_SER_HMAP_K_OBJ_V_OBJ_PTR(name, valueType, valueClass) \
	harray<keyClass> _lsKeys ## name = name.keys(); \
	harray<valueClass*> _lsValues ## name = name.values(_lsKeys ## name); \
	LS_SER_HARRAY_OBJ(_lsKeys ## name); \
	LS_SER_HARRAY_OBJ_PTR(_lsValues ## name);
#define LS_DES_HMAP_K_OBJ_V_OBJ_PTR(name, keyClass, valueClass) \
	name.clear(); \
	harray<keyClass> _lsKeys ## name; \
	LS_DES_HARRAY_OBJ(_lsKeys ## name, keyClass); \
	harray<valueClass*> _lsValues ## name; \
	LS_DES_HARRAY_OBJ_PTR(_lsValues ## name, valueClass); \
	_LS_ASSIGN_HMAP_KEYS_VALUES(name);

/// @note "valueType" can be char, unsigned char, int, unsigned uint, long, unsigned long, short, unsigned short, bool, float, double or hstr
#define LS_SER_HMAP_K_OBJ_PTR(name, valueType, keyClass) \
	harray<keyClass*> _lsKeys ## name = name.keys(); \
	harray<valueType> _lsValues ## name = name.values(_lsKeys ## name); \
	LS_SER_HARRAY_OBJ_PTR(_lsKeys ## name); \
	LS_SER_HARRAY(_lsValues ## name);
/// @note "valueType" can be char, unsigned char, int, unsigned uint, long, unsigned long, short, unsigned short, bool, float, double or hstr
/// @note "valueLoadType" can be char, uchar, int, uint, long, ulong, short, ushort, bool, float, double or hstr and has to correspond with "valueType"
#define LS_DES_HMAP_K_OBJ_PTR(name, keyClass, valueType, valueLoadType) \
	name.clear(); \
	harray<keyClass*> _lsKeys ## name; \
	LS_DES_HARRAY_OBJ_PTR(_lsKeys ## name, keyClass); \
	harray<valueType> _lsValues ## name; \
	LS_DES_HARRAY(_lsValues ## name, valueLoadType); \
	_LS_ASSIGN_HMAP_KEYS_VALUES(name);

#define LS_SER_HMAP_K_OBJ_PTR_V_OBJ(name, valueType, valueClass) \
	harray<keyClass*> _lsKeys ## name = name.keys(); \
	harray<valueClass> _lsValues ## name = name.values(_lsKeys ## name); \
	LS_SER_HARRAY_OBJ_PTR(_lsKeys ## name); \
	LS_SER_HARRAY_OBJ(_lsValues ## name);
#define LS_DES_HMAP_K_OBJ_PTR_V_OBJ(name, keyClass, valueClass) \
	name.clear(); \
	harray<keyClass*> _lsKeys ## name; \
	LS_DES_HARRAY_OBJ_PTR(_lsKeys ## name, keyClass); \
	harray<valueClass> _lsValues ## name; \
	LS_DES_HARRAY_OBJ(_lsValues ## name, valueClass); \
	_LS_ASSIGN_HMAP_KEYS_VALUES(name);

#define LS_SER_HMAP_K_OBJ_PTR_V_OBJ_PTR(name, valueType, valueClass) \
	harray<keyClass*> _lsKeys ## name = name.keys(); \
	harray<valueClass*> _lsValues ## name = name.values(_lsKeys ## name); \
	LS_SER_HARRAY_OBJ_PTR(_lsKeys ## name); \
	LS_SER_HARRAY_OBJ_PTR(_lsValues ## name);
#define LS_DES_HMAP_K_OBJ_PTR_V_OBJ_PTR(name, keyClass, valueClass) \
	name.clear(); \
	harray<keyClass*> _lsKeys ## name; \
	LS_DES_HARRAY_OBJ_PTR(_lsKeys ## name, keyClass); \
	harray<valueClass*> _lsValues ## name; \
	LS_DES_HARRAY_OBJ_PTR(_lsValues ## name, valueClass); \
	_LS_ASSIGN_HMAP_KEYS_VALUES(name);

namespace liteser
{
	liteserFnExport void serialize(hsbase* stream, Serializable* object);
	liteserFnExport void deserialize(hsbase* stream, Serializable* object);
	liteserFnExport void checkVersion(unsigned char major, unsigned char minor);

	/*
	/// @note Use only with integral types and hstr
	template <class T>
	void serialize(hsbase* stream, harray<T>& objects)
	{
		_LS_INIT_SERIALIZATION;
		LS_SER_HARRAY(objects);
	}
	
	/// @note Use only with integral types and hstr
	template <class T>
	void deserialize(hsbase* stream, harray<T>& objects)
	{
		_LS_INIT_DESERIALIZATION;
		LS_DES_HARRAY(objects, T);
	}
	
	template <class T>
	void serialize_obj(hsbase* stream, harray<T>& objects)
	{
		_LS_INIT_SERIALIZATION;
		LS_SER_HARRAY_OBJ(objects);
	}
	
	template <class T>
	void deserialize_obj(hsbase* stream, harray<T>& objects)
	{
		_LS_INIT_DESERIALIZATION;
		LS_DES_HARRAY_OBJ(objects, T);
	}
	
	template <class T>
	void serialize_obj(hsbase* stream, harray<T*>& objects)
	{
		_LS_INIT_SERIALIZATION;
		LS_SER_HARRAY_OBJ_PTR(objects);
	}
	
	template <class T>
	void deserialize_obj(hsbase* stream, harray<T*>& objects)
	{
		_LS_INIT_DESERIALIZATION;
		LS_DES_HARRAY_OBJ_PTR(objects, T);
	}
	*/
	
	liteserExport extern hmap<unsigned int, Serializable*> _lsIds;

}
#endif
