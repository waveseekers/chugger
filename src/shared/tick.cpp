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
ws::Tick::getPrice() const
{
    return price_;
}

std::string
ws::Tick::getSymbol() const
{
    return symbol_;
}

const char*
ws::Tick::getSymbolAsChar() const
{
    return symbol_.c_str();
}

unsigned long int
ws::Tick::getSessionInterval() const
{
    return timestamp_ - (timestamp_ % SESSION_SHARD_INTERVAL);
}

unsigned long int
ws::Tick::getTimestamp() const
{
    return timestamp_;
}

float
ws::Tick::getVolume() const
{
    return volume_;
}