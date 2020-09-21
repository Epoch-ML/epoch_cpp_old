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
		size_t strResult_n = lhs.size() + rhs.size() - 1;
		m_stringStorage = EPVector<TChar>(strResult_n, true);
		memcpy(m_stringStorage.data(), lhs.c_str(), (lhs.length() - 1) * sizeof(char));
		memcpy(m_stringStorage.data() + (lhs.length() - 1), rhs.c_str(), rhs.length() * sizeof(char));
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
		for (unsigned long i = 0; i < std::min(length(), rhs.length()); i++) {
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

	EPString operator+(const EPString& rhs) const {
		EPString strReturn = EPString(this->length() + rhs.length() - 1, true);
		
		memcpy(strReturn.m_stringStorage.data(), this->c_str(), (this->length() - 1) * sizeof(char));
		memcpy(strReturn.m_stringStorage.data() + (this->length() - 1), rhs.c_str(), rhs.length() * sizeof(char));

		return strReturn;
	}

	TChar& operator[](size_t idx) {
		return m_stringStorage[idx];
	}

	const TChar& operator[](size_t idx) const {
		return m_stringStorage[idx];
	}

	size_t length() const { return m_stringStorage.size(); }
	size_t size() const { return m_stringStorage.size(); }

	const TChar* c_str() const {
		return m_stringStorage.GetCBuffer();
	}

private:
	EPVector<TChar> m_stringStorage;
};


#endif // ! EP_STRING_H_