#include <iostream>
#include <typeinfo>

#include "BoostSimpleExample.hpp"
#include "Deferral.hpp"
#include "Discard.hpp"
#include "PersonWithMoney.hpp"
#include "Hierarchy.hpp"

using namespace std;

/**
 * @brief test deferral case of ingore/deferal events

λ BoostStateMachine.exe
[STM] [StateMachineDeferral] > Deferral::State::Idle
> IGNORE
[STM] [StateMachineDeferral] Event: Deferral::Event::Ignore
[STM] [StateMachineDeferral] Reaction: Deferral::Event::Ignore in Deferral::State::Idle
[STM] [StateMachineDeferral] Unconsumed event: Deferral::Event::Ignore
> First deferral
[STM] [StateMachineDeferral] Event: Deferral::Event::Other
[STM] [StateMachineDeferral] Reaction: Deferral::Event::Other in Deferral::State::Idle
[STM] [StateMachineDeferral] Deferral: Deferral::Event::Other in Deferral::State::Idle
> Second deferral
[STM] [StateMachineDeferral] Event: Deferral::Event::Deactivate
[STM] [StateMachineDeferral] Reaction: Deferral::Event::Deactivate in Deferral::State::Idle
[STM] [StateMachineDeferral] Deferral: Deferral::Event::Deactivate in Deferral::State::Idle
> ACTIVATE
[STM] [StateMachineDeferral] Event: Deferral::Event::Activate
[STM] [StateMachineDeferral] Reaction: Deferral::Event::Activate in Deferral::State::Idle
[STM] [StateMachineDeferral] < Deferral::State::Idle
[STM] [StateMachineDeferral] > Deferral::State::Active
[STM] [StateMachineDeferral] Reaction: Deferral::Event::Other in Deferral::State::Active
[STM] [StateMachineDeferral] Deferral: Deferral::Event::Other in Deferral::State::Active
[STM] [StateMachineDeferral] Reaction: Deferral::Event::Deactivate in Deferral::State::Active
[STM] [StateMachineDeferral] < Deferral::State::Active
[STM] [StateMachineDeferral] > Deferral::State::Idle
[STM] [StateMachineDeferral] Reaction: Deferral::Event::Other in Deferral::State::Idle
[STM] [StateMachineDeferral] Deferral: Deferral::Event::Other in Deferral::State::Idle
> EXIT
[STM] [StateMachineDeferral] < Deferral::State::Idle
end
 */
void testDeferralCase() {
    Deferral::StateMachine sm;
    // start on Idle
    sm.initiate();
    // ignore has no handling so nothing is done
    cout << "> IGNORE\n";
    sm.process_event(Deferral::Event::Ignore());
    
    // deactivate has no handling on idle, but it was deferral
    cout << "> First deferral\n";
    sm.process_event(Deferral::Event::Other());

    cout << "> Second deferral\n";
    sm.process_event(Deferral::Event::Deactivate());
    
    // activate makes to move to Active state but after this 
    // the deferal state (deactivate) is fired
    cout << "> ACTIVATE\n";
    sm.process_event(Deferral::Event::Activate());
    
    cout << "> EXIT\n";
    sm.terminate();
    cout << "end\n";
}
/**
 * @brief test discard state machine events

λ BoostStateMachine.exe
[STM] [StateMachineDiscard] > Discard::State::Idle
> DEACTIVATE
[STM] [StateMachineDiscard] Event: Discard::Event::Deactivate
[STM] [StateMachineDiscard] Reaction: Discard::Event::Deactivate in Discard::State::Idle
:before process event
[STM] [StateMachineDiscard] Event: Discard::Event::Activate
:after process event and before transit to active
[STM] [StateMachineDiscard] < Discard::State::Idle
[STM] [StateMachineDiscard] > Discard::State::Active
[STM] [StateMachineDiscard] Reaction: Discard::Event::Activate in Discard::State::Active
:before process event
[STM] [StateMachineDiscard] Event: Discard::Event::Deactivate
:after process event handling will be done by this state
[STM] [StateMachineDiscard] Reaction: Discard::Event::Deactivate in Discard::State::Active
[STM] [StateMachineDiscard] < Discard::State::Active
[STM] [StateMachineDiscard] > Discard::State::Idle
> ACTIVATE
[STM] [StateMachineDiscard] Event: Discard::Event::Activate
[STM] [StateMachineDiscard] Reaction: Discard::Event::Activate in Discard::State::Idle
[STM] [StateMachineDiscard] < Discard::State::Idle
[STM] [StateMachineDiscard] > Discard::State::Active
>EXIT
[STM] [StateMachineDiscard] < Discard::State::Active
end
 */
