#pragma once
#include "smcommon.h"
#include <functional>
#include <utility>

namespace commandqueue {

using namespace std;
namespace smutil = utils::statemachine;
namespace sc     = boost::statechart;
namespace mpl    = boost::mpl;

// possible states
namespace state {
struct Init;
struct Idle;
struct Busy;
}  // namespace state

// possible events
namespace event {
using Command = std::function<void()>;

struct EvStart : smutil::Event<EvStart> {};
struct EvStop : smutil::Event<EvStop> {};
struct EvExecute : smutil::Event<EvExecute> {
    explicit EvExecute(Command cmd)
        : m_command(std::move(cmd)) {}

    void execute() const {
        if(m_command) {
            m_command();
        }
    }

   private:
    Command m_command;
};
struct EvFinished : smutil::Event<EvFinished> {};
}  // namespace event


// state machine
struct StateMachine : smutil::StateMachine<StateMachine, state::Init> {
   public:
    StateMachine()
        : smutil::StateMachine<StateMachine, state::Init>("StateMachineCommandQueue") {}
};

// state implementation
namespace state {

/// init state
struct Init : smutil::State<Init, StateMachine> {
    explicit Init(my_context ctx)
        : smutil::State<Init, StateMachine>(std::move(ctx)) {}
    // clang-format off
    using reactions = mpl::list<
        sc::transition<event::EvStart, Idle>,
        sc::deferral<event::EvStop>,
        sc::deferral<event::EvExecute>,
        sc::deferral<event::EvFinished>
    >;
    // clang-format on
};

/// Idle state
struct Idle : smutil::State<Idle, StateMachine> {
    explicit Idle(my_context ctx)
        : smutil::State<Idle, StateMachine>(std::move(ctx)) {}
    // clang-format off
    using reactions = mpl::list<
        sc::deferral<event::EvStart>,
        sc::transition<event::EvStop, Init>,
        sc::custom_reaction<event::EvExecute>,
        sc::deferral<event::EvFinished>
    >;
    // clang-format on
    sc::result react(const event::EvExecute& event) {
        event.execute();
        return transit<Busy>();
    }
};

/// Busy state
struct Busy : smutil::State<Busy, StateMachine> {
    explicit Busy(my_context ctx)
        : smutil::State<Busy, StateMachine>(std::move(ctx)) {}
    // clang-format off
    using reactions = mpl::list<
        sc::deferral<event::EvStart>,
        sc::deferral<event::EvStop>,
        sc::deferral<event::EvExecute>,
        sc::transition<event::EvFinished, Idle>
    >;
    // clang-format on
};

}  // namespace state

}  // namespace commandqueue