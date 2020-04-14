#ifndef WIN64_SANDBOX_H_
#define WIN64_SANDBOX_H_

#include "core/ehm/ehm.h"

// epoch Windows 64 Sandbox
// epoch/src/sandbox/win64/Win64Sandbox.h

#include "sandbox/SandboxProcess.h"

class Win64SandboxProcess : 
	public SandboxProcess 
{

protected:
	Win64SandboxProcess();
	~Win64SandboxProcess();

public:
	virtual SandboxProcess::type GetType() override {
		return SandboxProcess::type::OS_WINDOW;
	}

private:
	// empty
};


#endif // ! WIN64_SANDBOX_H_