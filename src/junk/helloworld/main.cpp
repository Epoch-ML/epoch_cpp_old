#include "main.h"

#include <stdio.h>

void PrintHelloWorld() {

	// From global constant
	DEBUG_LINEOUT(g_kszHelloWorld);

	// From locally scoped static string
	DEBUG_LINEOUT("Hello, Worlds!");
}

int main(UNUSED int argc, UNUSED char *argv[]) {
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