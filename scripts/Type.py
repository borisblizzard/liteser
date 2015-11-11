class Type:

	NONE     = 0x00
	INT8	 = 0x01
	UINT8	 = 0x02
	INT16	 = 0x03
	UINT16	 = 0x04
	INT32    = 0x05
	UINT32   = 0x06
	INT64    = 0x07
	UINT64   = 0x08
	FLOAT    = 0x21
	DOUBLE	 = 0x22
	BOOL	 = 0x41
	OBJECT	 = 0x61
	OBJPTR	 = 0x62
	HSTR	 = 0x81
	HVERSION = 0x82
	HENUM    = 0x83
	GRECT    = 0x91
	GVEC2	 = 0x92
	GVEC3	 = 0x93
	HARRAY	 = 0xA1
	HMAP	 = 0xC1
	
	def __init__(self, value):
		self.value = value
		self.subType = []

	