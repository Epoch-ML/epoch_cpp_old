#include "TypesTestSuite.h"

#define TEST_INT_ARRAY_LENGTH 1000

#include <vector>

#include "core/types/EPTimedFunction.h"

RESULT TypesTestSuite::TestEPDynamicStorage() {
	RESULT r = R::OK;
	EPVector<int> intArray;
	std::vector<int> stdIntArray;

	{

		// TODO: Wrap up the below as test cases
		// TODO: Create a "ep timed function"

		// Case 1 - Check push back
		size_t nsEPConstructTime = EPTimedFunction<void()>::MakeAndRun([&]() -> void {
			intArray = EPVector<int>();
		}).get<1, size_t>();

		size_t nsEPPushTime = EPTimedFunction<void()>::MakeAndRun([&]() -> void {
			for (int i = 0; i < TEST_INT_ARRAY_LENGTH; i++) {
				intArray.PushBack(i);
			}
		}).get<1, size_t>();

		CBM(intArray.size() == TEST_INT_ARRAY_LENGTH,
			"Size %zu differs from expected size %d", intArray.size(), TEST_INT_ARRAY_LENGTH);

		size_t nsEPCheckTime = EPTimedFunction<void()>::MakeAndRun([&]() -> void {
			for (int i = 0; i < TEST_INT_ARRAY_LENGTH; i++) {
				CBM(intArray[i] == i, "intArray[%d]:%d differed from value %d expected", i, intArray[i], i);
			}
		Error:
			return;
		}).get<1, size_t>();

		// Case 2 - Check front insertion
		intArray = EPVector<int>();

		size_t nsEPInsertFrontTime = EPTimedFunction<void()>::MakeAndRun([&]() -> void {
			for (int i = TEST_INT_ARRAY_LENGTH - 1; i >= 0; i--) {
				intArray.PushFront(i);
			}
		}).get<1, size_t>();

		CBM(intArray.size() == TEST_INT_ARRAY_LENGTH,
			"Size %zu differs from expected size %d", intArray.size(), TEST_INT_ARRAY_LENGTH);

		for (int i = 0; i < TEST_INT_ARRAY_LENGTH; i++) {
			CBM(intArray[i] == i, "intArray[%d]:%d differed from value %d expected", i, intArray[i], i);
		}

		// Case 3 - Test performance against std::vector

		size_t nsSTDConstructTime = EPTimedFunction<void()>::MakeAndRun([&]() -> void {
			stdIntArray = std::vector<int>();
		}).get<1, size_t>();

		CLTDM(nsEPConstructTime, nsSTDConstructTime,
			"construct: std::vector faster",
			"construct: std::vector: %zu ns EPVector: %zu ns",
			nsSTDConstructTime, nsEPConstructTime);

		size_t nsSTDPushBackTime = EPTimedFunction<void()>::MakeAndRun([&]() -> void {
			for (int i = 0; i < TEST_INT_ARRAY_LENGTH; i++) {
				stdIntArray.push_back(i);
			}
		}).get<1, size_t>();

		CLTDM(nsEPPushTime, nsSTDPushBackTime,
			"pushback: std::vector faster",
			"pushback: std::vector: %zu ns EPVector: %zu ns",
			nsSTDPushBackTime, nsEPPushTime);

		size_t nsSTDCheckTime = EPTimedFunction<void()>::MakeAndRun([&]() -> void {
			for (int i = 0; i < TEST_INT_ARRAY_LENGTH; i++) {
				CBM(stdIntArray[i] == i, "intArray[%d]:%d differed from value %d expected", i, intArray[i], i);
			}
		Error:
			return;
		}).get<1, size_t>();

		CLTDM(nsEPCheckTime, nsSTDCheckTime,
			"check: std::vector faster",
			"check: std::vector: %zu ns EPVector: %zu ns",
			nsSTDCheckTime, nsEPCheckTime);
	}

Error:
	return r;
}