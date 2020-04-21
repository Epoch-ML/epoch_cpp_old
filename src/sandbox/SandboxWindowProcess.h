#ifndef SANDBOX_WINDOW_PROCESS_H_
#define SANDBOX_WINDOW_PROCESS_H_

#include "core/ehm/ehm.h"

// epoch Windows 64 Sandbox
// epoch/src/sandbox/win64/Win64Sandbox.h

#include "sandbox/SandboxProcess.h"

#include "core/math/rectangle.h"
#include "core/math/point.h"

#define DEFAULT_WINDOW_WIDTH 1920 / 2
#define DEFAULT_WINDOW_HEIGHT 1080 / 2

class SandboxWindowProcess :
	public SandboxProcess
{
public:
	SandboxWindowProcess() = default;
	virtual ~SandboxWindowProcess() override = default;

	virtual SandboxProcess::type GetType() override {
		return SandboxProcess::type::window;
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
		DEFAULT_WINDOW_WIDTH,
		DEFAULT_WINDOW_HEIGHT
	};

	point<int, 2> m_ptPosition;

	bool m_fFullscreen = false;
};


#endif // ! SANDBOX_WINDOW_PROCESS_H_