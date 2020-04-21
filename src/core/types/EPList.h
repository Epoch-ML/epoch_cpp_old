#ifndef EP_LIST_H_
#define EP_LIST_H_

#include "core/ehm/ehm.h"

// epoch list class
// epoch/src/core/types/EPList.h

template <typename TValue>
class EPList {
public:
	class node {
	public:
		node(const TValue& value, EPList::node *pNext = nullptr) :
			m_value(value),
			m_pNext(nullptr)
		{}

		~node() = default;

		node(const EPList::node &rhs) :
			m_value(rhs.m_value),
			m_pNext(rhs.m_pNext)
		{}

		node& operator=(const EPList::node& rhs) {
			m_value = rhs.m_value;
			m_pNext = rhs.m_pNext;
			return *this;
		}

		node(EPList::node&& rhs) :
			m_pNext(rhs.m_pNext),
			m_value(rhs.m_value)
		{
			rhs.m_pNext = nullptr;
		}

		node& operator=(EPList::node&& rhs) {
			m_value = rhs.m_value;
			m_pNext = rhs.m_pNext;

			rhs.m_pNext = nullptr;

			return *this;
		}

	public:
		TValue m_value;
		EPList::node* m_pNext = nullptr;
	};

	class iterator {
	public:
		iterator(EPList::node* pNode) noexcept : m_pNode(pNode) {}
		TValue& operator*() { return (m_pNode->m_value); }
		//TValue* operator->() { return m_pNode; }

		bool operator==(const iterator& rhs) { return m_pNode->m_value == rhs.m_pNode->m_value; }
		bool operator!=(const iterator& rhs) { return m_pNode->m_value != rhs.m_pNode->m_value; }

		iterator& operator++() {
			m_pNode = m_pNode->m_pNext;
			return *this;
		}

		iterator operator++(int) {
			m_pNode = m_pNode->m_pNext;
			return *this;
		}

	protected:
		EPList::node* m_pNode = nullptr;
	};

	class const_iterator : public iterator {
	public:
		const_iterator(EPList::node* pNode) noexcept : m_pNode(pNode) {}
		TValue& operator*() { return (m_pNode->m_value); }
		//TValue* operator->() { return m_pValue; }
		bool operator==(const iterator& rhs) { return m_pNode->m_value == rhs.m_pNode->m_value; }
		bool operator!=(const iterator& rhs) { return m_pNode->m_value != rhs.m_pNode->m_value; }
	};

	iterator begin() {
		return iterator(m_pRootNode);
	}

	iterator end() {
		node* pEnd = m_pRootNode;

		while (pEnd->m_pNext != nullptr)
			pEnd = pEnd->m_pNext;

		return iterator(pEnd);
	}

public:
	EPList() :
		m_pRootNode(nullptr)
	{}

	~EPList() {
		clear();
	}

	inline void clear() {
		while (m_pRootNode != nullptr) {
			node* pTemp = m_pRootNode;
			m_pRootNode = m_pRootNode->m_pNext;

			if (pTemp != nullptr) {
				delete pTemp;
				pTemp = nullptr;
			}
			m_listLength--;
		}
	}

	TValue PopFront() {
		TValue retVal;

		node* pTemp = m_pRootNode;
		m_pRootNode = m_pRootNode->m_pNext;

		retVal = pTemp->m_value;

		if (pTemp != nullptr) {
			delete pTemp;
			pTemp = nullptr;
		}
		m_listLength--;

		return retVal;
	}

	RESULT PushFront(const TValue& value) {
		RESULT r = R::OK;

		node* pTemp = m_pRootNode;

		m_pRootNode = new EPList::node(value, pTemp);
		CNM(m_pRootNode, "Failed to allocate node");

		m_listLength++;

	Error:
		return r;
	}

	TValue PopBack() {
		TValue retVal;

		node* pTemp = m_pRootNode;

		if (pTemp->m_pNext == nullptr) {
			retVal = pTemp->m_value;
			delete pTemp;
			pTemp = nullptr;
			m_pRootNode = nullptr;
		}
		else {
			while (pTemp->m_pNext != nullptr) {
				if (pTemp->m_pNext != nullptr && pTemp->m_pNext->m_pNext == nullptr)
					break;
				pTemp = pTemp->m_pNext;
			}

			retVal = pTemp->m_pNext->m_value;

			delete pTemp->m_pNext;
			pTemp->m_pNext = nullptr;
		}

		m_listLength--;

		return retVal;
	}

	RESULT PushBack(const TValue& value) {
		RESULT r = R::OK;

		if (m_pRootNode == nullptr) {
			m_pRootNode = new EPList::node(value, nullptr);
			CNM(m_pRootNode, "Failed to allocate EPList node");
		}
		else {
			node* pTemp = m_pRootNode;
			while (pTemp->m_pNext != nullptr) {
				pTemp = pTemp->m_pNext;
			}
			pTemp->m_pNext = new EPList::node(value, nullptr);
			CNM(pTemp->m_pNext, "Failed to allocate EPList node");
		}

		m_listLength++;

	Error:
		return r;
	}

	const size_t size() const {
		return m_listLength;
	}

	const TValue& operator[](size_t index) const {
		node* pTemp = m_pRootNode;
		
		for (int i = 0; i < index; i++)  
			pTemp = pTemp->m_pNext;

		return pTemp->m_value;
	}

	TValue& operator [](size_t index) {
		node* pTemp = m_pRootNode;

		for (int i = 0; i < index; i++)
			pTemp = pTemp->m_pNext;

		return pTemp->m_value;
	}

private:
	EPList::node* m_pRootNode = nullptr;
	size_t m_listLength = 0;
};

#endif // ! EP_LIST_H_