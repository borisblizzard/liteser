/// @file
/// @version 3.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <stdint.h>

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hsbase.h>
#include <hltypes/hstring.h>
#include <hltypes/hversion.h>

#include "liteser.h"
#include "Serializable.h"
#include "SerializeXml.h"
#include "Type.h"
#include "Utility.h"
#include "Variable.h"

#define WRITE_NODE(string) \
	stream->writeLine(_indent + "<" + hstr(string) + "/>");
#define OPEN_NODE(string) \
	stream->writeLine(_indent + "<" + hstr(string) + ">"); \
	_indent += "\t";
#define CLOSE_NODE(string) \
	_indent = _indent(0, _indent.size() - 1); \
	stream->writeLine(_indent + "</" + hstr(string) + ">");
#define START_LINE(string) \
	stream->write(_indent + "<" + hstr(string));
#define FINISH_LINE(string) \
	stream->writeLine(hstr(string) + "/>");

namespace liteser
{
	namespace xml
	{
		inline bool __dumpVariableStart(Variable* variable)
		{
			bool result = true;
			if (variable->type->identifier == Type::Identifier::Harray || variable->type->identifier == Type::Identifier::Hmap)
			{
				harray<hstr> types;
				foreach (Type*, it, variable->type->subTypes)
				{
					types += hsprintf("%02X", (*it)->identifier.value);
				}
				if (variable->containerSize > 0)
				{
					OPEN_NODE(hsprintf("Variable name=\"%s\" type=\"%02X\" sub_types=\"%s\"", variable->name.cStr(), variable->type->identifier.value, types.joined(',').cStr()));
				}
				else
				{
					WRITE_NODE(hsprintf("Variable name=\"%s\" type=\"%02X\" sub_types=\"%s\"", variable->name.cStr(), variable->type->identifier.value, types.joined(',').cStr()));
				}
			}
			else if (variable->type->identifier == Type::Identifier::ValueObject)
			{
				OPEN_NODE(hsprintf("Variable name=\"%s\" type=\"%02X\"", variable->name.cStr(), variable->type->identifier.value));
			}
			else if (variable->type->identifier == Type::Identifier::Object)
			{
				if (*variable->value<Serializable*>() != NULL)
				{
					OPEN_NODE(hsprintf("Variable name=\"%s\" type=\"%02X\"", variable->name.cStr(), variable->type->identifier.value));
				}
				else
				{
					WRITE_NODE(hsprintf("Variable name=\"%s\" type=\"%02X\"", variable->name.cStr(), variable->type->identifier.value));
					result = false;
				}
			}
			else
			{
				START_LINE(hsprintf("Variable name=\"%s\" type=\"%02X\" value=\"", variable->name.cStr(), variable->type->identifier.value));
			}
			return result;
		}

		inline void __dumpVariableFinish(Variable* variable)
		{
			if (variable->type->identifier == Type::Identifier::Harray || variable->type->identifier == Type::Identifier::Hmap)
			{
				if (variable->containerSize > 0)
				{
					CLOSE_NODE("Variable");
				}
			}
			else if (variable->type->identifier == Type::Identifier::ValueObject)
			{
				CLOSE_NODE("Variable");
			}
			else if (variable->type->identifier == Type::Identifier::Object)
			{
				if (*variable->value<Serializable*>() != NULL)
				{
					CLOSE_NODE("Variable");
				}
			}
			else
			{
				FINISH_LINE("\"");
			}
		}
		
