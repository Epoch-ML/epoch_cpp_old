#include "TypesTestSuite.h"

#define TEST_INT_ARRAY_LENGTH 1000

#include <list>

#include "test/EPTest.h"
#include "test/EPTestCase.h"

#include "core/types/EPList.h"

RESULT TypesTestSuite::TestEPList(EPTestBase* pEPTestBase) {
	RESULT r = R::OK;

	EPList<int> intArray;
	std::list<int> stdIntArray;

	// EPVector test case labels
	const char* kConstruct = "construct";
	const char* kPushBack = "pushback";
	const char* kCheck = "check";
	const char* kPushFront = "pushfront";
	const char* kForEach = "foreach";
	const char* kForIterator = "foriterator";
	const char* kPopBack = "popback";
	const char* kPopFront = "popfront";

	const char* kEPList = "EPList";
	const char* kSTLList = "std::list";

	EPTest<RESULT(EPTestBase*)>* pEPTest = dynamic_cast<EPTest<RESULT(EPTestBase*)>*>(pEPTestBase);
	CNM(pEPTest, "EPTest is nullptr");

	// Case 1 - Check push back
	// TODO: get rid of result return

	pEPTest->RegisterAndRunTC(kConstruct, kEPList, EPTestCase::expected::COMPARE,
		EPTimedFunction<RESULT(void)>(
			[&]() -> RESULT {
				intArray = EPList<int>();
				return R::OK;
			}
	));

	pEPTest->RegisterAndRunTC(kPushBack, kEPList, EPTestCase::expected::COMPARE,
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

	pEPTest->RegisterAndRunTC(kCheck, kEPList, EPTestCase::expected::COMPARE,
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

	pEPTest->RegisterAndRunTC(kForEach, kEPList, EPTestCase::expected::COMPARE,
		EPTimedFunction<RESULT(void)>(
			[&]() -> RESULT {
				int count = 0;
				for (auto& val : intArray) {
					if (val != count) {
						DEBUG_LINEOUT("intArray[%d]:%d differed from value %d expected", count, val, count);
						return R::FAIL;
					}
					count++;
				}
				return R::OK;
			}
	));

	pEPTest->RegisterAndRunTC(kForIterator, kEPList, EPTestCase::expected::COMPARE,
		EPTimedFunction<RESULT(void)>(
			[&]() -> RESULT {
				int count = 0;
				for (auto it = intArray.begin(); it != intArray.end(); it++) {
					int val = *it;
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
	intArray = EPList<int>();

	pEPTest->RegisterAndRunTC(kPushFront, kEPList, EPTestCase::expected::COMPARE,
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

	pEPTest->RegisterAndRunTC(kConstruct, kSTLList, EPTestCase::expected::COMPARE,
		EPTimedFunction<RESULT(void)>(
			[&]() -> RESULT {
				stdIntArray = std::list<int>();
				return R::OK;
			}
	));

	pEPTest->RegisterAndRunTC(kPushBack, kSTLList, EPTestCase::expected::COMPARE,
		EPTimedFunction<RESULT(void)>(
			[&]() -> RESULT {
				for (int i = 0; i < TEST_INT_ARRAY_LENGTH; i++) {
					stdIntArray.push_back(i);
				}
				return R::OK;
			}
	));

	// Not supported by std::list
	//pEPTest->RegisterAndRunTC(kCheck, kSTLList, EPTestCase::expected::COMPARE,
	//	EPTimedFunction<RESULT(void)>(
	//		[&]() -> RESULT {
	//			for (int i = 0; i < TEST_INT_ARRAY_LENGTH; i++) {
	//				if (stdIntArray[i] != i) {
	//					DEBUG_LINEOUT("intArray[%d]:%d differed from value %d expected", i, intArray[i], i);
	//					return R::FAIL;
	//				}
	//			}
	//			return R::OK;
	//		}
	//));

	pEPTest->RegisterAndRunTC(kForEach, kSTLList, EPTestCase::expected::COMPARE,
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

	pEPTest->RegisterAndRunTC(kForIterator, kSTLList, EPTestCase::expected::COMPARE,
		EPTimedFunction<RESULT(void)>(
			[&]() -> RESULT {
				int count = 0;
				for (auto it = stdIntArray.begin(); it != stdIntArray.end(); it++) {
					int val = *it;
					if (val != count) {
						DEBUG_LINEOUT("intArray[%d]:%d differed from value %d expected", count, val, count);
						return R::FAIL;
					}
					count++;
				}
				return R::OK;
			}
	));

	stdIntArray = std::list<int>();

	pEPTest->RegisterAndRunTC(kPushFront, kSTLList, EPTestCase::expected::COMPARE,
		EPTimedFunction<RESULT(void)>(
			[&]() -> RESULT {
				for (int i = TEST_INT_ARRAY_LENGTH - 1; i >= 0; i--) {
					stdIntArray.push_front(i);
				}
				return R::OK;
			}
	));

	// Check values (square bracket operator not supported)
	{
		int count = 0;
		for (auto it = stdIntArray.begin(); it != stdIntArray.end(); it++) {
			int val = *it;
			CBM((val == count), "intArray[%d]:%d differed from value %d expected", count, val, count);
			count++;
		}
	}

Error:
	return r;
}