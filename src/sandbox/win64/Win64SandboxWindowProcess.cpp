#include "Win64SandboxWindowProcess.h"

//#include "hal/HALFactory.h"

Win64SandboxWindowProcess::Win64SandboxWindowProcess() {
    // empty
}

Win64SandboxWindowProcess::~Win64SandboxWindowProcess() {
    // empty
}

RESULT Win64SandboxWindowProcess::Initialize() {
    RESULT r = R::OK;

	// Grab the instance for good measure
	m_hInstance = GetModuleHandle(NULL);
	CNM(m_hInstance, "Failed to retrieve instance handle");

	m_windowsClassExt = { 0 };
	m_windowsClassExt.cbSize = sizeof(WNDCLASSEX);
	m_windowsClassExt.style = CS_HREDRAW | CS_VREDRAW;
	m_windowsClassExt.lpfnWndProc = (WNDPROC)(Win64SandboxWindowProcess::StaticWndProc);
	m_windowsClassExt.hInstance = m_hInstance;
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

	m_dwWindowStyle = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

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
		nullptr,										// hWndParent
		nullptr,										// hMenu
		m_hInstance,									// hInstance
		this											// lpParam
	);

	CNM(m_hWindowHandle, "Failed to create window");

	// Get hardware ID from profile
	HW_PROFILE_INFO hwProfInfo;
	GetCurrentHwProfile(&hwProfInfo);
	m_strHardwareID = hwProfInfo.szHwProfileGuid;

	// At this point WM_CREATE message is sent/received and rx-ed by WndProc
	
	// High priority is where things like Task List reside, ignoring load on the operating system.
	// Use extreme care when using the high-priority class, because a high-priority class application can use nearly all available CPU time.
	SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);	

Error:
    return r;
}

// This will actually run the code
RESULT Win64SandboxWindowProcess::Process() {
	RESULT r = R::OK;

	//EPRef<HAL> pVulkanHAL = nullptr;
	//HAL::type halType = HAL::type::vulkan;

	CRM(Initialize(), "Failed to initialize win64 window");

	//pVulkanHAL = HALFactory::make(halType, EPRef<SandboxWindowProcess>(this));
	//CNM(pVulkanHAL, "Failed to create VulkanHAL");

	CRM(Show(), "Failed to show win64 window");

	while (IsRunning()) {
		CRM(HandleWin64Messages(), "Failed to handle win64 messages");

		//CRM(pVulkanHAL->Render(), "Failed to render frame");

		///Sleep(1000);

		// TODO: Swap buffers
		// CBM(SwapBuffers(m_hDeviceContext), "Failed to swap buiffers");

		//DreamConsole::GetConsole()->OnFrameRendered();
#ifdef _DEBUG
		if (IsShuttingDown() || GetAsyncKeyState(VK_ESCAPE)) {
#else
		if (IsShuttingDown()) {
#endif
			Sleep(1000);
			Kill();
		}
	}

Error:
	Kill();

	return r;
}

RESULT Win64SandboxWindowProcess::Show() {
	RESULT r = R::OK;

	m_fVisible = true;
	bool fPreviousVisible = ShowWindow(m_hWindowHandle, SW_SHOWDEFAULT);

Error:
	return r;
}

RESULT Win64SandboxWindowProcess::Hide() {
	RESULT r = R::OK;

	m_fVisible = false;
	bool fPreviousVisible = ShowWindow(m_hWindowHandle, SW_HIDE);

Error:
	return r;
}

rectangle<int> Win64SandboxWindowProcess::GetScreenDimensions() {
	return rectangle<int> {
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN)
	};
}

LRESULT CALLBACK Win64SandboxWindowProcess::StaticWndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam) {
	Win64SandboxWindowProcess* pWin64SandboxProcess = nullptr;

	if (msg == WM_CREATE) {
		// Save our object to USERDATA
		pWin64SandboxProcess = (Win64SandboxWindowProcess*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		SetWindowLongPtr(hWindow, GWLP_USERDATA, (LONG_PTR)(pWin64SandboxProcess));
	}
	else {
		// Retrieve our object from USERDATA
		pWin64SandboxProcess = (Win64SandboxWindowProcess*)GetWindowLongPtr(hWindow, GWLP_USERDATA);
		if (!pWin64SandboxProcess) {
			return DefWindowProc(hWindow, msg, wParam, lParam);
		}
	}

	return pWin64SandboxProcess->WndProc(hWindow, msg, wParam, lParam);
}

LRESULT CALLBACK Win64SandboxWindowProcess::WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam) {
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
			OnResize(LOWORD(lParam), HIWORD(lParam));
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

RESULT Win64SandboxWindowProcess::HandleWin64Messages() {
	RESULT r = R::OK;

	MSG msg;

	if (PeekMessage(&msg, nullptr, NULL, NULL, PM_REMOVE)) {
		if (msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST) {
			// Handle mouse event
		}
		else if (msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST) {
			// Handle key event
		}
		else if (WM_QUIT == msg.message) {
			CRM(Kill(), "Failed to kill win64 window process: %d", (int)(RESULT)(msg.wParam));
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

Error:
	return r;
}