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
#include <utilities/logger.h>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;
namespace vne
{

Logger::Logger(std::string fileName)
{
    initialize(fileName);
}

Logger::Logger(Logger const&)
{
}

Logger::~Logger()
{

}

Logger* Logger::logger_ = nullptr;
Logger* Logger::getInstance(std::string logFile)
{
    if ( Logger::logger_ == nullptr ) {
        logging::add_file_log( logFile );

        logging::core::get()->set_filter
        (
            logging::trivial::severity >= logging::trivial::info
        );

        logging::add_common_attributes();
        Logger::logger_ = new Logger(logFile);
    }

    return Logger::logger_;
}

void Logger::initialize(std::string fileName)
{
    BOOST_LOG(log_) << "Hello, World!";
    BOOST_LOG_SEV(log_, info) << "Hello, World2!";
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

}/* namespace vne */
/*
int main(int, char*[])
{
    logging::add_common_attributes();

    using namespace logging::trivial;

    vne::Logger::getInstance()->logInfo("himom");

    return 0;
}
*/
