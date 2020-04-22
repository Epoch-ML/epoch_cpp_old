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
#define LOG_EP_MSGS

// Compile Stuff
#if defined(__clang__)
	#define UNUSED __attribute__((unused))
#elif defined(__GNUC__) || defined(__GNUG__)
	#define UNUSED __attribute__((unused))
#elif defined(_MSC_VER)
	#define UNUSED /* empty */
#endif

#define EP_DEBUGGER_SIGNATURE			"EP::"
#define EP_DEBUGGER_SIGNATURE_SIZE		5			// size in bytes
#define	EP_DEBUGGER_OUTPUT_MAX_SIZE	1024

// DevEnv output (available in release) but should throw a production error
#ifdef _WIN32
	extern void OutputDebugString(wchar_t*);
	static char szDebugOutputString[EP_DEBUGGER_OUTPUT_MAX_SIZE] = { EP_DEBUGGER_SIGNATURE };

	//#define DEVENV_LINEOUT(str) do { OutputDebugString(str); } while(0); 

	#define DEVENV_LINEOUT(str, ...) do {																												\
		sprintf_s(szDebugOutputString + EP_DEBUGGER_SIGNATURE_SIZE, EP_DEBUGGER_OUTPUT_MAX_SIZE - EP_DEBUGGER_SIGNATURE_SIZE, str, ##__VA_ARGS__);	\
		if (szDebugOutputString[EP_DEBUGGER_SIGNATURE_SIZE] != '\n' && szDebugOutputString[EP_DEBUGGER_SIGNATURE_SIZE] != '\r')						\
			OutputDebugStringA(szDebugOutputString);																									\
		} while(0);
#endif

#include "logger/eplogger.h"

// Logging (needs DreamLogger included)
// This has been moved to project config, otherwise it breaks across multiple projects
// This requirement will be fixed when we move away from the monolithic build

#ifdef _ENABLE_LOGGING
#define EPLOG(logLevel, strMsg, ...) do { \
		EPLogger::instance()->log(EPLogger::level::logLevel, strMsg, ##__VA_ARGS__); \
	} while (0);
#else
	#define EPLOG(level, strMsg, ...)
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

#ifdef LOG_EP_MSGS
	#define	EP_LOG_OUTPUT_MAX_SIZE		1024
	static char szEPLogOutputString[EP_DEBUGGER_OUTPUT_MAX_SIZE] = { EP_DEBUGGER_SIGNATURE };

	#define EPLogLine(level, crt, r) EPLOG(error, "{}:{}:{}({:d}):Error: {:#x}\n", crt, __BASE_FILE__, __FUNCTION__, __LINE__, r)

	// TODO: The variable arguments snprintf copy is redundant since spdlog will take in a varargs
	// TODO: spdlog uses the fmt library, so we'd need to move everything to fmt 
	#define EPLogLineMessage(level, crt, r, msg, ...) do {																	\
		snprintf(szEPLogOutputString, EP_LOG_OUTPUT_MAX_SIZE, msg, ##__VA_ARGS__);																	\
		EPLOG(level, "{}:{}:{}({:d}:{:#x}):{}\n", crt, __BASE_FILE__, __FUNCTION__, __LINE__, r, szEPLogOutputString)		\
	} while (0);

	#define EPLogError(crt, r) EPLogLine(error, crt, r)
	#define EPLogErrorMessage(crt, r, msg, ...) EPLogLineMessage(error, crt, r, msg, ##__VA_ARGS__)

	#define EPLogWarning(crt, r) EPLogLine(warn, crt, r)
	#define EPLogWarningMessage(crt, r, msg, ...) EPLogLineMessage(warn, crt, r, msg, ##__VA_ARGS__)
#else
	#define EPLogLine(level, crt, r)
	#define EPLogLineMessage(level, crt, r, msg, ...)

	#define EPLogError(crt, r)
	#define EPLogErrorMessage(crt, r, msg, ...)

	#define EPLogWarning(crt, r)
	#define EPLogWarningMessage(crt, r, msg, ...)
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

