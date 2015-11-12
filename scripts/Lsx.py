from Object import *
from Type import *
from Util import *
from Variable import *

class Lsx:

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
			variable.value = (node.getAttribute("value") != 0)
		elif loadType == Type.HSTR:
			variable.value = Lsx._loadString(node)
		elif loadType == Type.HVERSION:
			variable.value = Lsx._loadVersion(node)
		elif loadType == Type.HENUM:
			variable.value = Lsx._loadEnum(node)
		elif loadType == Type.GRECT:
			variable.value = Lsx._loadGrect(node)
		elif loadType == Type.GVEC2:
			variable.value = Lsx._loadGvec2(node)
		elif loadType == Type.GVEC3:
			variable.value = Lsx._loadGvec3(node)
		elif loadType == Type.OBJECT:
			variable.value = Lsx._loadObject(node)
		elif loadType == Type.OBJPTR:
			variable.value = Lsx._loadObject(node)
		elif loadType == Type.HARRAY:
			Lsx.__loadContainer(node, variable, loadType)
		elif loadType == Type.HMAP:
			Lsx.__loadContainer(node, variable, loadType)
		else:
			print "ERROR: Load type not recognized!"

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
			for i in xrange(len(variable.subVariables)):
				Lsx.__loadVariable(children[i], variable.subVariables[i], variable.subVariables[i].type.value)

	@staticmethod
	def _loadString(node):
		return node.getAttribute("value")
		
	@staticmethod
	def _loadVersion(node):
		values = node.getAttribute("value").split(".")
		for i in xrange(len(values)):
			values[i] = int(values[i])
		return values

	@staticmethod
	def _loadEnum(node):
		return int(node.getAttribute("value"))

	@staticmethod
	def _loadGrect(node):
		values = node.getAttribute("value").split(",")
		for i in xrange(len(values)):
			values[i] = float(values[i])
		return values

	@staticmethod
	def _loadGvec2(node):
		values = node.getAttribute("value").split(",")
		for i in xrange(len(values)):
			values[i] = float(values[i])
		return values

	@staticmethod
	def _loadGvec3(node):
		values = node.getAttribute("value").split(",")
		for i in xrange(len(values)):
			values[i] = float(values[i])
		return values

	@staticmethod
	def _loadObject(node):
		if node.tagName != "Object":
			node = Util._getChildNodes(node)[0]
		id = int(node.getAttribute("id"))
		result, object = Util._tryGetObject(id)
		if not result:
			className = node.getAttribute("name")
			if object == None:
				object = Object(className)
			Util._tryMapObject(object)
			children = Util._getChildNodes(node)
			#size = Util.loadUint32()
			#for i in xrange(size):
			for child in children:
				variableName = child.getAttribute("name")
				loadType = Lsx._loadType(child)
				variable = Variable(variableName, loadType)
				object.variables.append(variable)
				Lsx.__loadVariable(child, variable, loadType)
		return object
		
	@staticmethod
	def dump(variable):
		Lsx._dumpType(variable.type.value)
		Lsx.__dumpVariable(variable)

	@staticmethod
	def _dumpType(type):
		return Util.dumpUint8(type)
	
	@staticmethod
	def __dumpVariable(variable):
		if variable.type.value == Type.INT8:
			Util.dumpInt8(variable.value)
		elif variable.type.value == Type.UINT8:
			Util.dumpUint8(variable.value)
		elif variable.type.value == Type.INT16:
			Util.dumpInt16(variable.value)
		elif variable.type.value == Type.UINT16:
			Util.dumpUint16(variable.value)
		elif variable.type.value == Type.INT32:
			Util.dumpInt32(variable.value)
		elif variable.type.value == Type.UINT32:
			Util.dumpUint32(variable.value)
		elif variable.type.value == Type.INT64:
			Util.dumpInt64(variable.value)
		elif variable.type.value == Type.UINT64:
			Util.dumpUint64(variable.value)
		elif variable.type.value == Type.FLOAT:
			Util.dumpFloat(variable.value)
		elif variable.type.value == Type.DOUBLE:
			Util.dumpDouble(variable.value)
		elif variable.type.value == Type.BOOL:
			Util.dumpBool(variable.value)
		elif variable.type.value == Type.HSTR:
			Lsx._dumpString(variable.value)
		elif variable.type.value == Type.HVERSION:
			Lsx._dumpVersion(variable.value)
		elif variable.type.value == Type.HENUM:
			Lsx._dumpEnum(variable.value)
		elif variable.type.value == Type.GRECT:
			Lsx._dumpGrect(variable.value)
		elif variable.type.value == Type.GVEC2:
			Lsx._dumpGvec2(variable.value)
		elif variable.type.value == Type.GVEC3:
			Lsx._dumpGvec3(variable.value)
		elif variable.type.value == Type.OBJECT:
			Lsx._dumpObject(variable.value)
		elif variable.type.value == Type.OBJPTR:
			Lsx._dumpObject(variable.value)
		elif variable.type.value == Type.HARRAY:
			Lsx.__dumpContainer(variable)
		elif variable.type.value == Type.HMAP:
			Lsx.__dumpContainer(variable)

	@staticmethod
	def __dumpContainer(variable):
		Util.dumpUint32(variable.containerSize)
		if variable.containerSize > 0:
			Util.dumpUint32(len(variable.type.subTypes))
			for type in variable.type.subTypes:
				Lsx._dumpType(type.value)
			for variable in variable.subVariables:
				Lsx.__dumpVariable(variable)

	@staticmethod
	def _dumpString(value):
		result, id = Util._tryMapString(value)
		Util.dumpUint32(id)
		if result:
			Util.dumpHstr(value)

	@staticmethod
	def _dumpVersion(value):
		for i in value:
			Util.dumpUint32(i)

	@staticmethod
	def _dumpEnum(value):
		Util.dumpUint32(value)

	@staticmethod
	def _dumpGrect(value):
		for i in value:
			Util.dumpFloat(i)

	@staticmethod
	def _dumpGvec2(value):
		for i in value:
			Util.dumpFloat(i)

	@staticmethod
	def _dumpGvec3(value):
		for i in value:
			Util.dumpFloat(i)

	@staticmethod
	def _dumpObject(value):
		result, id = Util._tryMapObject(value)
		Util.dumpUint32(id)
		if result:
			Lsx._dumpString(value.className)
			Util.dumpUint32(len(value.variables))
			for variable in value.variables:
				Lsx._dumpString(variable.name)
				Lsx._dumpType(variable.type.value)
				Lsx.__dumpVariable(variable)

