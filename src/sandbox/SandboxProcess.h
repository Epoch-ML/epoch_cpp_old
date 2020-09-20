#ifndef SANDBOX_PROCESS_H_
#define SANDBOX_PROCESS_H_

#include "core/ehm/ehm.h"

// epoch Sandbox Process
// epoch/src/sandbox/SandboxProcess.h

// Sandbox Process wraps the underlying OS process and provides the ability to create 
// a window or run an event loop / pump etc

#include "core/types/EPProcess.h"
#include "core/types/EPString.h"

#include <thread>

class SandboxProcess : 
	public EPProcess 
{
public:
	enum class type : uint32_t {
		window,
		console,
		custom
	};
	static const char* kSandboxProcessTypes[];

public:
	SandboxProcess();
	virtual ~SandboxProcess() override;

	virtual SandboxProcess::type GetType() = 0;
	virtual RESULT Initialize() = 0;

	// Process control
	RESULT Run() {
		RESULT r = R::OK;
		
		m_fRunning = true;
		m_threadSandboxProcess = std::thread(&SandboxProcess::Process, this);

	Error:
		return r;
	}

	RESULT Kill() {
		RESULT r = R::OK;

		m_fRunning = false;
		m_threadSandboxProcess.join();

	Error:
		return r;
	}

	virtual RESULT Process() = 0;

	const EPString<char>& GetName() const {
		return m_strProcessName;
	}

	RESULT SetName(const EPString<char>& strProcessName) {
		m_strProcessName = strProcessName;
		return R::OK;
	}

	static const char* GetProcessTypeName(SandboxProcess::type procType) {
		return kSandboxProcessTypes[static_cast<uint8_t>(procType)];
	}

	bool IsRunning() { return m_fRunning; }
	bool IsShuttingDown() { return m_fPendingShutdown; }
	RESULT QueueShutdown() { m_fPendingShutdown = true; return R::OK; }

private:
	EPString<char> m_strProcessName;
	std::thread m_threadSandboxProcess;

	bool m_fRunning = false;
	bool m_fPendingShutdown = false;
};

#endif // ! SANDBOX_PROCESS_H_