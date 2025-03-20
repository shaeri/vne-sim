/**
 * @file logger.cpp
 * @author Soroush Haeri <soroosh.haeri@me.com>
 * @date Jun 18, 2014
 * 
 * @copyright Copyright (c) Jun 18, 2014                      SOROUSH HAERI
 *            All Rights Reserved 
 *
 *            Permission to use, copy, modify, and distribute this software and its
 *            documentation for any purpose and without fee is hereby granted, provided
 *            that the above copyright notice appear in all copies and that both that
 *            copyright notice and this permission notice appear in supporting
 *            documentation, and that the name of the author not be used in advertising or
 *            publicity pertaining to distribution of the software without specific,
 *            written prior permission.
 *
 *            THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 *            ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS; IN NO EVENT SHALL
 *            AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
 *            DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 *            AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *            OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include "utilities/logger.h"
#include "core/config-manager.h"

#include <boost/filesystem.hpp>

#include <cstring>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

namespace vne
{
namespace utilities
{
    Logger::Logger() {}

    Logger::Logger(Logger const &) {}

    Logger::~Logger()
    {
        delete logger_;
    }

    Logger *Logger::logger_ = nullptr;
    Logger *Logger::Instance()
    {
        if (Logger::logger_ == nullptr) {
            char *logEnv = std::getenv("VNESIMLOGFILE");
            std::string logFile = logEnv != nullptr ? logEnv : "vnesim.log";

            if (boost::filesystem::exists(logFile))
                logging::add_file_log(logFile,
                                      keywords::format = "[%TimeStamp%] %Severity%: %Message%",
                                      keywords::auto_flush = true);

            const char *env_p = std::getenv("LOG_LEVEL");

            if (env_p != nullptr) {
                if (strcmp(env_p, "trace") == 0)
                    logging::core::get()->set_filter(logging::trivial::severity >=
                                                     logging::trivial::trace);
                else if (strcmp(env_p, "debug") == 0)
                    logging::core::get()->set_filter(logging::trivial::severity >=
                                                     logging::trivial::debug);
                else if (strcmp(env_p, "info") == 0)
                    logging::core::get()->set_filter(logging::trivial::severity >=
                                                     logging::trivial::info);
                else if (strcmp(env_p, "warning") == 0)
                    logging::core::get()->set_filter(logging::trivial::severity >=
                                                     logging::trivial::warning);
                else if (strcmp(env_p, "error") == 0)
                    logging::core::get()->set_filter(logging::trivial::severity >=
                                                     logging::trivial::error);
                else if (strcmp(env_p, "fatal") == 0)
                    logging::core::get()->set_filter(logging::trivial::severity >=
                                                     logging::trivial::fatal);
            } else {
                std::string log_level = ConfigManager::Instance()->getConfig<std::string>(
                    "utilities", "logLevel");
                if (strcmp(log_level.c_str(), "trace") == 0)
                    logging::core::get()->set_filter(logging::trivial::severity >=
                                                     logging::trivial::trace);
                else if (strcmp(log_level.c_str(), "debug") == 0)
                    logging::core::get()->set_filter(logging::trivial::severity >=
                                                     logging::trivial::debug);
                else if (strcmp(log_level.c_str(), "info") == 0)
                    logging::core::get()->set_filter(logging::trivial::severity >=
                                                     logging::trivial::info);
                else if (strcmp(log_level.c_str(), "warning") == 0)
                    logging::core::get()->set_filter(logging::trivial::severity >=
                                                     logging::trivial::warning);
                else if (strcmp(log_level.c_str(), "error") == 0)
                    logging::core::get()->set_filter(logging::trivial::severity >=
                                                     logging::trivial::error);
                else if (strcmp(log_level.c_str(), "fatal") == 0)
                    logging::core::get()->set_filter(logging::trivial::severity >=
                                                     logging::trivial::fatal);
                else
                    logging::core::get()->set_filter(logging::trivial::severity >=
                                                     logging::trivial::debug);
            }

            logging::add_common_attributes();
            Logger::logger_ = new Logger();
        }

        return Logger::logger_;
    }

    void Logger::logTrace(std::string message)
    {
        BOOST_LOG_SEV(log_, trace) << message;
    }
    void Logger::logInfo(std::string message)
    {
        BOOST_LOG_SEV(log_, info) << message;
    }

    void Logger::logDebug(std::string message)
    {
        BOOST_LOG_SEV(log_, debug) << message;
    }

    void Logger::logWarn(std::string message)
    {
        BOOST_LOG_SEV(log_, warning) << message;
    }

    void Logger::logError(std::string message)
    {
        BOOST_LOG_SEV(log_, error) << message;
    }

    void Logger::logFatal(std::string message)
    {
        BOOST_LOG_SEV(log_, fatal) << message;
    }
}  // namespace utilities
} /* namespace vne */