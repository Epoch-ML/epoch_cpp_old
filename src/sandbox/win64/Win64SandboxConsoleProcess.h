#ifndef WIN64_SANDBOX_CONSOLE_PROCESS_H_
#define WIN64_SANDBOX_CONSOLE_PROCESS_H_

#include "core/ehm/ehm.h"

// epoch Windows 64 Console Process 
// epoch/src/sandbox/win64/Win64SandboxConsoleProcess.h

#include "sandbox/SandboxConsoleProcess.h"

#include <windows.h>

#include "core/math/math.types.h"

class Win64SandboxConsoleProcess :
	public SandboxConsoleProcess
{

public:
	Win64SandboxConsoleProcess();

protected:
	virtual ~Win64SandboxConsoleProcess() override;

public:
	virtual RESULT Initialize() override;
	virtual RESULT Process() override;

	virtual RESULT Show() override;
	virtual RESULT Hide() override;

	rectangle<int> GetScreenDimensions();

private:
	int m_hConnection = NULL;
	HANDLE m_hStandardOutput = nullptr;
	CONSOLE_SCREEN_BUFFER_INFO m_ConsoleScreenBufferInfo = { 0 };
	FILE* m_pFileStream = nullptr;
	HWND m_hConsoleWindowHandle = nullptr;
};


#endif // ! WIN64_SANDBOX_CONSOLE_PROCESS_H_