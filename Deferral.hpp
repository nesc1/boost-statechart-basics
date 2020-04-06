#pragma once
#include <iostream>

#include "smcommon.h"

namespace Deferral {
using namespace std;
namespace smutil = Utils::StateMachine;
namespace sc  = boost::statechart;
namespace mpl = boost::mpl;


// state forwrding
namespace State {
struct Idle;
struct Active;
}  // namespace State

// events
namespace Event {
struct Activate : smutil::Event<Activate> {};
struct Deactivate : smutil::Event<Deactivate> {};
struct Ignore : smutil::Event<Ignore> {};
struct Other : smutil::Event<Other> {};
}  // namespace Event

// state machine
struct StateMachine : smutil::StateMachine<StateMachine, State::Idle> {
   public:
    StateMachine()
        : smutil::StateMachine<StateMachine, State::Idle>("StateMachineDeferral") {}
};

// state implementation
namespace State {
struct Idle : smutil::State<Idle, StateMachine> {
    Idle(my_context ctx)
        : smutil::State<Idle, StateMachine>(ctx) {}

    typedef mpl::list<

        sc::deferral<Event::Deactivate>,
        sc::deferral<Event::Other>,
        sc::transition<Event::Activate, Active>

        >
        reactions;
};
struct Active : smutil::State<Active, StateMachine> {
    Active(my_context ctx)
        : smutil::State<Active, StateMachine>(ctx) {}

    typedef mpl::list<

        sc::deferral<Event::Activate>,
        sc::deferral<Event::Other>,
        sc::transition<Event::Deactivate, Idle>

        >
        reactions;
};
}  // namespace State
}  // namespace Deferral