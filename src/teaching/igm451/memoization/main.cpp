#include "core/ehm/ehm.h"

#include <iostream>
#include <string>
#include <chrono>

using namespace std;

// yes, I know this large number will overflow int, but it
// demonstrates the issue with recursion failing where
// iteration doesn't
// you may need to change this number to fail on your
// particular system
#define MAX_ALLOWED 90000

// the beginning before we initialize elements in the arrays
#define MIN_ALLOWED 0

// global arrays to memoize and use dynamic programming with

int factorialArray[MAX_ALLOWED];
int factorialUsed = MIN_ALLOWED;
int fibArray[MAX_ALLOWED];
int fibUsed = MIN_ALLOWED;

// why recursive algorithms aren't generally good
// this will fail when the below version won't
int factorialRecursive(int n) {
	if (n < MAX_ALLOWED) {
		if (n <= 0 || n == 1) {
			return 1;
		}
		else {
			return n * factorialRecursive(n - 1);
		}
	}
	else return -1;

}

// memoization and dynamic programming for factorial
int factorial(int n) {
	if (n < MAX_ALLOWED) {
		if (n <= factorialUsed) {
			return factorialArray[n];
		}
		else {
			int product = factorialArray[factorialUsed];
			for (int i = factorialUsed + 1; i <= n; i++) {
				product *= i;
				factorialArray[i] = product;
			}
			factorialUsed = n;
			return product;
		}
	}
	else return -1;
}

// Memoization and dynamic programming for Fibonacci
int fib(int n) {
	if (n < MAX_ALLOWED) {
		if (n <= fibUsed) {
			return fibArray[n];
		}
		else {
			int fib = 0;
			for (int i = fibUsed + 1; i <= n; i++) {
				fibArray[i] = fibArray[i - 1] + fibArray[i - 2];
			}
			fibUsed = n;
			return fibArray[fibUsed];
		}
	}
	else return -1;
}

// simple tests
int main() {
	RESULT r = R::OK;

	// This will overflow below
	int value = 0; 

	auto timeStart = std::chrono::high_resolution_clock::now();
	auto timeFinish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(timeFinish - timeStart);

	// need to initialize the arrays with the first few elements
	factorialArray[0] = 1;
	factorialArray[1] = 1;
	factorialArray[2] = 2;
	factorialUsed = 2;
	value = factorial(12);
	
	DEBUG_LINEOUT("%d", value);

	value = factorial(13);
	DEBUG_LINEOUT("%d", value);


	value = factorial(MAX_ALLOWED);
	DEBUG_LINEOUT("%d", value); // should be -1
	fibArray[0] = 0;
	fibArray[1] = 1;
	fibUsed = 1;
	value = fib(20);
	DEBUG_LINEOUT("%d", value);
	value = fib(MAX_ALLOWED);
	DEBUG_LINEOUT("%d", value); // should be -1

	// let's check some timings
	timeStart = std::chrono::high_resolution_clock::now();
	value = factorial(MAX_ALLOWED - 1);
	timeFinish = std::chrono::high_resolution_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(timeFinish - timeStart);
	cout << elapsed.count() << endl;

	timeStart = std::chrono::high_resolution_clock::now();
	value = fib(MAX_ALLOWED - 1);
	timeFinish = std::chrono::high_resolution_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(timeFinish - timeStart);
	cout << elapsed.count() << endl;

	timeStart = std::chrono::high_resolution_clock::now();
	value = factorialRecursive(80000);
	timeFinish = std::chrono::high_resolution_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(timeFinish - timeStart);
	cout << elapsed.count() << endl;

	// this should do two things:
	// 1. takes about double the time for the timing above
	// 2. crashes
	// you may need to adjust MAX_ALLOWED for the crash part
	// the iterative solution is always faster 
	DEBUG_LINEOUT("%d", factorialRecursive(MAX_ALLOWED - 1));

	//system("pause");

Error:
	return (int)(r);
}
