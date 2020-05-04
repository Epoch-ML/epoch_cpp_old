#ifndef HAL_H_
#define HAL_H_

#include "core/ehm/ehm.h"

// epoch HAL (Hardware Abstraction Library)
// epoch/src/hal/hal.h

#include "core/types/EPObj.h"

#include "core/types/EPProcess.h"
#include "core/types/EPString.h"
#include "core/types/EPRef.h"

#include "sandbox/SandboxWindowProcess.h"

class HAL : 
	public EPObj
{
public:
	enum class type : uint32_t { 
		vulkan,
		custom
	};
	
	// TODO: Not ideal
	static const char* GetTypeName(HAL::type halType) {
		const EPVector<char*> kHALTypes = {
			"vulkan",
			"custom"
		};

		return kHALTypes[static_cast<uint8_t>(halType)];
	}

	RESULT SetSBWindowProcess(const EPRef<SandboxWindowProcess>& pSBWindowProcess) {
		RESULT r = R::OK;

		CNM(pSBWindowProcess, "Setting a nullptr sandbox window process")

		m_pSBWindowProcess = pSBWindowProcess;

	Error:
		return r;
	}

	const EPRef<SandboxWindowProcess>& GetSBWindowProcess() {
		return const_cast<const EPRef<SandboxWindowProcess>&>(m_pSBWindowProcess);
	}

public:
	HAL() = default;

	virtual HAL::type GetType() = 0;

	virtual RESULT Initialize(void) = 0;
	virtual RESULT Kill(void) = 0;
	virtual RESULT Render() = 0;
	virtual RESULT WaitForIdle() = 0;

private:
	EPRef<SandboxWindowProcess> m_pSBWindowProcess = nullptr;
};


#endif // ! HAL_H_