		inline void __dumpVariable(Variable* variable)
		{
			if (variable->type->identifier == Type::Identifier::Int8)				stream->write((int)*variable->value<char>());
			else if (variable->type->identifier == Type::Identifier::Uint8)			stream->write(*variable->value<unsigned char>());
			else if (variable->type->identifier == Type::Identifier::Int16)			stream->write(*variable->value<short>());
			else if (variable->type->identifier == Type::Identifier::Uint16)		stream->write(*variable->value<unsigned short>());
			else if (variable->type->identifier == Type::Identifier::Int32)			stream->write(*variable->value<int>());
			else if (variable->type->identifier == Type::Identifier::Uint32)		stream->write(*variable->value<unsigned int>());
			else if (variable->type->identifier == Type::Identifier::Int64)			stream->write(*variable->value<int64_t>());
			else if (variable->type->identifier == Type::Identifier::Uint64)		stream->write(*variable->value<uint64_t>());
			else if (variable->type->identifier == Type::Identifier::Float)			stream->write(hsprintf("%g", *variable->value<float>()));
			else if (variable->type->identifier == Type::Identifier::Double)		stream->write(hsprintf("%g", *variable->value<double>()));
			else if (variable->type->identifier == Type::Identifier::Bool)			stream->write(*variable->value<bool>());
			else if (variable->type->identifier == Type::Identifier::Hstr)			_dump(variable->value<hstr>());
			else if (variable->type->identifier == Type::Identifier::Hversion)		_dump(variable->value<hversion>());
			else if (variable->type->identifier == Type::Identifier::Henum)			_dump(variable->value<henum>());
			else if (variable->type->identifier == Type::Identifier::Grectf)		_dump(variable->value<grectf>());
			else if (variable->type->identifier == Type::Identifier::Gvec2f)		_dump(variable->value<gvec2f>());
			else if (variable->type->identifier == Type::Identifier::Gvec3f)		_dump(variable->value<gvec3f>());
			else if (variable->type->identifier == Type::Identifier::Grecti)		_dump(variable->value<grecti>());
			else if (variable->type->identifier == Type::Identifier::Gvec2i)		_dump(variable->value<gvec2i>());
			else if (variable->type->identifier == Type::Identifier::Gvec3i)		_dump(variable->value<gvec3i>());
			else if (variable->type->identifier == Type::Identifier::Grectd)		_dump(variable->value<grectd>());
			else if (variable->type->identifier == Type::Identifier::Gvec2d)		_dump(variable->value<gvec2d>());
			else if (variable->type->identifier == Type::Identifier::Gvec3d)		_dump(variable->value<gvec3d>());
			else if (variable->type->identifier == Type::Identifier::ValueObject)	_dump(variable->value<Serializable>());
			else if (variable->type->identifier == Type::Identifier::Object)		_dump(variable->value<Serializable*>());
			else if (variable->type->identifier == Type::Identifier::Harray)		__dumpContainer(variable);
			else if (variable->type->identifier == Type::Identifier::Hmap)			__dumpContainer(variable);
		}

		inline void __dumpContainerVariableStart(Variable* variable)
		{
			if (variable->type->identifier == Type::Identifier::Harray || variable->type->identifier == Type::Identifier::Hmap)
			{
				harray<hstr> types;
				foreach (Type*, it, variable->type->subTypes)
				{
					types += hsprintf("%02X", (*it)->identifier.value);
				}
				if (variable->containerSize > 0)
				{
					OPEN_NODE(hsprintf("Container sub_types=\"%s\"", types.joined(',').cStr()));
				}
				else
				{
					WRITE_NODE(hsprintf("Container sub_types=\"%s\"", types.joined(',').cStr()));
				}
			}
			else if (variable->type->identifier != Type::Identifier::ValueObject && variable->type->identifier != Type::Identifier::Object)
			{
				START_LINE("Element value=\"");
			}
		}

		inline void __dumpContainerVariableFinish(Variable* variable)
		{
			if (variable->type->identifier == Type::Identifier::Harray || variable->type->identifier == Type::Identifier::Hmap)
			{
				if (variable->containerSize > 0)
				{
					CLOSE_NODE("Container");
				}
			}
			else if (variable->type->identifier != Type::Identifier::ValueObject && variable->type->identifier != Type::Identifier::Object)
			{
				FINISH_LINE("\"");
			}
		}

		inline void __dumpContainer(Variable* variable)
		{
			if (variable->containerSize > 0)
			{
				foreach (Variable*, it, variable->subVariables)
				{
					__dumpContainerVariableStart(*it);
					__dumpVariable(*it);
					__dumpContainerVariableFinish(*it);
				}
			}
		}

		void _dump(const hstr* value)
		{
			if (value->size() > 0)
			{
				static std::ustring _chars = hstr("<\"&").uStr();
				static hmap<unsigned int, hstr> _substitutes;
				if (_substitutes.size() == 0)
				{
					_substitutes['<'] = "&lt;";
					_substitutes['"'] = "&quot;";
					_substitutes['&'] = "&amp;";
				}
				harray<hstr> segments;
				int index = 0;
				int foundIndex = -1;
				std::ustring uString = value->uStr();
				while (true)
				{
					foundIndex = (int)uString.find_first_of(_chars, index);
					if (foundIndex < 0)
					{
						break;
					}
					segments += hstr::fromUnicode(uString.substr(index, foundIndex - index).c_str());
					index = foundIndex + 1;
					segments += _substitutes[uString[foundIndex]];
				}
				if (segments.size() > 0)
				{
					int size = (int)uString.size();
					if (index < size)
					{
						segments += hstr::fromUnicode(uString.substr(index, size - index).c_str());
					}
					stream->write(segments.joined(""));
				}
				else
				{
					stream->write(*value);
				}
			}
		}

		void _dump(const hversion* value)
		{
			stream->write(value->toString(4));
		}

		void _dump(const henum* value)
		{
			stream->write(value->value);
		}

		void _dump(const grectf* value)
		{
			stream->write(hsprintf("%g,%g,%g,%g", value->x, value->y, value->w, value->h));
		}

		void _dump(const gvec2f* value)
		{
			stream->write(hsprintf("%g,%g", value->x, value->y));
		}

