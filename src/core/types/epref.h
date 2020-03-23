#ifndef EP_REF_H_
#define EP_REF_H_

#include "core/ehm/result.h"

// epoch reference
// epoch/src/core/types/epref.h

// A simple referenced count object

#include "core/types/EPObject.h"

#include <type_traits>

#ifndef NOCONCEPTS
	template <typename TEPObj> requires EPObjectDerived<TEPObj>
#else
	template <typename TEPObj>
#endif
class epref {
public:
	epref() {
		// 
	}

	~epref() {
		// 
	}

	TEPObj* get() {
		return m_pEPObj;
	}

public:
	bool operator==(const TEPObj* pEPObj) const {
		return m_pEPObj == pEPObj;
	}

	bool operator!=(const TEPObj* pEPObj) const {
		return m_pEPObj != pEPObj;
	}

	bool operator==(const epref<TEPObj> &pEPObj) const {
		return m_pEPObj == pEPObj.m_pEPObj;
	}

	bool operator!=(const epref<TEPObj>& pEPObj) const {
		return m_pEPObj != pEPObj.m_pEPObj;
	}

private:
	unsigned int m_refCount = 0;
	TEPObj* m_pEPObj = nullptr;
};

#endif // EP_REF_H_