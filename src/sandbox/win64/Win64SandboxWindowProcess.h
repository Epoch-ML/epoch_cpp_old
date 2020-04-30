#ifndef WIN64_SANDBOX_WINDOW_PROCESS_H_
#define WIN64_SANDBOX_WINDOW_PROCESS_H_

#include "core/ehm/ehm.h"

// epoch Windows 64 Sandbox
// epoch/src/sandbox/win64/Win64Sandbox.h

#include "sandbox/SandboxWindowProcess.h"

#include <windows.h>

#include "core/math/math.types.h"

class Win64SandboxWindowProcess : 
	public SandboxWindowProcess
{

public:
	Win64SandboxWindowProcess();

protected:
	virtual ~Win64SandboxWindowProcess() override;

public:
	virtual RESULT Initialize() override;
	virtual RESULT Process() override;

	virtual RESULT Show() override;
	virtual RESULT Hide() override;

	rectangle<int> GetScreenDimensions();

	virtual intptr_t GetNativeWindowHandle() const override {
		return reinterpret_cast<intptr_t>(m_hWindowHandle);
	}

private:
	LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK StaticWndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);

	RESULT HandleWin64Messages();

private:
	WNDCLASSEX m_windowsClassExt;
	HINSTANCE m_hInstance = nullptr;
	HDC m_hDeviceContext = nullptr;
	HWND m_hWindowHandle = nullptr;
	DWORD m_dwWindowStyle = 0;

	EPString<char> m_strHardwareID;
};


#endif // ! WIN64_SANDBOX_WINDOW_PROCESS_H_