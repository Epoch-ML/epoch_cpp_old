#ifndef SINGLETON_H_
#define SINGLETON_H_

#include "core/ehm/result.h"

// epoch core Types
// epoch/core/types/singleton.h
// The Singleton Type

template <typename TSingleton>
class singleton {
public:
	// This must be initialized by the singleton inherit
	static TSingleton *s_pInstance;

public:
	static TSingleton* instance() {
		// TODO: unused per the EHM fail that's going on in this file right now
		//RESULT r = R::OK;

		if (s_pInstance == nullptr) {
			s_pInstance = new TSingleton();
			//CN(s_pInstance);
			if (s_pInstance == nullptr) {
				goto Error;
			}

			// Optional Initialization
			//CR(s_pInstance->InitializeSingleton());
			if (RFAILED(s_pInstance->InitializeSingleton())) {
				goto Error;
			}
		}

	Success:
		return s_pInstance;

	Error:
		if (s_pInstance != nullptr) {
			delete s_pInstance;
			s_pInstance = nullptr;
		}

		return nullptr;
	}

protected:
	virtual RESULT InitializeSingleton() {
		return R::NOT_IMPLEMENTED;
	}
};

// Initialize the singleton
template<typename TSingleton>
TSingleton* singleton<TSingleton>::s_pInstance = nullptr;

#endif // ! SINGLETON_H_