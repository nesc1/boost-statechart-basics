# Project Boost state Machine Test

## Overview

Basic introduction to [Boost.Statechart](https://www.boost.org/doc/libs/1_72_0/libs/statechart/doc/index.html), a C++ library for finite state machines.

The current project aims to introduce `Boost.Statechart` and some of the common functionality when working with it.

For a simple example on how to use the `Boost.Statechart` library the boost webpage presents the example on `BoostSimpleExample.hpp` file.

The remaining examples use some utility classes located on `smcommon.h` file. Classes that extend the `Boost.Statechart` library in order to add log functionality mainly.

The project contain the following samples:

- `PersonWithMoney.hpp`: presents a simple example where event of get money and remove money change from one state to another;
- `Deferral.hpp`: sample to demonstrate the deferral use;
- `Discard.hpp`: sample to demonstrate discard usage;
- `hierarchy.hpp`: sample demonstrating the "hierarchy" use on  `Boost.Statechart` library

## Boost StateChart Functionality

Our sample use boost statechart as follow:

- a state machine class

```cpp
struct StateMachine : smutil::StateMachine<StateMachine, state::Idle> {
   public:
    StateMachine()
        : smutil::StateMachine<StateMachine, state::Idle>("StateMachine") {}
};
```

- one or more events

```cpp
...
struct Activate : smutil::Event<Activate> {};
...
```

- and one or more states to handle this events (each state has one or more reactions)

```cpp
struct Idle : smutil::State<Idle, StateMachine> {
    Idle(my_context ctx)
        : smutil::State<Idle, StateMachine>(ctx) {}

    typedef mpl::list<
        ...
        >
        reactions;
};
```

## Boost.Statechart Samples

Common `Boost.Statechart` library functionality is described in detail next.

### If nothing is stated for an event

The event will be unconsumed (nothing will execute) - discarded.

Example of `sm.process_event(Deferral::event::Ignore());` on state idle on `testDeferralCase()`. The event is ignored because on state `Idle` nothing is declared for event `Ignore`.

Unconsumed events are always tricky, we should have always some handling.

### Transition event

Normal transition from one state to the next state.

Can be automatic transition:

```cpp
    typedef mpl::list<
        ...
        sc::transition<event::Activate, Active>
        ...
        >
        reactions;
```

or custom reaction:

```cpp
    typedef mpl::list<
        ...
        sc::custom_reaction<event::Deactivate>,
        ...
        >
        reactions;

    sc::result react(const event::Deactivate &) {
        ...
        return transit<Active>();
    }
```

Custom reaction can do custom implementation that do one of the following:

- transit to another state - `return transit<Active>();`;
- or discard event and stay on the same state - `return discard_event();`

In the above example both transit from current state to the next state  `Active` state.

### Deferral state

```cpp
    typedef mpl::list<

        sc::deferral<event::Deactivate>,
        sc::transition<event::Activate, Active>

        >
        reactions;
```

Deferral state is the same as store the event for the next state to handle.

Note that deferral will be saved on queue and ordered. See example of `testDeferralCase()` where event `Other` is deferral on `Idle` state then `Deactivate` is also deferral. When `Active` state is activated `Other` is deferral again and then `Deactivate` event force to move to `Idle` state again, next `Other` event is deferral once more.

### hierarchy handling

hierarchy handling is a good way to handle more generic events. This generic handle can be also override as you can see on example `hierarchy.hpp`.

### orthogonal states

TODO

## Build

To build the samples project one of the following commands can help:

- using current build settings (Debug)

```batch
mkdir -p conan
cd conan
conan install .. -s build_type=Debug --build=outdated --update
cd ..
mkdir -p build
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_PROJECT_BoostStateMachine_INCLUDE=./conan/conan_paths.cmake
cmake --build . --config Debug --target <>
```

- using Visual Studio 2017 (Release):

```batch
conan install .. --build outdated -s compiler.version=15 -s build_type=Release -g cmake_paths --update
cmake .. -G "Visual Studio 15 2017 Win64" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../ -DCMAKE_PROJECT_BoostStateMachine_INCLUDE=./conan/conan_paths.cmake
cmake --build . --config Release --target install
```

- or Visual Studio 2019 with Ninja (RelWithDebInfo):

```batch
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvarsall.bat" x64
conan install .. --build outdated -s compiler.version=16 -s build_type=RelWithDebInfo -g cmake_paths --update
cmake ../.. -GNinja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_INSTALL_PREFIX=../../install -DCMAKE_PROJECT_BoostStateMachine_INCLUDE=./conan/conan_paths.cmake
cmake --build . --config Debug --target install
```

