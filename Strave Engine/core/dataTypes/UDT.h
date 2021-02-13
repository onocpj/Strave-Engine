#pragma once

#define UDT_ADD_LIBS
#if defined(UDT_ADD_LIBS)
	#include "Core.h"
#endif // !UDT_ADD_LIBS


#ifdef SV_PLATFORM_WINDOWS 
	#define NULL_PTR	nullptr
	#define EMPTY_STR	"" 

	#define UNDEF_PTR	NULL_PTR
	#define UNDEF_STR	EMPTY_STR
#endif

#define UNDEF_INT16		NULL
#define UNDEF_INT32		NULL
#define UNDEF_INT64		NULL

#define UNDEF_SMARTPTR		NULL
#define UNDEF_STD_VECTOR	NULL
#define UNDEF_FUNC		NULL

#define UNDEF_INT		NULL
#define UNDEF_INT32		NULL
#define UNDEF_INT64		NULL
#define UNDEF_UINT		NULL
#define UNDEF_UINT32		NULL
#define UNDEF_UINT64		NULL

#define UNDEF_BOOL		0

#define UNDEF_FLOAT		0.0f
#define UNDEF_DOUBLE		0.0
#define UNDEF_LFLOAT		UNDEF_FLOAT
#define UNDEF_LLFLOAT		UNDEF_FLOAT

#define UNDEF_VECTOR2F		{ UNDEF_FLOAT, UNDEF_FLOAT }
#define UNDEF_VECTOR2U		{ UNDEF_UINT, UNDEF_INT }
#define UNDEF_VECTOR2I		{ UNDEF_INT, UNDEF_INT }
#define UNDEF_VECTOR2V2F	{ UNDEF_VECTOR2F, UNDEF_VECTOR2F }
#define UNDEF_VECTOR2V2UI	{ UNDEF_VECTOR2U, UNDEF_VECTOR2U }		

#define ERROR_TYPE		-1


#undef UDT_ADD_LIBS
