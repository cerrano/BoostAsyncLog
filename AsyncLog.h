#ifndef _ASYNCLOG_H_
#define _ASYNCLOG_H_

#include <boost/thread/detail/singleton.hpp>
#include <boost/asio.hpp>
#include <boost/format.hpp>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <thread>
#include <vector>
#include <memory>

class AsyncLog : public boost::asio::io_service::service
{
public:
	AsyncLog(void);
	~AsyncLog(void);

	void init(void);
	void fini(void);

	/// Destroy all user-defined handler objects owned by the service.
	void shutdown_service()
	{
	}

	bool isInit() { return m_bInit; }	

	void userlog(const char *fmt, ...);
	void errorlog(const char *fmt, ...);
protected:
	void userlog_impl(const std::string& text);
	void errorlog_impl(const std::string& text);

	boost::asio::io_service m_io_service;				///< 
	boost::asio::io_service::work m_work;				///< 
	std::thread m_work_thread;							///< 
	bool m_bInit;
};
typedef boost::detail::thread::singleton<AsyncLog> GetAsyncLog;

#endif