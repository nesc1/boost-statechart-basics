#pragma once
#include <iostream>

#include "smcommon.h"

namespace discard {
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
}  // namespace event

// state machine
struct StateMachine : smutil::StateMachine<StateMachine, state::Idle> {
   public:
    StateMachine()
        : smutil::StateMachine<StateMachine, state::Idle>("StateMachineDiscard") {}
};

// state implementation
namespace state {

struct Idle : smutil::State<Idle, StateMachine> {
    explicit Idle(my_context ctx)
        : smutil::State<Idle, StateMachine>(ctx) {}

    // clang-format off
    using reactions = mpl::list<
        sc::custom_reaction<event::Deactivate>,
        sc::transition<event::Activate, Active>
    >;
    // clang-format on

    sc::result react(const event::Deactivate &) {
        cout << ":before process event\n";
        // this event will be handled by the transit state
        outermost_context().process_event(event::Activate());
        cout << ":after process event and before transit to active\n";
        // this is executed first (state is exited and entered again)
        return transit<Active>();
    }
};

struct Active : smutil::State<Active, StateMachine> {
    explicit Active(my_context ctx)
        : smutil::State<Active, StateMachine>(ctx) {}

    // clang-format off
    using reactions = mpl::list<
        sc::custom_reaction<event::Activate>,
        sc::transition<event::Deactivate, Idle>
    >;
    // clang-format on

    sc::result react(const event::Activate &) {
        cout << ":before process event\n";
        // this event will be handled by the transit state
        outermost_context().process_event(event::Deactivate());
        cout << ":after process event handling will be done by this state\n";
        return discard_event();
    }
};

}  // namespace state
}  // namespace discard