from .Object import *
from .Type import *
from .Util import *
from .Variable import *

class Lsx:

	XML_HEADER = b"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
	LITESER_XML_ROOT_BEGIN = b"<Liteser version=\"%d.%d\">\n\t\n" % (Util.VersionMajor, Util.VersionMinor)
	LITESER_XML_ROOT_END = b"\t\n</Liteser>\n"
	
	@staticmethod
	def load(node):
		if node.tagName != "Container":
			type = Type.OBJPTR
		else:
			type = Lsx._loadType(node)
		variable = Variable("", type)
		Lsx.__loadVariable(node, variable, type)
		return variable

	@staticmethod
	def _loadType(node):
		return int(node.getAttribute("type"), 16)
	
	@staticmethod
	def __loadVariable(node, variable, loadType):
		if loadType == Type.INT8:
			variable.value = int(node.getAttribute("value"))
		elif loadType == Type.UINT8:
			variable.value = int(node.getAttribute("value"))
		elif loadType == Type.INT16:
			variable.value = int(node.getAttribute("value"))
		elif loadType == Type.UINT16:
			variable.value = int(node.getAttribute("value"))
		elif loadType == Type.INT32:
			variable.value = int(node.getAttribute("value"))
		elif loadType == Type.UINT32:
			variable.value = int(node.getAttribute("value"))
		elif loadType == Type.INT64:
			variable.value = int(node.getAttribute("value"))
		elif loadType == Type.UINT64:
			variable.value = int(node.getAttribute("value"))
		elif loadType == Type.FLOAT:
			variable.value = float(node.getAttribute("value"))
		elif loadType == Type.DOUBLE:
			variable.value = float(node.getAttribute("value"))
		elif loadType == Type.BOOL:
			variable.value = (node.getAttribute("value") != "0")
		elif loadType == Type.HSTR:
			variable.value = Lsx._loadString(node)
		elif loadType == Type.HVERSION:
			variable.value = Lsx._loadVersion(node)
		elif loadType == Type.HENUM:
			variable.value = Lsx._loadEnum(node)
		elif loadType == Type.GRECTF:
			variable.value = Lsx._loadGrectf(node)
		elif loadType == Type.GVEC2F:
			variable.value = Lsx._loadGvec2f(node)
		elif loadType == Type.GVEC3F:
			variable.value = Lsx._loadGvec3f(node)
		elif loadType == Type.GRECTI:
			variable.value = Lsx._loadGrecti(node)
		elif loadType == Type.GVEC2I:
			variable.value = Lsx._loadGvec2i(node)
		elif loadType == Type.GVEC3I:
			variable.value = Lsx._loadGvec3i(node)
		elif loadType == Type.GRECTD:
			variable.value = Lsx._loadGrectd(node)
		elif loadType == Type.GVEC2D:
			variable.value = Lsx._loadGvec2d(node)
		elif loadType == Type.GVEC3D:
			variable.value = Lsx._loadGvec3d(node)
		elif loadType == Type.OBJECT:
			variable.value = Lsx._loadObject(node)
		elif loadType == Type.OBJPTR:
			variable.value = Lsx._loadObject(node)
		elif loadType == Type.HARRAY:
			Lsx.__loadContainer(node, variable, loadType)
		elif loadType == Type.HMAP:
			Lsx.__loadContainer(node, variable, loadType)
		else:
			print("ERROR: Load type not recognized!")

	@staticmethod
	def __loadContainer(node, variable, type):
		children = Util._getChildNodes(node)
		variable.containerSize = len(children)
		if variable.containerSize > 0:
			subTypes = node.getAttribute("sub_types").split(",")
			for subType in subTypes:
				loadType = int(subType, 16)
				if loadType == Type.HARRAY or loadType == Type.HMAP:
					raise Exception("Template container within a template container detected, not supported: %02X" % loadType)
				variable.type.subTypes.append(Type(loadType))
			variable.createSubVariables()
			for i in range(len(variable.subVariables)):
				Lsx.__loadVariable(children[i], variable.subVariables[i], variable.subVariables[i].type.value)
			if len(variable.type.subTypes) > 1: # requires this correction due to HMAP having subcontainers
				variable.containerSize = variable.subVariables[0].containerSize

	@staticmethod
	def _loadString(node):
		return node.getAttribute("value")
		
	@staticmethod
	def _loadVersion(node):
		values = node.getAttribute("value").split(".")
		for i in range(len(values)):
			values[i] = int(values[i])
		return values

	@staticmethod
	def _loadEnum(node):
		return int(node.getAttribute("value"))

	@staticmethod
	def _loadGrectf(node):
		values = node.getAttribute("value").split(",")
		for i in range(len(values)):
			values[i] = float(values[i])
		return values

	@staticmethod
	def _loadGvec2f(node):
		values = node.getAttribute("value").split(",")
		for i in range(len(values)):
			values[i] = float(values[i])
		return values

	@staticmethod
	def _loadGvec3f(node):
		values = node.getAttribute("value").split(",")
		for i in range(len(values)):
			values[i] = float(values[i])
		return values

	@staticmethod
	def _loadGrecti(node):
		values = node.getAttribute("value").split(",")
		for i in range(len(values)):
			values[i] = int(values[i])
		return values

	@staticmethod
	def _loadGvec2i(node):
		values = node.getAttribute("value").split(",")
		for i in range(len(values)):
			values[i] = int(values[i])
		return values

	@staticmethod
	def _loadGvec3i(node):
		values = node.getAttribute("value").split(",")
		for i in range(len(values)):
			values[i] = int(values[i])
		return values

	@staticmethod
	def _loadGrectd(node):
		values = node.getAttribute("value").split(",")
		for i in range(len(values)):
			values[i] = float(values[i])
		return values

	@staticmethod
	def _loadGvec2d(node):
		values = node.getAttribute("value").split(",")
		for i in range(len(values)):
			values[i] = float(values[i])
		return values

	@staticmethod
	def _loadGvec3d(node):
		values = node.getAttribute("value").split(",")
		for i in range(len(values)):
			values[i] = float(values[i])
		return values

	@staticmethod
	def _loadObject(node):
		if node.tagName != "Object" and node.tagName != "Null":
			children = Util._getChildNodes(node)
			if len(children) == 0:
				return None
			node = children[0]
		if node.tagName == "Null":
			return None
		id = 0
		idExists = node.hasAttribute("id")
		if idExists:
			id = int(node.getAttribute("id"))
		result, object = Util._tryGetObject(id, idExists)
		if not result:
			className = node.getAttribute("name")
			if object == None:
				object = Object(className)
			Util._tryMapObject(object)
			children = Util._getChildNodes(node)
			for child in children:
				variableName = child.getAttribute("name")
				loadType = Lsx._loadType(child)
				variable = Variable(variableName, loadType)
				object.variables.append(variable)
				Lsx.__loadVariable(child, variable, loadType)
		return object
		
	@staticmethod
	def dump(variable):
		if variable.type.value == Type.OBJPTR:
			Lsx.__dumpVariable(variable)
		else:
			Util._openNode("Container type=\"%02X\" sub_types=\"%02X\"" % (variable.type.value, variable.type.subTypes[0].value))
			if variable.type.subTypes[0].value == Type.OBJPTR:
				for subVariable in variable.subVariables:
					Lsx.__dumpVariable(subVariable)
			else:
				for subVariable in variable.subVariables:
					Util._startLine("Element value=\"")
					Lsx.__dumpVariable(subVariable)
					Util._finishLine("\"")
			Util._closeNode("Container")

	@staticmethod
	def __dumpVariableStart(variable):
		result = True
		if variable.type.value == Type.HARRAY or variable.type.value == Type.HMAP:
			types = []
			for type in variable.type.subTypes:
				types.append("%02X" % type.value)
			if variable.containerSize > 0:
				Util._openNode("Variable name=\"%s\" type=\"%02X\" sub_types=\"%s\"" % (variable.name, variable.type.value, ",".join(types)))
			else:
				Util._writeNode("Variable name=\"%s\" type=\"%02X\" sub_types=\"%s\"" % (variable.name, variable.type.value, ",".join(types)))
		elif variable.type.value == Type.OBJECT:
			Util._openNode("Variable name=\"%s\" type=\"%02X\"" % (variable.name, variable.type.value))
		elif variable.type.value == Type.OBJPTR:
			if variable.value != None:
				Util._openNode("Variable name=\"%s\" type=\"%02X\"" % (variable.name, variable.type.value))
			else:
				Util._writeNode("Variable name=\"%s\" type=\"%02X\"" % (variable.name, variable.type.value))
				result = False
		else:
			Util._startLine("Variable name=\"%s\" type=\"%02X\" value=\"" % (variable.name, variable.type.value))
		return result

	@staticmethod
	def __dumpVariableFinish(variable):
		if variable.type.value == Type.HARRAY or variable.type.value == Type.HMAP:
			if variable.containerSize > 0:
				Util._closeNode("Variable");
		elif variable.type.value == Type.OBJECT or variable.type.value == Type.OBJPTR:
			Util._closeNode("Variable")
		else:
			Util._finishLine("\"")
	
	@staticmethod
	def __dumpVariable(variable):
		if variable.type.value == Type.INT8:
			Util._write(str(variable.value))
		elif variable.type.value == Type.UINT8:
			Util._write(str(variable.value))
		elif variable.type.value == Type.INT16:
			Util._write(str(variable.value))
		elif variable.type.value == Type.UINT16:
			Util._write(str(variable.value))
		elif variable.type.value == Type.INT32:
			Util._write(str(variable.value))
		elif variable.type.value == Type.UINT32:
			Util._write(str(variable.value))
		elif variable.type.value == Type.INT64:
			Util._write(str(variable.value))
		elif variable.type.value == Type.UINT64:
			Util._write(str(variable.value))
		elif variable.type.value == Type.FLOAT:
			Util._write("%g" % variable.value)
		elif variable.type.value == Type.DOUBLE:
			Util._write("%g" % variable.value)
		elif variable.type.value == Type.BOOL:
			value = 0
			if variable.value:
				value = 1
			Util._write(str(value))
		elif variable.type.value == Type.HSTR:
			Lsx._dumpString(variable.value)
		elif variable.type.value == Type.HVERSION:
			Lsx._dumpVersion(variable.value)
		elif variable.type.value == Type.HENUM:
			Lsx._dumpEnum(variable.value)
		elif variable.type.value == Type.GRECTF:
			Lsx._dumpGrectf(variable.value)
		elif variable.type.value == Type.GVEC2F:
			Lsx._dumpGvec2f(variable.value)
		elif variable.type.value == Type.GVEC3F:
			Lsx._dumpGvec3f(variable.value)
		elif variable.type.value == Type.GRECTI:
			Lsx._dumpGrecti(variable.value)
		elif variable.type.value == Type.GVEC2I:
			Lsx._dumpGvec2i(variable.value)
		elif variable.type.value == Type.GVEC3I:
			Lsx._dumpGvec3i(variable.value)
		elif variable.type.value == Type.GRECTD:
			Lsx._dumpGrectd(variable.value)
		elif variable.type.value == Type.GVEC2D:
			Lsx._dumpGvec2d(variable.value)
		elif variable.type.value == Type.GVEC3D:
			Lsx._dumpGvec3d(variable.value)
		elif variable.type.value == Type.OBJECT:
			Lsx._dumpObject(variable.value)
		elif variable.type.value == Type.OBJPTR:
			Lsx._dumpObject(variable.value)
		elif variable.type.value == Type.HARRAY:
			Lsx.__dumpContainer(variable)
		elif variable.type.value == Type.HMAP:
			Lsx.__dumpContainer(variable)

	@staticmethod
	def __dumpContainerVariableStart(variable):
		if variable.type.value == Type.HARRAY or variable.type.value == Type.HMAP:
			types = []
			for type in variable.type.subTypes:
				types.append("%02X" % type.value)
			if variable.containerSize > 0:
				Util._openNode("Container sub_types=\"%s\"" % ",".join(types))
			else:
				Util._writeNode("Container sub_types=\"%s\"" % ",".join(types))
		elif variable.type.value != Type.OBJECT and variable.type.value != Type.OBJPTR:
			Util._startLine("Element value=\"")

	@staticmethod
	def __dumpContainerVariableFinish(variable):
		if variable.type.value == Type.HARRAY or variable.type.value == Type.HMAP:
			if variable.containerSize > 0:
				Util._closeNode("Container")
		elif variable.type.value != Type.OBJECT and variable.type.value != Type.OBJPTR:
			Util._finishLine("\"")

	@staticmethod
	def __dumpContainer(variable):
		if variable.containerSize > 0:
			for subVariable in variable.subVariables:
				Lsx.__dumpContainerVariableStart(subVariable)
				Lsx.__dumpVariable(subVariable)
				Lsx.__dumpContainerVariableFinish(subVariable)

	@staticmethod
	def _dumpString(value):
		Util._write(value)

	@staticmethod
	def _dumpVersion(value):
		values = []
		for i in value:
			values.append(str(i))
		Util._write(".".join(values))

	@staticmethod
	def _dumpEnum(value):
		Util._write(str(value))

	@staticmethod
	def _dumpGrectf(value):
		values = []
		for i in value:
			values.append("%g" % i)
		Util._write(",".join(values))

	@staticmethod
	def _dumpGvec2f(value):
		values = []
		for i in value:
			values.append("%g" % i)
		Util._write(",".join(values))

	@staticmethod
	def _dumpGvec3f(value):
		values = []
		for i in value:
			values.append("%g" % i)
		Util._write(",".join(values))

	@staticmethod
	def _dumpGrecti(value):
		values = []
		for i in value:
			values.append("%d" % i)
		Util._write(",".join(values))

	@staticmethod
	def _dumpGvec2i(value):
		values = []
		for i in value:
			values.append("%d" % i)
		Util._write(",".join(values))

	@staticmethod
	def _dumpGvec3i(value):
		values = []
		for i in value:
			values.append("%d" % i)
		Util._write(",".join(values))

	@staticmethod
	def _dumpGrectd(value):
		values = []
		for i in value:
			values.append("%g" % i)
		Util._write(",".join(values))

	@staticmethod
	def _dumpGvec2d(value):
		values = []
		for i in value:
			values.append("%g" % i)
		Util._write(",".join(values))

	@staticmethod
	def _dumpGvec3d(value):
		values = []
		for i in value:
			values.append("%g" % i)
		Util._write(",".join(values))

	@staticmethod
	def _dumpObject(value):
		result, id = Util._tryMapObject(value)
		if result:
			if len(value.variables) > 0:
				if Util._allowMultiReferencing:
					Util._openNode("Object name=\"%s\" id=\"%d\"" % (value.className, id))
				else:
					Util._openNode("Object name=\"%s\"" % value.className)
				for variable in value.variables:
					if Lsx.__dumpVariableStart(variable):
						Lsx.__dumpVariable(variable)
						Lsx.__dumpVariableFinish(variable)
				Util._closeNode("Object");
			elif Util._allowMultiReferencing:
				Util._writeNode("Object name=\"%s\" id=\"%d\"" % (value.className, id))
			else:
				Util._writeNode("Object name=\"%s\"" % value.className)
		else:
			Util._writeNode("Object id=\"%d\"" % id)

