#ifndef MATRIX_H_
#define MATRIX_H_

#include "core/ehm/result.h"

// epoch matrix
// epoch/src/core/math/matrix.h

// Row Major
// N - rows
// M - columns
#define MATRIX_ROW_MAJOR

// Column Major
//#define MATRIX_ROW_MAJOR

class matrix_base {
public:
	matrix_base() = default;
	virtual ~matrix_base() = default;
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
	virtual ~matrix() override = default;

	matrix(std::initializer_list<TValue> values) {
		memcpy(data, &values, sizeof(data));
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
	proxy& operator[](const int index) {
		int lookUpValue = (index * N);
		matrix::proxy retProxy(&(data[lookUpValue]));
		return retProxy;
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
		return m_data[i];
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

	RESULT clear() {
		memset(&data, 0, sizeof(TValue) * N * M);
		return R::OK;
	}

	
};

#endif // ! MATRIX_H_