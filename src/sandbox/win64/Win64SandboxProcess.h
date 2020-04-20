#ifndef WIN64_SANDBOX_H_
#define WIN64_SANDBOX_H_

#include "core/ehm/ehm.h"

// epoch Windows 64 Sandbox
// epoch/src/sandbox/win64/Win64Sandbox.h

#include "sandbox/SandboxProcess.h"

#include <windows.h>

#include "core/math/rectangle.h"

class Win64SandboxProcess : 
	public SandboxProcess 
{

public:
	Win64SandboxProcess();

protected:
	virtual ~Win64SandboxProcess() override;

public:
	virtual RESULT Initialize() override;
	virtual RESULT Run() override;
	virtual RESULT Kill() override;

	virtual SandboxProcess::type GetType() override {
		return SandboxProcess::type::window;
	}

public:
	RESULT SetDimensions(int pxWidth, int pxHeight);

private:
	LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK StaticWndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HDC m_hDeviceContext = nullptr;

	int m_pxWidth;
	int m_pxHeight;

	rectangle<int> m_dimensions;
};


#endif // ! WIN64_SANDBOX_H_