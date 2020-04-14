#ifndef EP_LOGGER_H_
#define EP_LOGGER_H_
#pragma once

#include "core/ehm/result.h"

// epoch logger
// epoch/src/logger/eplogger.h
// The epoch logger 

#include "core/types/singleton.h"

#include "spdlog/spdlog.h"

#define SPDLOG_QUEUE_SIZE 1024
#define SPDLOG_THREAD_COUNT 1

// The curiously recursive template 
class EPLogger : public singleton<EPLogger> {
	friend class singleton<EPLogger>;

public:
	enum class level {
		info,
		warn,
		error,
		critical
	};

protected:
	EPLogger();
	virtual ~EPLogger();

public: 
	template <typename... Args>
	RESULT log(EPLogger::level logLevel, const char* pszMessage, const Args& ... args) {
		RESULT r = R::PASS;

		//CNM(m_pSpdlogger);
		if (m_pSpdlogger == nullptr) {
			goto Error;
		}

		switch (logLevel) {
			case EPLogger::level::info: {
				m_pSpdlogger->info(pszMessage, args...);
			} break;

			case EPLogger::level::warn: {
				m_pSpdlogger->warn(pszMessage, args...);
			} break;

			case EPLogger::level::error: {
				m_pSpdlogger->error(pszMessage, args...);
			} break;

			case EPLogger::level::critical: {
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

	std::string m_strEPLogPath;
	const std::string m_kEPLoggerName = "eplogger";
};

#endif // ! EP_LOGGER_H_