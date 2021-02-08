#include <iostream>
#include <typeinfo>

#include "Hierarchy.hpp"

using namespace std;

/**
 * @brief example using base states that work like class virtual methods
 *
λ BoostStateMachine.exe
>init
[STM] [hierarchy] > hierarchy::state::Stopped
>increase speed -> walking
[STM] [hierarchy] event: hierarchy::event::SpeedUp
[STM] [hierarchy] Reaction: hierarchy::event::SpeedUp in hierarchy::state::Stopped
[STM] [hierarchy] < hierarchy::state::Stopped
[STM] [hierarchy] > hierarchy::state::Moving
[STM] [hierarchy] > hierarchy::state::Walking
>increase speed -> running
[STM] [hierarchy] event: hierarchy::event::SpeedUp
[STM] [hierarchy] Reaction: hierarchy::event::SpeedUp in hierarchy::state::Walking
[STM] [hierarchy] < hierarchy::state::Walking
[STM] [hierarchy] > hierarchy::state::Running
>increase speed -> sprinting
[STM] [hierarchy] event: hierarchy::event::SpeedUp
[STM] [hierarchy] Reaction: hierarchy::event::SpeedUp in hierarchy::state::Running
[STM] [hierarchy] < hierarchy::state::Running
[STM] [hierarchy] > hierarchy::state::Sprinting
>hard stop
[STM] [hierarchy] event: hierarchy::event::Stop
[STM] [hierarchy] Reaction: hierarchy::event::Stop in hierarchy::state::Sprinting
:can not stop at this state...
[STM] [hierarchy] event: hierarchy::event::Stop
[STM] [hierarchy] < hierarchy::state::Sprinting
[STM] [hierarchy] > hierarchy::state::Running
[STM] [hierarchy] Reaction: hierarchy::event::Stop in hierarchy::state::Running
[STM] [hierarchy] Reaction: hierarchy::event::Stop in hierarchy::state::Moving
[STM] [hierarchy] < hierarchy::state::Running
[STM] [hierarchy] < hierarchy::state::Moving
[STM] [hierarchy] > hierarchy::state::Stopped
>increase speed -> walking
[STM] [hierarchy] event: hierarchy::event::SpeedUp
[STM] [hierarchy] Reaction: hierarchy::event::SpeedUp in hierarchy::state::Stopped
[STM] [hierarchy] < hierarchy::state::Stopped
[STM] [hierarchy] > hierarchy::state::Moving
[STM] [hierarchy] > hierarchy::state::Walking
>normal stop
[STM] [hierarchy] event: hierarchy::event::Stop
[STM] [hierarchy] Reaction: hierarchy::event::Stop in hierarchy::state::Walking
[STM] [hierarchy] Reaction: hierarchy::event::Stop in hierarchy::state::Moving
[STM] [hierarchy] < hierarchy::state::Walking
[STM] [hierarchy] < hierarchy::state::Moving
[STM] [hierarchy] > hierarchy::state::Stopped
[STM] [hierarchy] < hierarchy::state::Stopped
end
 */
int main(int argc, char *argv[])
{
    hierarchy::StateMachine sm;

    cout << ">init\n";
    sm.initiate();

    cout << ">increase speed -> walking\n";
    sm.process_event(hierarchy::event::SpeedUp());

    cout << ">increase speed -> running\n";
    sm.process_event(hierarchy::event::SpeedUp());

    cout << ">increase speed -> sprinting\n";
    sm.process_event(hierarchy::event::SpeedUp());

    cout << ">hard stop\n";
    sm.process_event(hierarchy::event::Stop());

    cout << ">increase speed -> walking\n";
    sm.process_event(hierarchy::event::SpeedUp());

    cout << ">normal stop\n";
    sm.process_event(hierarchy::event::Stop());

    sm.terminate();
    cout << "end\n";

    return 0;
}