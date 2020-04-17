
#include <iostream>
#include <cstdint>
#include <emmintrin.h> // Define SSE2 intrinsic functions
#include <immintrin.h>


#define SIZE 1024

short int a[SIZE], b[SIZE], c[SIZE];
short int a2[SIZE], b2[SIZE], c2[SIZE];

float vec1[3] = { 1.0f, 2.0f, 3.0f };
float vec2[3] = { 1.0f, 2.0f, 3.0f };

alignas(16) float simdAddVec1[SIZE];
alignas(16) float simdAddVec2[SIZE];
alignas(32) float simdAddVec3[SIZE];
alignas(32) float simdAddVec4[SIZE];

// dot product function assumes both arrays are the same size
// so we don't check for that error
void dot_product(float v1[], float v2[], float& result, int size) {
	for (int i = 0; i < size; ++i) {
		result += v1[i] * v2[i];
	}
}

void automaticVectorizationExample() {
	// This will auto vectorize with most compilers
	for (int i = 0; i < SIZE; i++) {
		a[i] = b[i] + 2;
	}
}

// Loop with branch
void SelectAddMul(short int aa[], short int bb[], short int cc[]) {
	for (int i = 0; i < SIZE; i++) {
		aa[i] = (bb[i] > 0) ? (cc[i] + 2) : (bb[i] * cc[i]);
		//printf("%d\r\n", aa[i]);
	}

}

// Branch/loop function vectorized:
void SelectAddMul2(short int aa[], short int bb[], short int cc[]) {
	// Make a vector of (0,0,0,0,0,0,0,0)
	__m128i zero = _mm_set1_epi16(0);

	// Make a vector of (2,2,2,2,2,2,2,2)
	__m128i two = _mm_set1_epi16(2);
	
	// Roll out loop by eight to fit the eight-element vectors:
	for (int i = 0; i < SIZE; i += 8) {
		// Load eight consecutive elements from bb into vector b:
		__m128i b = _mm_loadu_si128((__m128i const*)(bb + i));

		// Load eight consecutive elements from cc into vector c:
		__m128i c = _mm_loadu_si128((__m128i const*)(cc + i));
		
		// Add 2 to each element in vector c
		__m128i c2 = _mm_add_epi16(c, two);
		
		// Multiply b and c
		__m128i bc = _mm_mullo_epi16(b, c);
		
		// Compare each element in b to 0 and generate a bit-mask:
		__m128i mask = _mm_cmpgt_epi16(b, zero);
		
		// AND each element in vector c2 with the bit-mask:
		c2 = _mm_and_si128(c2, mask);
		
		// AND each element in vector bc with the inverted bit-mask:
		bc = _mm_andnot_si128(mask, bc);
		
		// OR the results of the two AND operations:
		__m128i a = _mm_or_si128(c2, bc);
		
		// Store the result vector in eight consecutive elements in aa:
		_mm_storeu_si128((__m128i*)(aa + i), a);
	}
}

// addition function without simd
void AddNoSIMD(float v1[], float v2[], float& result, int size) {
	float tmpResult = 0.f;
	for (int i = 0; i < size; ++i) {
		tmpResult += v1[i] + v2[i];
	}
	result = tmpResult;
}

// addition function with SIMD SSE
void AddSSE(float v1[], float v2[], float& result, int size) {
	// Make a results vector since the above memory is 
	// actually the array locations and we don't want to change those
	__m128 resultSIMD = _mm_set_ps1(0);
	__m128 first = _mm_load_ps(v1);
	__m128 second = _mm_load_ps(v2);

	// Roll out loop by 4 to fit the four-element vectors
	for (int i = 0; i < SIZE; i += 4) {
		// Make vectors for the 4 results per operation
		first = _mm_load_ps(v1 + i);
		second = _mm_load_ps(v2 + i);

		// Add the first and second to the result
		resultSIMD = _mm_add_ps(resultSIMD, first);
		resultSIMD = _mm_add_ps(resultSIMD, second);
	}
	// take the result and add its contents up for final result
	// note I did things this way to compare results of functions
	// easily

	alignas(16) float results[4] = { 0, 0, 0, 0 };
	_mm_store_ps(results, resultSIMD);

	// add up all the results for final result
	result = (results[0] + results[1]) + (results[2] + results[3]);

}
// addition function with SIMD AVX
void AddAVX(float v1[], float v2[], float& result, int size) {
	// Make a results vector since the above memory is 
	// actually the array locations and we don't want to change those
	__m256 resultSIMD = _mm256_set1_ps(0);

	// Roll out loop by 8 to fit the eight-element vectors
	for (int i = 0; i < SIZE; i += 8) {
		// Make vectors for the 8 results per operation
		__m256 first = _mm256_load_ps(v1 + i);
		__m256 second = _mm256_load_ps(v2 + i);
		// Add the first and second to the result
		resultSIMD = _mm256_add_ps(resultSIMD, first);
		resultSIMD = _mm256_add_ps(resultSIMD, second);
	}

	// take the result and add its contents up for final result
	// note I did things this way to compare results of functions
	// easily
	alignas(32) float results[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	_mm256_store_ps(results, resultSIMD);

	// add up all the results for final result
	result = (results[0] + results[1]) + (results[2] + results[3]) +
		(results[4] + results[5]) + (results[6] + results[7]);
}



void fillArrays() {
	for (int j = 0; j < SIZE; j++) {
		a[j] = rand() % 100;
		a2[j] = a[j];
		b[j] = rand() % 100;
		b2[j] = b[j];
		c[j] = rand() % 100;
		c2[j] = c2[j];

		simdAddVec1[j] = 1.f;
		simdAddVec2[j] = 1.f;
		simdAddVec3[j] = 1.f;
		simdAddVec4[j] = 1.f;
	}
}

// get three numbers and average them
int main(int argc, char* argv[]) {

	fillArrays();

	automaticVectorizationExample();

	fillArrays();

	float result = 0;
	dot_product(vec1, vec2, result, 3);

	for (int i = 0; i < 6000; i++) {
		SelectAddMul(a, b, c);
		SelectAddMul2(a2, b2, c2);
	}

	// Add SIMD example

	fillArrays();

	for (int i = 0; i < 6000; i++) {
		float result = 0;
		AddNoSIMD(simdAddVec1, simdAddVec2, result, SIZE);
		//printf("%f\r\n", result);
		AddSSE(simdAddVec1, simdAddVec2, result, SIZE);
		//printf("%f\r\n", result);
		//addNoSimd(simdAddVec3, simdAddVec4, result, SIZE);
		//printf("%f\r\n", result);
		AddAVX(simdAddVec3, simdAddVec4, result, SIZE);
		//printf("%f\r\n", result);
	}


	//dot_product(vec1, vec2, result, 3);

	//for (int i = 0; i < 6000; i++) {
	//	SelectAddMul(a, b, c);
	//	SelectAddMul2(a2, b2, c2);
	//}
}

