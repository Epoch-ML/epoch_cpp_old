#ifndef SANDBOX_H_
#define SANDBOX_H_

#include "core/ehm/result.h"

// epoch sandbox
// epoch/src/sandbox/sandbox.h

#include "core/types/EPObj.h"
#include "core/types/EPRef.h"

class sandbox : 
    public EPObj 
{

protected:
    sandbox();
    ~sandbox();

public:
    static EPRef<sandbox> make() {

    }

private:
    // 
};

#endif // ! SANDBOX_H_