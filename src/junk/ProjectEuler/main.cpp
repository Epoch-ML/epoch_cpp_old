#include "ProjectEuler.h"

#include <stdio.h>

#include "core/types/EPTuple.h"
#include "core/types/EPFunction.h"

int main(int argc, char* argv[]) {
	RESULT r = RESULT::OK;

	DEBUG_LINEOUT("Starting...");

	//ProjectEuler projectEuler; 
	//projectEuler.Run("problem 8");

	// Test the tuple
	tuple<int, int, int> newTuple(5, 10, 15);
	DEBUG_LINEOUT("(%d, %d, %d)", 
		newTuple.get<0, int>(), 
		newTuple.get<1, int>(), 
		newTuple.get<2, int>()
	);

	// Test our EPfunction
	EPFunction<RESULT(int)> fnEpfunction = [](int a) -> RESULT {
		RESULT r = R::OK;

		if (a == 5)
			r = R::FAIL;

		return r;
	};

	CRM(fnEpfunction(5), "this should fail bro");

Error:
	return 0;
}