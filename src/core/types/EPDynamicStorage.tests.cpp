#include "TypesTestSuite.h"

#define TEST_INT_ARRAY_LENGTH 1000

#include <vector>

RESULT TypesTestSuite::TestEPDynamicStorage() {
	RESULT r = R::OK;

	{

		// TODO: Wrap up the below as test cases
		// TODO: Create a "ep timed function"

		// Case 1 - Check push back
		auto timeStart = std::chrono::high_resolution_clock::now();
		EPDynamicStorage<int> intArray = EPDynamicStorage<int>();
		auto timeEnd = std::chrono::high_resolution_clock::now();
		size_t nsEPConstructTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();

		timeStart = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < TEST_INT_ARRAY_LENGTH; i++) {
			intArray.PushBack(i);
		}
		timeEnd = std::chrono::high_resolution_clock::now();
		size_t nsEPPushTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();

		CBM(intArray.size() == TEST_INT_ARRAY_LENGTH,
			"Size %zu differs from expected size %d", intArray.size(), TEST_INT_ARRAY_LENGTH);

		timeStart = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < TEST_INT_ARRAY_LENGTH; i++) {
			CBM(intArray[i] == i, "intArray[%d]:%d differed from value %d expected", i, intArray[i], i);
		}
		timeEnd = std::chrono::high_resolution_clock::now();
		size_t nsEPCheckTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();

		// Case 2 - Check front insertion
		intArray = EPDynamicStorage<int>();

		timeStart = std::chrono::high_resolution_clock::now();
		for (int i = TEST_INT_ARRAY_LENGTH - 1; i >= 0; i--) {
			intArray.PushFront(i);
		}
		timeEnd = std::chrono::high_resolution_clock::now();
		size_t nsEPInsertFrontTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();

		CBM(intArray.size() == TEST_INT_ARRAY_LENGTH,
			"Size %zu differs from expected size %d", intArray.size(), TEST_INT_ARRAY_LENGTH);

		for (int i = 0; i < TEST_INT_ARRAY_LENGTH; i++) {
			CBM(intArray[i] == i, "intArray[%d]:%d differed from value %d expected", i, intArray[i], i);
		}

		// Case 3 - Test performance against std::vector

		timeStart = std::chrono::high_resolution_clock::now();
		std::vector<int> stdIntArray = std::vector<int>();
		timeEnd = std::chrono::high_resolution_clock::now();
		size_t nsSTDConstructTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();

		CBM(nsSTDConstructTime > nsEPConstructTime,
			"%zu ns std::vector construct faster than %zu EPDynamicStorage construct",
			nsSTDConstructTime, nsEPConstructTime);

		timeStart = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < TEST_INT_ARRAY_LENGTH; i++) {
			stdIntArray.push_back(i);
		}
		timeEnd = std::chrono::high_resolution_clock::now();
		size_t nsSTDPushBackTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();

		CBM(nsSTDPushBackTime > nsEPPushTime,
			"%zu ns std::vector push back faster than %zu EPDynamicStorage push back",
			nsSTDPushBackTime, nsEPPushTime);

		timeStart = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < TEST_INT_ARRAY_LENGTH; i++) {
			CBM(stdIntArray[i] == i, "intArray[%d]:%d differed from value %d expected", i, intArray[i], i);
		}
		timeEnd = std::chrono::high_resolution_clock::now();
		size_t nsSTDCheckTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart).count();

		CBM(nsSTDCheckTime > nsEPCheckTime,
			"%zu ns std::vector check time faster than %zu EPDynamicStorage check time",
			nsSTDCheckTime, nsEPCheckTime);
	}

Error:
	return r;
}