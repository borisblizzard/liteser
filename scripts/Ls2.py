from Object import *
from Type import *
from Variable import *

class Ls2:

	@staticmethod
	def load():
		type = Ls2._loadType()
		data = None
		if type == Type.OBJPTR:
			data = Ls2._loadObject()
		elif type == Type.HARRAY:
			subTypes = Util._loadUint32()
			if subTypes != 1:
				raise Exception("Cannot load object from stream that does not contain a harray!")
			data = Ls2._loadHarray(_loadType())
		if data == None:
			raise Exception("Cannot load object from stream that does not contain properly serialized Liteser data!")
		return data

	@staticmethod
	def _loadType():
		return Util._loadUint8()
		
	@staticmethod
	def __loadVariable(variable, loadType):
		if loadType == Type.INT8:
			variable.value = Util._loadInt8()
		elif loadType == Type.UINT8:
			variable.value = Util._loadUint8()
		elif loadType == Type.INT16:
			variable.value = Util._loadInt16()
		elif loadType == Type.UINT16:
			variable.value = Util._loadUint16()
		elif loadType == Type.INT32:
			variable.value = Util._loadInt32()
		elif loadType == Type.UINT32:
			variable.value = Util._loadUint32()
		elif loadType == Type.INT64:
			variable.value = Util._loadInt64()
		elif loadType == Type.UINT64:
			variable.value = Util._loadUint64()
		elif loadType == Type.FLOAT:
			variable.value = Util._loadFloat()
		elif loadType == Type.DOUBLE:
			variable.value = Util._loadDouble()
		elif loadType == Type.BOOL:
			variable.value = Util._loadBool()
		elif loadType == Type.HSTR:
			variable.value = Ls2._loadString()
		elif loadType == Type.HVERSION:
			variable.value = Ls2._loadVersion()
		elif loadType == Type.HENUM:
			variable.value = Ls2._loadEnum()
		elif loadType == Type.GRECT:
			variable.value = Ls2._loadGrect()
		elif loadType == Type.GVEC2:
			variable.value = Ls2._loadGvec2()
		elif loadType == Type.GVEC3:
			variable.value = Ls2._loadGvec3()
		elif loadType == Type.OBJECT:
			variable.value = Ls2._loadObject()
		elif loadType == Type.OBJPTR:
			variable.value = Ls2._loadObject()
		elif loadType == Type.HARRAY:
			Util._loadContainer(variable, loadType)
		elif loadType == Type.HMAP:
			Util._loadContainer(variable, loadType)
		else:
			print "ERROR: Load type nore recognized!"

	@staticmethod
	def __loadContainer(variable, type):
		variable.containerSize = _loadUint32()
		if variable.containerSize > 0:
			typeSize = Util._loadUint32();
			for i in xrange(typeSize):
				loadType = Ls2._loadType()
				if loadType == Type.HARRAY or loadType == Type.HMAP:
					raise Exception("Template container within a template container detected, not supported: %02X" % loadType)
				variable.type.subTypes.append(Type(loadType))
			for i in xrange(variable.containerSize):
				variable = Variable("", type)
				variable.subVariables.append(variable)
				Util._loadVariable(variable, variable.type.value);

	@staticmethod
	def _loadString():
		id = Util._loadUint32()
		result, value = Util.__tryGetString(id)
		if result:
			value = Util._loadHstr()
			Util.__tryMapString(id, value);
		return value
		
	@staticmethod
	def _loadVersion():
		return [Util._loadUint32(), Util._loadUint32(), Util._loadUint32(), Util._loadUint32()]

	@staticmethod
	def _loadEnum():
		return Util._loadUint32()

	@staticmethod
	def _loadGrect():
		return [Util._loadUint32(), Util._loadUint32(), Util._loadUint32(), Util._loadUint32()]

	@staticmethod
	def _loadGvec2():
		return [Util._loadUint32(), Util._loadUint32()]

	@staticmethod
	def _loadGvec3():
		return [Util._loadUint32(), __loadUint32(), Util._loadUint32()]

	@staticmethod
	def _loadObject():
		id = _loadUint32()
		result, object = Util.__tryGetObject(id)
		if not result:
			className = Ls2._loadString()
			if object == None:
				object = Object(className)
			Util.__tryMapObject(object)
			size = Util._loadUint32()
			for i in xrange(size):
				loadType = Ls2._loadType()
				variable = Variable(Ls2._loadString(), loadType)
				object.variables.add(variable)
				Util._loadVariable(variable, loadType)
		return object
		
