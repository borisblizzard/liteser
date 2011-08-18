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
		} \
		unsigned int _lsId;
#define LS_SER_END \
		return true; \
	} 

#define LS_DES_BEGIN(classe, superclass) \
	bool classe::deserialize(hfile* file)\
	{ \
		if (!superclass::deserialize(file)) \
		{ \
			return false; \
		} \
		unsigned int _lsId; \
		int _lsCount;
#define LS_DES_END \
		return true; \
	}

#define LS_SER(name) file->dump(name);
/// @note "type" can be char, uchar, int, uint, long, ulong, short, ushort, bool, float, double or hstr
#define LS_DES(name, type) name = file->load_ ## type();

#define LS_SER_ARRAY(name, size) \
	file->dump(size); \
	for (int _lsI = 0; _lsI < size; _lsI++) \
	{ \
		file->dump(name[_lsI]); \
	}
/// @note "type" can be char, uchar, int, uint, long, ulong, short, ushort, bool, float, double or hstr
/// @note "cppType" can be char, unsigned char, int, unsigned uint, long, unsigned long, short, unsigned short, bool, float, double or hstr
#define LS_DES_ARRAY(name, type, cppType) \
	_lsCount = file->load_int(); \
	if (name != NULL) \
	{ \
		delete [] name; \
		name = new cppType[_lsCount]; \
	} \
	for (int _lsI = 0; _lsI < _lsCount; _lsI++) \
	{ \
		name[_lsI] = file->load_ ## type(); \
	}

#define LS_SER_HARRAY(name) \
	file->dump(name.size()); \
	for (int _lsI = 0; _lsI < name.size(); _lsI++) \
	{ \
		file->dump(name[_lsI]); \
	}
/// @note "type" can be char, uchar, int, uint, long, ulong, short, ushort, bool, float, double or hstr
#define LS_DES_HARRAY(name, type) \
	name.clear(); \
	_lsCount = file->load_int(); \
	for (int _lsI = 0; _lsI < _lsCount; _lsI++) \
	{ \
		name += file->load_ ## type(); \
	}

#define LS_SER_OBJECT(name) name.serialize(file);
#define LS_DES_OBJECT(name) \
	_lsId = file->load_uint(); \
	ids[_lsId] = &name; \
	name.deserialize(file);

#define LS_SER_ARRAY_OBJECT(name, size) \
	file->dump(size); \
	for (int _lsI = 0; _lsI < size; _lsI++) \
	{ \
		name[_lsI].serialize(file); \
	}
#define LS_DES_ARRAY_OBJECT(name, classe) \
	_lsCount = file->load_int(); \
	if (name != NULL) \
	{ \
		delete [] name; \
		name = new classe[_lsCount]; \
	} \
	for (int _lsI = 0; _lsI < _lsCount; _lsI++) \
	{ \
		classe _lsInstance; \
		_lsInstance.deserialize(file); \
		name[i] = _lsInstance; \
	}

#define LS_SER_HARRAY_OBJECT(name) \
	file->dump(name.size()); \
	for (int _lsI = 0; _lsI < name.size(); _lsI++) \
	{ \
		name[_lsI].serialize(file); \
	}
#define LS_DES_HARRAY_OBJECT(name, classe) \
	_lsCount = file->load_int(); \
	for (int _lsI = 0; _lsI < _lsCount; _lsI++) \
	{ \
		classe _lsInstance; \
		_lsInstance.deserialize(file); \
		name += _lsInstance; \
	}

#define LS_SER_OBJECT_PTR(name) \
	if (name != NULL) \
	{ \
		name->serialize(file); \
	} \
	else \
	{ \
		file->dump(0); \
	}
#define LS_DES_OBJECT_PTR(name, classe) \
	_lsId = file->load_uint(); \
	if (_lsId != 0) \
	{ \
		if (ids.has_key(_lsId)) \
		{ \
			name = (classe*)ids[_lsId]; \
		} \
		else \
		{ \
			if (name == NULL) \
			{ \
				name = new classe(); \
			} \
			ids[_lsId] = name; \
			name->deserialize(file); \
		} \
	} \
	else if (name != NULL) \
	{ \
		delete name; \
		name = NULL; \
	}

#define LS_SER_ARRAY_OBJECT_PTR(name, size) \
	file->dump(size); \
	for (int _lsI = 0; _lsI < size; _lsI++) \
	{ \
		LS_SER_OBJECT_PTR(name[_lsI]); \
	}
#define LS_DES_ARRAY_OBJECT_PTR(name, classe) \
	_lsCount = file->load_int(); \
	for (int _lsI = 0; _lsI < _lsCount; _lsI++) \
	{ \
		classe* _lsInstance = NULL; \
		LS_DES_OBJECT_PTR(_lsInstance, classe); \
		name[_lsI] = _lsInstance; \
	}

#define LS_SER_HARRAY_OBJECT_PTR(name) \
	file->dump(name.size()); \
	for (int _lsI = 0; _lsI < name.size(); _lsI++) \
	{ \
		LS_SER_OBJECT_PTR(name[_lsI]); \
	}
#define LS_DES_HARRAY_OBJECT_PTR(name, classe) \
	_lsCount = file->load_int(); \
	for (int _lsI = 0; _lsI < _lsCount; _lsI++) \
	{ \
		classe* _lsInstance = NULL; \
		LS_DES_OBJECT_PTR(_lsInstance, classe); \
		name += _lsInstance; \
	}

namespace liteser
{
	liteserFnExport void serialize(hfile* file, Serializable* object);
	liteserFnExport void deserialize(hfile* file, Serializable* object);

}
#endif
