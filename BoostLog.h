#ifndef _BOOST_LOG_H_
#define _BOOST_LOG_H_
#pragma warning(push)
#pragma warning(disable:4819)
#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/format.hpp>
#include <string>
#pragma warning(pop)
#pragma warning(disable:4503)

#define BOOST_LOG_BUFFER_SIZE	4096

class BoostLog
{
public:
	BoostLog(void);

	~BoostLog(void);

	static void init(const std::string& path, bool onFile, bool onConsole);

	static void fini();

	static bool isInit() { return ms_bInit; }

	static void userLog(const char *fmt, ...);

	static void errorLog(const char *fmt, ...);
private:
	static bool ms_bInit;
};

#endif