#ifndef CHUGGER_TICK_HPP
#define CHUGGER_TICK_HPP

#include <string>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace ws
{
/*****************************************************************************
 *
 * Time class
 *
 *****************************************************************************/
class Tick
{
public:
    // constructor & destructor
    Tick(std::string = "",
         unsigned long int = 0,
         float = 0,
         float = 0);
    ~Tick();

    // accessors
    float				getPrice();
    std::string			getSymbol();
    unsigned long int	getTimestamp();
    float				getVolume();

private:
    std::string			symbol_;
    unsigned long int	timestamp_;
    float				price_;
    float				volume_;
};

} // namespace ws

#endif //CHUGGER_TICK_HPP
