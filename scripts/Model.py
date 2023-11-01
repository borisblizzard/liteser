import xml.dom.minidom

from xml.dom.minidom import parse

from .Ls3 import *
from .Lsx import *
from .Util import *

class Model:

	HEADER_SIZE = 4 * 2 + 2 # unsigned int * 2 + bool * 2
	Formats = ["ls3", "lsx"]
	
	@staticmethod
	def readFile(format, filename):
		if format == Model.Formats[0]:
			return Model.readLs3(filename)
		if format == Model.Formats[1]:
			return Model.readLsx(filename)
		raise Exception("Format not supported")

	@staticmethod
	def writeFile(format, filename, data):
		if format == Model.Formats[0]:
			Model.writeLs3(filename, data)
		elif format == Model.Formats[1]:
			Model.writeLsx(filename, data)
		else:
			raise Exception("Format not supported")

	@staticmethod
	def readLs3(filename):
		file = None
		data = None
		try:
			file = open(filename, "rb")
			Util.start(file)
			header0 = Util.loadUint8()
			header1 = Util.loadUint8()
			header2 = Util.loadUint8()
			header3 = Util.loadUint8()
			if header0 != Util.Header0 or header1 != Util.Header1 or header2 != Util.Header2 or header3 != Util.Header3:
				raise Exception("Invalid header!")
			headerSize = Util.loadUint32()
			major = Util.loadUint32()
			minor = Util.loadUint32()
			Util._allowMultiReferencing = Util.loadBool()
			Util._stringPooling = Util.loadBool()
			Model._checkVersion(major, minor)
			data = Ls3.load()
		finally:
			if file != None:
				Util.finish(file)
				file.close()
		return data
		
	@staticmethod
	def writeLs3(filename, data):
		file = None
		try:
			file = open(filename, "wb")
			Util.start(file)
			Util.dumpUint8(Util.Header0)
			Util.dumpUint8(Util.Header1)
			Util.dumpUint8(Util.Header2)
			Util.dumpUint8(Util.Header3)
			Util.dumpUint32(Model.HEADER_SIZE)
			Util.dumpUint32(Util.VersionMajor)
			Util.dumpUint32(Util.VersionMinor)
			Util.dumpBool(Util._allowMultiReferencing)
			Util.dumpBool(Util._stringPooling)
			Ls3.dump(data)
		finally:
			if file != None:
				Util.finish(file)
				file.close()
		
	@staticmethod
	def readLsx(filename):
		file = None
		data = None
		try:
			file = xml.dom.minidom.parse(filename)
			Util.start(file)
			root = file.documentElement
			if root.nodeName != "Liteser":
				raise Exception("Invalid header!")
			versionString = root.getAttribute("version")
			versions = versionString.split(".")
			if len(versions) != 2:
				raise Exception("Invalid header!")
			major = int(versions[0])
			minor = int(versions[1])
			Model._checkVersion(major, minor)
			children = Util._getChildNodes(root)
			if len(children) != 1:
				raise Exception("Cannot load object from file that does not contain any data!")
			data = Lsx.load(children[0])
		finally:
			if file != None:
				Util.finish(file)
		return data
		
	@staticmethod
	def writeLsx(filename, data):
		file = None
		try:
			file = open(filename, "wb")
			Util.start(file)
			Util._allowMultiReferencing = True
			Util._stringPooling = True
			Util.stream.write(Lsx.XML_HEADER)
			Util.stream.write(Lsx.LITESER_XML_ROOT_BEGIN)
			Lsx.dump(data)
			Util.stream.write(Lsx.LITESER_XML_ROOT_END)
		finally:
			if file != None:
				Util.finish(file)
				file.close()
		
	@staticmethod
	def _checkVersion(major, minor):
		if major != Util.VersionMajor:
			raise Exception("Liteser Read Error! Version mismatch: expected %d.%d, got %d.%d" % (Util.VersionMajor, Util.VersionMinor, major, minor))
		if minor < Util.VersionMinor:
			raise Exception("Minor version mismatch while loading: expected %d.%d, got %d.%d" % (Util.VersionMajor, Util.VersionMinor, major, minor))
