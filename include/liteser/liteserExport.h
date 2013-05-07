/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines macros for DLL exports/imports.

#ifndef LITESER_EXPORT_H
#define LITESER_EXPORT_H

	/// @def liteserExport
	/// @brief Macro for DLL exports/imports.
	/// @def liteserFnExport
	/// @brief Macro for function DLL exports/imports.
	#ifdef _LIB
		#define liteserExport
		#define liteserFnExport
	#else
		#ifdef _WIN32
			#ifdef LITESER_EXPORTS
				#define liteserExport __declspec(dllexport)
				#define liteserFnExport __declspec(dllexport)
			#else
				#define liteserExport __declspec(dllimport)
				#define liteserFnExport __declspec(dllimport)
			#endif
		#else
			#define liteserExport __attribute__ ((visibility("default")))
			#define liteserFnExport
		#endif
	#endif
	#ifndef DEPRECATED_ATTRIBUTE
		#ifdef _MSC_VER
			#define DEPRECATED_ATTRIBUTE __declspec(deprecated("function is deprecated"))
		#else
			#define DEPRECATED_ATTRIBUTE __attribute__((deprecated))
		#endif
	#endif

#endif

