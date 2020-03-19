#include "result.h"

RESULT operator &(RESULT lhs, RESULT rhs) {
	return static_cast<RESULT> (
		static_cast<std::underlying_type<RESULT>::type>(lhs) &
		static_cast<std::underlying_type<RESULT>::type>(rhs)
	);
}

uint32_t operator &(RESULT lhs, uint32_t rhs) {
	return static_cast<uint32_t> (
		static_cast<std::underlying_type<RESULT>::type>(lhs) & (rhs)
	);
}