// compare
#define C_BLACK(strString)	"\x1B[30;1m" strString "\033[0m"
#define C_RED(strString)		"\x1B[31;1m" strString "\033[0m"
#define C_GREEN(strString)	"\x1B[32;1m" strString "\033[0m"
#define C_YELLOW(strString)	"\x1B[33;1m" strString "\033[0m"
#define C_BLUE(strString)	"\x1B[34;1m" strString "\033[0m"
#define C_MAGENTA(strString)	"\x1B[35;1m" strString "\033[0m"
#define C_CYAN(strString) 	"\x1B[36;1m" strString "\033[0m"
#define C_WHITE(strString) 	"\x1B[37;1m" strString "\033[0m"

// Extended 256 colors
#define C_PINK(strString) 	"\x1B[38;5;207m" strString "\033[0m"
#define C_ORANGE(strString) 	"\x1B[38;5;202m" strString "\033[0m"
#define C_PURPLE(strString) 	"\x1B[38;5;93m" strString "\033[0m"

inline const char* CMP_STR(size_t lhs, size_t rhs) {
	if (lhs == rhs) return C_BLUE("equal");
	else if (lhs < rhs) return C_GREEN("less than");
	else return C_ORANGE("greater than");
}

#define DEBUG_CMP(label, lhsname, lhs, rhsname, rhs, pct) do{ DEBUG_LINEOUT("%s: %s: %zu %s %s: %zu - %d%%", label, lhsname, lhs, CMP_STR(lhs, rhs), rhsname, rhs, pct); } while(0);

// check result value
#define CR(res) do{r=(res);if(r&0x80000000){EPLogError("CR", r); goto Error;}} while(0);
#define CRM(res, msg, ...) do{r= (res);if(r&0x80000000){ EPLogErrorMessage("CRM", res, msg, ##__VA_ARGS__); DEBUG_OUT(CurrentFileLine); DEBUG_OUT(msg, ##__VA_ARGS__); DEBUG_OUT("Error: 0x%x\n",r); goto Error;}}while(0);

// check null
#define CN(pointer) do{if((pointer) == nullptr) {r = R::FAIL; EPLogError("CN", r); goto Error;}} while(0);
#define CNR(pointer, rcode) do{if((pointer) == nullptr) {r = rcode; EPLogError("CN", r); goto Error;}} while(0);
#define CNM(pointer, msg, ...) do{if((pointer) == nullptr) {r = RESULT::FAIL; EPLogErrorMessage("CNM", r, msg, ##__VA_ARGS__); DEBUG_OUT(CurrentFileLine); DEBUG_OUT(msg, ##__VA_ARGS__); DEBUG_OUT("\n"); goto Error; }} while(0);

// check boolean conditional
#define CB(condition) do{if(!condition) {r = R::FAIL; EPLogError("CB", r); goto Error;}} while(0);
#define CBR(condition, rcode) do{if(!condition) {r = rcode; EPLogError("CBR", r); goto Error;}} while(0);
#define CBM(condition, msg, ...) do{if(condition == false) {r = R::FAIL; EPLogErrorMessage("CBM", r, msg, ##__VA_ARGS__); DEBUG_OUT(CurrentFileLine); DEBUG_OUT(msg, ##__VA_ARGS__); DEBUG_OUT("\n"); goto Error; }} while(0);
#define CBRM(condition, rcode, msg, ...) do{if(condition == false) {r = rcode; EPLogErrorMessage("CBM", r, msg, ##__VA_ARGS__); DEBUG_OUT(CurrentFileLine); DEBUG_OUT(msg, ##__VA_ARGS__); DEBUG_OUT("\n"); goto Error; }} while(0);

