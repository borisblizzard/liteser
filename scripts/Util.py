import os
import struct
import xml.dom.minidom

from xml.dom.minidom import parse

class Util:
	
	Header0 = struct.unpack("<b", "L")[0]
	Header1 = struct.unpack("<b", "S")[0]
	VersionMajor = 2
	VersionMinor = 7
	
	@staticmethod
	def getFileList(path):
		print "  checking %s" % path
		files = []
		dirs = []
		for f in os.listdir(path + "/"):
			if f == ".svn" or f == "." or f == "..":
				continue
			name = path + "/" + f
			try:
				os.listdir(name)
				dirs.append(name)
			except:
				files.append(name)
		list = files
		for dir in dirs:
			list.extend(Util.getFileList(dir))
		return list
		
	objects = []
	strings = []
	objectIds = {}
	stringIds = {}
	stream = None
	_indent = "\t"
	
	@staticmethod
	def start(_stream):
		Util.objects = []
		Util.strings = []
		Util.objectIds = {}
		Util.stringIds = {}
		Util.stream = _stream
		Util._indent = "\t"
	
	@staticmethod
	def finish(_stream):
		Util.objects = []
		Util.strings = []
		Util.objectIds = {}
		Util.stringIds = {}
		Util.stream = None
		Util._indent = "\t"

	@staticmethod
	def _tryGetObject(id, idExists = True):
		if id == 0 or idExists:
			return idExists, None
		if id - 1 >= len(Util.objects):
			return False, None
		return True, Util.objects[id - 1]

	@staticmethod
	def _tryMapObject(object):
		if object == None:
			return False, 0
		if not object in Util.objectIds.keys():
			Util.objects.append(object)
			id = len(Util.objects)
			Util.objectIds[object] = id - 1
			return True, id
		return False, Util.objectIds[object] + 1

	@staticmethod
	def _tryGetString(id):
		if id == 0:
			return True, ""
		if id - 1 >= len(Util.strings):
			return False, ""
		return True, Util.strings[id - 1]
	
	@staticmethod
	def _tryMapString(string):
		if string == "":
			return False, 0
		if not string in Util.stringIds.keys():
			Util.strings.append(string)
			id = len(Util.strings)
			Util.stringIds[string] = id - 1
			return True, id
		return False, Util.stringIds[string] + 1
		
	# XML utility
	@staticmethod
	def _getChildNodes(node):
		result = []
		for child in node.childNodes:
			if child.nodeType == node.ELEMENT_NODE:
				result.append(child)
		return result
		
	@staticmethod
	def _indentIncrease():
		Util._indent += "\t"

	@staticmethod
	def _indentDecrease():
		Util._indent = Util._indent[0:len(Util._indent) - 1]
		
	@staticmethod
	def _writeNode(string):
		Util.stream.write(Util._indent + "<" + string + "/>\n")

	@staticmethod
	def _openNode(string):
		Util.stream.write(Util._indent + "<" + string + ">\n")
		Util._indentIncrease()
		
	@staticmethod
	def _closeNode(string):
		Util._indentDecrease()
		Util.stream.write(Util._indent + "</" + string + ">\n")

	@staticmethod
	def _startLine(string):
		Util.stream.write(Util._indent + "<" + string)

	@staticmethod
	def _finishLine(string):
		Util.stream.write(string + "/>\n")

	# binary data loading
	@staticmethod
	def loadInt8():
		return struct.unpack("<b", Util.stream.read(1))[0]
		
	@staticmethod
	def loadUint8():
		return struct.unpack("<B", Util.stream.read(1))[0]
		
	@staticmethod
	def loadInt16():
		return struct.unpack("<h", Util.stream.read(2))[0]
		
	@staticmethod
	def loadUint16():
		return struct.unpack("<H", Util.stream.read(2))[0]
		
	@staticmethod
	def loadInt32():
		return struct.unpack("<i", Util.stream.read(4))[0]
		
	@staticmethod
	def loadUint32():
		return struct.unpack("<I", Util.stream.read(4))[0]
		
	@staticmethod
	def loadInt64():
		return struct.unpack("<q", Util.stream.read(8))[0]
		
	@staticmethod
	def loadUint64():
		return struct.unpack("<Q", Util.stream.read(8))[0]
		
	@staticmethod
	def loadFloat():
		return struct.unpack("<f", Util.stream.read(4))[0]
		
	@staticmethod
	def loadDouble():
		return struct.unpack("<d", Util.stream.read(8))[0]
		
	@staticmethod
	def loadBool():
		return (struct.unpack("<b", Util.stream.read(1))[0] != 0)
		
	@staticmethod
	def loadHstr():
		return Util.stream.read(Util.loadUint32())
	
	# binary data dumping
	@staticmethod
	def dumpInt8(value):
		return Util.stream.write(struct.pack("<b", value))
		
	@staticmethod
	def dumpUint8(value):
		return Util.stream.write(struct.pack("<B", value))
		
	@staticmethod
	def dumpInt16(value):
		return Util.stream.write(struct.pack("<h", value))
		
	@staticmethod
	def dumpUint16(value):
		return Util.stream.write(struct.pack("<H", value))
		
	@staticmethod
	def dumpInt32(value):
		return Util.stream.write(struct.pack("<i", value))
		
	@staticmethod
	def dumpUint32(value):
		return Util.stream.write(struct.pack("<I", value))
		
	@staticmethod
	def dumpInt64(value):
		return Util.stream.write(struct.pack("<q", value))
		
	@staticmethod
	def dumpUint64(value):
		return Util.stream.write(struct.pack("<Q", value))
		
	@staticmethod
	def dumpFloat(value):
		return Util.stream.write(struct.pack("<f", value))
		
	@staticmethod
	def dumpDouble(value):
		return Util.stream.write(struct.pack("<d", value))
		
	@staticmethod
	def dumpBool(value):
		data = 0
		if value:
			data = 1
		return Util.stream.write(struct.pack("<b", data))
		
	@staticmethod
	def dumpHstr(value):
		Util.dumpUint32(len(value))
		Util.stream.write(value)
	
