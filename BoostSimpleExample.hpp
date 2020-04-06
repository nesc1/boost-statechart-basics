#pragma once
#include "smcommon.h"

namespace BoostExample {
using namespace std;
namespace sc  = boost::statechart;
namespace mpl = boost::mpl;
///////////////////////////////////////////////////7
/// another example (simple)
struct EvReset : sc::event<EvReset> {};
struct EvStartStop : sc::event<EvStartStop> {};

struct Active;
struct Stopped;
struct Running;
struct StopWatch : sc::state_machine<StopWatch, Active> {
    // startTime_ remains uninitialized, because there is no reasonable default
    StopWatch()
        : elapsedTime_(0.0) {}
    ~StopWatch() { terminate(); }

    double ElapsedTime() const {
        // Ugly switch over the current state.
        if(state_cast<const Stopped *>() != 0) {
            return elapsedTime_;
        } else if(state_cast<const Running *>() != 0) {
            return elapsedTime_ + std::difftime(std::time(0), startTime_);
        } else  // we're terminated
        {
            throw std::bad_cast();
        }
    }

    // elapsedTime_ is only meaningful when the machine is not terminated
    double elapsedTime_;
    // startTime_ is only meaningful when the machine is in Running
    std::time_t startTime_;
};

struct Active : sc::state<Active, StopWatch, Stopped> {
    typedef sc::transition<EvReset, Active> reactions;

    Active(my_context ctx)
        : my_base(ctx) {
        outermost_context().elapsedTime_ = 0.0;
    }
};

struct Running : sc::state<Running, Active> {
    typedef sc::transition<EvStartStop, Stopped> reactions;

    Running(my_context ctx)
        : my_base(ctx) {
        outermost_context().startTime_ = std::time(0);
    }

    ~Running() { outermost_context().elapsedTime_ += std::difftime(std::time(0), outermost_context().startTime_); }
};

struct Stopped : sc::simple_state<Stopped, Active> {
    typedef sc::transition<EvStartStop, Running> reactions;
};
}  // namespace BoostExample