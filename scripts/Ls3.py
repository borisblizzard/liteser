from Object import *
from Type import *
from Util import *
from Variable import *

class Ls3:

	@staticmethod
	def load():
		type = Ls3._loadType()
		variable = Variable("", type)
		Ls3.__loadVariable(variable, type)
		return variable

	@staticmethod
	def _loadType():
		return Util.loadUint8()
	
	@staticmethod
	def __loadVariable(variable, loadType):
		if loadType == Type.INT8:
			variable.value = Util.loadInt8()
		elif loadType == Type.UINT8:
			variable.value = Util.loadUint8()
		elif loadType == Type.INT16:
			variable.value = Util.loadInt16()
		elif loadType == Type.UINT16:
			variable.value = Util.loadUint16()
		elif loadType == Type.INT32:
			variable.value = Util.loadInt32()
		elif loadType == Type.UINT32:
			variable.value = Util.loadUint32()
		elif loadType == Type.INT64:
			variable.value = Util.loadInt64()
		elif loadType == Type.UINT64:
			variable.value = Util.loadUint64()
		elif loadType == Type.FLOAT:
			variable.value = Util.loadFloat()
		elif loadType == Type.DOUBLE:
			variable.value = Util.loadDouble()
		elif loadType == Type.BOOL:
			variable.value = Util.loadBool()
		elif loadType == Type.HSTR:
			variable.value = Ls3._loadString()
		elif loadType == Type.HVERSION:
			variable.value = Ls3._loadVersion()
		elif loadType == Type.HENUM:
			variable.value = Ls3._loadEnum()
		elif loadType == Type.GRECT:
			variable.value = Ls3._loadGrect()
		elif loadType == Type.GVEC2:
			variable.value = Ls3._loadGvec2()
		elif loadType == Type.GVEC3:
			variable.value = Ls3._loadGvec3()
		elif loadType == Type.OBJECT:
			variable.value = Ls3._loadObject()
		elif loadType == Type.OBJPTR:
			variable.value = Ls3._loadObject()
		elif loadType == Type.HARRAY:
			Ls3.__loadContainer(variable, loadType)
		elif loadType == Type.HMAP:
			Ls3.__loadContainer(variable, loadType)
		else:
			print "ERROR: Load type not recognized!"

	@staticmethod
	def __loadContainer(variable, type):
		variable.containerSize = Util.loadUint32()
		if variable.containerSize > 0:
			typeSize = Util.loadUint32()
			for i in xrange(typeSize):
				loadType = Ls3._loadType()
				if loadType == Type.HARRAY or loadType == Type.HMAP:
					raise Exception("Template container within a template container detected, not supported: %02X" % loadType)
				variable.type.subTypes.append(Type(loadType))
			variable.createSubVariables()
			for subVariable in variable.subVariables:
				Ls3.__loadVariable(subVariable, subVariable.type.value)

	@staticmethod
	def _loadString():
		if not Util._stringPooling:
			return Util.loadHstr()
		id = Util.loadUint32()
		result, value = Util._tryGetString(id)
		if not result:
			value = Util.loadHstr()
			Util._tryMapString(value)
		return value
		
	@staticmethod
	def _loadVersion():
		return [Util.loadUint32(), Util.loadUint32(), Util.loadUint32(), Util.loadUint32()]

	@staticmethod
	def _loadEnum():
		return Util.loadUint32()

	@staticmethod
	def _loadGrect():
		return [Util.loadFloat(), Util.loadFloat(), Util.loadFloat(), Util.loadFloat()]

	@staticmethod
	def _loadGvec2():
		return [Util.loadFloat(), Util.loadFloat()]

	@staticmethod
	def _loadGvec3():
		return [Util.loadFloat(), Util.loadFloat(), Util.loadFloat()]

	@staticmethod
	def _loadObject():
		id = 0
		result = False
		object = None
		if Util._allowMultiReferencing:
			id = Util.loadUint32()
			result, object = Util._tryGetObject(id)
		if not result:
			className = Ls3._loadString()
			if object == None:
				object = Object(className)
			if Util._allowMultiReferencing:
				Util._tryMapObject(object)
			size = Util.loadUint32()
			for i in xrange(size):
				variableName = Ls3._loadString()
				loadType = Ls3._loadType()
				variable = Variable(variableName, loadType)
				object.variables.append(variable)
				Ls3.__loadVariable(variable, loadType)
		return object
		
	@staticmethod
	def dump(variable):
		Ls3._dumpType(variable.type.value)
		Ls3.__dumpVariable(variable)

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
			Ls3._dumpString(variable.value)
		elif variable.type.value == Type.HVERSION:
			Ls3._dumpVersion(variable.value)
		elif variable.type.value == Type.HENUM:
			Ls3._dumpEnum(variable.value)
		elif variable.type.value == Type.GRECT:
			Ls3._dumpGrect(variable.value)
		elif variable.type.value == Type.GVEC2:
			Ls3._dumpGvec2(variable.value)
		elif variable.type.value == Type.GVEC3:
			Ls3._dumpGvec3(variable.value)
		elif variable.type.value == Type.OBJECT:
			Ls3._dumpObject(variable.value)
		elif variable.type.value == Type.OBJPTR:
			Ls3._dumpObject(variable.value)
		elif variable.type.value == Type.HARRAY:
			Ls3.__dumpContainer(variable)
		elif variable.type.value == Type.HMAP:
			Ls3.__dumpContainer(variable)

	@staticmethod
	def __dumpContainer(variable):
		Util.dumpUint32(variable.containerSize)
		if variable.containerSize > 0:
			Util.dumpUint32(len(variable.type.subTypes))
			for type in variable.type.subTypes:
				Ls3._dumpType(type.value)
			for subVariable in variable.subVariables:
				Ls3.__dumpVariable(subVariable)

	@staticmethod
	def _dumpString(value):
		result = True
		if Util._stringPooling:
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
		result = True
		if Util._allowMultiReferencing:
			result, id = Util._tryMapObject(value)
			Util.dumpUint32(id)
		if result:
			Ls3._dumpString(value.className)
			Util.dumpUint32(len(value.variables))
			for variable in value.variables:
				Ls3._dumpString(variable.name)
				Ls3._dumpType(variable.type.value)
				Ls3.__dumpVariable(variable)

