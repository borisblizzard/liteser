/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
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
			#define liteserFnExport __attribute__ ((visibility("default")))
		#endif
	#endif

#endif

