#ifndef WIN64_SANDBOX_H_
#define WIN64_SANDBOX_H_

#include "core/ehm/ehm.h"

// epoch Windows 64 Sandbox
// epoch/src/sandbox/win64/Win64Sandbox.h

#include "sandbox/sandbox.h"

class Win64Sandbox : 
	public sandbox 
{

protected:
	Win64Sandbox();
	~Win64Sandbox();

private:
	// empty
};


#endif // ! WIN64_SANDBOX_H_