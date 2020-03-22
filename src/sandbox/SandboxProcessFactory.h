#ifndef SANDBOX_PROCESS_FACTORY_H_
#define SANDBOX_PROCESS_FACTORY_H_

#include "core/ehm/ehm.h"

// epoch sandbox process factory
// epoch/src/sandbox/SandboxProcessFactory.h

# include "core/types/EPFactory.h"

#include "sandbox/SandboxProcess.h"

class SandboxProcessFactory : public EPFactory<SandboxProcess> {
public:
	SandboxProcessFactory() {
		//
	}

	~SandboxProcessFactory() {
		//
	}

private:
	static epref<SandboxProcess> make() override;
};

#endif // ! SANDBOX_PROCESS_FACTORY_H_