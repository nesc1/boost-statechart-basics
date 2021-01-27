#include <iostream>
#include <typeinfo>

#include "Deferral.hpp"

using namespace std;

/**
 * @brief test deferral case of ingore/deferal events

λ BoostStateMachine.exe
[STM] [StateMachineDeferral] > Deferral::state::Idle
> IGNORE
[STM] [StateMachineDeferral] event: Deferral::event::Ignore
[STM] [StateMachineDeferral] Reaction: Deferral::event::Ignore in Deferral::state::Idle
[STM] [StateMachineDeferral] Unconsumed event: Deferral::event::Ignore
> First deferral
[STM] [StateMachineDeferral] event: Deferral::event::Other
[STM] [StateMachineDeferral] Reaction: Deferral::event::Other in Deferral::state::Idle
[STM] [StateMachineDeferral] Deferral: Deferral::event::Other in Deferral::state::Idle
> Second deferral
[STM] [StateMachineDeferral] event: Deferral::event::Deactivate
[STM] [StateMachineDeferral] Reaction: Deferral::event::Deactivate in Deferral::state::Idle
[STM] [StateMachineDeferral] Deferral: Deferral::event::Deactivate in Deferral::state::Idle
> ACTIVATE
[STM] [StateMachineDeferral] event: Deferral::event::Activate
[STM] [StateMachineDeferral] Reaction: Deferral::event::Activate in Deferral::state::Idle
[STM] [StateMachineDeferral] < Deferral::state::Idle
[STM] [StateMachineDeferral] > Deferral::state::Active
[STM] [StateMachineDeferral] Reaction: Deferral::event::Other in Deferral::state::Active
[STM] [StateMachineDeferral] Deferral: Deferral::event::Other in Deferral::state::Active
[STM] [StateMachineDeferral] Reaction: Deferral::event::Deactivate in Deferral::state::Active
[STM] [StateMachineDeferral] < Deferral::state::Active
[STM] [StateMachineDeferral] > Deferral::state::Idle
[STM] [StateMachineDeferral] Reaction: Deferral::event::Other in Deferral::state::Idle
[STM] [StateMachineDeferral] Deferral: Deferral::event::Other in Deferral::state::Idle
> EXIT
[STM] [StateMachineDeferral] < Deferral::state::Idle
end
 */
int main(int argc, char *argv[]) {
    deferral::StateMachine sm;
    // start on Idle
    sm.initiate();
    // ignore has no handling so nothing is done
    cout << "> IGNORE\n";
    sm.process_event(deferral::event::Ignore());

    // deactivate has no handling on idle, but it was deferral
    cout << "> First deferral\n";
    sm.process_event(deferral::event::Other());

    cout << "> Second deferral\n";
    sm.process_event(deferral::event::Deactivate());

    // activate makes to move to Active state but after this
    // the deferal state (deactivate) is fired
    cout << "> ACTIVATE\n";
    sm.process_event(deferral::event::Activate());

    cout << "> EXIT\n";
    sm.terminate();
    cout << "end\n";

    return 0;
}