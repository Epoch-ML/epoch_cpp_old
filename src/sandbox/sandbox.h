#ifndef SANDBOX_H_
#define SANDBOX_H_

#include "core/ehm/ehm.h"

// epoch sandbox
// epoch/src/sandbox/sandbox.h

#include "core/types/EPObj.h"
#include "core/types/EPRef.h"

class sandbox :  
    public EPObj 
{

public:
    enum class platform {
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
};

#endif // ! SANDBOX_H_