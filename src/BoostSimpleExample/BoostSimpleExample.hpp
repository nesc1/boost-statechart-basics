#pragma once
#include "smcommon.h"

namespace BoostExample {
using namespace std;
namespace sc  = boost::statechart;
namespace mpl = boost::mpl;

/// another example (simple)
struct EvReset : sc::event<EvReset> {};
struct EvStartStop : sc::event<EvStartStop> {};

struct Active;
struct Stopped;
struct Running;

struct StopWatch : sc::state_machine<StopWatch, Active> {
    // m_startTime remains uninitialized, because there is no reasonable default
    StopWatch() = default;

    double elapsedTime() const {
        // Ugly switch over the current state.
        if(state_cast<const Stopped *>() != 0) {
            return m_elapsedTime;
        }

        if(state_cast<const Running *>() != 0) {
            return m_elapsedTime + std::difftime(std::time(0), m_startTime);
        }

        throw std::bad_cast();
    }

    // m_elapsedTime is only meaningful when the machine is not terminated
    double m_elapsedTime = 0.0;
    // m_startTime is only meaningful when the machine is in Running
    std::time_t m_startTime;
};

struct Active : sc::state<Active, StopWatch, Stopped> {
    typedef sc::transition<EvReset, Active> reactions;

    Active(my_context ctx)
        : my_base(ctx) {
        outermost_context().m_elapsedTime = 0.0;
    }
};

struct Running : sc::state<Running, Active> {
    typedef sc::transition<EvStartStop, Stopped> reactions;

    Running(my_context ctx)
        : my_base(ctx) {
        outermost_context().m_startTime = std::time(0);
    }

    ~Running() { outermost_context().m_elapsedTime += std::difftime(std::time(0), outermost_context().m_startTime); }
};

struct Stopped : sc::simple_state<Stopped, Active> {
    typedef sc::transition<EvStartStop, Running> reactions;
};
}  // namespace BoostExample