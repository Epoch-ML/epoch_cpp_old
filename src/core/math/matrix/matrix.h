#ifndef MATRIX_H_
#define MATRIX_H_

#include "core/ehm/result.h"

// epoch matrix
// epoch/src/core/math/matrix.h

// Row Major
// N - rows
// M - columns
//#define MATRIX_ROW_MAJOR

// Column Major
#define MATRIX_COLUMN_MAJOR

// TODO: SIMD this stuff

class matrix_base {
public:
	matrix_base() = default;
	~matrix_base() = default;
};

template <typename TValue, int N = 4, int M = 4>
class matrix : public matrix_base {

public:
	TValue data[N * M];
	TValue* GetData() { return data; }

#ifdef RANGE_CHECK
	inline RESULT rangeCheck(unsigned i, unsigned j) const {
		if (i > N)
			return R::MATRIX_ROW_OUT_OF_RANGE;
		if (j > M)
			return R::MATRIX_COL_OUT_OF_RANGE;

		return R::OK;
	}

	inline RESULT rangeCheck(unsigned i) const {
		if (i >= N * M)
			return R::MATRIX_ROW_OUT_OF_RANGE;

		return R::OK;
	}
#endif

public:
	matrix() = default;

	matrix(TValue initVal) {
		if (initVal != 0)
			set(initVal);
		else
			clear();
	}

	~matrix() = default;

	matrix(std::initializer_list<TValue> values) {
		
		// TODO: initializer lists for matrix, point, vector
		//memcpy(data, &values, sizeof(data));

		int index = 0;
		for (auto val : values) {
			data[index++] = val;
		}
	}

	matrix(const matrix& rhs) {
		memcpy(data, rhs.data, sizeof(TValue) * N * M);
	}

	matrix& operator=(const matrix& rhs) {
		memcpy(data, rhs.data, sizeof(TValue) * N * M);
		return *this;
	}

	matrix& operator=(matrix& rhs) {
		memcpy(data, rhs.data, sizeof(TValue) * N * M);
		return *this;
	}

	matrix(matrix&& rhs) {
		memcpy(data, rhs.data, sizeof(TValue) * N * M);
		memset(rhs.data, 0, sizeof(TValue) * N * M);
	}

	matrix& operator=(matrix&& rhs) {
		memcpy(data, rhs.data, sizeof(TValue) * N * M);
		memset(rhs.data, 0, sizeof(TValue) * N * M);

		return *this;
	}

	RESULT PrintMatrix() {
		DEBUG_LINEOUT("matrix %d x %d | N: %d  M:%d ", rows(), cols(), N, M);
		DEBUG_OUT("| ");
		for (int i = 0; i < (N * M); i++) {
			if ((i != 0) && (i % M) == 0) {
				DEBUG_LINEOUT(" |");
				DEBUG_OUT("| ");
			}
			DEBUG_OUT("%02f ", m_data[i]);
		}
		DEBUG_LINEOUT("| ");
		return R::OK;
	}

public:
	// Proxy object passed back from look up
	class proxy {
	public:
		proxy(TValue* pProxyArray) :
			m_pProxyArray(pProxyArray)
		{}

		// TODO: Check bounds
		TValue& operator[](const int index) const {
			return m_pProxyArray[index];
		}

	private:
		TValue* m_pProxyArray = nullptr;
	};

	// TODO: Check bounds
	proxy operator[](const int index) {
		int lookUpValue = (index * N);
		
		//matrix::proxy retProxy(&(data[lookUpValue]));

		return matrix::proxy(&(data[lookUpValue]));
	}

#if defined(MATRIX_ROW_MAJOR)
	#define MATRIX_LOOK_UP(i, j) (data[(i * N) + j])
#else 
	#define MATRIX_LOOK_UP(i, j) (data[(j * M) + i])
#endif

	TValue& operator()(unsigned i) {
#ifdef RANGE_CHECK
		rangeCheck(i);
#endif
		return data[i];
	}

	const TValue& operator()(unsigned i) const {
#ifdef RANGE_CHECK
		rangeCheck(i);
#endif
		return data[i];
	}

	TValue& operator()(unsigned i, unsigned j) {
#ifdef RANGE_CHECK
		rangeCheck(i, j);
#endif
		return MATRIX_LOOK_UP(i, j);
	}

	const TValue& operator()(unsigned i, unsigned j) const {
#ifdef RANGE_CHECK
		rangeCheck(i, j);
#endif
		return MATRIX_LOOK_UP(i, j);
	}

	inline const TValue& element(unsigned i, unsigned j) const {
#ifdef RANGE_CHECK
		rangeCheck(i, j);
#endif
		return MATRIX_LOOK_UP(i, j);
	}

	inline TValue& element(unsigned i, unsigned j) {
#ifdef RANGE_CHECK
		rangeCheck(i, j);
#endif
		return MATRIX_LOOK_UP(i, j);
	}

public:
	static int rows() { return N; }
	static int cols() { return M; }

	inline RESULT clear() {
		memset(&data, 0, sizeof(TValue) * N * M);
		return R::OK;
	}

	inline RESULT set(TValue val) {
		for (int i = 0; i < (N * M); i++) {
			data[i] = val;
		}

		return R::OK;
	}

