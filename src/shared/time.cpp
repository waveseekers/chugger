#include <exception>
#include "time.hpp"

using namespace boost::posix_time;
using namespace boost::gregorian;

/**
 * Time constructor
 *
 * @param Posix datetime YYYY-MM-DD hh:mm:ss.fff
 *
 */
ws::Time::Time( const std::string& datetime ) :
        datetime_( datetime )
{
    SetTimestamp(datetime);
};

/**
 * Time destructor
 *
 */
ws::Time::~Time()
{
    // destructor
}

/**
 * Returns the Unix timestamp in milliseconds for the date object.
 *
 * @return timestamp milliseconds
 */
unsigned long int
ws::Time::GetTimestampMilliseconds()
{
    return timestamp_;
}

/**
 * Returns the database shard interval. This method is a helper function
 * for determining the session shard interval when inserting time series
 * data into a Cassandra keyspace.
 *
 * @return timestamp milliseconds
 */
unsigned long int
ws::Time::GetSessionIntervalMilliseconds()
{
    return timestamp_ - (timestamp_ % SESSION_SHARD_INTERVAL);
}

/**
 * Converts constructor datetime to a Unix timestamp in milliseconds
 * and sets it to private member timestamp_.
 *
 */
void
ws::Time::SetTimestamp(const std::string &datetime)
{
    try
    {
        boost::posix_time::ptime t(boost::posix_time::time_from_string(datetime));
        boost::posix_time::ptime start(boost::gregorian::date(1970, 1, 1));
        boost::posix_time::time_duration time_diff = t - start;
        timestamp_ = time_diff.total_milliseconds();
    }
    catch (std::exception &e)
    {
        printf("[SetTimestamp]: Invalid datetime string.\n");
    }
}