#ifndef EHM_H_
#define EHM_H_

#include "result.h"

// EPOCH Core
// epoch/src/core/ehm.h
// Error Handling Macros (EHM)

// Config
// TODO: Move to core config.h
#define DEBUG_OUT_TO_CONSOLE
//#define DEBUG_OUT_TO_WIN_DEBUGGER
#define DEBUG_FILE_LINE
#define _ENABLE_LOGGING
#define LOG_SBVR_MSGS

// Compile Stuff
#if defined(__clang__)
	#define UNUSED __attribute__((unused))
#elif defined(__GNUC__) || defined(__GNUG__)
	#define UNUSED __attribute__((unused))
#elif defined(_MSC_VER)
	#define UNUSED /* empty */
#endif

#define SBVR_DEBUGGER_SIGNATURE			"SBVR::"
#define SBVR_DEBUGGER_SIGNATURE_SIZE		5			// size in bytes
#define	SBVR_DEBUGGER_OUTPUT_MAX_SIZE	1024

// DevEnv output (available in release) but should throw a production error
#ifdef _WIN32
	extern void OutputDebugString(wchar_t*);
	static char szDebugOutputString[SBVR_DEBUGGER_OUTPUT_MAX_SIZE] = { SBVR_DEBUGGER_SIGNATURE };

	//#define DEVENV_LINEOUT(str) do { OutputDebugString(str); } while(0); 

	#define DEVENV_LINEOUT(str, ...) do {																												\
		sprintf_s(szDebugOutputString + SBVR_DEBUGGER_SIGNATURE_SIZE, SBVR_DEBUGGER_OUTPUT_MAX_SIZE - SBVR_DEBUGGER_SIGNATURE_SIZE, str, ##__VA_ARGS__);	\
		if (szDebugOutputString[SBVR_DEBUGGER_SIGNATURE_SIZE] != '\n' && szDebugOutputString[SBVR_DEBUGGER_SIGNATURE_SIZE] != '\r')						\
			OutputDebugStringA(szDebugOutputString);																									\
		} while(0);
#endif

#include "logger/sblogger.h"

// Logging (needs DreamLogger included)
// This has been moved to project config, otherwise it breaks across multiple projects
// This requirement will be fixed when we move away from the monolithic build

#ifdef _ENABLE_LOGGING
#define SBVRLOG(logLevel, strMsg, ...) do { \
		SBLogger::instance()->log(SBLogger::level::logLevel, strMsg, ##__VA_ARGS__); \
	} while (0);
#else
	#define SBVRLOG(level, strMsg, ...)
#endif

#ifdef DEBUG_FILE_LINE
	#define CurrentFileLine "%s line#%d func:%s ", __FILE__, __LINE__, __FUNCTION__
#else
	#define CurrentFileLine ""
#endif

#ifndef __FUNCTION_NAME__
	#ifdef WIN32   //WINDOWS
		#define CurrentFunctionName  __FUNCTION__  
	#else          //*NIX
		#define CurrentFunctionName   __func__ 
	#endif
#else
	#define CurrentFunctionName __FUNCTION_NAME__
#endif

// TODO: Visual studio dosesn't support __BASE_FILE__ correctly
#ifndef __BASE_FILE__
	#define __BASE_FILE__ __FILE__
#endif

