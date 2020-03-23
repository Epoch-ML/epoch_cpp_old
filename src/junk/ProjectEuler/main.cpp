#include "main.h"

#include "euler.h"

#include <stdio.h>

int main(UNUSED int argc, UNUSED char* argv[]) {
	RESULT r = RESULT::OK;

	// Parsed out CRM - this is crashing the compiler?
	char szMessage[] = "Test failure message %d";
	int testMessageArgument = 5;

	PrintHelloWorld();

	CBM(false, "This is supposed to fail");

	// Above is equivalent of
	CRM(R::FAIL, szMessage, testMessageArgument);

Error:
	return 0;
}