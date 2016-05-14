#include "tick.hpp"

/**
 * Tick constructor
 *
 * @param string    tick symbol
 * @param integer   timestamp
 * @param float     price
 * @param float     volume
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

float ws::Tick::getPrice()
{
    return price_;
}

std::string ws::Tick::getSymbol()
{
    return symbol_;
}

unsigned long int ws::Tick::getTimestamp()
{
    return timestamp_;
}

float ws::Tick::getVolume(){
    return volume_;
}