void testDiscard() {
    Discard::StateMachine sm;
    // start on stIdle
    sm.initiate();
    // deactivate will be handled by a custom reaction that will change to itself and then this new state will handle
    // activate event
    cout << "> DEACTIVATE\n";
    sm.process_event(Discard::Event::Deactivate());
    
    // now that we are in the active state and on the custom reaction a discard event is called then the current state
    // will handle the deactivate
    cout << "> ACTIVATE\n";
    sm.process_event(Discard::Event::Activate());

    cout << ">EXIT\n";
    sm.terminate();
    cout << "end\n";
}

/**
 * @brief more complex example

λ BoostStateMachine.exe
>init
[STM] [Person] > Money::State::Happy
[STM] [Person] > Money::State::SomewhatHappy
>going to lose money
[STM] [Person] Event: Money::Event::lostMoney
[STM] [Person] Reaction: Money::Event::lostMoney in Money::State::SomewhatHappy
[STM] [Person] Reaction: Money::Event::lostMoney in Money::State::Happy
[STM] [Person] < Money::State::SomewhatHappy
[STM] [Person] < Money::State::Happy
[STM] [Person] > Money::State::Sad
>going to found money
[STM] [Person] Event: Money::Event::gotMoney
[STM] [Person] Reaction: Money::Event::gotMoney in Money::State::Sad
[STM] [Person] < Money::State::Sad
[STM] [Person] > Money::State::Happy
[STM] [Person] > Money::State::SomewhatHappy
>going to found money
[STM] [Person] Event: Money::Event::gotMoney
[STM] [Person] Reaction: Money::Event::gotMoney in Money::State::SomewhatHappy
[STM] [Person] < Money::State::SomewhatHappy
[STM] [Person] > Money::State::ExtremelyHappy
>going to found money
[STM] [Person] Event: Money::Event::gotMoney
[STM] [Person] Reaction: Money::Event::gotMoney in Money::State::ExtremelyHappy
i'm rich...
[STM] [Person] Event: Money::Event::lostMoney
in rich -> sad state
[STM] [Person] < Money::State::ExtremelyHappy
[STM] [Person] < Money::State::Happy
[STM] [Person] > Money::State::Sad
[STM] [Person] Reaction: Money::Event::lostMoney in Money::State::Sad
[STM] [Person] Unconsumed event: Money::Event::lostMoney
[STM] [Person] < Money::State::Sad
end
 */
void testPersonSTM() {
    Money::StateMachine sm;
    cout << ">init\n";
    sm.initiate();  // go directly to SomewhatHappyState, but HappyState is created because is the first state
    cout << ">going to lose money\n";
    sm.process_event(Money::Event::lostMoney());  // go to sad state
    cout << ">going to found money\n";
    sm.process_event(Money::Event::gotMoney());  // go to SomewhatHappyState
    cout << ">going to found money\n";
    sm.process_event(Money::Event::gotMoney());  // go to ExtremelyHappyState
    cout << ">going to found money\n";
    sm.process_event(Money::Event::gotMoney());  // can't be more happy
    sm.terminate();
    cout << "end\n";
}

