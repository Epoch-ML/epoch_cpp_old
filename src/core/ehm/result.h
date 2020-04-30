#ifndef RESULT_H_
#define RESULT_H_
#pragma once

// EPOCH Core
// epoch/src/core/result.h
// result (RESULT)

#include <cstdint>
#include <type_traits>

#if defined(_WIN32)

	#ifdef WIN32_LEAN_AND_MEAN
		#undef WIN32_LEAN_AND_MEAN
	#endif

	#ifndef WIN32
		#define WIN32
	#endif
	
	#pragma warning( disable:  4102)		// Disable unreferenced label warning 

#elif defined(__APPLE__)
	#pragma GCC diagnostic ignored "-Wunused-label"
#elif defined (__linux__)
	#pragma GCC diagnostic ignored "-Wunused-label"
#else
	#pragma GCC diagnostic ignored "-Wunused-label"
#endif

#define RFAILED(res) (res & 0x80000000)
#define RSUCCESS(res) (!RFAILED(res))

typedef enum class RESULT : uint32_t {
	// Success codes
	OK		= 0x00000000,
	SUCCESS = 0x00000000,
	SUCCEED = 0x00000000,
	PASS		= 0x00000000,

	NOT_IMPLEMENTED,
	NO_RESULT,
	SKIPPED,

	// Fail codes
	FAIL = 0x80000000,

	MATRIX_ROW_OUT_OF_RANGE,
	MATRIX_COL_OUT_OF_RANGE,

	MEMORY_ALLOCATION_FAILED,

	PLATFORM_NOT_SUPPORTED
} R ;

// TODO: Kind of torn about the performance of this
// might choose to go back to just INT based RESULT values

RESULT operator &(RESULT lhs, RESULT rhs);
uint32_t operator &(RESULT lhs, uint32_t rhs) ;

#endif // ! RESULT_H_