		void _dump(const gvec3f* value)
		{
			stream->write(hsprintf("%g,%g,%g", value->x, value->y, value->z));
		}

		void _dump(const grecti* value)
		{
			stream->write(hsprintf("%d,%d,%d,%d", value->x, value->y, value->w, value->h));
		}

		void _dump(const gvec2i* value)
		{
			stream->write(hsprintf("%d,%d", value->x, value->y));
		}

		void _dump(const gvec3i* value)
		{
			stream->write(hsprintf("%d,%d,%d", value->x, value->y, value->z));
		}

		void _dump(const grectd* value)
		{
			stream->write(hsprintf("%g,%g,%g,%g", value->x, value->y, value->w, value->h));
		}

		void _dump(const gvec2d* value)
		{
			stream->write(hsprintf("%g,%g", value->x, value->y));
		}

		void _dump(const gvec3d* value)
		{
			stream->write(hsprintf("%g,%g,%g", value->x, value->y, value->z));
		}

		void _dump(Serializable* value)
		{
			unsigned int id = 0;
			if (value == NULL)
			{
				WRITE_NODE("Null");
				return;
			}
			if (!_currentHeader.allowMultiReferencing || __tryMapObject(&id, value))
			{
				value->_lsOnSerializing();
				hstr name = value->_lsName();
				harray<Variable*> variables = value->_lsVars();
				int variablesSize = variables.size();
				if (variablesSize > 0)
				{
					if (_currentHeader.allowMultiReferencing)
					{
						OPEN_NODE(hsprintf("Object name=\"%s\" id=\"%d\"", name.cStr(), id));
					}
					else
					{
						OPEN_NODE(hsprintf("Object name=\"%s\"", name.cStr()));
					}
					foreach (Variable*, it, variables)
					{
						if (__dumpVariableStart(*it))
						{
							__dumpVariable(*it);
							__dumpVariableFinish(*it);
						}
						delete (*it);
					}
					CLOSE_NODE("Object");
				}
				else if (_currentHeader.allowMultiReferencing)
				{
					WRITE_NODE(hsprintf("Object name=\"%s\" id=\"%d\"", name.cStr(), id));
				}
				else
				{
					WRITE_NODE(hsprintf("Object name=\"%s\"", name.cStr()));
				}
				value->_lsOnSerialized();
			}
			else
			{
				WRITE_NODE(hsprintf("Object id=\"%d\"", id));
			}
		}

		void _dump(Serializable** value)
		{
			_dump(*value);
		}

		void _dumpHarray(const harray<Serializable*>* value)
		{
			foreachc (Serializable*, it, *value)
			{
				_dump(*it);
			}
		}

		void _dumpHarray(const harray<char>* value)
		{
			foreachc (char, it, *value)
			{
				START_LINE("Element value=\"");
				stream->write((int)(*it));
				FINISH_LINE("\"");
			}
		}

#define DEFINE_DUMP_HARRAY(type) \
		void _dumpHarray(const harray<type>* value) \
		{ \
			foreachc (type, it, *value) \
			{ \
				START_LINE("Element value=\""); \
				stream->write(*it); \
				FINISH_LINE("\""); \
			} \
		}

#define DEFINE_DUMP_HARRAY_C(type) \
		void _dumpHarray(const harray<type>* value) \
		{ \
			foreachc (type, it, *value) \
			{ \
				START_LINE("Element value=\""); \
				_dump(&(*it)); \
				FINISH_LINE("\""); \
			} \
		}

		DEFINE_DUMP_HARRAY(unsigned char);
		DEFINE_DUMP_HARRAY(short);
		DEFINE_DUMP_HARRAY(unsigned short);
		DEFINE_DUMP_HARRAY(int);
		DEFINE_DUMP_HARRAY(unsigned int);
		DEFINE_DUMP_HARRAY(int64_t);
		DEFINE_DUMP_HARRAY(uint64_t);
		DEFINE_DUMP_HARRAY(float);
		DEFINE_DUMP_HARRAY(double);
		DEFINE_DUMP_HARRAY_C(hstr);
		DEFINE_DUMP_HARRAY_C(hversion);
		DEFINE_DUMP_HARRAY_C(henum);
		DEFINE_DUMP_HARRAY_C(grectf);
		DEFINE_DUMP_HARRAY_C(gvec2f);
		DEFINE_DUMP_HARRAY_C(gvec3f);
		DEFINE_DUMP_HARRAY_C(grecti);
		DEFINE_DUMP_HARRAY_C(gvec2i);
		DEFINE_DUMP_HARRAY_C(gvec3i);
		DEFINE_DUMP_HARRAY_C(grectd);
		DEFINE_DUMP_HARRAY_C(gvec2d);
		DEFINE_DUMP_HARRAY_C(gvec3d);
	}

}
