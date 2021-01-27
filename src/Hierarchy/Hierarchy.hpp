#pragma once
#include <iostream>
#include <utility>

#include "smcommon.h"

using namespace std;

namespace hierarchy {

namespace smutil = utils::statemachine;
namespace sc     = boost::statechart;
namespace mpl    = boost::mpl;

// states (fwd)
namespace state {
struct Stopped;
struct Moving;
struct Walking;
struct Running;
struct Sprinting;
}  // namespace state


// events
namespace event {
struct SpeedUp : smutil::Event<SpeedUp> {};
struct SpeedDown : smutil::Event<SpeedDown> {};
struct Stop : smutil::Event<Stop> {};
}  // namespace event

// state machine
struct StateMachine : smutil::StateMachine<StateMachine, state::Stopped> {
   public:
    StateMachine()
        : smutil::StateMachine<StateMachine, state::Stopped>("hierarchy") {}
};

// state implementation
namespace state {
struct Stopped : smutil::State<Stopped, StateMachine> {
    explicit Stopped(my_context ctx)
        : smutil::State<Stopped, StateMachine>(ctx) {}

    // clang-format off
    using reactions = mpl::list<
        sc::transition<event::SpeedUp, Walking>
    >;
    // clang-format on
};

/// base state that is moving
struct Moving : smutil::State<Moving, StateMachine, Walking> {
    explicit Moving(my_context ctx)
        : smutil::State<Moving, StateMachine, Walking>(std::move(ctx)) {}

    // clang-format off
    using reactions = mpl::list<
        sc::transition<event::Stop, Stopped>
    >;
    // clang-format on
};

/// child state of moving that is walking
struct Walking : smutil::State<Walking, Moving> {
    explicit Walking(my_context ctx)
        : smutil::State<Walking, Moving>(std::move(ctx)) {}

    // clang-format off
    using reactions = mpl::list<
        sc::transition<event::SpeedDown, Stopped>,
        sc::transition<event::SpeedUp, Running>
    >;
    // clang-format on
};

/// child state of moving that is running
struct Running : smutil::State<Running, Moving> {
    explicit Running(my_context ctx)
        : smutil::State<Running, Moving>(std::move(ctx)) {}

    // clang-format off
    using reactions = mpl::list<
        sc::transition<event::SpeedDown, Walking>,
        sc::transition<event::SpeedUp, Sprinting>
    >;
    // clang-format on
};

/// child state of moving that is sprinting
struct Sprinting : smutil::State<Sprinting, Moving> {
    explicit Sprinting(my_context ctx)
        : smutil::State<Sprinting, Moving>(std::move(ctx)) {}

    // clang-format off
    using reactions = mpl::list<
        sc::transition<event::SpeedDown, Running>,
        sc::custom_reaction<event::SpeedUp>,
        sc::custom_reaction<event::Stop>
    >;
    // clang-format on

    sc::result react(const event::Stop &) {
        cout << ":can not stop at this state...\n";
        outermost_context().process_event(event::Stop());
        return transit<Running>();
    }
    sc::result react(const event::SpeedUp &) {
        cout << ":not possible to increase more speed...\n";
        return discard_event();
    }
};


}  // namespace state
}  // namespace hierarchy
