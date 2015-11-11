import os
import struct

class Util:
	
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
	def _start(_stream):
		Util.objects = []
		Util.strings = []
		Util.objectIds = {}
		Util.stringIds = {}
		Util.stream = _stream
		Util._indent = "\t"
	
	@staticmethod
	def _finish(_stream):
		Util.objects = []
		Util.strings = []
		Util.objectIds = {}
		Util.stringIds = {}
		Util.stream = None
		Util._indent = "\t"

	@staticmethod
	def __tryGetObject(id):
		if id == 0:
			return True, None;
		if id - 1 >= len(Util.objects):
			return False, None;
		return True, Util.objects[id - 1]

	@staticmethod
	def __tryMapObject(object):
		if object == None:
			return False, 0
		if not object in Util.objectIds.keys():
			Util.objects.append(object)
			id = len(Util.objects)
			Util.objectIds[object] = id - 1;
			return True, id
		return False, Util.objectIds[object]

	@staticmethod
	def __tryGetString(id):
		if id == 0:
			return True, ""
		if id - 1 >= len(Util.strings):
			return False, ""
		return True, Util.strings[id - 1]
	
	@staticmethod
	def __tryMapString(string):
		if string == "":
			return False, 0
		if not string in Util.stringIds.keys():
			Util.strings.append(string)
			id = len(Util.strings)
			Util.stringIds[string] = id - 1;
			return True, id
		return False, Util.stringIds[string]

	@staticmethod
	def __forceMapEmptyObject():
		Util.objects.append(None)

	# binary data loading
	@staticmethod
	def _loadInt8():
		return struct.unpack("<b", Util.stream.read(1))[0]
		
	@staticmethod
	def _loadUint8():
		return struct.unpack("<B", Util.stream.read(1))[0]
		
	@staticmethod
	def _loadInt16():
		return struct.unpack("<h", Util.stream.read(2))[0]
		
	@staticmethod
	def _loadUint16():
		return struct.unpack("<H", Util.stream.read(2))[0]
		
	@staticmethod
	def _loadInt32():
		return struct.unpack("<i", Util.stream.read(4))[0]
		
	@staticmethod
	def _loadUint32():
		return struct.unpack("<I", Util.stream.read(4))[0]
		
	@staticmethod
	def _loadInt64():
		return struct.unpack("<q", Util.stream.read(8))[0]
		
	@staticmethod
	def _loadUint64():
		return struct.unpack("<Q", Util.stream.read(8))[0]
		
	@staticmethod
	def _loadFloat():
		return struct.unpack("<f", Util.stream.read(4))[0]
		
	@staticmethod
	def _loadDouble():
		return struct.unpack("<d", Util.stream.read(8))[0]
		
	@staticmethod
	def _loadBool():
		return (struct.unpack("<b", Util.stream.read(1))[0] != 0)
		
	@staticmethod
	def _loadHstr():
		return Util.stream.read(Util._loadUint32())
		
