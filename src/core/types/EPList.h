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
		node(const TValue& value, EPList::node *pNext = nullptr, EPList::node* pPrev = nullptr) :
			m_value(value),
			m_pNext(pNext),
			m_pPrev(pPrev)
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
		EPList::node* m_pPrev = nullptr;
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

		iterator& operator--() {
			m_pNode = m_pNode->m_pPrev;
			return *this;
		}

		iterator operator--(int) {
			m_pNode = m_pNode->m_pPrev;
			return *this;
		}

	protected:
		EPList::node* m_pNode = nullptr;
	};

	class const_iterator : public iterator {
	public:
		const_iterator(EPList::node* pNode) noexcept : iterator(pNode) {}
		TValue& operator*() { return (this->m_pNode->m_value); }
		//TValue* operator->() { return m_pValue; }
		bool operator==(const iterator& rhs) { return this->m_pNode->m_value == rhs.m_pNode->m_value; }
		bool operator!=(const iterator& rhs) { return this->m_pNode->m_value != rhs.m_pNode->m_value; }
	};

	iterator begin() {
		return iterator(m_pRootNode);
	}

	iterator end() {
		return iterator(m_pLastNode);
	}

public:
	EPList() :
		m_pRootNode(nullptr),
		m_pLastNode(nullptr),
		m_pCurrentNode(nullptr),
		m_currentNode_idx(0),
		m_listLength(0)
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

		m_pLastNode = nullptr;
		m_pCurrentNode = nullptr;
		m_currentNode_idx = 0;
	}

	// Doesn't affect last node
	TValue PopFront() {
		TValue retVal;

		node* pTemp = m_pRootNode;
		m_pRootNode = m_pRootNode->m_pNext;
		
		if(m_pRootNode != nullptr)
			m_pRootNode->m_pPrev = nullptr;

		retVal = pTemp->m_value;

		if (m_pCurrentNode == pTemp) {
			m_pCurrentNode = m_pRootNode;
			m_currentNode_idx = 0;
		}
		else {
			m_currentNode_idx--;
		}

		m_listLength--;

		if (pTemp != nullptr) {
			delete pTemp;
			pTemp = nullptr;
		}


		return retVal;
	}

	RESULT PushFront(const TValue& value) {
		RESULT r = R::OK;

		node* pTemp = m_pRootNode;

		m_pRootNode = new EPList::node(value, pTemp, nullptr);
		CNM(m_pRootNode, "Failed to allocate node");

		if (pTemp == nullptr) {
			m_pLastNode = m_pRootNode;
		}
		else {
			pTemp->m_pPrev = m_pRootNode;
		}


		m_listLength++;
		m_currentNode_idx++;

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
			m_pLastNode = nullptr;
			m_pCurrentNode = nullptr;
			m_currentNode_idx = 0;
		}
		else {
			pTemp = m_pLastNode->m_pPrev;

			// Reset current node if it's the last one
			if (m_pCurrentNode == pTemp->m_pNext) {
				m_pCurrentNode = m_pRootNode;
				m_currentNode_idx = 0;
			}

			retVal = pTemp->m_pNext->m_value;

			delete pTemp->m_pNext;
			pTemp->m_pNext = nullptr;

			// Update the last node
			m_pLastNode = pTemp;
		}

		m_listLength--;

		return retVal;
	}

	// This won't affect the current node
	RESULT PushBack(const TValue& value) {
		RESULT r = R::OK;

		if (m_pRootNode == nullptr) {
			m_pRootNode = new EPList::node(value, nullptr, nullptr);
			CNM(m_pRootNode, "Failed to allocate EPList node");

			m_pLastNode = m_pRootNode;
		}
		else {
			m_pLastNode->m_pNext = new EPList::node(value, nullptr, m_pLastNode);
			CNM(m_pLastNode->m_pNext, "Failed to allocate EPList node");

			// Update the last node
			m_pLastNode = m_pLastNode->m_pNext;
		}

		m_listLength++;

	Error:
		return r;
	}

	size_t size() const {
		return m_listLength;
	}

	const TValue& operator[](size_t index) const {
		if (index == 0)
			return m_pRootNode->m_value;
		else if(index == m_listLength - 1)
			return m_pLastNode->m_value;

		// Figure out if cursor is closer or start of list is closer
		if (index - m_currentNode_idx >= 0 && index - m_currentNode_idx >= index) {
			m_currentNode_idx = 0;
			m_pCurrentNode = m_pRootNode;
		}

		// Using i avoid cache bs
		for (; m_currentNode_idx < index; m_currentNode_idx++) {
			m_pCurrentNode = m_pCurrentNode->m_pNext;
		}

		return m_pCurrentNode->m_value;
	}

	TValue& operator [](size_t index) {
		// Faster for pop front/back operations
		if (index == 0)
			return m_pRootNode->m_value;
		else if (index == m_listLength - 1)
			return m_pLastNode->m_value;

		// Figure out if cursor is closer or start of list is closer
		if (index - m_currentNode_idx >= 0 && index - m_currentNode_idx >= index) {
			m_currentNode_idx = 0;
			m_pCurrentNode = m_pRootNode;
		}

		// Using i avoid cache bs
		for (; m_currentNode_idx < index; m_currentNode_idx++) {
			m_pCurrentNode = m_pCurrentNode->m_pNext;
		}

		return m_pCurrentNode->m_value;
	}

private:
	EPList::node* m_pRootNode = nullptr;

	// Keep track of the last node so we don't need to iterate to it every time
	EPList::node* m_pLastNode = nullptr;
	
	// Use as a way to not have to go through the entire list every single time
	// we use the bracket 
	EPList::node* m_pCurrentNode = nullptr;
	size_t m_currentNode_idx = 0;

	size_t m_listLength = 0;
};

#endif // ! EP_LIST_H_