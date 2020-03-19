#include "eplogger.h"

#include "core/ehm/ehm.h"

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <ctime>

// TODO: Replace all of this with PathManager / FS/OS Framework thing
#if (defined(_WIN32) || defined(_WIN64))
	
	std::string GetPathOfExecutible() {
		char szPathResult[MAX_PATH];
		return std::string(szPathResult, GetModuleFileNameA(nullptr, szPathResult, MAX_PATH));
	}

	std::string GetCommandLineString() {
		return std::string(GetCommandLineA());
	}

	uint32_t GetProcessID() {
		return GetCurrentProcessId();
	}
#elif (defined(__APPLE__))
	#include <string>
	#include <limits.h>
	#include <unistd.h>

	#include <mach-o/dyld.h>

	std::string GetPathOfExecutible() {
		char szPath[PATH_MAX];
		uint32_t szPath_n = sizeof(szPath);
		memset(szPath, 0, szPath_n);
		
		if(_NSGetExecutablePath(szPath, &szPath_n) == 0)
			return std::string(szPath, szPath_n);
		else
    		return std::string();
	}

	std::string GetCommandLineString() {
		// TODO: Not implemented currently
		return "nimpl";
	}

	uint32_t GetProcessID() {
		return (int)(getpid());
	}

#elif (defined(__linux) || defined(__linux__))
	#include <string>
	#include <limits.h>
	#include <unistd.h>

	std::string GetPathOfExecutible() {
		char result[PATH_MAX];
		ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
		return std::string(result, (count > 0) ? count : 0);
	}

	std::string GetCommandLineString() {
		// TODO: Not imp.
		return "";
	}

	uint32_t GetProcessID() {
		return (int)(getpid());
	}

#endif

std::string GetFolderPathOfExecutible() {

#ifdef EPOCHLOGDIR
	return std::string(EPOCHLOGDIR);
#else
	std::string strExecPath = GetPathOfExecutible();

	#if (defined(_WIN32) || defined(_WIN64))
		auto slashPosition = strExecPath.find_last_of("\\/");
	#else
		auto slashPosition = strExecPath.find_last_of("/");
	#endif
		return strExecPath.substr(0, slashPosition);
#endif
}


SBLogger::SBLogger() {
	// empty
}

SBLogger::~SBLogger() {
	if (m_pSpdlogger != nullptr) {
		m_pSpdlogger = nullptr;
	}

	spdlog::drop_all();
}

RESULT SBLogger::Initialize() {
	RESULT r = R::PASS;

	std::time_t timeNow = std::time(nullptr);
	std::tm* localTimeNow = std::localtime(&timeNow);

	char szTime[32];
	std::strftime(szTime, 32, "%Y-%m-%d_%H-%M-%S", localTimeNow);

#if (defined(_WIN32) || defined(_WIN64))
	m_strSBLogPath = GetFolderPathOfExecutible() + "\\" + "log-" + szTime + ".log";
#else
	m_strSBLogPath = GetFolderPathOfExecutible() + "/" + "log-" + szTime + ".log";
#endif
	
	// Configure async a bit 
	spdlog::init_thread_pool(SPDLOG_QUEUE_SIZE, SPDLOG_THREAD_COUNT);
	spdlog::flush_every(std::chrono::seconds(1));

	m_pSpdlogger = spdlog::create_async<spdlog::sinks::basic_file_sink_mt>(m_kSBLoggerName, m_strSBLogPath);
	CN(m_pSpdlogger);

	log(level::info, "SBVR Process Launched: {}", GetPathOfExecutible());
	log(level::info, "SBVR Process ID: {:d}", GetProcessID());

Error:
	return r;
}

RESULT SBLogger::InitializeSingleton() {
	RESULT r = R::PASS;

	CRM(Initialize(), "Failed to initialize logger");

Error:
	return r;
}