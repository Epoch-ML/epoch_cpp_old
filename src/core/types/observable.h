#ifndef OBSERVABLE_H_
#define OBSERVABLE_H_

#include "core/ehm/result.h"

// epoch observable
// epoch/src/core/types/observable.h

// Implements the observer pattern in a generic way

#include "EPVector.h"
#include "EPRef.h"

template<typename CObserver>
class observable 
{
public:
	observable() = default;
	~observable() = default;

public:
	RESULT RegisterObserver(EPRef<CObserver> pObserver) {
		RESULT r = R::OK;

		CB(m_observers.exists(pObserver) == false);
		m_observers.PushBack(pObserver);

	Error:
		return r;
	}

protected:
	EPVector<EPRef<CObserver>> m_observers;
};

#endif // ! OBSERVABLE_H_