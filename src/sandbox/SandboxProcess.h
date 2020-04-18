#ifndef SANDBOX_PROCESS_H_
#define SANDBOX_PROCESS_H_

#include "core/ehm/ehm.h"

// epoch Sandbox Process
// epoch/src/sandbox/SandboxProcess.h

// Sandbox Process wraps the underlying OS process and provides the ability to create 
// a window or run an event loop / pump etc

#include "core/types/EPProcess.h"
#include "core/types/EPString.h"

class SandboxProcess : 
	public EPProcess 
{
public:
	typedef enum class type : uint32_t {
		window,
		console,
		custom
	} SandboxProcessType;

public:
	SandboxProcess();
	~SandboxProcess();

	virtual SandboxProcess::type GetType() = 0;

	// Process control
	virtual RESULT Run() = 0;
	virtual RESULT Kill() = 0;

	const EPString<char>& GetName() {
		return m_strProcessName;
	}

private:
	EPString<char> m_strProcessName;
};

#endif // ! SANDBOX_PROCESS_H_