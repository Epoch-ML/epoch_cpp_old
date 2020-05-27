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
		const unsigned get() const { return m_count; }
		unsigned get() { return m_count; }

		void operator++() { m_count++; }

		void operator--() { m_count--; }

	private:
		unsigned int m_count = 0;
	};

public:
	EPRef(TEPObj* pObject) :
		m_pEPObj(pObject),
		m_pRefCounter(new ref_counter())
	{
		if (m_pEPObj != nullptr) {
			m_pRefCounter->operator++();
		}
	}

	template <typename TOther>
	explicit EPRef(TOther* pObject) :
		m_pEPObj(dynamic_cast<TEPObj*>(pObject)),
		m_pRefCounter(new ref_counter())
	{
		if (m_pEPObj != nullptr) {
			m_pRefCounter->operator++();
		}
	}

	// copy
	explicit EPRef(const EPRef& pEPObj) {
		m_pEPObj = pEPObj.m_pEPObj;
		m_pRefCounter = pEPObj.m_pRefCounter;
		
		if (m_pEPObj != nullptr) {
			m_pRefCounter->operator++();
		}
	}

	explicit EPRef(EPRef& pEPObj) {
		m_pEPObj = pEPObj.m_pEPObj;
		m_pRefCounter = pEPObj.m_pRefCounter;

		if (m_pEPObj != nullptr) {
			m_pRefCounter->operator++();
		}
	}

	// move
	explicit EPRef(EPRef&& pEPObj) {
		m_pEPObj = pEPObj.m_pEPObj;
		m_pRefCounter = pEPObj.m_pRefCounter;

		// We're adding and then deleting a reference 
		// so I guess they cancel each other out
		if(m_pRefCounter != nullptr)
			m_pRefCounter->operator++();

		//pEPObj.DecrementCount();
			
		pEPObj.m_pEPObj = nullptr;
	}

	// Copy assignment
	EPRef<TEPObj>& operator=(const EPRef& pEPObj) {
		if (m_pEPObj != pEPObj.m_pEPObj) {

			m_pEPObj = pEPObj.m_pEPObj;
			m_pRefCounter = pEPObj.m_pRefCounter;
			
			if (m_pEPObj != nullptr) {
				m_pRefCounter->operator++();
			}

		}

		return *this;
	}

	EPRef<TEPObj>& operator=(EPRef& pEPObj) {
		if (m_pEPObj != pEPObj.m_pEPObj) {

			m_pEPObj = pEPObj.m_pEPObj;
			m_pRefCounter = pEPObj.m_pRefCounter;
			
			if (m_pEPObj != nullptr) {
				m_pRefCounter->operator++();
			}
		}

		return *this;
	}

	// move assignment
	
	EPRef<TEPObj>& operator=(EPRef&& pEPObj) {
		if (m_pEPObj != pEPObj.m_pEPObj) {
			m_pEPObj = pEPObj.m_pEPObj;
			m_pRefCounter = pEPObj.m_pRefCounter;

			// This seems needed since the decrement is being called regardless
			if (m_pEPObj != nullptr) {
				m_pRefCounter->operator++();
			}

			//pEPObj.DecrementCount();

			pEPObj.m_pEPObj = nullptr;
		}

		return *this;
	}

	// To other EPRef
	template <class TOther>
	EPRef(const EPRef<TOther>& pEPObj) {
		m_pEPObj = (TEPObj*)(pEPObj.get());
		m_pRefCounter = (ref_counter*)pEPObj.get_ref_counter();
		
		if (m_pEPObj != nullptr) {
			m_pRefCounter->operator++();
		}
	}

	template <class TOther>
	EPRef<TEPObj>& operator=(const EPRef<TOther>& pEPObj) const {
		if ((ptrdiff_t)(m_pEPObj) != (ptrdiff_t)(pEPObj.m_pEPObj)) {

			m_pEPObj = (TEPObj*)(pEPObj.get());
			m_pRefCounter = (ref_counter*)pEPObj.get_ref_counter();
			
			if (m_pEPObj != nullptr) {
				m_pRefCounter->operator++();
			}

		}

		return *this;
	}

	template <class TOther>
	EPRef<TEPObj>& operator=(EPRef<TOther>& pEPObj) {
		if ((ptrdiff_t)(m_pEPObj) != (ptrdiff_t)(pEPObj.m_pEPObj)) {

			m_pEPObj = (TEPObj*)(pEPObj.get());
			m_pRefCounter = (ref_counter*)pEPObj.get_ref_counter();
			
			if (m_pEPObj != nullptr) {
				m_pRefCounter->operator++();
			}

		}

		return *this;
	}

	template <class TOther>
	EPRef<TEPObj>& operator=(EPRef<TOther>&& pEPObj) {
		if ((ptrdiff_t)(m_pEPObj) != (ptrdiff_t)(pEPObj.m_pEPObj)) {
			m_pEPObj = (TEPObj*)(pEPObj.get());
			m_pRefCounter = (ref_counter*)pEPObj.get_ref_counter();

			// This seems needed since the decrement is being called regardless
			if (m_pEPObj != nullptr) {
				m_pRefCounter->operator++();
			}

			//pEPObj.DecrementCount();

			pEPObj.m_pEPObj = nullptr;
		}

		return *this;
	}

	template <class TOther>
	EPRef(EPRef<TOther>&& pEPObj) {
		m_pEPObj = (TEPObj*)(pEPObj.get());
		m_pRefCounter = (ref_counter*)pEPObj.get_ref_counter();

		// We're adding and then deleting a reference 
		// so I guess they cancel each other out
		if (m_pRefCounter != nullptr)
			m_pRefCounter->operator++();

		//pEPObj.DecrementCount();

		pEPObj.m_pEPObj = nullptr;
	}

	template <class TOther>
	EPRef<TEPObj>& operator=(TOther* pOther) {
		if ((ptrdiff_t)(m_pEPObj) != (ptrdiff_t)(pOther)) {

			m_pEPObj = dynamic_cast<TEPObj*>(pOther);
			if (m_pEPObj != nullptr) {
				m_pRefCounter = new ref_counter(1);
			}
			else {
				if (m_pRefCounter != nullptr) {
					delete m_pRefCounter;
					m_pRefCounter = nullptr;
				}
			}
		}

		return *this;
	}

	EPRef(std::nullptr_t) noexcept :
		m_pEPObj(nullptr),
		m_pRefCounter(nullptr)
	{
		//
	}

	/*
	// TODO: Double check these - do we ever really need to handle a TOther object
	// that's not a TEPObj or EPRef<TOther> type scenario?
	template <class TOther>
	explicit EPRef(const TOther &otherObj) {
		if (m_pEPObj != otherObj) {
			m_pEPObj = static_cast<TEPObj*>(otherObj);
			m_pRefCounter = new ref_counter(1);
		}
	}

	template <class TOther>
	explicit EPRef(TOther& otherObj) {
		if (m_pEPObj != otherObj) {
			m_pEPObj = static_cast<TEPObj*>(otherObj);
			m_pRefCounter = new ref_counter(1);
		}
	}

	template <class TOther>
	EPRef<TEPObj>& operator=(const TOther &otherObj) {
		if (m_pEPObj != (TEPObj*)(otherObj)) {

			if (m_pEPObj != nullptr) {
				DecrementCount();
			}

			m_pEPObj = static_cast<TEPObj*>(otherObj);

			m_pRefCounter = new ref_counter();

			if (m_pEPObj != nullptr) {
				m_pRefCounter->operator++();
			}
		}

		return *this;
	}

	template <class TOther>
	EPRef<TEPObj>& operator=(TOther& otherObj) {
		if ((ptrdiff_t)(m_pEPObj) != (ptrdiff_t)(otherObj)) {
			if (m_pEPObj != nullptr) {
				DecrementCount();
			}

			m_pEPObj = (TEPObj*)(otherObj);

			m_pRefCounter = new ref_counter();

			if (m_pEPObj != nullptr) {
				m_pRefCounter->operator++();
			}
		}

		return *this;
	}
	//*/

	~EPRef() {
		DecrementCount();
		m_pEPObj = nullptr;
		m_pRefCounter = nullptr;
	}

	inline RESULT DecrementCount() {
		RESULT r = R::OK;

		CN(m_pRefCounter);

		m_pRefCounter->operator--();

		if (m_pRefCounter->get() == 0) {
			delete m_pRefCounter;

			if (m_pEPObj != nullptr) {
				delete m_pEPObj;
			}

			m_pEPObj = nullptr;
			m_pRefCounter = nullptr;
		}

	Error:
		return r;
	}

	inline RESULT IncrementCount() {
		RESULT r = R::OK;

		CN(m_pRefCounter);
		m_pRefCounter->operator++();

	Error:
		return r;
	}

	ref_counter* get_ref_counter() { return m_pRefCounter; }
	const ref_counter* get_ref_counter() const { return m_pRefCounter; }

	TEPObj* get() { return m_pEPObj; }
	const TEPObj* get() const { return m_pEPObj; }

	TEPObj* operator->() { return m_pEPObj; }
	const TEPObj* operator->() const { return m_pEPObj; }

public:
	bool operator==(const TEPObj* pEPObj) const {
		return m_pEPObj == pEPObj;
	}

	bool operator==(std::nullptr_t) noexcept {
		return (m_pEPObj == nullptr);
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
		if ((ptrdiff_t)(m_pEPObj) != (ptrdiff_t)(pEPObj)) {

			if (m_pEPObj != nullptr) {
				DecrementCount();
			}

			m_pEPObj = pEPObj;

			m_pRefCounter = new ref_counter();

			if (m_pEPObj != nullptr) {
				m_pRefCounter->operator++();
			}
		}

		return *this;
	}

	EPRef& operator=(std::nullptr_t) noexcept {
		if (m_pEPObj != nullptr) {
			DecrementCount();
		}

		m_pEPObj = nullptr;
		m_pRefCounter = nullptr;

		return *this;
	}

protected:
	ref_counter* m_pRefCounter = nullptr;
	TEPObj* m_pEPObj = nullptr;
};

#endif // EP_REF_H_