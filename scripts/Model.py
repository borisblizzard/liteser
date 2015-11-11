import struct

from Ls2 import *
from Util import *

class Model:

	Formats = ["lsx", "ls2"]
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
			return Model.readLsx(filename)
		if format == Model.Formats[1]:
			return Model.readLs2(filename)
		raise Exception("ERROR: Format not supported")

	@staticmethod
	def readLs2(filename):
		file = None
		data = None
		try:
			file = open(filename, "rb")
			Util._start(file)
			header0 = Util._loadUint8()
			header1 = Util._loadUint8()
			major = Util._loadUint8()
			minor = Util._loadUint8()
			if header0 != Model.Header0 or header1 != Model.Header1:
				raise Exception("Invalid header!")
			Model._checkVersion(major, minor)
			data = Ls2.load(file)
		finally:
			if file != None:
				Util._finish(file)
				file.close()
		return data
		
	@staticmethod
	def _checkVersion(major, minor):
		if major != Model.VersionMajor:
			raise Exception("Liteser Read Error! Version mismatch: expected %d.%d, got %d.%d" % (Model.VersionMajor, Model.VersionMinor, major, minor))
		if minor < Model.VersionMinor:
			raise Exception("Minor version mismatch while loading: expected %d.%d, got %d.%d" % (Model.VersionMajor, Model.VersionMinor, major, minor))