	inline RESULT SetIdentity(TValue val = 1.0f) {
		RESULT r = R::OK;

		// Ensure square matrix
		CBM((N == M), "Cant sent identity matrix on %dx%d dimensions", N, M);

		clear();

		for (int i = 0; i < N; i++) {
			this->element(i, i) = val;
		}

	Error:
		return r;
	}

	RESULT randomize(TValue maxval = 10.0f) {
		srand(time(nullptr));

		for (int i = 0; i < (N * M); i++) {
			float randval = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			data[i] = maxval * randval;
		}

		return R::OK;
	}

	inline TValue sum() noexcept {
		TValue sumVal = 0.0f;

		for (size_t i = 0; i < (N * M); i++) {
			sumVal += data[i];
		}

		return sumVal;
	}

	/*
	// TODO: determinant 
	RESULT normalize() {
		TValue s = sum();
		
		if(s != 0.0f)
			operator*=(1.0f/s);
		
		return R::OK;
	}

	matrix normal() {
		TValue s = sum();

		if (s != 0.0f)
			return matrix(*this).operator*=(1.0f / s);
		else
			return matrix();
	}
	*/

	static matrix<TValue, N, M> identity(TValue val = 1.0f) {
		matrix<TValue, N, M> retMatrix;
		retMatrix.identity(val);
		return retMatrix;
	}

	static matrix<TValue, N, M> zeros() {
		matrix<TValue, N, M> retMatrix;
		retMatrix.clear();
		return retMatrix;
	}

	static matrix<TValue, N, M> ones() {
		matrix<TValue, N, M> retMatrix;
		retMatrix.set(1.0f);
		return retMatrix;
	}

	// Matrix Operators
	//matrix& operator*=(const matrix& rhs) {
	//	for (int i = 0; i < (N * M); i++)
	//		data[i] *= a;
	//
	//	return *this;
	//}
	//matrix operator*(const matrix& rhs) { return matrix(*this).operator*=(rhs); }

	matrix& operator+=(const matrix& rhs) {
		for (size_t i = 0; i < (N * M); i++) {
			data[i] += rhs.data[i];
		}
		return *this;
	}
	matrix operator+(const matrix& rhs) { return matrix(*this).operator+=(rhs); }

	matrix& operator-=(const matrix& rhs) {
		for (size_t i = 0; i < (N * M); i++) {
			data[i] -= rhs.data[i];
		}
		return *this;
	}
	matrix operator-(const matrix& rhs) { return matrix(*this).operator-=(rhs); }

	// Scalar Operators
	matrix& operator+=(TValue val) {
		for (size_t i = 0; i < (N * M); i++) {
			data[i] += val;
		}
		return *this;
	}
	matrix operator+(TValue val) { return matrix(*this).operator+=(val); }

	matrix& operator-=(TValue val) {
		for (size_t i = 0; i < (N * M); i++) {
			data[i] -= val;
		}
		return *this;
	}
	matrix operator-(TValue val) { return matrix(*this).operator-=(val); }
	matrix operator-() { return matrix(*this).operator*=(-1.0f); }

	matrix& operator*=(TValue val) {
		for (size_t i = 0; i < (N * M); i++) {
			data[i] *= val;
		}
		return *this;
	}
	matrix operator*(TValue val) { return matrix(*this).operator*=(val); }
	friend matrix operator*(TValue val, const matrix& rhs) { return matrix(rhs).operator*=(val); }

	matrix& operator/=(TValue val) {
		for (size_t i = 0; i < (N * M); i++) {
			data[i] /= val;
		}
		return *this;
	}
	matrix operator/(TValue val) { return matrix(*this).operator/=(val); }

	matrix& operator%=(TValue val) {
		for (size_t i = 0; i < (N * M); i++) {
			data[i] %= val;
		}
		return *this;
	}
	matrix operator%(TValue val) { return matrix(*this).operator%=(val); }

};

// TODO: SIMD this baby
// TODO: Move into above?
// Matrix multiply 

// square matrix multiplication
template <typename TValue, int N>
matrix<TValue, N, N> operator*(
	const matrix<TValue, N, N>& lhs, 
	const matrix<TValue, N, N>& rhs) 
{
	matrix<TValue, N, N> matResult(0);

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < N; k++) {
				matResult.element(i, j) += lhs.element(i, k) * rhs.element(k, j);
			}
		}
	}

	return matResult;
}

// N x M multiplied by a M x 1
template <typename TValue, int N, int M>
matrix<TValue, N, 1> operator*(
	const matrix<TValue, N, M>& lhs, 
	const matrix<TValue, M, 1>& rhs) 
{
	matrix<TValue, N, 1> vResult(0);

	for (int i = 0; i < N; i++)
		for (int k = 0; k < M; k++)
			vResult.element(i, 0) += lhs.element(i, k) * rhs.element(k, 0);

	return vResult;
}

// N x M multiplied by M x W, should work for all N, M, W
template <typename TValue, int N, int M, int W>
matrix<TValue, N, W> operator*(
	const matrix<TValue, N, M>& lhs, 
	const matrix<TValue, M, W>& rhs) 
{
	matrix<TValue, N, W> matResult(0);

	for (int i = 0; i < N; i++)
		for (int j = 0; j < W; j++)
			for (int k = 0; k < M; k++)
				matResult.element(i, j) += lhs.element(i, k) * rhs.element(k, j);

	return matResult;
}

#endif // ! MATRIX_H_