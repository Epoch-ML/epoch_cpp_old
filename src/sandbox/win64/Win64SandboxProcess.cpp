#include "Win64SandboxProcess.h"

Win64SandboxProcess::Win64SandboxProcess() {
    // empty
}

Win64SandboxProcess::~Win64SandboxProcess() {
    // empty
}

RESULT Win64SandboxProcess::Initialize() {
    RESULT r = R::OK;

	m_windowsClassExt = { 0 };
	m_windowsClassExt.cbSize = sizeof(WNDCLASSEX);
	m_windowsClassExt.style = CS_HREDRAW | CS_VREDRAW;
	m_windowsClassExt.lpfnWndProc = (WNDPROC)(Win64SandboxProcess::StaticWndProc);
	m_windowsClassExt.hInstance = nullptr;
	m_windowsClassExt.lpszClassName = "Win64 Sandbox Process";
	m_windowsClassExt.hCursor = LoadCursor(nullptr, IDC_ARROW);
	m_windowsClassExt.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	//m_windowsClassExt.cbClsExtra = 0;
	//m_windowsClassExt.cbWndExtra = 0;
	//m_windowsClassExt.hIcon = LoadIcon(m_hInstance, IDI_APPLICATION);
	//m_windowsClassExt.lpszMenuName = nullptr;
	//m_windowsClassExt.hIconSm = LoadIcon(m_hInstance, IDI_APPLICATION);

	if (RegisterClassEx(&m_windowsClassExt) == 0) {
		MessageBox(nullptr, "Failed to register win64 window class", "Epoch Sandbox Error", NULL);
		return R::FAIL;
	}

	m_dwWindowStyle = WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	if (m_fFullscreen) {
		m_ptPosition = { 0, 0 };
		m_dwWindowStyle |= WS_POPUP;
		m_rectDimensions = GetScreenDimensions();
	}
	else {
		auto screenDims = GetScreenDimensions();
		m_ptPosition = {
			(screenDims.width() - m_rectDimensions.width()) / 2,
			(screenDims.height() - m_rectDimensions.height()) / 2,
		};
	}

	m_hWindowHandle = CreateWindow(
		"Win64 Sandbox Process",						// lpClassName
		nullptr,										// lpWindowName
		m_dwWindowStyle,								// dwStyle
		m_ptPosition.x(),							// x coordinate 
		m_ptPosition.y(),							// y coordinate
		m_rectDimensions.width(),					// width of window 
		m_rectDimensions.height(), 					// height of the window
		nullptr,										// // hWndParent
		nullptr,										// hMenu
		nullptr,										// hInstance
		this											// lpParam
	);

	// Get hardware ID from profile
	HW_PROFILE_INFO hwProfInfo;
	GetCurrentHwProfile(&hwProfInfo);
	m_strHardwareID = hwProfInfo.szHwProfileGuid;

	// At this point WM_CREATE message is sent/received and rx-ed by WndProc
	
	// High priority is where things like Task List reside, ignoring load on the operating system.
	// Use extreme care when using the high-priority class, because a high-priority class application can use nearly all available CPU time.
	SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);	
																		
	// TODO: Prob good to get a hInstance

	// TODO: This should go into a show window call
	ShowWindow(m_hWindowHandle, SW_RESTORE);

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

rectangle<int> Win64SandboxProcess::GetScreenDimensions() {
	return rectangle<int> {
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN)
	};
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