// compare less than
#define CLT(lhs, rhs) do{if(lhs >= rhs) {r = R::FAIL; EPLogError("CLT", r); goto Error;}} while(0);
#define CLTR(lhs, rhs, rcode) do{if(lhs >= rhs) {r = rcode; EPLogError("CLTR", r); goto Error;}} while(0);
#define CLTDM(lhs, rhs, emsg, dmsg, ...) do{if(lhs >= rhs) {r = R::FAIL; EPLogErrorMessage("CLTM", r, dmsg, ##__VA_ARGS__); DEBUG_OUT(CurrentFileLine); DEBUG_OUT(emsg); DEBUG_OUT(" : "); DEBUG_OUT(dmsg, ##__VA_ARGS__); DEBUG_OUT("\n"); goto Error; }} while(0);
#define CLTM(lhs, rhs, msg, ...) do{if(lhs >= rhs) {r = R::FAIL; EPLogErrorMessage("CLTM", r, msg, ##__VA_ARGS__); DEBUG_OUT(CurrentFileLine); DEBUG_OUT(msg, ##__VA_ARGS__); DEBUG_OUT("\n"); goto Error; }} while(0);
#define CLTRM(condition, rcode, msg, ...) do{if(lhs >= rhs) {r = rcode; EPLogErrorMessage("CLTRM", r, msg, ##__VA_ARGS__); DEBUG_OUT(CurrentFileLine); DEBUG_OUT(msg, ##__VA_ARGS__); DEBUG_OUT("\n"); goto Error; }} while(0);

// compare greater than
#define CGT(lhs, rhs) do{if(lhs <= rhs) {r = R::FAIL; EPLogError("CGT", r); goto Error;}} while(0);
#define CGTR(lhs, rhs, rcode) do{if(lhs <= rhs) {r = rcode; EPLogError("CGTR", r); goto Error;}} while(0);
#define CGTDM(lhs, rhs, emsg, dmsg, ...) do{if(lhs <= rhs) {r = R::FAIL; EPLogErrorMessage("CGTM", r, dmsg, ##__VA_ARGS__); DEBUG_OUT(CurrentFileLine); DEBUG_OUT(emsg); DEBUG_OUT(" : "); DEBUG_OUT(dmsg, ##__VA_ARGS__); DEBUG_OUT("\n"); goto Error; }} while(0);
#define CGTM(lhs, rhs, msg, ...) do{if(lhs <= rhs) {r = R::FAIL; EPLogErrorMessage("CGTM", r, msg, ##__VA_ARGS__); DEBUG_OUT(CurrentFileLine); DEBUG_OUT(msg, ##__VA_ARGS__); DEBUG_OUT("\n"); goto Error; }} while(0);
#define CGTRM(condition, rcode, msg, ...) do{if(lhs <= rhs) {r = rcode; EPLogErrorMessage("CGTRM", r, msg, ##__VA_ARGS__); DEBUG_OUT(CurrentFileLine); DEBUG_OUT(msg, ##__VA_ARGS__); DEBUG_OUT("\n"); goto Error; }} while(0);

// compare equal than
#define CEQ(lhs, rhs) do{if(lhs == rhs) {r = R::FAIL; EPLogError("CEQ", r); goto Error;}} while(0);
#define CEQR(lhs, rhs, rcode) do{if(lhs == rhs) {r = rcode; EPLogError("CEQR", r); goto Error;}} while(0);
#define CEQDM(lhs, rhs, emsg, dmsg, ...) do{if(lhs == rhs) {r = R::FAIL; EPLogErrorMessage("CEQM", r, dmsg, ##__VA_ARGS__); DEBUG_OUT(CurrentFileLine); DEBUG_OUT(emsg); DEBUG_OUT(" : "); DEBUG_OUT(dmsg, ##__VA_ARGS__); DEBUG_OUT("\n"); goto Error; }} while(0);
#define CEQM(lhs, rhs, msg, ...) do{if(lhs == rhs) {r = R::FAIL; EPLogErrorMessage("CEQM", r, msg, ##__VA_ARGS__); DEBUG_OUT(CurrentFileLine); DEBUG_OUT(msg, ##__VA_ARGS__); DEBUG_OUT("\n"); goto Error; }} while(0);
#define CEQRM(condition, rcode, msg, ...) do{if(lhs == rhs) {r = rcode; EPLogErrorMessage("CEQRM", r, msg, ##__VA_ARGS__); DEBUG_OUT(CurrentFileLine); DEBUG_OUT(msg, ##__VA_ARGS__); DEBUG_OUT("\n"); goto Error; }} while(0);



#endif // !EHM_H_