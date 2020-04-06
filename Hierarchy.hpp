#pragma once
#include <iostream>

#include "smcommon.h"

using namespace std;

namespace Hierarchy {

namespace smutil = Utils::StateMachine;
namespace sc     = boost::statechart;
namespace mpl    = boost::mpl;

// states (fwd)
namespace State {
struct Stopped;
struct Moving;
struct Walking;
struct Running;
struct Sprinting;
}  // namespace State


// events
namespace Event {
struct SpeedUp : smutil::Event<SpeedUp> {};
struct SpeedDown : smutil::Event<SpeedDown> {};
struct Stop : smutil::Event<Stop> {};
}  // namespace Event

// state machine
struct StateMachine : smutil::StateMachine<StateMachine, State::Stopped> {
   public:
    StateMachine()
        : smutil::StateMachine<StateMachine, State::Stopped>("Hierarchy") {}
};

// state implementation
namespace State {
struct Stopped : smutil::State<Stopped, StateMachine> {
    Stopped(my_context ctx)
        : smutil::State<Stopped, StateMachine>(ctx) {}

    typedef mpl::list<

        // discard Event::Stop and Event::SpeedDown (we are already stopped)
        sc::transition<Event::SpeedUp, Walking>

        >
        reactions;
};

struct Moving : smutil::State<Moving, StateMachine, Walking> {
    Moving(my_context ctx)
        : smutil::State<Moving, StateMachine, Walking>(ctx) {}

    typedef mpl::list<

        // all states should respond to imeditly stop when call to stop
        sc::transition<Event::Stop, Stopped>

        >
        reactions;
};

struct Walking : smutil::State<Walking, Moving> {
    Walking(my_context ctx)
        : smutil::State<Walking, Moving>(ctx) {}

    typedef mpl::list<

        sc::transition<Event::SpeedDown, Stopped>,
        sc::transition<Event::SpeedUp, Running>

        >
        reactions;
};

struct Running : smutil::State<Running, Moving> {
    Running(my_context ctx)
        : smutil::State<Running, Moving>(ctx) {}

    typedef mpl::list<

        sc::transition<Event::SpeedDown, Walking>,
        sc::transition<Event::SpeedUp, Sprinting>

        >
        reactions;
};

struct Sprinting : smutil::State<Sprinting, Moving> {
    Sprinting(my_context ctx)
        : smutil::State<Sprinting, Moving>(ctx) {}

    typedef mpl::list<

        sc::transition<Event::SpeedDown, Running>,
        sc::custom_reaction<Event::SpeedUp>,
        sc::custom_reaction<Event::Stop>

        >
        reactions;

    sc::result react(const Event::Stop &) {
        cout << ":can not stop at this state...\n";
        outermost_context().process_event(Event::Stop());
        return transit<Running>();
    }
    sc::result react(const Event::SpeedUp &) {
        cout << ":not possible to increase more speed...\n";
        return discard_event();
    }
};


}  // namespace State
}  // namespace Hierarchy
