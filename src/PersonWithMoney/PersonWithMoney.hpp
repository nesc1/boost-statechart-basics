#pragma once
#include <iostream>

#include "smcommon.h"

namespace money {
using namespace std;
namespace smutil = utils::statemachine;
namespace sc     = boost::statechart;
namespace mpl    = boost::mpl;

// states
namespace state {
struct Happy;
struct Sad;
struct ExtremelyHappy;
struct SomewhatHappy;
}  // namespace state

// events
namespace event {
struct GotMoney : smutil::Event<GotMoney> {};
struct LostMoney : smutil::Event<LostMoney> {};
}  // namespace event

// state machine
struct StateMachine : smutil::StateMachine<StateMachine, state::Happy> {
   public:
    StateMachine()
        : smutil::StateMachine<StateMachine, state::Happy>("Person") {}
};

// state implementation
namespace state {

struct Sad : smutil::State<Sad, StateMachine> {
    explicit Sad(my_context ctx)
        : smutil::State<Sad, StateMachine>(ctx) {}

    using reactions = sc::transition<event::GotMoney, Happy>;
};

struct Happy : smutil::State<Happy, StateMachine, SomewhatHappy> {
    explicit Happy(my_context ctx)
        : smutil::State<Happy, StateMachine, SomewhatHappy>(ctx) {}

    // clang-format off
    using reactions = mpl::list<
        sc::transition<event::LostMoney, Sad>,
        sc::transition<event::GotMoney, SomewhatHappy>
    >;
    // clang-format on
};

struct SomewhatHappy : smutil::State<SomewhatHappy, Happy> {
    explicit SomewhatHappy(my_context ctx)
        : smutil::State<SomewhatHappy, Happy>(ctx) {}

    // sc::transition<LostMoney, Sad> is not necessary because the base already declare it
    using reactions = sc::transition<event::GotMoney, ExtremelyHappy>;
};

struct ExtremelyHappy : smutil::State<ExtremelyHappy, Happy> {
    explicit ExtremelyHappy(my_context ctx)
        : smutil::State<ExtremelyHappy, Happy>(ctx) {}

    // deferral = postponed
    // clang-format off
    using reactions = mpl::list<
        sc::deferral<event::LostMoney>,
        sc::custom_reaction<event::GotMoney>
    >;
    // clang-format on

    // sc::result                                                                       react(const LostMoney&) {
    //    cout << "got lost money... not so happy any more\n";
    //    return transit<SomewhatHappy>();
    //}

    /**
     * @brief react example
     *
     * @return sc::result
     *
     * in this react we can see that process event will be processed after this function is processed
     * so if we are on the same state (discard_event) then this state will handle the LostMoney, but
     * if we transit to another state then is that state that will handle the event LostMoney
     *
     * if the current state does not handle the event the base state may handle it and recursivelly
     *
     * if nothing is said about the event handling on the state then an unconsumed event is trigger
     * example of this is the following the SadState (next state) does not process the LostMoney event
     * so to avoid unconsumed events you may add sc::deferral<LostMoney> on SadState
     */
    sc::result react(const event::GotMoney &) {
        cout << "i'm rich...\n";
        outermost_context().process_event(event::LostMoney());
        cout << "in rich -> sad state\n";
        // return discard_event();
        return transit<Sad>();
    }
};

}  // namespace state
}  // namespace money