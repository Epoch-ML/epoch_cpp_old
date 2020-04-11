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

	EPString(const TChar &szString[]) :
		m_stringStorage(szString)
	{
		//
	}

	~EPString() = default;

private:
	EPDynamicStorage<TChar> m_stringStorage;
};


#endif // ! EP_STRING_H_