#ifdef LOG_SBVR_MSGS
	#define	SBVR_LOG_OUTPUT_MAX_SIZE		1024
	static char szSBVRLogOutputString[SBVR_DEBUGGER_OUTPUT_MAX_SIZE] = { SBVR_DEBUGGER_SIGNATURE };

	#define SBVRLogLine(level, crt, r) SBVRLOG(error, "{}:{}:{}({:d}):Error: {:#x}\n", crt, __BASE_FILE__, __FUNCTION__, __LINE__, r)

	// TODO: The variable arguments snprintf copy is redundant since spdlog will take in a varargs
	// TODO: spdlog uses the fmt library, so we'd need to move everything to fmt 
	#define SBVRLogLineMessage(level, crt, r, msg, ...) do {																	\
		snprintf(szSBVRLogOutputString, SBVR_LOG_OUTPUT_MAX_SIZE, msg, ##__VA_ARGS__);																	\
		SBVRLOG(level, "{}:{}:{}({:d}:{:#x}):{}\n", crt, __BASE_FILE__, __FUNCTION__, __LINE__, r, szSBVRLogOutputString)		\
	} while (0);

	#define SBVRLogError(crt, r) SBVRLogLine(error, crt, r)
	#define SBVRLogErrorMessage(crt, r, msg, ...) SBVRLogLineMessage(error, crt, r, msg, ##__VA_ARGS__)

	#define SBVRLogWarning(crt, r) SBVRLogLine(warn, crt, r)
	#define SBVRLogWarningMessage(crt, r, msg, ...) SBVRLogLineMessage(warn, crt, r, msg, ##__VA_ARGS__)
#else
	#define SBVRLogLine(level, crt, r)
	#define SBVRLogLineMessage(level, crt, r, msg, ...)

	#define SBVRLogError(crt, r)
	#define SBVRLogErrorMessage(crt, r, msg, ...)

	#define SBVRLogWarning(crt, r)
	#define SBVRLogWarningMessage(crt, r, msg, ...)
#endif

#if defined(DEBUG_OUT_TO_CONSOLE)
	#define CONSOLE_OUT(str, ...) do { printf(str, ##__VA_ARGS__); } while(0);
#elif defined(DEBUG_OUT_TO_WIN_DEBUGGER)
	// TODO: 
#endif

// Console Output
#ifdef _DEBUG
    #define DEBUG_OUT(str, ...) do { CONSOLE_OUT(str, ##__VA_ARGS__); } while(0);
    #define DEBUG_LINEOUT(str, ...) do { CONSOLE_OUT(str, ##__VA_ARGS__); CONSOLE_OUT("\n"); } while(0); 
	#define DEBUG_LINEOUT_RETURN(str, ...) do { CONSOLE_OUT(str, ##__VA_ARGS__); CONSOLE_OUT("\r"); } while(0); 
	#define DEBUG_SYSTEM_PAUSE() do { system("pause"); } while(0); 
#else
	#define DEBUG_OUT(str, ...)
	#define DEBUG_LINEOUT(str, ...)
	#define DEBUG_LINEOUT_RETURN(str, ...) 
	#define DEBUG_SYSTEM_PAUSE()
#endif

// check result value
#define CR(res) do{r=(res);if(r&0x80000000){SBVRLogError("CR", r); goto Error;}} while(0);
#define CRM(res, msg, ...) do{r= (res);if(r&0x80000000){ SBVRLogErrorMessage("CRM", res, msg, ##__VA_ARGS__); DEBUG_OUT(CurrentFileLine); DEBUG_OUT(msg, ##__VA_ARGS__); DEBUG_OUT("Error: 0x%x\n",r); goto Error;}}while(0);

// check null
#define CN(pointer) do{if((pointer) == nullptr) {r = R::FAIL; SBVRLogError("CN", r); goto Error;}} while(0);
#define CNM(pointer, msg, ...) do{if((pointer) == nullptr) {r = RESULT::FAIL; SBVRLogErrorMessage("CNM", r, msg, ##__VA_ARGS__); DEBUG_OUT(CurrentFileLine); DEBUG_OUT(msg, ##__VA_ARGS__); DEBUG_OUT("\n"); goto Error; }} while(0);

// check boolean conditional
#define CB(condition) do{if(!condition) {r = R::FAIL; SBVRLogError("CB", r); goto Error;}} while(0);
#define CBM(condition, msg, ...) do{if(condition == false) {r = R::FAIL; SBVRLogErrorMessage("CBM", r, msg, ##__VA_ARGS__); DEBUG_OUT(CurrentFileLine); DEBUG_OUT(msg, ##__VA_ARGS__); DEBUG_OUT("\n"); goto Error; }} while(0);

#endif // !EHM_H_