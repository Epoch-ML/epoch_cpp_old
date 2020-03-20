#ifndef EP_PROCESS_H_
#define EP_PROCESS_H_

#include "core/ehm/ehm.h"

// epoch process
// epoch/src/core/types/EPProcess.h

// This represents a process (vs a thread, task etc)

#include "core/types/EPObject.h"

class EPProcess : public EPObject {
public:
	EPProcess() {
		// 
	}

	~EPProcess() {
		// 
	}
};

#endif // EP_PROCESS_H_