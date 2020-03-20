#ifndef DIRTY_H_
#define DIRTY_H_

#include "core/ehm/result.h"

// epoch types: dirty
// epoch/src/core/types/dirty.h

// Simple dirty flag pattern

class dirty {
private:
	bool m_fDirty;

public:
	dirty(bool fStartState = false) :
		m_fDirty(fStartState)
	{
		/* empty stub */
	}

	~dirty() {
		/* empty stub */
	}

public:
	// Mark the object as dirty, data should be updated by the renderer
	virtual RESULT SetDirty() {
		
		m_fDirty = true;
		
		return R::PASS;
	}

	RESULT CleanDirty() {
		
		m_fDirty = false;

		return R::PASS;
	}

	// Check if dirty, and clean the dirty state
	bool CheckAndCleanDirty() {
		bool fDirty = m_fDirty;
		m_fDirty = false;

		return fDirty;
	}

	bool IsDirty() {
		return m_fDirty;
	}
};

#endif // ! DIRTY_H_
