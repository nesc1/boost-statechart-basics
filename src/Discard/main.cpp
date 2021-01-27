#include <iostream>
#include <typeinfo>

#include "Discard.hpp"

using namespace std;

/**
 * @brief test discard state machine events

λ BoostStateMachine.exe
[STM] [StateMachineDiscard] > Discard::state::Idle
> DEACTIVATE
[STM] [StateMachineDiscard] event: Discard::event::Deactivate
[STM] [StateMachineDiscard] Reaction: Discard::event::Deactivate in Discard::state::Idle
:before process event
[STM] [StateMachineDiscard] event: Discard::event::Activate
:after process event and before transit to active
[STM] [StateMachineDiscard] < Discard::state::Idle
[STM] [StateMachineDiscard] > Discard::state::Active
[STM] [StateMachineDiscard] Reaction: Discard::event::Activate in Discard::state::Active
:before process event
[STM] [StateMachineDiscard] event: Discard::event::Deactivate
:after process event handling will be done by this state
[STM] [StateMachineDiscard] Reaction: Discard::event::Deactivate in Discard::state::Active
[STM] [StateMachineDiscard] < Discard::state::Active
[STM] [StateMachineDiscard] > Discard::state::Idle
> ACTIVATE
[STM] [StateMachineDiscard] event: Discard::event::Activate
[STM] [StateMachineDiscard] Reaction: Discard::event::Activate in Discard::state::Idle
[STM] [StateMachineDiscard] < Discard::state::Idle
[STM] [StateMachineDiscard] > Discard::state::Active
>EXIT
[STM] [StateMachineDiscard] < Discard::state::Active
end
 */
int main(int argc, char *argv[]) {
    discard::StateMachine sm;
    // start on stIdle
    sm.initiate();
    // deactivate will be handled by a custom reaction that will change to itself and then this new state will handle
    // activate event
    cout << "> DEACTIVATE\n";
    sm.process_event(discard::event::Deactivate());

    // now that we are in the active state and on the custom reaction a discard event is called then the current state
    // will handle the deactivate
    cout << "> ACTIVATE\n";
    sm.process_event(discard::event::Activate());

    cout << ">EXIT\n";
    sm.terminate();
    cout << "end\n";

    return 0;
}