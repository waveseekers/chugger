#ifndef CHUGGER_TICK_HPP
#define CHUGGER_TICK_HPP

#include <string>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "time.hpp"

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
    float				getPrice() const;
    std::string			getSymbol() const;
    const char*			getSymbolAsChar() const;
    unsigned long int	getSessionInterval() const;
    unsigned long int	getTimestamp() const;
    float				getVolume() const;

private:
    std::string			symbol_;
    unsigned long int	timestamp_;
    float				price_;
    float				volume_;
};

} // namespace ws

#endif //CHUGGER_TICK_HPP
