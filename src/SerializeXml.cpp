/// @file
/// @version 2.7
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <stdint.h>

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
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
		inline void __dumpVariableStart(Variable* variable)
		{
			if (variable->type->value == Type::HARRAY || variable->type->value == Type::HMAP)
			{
				harray<hstr> types;
				foreach (Type*, it, variable->type->subTypes)
				{
					types += hsprintf("%02X", (*it)->value);
				}
				if (variable->containerSize > 0)
				{
					OPEN_NODE(hsprintf("Variable name=\"%s\" type=\"%02X\" sub_types=\"%s\"", variable->name.cStr(), variable->type->value, types.joined(',').cStr()));
				}
				else
				{
					WRITE_NODE(hsprintf("Variable name=\"%s\" type=\"%02X\" sub_types=\"%s\"", variable->name.cStr(), variable->type->value, types.joined(',').cStr()));
				}
			}
			else if (variable->type->value == Type::OBJECT || variable->type->value == Type::OBJPTR)
			{
				OPEN_NODE(hsprintf("Variable name=\"%s\" type=\"%02X\"", variable->name.cStr(), variable->type->value));
			}
			else
			{
				START_LINE(hsprintf("Variable name=\"%s\" type=\"%02X\" value=\"", variable->name.cStr(), variable->type->value));
			}
		}

		inline void __dumpVariableFinish(Variable* variable)
		{
			if (variable->type->value == Type::HARRAY || variable->type->value == Type::HMAP)
			{
				if (variable->containerSize > 0)
				{
					CLOSE_NODE("Variable");
				}
			}
			else if (variable->type->value == Type::OBJECT || variable->type->value == Type::OBJPTR)
			{
				CLOSE_NODE("Variable");
			}
			else
			{
				FINISH_LINE("\"");
			}
		}
		
		inline void __dumpVariable(Variable* variable)
		{
			switch (variable->type->value)
			{
			case Type::INT8:		stream->write((int)*variable->value<char>());				break;
			case Type::UINT8:		stream->write(*variable->value<unsigned char>());			break;
			case Type::INT16:		stream->write(*variable->value<short>());					break;
			case Type::UINT16:		stream->write(*variable->value<unsigned short>());			break;
			case Type::INT32:		stream->write(*variable->value<int>());						break;
			case Type::UINT32:		stream->write(*variable->value<unsigned int>());			break;
			case Type::INT64:		stream->write(*variable->value<int64_t>());					break;
			case Type::UINT64:		stream->write(*variable->value<uint64_t>());				break;
			case Type::FLOAT:		stream->write(hsprintf("%g", *variable->value<float>()));	break;
			case Type::DOUBLE:		stream->write(hsprintf("%g", *variable->value<double>()));	break;
			case Type::BOOL:		stream->write(*variable->value<bool>());					break;
			case Type::HSTR:		_dump(variable->value<hstr>());								break;
			case Type::HVERSION:	_dump(variable->value<hversion>());							break;
			case Type::HENUM:		_dump(variable->value<henum>());							break;
			case Type::GRECT:		_dump(variable->value<grect>());							break;
			case Type::GVEC2:		_dump(variable->value<gvec2>());							break;
			case Type::GVEC3:		_dump(variable->value<gvec3>());							break;
			case Type::OBJECT:		_dump(variable->value<Serializable>());						break;
			case Type::OBJPTR:		_dump(variable->value<Serializable*>());					break;
			case Type::HARRAY:		__dumpContainer(variable);									break;
			case Type::HMAP:		__dumpContainer(variable);									break;
			default:																			break;
			}
		}

		inline void __dumpContainerVariableStart(Variable* variable)
		{
			if (variable->type->value == Type::HARRAY || variable->type->value == Type::HMAP)
			{
				harray<hstr> types;
				foreach (Type*, it, variable->type->subTypes)
				{
					types += hsprintf("%02X", (*it)->value);
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
			else if (variable->type->value != Type::OBJECT && variable->type->value != Type::OBJPTR)
			{
				START_LINE("Element value=\"");
			}
		}

		inline void __dumpContainerVariableFinish(Variable* variable)
		{
			if (variable->type->value == Type::HARRAY || variable->type->value == Type::HMAP)
			{
				if (variable->containerSize > 0)
				{
					CLOSE_NODE("Container");
				}
			}
			else if (variable->type->value != Type::OBJECT && variable->type->value != Type::OBJPTR)
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

		void _dump(hstr* value)
		{
			stream->write(*value);
		}

		void _dump(hversion* value)
		{
			stream->write(value->toString(4));
		}

		void _dump(henum* value)
		{
			stream->write(value->value);
		}

		void _dump(grect* value)
		{
			stream->write(hsprintf("%g,%g,%g,%g", value->x, value->y, value->w, value->h));
		}

		void _dump(gvec2* value)
		{
			stream->write(hsprintf("%g,%g", value->x, value->y));
		}

		void _dump(gvec3* value)
		{
			stream->write(hsprintf("%g,%g,%g", value->x, value->y, value->z));
		}

		void _dump(Serializable* value)
		{
			unsigned int id;
			if (__tryMapObject(&id, value))
			{
				hstr name = value->_lsName();
				harray<Variable*> variables = value->_lsVars();
				int variablesSize = variables.size();
				if (variablesSize > 0)
				{
					OPEN_NODE(hsprintf("Object name=\"%s\" id=\"%d\"", name.cStr(), id));
					foreach (Variable*, it, variables)
					{
						__dumpVariableStart(*it);
						__dumpVariable(*it);
						__dumpVariableFinish(*it);
						delete (*it);
					}
					CLOSE_NODE("Object");
				}
				else
				{
					WRITE_NODE(hsprintf("Object name=\"%s\" id=\"%d\"", name.cStr(), id));
				}
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

		void _dumpHarray(harray<Serializable*>* value)
		{
			foreach (Serializable*, it, *value)
			{
				_dump(*it);
			}
		}

		void _dumpHarray(harray<char>* value)
		{
			foreach (char, it, *value)
			{
				START_LINE("Element value=\"");
				stream->write((int)(*it));
				FINISH_LINE("\"");
			}
		}

#define DEFINE_DUMP_HARRAY(type) \
		void _dumpHarray(harray<type>* value) \
		{ \
			foreach (type, it, *value) \
			{ \
				START_LINE("Element value=\""); \
				stream->write(*it); \
				FINISH_LINE("\""); \
			} \
		}

#define DEFINE_DUMP_HARRAY_C(type) \
		void _dumpHarray(harray<type>* value) \
		{ \
			foreach (type, it, *value) \
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
		DEFINE_DUMP_HARRAY_C(grect);
		DEFINE_DUMP_HARRAY_C(gvec2);
		DEFINE_DUMP_HARRAY_C(gvec3);
	}

}
