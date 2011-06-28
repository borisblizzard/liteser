	!!! IMPORTANT !!!

The term "object" is used in the context of an instance of a class derived from liteser::Serializable!

------------------------------------------------------------------------------------------------------

Lite Serializer supports following types:

1. simple types:
	- char
	- unsigned char
	- int
	- unsigned int
	- long
	- unsigned long
	- short
	- unsigned short
	- bool
	- float
	- double
2. extended types:
	- hltypes::String
	- hltypes::Array (containing either simple types, objects or object pointers)
3. objects:
	- allocated on the stack only
	- allocated on the heap only


------------------------------------------------------------------------------------------------------

Lite Serializer does not support following types:

1. pointers to simple types
2. c-type arrays (basically the same as 1.)
3. pointers to objects that are allocated on the stack
4. instances of classes that are not derived from liteser::Serializable
5. classes without an empty default constructor
6. hltypes::Map
