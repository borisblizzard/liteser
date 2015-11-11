class Variable:

	def __init__(self, name, type):
		self.name = name
		self.type = Type(type)
		self.subVariables = []
		self.containerSize = 0
		
