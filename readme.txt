------------------------------------------------------------------------------------------------------
Lite Serializer 2.7 Readme
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
requirements. Include <liteser/Serializable.h> to have access to all macros that you need.

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

IMPORTANT:
If you are compiling the classes in a static library or if you have the classes in an .exe, there is
a possibility that the compiler will optimize the code and strip unused code if you aren't
instantiating any objects during runtime (except loading from a file). This can be prevented in two
different ways. This is not required for abstract classes.

1. You can manually force a static instance LS_FORCE_REGISTER(CLASS_PATH, CLASS_NAME) where CLASS_PATH
   is the full path identifier for your class with all prepending namespaces and CLASS_NAME is the
   same without all the "::" between namespaces/class names or some other unique name. You can (and
   should) put it in the same namespace where the class was declared. If you use this method, make
   sure that creating and immediately destroying the object does not cause memory leaks.
2. You can register the class manually with LS_REGISTER(CLASS_PATH) where CLASS_PATH is the full path
   identifier for your class with all prepending namespaces. You should do this during the
   initialization.

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
- Keep in mind that you are limited to defining serializable variables only within one of the scopes
  (public, protected, private, etc.).
- Make sure that there is a white space between the type and the variable name. Something like
  "(float)anotherVar" will not work while "(float) anotherVar" will. This is necessary due to how C++
  processes macros.

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
	- gtypes::Vector2
	- gtypes::Rectangle
	- hltypes::String
	- hltypes::Enum
	
3. container types:
	- hltypes::Array (see "Limitations" for more information)
	- hltypes::Map (see "Limitations" for more information)

4. classes that inherit liteser::Serializable:
	- allocated on the heap only (e.g. Serializable*)

Limitations:
- enum is not supported directly. Use henum instead.
- Stack-allocated objects are only supported in plain format and never in containers.
- hltypes::Array does not support bool due to the implementation of bool within std::vector which
  does not allow them to be modified in a simple way.
- hltypes::Array cannot contain another container type.
- hltypes::Array cannot contain a henum-derived types.
- hltypes::Map requires a workaround. Instead of directly declaring hltypes::Map<keyType, valueType>,
  you have to create a typedef first.
- hltypes::Map keys do not support bool due to the implementation of bool within std::map which does
  not allow them to be modified in a simple way.
- hltypes::Map keys and values do not support container types.
- hltypes::Map keys and values do not support henum-derived types.

------------------------------------------------------------------------------------------------------

	5. Object models with loops

If you have classes that have pointers to classes which inherit the first class, you cannot use
LS_VARS, because you would have to include the derived class' header which would result in an
inclusion loop and your code won't compile. For this problem you are offered 2 additional macros.
LS_VARS_DECLARE(...) is added in the header while LS_VARS_DEFINE(class, superclass, ...) is added in
the source file.

Example:

	// in your header
	LS_VARS_DECLARE
	(
		(int) var1,
		(float) anotherVar
	);
	// in your source file
	LS_VARS_DEFINE
	(
		mynamespace::MyClass,
		liteser::Serializable,
		(int) var1,
		(float) anotherVar
	);

Note that this approach allows you to serialize variables that have different scopes (public,
protected, private, etc.) and variables that are not actually included in LS_VARS_DEFINE which is not
possible when using LS_VARS only.
