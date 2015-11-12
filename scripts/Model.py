import struct

from Ls2 import *
from Util import *

class Model:

	Formats = ["ls2", "lsx"]
	Header0 = struct.unpack("<b", "L")[0]
	Header1 = struct.unpack("<b", "S")[0]
	VersionMajor = 2
	VersionMinor = 7
	
	SEEK_CUR = 0
	SEEK_SET = 1
	SEEK_END = 2
	
	@staticmethod
	def readFile(format, filename):
		if format == Model.Formats[0]:
			return Model.readLs2(filename)
		if format == Model.Formats[1]:
			return Model.readLsx(filename)
		raise Exception("Format not supported")

	@staticmethod
	def writeFile(format, filename, data):
		if format == Model.Formats[0]:
			Model.writeLs2(filename, data)
		elif format == Model.Formats[1]:
			Model.writeLsx(filename, data)
		else:
			raise Exception("Format not supported")

	@staticmethod
	def readLs2(filename):
		file = None
		data = None
		try:
			file = open(filename, "rb")
			Util.start(file)
			header0 = Util.loadUint8()
			header1 = Util.loadUint8()
			major = Util.loadUint8()
			minor = Util.loadUint8()
			if header0 != Model.Header0 or header1 != Model.Header1:
				raise Exception("Invalid header!")
			Model._checkVersion(major, minor)
			data = Ls2.load()
		finally:
			if file != None:
				Util.finish(file)
				file.close()
		return data
		
	@staticmethod
	def writeLs2(filename, data):
		file = None
		try:
			file = open(filename, "wb")
			Util.start(file)
			Util.dumpUint8(Model.Header0)
			Util.dumpUint8(Model.Header1)
			Util.dumpUint8(Model.VersionMajor)
			Util.dumpUint8(Model.VersionMinor)
			Ls2.dump(data)
		finally:
			if file != None:
				Util.finish(file)
				file.close()
		
	@staticmethod
	def _checkVersion(major, minor):
		if major != Model.VersionMajor:
			raise Exception("Liteser Read Error! Version mismatch: expected %d.%d, got %d.%d" % (Model.VersionMajor, Model.VersionMinor, major, minor))
		if minor < Model.VersionMinor:
			raise Exception("Minor version mismatch while loading: expected %d.%d, got %d.%d" % (Model.VersionMajor, Model.VersionMinor, major, minor))
