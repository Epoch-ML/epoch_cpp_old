#ifndef EP_DYNAMIC_STORAGE_H_
#define EP_DYNAMIC_STORAGE_H_

#include "core/ehm/ehm.h"

// epoch dynamic storage class
// epoch/src/core/types/EPDynamicStorage.h

// Dynamic storage (similar to vector) class that works the way WE FUCKING WANT IT TO

#include "core/types/EPObj.h"

// TODO: Needs test suite (all the core shits need test suites)

template <typename TStorage>
class EPVector {
public:
	EPVector() :
		m_pBuffer_n(0),
		m_pBuffer(nullptr),
		m_pBuffer_c(0)
	{
		//
	}

	~EPVector() {
		delete[] m_pBuffer;

		m_pBuffer_c = 0;
		m_pBuffer_n = 0;
		m_pBuffer = nullptr;
	}

	EPVector(size_t initialSize, const TStorage& initValue) :
		m_pBuffer_n(initialSize),
		m_pBuffer_c(m_pBuffer_n)
	{
		m_pBuffer = new TStorage[m_pBuffer_n];
	}

	EPVector(const TStorage staticValueArray[], size_t staticValueArray_n) {
		m_pBuffer_n = staticValueArray_n;
		m_pBuffer = new TStorage[m_pBuffer_n];

		memset(m_pBuffer, 0, m_pBuffer_n);
		memcpy(m_pBuffer, staticValueArray, m_pBuffer_n);

		m_pBuffer_c = m_pBuffer_n;
	}

	EPVector(const EPVector& rhs) {
		m_pBuffer_n = rhs.m_pBuffer_n;
		m_pBuffer = new TStorage[m_pBuffer_n];
		m_pBuffer_c = rhs.m_pBuffer_c;
		
		memset(m_pBuffer, 0, m_pBuffer_n);
		memcpy(m_pBuffer, rhs.m_pBuffer, m_pBuffer_n);
	}

	EPVector& operator=(const EPVector& rhs) {
		m_pBuffer_n = rhs.m_pBuffer_n;
		m_pBuffer = new TStorage[m_pBuffer_n];
		m_pBuffer_c = rhs.m_pBuffer_c;

		memset(m_pBuffer, 0, m_pBuffer_n);
		memcpy(m_pBuffer, rhs.m_pBuffer, m_pBuffer_n);

		return *this;
	}

	EPVector(EPVector&& rhs) {
		m_pBuffer_n = rhs.m_pBuffer_n;
		m_pBuffer = rhs.m_pBuffer;
		m_pBuffer_c = rhs.m_pBuffer_c;
		rhs.m_pBuffer = nullptr;
	}

	EPVector& operator=(EPVector&& rhs) {
		m_pBuffer_n = rhs.m_pBuffer_n;
		m_pBuffer = rhs.m_pBuffer;
		m_pBuffer_c = rhs.m_pBuffer_c;
		rhs.m_pBuffer = nullptr;

		return *this;
	}

	inline RESULT Allocate(size_t newSize) {
		RESULT r = R::OK;

		size_t pTempBuffer_n = newSize;
		TStorage* pTempBuffer = new TStorage[pTempBuffer_n];
		CNR(pTempBuffer, R::MEMORY_ALLOCATION_FAILED);

		// set to zero and swap
		memset(pTempBuffer, 0, sizeof(TStorage) * pTempBuffer_n);	// this will ensure null termination 

		std::swap(pTempBuffer, m_pBuffer);
		std::swap(pTempBuffer_n, m_pBuffer_n);

	Error:
		if (pTempBuffer != nullptr) {
			delete[] pTempBuffer;
			pTempBuffer = nullptr;
		}

		return r;
	}

	RESULT PushBack(const TStorage& value) noexcept  {
		RESULT r = R::OK;

		if (m_pBuffer == nullptr) {
			if (this->Allocate(1) != R::OK)
				return R::MEMORY_ALLOCATION_FAILED;
		}

		if (m_pBuffer_c == m_pBuffer_n) {
			if (DoubleSize() != R::OK)
				return R::MEMORY_ALLOCATION_FAILED;
		}

		m_pBuffer[m_pBuffer_c++] = value;

	Error:
		return r;
	}

	inline RESULT PushFront(const TStorage& value) noexcept {
		if (m_pBuffer == nullptr) {
			if (this->Allocate(1) != R::OK)
				return R::MEMORY_ALLOCATION_FAILED;
		}

		if (ShiftRight() != R::OK)
			return R::MEMORY_ALLOCATION_FAILED;

		m_pBuffer[0] = value;

		return R::OK;
	}

	// This is just providing a pointer to the data
	const TStorage* GetCBuffer() const {
		const TStorage* pData = (const TStorage*)m_pBuffer;
		return pData;
	}

	const TStorage& operator[](size_t index) const { 
		return m_pBuffer[index]; 
	}

	TStorage& operator [](size_t index) { 
		return m_pBuffer[index];
	}

	const size_t size() const { 
		return m_pBuffer_c;
	}
	
	inline void clear(bool fDeallocate = true) {
		if (fDeallocate) {
			if (m_pBuffer != nullptr) {
				delete[] m_pBuffer;
				m_pBuffer = nullptr;
			}
			m_pBuffer_c = 0;
			m_pBuffer_n = 0;
		}
		else {
			memset(m_pBuffer, 0, sizeof(TStorage) * m_pBuffer_n);
		}
	}

private:
	inline RESULT ShiftRight() noexcept  {
		if (m_pBuffer_c == m_pBuffer_n) {
			if (DoubleSize() != R::OK)
				return R::MEMORY_ALLOCATION_FAILED;
		}
		
		TStorage tempVal;
		TStorage lastValue = m_pBuffer[0];
		m_pBuffer[0] = 0;

		for (size_t i = 1; i <= m_pBuffer_c; i++) {
			tempVal = m_pBuffer[i];
			m_pBuffer[i] = lastValue;
			lastValue = tempVal;
		}

		m_pBuffer_c++;

		return R::OK;
	}

	inline RESULT DoubleSize() noexcept {
		size_t pTempBuffer_n = m_pBuffer_n << 1;
		TStorage* pTempBuffer = new TStorage[pTempBuffer_n];

		if(pTempBuffer == nullptr)
			return R::MEMORY_ALLOCATION_FAILED;

		// Copy and swap
		memset(pTempBuffer, 0, sizeof(TStorage) * pTempBuffer_n);	// this will ensure null termination 
		memcpy(pTempBuffer, m_pBuffer, sizeof(TStorage) * m_pBuffer_n);

		std::swap(pTempBuffer, m_pBuffer);
		std::swap(pTempBuffer_n, m_pBuffer_n);

		if (pTempBuffer != nullptr) {
			delete[] pTempBuffer;
			pTempBuffer = nullptr;
		}

		return R::OK;
	}

private:
	TStorage* m_pBuffer = nullptr;
	size_t m_pBuffer_n = 0;
	size_t m_pBuffer_c = 0;
};

#endif // ! EP_DYNAMIC_STORAGE_H_