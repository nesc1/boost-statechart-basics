#pragma once
#include <iostream>

#include "smcommon.h"

namespace Money {
using namespace std;
namespace smutil = Utils::StateMachine;
namespace sc     = boost::statechart;
namespace mpl    = boost::mpl;

// states
namespace State {
struct Happy;
struct Sad;
struct ExtremelyHappy;
struct SomewhatHappy;
}  // namespace State

// events
namespace Event {
struct gotMoney : smutil::Event<gotMoney> {};
struct lostMoney : smutil::Event<lostMoney> {};
}  // namespace Event

// state machine
struct StateMachine : smutil::StateMachine<StateMachine, State::Happy> {
   public:
    StateMachine()
        : smutil::StateMachine<StateMachine, State::Happy>("Person") {}
};

// state implementation
namespace State {
struct Sad : smutil::State<Sad, StateMachine> {
    Sad(my_context ctx)
        : smutil::State<Sad, StateMachine>(ctx) {}

    typedef sc::transition<Event::gotMoney, Happy> reactions;
};
struct Happy : smutil::State<Happy, StateMachine, SomewhatHappy> {
    Happy(my_context ctx)
        : smutil::State<Happy, StateMachine, SomewhatHappy>(ctx) {}

    typedef mpl::list<

        sc::transition<Event::lostMoney, Sad>,
        sc::transition<Event::gotMoney, SomewhatHappy>

        >
        reactions;
};
struct SomewhatHappy : smutil::State<SomewhatHappy, Happy> {
    SomewhatHappy(my_context ctx)
        : smutil::State<SomewhatHappy, Happy>(ctx) {}

    // sc::transition<lostMoney, Sad> is not necessary because the base already declare it
    typedef sc::transition<Event::gotMoney, ExtremelyHappy> reactions;
};
struct ExtremelyHappy : smutil::State<ExtremelyHappy, Happy> {
    ExtremelyHappy(my_context ctx)
        : smutil::State<ExtremelyHappy, Happy>(ctx) {}

    // deferral = postponed
    typedef mpl::list<

        sc::deferral<Event::lostMoney>,
        sc::custom_reaction<Event::gotMoney>

        >
        reactions;
    // sc::result                                                                       react(const lostMoney&) {
    //    cout << "got lost money... not so happy any more\n";
    //    return transit<SomewhatHappy>();
    //}

    /**
     * @brief react example
     *
     * @return sc::result
     *
     * in this react we can see that process event will be processed after this function is processed
     * so if we are on the same state (discard_event) then this state will handle the lostMoney, but
     * if we transit to another state then is that state that will handle the event lostMoney
     *
     * if the current state does not handle the event the base state may handle it and recursivelly
     *
     * if nothing is said about the event handling on the state then an unconsumed event is trigger
     * example of this is the following the SadState (next state) does not process the lostMoney event
     * so to avoid unconsumed events you may add sc::deferral<lostMoney> on SadState
     */
    sc::result react(const Event::gotMoney &) {
        cout << "i'm rich...\n";
        outermost_context().process_event(Event::lostMoney());
        cout << "in rich -> sad state\n";
        // return discard_event();
        return transit<Sad>();
    }
};
}  // namespace State
}  // namespace Money