#ifndef SANDBOX_PROCESS_H_
#define SANDBOX_PROCESS_H_

#include "core/ehm/ehm.h"

// epoch Sandbox Process
// epoch/src/sandbox/SandboxProcess.h

// Sandbox Process wraps the underlying OS process and provides the ability to create 
// a window or run an event loop / pump etc

#include "core/types/EPProcess.h"

class SandboxProcess : public EPProcess {
public:
	SandboxProcess();
	~SandboxProcess();

private:

};

#endif // ! SANDBOX_PROCESS_H_