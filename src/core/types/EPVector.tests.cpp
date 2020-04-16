#include "TypesTestSuite.h"

#define TEST_INT_ARRAY_LENGTH 1000

#include <vector>

#include "test/EPTestCase.h"

RESULT TypesTestSuite::TestEPDynamicStorage(EPTestBase *pEPTestBase) {
	RESULT r = R::OK;

	EPVector<int> intArray;
	std::vector<int> stdIntArray;

	// EPVector test cases
	EPTestCase tcEPVectorConstruct;
	EPTestCase tcEPVectorPushBack;
	EPTestCase tcEPVectorCheck;
	EPTestCase tcEPVectorPushFront;

	// std::vector test cases (for comparisons) 
	EPTestCase tcSTDVectorConstruct;
	EPTestCase tcSTDVectorPushBack;
	EPTestCase tcSTDVectorCheck;
	EPTestCase tcSTDVectorInsertFront;

	EPTest<RESULT(EPTestBase*)>* pEPTest = dynamic_cast<EPTest<RESULT(EPTestBase*)>*>(pEPTestBase);
	CNM(pEPTest, "EPTest is nullptr");

	// TODO: Wrap up the below as test cases
	// TODO: Create a "ep timed function"

	// Case 1 - Check push back
	// TODO: get rid of result return
	tcEPVectorConstruct = EPTestCase::MakeAndRun("construct", "EPVector",
		EPTimedFunction<RESULT(void)>(
			[&]() -> RESULT {
				intArray = EPVector<int>();
				return R::OK;
			}
	));

	pEPTest->RegisterAndRunTC("construct", "EPVector",
		EPTimedFunction<RESULT(void)>(
			[&]() -> RESULT {
				intArray = EPVector<int>();
				return R::OK;
			}
	));

	tcEPVectorPushBack = EPTestCase::MakeAndRun("pushback", "EPVector",
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

	tcEPVectorCheck = EPTestCase::MakeAndRun("check", "EPVector",
		EPTimedFunction<RESULT(void)>(
			[&]() -> RESULT {
				RESULT r = R::OK;
				for (int i = 0; i < TEST_INT_ARRAY_LENGTH; i++) {
					CBM(intArray[i] == i, "intArray[%d]:%d differed from value %d expected", i, intArray[i], i);
				}
			Error:
				return r;
			}
	));

	// Case 2 - Check front insertion
	intArray = EPVector<int>();

	tcEPVectorPushFront = EPTestCase::MakeAndRun("pushfront", "EPVector",
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

	tcSTDVectorConstruct = EPTestCase::MakeAndRun("construct", "std::vector",
		EPTimedFunction<RESULT(void)>(
			[&]() -> RESULT {
				stdIntArray = std::vector<int>();
			return R::OK;
			}
	));

	tcSTDVectorPushBack = EPTestCase::MakeAndRun("pushback", "std::vector",
		EPTimedFunction<RESULT(void)>(
			[&]() -> RESULT {
				for (int i = 0; i < TEST_INT_ARRAY_LENGTH; i++) {
					stdIntArray.push_back(i);
				}
				return R::OK;
			}
	));

	tcSTDVectorCheck = EPTestCase::MakeAndRun("check", "std::vector",
		EPTimedFunction<RESULT(void)>(
			[&]() -> RESULT {
				RESULT r = R::OK;
				for (int i = 0; i < TEST_INT_ARRAY_LENGTH; i++) {
					CBM(stdIntArray[i] == i,
						"intArray[%d]:%d differed from value %d expected", i, intArray[i], i);
				}
			Error:
				return r;
			}
	));

	stdIntArray = std::vector<int>();

	tcSTDVectorInsertFront = EPTestCase::MakeAndRun("insertfront", "std::vector",
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

	// Comparisons and output
	CR(EPTestCase::CompareTestCases(tcEPVectorConstruct, tcSTDVectorConstruct, EPTestCase::expected::COMPARE));
	CR(EPTestCase::CompareTestCases(tcEPVectorPushBack, tcSTDVectorPushBack, EPTestCase::expected::FASTER));
	CR(EPTestCase::CompareTestCases(tcEPVectorCheck, tcSTDVectorCheck, EPTestCase::expected::COMPARE));
	CR(EPTestCase::CompareTestCases(tcEPVectorPushFront, tcSTDVectorInsertFront, EPTestCase::expected::FASTER));
	
Error:
	return r;
}