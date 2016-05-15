#include "tick.hpp"

/**
 * Tick constructor
 *
 * @param[in] string    tick symbol
 * @param[in] integer   timestamp
 * @param[in] float     price
 * @param[in] float     volume
 *
 */
ws::Tick::Tick(std::string s, unsigned long int t, float p, float v) :
        symbol_(s),
        timestamp_(t),
        price_(p),
        volume_(v)
{
    // TODO validation
}

ws::Tick::~Tick()
{
    // destructor
}

float
ws::Tick::getPrice()
{
    return price_;
}

std::string
ws::Tick::getSymbol()
{
    return symbol_;
}

const char*
ws::Tick::getSymbolAsChar()
{
    return symbol_.c_str();
}

unsigned long int
ws::Tick::getSessionInterval()
{
    return timestamp_ - (timestamp_ % SESSION_SHARD_INTERVAL);
}

unsigned long int
ws::Tick::getTimestamp()
{
    return timestamp_;
}

float
ws::Tick::getVolume()
{
    return volume_;
}