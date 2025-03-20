/**
 * @file logger.h
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
#ifndef LOGGER_H_
#define LOGGER_H_
#define BOOST_ALL_DYN_LINK

#include <map>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

using namespace logging::trivial;

namespace vne
{
namespace utilities
{
    class Logger
    {
       public:
        static Logger *Instance();

        void logTrace(std::string message);
        void logInfo(std::string message);
        void logDebug(std::string message);
        void logWarn(std::string message);
        void logError(std::string message);
        void logFatal(std::string message);

       private:
        Logger();
        Logger(Logger const &);
        Logger &operator=(Logger const &);
        virtual ~Logger();

        src::severity_logger<severity_level> log_;

        static Logger *logger_;  // singleton instance
    };
}  // namespace utilities
}  // namespace vne

#endif /* LOGGER_H_ */
