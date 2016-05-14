#include "time.hpp"

/**
 * Time constructor
 *
 * @param Posix datetime    YYYY-MM-DD hh:mm:ss.fff
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
 * Converts constructor datetime to a Unix timestamp in milliseconds
 * and sets it to private member timestamp_.
 *
 */
void ws::Time::SetTimestamp(const std::string &datetime)
{
    boost::posix_time::ptime t(boost::posix_time::time_from_string(datetime));
    boost::posix_time::ptime start(boost::gregorian::date(1970, 1, 1));
    boost::posix_time::time_duration time_diff = t - start;
    timestamp_ = time_diff.total_milliseconds();
}