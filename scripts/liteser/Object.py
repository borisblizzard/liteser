class Object:

	def __init__(self, className):
		self.className = className
		self.variables = []
		
	def findVariable(self, name):
		for variable in self.variables:
			if variable.name == name:
				return variable
		return None
		
	