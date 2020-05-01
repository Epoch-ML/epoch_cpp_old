#ifndef EP_STRING_H_
#define EP_STRING_H_

#include "core/ehm/ehm.h"

// epoch string class
// epoch/src/core/types/EPString.h

// String class that works the way WE FUCKING WANT it to

#include "core/types/EPObj.h"
#include "core/types/EPVector.h"

#include <string.h>

// TODO: expand to wide string as well

template <typename TChar>
class EPString {
public:
	// TODO: Maybe make this more general
	struct compare_LT {
		bool operator()(const EPString& lhs, const EPString& rhs) const {
			return lhs < rhs;
		}
	};

public:
	EPString() :
		m_stringStorage()
	{
		// 
	}

	virtual ~EPString() = default;

	EPString(const EPString& rhs) :
		m_stringStorage(rhs.m_stringStorage)
	{
		//
	}

	EPString(EPString&& rhs) :
		m_stringStorage(rhs.m_stringStorage)
	{
		rhs.m_stringStorage.clear(true);
	}

	EPString(const TChar szString[]) :
		m_stringStorage(szString, strlen(szString) + 1)
	{
		//
	}

	EPString(size_t initSize) :
		m_stringStorage(initSize, true)
	{
		//
	}

	EPString(const EPString& lhs, const EPString& rhs) {
		size_t strResult_n = lhs.size() + rhs.size();
		m_stringStorage = EPVector<TChar>(strResult_n, true);
		memcpy(m_stringStorage.data(), lhs.c_str(), lhs.length() * sizeof(TCHAR));
		memcpy(m_stringStorage.data() + lhs.length(), rhs.c_str(), rhs.length() * sizeof(TCHAR));
	}

	EPString(const TChar szString[], size_t szString_n) :
		m_stringStorage(szString, szString_n)
	{
		//
	}

	EPString& operator=(EPString& rhs) {
		m_stringStorage = rhs.m_stringStorage;
		return *this;
	}

	EPString& operator=(const EPString& rhs) {
		m_stringStorage = rhs.m_stringStorage;
		return *this;
	}

	EPString& operator=(EPString&& rhs) {
		m_stringStorage = rhs.m_stringStorage;
		rhs.m_stringStorage.clear(true);
		return *this;
	}

	inline bool operator==(const EPString& rhs) const {
		auto pLeft = c_str();
		auto pRight = rhs.c_str();

		if (pLeft == nullptr && pRight == nullptr) {
			return false;
		}
		else if (pLeft == nullptr || pRight == nullptr) {
			// One is nullptr but the other is not
			return true;
		}

		return strcmp(pLeft, pRight) == 0;
	}

	inline bool operator==(EPString& rhs) {
		auto pLeft = c_str();
		auto pRight = rhs.c_str();

		if (pLeft == nullptr && pRight == nullptr) {
			return false;
		}
		else if (pLeft == nullptr || pRight == nullptr) {
			// One is nullptr but the other is not
			return true;
		}

		return strcmp(pLeft, pRight) == 0;
	}

	inline bool operator==(const TChar* pszCString) const {
		auto pLeft = c_str();
		auto pRight = pszCString;

		if (pLeft == nullptr && pRight == nullptr) {
			return false;
		}
		else if (pLeft == nullptr || pRight == nullptr) {
			// One is nullptr but the other is not
			return true;
		}

		return strcmp(pLeft, pRight) == 0;
	}

	inline bool operator==(TChar* pszCString) {
		auto pLeft = c_str();
		auto pRight = pszCString;

		if (pLeft == nullptr && pRight == nullptr) {
			return false;
		}
		else if (pLeft == nullptr || pRight == nullptr) {
			// One is nullptr but the other is not
			return true;
		}

		return strcmp(pLeft, pRight) == 0;
	}

	inline bool operator<(const EPString& rhs) const {
		for (int i = 0; i < std::min(length(), rhs.length()); i++) {
			TChar cLeft = m_stringStorage[i];
			TChar cRight = rhs[i];

			if (cLeft != cRight) {
				if (cLeft < cRight) {
					return true;
				}
				else {
					return false;
				}
			}
		}

		return false;
	}

	EPString& operator+(const EPString& rhs) const {
		return EPString(*this, rhs);
	}

	TChar& operator[](size_t idx) {
		return m_stringStorage[idx];
	}

	const TChar& operator[](size_t idx) const {
		return m_stringStorage[idx];
	}

	const size_t length() const { return m_stringStorage.size(); }
	const size_t size() const { return m_stringStorage.size(); }

	const TChar* c_str() const {
		return m_stringStorage.GetCBuffer();
	}

private:
	EPVector<TChar> m_stringStorage;
};


#endif // ! EP_STRING_H_