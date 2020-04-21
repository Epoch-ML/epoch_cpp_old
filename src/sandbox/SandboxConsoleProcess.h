#ifndef SANDBOX_CONSOLE_PROCESS_H_
#define SANDBOX_CONSOLE_PROCESS_H_

#include "core/ehm/ehm.h"

// epoch console process
// epoch/src/sandbox/SandboxConsoleProcess.h

#include "sandbox/SandboxProcess.h"

#include "core/math/rectangle.h"
#include "core/math/point.h"

#define DEFAULT_CONSOLE_WIDTH 1920 / 2
#define DEFAULT_CONSOLE_HEIGHT 1080 / 2
#define DEFAULT_MAX_CONSOLE_LINES 500

class SandboxConsoleProcess :
	public SandboxProcess
{
public:
	SandboxConsoleProcess() = default;
	virtual ~SandboxConsoleProcess() override = default;

	virtual SandboxProcess::type GetType() override {
		return SandboxProcess::type::console;
	}

	virtual RESULT Show() = 0;
	virtual RESULT Hide() = 0;

public:
	RESULT SetDimensions(int pxWidth, int pxHeight) {
		m_rectDimensions = { pxWidth, pxHeight };
		return R::OK;
	}

protected:
	rectangle<int> m_rectDimensions = {
		DEFAULT_CONSOLE_WIDTH,
		DEFAULT_CONSOLE_HEIGHT
	};

	point<int, 2> m_ptPosition;

	bool m_fVisible = true;
};


#endif // ! SANDBOX_WINDOW_PROCESS_H_