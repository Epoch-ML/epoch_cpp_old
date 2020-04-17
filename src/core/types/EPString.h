#ifndef EP_STRING_H_
#define EP_STRING_H_

#include "core/ehm/ehm.h"

// epoch string class
// epoch/src/core/types/EPString.h

// String class that works the way WE FUCKING WANT it to

#include "core/types/EPObj.h"
#include "core/types/EPVector.h"

// TODO: expand to wide string as well

template <typename TChar>
class EPString : public EPObj {
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

	inline bool operator==(const EPString& rhs) {
		return strcmp(c_str(), rhs.c_str());
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

	TChar& operator[](size_t idx) {
		return m_stringStorage[idx];
	}

	const TChar& operator[](size_t idx) const {
		return m_stringStorage[idx];
	}

	const size_t length() const { return m_stringStorage.size(); }
	const size_t size() const { return m_stringStorage.size(); }

	const TChar* c_str() {
		return m_stringStorage.GetCBuffer();
	}

private:
	EPVector<TChar> m_stringStorage;
};


#endif // ! EP_STRING_H_