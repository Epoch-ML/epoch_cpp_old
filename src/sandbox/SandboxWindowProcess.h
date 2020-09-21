#ifndef SANDBOX_WINDOW_PROCESS_H_
#define SANDBOX_WINDOW_PROCESS_H_

#include "core/ehm/ehm.h"

// epoch Windows 64 Sandbox
// epoch/src/sandbox/win64/Win64Sandbox.h

#include "sandbox/SandboxProcess.h"

#include "core/math/rectangle.h"
#include "core/math/point.h"

#include "core/types/observable.h"

#define DEFAULT_WINDOW_WIDTH 1920 
#define DEFAULT_WINDOW_HEIGHT 1080 

class SandboxWindowProcessObserver {
public:
	virtual RESULT OnResize(uint32_t width, uint32_t height) = 0;
};

class SandboxWindowProcess :
	public SandboxProcess,
	public observable<SandboxWindowProcessObserver>
{
public:
	SandboxWindowProcess() = default;
	virtual ~SandboxWindowProcess() override = default;

	virtual SandboxProcess::type GetType() override {
		return SandboxProcess::type::window;
	}

	virtual RESULT Show() = 0;
	virtual RESULT Hide() = 0;

	RESULT OnResize(uint32_t width, uint32_t height) {
		RESULT r = R::OK;

		for (auto& pObserver : m_observers) {
			CRM(pObserver->OnResize(width, height), "OnResize observer callback failed");
		}

	Error:
		return r;
	}

	virtual intptr_t GetNativeWindowHandle() const = 0;

public:
	RESULT SetDimensions(int pxWidth, int pxHeight) {
		m_rectDimensions = { pxWidth, pxHeight };
		return R::OK;
	}

	const rectangle<int>& GetDimensions() const {
		return const_cast<rectangle<int>&>(m_rectDimensions);
	}

	inline int GetHeight() const {
		return m_rectDimensions.height();
	}

	inline int GetWidth() const {
		return m_rectDimensions.width();
	}

protected:
	rectangle<int> m_rectDimensions = {
		DEFAULT_WINDOW_WIDTH,
		DEFAULT_WINDOW_HEIGHT
	};

	point<int, 2> m_ptPosition;

	bool m_fFullscreen = false;
	bool m_fVisible = true;
};


#endif // ! SANDBOX_WINDOW_PROCESS_H_