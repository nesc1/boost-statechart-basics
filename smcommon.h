#pragma once
#include <boost/algorithm/string.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/deferral.hpp>
#include <boost/statechart/event.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/type_index.hpp>

namespace Utils {
namespace StateMachine {

namespace sc  = boost::statechart;
namespace mpl = boost::mpl;

/**
 * @internal
 * @brief The helper class to store an entity name and log information.
 */
class EntityInfo {
   public:
    const std::string &name() const { return m_name; }

   protected:
    explicit EntityInfo(const std::type_info &typeId) {
        m_name = boost::core::demangle(typeId.name());
        ::boost::erase_all(m_name, "struct ");
        ::boost::erase_all(m_name, "class ");
    }
    virtual ~EntityInfo() = default;

   private:
    std::string m_name;
};
/**
 * @brief The state-machine event.
 *
 * This is a logged event class. An event name for logging purposes will be initialised based
 * on the most derived typename (the subclass).
 */
template <class MostDerived, class Allocator = std::allocator<void>>
struct Event : sc::event<MostDerived, Allocator>, EntityInfo {
   protected:
    /**
     * @brief The constructor.
     *
     * The constructor initialises a name of this event.
     */
    Event()
        : EntityInfo(typeid(MostDerived)) {}
};

/**
 * @brief The state.
 */
template <class MostDerived,
          class Context,
          class InnerInitial                          = boost::mpl::list<>,
          boost::statechart::history_mode historyMode = boost::statechart::history_mode::has_no_history>
struct State : boost::statechart::state<MostDerived, Context, InnerInitial, historyMode>, EntityInfo {
   public:
    /// The base class of State.
    using base_type = sc::state<MostDerived, Context, InnerInitial, historyMode>;
    /// The base class of State's subclasses. Required by @c boost::statechart.
    using my_base                = State;
    using event_base_type        = typename base_type::event_base_type;
    using rtti_policy_type       = typename base_type::rtti_policy_type;
    using outermost_context_type = typename base_type::outermost_context_type;

    State(typename base_type::my_context ctx)
        : base_type(ctx)
        , EntityInfo(typeid(MostDerived)) {
        this->outermost_context().logStateEntry(name());
    }
    virtual ~State() { this->outermost_context().logStateExit(name()); }

    /**
     * @internal
     * @brief The logging version of @c boost::statechart::simple_state::react_impl.
     */
    sc::detail::reaction_result react_impl(const event_base_type &            event,
                                           typename rtti_policy_type::id_type eventType) override {
        const auto  eventName = outermost_context_type::eventInfo(event);
        const auto &stateName = name();

        this->outermost_context().logReactOnEvent(eventName, stateName);

        /* Call the real implementation of react_impl.
         * NOTE: if it returns do_discard_event, then "this" pointer is not valid any more;
         *       id does something like "delete this" in case of a state change.
         */
        auto reactionResult = base_type::react_impl(event, eventType);

        /* A special handling for deferred events. The "this" pointer is valid. */
        if(reactionResult == sc::detail::do_defer_event) {
            this->outermost_context().logFirstDeferral(eventName, stateName);
        }

        return reactionResult;
    }
};


class StateMachineBase {
   public:
    /**
     * @brief Get a state-machine name.
     */
    const std::string &machineName() const { return m_machineName; }

    virtual void logStateEntry(const std::string &statename) {
        std::cout << "[STM] [" << machineName() << "] > " << statename << "\n";
    }
    virtual void logStateExit(const std::string &statename) {
        std::cout << "[STM] [" << machineName() << "] < " << statename << "\n";
    }
    virtual void logProcessEvent(const std::string &eventname) {
        std::cout << "[STM] [" << machineName() << "] Event: " << eventname << "\n";
    }
    virtual void logReactOnEvent(const std::string &eventname, const std::string &statename) {
        std::cout << "[STM] [" << machineName() << "] Reaction: " << eventname << " in " << statename << "\n";
    }
    virtual void logFirstDeferral(const std::string &eventname, const std::string &statename) {
        std::cout << "[STM] [" << machineName() << "] Deferral: " << eventname << " in " << statename << "\n";
    }
    virtual void logUnconsumedEvent(const std::string &eventname) {
        std::cout << "[STM] [" << machineName() << "] Unconsumed event: " << eventname << "\n";
    }

   protected:
    StateMachineBase(const std::string &name)
        : m_machineName(name)
        , m_isCurrentlyWorking(false) {}
    virtual ~StateMachineBase() {}

    std::string m_machineName;
    bool        m_isCurrentlyWorking;
};
/**
 * @brief The extension to a @c boost::statechart::state_machine.
 */
template <class MostDerived,
          class InitialState,
          class Allocator           = std::allocator<void>,
          class ExceptionTranslator = sc::null_exception_translator>
struct StateMachine : sc::state_machine<MostDerived, InitialState, Allocator, ExceptionTranslator>, StateMachineBase {
   public:
    using base_type           = sc::state_machine<MostDerived, InitialState, Allocator, ExceptionTranslator>;
    using event_base_ptr_type = typename base_type::event_base_ptr_type;
    using event_base_type     = typename base_type::event_base_type;
    using state_base_type     = typename base_type::state_base_type;

    /**
     * @brief The constructor.
     * @param name A name of the state-machine.
     *
     * Construct a state-machine with a given name. The name is used by a default logging
     * implementation.
     */
    explicit StateMachine(const std::string &name)
        : StateMachineBase(name) {}

    /**
     * @brief Process a given event.
     * @param event An event to process.
     *
     * This is a reimplementation of @c boost::statechart::process_event().
     *
     * This method calls @ref logProcessEvent(). Then, depending on if this is a reentrant
     * call, it either calls @c boost::statechart::process_event() (if not reentered) or @c
     * boost::statechart::post_event() (if reentered).
     */
    void process_event(const event_base_type &event) {
        logProcessEvent(eventInfo(event));
        if(m_isCurrentlyWorking) {
            base_type::post_event(event);
        } else {
            m_isCurrentlyWorking = true;
            /* This must not throw. */
            base_type::process_event(event);
            m_isCurrentlyWorking = false;
        }
    }

    /**
     * @brief Handle an unconsumed event.
     * @param event An unconsumed event.
     *
     * This method is a reimplementation of @c boost::statechart::unconsumed_event().
     * The default implementation calls @ref logUnconsumedEvent().
     */
    void unconsumed_event(const event_base_type &event) { logUnconsumedEvent(eventInfo(event)); }

    template <typename T>
    static const EntityInfo *privateEntityInfo(const T &entity) {
        return dynamic_cast<const EntityInfo *>(&entity);
    }
    /**
     * @brief Get an event information.
     * @param event An event class.
     * @return A textual name of a given event and an additional logging information.
     */
    static std::string eventInfo(const event_base_type &event) {
        const EntityInfo *info = privateEntityInfo(event);
        return info ? info->name() : boost::core::demangle(typeid(event).name());
    }
};

}  // namespace StateMachine
}  // namespace Utils