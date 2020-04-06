#pragma once
#include <iostream>

#include "smcommon.h"

namespace Discard {
using namespace std;
namespace smutil = Utils::StateMachine;
namespace sc  = boost::statechart;
namespace mpl = boost::mpl;

// state forwrding
namespace State {
struct Idle;
struct Active;
}

// events
namespace Event {
struct Activate : smutil::Event<Activate> {};
struct Deactivate : smutil::Event<Deactivate> {};
struct Ignore : smutil::Event<Ignore> {};
}

// state machine
struct StateMachine : smutil::StateMachine<StateMachine, State::Idle> {
   public:
    StateMachine()
        : smutil::StateMachine<StateMachine, State::Idle>("StateMachineDiscard") {}
};

// state implementation
namespace State {
struct Idle : smutil::State<Idle, StateMachine> {
    Idle(my_context ctx)
        : smutil::State<Idle, StateMachine>(ctx) {}

    typedef mpl::list<

        sc::custom_reaction<Event::Deactivate>,
        sc::transition<Event::Activate, Active>

        >
        reactions;

    sc::result react(const Event::Deactivate &) {
        cout << ":before process event\n";
        // this event will be handled by the transit state
        outermost_context().process_event(Event::Activate());
        cout << ":after process event and before transit to active\n";
        // this is executed first (state is exited and entered again)
        return transit<Active>();
    }
};
struct Active : smutil::State<Active, StateMachine> {
    Active(my_context ctx)
        : smutil::State<Active, StateMachine>(ctx) {}

    typedef mpl::list<

        sc::custom_reaction<Event::Activate>,
        sc::transition<Event::Deactivate, Idle>

        >
        reactions;

    sc::result react(const Event::Activate &) {
        cout << ":before process event\n";
        // this event will be handled by the transit state
        outermost_context().process_event(Event::Deactivate());
        cout << ":after process event handling will be done by this state\n";
        return discard_event();
    }
};
}
}  // namespace Discard