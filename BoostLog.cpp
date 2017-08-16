#include "BoostLog.h"

bool BoostLog::ms_bInit = false;

BoostLog::BoostLog(void)
{
}

BoostLog::~BoostLog(void)
{
}

void BoostLog::init(const std::string& path, bool onFile, bool onConsole)
{
	/* init boost log
	* 1. Add common attributes
	* 2. set log filter to trace
	*/
	boost::log::add_common_attributes();
	boost::log::core::get()->add_global_attribute("Scope",
		boost::log::attributes::named_scope());
	boost::log::core::get()->set_filter(
		boost::log::trivial::severity >= boost::log::trivial::trace
		);

	/* log formatter:
	* [TimeStamp] [ThreadId] [Severity Level] [Scope] Log message
	*/
	auto fmtTimeStamp = boost::log::expressions::
		//format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f");
		format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S");
	auto fmtThreadId = boost::log::expressions::
		attr<boost::log::attributes::current_thread_id::value_type>("ThreadID");
	auto fmtSeverity = boost::log::expressions::
		attr<boost::log::trivial::severity_level>("Severity");
	auto fmtScope = boost::log::expressions::format_named_scope("Scope",
		boost::log::keywords::format = "%n(%f:%l)",
		boost::log::keywords::iteration = boost::log::expressions::reverse,
		boost::log::keywords::depth = 2);
	boost::log::formatter logFmt =
		/*boost::log::expressions::format("[%1%] (%2%) [%3%] [%4%] %5%")
		% fmtTimeStamp % fmtThreadId % fmtSeverity % fmtScope
		% boost::log::expressions::smessage;*/
		boost::log::expressions::format("[%1%] (%2%) [%3%] %4%")
		% fmtTimeStamp % fmtThreadId % fmtSeverity % boost::log::expressions::smessage;

	/* console sink */
	if (onConsole)
	{
		auto consoleSink = boost::log::add_console_log(std::clog);
		consoleSink->set_formatter(logFmt);
	}
	
	if (onFile)
	{
		/* fs sink */
		auto fsSink = boost::log::add_file_log(
			boost::log::keywords::file_name = path + "%Y-%m-%d_%H-%M-%S.log",
			boost::log::keywords::rotation_size = 100 * 1024 * 1024,
			boost::log::keywords::min_free_space = 250 * 1024 * 1024,
			boost::log::keywords::open_mode = std::ios_base::app);
		fsSink->set_formatter(logFmt);
		fsSink->locked_backend()->auto_flush(true);
	}
	ms_bInit = true;
}

void BoostLog::fini()
{
	ms_bInit = false;
}

void BoostLog::userLog(const char *fmt, ...)
{
	if (!ms_bInit)
		return;

	char buf[BOOST_LOG_BUFFER_SIZE];
	memset(buf, 0, sizeof(buf));

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, BOOST_LOG_BUFFER_SIZE - 1, fmt, ap);
	va_end(ap);

	BOOST_LOG_TRIVIAL(info) << buf;
}

void BoostLog::errorLog(const char *fmt, ...)
{
	if (!ms_bInit)
		return;

	char buf[BOOST_LOG_BUFFER_SIZE];
	memset(buf, 0, sizeof(buf));

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, BOOST_LOG_BUFFER_SIZE - 1, fmt, ap);
	va_end(ap);

	BOOST_LOG_TRIVIAL(error) << buf;
}
