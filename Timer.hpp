#ifndef TIMER_HPP
#define TIMER_HPP

#include <iostream>
#include <chrono>

class Timer{
public:
    Timer() : m_begin(clock_t::now()) {}
    void reset() { m_begin = clock_t::now(); }
    double elapsed() const {
        return std::chrono::duration_cast<second_t>
            (clock_t::now() - m_begin).count();
    }

private:
    typedef std::chrono::high_resolution_clock clock_t;
    typedef std::chrono::duration<double, std::ratio<1> > second_t;
    std::chrono::time_point<clock_t> m_begin;
};

#endif // !TIMER_HPP
