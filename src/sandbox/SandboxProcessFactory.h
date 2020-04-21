#ifndef SANDBOX_PROCESS_FACTORY_H_
#define SANDBOX_PROCESS_FACTORY_H_

#include "core/ehm/ehm.h"

// epoch sandbox process factory
// epoch/src/sandbox/SandboxProcessFactory.h

# include "core/types/EPFactory.h"

#include "sandbox/sandbox.h"
#include "sandbox/SandboxProcess.h"

class SandboxProcessFactory : 
	public EPFactory<SandboxProcessFactory, SandboxProcess, EPString<char>, SandboxProcess::type, sandbox::platform> 
{
	friend EPFactory;

public:
	SandboxProcessFactory() {
		//
	}

	~SandboxProcessFactory() {
		//
	}

protected:
	static EPRef<SandboxProcess> InternalMake(const EPString<char>&, SandboxProcess::type, sandbox::platform);

private:
	static EPRef<SandboxProcess> SandboxProcessFactory::InternalMakeWin64(const EPString<char>&, SandboxProcess::type);
};

#endif // ! SANDBOX_PROCESS_FACTORY_H_