/// @file
/// @version 3.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hdir.h>
#include <hltypes/hexception.h>
#include <hltypes/hfile.h>
#include <hltypes/hlog.h>
#include <hltypes/hmap.h>
#include <hltypes/hrdir.h>
#include <hltypes/hresource.h>
#include <hltypes/hsbase.h>
#include <hltypes/hstream.h>
#include <hltypes/hstring.h>
#include <hltypes/hversion.h>
#include <hlxml/Document.h>
#include <hlxml/Node.h>

#include "Deserialize.h"
#include "DeserializeXml.h"
#include "Header.h"
#include "liteser.h"
#include "Serializable.h"
#include "Serialize.h"
#include "SerializeXml.h"
#include "Utility.h"
#include "Variable.h"

#define XML_HEADER "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
#define LITESER_XML_ROOT_BEGIN hsprintf("<Liteser version=\"%s\">\n", version.toString(2).cStr())
#define LITESER_XML_ROOT_END "\n</Liteser>"

#define DEFINE_HARRAY_SERIALIZER(type) \
	bool serialize(hsbase* stream, const harray<type>& value, bool allowMultiReferencing, bool stringPooling) \
	{ \
		if (!stream->isOpen()) \
		{ \
			throw FileNotOpenException("Liteser Stream"); \
		} \
		_start(stream); \
		Header header(allowMultiReferencing, stringPooling); \
		_setup(stream, header); \
		_writeHeader(stream, header); \
		_dumpHarray(&value); \
		_finish(stream); \
		return true; \
	}
	
