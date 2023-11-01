from .Type import *

class Variable:

	def __init__(self, name, type, value = None):
		self.name = name
		if not isinstance(type, list):
			self.type = Type(type)
		else:
			self.type = Type(type[0], type[1 : len(type)])
		self.subVariables = []
		self.containerSize = 0
		if isinstance(type, list):
			self.value = None
			if value != None:
				self.setupComplexValue(value);
		else:
			self.value = value
			
	def setupComplexValue(self, value):
		if self.type.value == Type.HARRAY and isinstance(value, list):
			self.containerSize = len(value)
			self.createSubVariables()
			for i in range(self.containerSize):
				self.subVariables[i].value = value[i]
		elif self.type.value == Type.HMAP and isinstance(value, dict):
			self.containerSize = len(value)
			self.createSubVariables()
			keys = value.keys()
			for key in keys:
				self.subVariables[0].append(key)
				self.subVariables[1].append(value[key])
		
		
	def createSubVariables(self):
		if self.type.value == Type.HARRAY:
			for i in range(self.containerSize):
				self.subVariables.append(Variable("", self.type.subTypes[0].value))
		if self.type.value == Type.HMAP:
			for i in range(2):
				self.subVariables.append(Variable("", Type.HARRAY))
