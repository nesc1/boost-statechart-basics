#include <iostream>
#include <typeinfo>

#include "PersonWithMoney.hpp"

using namespace std;

/**
 * @brief more complex example

λ BoostStateMachine.exe
>init
[STM] [Person] > Money::state::Happy
[STM] [Person] > Money::state::SomewhatHappy
>going to lose money
[STM] [Person] event: Money::event::LostMoney
[STM] [Person] Reaction: Money::event::LostMoney in Money::state::SomewhatHappy
[STM] [Person] Reaction: Money::event::LostMoney in Money::state::Happy
[STM] [Person] < Money::state::SomewhatHappy
[STM] [Person] < Money::state::Happy
[STM] [Person] > Money::state::Sad
>going to found money
[STM] [Person] event: Money::event::GotMoney
[STM] [Person] Reaction: Money::event::GotMoney in Money::state::Sad
[STM] [Person] < Money::state::Sad
[STM] [Person] > Money::state::Happy
[STM] [Person] > Money::state::SomewhatHappy
>going to found money
[STM] [Person] event: Money::event::GotMoney
[STM] [Person] Reaction: Money::event::GotMoney in Money::state::SomewhatHappy
[STM] [Person] < Money::state::SomewhatHappy
[STM] [Person] > Money::state::ExtremelyHappy
>going to found money
[STM] [Person] event: Money::event::GotMoney
[STM] [Person] Reaction: Money::event::GotMoney in Money::state::ExtremelyHappy
i'm rich...
[STM] [Person] event: Money::event::LostMoney
in rich -> sad state
[STM] [Person] < Money::state::ExtremelyHappy
[STM] [Person] < Money::state::Happy
[STM] [Person] > Money::state::Sad
[STM] [Person] Reaction: Money::event::LostMoney in Money::state::Sad
[STM] [Person] Unconsumed event: Money::event::LostMoney
[STM] [Person] < Money::state::Sad
end
 */
int main(int argc, char *argv[]) {
    money::StateMachine sm;
    cout << ">init\n";
    sm.initiate();  // go directly to SomewhatHappyState, but HappyState is created because is the first state
    cout << ">going to lose money\n";
    sm.process_event(money::event::LostMoney());  // go to sad state
    cout << ">going to found money\n";
    sm.process_event(money::event::GotMoney());  // go to SomewhatHappyState
    cout << ">going to found money\n";
    sm.process_event(money::event::GotMoney());  // go to ExtremelyHappyState
    cout << ">going to found money\n";
    sm.process_event(money::event::GotMoney());  // can't be more happy
    sm.terminate();
    cout << "end\n";

    return 0;
}