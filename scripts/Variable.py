from Type import *

class Variable:

	def __init__(self, name, type):
		self.name = name
		self.type = Type(type)
		self.subVariables = []
		self.containerSize = 0
		self.value = None
		
	def createSubVariables(self):
		if self.type.value == Type.HARRAY:
			for i in xrange(self.containerSize):
				self.subVariables.append(Variable("", self.type.subTypes[0].value))
		if self.type.value == Type.HMAP:
			for i in xrange(2):
				self.subVariables.append(Variable("", Type.HARRAY))
