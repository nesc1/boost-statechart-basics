#pragma once
#include <iostream>

#include "smcommon.h"

namespace deferral {
using namespace std;
namespace smutil = utils::statemachine;
namespace sc     = boost::statechart;
namespace mpl    = boost::mpl;


// state forwrding
namespace state {
struct Idle;
struct Active;
}  // namespace state

// events
namespace event {
struct Activate : smutil::Event<Activate> {};
struct Deactivate : smutil::Event<Deactivate> {};
struct Ignore : smutil::Event<Ignore> {};
struct Other : smutil::Event<Other> {};
}  // namespace event

// state machine
struct StateMachine : smutil::StateMachine<StateMachine, state::Idle> {
   public:
    StateMachine()
        : smutil::StateMachine<StateMachine, state::Idle>("StateMachineDeferral") {}
};

// state implementation
namespace state {
struct Idle : smutil::State<Idle, StateMachine> {
    explicit Idle(my_context ctx)
        : smutil::State<Idle, StateMachine>(ctx) {}

    // clang-format off
    using reactions = mpl::list<
        sc::deferral<event::Deactivate>,
        sc::deferral<event::Other>,
        sc::transition<event::Activate, Active>
    >;
    // clang-format on
};
struct Active : smutil::State<Active, StateMachine> {
    explicit Active(my_context ctx)
        : smutil::State<Active, StateMachine>(ctx) {}

    // clang-format off
    using reactions = mpl::list<
        sc::deferral<event::Activate>,
        sc::deferral<event::Other>,
        sc::transition<event::Deactivate, Idle>
    >;
    // clang-format on
};
}  // namespace state
}  // namespace deferral