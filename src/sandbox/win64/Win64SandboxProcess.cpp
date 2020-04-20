#include "Win64SandboxProcess.h"

Win64SandboxProcess::Win64SandboxProcess() {
    // empty
}

Win64SandboxProcess::~Win64SandboxProcess() {
    // empty
}

RESULT Win64SandboxProcess::Initialize() {
    RESULT r = R::OK;

	// 

Error:
    return r;
}

RESULT Win64SandboxProcess::Run() {
	RESULT r = R::NOT_IMPLEMENTED;

Error:
	return r;
}

RESULT Win64SandboxProcess::Kill() {
	RESULT r = R::NOT_IMPLEMENTED;

Error:
	return r;
}

LRESULT CALLBACK Win64SandboxProcess::StaticWndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam) {
	Win64SandboxProcess* pWin64SandboxProcess = nullptr;

	if (msg == WM_CREATE) {
		// Save our object to USERDATA
		pWin64SandboxProcess = (Win64SandboxProcess*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		SetWindowLongPtr(hWindow, GWLP_USERDATA, (LONG_PTR)(pWin64SandboxProcess));
	}
	else {
		// Retrieve our object from USERDATA
		pWin64SandboxProcess = (Win64SandboxProcess*)GetWindowLongPtr(hWindow, GWLP_USERDATA);
		if (!pWin64SandboxProcess) {
			return DefWindowProc(hWindow, msg, wParam, lParam);
		}
	}

	return pWin64SandboxProcess->WndProc(hWindow, msg, wParam, lParam);
}

LRESULT CALLBACK Win64SandboxProcess::WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam) {
	RESULT r = R::OK;

	switch (msg) {
		case WM_CREATE: {
			HDC hDeviceContext = GetDC(hWindow);

			if (hDeviceContext == nullptr) {
				DEBUG_LINEOUT("Failed to capture Device Context");
				//PostQuitMessage(0);
				Kill();
				return 0L;
			}

			m_hDeviceContext = hDeviceContext;
		} break;

		case WM_DESTROY: {
			DEBUG_LINEOUT("Windows Sandbox being destroyed");
			//PostQuitMessage(0);
			Kill();
			return 0L;
		} break;

		case WM_SIZE: {
			SetDimensions(LOWORD(lParam), HIWORD(lParam));
		} break;

		case WM_COPYDATA: {
			// TODO: Leving this here since it's a good thing to remember to use if needed
			//PCOPYDATASTRUCT pDataStruct = (PCOPYDATASTRUCT)lParam;

		
		} break;

		default: {
			// Empty stub
		} break;
	}

Error:
	// Fall through for all messages for now
	return DefWindowProc(hWindow, msg, wParam, lParam);
}

RESULT Win64SandboxProcess::SetDimensions(int pxWidth, int pxHeight) {
	RESULT r = R::OK;

	m_dimensions = { pxWidth, pxHeight };

Error:
	return r;
}