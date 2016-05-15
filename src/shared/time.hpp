#ifndef CHUGGER_TIME_HPP
#define CHUGGER_TIME_HPP

#include <string>
#include <time.h>
//#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
//#include "datalib.hpp"

static const int SESSION_SHARD_INTERVAL = 3600000; // 1 hour in milliseconds
static const int SESSION_SHARD_INTERVAL_BARS = SESSION_SHARD_INTERVAL * 2;
typedef unsigned long int TimestampType;

namespace ws {

/*****************************************************************************
 *
 * WSTime class
 *
 *****************************************************************************/
    class Time
    {
    public:
        Time(const std::string&);
        ~Time();

        unsigned long int	GetSessionIntervalMilliseconds();
        unsigned long int	GetTimestampMilliseconds();
        void                SetTimestamp(const std::string&);

    private:
        std::string datetime_;
        unsigned long int timestamp_;
    };

} // namespace ws

#endif //CHUGGER_TIME_HPP
