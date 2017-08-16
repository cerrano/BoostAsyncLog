#include "BoostLog.h"
#include "AsyncLog.h"

#define ASYNC_LOG_BUF_LEN	4096

AsyncLog::AsyncLog(void) : 
boost::asio::io_service::service(m_io_service)
, m_work(m_io_service)
, m_bInit(false)
{

}

AsyncLog::~AsyncLog(void)
{

}

void AsyncLog::init(void)
{
	try
	{
		// Start running the io_service.  The work_ object will keep
		// io_service::run() from returning even if there is no real work
		// queued into the io_service.
		auto self = this;
		m_work_thread = std::thread([self]()
		{
			self->m_bInit = true;
			self->m_io_service.run();
		});
	}
	catch (std::exception& e)
	{
		m_bInit = false;

		BOOST_LOG_TRIVIAL(error) << "[AsyncLog::init] init thread failed!!!, exception = " << e.what();
	}
}

void AsyncLog::fini(void)
{
	m_io_service.stop();

	m_work_thread.join();

	m_bInit = false;
}

void AsyncLog::userlog(const char *fmt, ...)
{
	if (!isInit())
		return;

	char buf[ASYNC_LOG_BUF_LEN];
	memset(buf, 0, sizeof(buf));
	//
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, ASYNC_LOG_BUF_LEN - 1, fmt, ap);
	va_end(ap);

	// Pass the work of opening the file to the background thread.
	std::ostringstream os;
	os << buf;
	m_io_service.post(boost::bind(&AsyncLog::userlog_impl, this, os.str()));
}

void AsyncLog::errorlog(const char *fmt, ...)
{
	if (!isInit())
		return;

	char buf[ASYNC_LOG_BUF_LEN];
	memset(buf, 0, sizeof(buf));
	//
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, ASYNC_LOG_BUF_LEN - 1, fmt, ap);
	va_end(ap);

	// Pass the work of opening the file to the background thread.
	std::ostringstream os;
	os << buf;
	m_io_service.post(boost::bind(&AsyncLog::errorlog_impl, this, os.str()));
}

void AsyncLog::userlog_impl(const std::string& text)
{
	if (!BoostLog::isInit())
		return;

	BOOST_LOG_TRIVIAL(info) << text.c_str();
}

void AsyncLog::errorlog_impl(const std::string& text)
{
	if (!BoostLog::isInit())
		return;

	BOOST_LOG_TRIVIAL(error) << text.c_str();
}