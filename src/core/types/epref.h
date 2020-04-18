#ifndef EP_REF_H_
#define EP_REF_H_

#include "core/ehm/result.h"

// epoch reference
// epoch/src/core/types/epref.h

// A simple referenced count object

#include "core/types/EPObj.h"

#include <type_traits>

//#ifndef NOCONCEPTS
//	template <typename TEPObj> requires EPObjectDerived<TEPObj>
//#else
//	template <typename TEPObj>
//#endif

template <typename TEPObj>
class EPRef {

public:
	class ref_counter {
	public:
		ref_counter() : m_count(0) {}
		ref_counter(unsigned int val) : m_count(val) {}

		~ref_counter() = default;

		ref_counter(const ref_counter&) = delete;
		ref_counter& operator=(const ref_counter&) = delete;
		ref_counter(ref_counter&&) = delete;
		ref_counter& operator=(ref_counter&&) = delete;

		void reset() { m_count = 0; }
		unsigned get() { return m_count; }

		void operator++() { m_count++; }
		void operator--() { m_count++; }

	private:
		unsigned int m_count = 0;
	};

public:
	explicit EPRef(TEPObj* pObject = nullptr) :
		m_pEPObj(pObject),
		m_pRefCounter(new ref_counter())
	{
		if (pObject != nullptr) {
			m_pRefCounter->operator++();
		}
	}

	EPRef(const EPRef& pEPObj) {
		if (m_pEPObj != pEPObj.m_pEPObj) {
			m_pEPObj = pEPObj.m_pEPObj;
		}
	}

	EPRef(std::nullptr_t) noexcept :
		m_pEPObj(nullptr),
		m_pRefCounter(nullptr)
	{
		//
	}

	// TODO: Double check this
	template <class TOther>
	explicit EPRef(const TOther &otherObj) {
		if (m_pEPObj != otherObj) {
			m_pEPObj = (TEPObj*)(otherObj);
			m_pRefCounter = new ref_counter(1);
		}
	}

	~EPRef() {
		DecrementCount();
	}

	inline RESULT DecrementCount() {
		RESULT r = R::OK;

		CN(m_pRefCounter)

		m_pRefCounter->operator--();

		if (m_pRefCounter->get() == 0) {
			delete m_pRefCounter;

			if (m_pEPObj != nullptr) {
				delete m_pEPObj;
			}
		}

		m_pEPObj = nullptr;
		m_pRefCounter = nullptr;

	Error:
		return r;
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

	bool operator==(const EPRef<TEPObj> &pEPObj) const {
		return m_pEPObj == pEPObj.m_pEPObj;
	}


	bool operator!=(const EPRef<TEPObj>& pEPObj) const {
		return m_pEPObj != pEPObj.m_pEPObj;
	}

	EPRef<TEPObj> &operator=(const TEPObj* pEPObj) const {
		if (m_pEPObj != pEPObj) {
			m_pEPObj = pEPObj;
		}

		return *this;
	}

	template <class TOther>
	EPRef<TEPObj>& operator=(const TOther &otherObj) const {
		if (m_pEPObj != otherObj) {
			m_pEPObj = (TEPObj*)(otherObj);
		}

		return *this;
	}

	EPRef& operator=(std::nullptr_t) noexcept {
		if (m_pEPObj != nullptr) {
			DecrementCount();
		}

		return *this;
	}

private:
	ref_counter* m_pRefCounter = nullptr;
	TEPObj* m_pEPObj = nullptr;
};

#endif // EP_REF_H_