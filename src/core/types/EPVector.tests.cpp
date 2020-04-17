#include "TypesTestSuite.h"

#define TEST_INT_ARRAY_LENGTH 1000

#include <vector>

#include "test/EPTest.h"
#include "test/EPTestCase.h"

RESULT TypesTestSuite::TestEPDynamicStorage(EPTestBase *pEPTestBase) {
	RESULT r = R::OK;

	EPVector<int> intArray;
	std::vector<int> stdIntArray;

	// EPVector test case labels
	const char *kConstruct = "construct";
	const char *kPushBack = "pushback";
	const char *kCheck = "check";
	const char *kPushFront = "pushfront";
	const char* kForEach = "foreach";

	const char* kEPVector = "EPVector";
	const char* kSTLVector = "std::vector";

	EPTest<RESULT(EPTestBase*)>* pEPTest = dynamic_cast<EPTest<RESULT(EPTestBase*)>*>(pEPTestBase);
	CNM(pEPTest, "EPTest is nullptr");

	// Case 1 - Check push back
	// TODO: get rid of result return

	pEPTest->RegisterAndRunTC(kConstruct, kEPVector, EPTestCase::expected::COMPARE,
		EPTimedFunction<RESULT(void)>(
			[&]() -> RESULT {
				intArray = EPVector<int>();
				return R::OK;
			}
	));

	pEPTest->RegisterAndRunTC(kPushBack, kEPVector, EPTestCase::expected::COMPARE,
		EPTimedFunction<RESULT(void)>(
			[&]() -> RESULT {
				for (int i = 0; i < TEST_INT_ARRAY_LENGTH; i++) {
					intArray.PushBack(i);
				}
				return R::OK;
			}
	));

	CBM(intArray.size() == TEST_INT_ARRAY_LENGTH,
		"Size %zu differs from expected size %d", 
			intArray.size(), TEST_INT_ARRAY_LENGTH);

	pEPTest->RegisterAndRunTC(kCheck, kEPVector, EPTestCase::expected::COMPARE,
		EPTimedFunction<RESULT(void)>(
			[&]() -> RESULT {
				for (int i = 0; i < TEST_INT_ARRAY_LENGTH; i++) {
					if (intArray[i] != i) {
						DEBUG_LINEOUT("intArray[%d]:%d differed from value %d expected", i, intArray[i], i);
						return R::FAIL;
					}
				}
				return R::OK;
			}
	));

	pEPTest->RegisterAndRunTC(kForEach, kEPVector, EPTestCase::expected::COMPARE,
		EPTimedFunction<RESULT(void)>(
			[&]() -> RESULT {
				int count = 0;
				for (auto &val : intArray) {
					if (val != count) {
						DEBUG_LINEOUT("intArray[%d]:%d differed from value %d expected", count, val, count);
						return R::FAIL;
					}
					count++;
				}
				return R::OK;
			}
	));

	// Case 2 - Check front insertion
	intArray = EPVector<int>();

	pEPTest->RegisterAndRunTC(kPushFront, kEPVector, EPTestCase::expected::COMPARE,
		EPTimedFunction<RESULT(void)>(
			[&]() -> RESULT {
				for (int i = TEST_INT_ARRAY_LENGTH - 1; i >= 0; i--) {
					intArray.PushFront(i);
				}
				return R::OK;
			}
	));

	CBM(intArray.size() == TEST_INT_ARRAY_LENGTH,
		"Size %zu differs from expected size %d", 
			intArray.size(), TEST_INT_ARRAY_LENGTH);

	for (int i = 0; i < TEST_INT_ARRAY_LENGTH; i++) {
		CBM(intArray[i] == i, 
			"intArray[%d]:%d differed from value %d expected", i, intArray[i], i);
	}

	// Case 3 - Test performance against std::vector

	pEPTest->RegisterAndRunTC(kConstruct, kSTLVector, EPTestCase::expected::COMPARE,
		EPTimedFunction<RESULT(void)>(
			[&]() -> RESULT {
				stdIntArray = std::vector<int>();
			return R::OK;
			}
	));

	pEPTest->RegisterAndRunTC(kPushBack, kSTLVector, EPTestCase::expected::COMPARE,
		EPTimedFunction<RESULT(void)>(
			[&]() -> RESULT {
				for (int i = 0; i < TEST_INT_ARRAY_LENGTH; i++) {
					stdIntArray.push_back(i);
				}
				return R::OK;
			}
	));

	pEPTest->RegisterAndRunTC(kCheck, kSTLVector, EPTestCase::expected::COMPARE,
		EPTimedFunction<RESULT(void)>(
			[&]() -> RESULT {
				for (int i = 0; i < TEST_INT_ARRAY_LENGTH; i++) {
					if (stdIntArray[i] != i) {
						DEBUG_LINEOUT("intArray[%d]:%d differed from value %d expected", i, intArray[i], i);
						return R::FAIL;
					}
				}			
				return R::OK;
			}
	));

	pEPTest->RegisterAndRunTC(kForEach, kSTLVector, EPTestCase::expected::COMPARE,
		EPTimedFunction<RESULT(void)>(
			[&]() -> RESULT {
				int count = 0;
				for (auto& val : stdIntArray) {
					if (val != count) {
						DEBUG_LINEOUT("stdIntArray[%d]:%d differed from value %d expected", count, val, count);
						return R::FAIL;
					}
					count++;
				}
				return R::OK;
			}
	));

	stdIntArray = std::vector<int>();

	pEPTest->RegisterAndRunTC(kPushFront, kSTLVector, EPTestCase::expected::COMPARE,
		EPTimedFunction<RESULT(void)>(
			[&]() -> RESULT {
				for (int i = TEST_INT_ARRAY_LENGTH - 1; i >= 0; i--) {
					stdIntArray.insert(stdIntArray.begin(), i);
				}
				return R::OK;
			}
	));

	for (int i = 0; i < TEST_INT_ARRAY_LENGTH; i++) {
		CBM(stdIntArray[i] == i, "intArray[%d]:%d differed from value %d expected", i, intArray[i], i);
	}
	
Error:
	return r;
}