#define DEFINE_HARRAY_DESERIALIZER(type) \
	bool deserialize(hsbase* stream, harray<type>* value) \
	{ \
		if (!stream->isOpen()) \
		{ \
			throw FileNotOpenException("Liteser Stream"); \
		} \
		if (value->size() > 0) \
		{ \
			throw Exception("Output harray is not empty!"); \
		} \
		_start(stream); \
		unsigned int size = 0; \
		Header header; \
		_readHeader(stream, header); \
		_setup(stream, header); \
		_checkVersion(); \
		if (header.version.major > 2 || (header.version.major == 2 && header.version.minor >= 7)) \
		{ \
			if (_loadIdentifier() != Type::Identifier::Harray) \
			{ \
				_finish(stream); \
				throw Exception("Cannot load object from file that does not contain a harray<" #type ">!"); \
			} \
			size = stream->loadUint32(); \
			if (size > 0) \
			{ \
				if (stream->loadUint32() != 1) \
				{ \
					_finish(stream); \
					throw Exception("Cannot load object from file that does not contain a harray<" #type ">!"); \
				} \
				Type subType; \
				subType.assign((VPtr<type>*)NULL); \
				if (_loadIdentifier() != subType.identifier) \
				{ \
					_finish(stream); \
					throw Exception("Cannot load object from file that does not contain a harray<" #type ">!"); \
				} \
			} \
		} \
		else \
		{ \
			size = stream->loadUint32(); \
		} \
		_loadHarray(value, size); \
		_finish(stream); \
		return true; \
	}

#define DEFINE_HARRAY_SERIALIZER_XML(type) \
	bool serialize(hsbase* stream, const harray<type>& value, bool allowMultiReferencing) \
	{ \
		if (!stream->isOpen()) \
		{ \
			throw FileNotOpenException("Liteser XML Stream"); \
		} \
		_start(stream); \
		Header header(allowMultiReferencing, false); \
		_setup(stream, header); \
		stream->writeLine(XML_HEADER); \
		stream->writeLine(LITESER_XML_ROOT_BEGIN); \
		Type subType; \
		subType.assign((VPtr<type>*)NULL); \
		stream->writeLine("\t<Container type=\"" + hsprintf("%02X", Type::Identifier::Harray.value) + "\" sub_types=\"" + hsprintf("%02X", subType.identifier.value) + "\">"); \
		_indent += "\t"; \
		xml::_dumpHarray(&value); \
		_indent = _indent(0, _indent.size() - 1); \
		stream->writeLine("\t</Container>"); \
		stream->writeLine(LITESER_XML_ROOT_END); \
		_finish(stream); \
		return true; \
	}
	
#define DEFINE_HARRAY_DESERIALIZER_XML(type) \
	bool deserialize(hsbase* stream, harray<type>* value) \
	{ \
		if (!stream->isOpen()) \
		{ \
			throw FileNotOpenException("Liteser Stream"); \
		} \
		if (value->size() > 0) \
		{ \
			throw Exception("Output harray is not empty!"); \
		} \
		_start(stream); \
		hlxml::Document doc(*stream); \
		hlxml::Node* root = doc.root(); \
		Header header; \
		_readXmlHeader(root, header); \
		_setup(stream, header); \
		_checkVersion(); \
		hlxml::Node* node = root; \
		if (header.version.major > 2 || (header.version.major == 2 && header.version.minor >= 7)) \
		{ \
			if (root->children.size() != 1 || root->children.first()->name != "Container" || root->children.first()->pstr("type", "00").unhex() != Type::Identifier::Harray.value) \
			{ \
				_finish(stream); \
				throw Exception("Cannot load object from file that does not contain a harray<\"" #type "\">!"); \
			} \
			node = root->children.first(); \
			Type subType; \
			subType.assign((VPtr<type>*)NULL); \
			harray<hstr> subTypes = node->pstr("sub_types", "00").split(',', -1, true); \
			if (subTypes.size() != 1 || subTypes.first().unhex() != subType.identifier.value) \
			{ \
				_finish(stream); \
				throw Exception("Cannot load object from file that does not contain a harray<" #type ">!"); \
			} \
		} \
		xml::_loadHarray(node, value); \
		_finish(stream); \
		return true; \
	}

#define DEFINE_LOAD_HARRAY_FROM_FILE(type) \
	bool loadArrayFromFile(chstr path, harray<type>* dataArray, bool warn) \
	{ \
		hfile stream; \
		return _loadArrayFrom<type>(&stream, &hfile::exists, path, dataArray, warn); \
	}

#define DEFINE_LOAD_HARRAY_FROM_RESOURCE(type) \
	bool loadArrayFromResource(chstr path, harray<type>* dataArray, bool warn) \
	{ \
		hresource stream; \
		return _loadArrayFrom<type>(&stream, &hresource::exists, path, dataArray, warn); \
	}

#define DEFINE_SAVE_HARRAY_TO_FILE(type, allowMultiReferencing) \
	bool saveArrayToFile(chstr path, const harray<type>& dataArray, bool warn) \
	{ \
		try \
		{ \
			hfile file; \
			file.open(path, hfile::AccessMode::Write); \
			serialize(&file, dataArray, allowMultiReferencing, true); \
			file.close(); \
			return true; \
		} \
		catch (hexception& e) \
		{ \
			if (warn) \
			{ \
				hlog::error(logTag, "Could not save: " + path + "\n" + e.getMessage()); \
			} \
		} \
		return false; \
	}

#define DEFINE_SAVE_HARRAY_TO_FILE_XML(type, allowMultiReferencing) \
	bool saveArrayToFile(chstr path, const harray<type>& dataArray, bool warn) \
	{ \
		try \
		{ \
			hfile file; \
			file.open(path, hfile::AccessMode::Write); \
			xml::serialize(&file, dataArray, allowMultiReferencing); \
			file.close(); \
			return true; \
		} \
		catch (hexception& e) \
		{ \
			if (warn) \
			{ \
				hlog::error(logTag, "Could not save: " + path + "\n" + e.getMessage()); \
			} \
		} \
		return false; \
	}

namespace liteser
{
	const hstr logTag = "liteser";
	const hversion version(3, 2);

	bool serialize(hsbase* stream, Serializable* object, bool allowMultiReferencing, bool stringPooling)
	{
		if (!stream->isOpen())
		{
			throw FileNotOpenException("Liteser Stream");
		}
		// TODO - add exception handling
		_start(stream);
		Header header(allowMultiReferencing, stringPooling);
		_setup(stream, header);
		_writeHeader(stream, header);
		_dumpType(Type::Identifier::Object);
		_dump(&object);
		_finish(stream);
		return true;
	}

	DEFINE_HARRAY_SERIALIZER(Serializable*);
	DEFINE_HARRAY_SERIALIZER(char);
	DEFINE_HARRAY_SERIALIZER(unsigned char);
	DEFINE_HARRAY_SERIALIZER(short);
	DEFINE_HARRAY_SERIALIZER(unsigned short);
	DEFINE_HARRAY_SERIALIZER(int);
	DEFINE_HARRAY_SERIALIZER(unsigned int);
	DEFINE_HARRAY_SERIALIZER(int64_t);
	DEFINE_HARRAY_SERIALIZER(uint64_t);
	DEFINE_HARRAY_SERIALIZER(float);
	DEFINE_HARRAY_SERIALIZER(double);
	DEFINE_HARRAY_SERIALIZER(hstr);
	DEFINE_HARRAY_SERIALIZER(hversion);
	DEFINE_HARRAY_SERIALIZER(henum);
	DEFINE_HARRAY_SERIALIZER(grectf);
	DEFINE_HARRAY_SERIALIZER(gvec2f);
	DEFINE_HARRAY_SERIALIZER(gvec3f);
	DEFINE_HARRAY_SERIALIZER(grecti);
	DEFINE_HARRAY_SERIALIZER(gvec2i);
	DEFINE_HARRAY_SERIALIZER(gvec3i);
	DEFINE_HARRAY_SERIALIZER(grectd);
	DEFINE_HARRAY_SERIALIZER(gvec2d);
	DEFINE_HARRAY_SERIALIZER(gvec3d);

	bool deserialize(hsbase* stream, Serializable** object)
	{
		if (!stream->isOpen())
		{
			throw FileNotOpenException("Liteser Stream");
		}
		if (*object != NULL)
		{
			throw Exception("Given pointer to object for deserialization is not NULL.");
		}
		// TODO - add exception handling
		_start(stream);
		Header header;
		_readHeader(stream, header);
		_setup(stream, header);
		_checkVersion();
		if (header.version.major > 2 || (header.version.major == 2 && header.version.minor >= 7)) // this compatibility could be limited to only 2.7 at some point
		{
			Type::Identifier identifier = _loadIdentifier();
			if (identifier != Type::Identifier::Object)
			{
				_finish(stream);
				throw Exception("Cannot load object from file that does not contain an object!");
			}
		}
		_load(object);
		_finish(stream);
		return true;
	}

	DEFINE_HARRAY_DESERIALIZER(Serializable*);
	DEFINE_HARRAY_DESERIALIZER(char);
	DEFINE_HARRAY_DESERIALIZER(unsigned char);
	DEFINE_HARRAY_DESERIALIZER(short);
	DEFINE_HARRAY_DESERIALIZER(unsigned short);
	DEFINE_HARRAY_DESERIALIZER(int);
	DEFINE_HARRAY_DESERIALIZER(unsigned int);
	DEFINE_HARRAY_DESERIALIZER(int64_t);
	DEFINE_HARRAY_DESERIALIZER(uint64_t);
	DEFINE_HARRAY_DESERIALIZER(float);
	DEFINE_HARRAY_DESERIALIZER(double);
	DEFINE_HARRAY_DESERIALIZER(hstr);
	DEFINE_HARRAY_DESERIALIZER(hversion);
	DEFINE_HARRAY_DESERIALIZER(henum);
	DEFINE_HARRAY_DESERIALIZER(grectf);
	DEFINE_HARRAY_DESERIALIZER(gvec2f);
	DEFINE_HARRAY_DESERIALIZER(gvec3f);
	DEFINE_HARRAY_DESERIALIZER(grecti);
	DEFINE_HARRAY_DESERIALIZER(gvec2i);
	DEFINE_HARRAY_DESERIALIZER(gvec3i);
	DEFINE_HARRAY_DESERIALIZER(grectd);
	DEFINE_HARRAY_DESERIALIZER(gvec2d);
	DEFINE_HARRAY_DESERIALIZER(gvec3d);

	namespace xml
	{
		bool serialize(hsbase* stream, Serializable* object, bool allowMultiReferencing)
		{
			if (!stream->isOpen())
			{
				throw FileNotOpenException("Liteser Stream");
			}
			// TODO - add exception handling
			_start(stream);
			Header header(allowMultiReferencing, false);
			_setup(stream, header);
			stream->writeLine(XML_HEADER);
			stream->writeLine(LITESER_XML_ROOT_BEGIN);
			xml::_dump(&object);
			stream->writeLine(LITESER_XML_ROOT_END);
			_finish(stream);
			return true;
		}

		DEFINE_HARRAY_SERIALIZER_XML(Serializable*);
		DEFINE_HARRAY_SERIALIZER_XML(char);
		DEFINE_HARRAY_SERIALIZER_XML(unsigned char);
		DEFINE_HARRAY_SERIALIZER_XML(short);
		DEFINE_HARRAY_SERIALIZER_XML(unsigned short);
		DEFINE_HARRAY_SERIALIZER_XML(int);
		DEFINE_HARRAY_SERIALIZER_XML(unsigned int);
		DEFINE_HARRAY_SERIALIZER_XML(int64_t);
		DEFINE_HARRAY_SERIALIZER_XML(uint64_t);
		DEFINE_HARRAY_SERIALIZER_XML(float);
		DEFINE_HARRAY_SERIALIZER_XML(double);
		DEFINE_HARRAY_SERIALIZER_XML(hstr);
		DEFINE_HARRAY_SERIALIZER_XML(hversion);
		DEFINE_HARRAY_SERIALIZER_XML(henum);
		DEFINE_HARRAY_SERIALIZER_XML(grectf);
		DEFINE_HARRAY_SERIALIZER_XML(gvec2f);
		DEFINE_HARRAY_SERIALIZER_XML(gvec3f);
		DEFINE_HARRAY_SERIALIZER_XML(grecti);
		DEFINE_HARRAY_SERIALIZER_XML(gvec2i);
		DEFINE_HARRAY_SERIALIZER_XML(gvec3i);
		DEFINE_HARRAY_SERIALIZER_XML(grectd);
		DEFINE_HARRAY_SERIALIZER_XML(gvec2d);
		DEFINE_HARRAY_SERIALIZER_XML(gvec3d);

		bool deserialize(hsbase* stream, Serializable** object)
		{
			if (!stream->isOpen())
			{
				throw FileNotOpenException("Liteser Stream");
			}
			if (*object != NULL)
			{
				throw Exception("Given pointer to object for deserialization is not NULL.");
			}
			// TODO - add exception handling
			_start(stream);
			hlxml::Document doc(*stream);
			hlxml::Node* root = doc.root();
			Header header;
			_readXmlHeader(root, header);
			_setup(stream, header);
			_checkVersion();
			hlxml::Node* node = root;
			if (header.version.major > 2 || (header.version.major == 2 && header.version.minor >= 7))
			{
				if (root->children.size() != 1 || root->children.first()->name != "Object")
				{
					_finish(stream);
					throw Exception("Cannot load object from file that does not contain one object!");
				}
				node = root->children.first();
			}
			xml::_load(node, object);
			_finish(stream);
			return true;
		}

		DEFINE_HARRAY_DESERIALIZER_XML(Serializable*);
		DEFINE_HARRAY_DESERIALIZER_XML(char);
		DEFINE_HARRAY_DESERIALIZER_XML(unsigned char);
		DEFINE_HARRAY_DESERIALIZER_XML(short);
		DEFINE_HARRAY_DESERIALIZER_XML(unsigned short);
		DEFINE_HARRAY_DESERIALIZER_XML(int);
		DEFINE_HARRAY_DESERIALIZER_XML(unsigned int);
		DEFINE_HARRAY_DESERIALIZER_XML(int64_t);
		DEFINE_HARRAY_DESERIALIZER_XML(uint64_t);
		DEFINE_HARRAY_DESERIALIZER_XML(float);
		DEFINE_HARRAY_DESERIALIZER_XML(double);
		DEFINE_HARRAY_DESERIALIZER_XML(hstr);
		DEFINE_HARRAY_DESERIALIZER_XML(hversion);
		DEFINE_HARRAY_DESERIALIZER_XML(henum);
		DEFINE_HARRAY_DESERIALIZER_XML(grectf);
		DEFINE_HARRAY_DESERIALIZER_XML(gvec2f);
		DEFINE_HARRAY_DESERIALIZER_XML(gvec3f);
		DEFINE_HARRAY_DESERIALIZER_XML(grecti);
		DEFINE_HARRAY_DESERIALIZER_XML(gvec2i);
		DEFINE_HARRAY_DESERIALIZER_XML(gvec3i);
		DEFINE_HARRAY_DESERIALIZER_XML(grectd);
		DEFINE_HARRAY_DESERIALIZER_XML(gvec2d);
		DEFINE_HARRAY_DESERIALIZER_XML(gvec3d);

	}

	template <typename T>
	inline bool _loadArrayFrom(hfbase* stream, bool (*fileExists)(chstr, bool), chstr path, harray<T>* dataArray, bool warn)
	{
		hstr pathLs3 = path + LITESER_LS3_EXTENSION;
		hstr pathLsx = path + LITESER_LSX_EXTENSION;
		if (fileExists(pathLs3, true))
		{
			hlog::write(logTag, "Loading: " + pathLs3);
			stream->open(pathLs3);
			deserialize(stream, dataArray);
			return true;
		}
		if (fileExists(pathLsx, true))
		{
			hlog::write(logTag, "Loading: " + pathLsx);
			stream->open(pathLsx);
			xml::deserialize(stream, dataArray);
			return true;
		}
		if (warn)
		{
			hlog::warn(logTag, "Could not load: " + path);
		}
		return false;
	}

	DEFINE_LOAD_HARRAY_FROM_FILE(Serializable*);
	DEFINE_LOAD_HARRAY_FROM_FILE(char);
	DEFINE_LOAD_HARRAY_FROM_FILE(unsigned char);
	DEFINE_LOAD_HARRAY_FROM_FILE(short);
	DEFINE_LOAD_HARRAY_FROM_FILE(unsigned short);
	DEFINE_LOAD_HARRAY_FROM_FILE(int);
	DEFINE_LOAD_HARRAY_FROM_FILE(unsigned int);
	DEFINE_LOAD_HARRAY_FROM_FILE(int64_t);
	DEFINE_LOAD_HARRAY_FROM_FILE(uint64_t);
	DEFINE_LOAD_HARRAY_FROM_FILE(float);
	DEFINE_LOAD_HARRAY_FROM_FILE(double);
	DEFINE_LOAD_HARRAY_FROM_FILE(hstr);
	DEFINE_LOAD_HARRAY_FROM_FILE(hversion);
	DEFINE_LOAD_HARRAY_FROM_FILE(henum);
	DEFINE_LOAD_HARRAY_FROM_FILE(grectf);
	DEFINE_LOAD_HARRAY_FROM_FILE(gvec2f);
	DEFINE_LOAD_HARRAY_FROM_FILE(gvec3f);
	DEFINE_LOAD_HARRAY_FROM_FILE(grecti);
	DEFINE_LOAD_HARRAY_FROM_FILE(gvec2i);
	DEFINE_LOAD_HARRAY_FROM_FILE(gvec3i);
	DEFINE_LOAD_HARRAY_FROM_FILE(grectd);
	DEFINE_LOAD_HARRAY_FROM_FILE(gvec2d);
	DEFINE_LOAD_HARRAY_FROM_FILE(gvec3d);

	DEFINE_LOAD_HARRAY_FROM_RESOURCE(Serializable*);
	DEFINE_LOAD_HARRAY_FROM_RESOURCE(char);
	DEFINE_LOAD_HARRAY_FROM_RESOURCE(unsigned char);
	DEFINE_LOAD_HARRAY_FROM_RESOURCE(short);
	DEFINE_LOAD_HARRAY_FROM_RESOURCE(unsigned short);
	DEFINE_LOAD_HARRAY_FROM_RESOURCE(int);
	DEFINE_LOAD_HARRAY_FROM_RESOURCE(unsigned int);
	DEFINE_LOAD_HARRAY_FROM_RESOURCE(int64_t);
	DEFINE_LOAD_HARRAY_FROM_RESOURCE(uint64_t);
	DEFINE_LOAD_HARRAY_FROM_RESOURCE(float);
	DEFINE_LOAD_HARRAY_FROM_RESOURCE(double);
	DEFINE_LOAD_HARRAY_FROM_RESOURCE(hstr);
	DEFINE_LOAD_HARRAY_FROM_RESOURCE(hversion);
	DEFINE_LOAD_HARRAY_FROM_RESOURCE(henum);
	DEFINE_LOAD_HARRAY_FROM_RESOURCE(grectf);
	DEFINE_LOAD_HARRAY_FROM_RESOURCE(gvec2f);
	DEFINE_LOAD_HARRAY_FROM_RESOURCE(gvec3f);
	DEFINE_LOAD_HARRAY_FROM_RESOURCE(grecti);
	DEFINE_LOAD_HARRAY_FROM_RESOURCE(gvec2i);
	DEFINE_LOAD_HARRAY_FROM_RESOURCE(gvec3i);
	DEFINE_LOAD_HARRAY_FROM_RESOURCE(grectd);
	DEFINE_LOAD_HARRAY_FROM_RESOURCE(gvec2d);
	DEFINE_LOAD_HARRAY_FROM_RESOURCE(gvec3d);

	inline bool _loadObjectFrom(hfbase* stream, bool (*fileExists)(chstr, bool), chstr path, liteser::Serializable** object, bool warn)
	{
		hstr pathLs3 = path + LITESER_LS3_EXTENSION;
		hstr pathLsx = path + LITESER_LSX_EXTENSION;
		if (fileExists(pathLs3, true))
		{
			hlog::write(logTag, "Loading: " + pathLs3);
			stream->open(pathLs3);
			deserialize(stream, object);
			return true;
		}
		if (fileExists(pathLsx, true))
		{
			hlog::write(logTag, "Loading: " + pathLsx);
			stream->open(pathLsx);
			xml::deserialize(stream, object);
			return true;
		}
		if (warn)
		{
			hlog::warn(logTag, "Could not load: " + path);
		}
		return false;
	}

	bool loadObjectFromFile(chstr path, liteser::Serializable** object, bool warn)
	{
		hfile stream;
		return _loadObjectFrom(&stream, &hfile::exists, path, object, warn);
	}

	bool loadObjectFromResource(chstr path, liteser::Serializable** object, bool warn)
	{
		hresource stream;
		return _loadObjectFrom(&stream, &hresource::exists, path, object, warn);
	}

	inline bool _loadArrayFromList(const harray<hstr>& files, bool (*loader)(chstr, liteser::Serializable**, bool),
		chstr path, harray<liteser::Serializable*>* dataArray, bool warn)
	{
		harray<hstr> filesLs3;
		harray<hstr> filesLsx;
		foreachc (hstr, it, files)
		{
			if ((*it).endsWith(LITESER_LS3_EXTENSION))
			{
				filesLs3 += hfbase::withoutExtension(*it);
			}
			else if ((*it).endsWith(LITESER_LSX_EXTENSION))
			{
				filesLsx += hfbase::withoutExtension(*it);
			}
		}
		filesLsx /= filesLs3;
		harray<hstr> filenames = filesLs3 + filesLsx;
		liteser::Serializable* object = NULL;
		foreach (hstr, it, filenames)
		{
			object = NULL;
			loader(hdbase::joinPath(path, (*it)), &object, warn);
			dataArray->add(object);
		}
		return true;
	}

	bool loadArrayFromDirectory(chstr path, harray<liteser::Serializable*>* dataArray, bool warn)
	{
		if (hdir::exists(path))
		{
			hlog::write(logTag, "Loading: " + path);
			return _loadArrayFromList(hdir::files(path).sorted(), &loadObjectFromFile, path, dataArray, warn);
		}
		if (warn)
		{
			hlog::warn(logTag, "Could not load: " + path);
		}
		return false;
	}

	bool loadArrayFromResourceDirectory(chstr path, harray<liteser::Serializable*>* dataArray, bool warn)
	{
		if (hrdir::exists(path))
		{
			hlog::write(logTag, "Loading: " + path);
			return _loadArrayFromList(hrdir::files(path).sorted(), &loadObjectFromResource, path, dataArray, warn);
		}
		if (warn)
		{
			hlog::warn(logTag, "Could not load: " + path);
		}
		return false;
	}

	DEFINE_SAVE_HARRAY_TO_FILE(Serializable*, true);
	DEFINE_SAVE_HARRAY_TO_FILE(char, false);
	DEFINE_SAVE_HARRAY_TO_FILE(unsigned char, false);
	DEFINE_SAVE_HARRAY_TO_FILE(short, false);
	DEFINE_SAVE_HARRAY_TO_FILE(unsigned short, false);
	DEFINE_SAVE_HARRAY_TO_FILE(int, false);
	DEFINE_SAVE_HARRAY_TO_FILE(unsigned int, false);
	DEFINE_SAVE_HARRAY_TO_FILE(int64_t, false);
	DEFINE_SAVE_HARRAY_TO_FILE(uint64_t, false);
	DEFINE_SAVE_HARRAY_TO_FILE(float, false);
	DEFINE_SAVE_HARRAY_TO_FILE(double, false);
	DEFINE_SAVE_HARRAY_TO_FILE(hstr, false);
	DEFINE_SAVE_HARRAY_TO_FILE(hversion, false);
	DEFINE_SAVE_HARRAY_TO_FILE(henum, false);
	DEFINE_SAVE_HARRAY_TO_FILE(grectf, false);
	DEFINE_SAVE_HARRAY_TO_FILE(gvec2f, false);
	DEFINE_SAVE_HARRAY_TO_FILE(gvec3f, false);
	DEFINE_SAVE_HARRAY_TO_FILE(grecti, false);
	DEFINE_SAVE_HARRAY_TO_FILE(gvec2i, false);
	DEFINE_SAVE_HARRAY_TO_FILE(gvec3i, false);
	DEFINE_SAVE_HARRAY_TO_FILE(grectd, false);
	DEFINE_SAVE_HARRAY_TO_FILE(gvec2d, false);
	DEFINE_SAVE_HARRAY_TO_FILE(gvec3d, false);

	namespace xml
	{
		DEFINE_SAVE_HARRAY_TO_FILE_XML(Serializable*, true);
		DEFINE_SAVE_HARRAY_TO_FILE_XML(char, false);
		DEFINE_SAVE_HARRAY_TO_FILE_XML(unsigned char, false);
		DEFINE_SAVE_HARRAY_TO_FILE_XML(short, false);
		DEFINE_SAVE_HARRAY_TO_FILE_XML(unsigned short, false);
		DEFINE_SAVE_HARRAY_TO_FILE_XML(int, false);
		DEFINE_SAVE_HARRAY_TO_FILE_XML(unsigned int, false);
		DEFINE_SAVE_HARRAY_TO_FILE_XML(int64_t, false);
		DEFINE_SAVE_HARRAY_TO_FILE_XML(uint64_t, false);
		DEFINE_SAVE_HARRAY_TO_FILE_XML(float, false);
		DEFINE_SAVE_HARRAY_TO_FILE_XML(double, false);
		DEFINE_SAVE_HARRAY_TO_FILE_XML(hstr, false);
		DEFINE_SAVE_HARRAY_TO_FILE_XML(hversion, false);
		DEFINE_SAVE_HARRAY_TO_FILE_XML(henum, false);
		DEFINE_SAVE_HARRAY_TO_FILE_XML(grectf, false);
		DEFINE_SAVE_HARRAY_TO_FILE_XML(gvec2f, false);
		DEFINE_SAVE_HARRAY_TO_FILE_XML(gvec3f, false);
		DEFINE_SAVE_HARRAY_TO_FILE_XML(grecti, false);
		DEFINE_SAVE_HARRAY_TO_FILE_XML(gvec2i, false);
		DEFINE_SAVE_HARRAY_TO_FILE_XML(gvec3i, false);
		DEFINE_SAVE_HARRAY_TO_FILE_XML(grectd, false);
		DEFINE_SAVE_HARRAY_TO_FILE_XML(gvec2d, false);
		DEFINE_SAVE_HARRAY_TO_FILE_XML(gvec3d, false);

	}


	bool clone(Serializable* input, Serializable** output)
	{
		if (*output != NULL)
		{
			throw Exception("Output does not point to NULL!");
		}
		hstream stream;
		_start(&stream);
		Header header(true, true);
		_setup(&stream, header);
		_dump(&input);
		_finish(&stream);
		stream.rewind();
		_start(&stream);
		_setup(&stream, header);
		_load(output);
		_finish(&stream);
		return true;
	}

}
