#include "Win64SandboxConsoleProcess.h"

#include <stdio.h>
#include <io.h>
#include <fcntl.h>

Win64SandboxConsoleProcess::Win64SandboxConsoleProcess() {
	// empty
}

Win64SandboxConsoleProcess::~Win64SandboxConsoleProcess() {
	// empty
}

RESULT Win64SandboxConsoleProcess::Initialize() {
	RESULT r = R::OK;

	// See if we have a console alive and if so lets free it (might need to save the output and transfer)
	CBM(FreeConsole(), "Failed to free console");

	// Allocate a console for this app
	CBM(AllocConsole(), "Failed to allocate a win64 console");

	// This is not portable code but also works for the below
	//CBM(AttachConsole(GetCurrentProcessId()), "Failed to attach to console");
	//freopen("CONOUT$", "wt", stdout);
	//freopen("CONOUT$", "wt", stderr);
	//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

	// Change console buffer info 
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &m_ConsoleScreenBufferInfo);

	m_ConsoleScreenBufferInfo.dwSize.Y = DEFAULT_MAX_CONSOLE_LINES;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), m_ConsoleScreenBufferInfo.dwSize);

	// Redirect unbuffered STDOUT to the console

	m_hStandardOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	m_hConnection = _open_osfhandle((intptr_t)(m_hStandardOutput), _O_TEXT);
	m_pFileStream = _fdopen(m_hConnection, "w");
	*stdout = *m_pFileStream;
	setvbuf(stdout, NULL, _IONBF, 0);

	// Redirect unbuffered STDIN 

	m_hStandardOutput = GetStdHandle(STD_INPUT_HANDLE);
	m_hConnection = _open_osfhandle((intptr_t)(m_hStandardOutput), _O_TEXT);
	m_pFileStream = _fdopen(m_hConnection, "r");
	*stdin = *m_pFileStream;
	setvbuf(stdin, NULL, _IONBF, 0);

	// Redirect unbuffered STDERR 

	m_hStandardOutput = GetStdHandle(STD_ERROR_HANDLE);
	m_hConnection = _open_osfhandle((intptr_t)(m_hStandardOutput), _O_TEXT);
	m_pFileStream = _fdopen(m_hConnection, "w");
	*stderr = *m_pFileStream;
	setvbuf(stderr, NULL, _IONBF, 0);

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well

	std::ios::sync_with_stdio();

	// Capture the window handle
	m_hConsoleWindowHandle = GetConsoleWindow();
	CNM(m_hConsoleWindowHandle, "Failed to capture window handle");

Error:
	return r;
}

// This will actually run the code
RESULT Win64SandboxConsoleProcess::Process() {
	RESULT r = R::OK;

	CRM(Initialize(), "Failed to initialize win64 console");

	CRM(Show(), "Failed to show win64 console");

	while (IsRunning()) {
		
		// Lets not burn hot
		Sleep(5000);

		printf("*tick*\n");

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

RESULT Win64SandboxConsoleProcess::Show() {
	RESULT r = R::OK;

	m_fVisible = true;
	bool fPreviousVisible = ShowWindow(m_hConsoleWindowHandle, SW_SHOWDEFAULT);

Error:
	return r;
}

RESULT Win64SandboxConsoleProcess::Hide() {
	RESULT r = R::OK;

	m_fVisible = false;
	bool fPreviousVisible = ShowWindow(m_hConsoleWindowHandle, SW_HIDE);

Error:
	return r;
}

rectangle<int> Win64SandboxConsoleProcess::GetScreenDimensions() {
	return rectangle<int> {
		GetSystemMetrics(SM_CXSCREEN),
			GetSystemMetrics(SM_CYSCREEN)
	};
}