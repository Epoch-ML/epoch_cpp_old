#ifndef SANDBOX_H_
#define SANDBOX_H_

#include "core/ehm/ehm.h"

// epoch sandbox
// epoch/src/sandbox/sandbox.h

#include "core/types/EPObj.h"
#include "core/types/EPRef.h"
#include "core/types/EPString.h"

#include "SandboxProcess.h"

extern const char* kPlatformNames[];

class sandbox :
    public EPObj
{

public:
    enum class platform : uint8_t {
        win64,
        osx,
        linux,
        android,
        ios,
        unix,
        posix,
        none
    };

protected:
    sandbox();

public:
    ~sandbox();

    RESULT CreateSandboxProcess(EPString<char> strProcessName, SandboxProcess::type procType);
    EPRef<SandboxProcess> GetSandboxProcess(EPString<char> strProcessName);
    RESULT RunSandboxProcess(EPString<char> strProcessName);
    RESULT KillSandboxProcess(EPString<char> strProcessName);

    static const char* GetPlatformName(sandbox::platform plat) {
        return kPlatformNames[static_cast<uint8_t>(plat)];
    }

private:
    RESULT SetPlatform(sandbox::platform plat) {
        m_platform = plat;
        return R::OK;
    }

public:
    static sandbox::platform GetCurrentPlatform();
    static EPRef<sandbox> make();

private:
    sandbox::platform m_platform = sandbox::platform::none;

private:
    EPVector<EPRef<SandboxProcess>> m_sandboxProcesses;
};

#endif // ! SANDBOX_H_