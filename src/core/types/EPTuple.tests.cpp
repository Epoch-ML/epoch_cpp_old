#include "TypesTestSuite.h"

#include "core/types/EPRef.h"
#include "core/types/EPTuple.h"

RESULT TestEPTuple(EPTestBase* pEPTestBase);

RESULT TypesTestSuite::TestEPTuple(EPTestBase* pEPTestBase) {
	RESULT r = R::OK;

	// Case 1 tuple<int, int>
	{
		EPTuple<int, int> int_tuple = EPTuple<int, int>(5, 7);
		
		CBM((int_tuple.GetNumElements() == 2), "Expected 2 elements");

		// Get operator for int
		CBM((int_tuple.get<0,int>() == 5), 
			"Failed to create a valid tuple %d expected 5", (int_tuple.get<0,int>()));

		CBM((int_tuple.get<1,int>() == 7), 
			"Failed to create a valid tuple %d expected 7", (int_tuple.get<1,int>()));

		// Set operator
		int_tuple.set<1, int>(10);

		CBM((int_tuple.get<1,int>() == 10), 
			"Failed to create a valid tuple %d expected 10", (int_tuple.get<1,int>()));

		// equality 
		EPTuple<int, int> int_tuple_2 = EPTuple<int, int>(5, 10);

		CBM((int_tuple == int_tuple_2), "Tuples should be equivalent");
	}
	
	// Case 1 - tuple<bool, uint32_t>
	{
		EPTuple<bool, uint32_t> bool_uint32_tuple = EPTuple<bool, uint32_t>(true, 0xAA);

		// Get
		CBM((bool_uint32_tuple.get<0,bool>() == true), 
			"Failed to create a valid tuple %d expected 1", (int)(bool_uint32_tuple.get<0,bool>()));

		CBM((bool_uint32_tuple.get<1,uint32_t>() == 0xAA), 
			"Failed to create a valid tuple 0x%x expected 0xAA", (bool_uint32_tuple.get<1,uint32_t>()));
	}

Error:
	return r;
}