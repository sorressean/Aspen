#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>

#define BOOST_LOG_DYN_LINK 1
#include <boost/core/null_deleter.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>

#include "log.h"

namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
using namespace std;

typedef sinks::synchronous_sink< sinks::text_file_backend > file_sink;
typedef sinks::synchronous_sink< sinks::text_ostream_backend > text_sink;

template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (std::basic_ostream< CharT, TraitsT >& strm, SeverityLevel lvl)
{
    static const char* const str[] =
    {
        "normal",
        "notification",
        "warning",
        "error",
        "critical"
    };
    if (static_cast<int>(lvl) < sizeof(str))
        strm << str[static_cast<size_t>(lvl)];
    else
        strm << static_cast< int >(lvl);
    return strm;
}

struct LogData
{
//we need to use boost's shared_ptr otherwise it won't accept our sink.
    boost::shared_ptr<file_sink> fileSink;
    boost::shared_ptr<text_sink> consoleSink;
    src::severity_logger< SeverityLevel> severityLogger;
};

std::shared_ptr<Log> Log::_instance = nullptr;

shared_ptr<Log> Log::Instance()
{
    return Log::_instance;
}

void Log::Initialize()
{
    if (!Log::_instance)
        {
            Log::_instance = shared_ptr<Log>(new Log);
Log::_instance->Setup();
        }
}

void Log::Setup()
{
    auto instance = Log::_instance;
    if (!instance)
        throw(std::logic_error("Can not set up without initialize being called."));

    _logData = make_unique<LogData>();
    _logData->fileSink  = boost::shared_ptr< file_sink > (new file_sink(keywords::file_name =LOG_FILE_PATTERN));
    auto fileBackend = _logData->fileSink->locked_backend();
    fileBackend->auto_flush();
    fileBackend->set_file_collector(sinks::file::make_collector(
                                        keywords::target = LOG_DIRECTORY,
                                        keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
                                        keywords::max_size = LOG_MAX_COLLECTED_FILES_SIZE,
                                        keywords::max_files = LOG_MAX_FILES));
    fileBackend->scan_for_files();
    _logData->fileSink->set_formatter(
        expr::format("%1%: [%2%] - %3%")
        % expr::attr< unsigned int >("RecordID")
        % expr::attr< boost::posix_time::ptime >("TimeStamp")
        % expr::smessage
    );
    logging::core::get()->add_sink(_logData->fileSink);

#ifdef LOG_CONSOLE
    _logData->consoleSink = boost::shared_ptr<text_sink>(new text_sink);
    auto consoleBackend = _logData->consoleSink->locked_backend();
    _logData->consoleSink->set_formatter(
        expr::format("[%2%] - %3%")
        % expr::attr< boost::posix_time::ptime >("TimeStamp")
        % expr::smessage
    );
    boost::shared_ptr< std::ostream > outputStream(&std::clog, boost::null_deleter());
    consoleBackend->add_stream(outputStream);
    logging::core::get()->add_sink(_logData->consoleSink);
#endif

    logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());
    logging::core::get()->add_global_attribute("RecordID", attrs::counter< unsigned int >());
}

void Log::Write(const std::string& message)
{
    Write(SeverityLevel::Info, message);
}
void Log::Write(SeverityLevel level, const std::string& message)
{
    BOOST_LOG_SEV(_logData->severityLogger, level) << message;
}
