/// @file
/// @author  Boris Mikic
/// @version 1.0
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
#include <hltypes/hfile.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "liteserExport.h"
#include "Serializable.h"

#define LS_MAKE_SERIALIZABLE \
	bool serialize(hfile* file); \
	bool deserialize(hfile* file);

#define LS_SER_BEGIN(classe, superclass) \
	bool classe::serialize(hfile* file)\
	{ \
		if (!superclass::serialize(file)) \
		{ \
			return false; \
		}
#define LS_SER_END \
		return true; \
	} 

#define LS_DES_BEGIN(classe, superclass) \
	bool classe::deserialize(hfile* file)\
	{ \
		if (!superclass::deserialize(file)) \
		{ \
			return false; \
		}
#define LS_DES_END \
		return true; \
	}

#define LS_SER(name) file->dump(name);
/// @note "type" can be char, uchar, int, uint, long, ulong, short, ushort, bool, float, double or hstr
#define LS_DES(name, type) name = file->load_ ## type();

#define LS_SER_ARRAY(name, size) \
	file->dump(size); \
	for (int _lsI ## name = 0; _lsI ## name < size; _lsI ## name++) \
	{ \
		file->dump(name[_lsI ## name]); \
	}
/// @note "type" can be char, uchar, int, uint, long, ulong, short, ushort, bool, float, double or hstr
/// @note "cppType" can be char, unsigned char, int, unsigned uint, long, unsigned long, short, unsigned short, bool, float, double or hstr
#define LS_DES_ARRAY(name, type, cppType) \
	int _lsCount ## name = file->load_int(); \
	if (name != NULL) \
	{ \
		delete [] name; \
		name = new cppType[_lsCount ## name]; \
	} \
	for (int _lsI ## name = 0; _lsI ## name < _lsCount ## name; _lsI ## name++) \
	{ \
		name[_lsI ## name] = file->load_ ## type(); \
	}

#define LS_SER_HARRAY(name) \
	file->dump(name.size()); \
	for (int _lsI ## name = 0; _lsI ## name < name.size(); _lsI ## name++) \
	{ \
		file->dump(name[_lsI ## name]); \
	}
/// @note "type" can be char, uchar, int, uint, long, ulong, short, ushort, bool, float, double or hstr
#define LS_DES_HARRAY(name, type) \
	name.clear(); \
	int _lsCount ## name = file->load_int(); \
	for (int _lsI ## name = 0; _lsI ## name < _lsCount ## name; _lsI ## name++) \
	{ \
		name += file->load_ ## type(); \
	}

#define LS_SER_OBJ(name) name.serialize(file);
#define LS_DES_OBJ(name) \
	int _lsId ## name = file->load_uint(); \
	liteser::_lsIds[_lsId ## name] = &name; \
	name.deserialize(file);

#define LS_SER_ARRAY_OBJ(name, size) \
	file->dump(size); \
	for (int _lsI ## name = 0; _lsI ## name < size; _lsI ## name++) \
	{ \
		name[_lsI ## name].serialize(file); \
	}
#define LS_DES_ARRAY_OBJ(name, classe) \
	int _lsCount ## name = file->load_int(); \
	if (name != NULL) \
	{ \
		delete [] name; \
		name = new classe[_lsCount ## name]; \
	} \
	for (int _lsI ## name = 0; _lsI ## name < _lsCount ## name; _lsI ## name++) \
	{ \
		classe _lsInstance; \
		_lsInstance.deserialize(file); \
		name[_lsI ## name] = _lsInstance; \
	}

#define LS_SER_HARRAY_OBJ(name) \
	file->dump(name.size()); \
	for (int _lsI ## name = 0; _lsI ## name < name.size(); _lsI ## name++) \
	{ \
		name[_lsI ## name].serialize(file); \
	}
#define LS_DES_HARRAY_OBJ(name, classe) \
	int _lsCount ## name = file->load_int(); \
	for (int _lsI ## name = 0; _lsI ## name < _lsCount ## name; _lsI ## name++) \
	{ \
		classe _lsInstance; \
		_lsInstance.deserialize(file); \
		name += _lsInstance; \
	}

#define LS_SER_OBJ_PTR(name) \
	if (name != NULL) \
	{ \
		name->serialize(file); \
	} \
	else \
	{ \
		file->dump(0); \
	}
#define LS_DES_OBJ_PTR(name, classe) \
	int _lsId ## name = file->load_uint(); \
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
			name->deserialize(file); \
		} \
	} \
	else if (name != NULL) \
	{ \
		delete name; \
		name = NULL; \
	}

#define LS_SER_ARRAY_OBJ_PTR(name, size) \
	file->dump(size); \
	for (int _lsI ## name = 0; _lsI ## name < size; _lsI ## name++) \
	{ \
		LS_SER_OBJ_PTR(name[_lsI ## name]); \
	}
#define LS_DES_ARRAY_OBJ_PTR(name, classe) \
	int _lsCount ## name = file->load_int(); \
	for (int _lsI ## name = 0; _lsI ## name < _lsCount; _lsI ## name++) \
	{ \
		classe* _lsInstance = NULL; \
		LS_DES_OBJ_PTR(_lsInstance, classe); \
		name[_lsI ## name] = _lsInstance; \
	}

#define LS_SER_HARRAY_OBJ_PTR(name) \
	file->dump(name.size()); \
	for (int _lsI ## name = 0; _lsI ## name < name.size(); _lsI ## name++) \
	{ \
		LS_SER_OBJ_PTR(name[_lsI ## name]); \
	}
#define LS_DES_HARRAY_OBJ_PTR(name, classe) \
	int _lsCount ## name = file->load_int(); \
	for (int _lsI ## name = 0; _lsI ## name < _lsCount ## name; _lsI ## name++) \
	{ \
		classe* _lsInstance = NULL; \
		LS_DES_OBJ_PTR(_lsInstance, classe); \
		name += _lsInstance; \
	}

namespace liteser
{
	liteserFnExport void serialize(hfile* file, Serializable* object);
	liteserFnExport void deserialize(hfile* file, Serializable* object);
	liteserFnExport void checkVersion(unsigned char major, unsigned char minor);

	template <class T>
	void serialize(hfile* file, harray<T>& objects)
	{
		_lsIds.clear();
		file->dump((unsigned char)VERSION_MAJOR);
		file->dump((unsigned char)VERSION_MINOR);
		LS_SER_HARRAY_OBJ(objects);
	}
	
	template <class T>
	void deserialize(hfile* file, harray<T>& objects)
	{
		_lsIds.clear();
		unsigned char major = file->load_uchar();
		unsigned char minor = file->load_uchar();
		checkVersion(major, minor);
		LS_DES_HARRAY_OBJ(objects, T);
	}
	
	template <class T>
	void serialize(hfile* file, harray<T*>& objects)
	{
		_lsIds.clear();
		file->dump((unsigned char)VERSION_MAJOR);
		file->dump((unsigned char)VERSION_MINOR);
		LS_SER_HARRAY_OBJ_PTR(objects);
	}
	
	template <class T>
	void deserialize(hfile* file, harray<T*>& objects)
	{
		_lsIds.clear();
		unsigned char major = file->load_uchar();
		unsigned char minor = file->load_uchar();
		checkVersion(major, minor);
		LS_DES_HARRAY_OBJ_PTR(objects, T);
	}
	
	liteserExport extern hmap<unsigned int, Serializable*> _lsIds;

}
#endif