/**
 * @brief example using base states that work like class virtual methods
 * 
λ BoostStateMachine.exe
>init
[STM] [Hierarchy] > Hierarchy::State::Stopped
>increase speed -> walking
[STM] [Hierarchy] Event: Hierarchy::Event::SpeedUp
[STM] [Hierarchy] Reaction: Hierarchy::Event::SpeedUp in Hierarchy::State::Stopped
[STM] [Hierarchy] < Hierarchy::State::Stopped
[STM] [Hierarchy] > Hierarchy::State::Moving
[STM] [Hierarchy] > Hierarchy::State::Walking
>increase speed -> running
[STM] [Hierarchy] Event: Hierarchy::Event::SpeedUp
[STM] [Hierarchy] Reaction: Hierarchy::Event::SpeedUp in Hierarchy::State::Walking
[STM] [Hierarchy] < Hierarchy::State::Walking
[STM] [Hierarchy] > Hierarchy::State::Running
>increase speed -> sprinting
[STM] [Hierarchy] Event: Hierarchy::Event::SpeedUp
[STM] [Hierarchy] Reaction: Hierarchy::Event::SpeedUp in Hierarchy::State::Running
[STM] [Hierarchy] < Hierarchy::State::Running
[STM] [Hierarchy] > Hierarchy::State::Sprinting
>hard stop
[STM] [Hierarchy] Event: Hierarchy::Event::Stop
[STM] [Hierarchy] Reaction: Hierarchy::Event::Stop in Hierarchy::State::Sprinting
:can not stop at this state...
[STM] [Hierarchy] Event: Hierarchy::Event::Stop
[STM] [Hierarchy] < Hierarchy::State::Sprinting
[STM] [Hierarchy] > Hierarchy::State::Running
[STM] [Hierarchy] Reaction: Hierarchy::Event::Stop in Hierarchy::State::Running
[STM] [Hierarchy] Reaction: Hierarchy::Event::Stop in Hierarchy::State::Moving
[STM] [Hierarchy] < Hierarchy::State::Running
[STM] [Hierarchy] < Hierarchy::State::Moving
[STM] [Hierarchy] > Hierarchy::State::Stopped
>increase speed -> walking
[STM] [Hierarchy] Event: Hierarchy::Event::SpeedUp
[STM] [Hierarchy] Reaction: Hierarchy::Event::SpeedUp in Hierarchy::State::Stopped
[STM] [Hierarchy] < Hierarchy::State::Stopped
[STM] [Hierarchy] > Hierarchy::State::Moving
[STM] [Hierarchy] > Hierarchy::State::Walking
>normal stop
[STM] [Hierarchy] Event: Hierarchy::Event::Stop
[STM] [Hierarchy] Reaction: Hierarchy::Event::Stop in Hierarchy::State::Walking
[STM] [Hierarchy] Reaction: Hierarchy::Event::Stop in Hierarchy::State::Moving
[STM] [Hierarchy] < Hierarchy::State::Walking
[STM] [Hierarchy] < Hierarchy::State::Moving
[STM] [Hierarchy] > Hierarchy::State::Stopped
[STM] [Hierarchy] < Hierarchy::State::Stopped
end
 */
void testHierarchy(){
    Hierarchy::StateMachine sm;

    cout << ">init\n";
    sm.initiate();

    cout << ">increase speed -> walking\n";
    sm.process_event(Hierarchy::Event::SpeedUp());

    cout << ">increase speed -> running\n";
    sm.process_event(Hierarchy::Event::SpeedUp());

    cout << ">increase speed -> sprinting\n";
    sm.process_event(Hierarchy::Event::SpeedUp());

    cout << ">hard stop\n";
    sm.process_event(Hierarchy::Event::Stop());

    cout << ">increase speed -> walking\n";
    sm.process_event(Hierarchy::Event::SpeedUp());

    cout << ">normal stop\n";
    sm.process_event(Hierarchy::Event::Stop());

    sm.terminate();
    cout << "end\n";
}

int main(int argc, char *argv[]) {
    
    testPersonSTM();
    testDeferralCase();
    testDiscard();
    testHierarchy();
    
    return 0;
}