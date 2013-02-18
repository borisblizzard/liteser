------------------------------------------------------------------------------------------------------
Lite Serializer 2.0 Readme
------------------------------------------------------------------------------------------------------

	1. About

Lite Serializer is a library intended for simplifying serialization and deserialization procedures in
C++. As C++ does not support reflection and automatic serialization, this library is a utility that
can be used to make these procedures as simple and painless as possible. Due to the nature of C++, how
objects are stored in memory and how C++ is compiled, there are certain limits that apply for objects
that you wish to serialize.

------------------------------------------------------------------------------------------------------

	2. Usage

In order to make one of your classes serializable, it needs to fulfill 5 prerequisites. If your class
is abstract (i.e. it has pure virtual methods), then it must fulfill 4 slightly different
requirements.

1. Your class must inherit the class liteser::Serializable.
2. Your class must possess a default constructor without parameters. This constructor will be used to
   create new instances of your classes during deserialization.
3. In your header you must add a call for the macro LS_CLASS_DECLARE(CLASS_PATH) where CLASS_PATH
   is the full path identifier for your class with all prepending namespaces. This is important due
   to the nature how objects are deserialized.
4. In your implementation file (NEVER IN YOUR HEADER), you must add a call for the macro
   LS_CLASS_DEFINE(CLASS_PATH) where CLASS_PATH is the full path identifier for your class with all
   prepending namespaces.
5. You must have variables for serialization defined (see below form more information).

If you are using an abstract class, requirement 4 is not needed and in requirement 3 use
LS_CLASS_DECLARE_ABSTRACT(CLASS_PATH) instead of LS_CLASS_DECLARE(CLASS_PATH).

------------------------------------------------------------------------------------------------------

	3. Variable declarations

In order to make variables serializable, you have to declare them within the LS_VARS(superclass, ...)
macro. The first argument of LS_VARS is the superclass of the current class. Technically you can skip
classes in the inheritance hierarchy, but it is not recommended as this contradicts clean design.
The other arguments are variable definitions in a special format with the type being surrounded by
parenthesis followed by a space and the variable name within a single argument.

Example:

	LS_VARS
	(
		liteser::Serializable,
		(int) var1,
		(float) anotherVar
	);

Example:

	LS_VARS
	(
		liteser::Serializable, (int) var1, (float) anotherVar
	);

This format is important because of the nature how the macro definitions translate into automated
typed serialization and deserialization procedures.
This macro call will declare all variables within the class as well as their serialization and
deserialization procedures.

Limitations:
Keep in mind that you are limited to defining serializable variables only within one of the scopes
(public, protected, private, etc.).

------------------------------------------------------------------------------------------------------

	4. Serializable types

Following types can be declared as serializable variables.

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
	- hltypes::Array (see "Limitations" for more information)
	- hltypes::Map (see "Limitations" for more information)

3. classes that inherit liteser::Serializable:
	- allocated on the heap only (e.g. Serializable*)

Limitations:
- Stack-allocated objects are not supported due to differences in memory allocation and the
  impossibility of dynamic casting of objects that are not pointers to objects.
- hltypes::Array does not support bool due to the implementation of bool within std::vector which
  does not allow them to be modified in a simple way.
- hltypes::Array can only contain hltypes::String from the extended types.
- hltypes::Map requires a simple workaround. Instead of directly declaring
  hltypes::Map<keyType, valueType>, use the provided helper macro HL_HMAP_MACRO_FIX(keyType, valueType).
- hltypes::Map keys do not support bool due to the implementation of bool within std::map which does
  not allow them to be modified in a simple way.
- hltypes::Map values do not support hltypes::Array and hltypes::Map.
- enum is not supported directly (see below for more information).

------------------------------------------------------------------------------------------------------

	5. Workaround for enum

If you want to serialize an enum, there is an indirect way to do this without changing too much of
your code. Instead of creating an enum, create a typedef for int (or unsigned int) and instead of
declaring a possible list of enum values, declare all values as static const values of the new type.
