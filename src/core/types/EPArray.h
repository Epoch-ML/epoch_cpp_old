#ifndef EP_ARRAY_H_
#define EP_ARRAY_H_

#include "core/ehm/ehm.h"

// epoch array (storage) class
// epoch/src/core/types/EPArray.h

#include "core/types/EPObj.h"

// TODO: Needs test suite (all the core shits need test suites)

template <typename TStorage, int N>
class EPArray {

public:
	class iterator {
	public:
		iterator(TStorage* pValue) noexcept : m_pValue(pValue) {}
		TStorage& operator*() { return (*m_pValue); }
		TStorage* operator->() { return m_pValue; }

		bool operator==(const iterator& rhs) { return m_pValue == rhs.m_pValue; }
		bool operator!=(const iterator& rhs) { return m_pValue != rhs.m_pValue; }

		iterator& operator++() {
			++m_pValue;
			return *this;
		}

		iterator operator++(int) {
			++m_pValue;
			return *this;
		}

		iterator& operator--() {
			--m_pValue;
			return *this;
		}

		iterator operator--(int) {
			--m_pValue;
			return *this;
		}

	protected:
		TStorage* m_pValue = nullptr;
	};

	class const_iterator : public iterator {
	public:
		const_iterator(TStorage* pValue) noexcept : iterator(pValue) {}
		TStorage& operator*() { return (*m_pValue); }
		TStorage* operator->() { return m_pValue; }
		bool operator==(const const_iterator& rhs) { return m_pValue == rhs.m_pValue; }
		bool operator!=(const const_iterator& rhs) { return m_pValue != rhs.m_pValue; }
	};

	iterator begin() noexcept {
		return iterator(&data[0]);
	}

	const_iterator begin() const noexcept {
		return const_iterator(&data[0]);
	}

	iterator end() noexcept {
		return iterator(&data[N - 1]);
	}

	const_iterator end() const noexcept {
		return const_iterator(&data[N - 1]);
	}

public:
	EPArray()  {
		memset(data, 0, sizeof(data));
	}

	~EPArray() = default;

	EPArray(std::initializer_list<TStorage> list)  {
		memcpy(data, &list, sizeof(data));
	}

	EPArray(const EPArray& rhs) {
		memcpy(data, rhs.data, sizeof(data));
	}

	EPArray& operator=(const EPArray& rhs) {
		memcpy(data, rhs.data, sizeof(data));
		return *this;
	}

	EPArray(EPArray&& rhs) {
		memcpy(data, rhs.data, sizeof(data));
		memset(rhs.data, 0, sizeof(rhs.data));
	}

	EPArray& operator=(EPArray&& rhs) {
		memcpy(data, rhs.data, sizeof(data));
		memset(rhs.data, 0, sizeof(rhs.data));
		return *this;
	}

	const TStorage& operator[](size_t index) const {
		return data[index];
	}

	TStorage& operator [](size_t index) {
		return data[index];
	}

	const size_t size() const {
		return N;
	}

public:
	TStorage data[N];
};

#endif // ! EP_ARRAY_H_