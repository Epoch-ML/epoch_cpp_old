#ifndef EP_REF_H_
#define EP_REF_H_

#include "core/ehm/result.h"

// epoch reference
// epoch/src/core/types/epref.h

// A simple referenced count object

#include "core/types/EPObj.h"

#include <type_traits>

#ifndef NOCONCEPTS
	template <typename TEPObj> requires EPObjectDerived<TEPObj>
#else
	template <typename TEPObj>
#endif
class epref {
public:
	epref() : 
		m_pEPObj(nullptr)
	{}

	epref(const epref& pEPObj) {
		if (m_pEPObj != pEPObj.m_pEPObj) {
			m_pEPObj = pEPObj.m_pEPObj;
		}
	}

	// TODO: Double check this
	template <class TOther>
	epref(const TOther &otherObj) {
		if (m_pEPObj != otherObj) {
			m_pEPObj = (TEPObj*)(otherObj);
		}
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

	epref<TEPObj> &operator=(const TEPObj* pEPObj) const {
		if(m_pEPObj != pEPObj)
			m_pEPObj = pEPObj;

		// 

		return *this;
	}

	// TODO: Double check this when we actually flesh out the ref count lib
	template <class TOther>
	epref<TEPObj>& operator=(const TOther &otherObj) const {
		if (m_pEPObj != otherObj) {
			m_pEPObj = (TEPObj*)(otherObj);
		}

		return *this;
	}

private:
	unsigned int m_refCount = 0;
	TEPObj* m_pEPObj = nullptr;
};

#endif // EP_REF_H_