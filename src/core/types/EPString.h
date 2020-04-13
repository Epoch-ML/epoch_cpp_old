#ifndef EP_STRING_H_
#define EP_STRING_H_

#include "core/ehm/ehm.h"

// epoch string class
// epoch/src/core/types/EPString.h

// String class that works the way WE FUCKING WANT it to

#include "core/types/EPObj.h"
#include "core/types/EPDynamicStorage.h"

// TODO: expand to wide string as well

template <typename TChar>
class EPString : public EPObj {
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

	const TChar* c_str() {
		return m_stringStorage.GetCBuffer();
	}

private:
	EPDynamicStorage<TChar> m_stringStorage;
};


#endif // ! EP_STRING_H_