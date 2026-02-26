#pragma once

#if defined(_WIN32)
	#ifdef DYNAMIC_LIBRARY_EXPORT
		#define REC_ENGINE_API __declspec(dllexport)
	#else
		#define REC_ENGINE_API __declspec(dllimport)
	#endif
#else
	#define REC_ENGINE_API
#endif