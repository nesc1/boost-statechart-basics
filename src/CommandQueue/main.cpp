#include <iostream>
#include <typeinfo>

#include "CommandQueue.hpp"

using namespace std;
/*
λ src\CommandQueue\CommandQueue.exe
[STM] [StateMachineCommandQueue] > commandqueue::state::Init
not starting executing:
[STM] [StateMachineCommandQueue] event: commandqueue::event::EvExecute
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvExecute in commandqueue::state::Init
[STM] [StateMachineCommandQueue] Deferral: commandqueue::event::EvExecute in commandqueue::state::Init
finish first call:
[STM] [StateMachineCommandQueue] event: commandqueue::event::EvFinished
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvFinished in commandqueue::state::Init
[STM] [StateMachineCommandQueue] Deferral: commandqueue::event::EvFinished in commandqueue::state::Init
execute second call:
[STM] [StateMachineCommandQueue] event: commandqueue::event::EvExecute
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvExecute in commandqueue::state::Init
[STM] [StateMachineCommandQueue] Deferral: commandqueue::event::EvExecute in commandqueue::state::Init
start:
[STM] [StateMachineCommandQueue] event: commandqueue::event::EvStart
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvStart in commandqueue::state::Init
[STM] [StateMachineCommandQueue] < commandqueue::state::Init
[STM] [StateMachineCommandQueue] > commandqueue::state::Idle
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvExecute in commandqueue::state::Idle
first call to execute
[STM] [StateMachineCommandQueue] < commandqueue::state::Idle
[STM] [StateMachineCommandQueue] > commandqueue::state::Busy
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvFinished in commandqueue::state::Busy
[STM] [StateMachineCommandQueue] < commandqueue::state::Busy
[STM] [StateMachineCommandQueue] > commandqueue::state::Idle
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvExecute in commandqueue::state::Idle
second call to execute
[STM] [StateMachineCommandQueue] < commandqueue::state::Idle
[STM] [StateMachineCommandQueue] > commandqueue::state::Busy
finish second call:
[STM] [StateMachineCommandQueue] event: commandqueue::event::EvFinished
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvFinished in commandqueue::state::Busy
[STM] [StateMachineCommandQueue] < commandqueue::state::Busy
[STM] [StateMachineCommandQueue] > commandqueue::state::Idle
execute third call:
[STM] [StateMachineCommandQueue] event: commandqueue::event::EvExecute
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvExecute in commandqueue::state::Idle
third call to execute
[STM] [StateMachineCommandQueue] < commandqueue::state::Idle
[STM] [StateMachineCommandQueue] > commandqueue::state::Busy
fourd third call:
[STM] [StateMachineCommandQueue] event: commandqueue::event::EvExecute
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvExecute in commandqueue::state::Busy
[STM] [StateMachineCommandQueue] Deferral: commandqueue::event::EvExecute in commandqueue::state::Busy
stop:
[STM] [StateMachineCommandQueue] event: commandqueue::event::EvStop
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvStop in commandqueue::state::Busy
[STM] [StateMachineCommandQueue] Deferral: commandqueue::event::EvStop in commandqueue::state::Busy
finish third call:
[STM] [StateMachineCommandQueue] event: commandqueue::event::EvFinished
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvFinished in commandqueue::state::Busy
[STM] [StateMachineCommandQueue] < commandqueue::state::Busy
[STM] [StateMachineCommandQueue] > commandqueue::state::Idle
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvExecute in commandqueue::state::Idle
fourd call to execute
[STM] [StateMachineCommandQueue] < commandqueue::state::Idle
[STM] [StateMachineCommandQueue] > commandqueue::state::Busy
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvStop in commandqueue::state::Busy
[STM] [StateMachineCommandQueue] Deferral: commandqueue::event::EvStop in commandqueue::state::Busy
execute last call:
[STM] [StateMachineCommandQueue] event: commandqueue::event::EvExecute
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvExecute in commandqueue::state::Busy
[STM] [StateMachineCommandQueue] Deferral: commandqueue::event::EvExecute in commandqueue::state::Busy
start:
[STM] [StateMachineCommandQueue] event: commandqueue::event::EvStart
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvStart in commandqueue::state::Busy
[STM] [StateMachineCommandQueue] Deferral: commandqueue::event::EvStart in commandqueue::state::Busy
stop:
[STM] [StateMachineCommandQueue] event: commandqueue::event::EvStop
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvStop in commandqueue::state::Busy
[STM] [StateMachineCommandQueue] Deferral: commandqueue::event::EvStop in commandqueue::state::Busy
finish fourd call:
[STM] [StateMachineCommandQueue] event: commandqueue::event::EvFinished
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvFinished in commandqueue::state::Busy
[STM] [StateMachineCommandQueue] < commandqueue::state::Busy
[STM] [StateMachineCommandQueue] > commandqueue::state::Idle
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvStop in commandqueue::state::Idle
[STM] [StateMachineCommandQueue] < commandqueue::state::Idle
[STM] [StateMachineCommandQueue] > commandqueue::state::Init
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvExecute in commandqueue::state::Init
[STM] [StateMachineCommandQueue] Deferral: commandqueue::event::EvExecute in commandqueue::state::Init
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvStart in commandqueue::state::Init
[STM] [StateMachineCommandQueue] < commandqueue::state::Init
[STM] [StateMachineCommandQueue] > commandqueue::state::Idle
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvExecute in commandqueue::state::Idle
last call to execute
[STM] [StateMachineCommandQueue] < commandqueue::state::Idle
[STM] [StateMachineCommandQueue] > commandqueue::state::Busy
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvStop in commandqueue::state::Busy
[STM] [StateMachineCommandQueue] Deferral: commandqueue::event::EvStop in commandqueue::state::Busy
finish last call:
[STM] [StateMachineCommandQueue] event: commandqueue::event::EvFinished
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvFinished in commandqueue::state::Busy
[STM] [StateMachineCommandQueue] < commandqueue::state::Busy
[STM] [StateMachineCommandQueue] > commandqueue::state::Idle
[STM] [StateMachineCommandQueue] Reaction: commandqueue::event::EvStop in commandqueue::state::Idle
[STM] [StateMachineCommandQueue] < commandqueue::state::Idle
[STM] [StateMachineCommandQueue] > commandqueue::state::Init
[STM] [StateMachineCommandQueue] < commandqueue::state::Init
*/
int main(int /*argc*/, char* /*argv*/[]) {
    commandqueue::StateMachine stm;
    stm.initiate();

    cout << "not starting executing:\n";
    stm.process_event(commandqueue::event::EvExecute([]() { cout << "first call to execute\n"; }));
    cout << "finish first call:\n";
    stm.process_event(commandqueue::event::EvFinished());
    cout << "execute second call:\n";
    stm.process_event(commandqueue::event::EvExecute([]() { cout << "second call to execute\n"; }));
    cout << "start:\n";
    stm.process_event(commandqueue::event::EvStart());
    cout << "finish second call:\n";
    stm.process_event(commandqueue::event::EvFinished());
    cout << "execute third call:\n";
    stm.process_event(commandqueue::event::EvExecute([]() { cout << "third call to execute\n"; }));
    cout << "fourd third call:\n";
    stm.process_event(commandqueue::event::EvExecute([]() { cout << "fourd call to execute\n"; }));
    cout << "stop:\n";
    stm.process_event(commandqueue::event::EvStop());
    cout << "finish third call:\n";
    stm.process_event(commandqueue::event::EvFinished());
    cout << "execute last call:\n";
    stm.process_event(commandqueue::event::EvExecute([]() { cout << "last call to execute\n"; }));
    cout << "start:\n";
    stm.process_event(commandqueue::event::EvStart());
    cout << "stop:\n";
    stm.process_event(commandqueue::event::EvStop());
    cout << "finish fourd call:\n";
    stm.process_event(commandqueue::event::EvFinished());
    cout << "finish last call:\n";
    stm.process_event(commandqueue::event::EvFinished());

    stm.terminate();
    return 0;
}