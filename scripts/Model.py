import xml.dom.minidom

from xml.dom.minidom import parse

from Ls2 import *
from Lsx import *
from Util import *

class Model:

	Formats = ["ls2", "lsx"]
	
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
			if header0 != Util.Header0 or header1 != Util.Header1:
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
			Util.dumpUint8(Util.Header0)
			Util.dumpUint8(Util.Header1)
			Util.dumpUint8(Util.VersionMajor)
			Util.dumpUint8(Util.VersionMinor)
			Ls2.dump(data)
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
