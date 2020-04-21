#ifndef WIN64_SANDBOX_H_
#define WIN64_SANDBOX_H_

#include "core/ehm/ehm.h"

// epoch Windows 64 Sandbox
// epoch/src/sandbox/win64/Win64Sandbox.h

#include "sandbox/SandboxWindowProcess.h"

#include <windows.h>

#include "core/math/math.types.h"

class Win64SandboxProcess : 
	public SandboxWindowProcess
{

public:
	Win64SandboxProcess();

protected:
	virtual ~Win64SandboxProcess() override;

public:
	virtual RESULT Initialize() override;
	virtual RESULT Run() override;
	virtual RESULT Kill() override;

	rectangle<int> GetScreenDimensions();

private:
	LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK StaticWndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	WNDCLASSEX m_windowsClassExt;
	HINSTANCE m_hInstance = nullptr;
	HDC m_hDeviceContext = nullptr;
	HWND m_hWindowHandle = nullptr;
	DWORD m_dwWindowStyle = 0;

	EPString<char> m_strHardwareID;
};


#endif // ! WIN64_SANDBOX_H_