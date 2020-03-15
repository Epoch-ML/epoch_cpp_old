#ifndef SB_LOGGER_H_
#define SB_LOGGER_H_
#pragma once

#include "core/ehm/result.h"

// epoch logger
// epoch/src/logger/eplogger.h
// The epoch logger 

#include "core/types/singleton.h"

#include "third_party/spdlog/include/spdlog/spdlog.h"

#define SPDLOG_QUEUE_SIZE 1024
#define SPDLOG_THREAD_COUNT 1

// The curiously recursive template 
class SBLogger : public singleton<SBLogger> {
	friend class singleton<SBLogger>;

public:
	enum class level {
		info,
		warn,
		error,
		critical
	};

protected:
	SBLogger();
	virtual ~SBLogger();

public: 
	template <typename... Args>
	RESULT log(SBLogger::level logLevel, const char* pszMessage, const Args& ... args) {
		RESULT r = R::PASS;

		//CNM(m_pSpdlogger);
		if (m_pSpdlogger == nullptr) {
			goto Error;
		}

		switch (logLevel) {
			case SBLogger::level::info: {
				m_pSpdlogger->info(pszMessage, args...);
			} break;

			case SBLogger::level::warn: {
				m_pSpdlogger->warn(pszMessage, args...);
			} break;

			case SBLogger::level::error: {
				m_pSpdlogger->error(pszMessage, args...);
			} break;

			case SBLogger::level::critical: {
				m_pSpdlogger->critical(pszMessage, args...);
			} break;
		}

	Error:
		return r;
	}

	RESULT Initialize();

	 virtual RESULT InitializeSingleton() override;

private:
	std::shared_ptr<spdlog::logger> m_pSpdlogger = nullptr;

	std::string m_strSBLogPath;
	const std::string m_kSBLoggerName = "sblogger";
};

#endif // ! SB_LOGGER_H_