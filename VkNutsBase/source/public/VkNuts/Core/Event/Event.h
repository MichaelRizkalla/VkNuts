#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/Event/EventType.hpp>
#include <VkNuts/Core/Event/EventCategory.hpp>

#define EVENT_TYPE_INTERFACE(eventType)                                             \
    static EventType getStaticEventType() noexcept { return EventType::eventType; } \
    EventType        getEventType() const noexcept override { return EventType::eventType; }

#define EVENT_CATEGORY_INTERFACE(eventCategories)                                                                                                                        \
    static std::underlying_type_t< EventCategory > getStaticEventCategory() noexcept { return static_cast< std::underlying_type_t< EventCategory > >(eventCategories); } \
    std::underlying_type_t< EventCategory >        getEventCategories() const noexcept override {                                                                        \
        return static_cast< std::underlying_type_t< EventCategory > >(eventCategories);                                                                           \
    }

#define EVENT_NAME_INTERFACE(name) NUTS_LOG(const char* getName() const noexcept override { return #name; })

#define EVENT_TO_STRING_INTERFACE(text)                                       \
    NUTS_LOG(std::string getStringRepresentation() const noexcept override { \
        std::stringstream ss;                                                \
        ss << text;                                                          \
        return ss.str();                                                     \
    })

#define EVENT_INTERFACE(eventType, eventCategories, name, toStringText) \
    EVENT_TYPE_INTERFACE(eventType) EVENT_CATEGORY_INTERFACE(eventCategories) EVENT_NAME_INTERFACE(name) EVENT_TO_STRING_INTERFACE(toStringText)

namespace nuts {
    struct Event {
      public:
        Event() : mIsHandled(false) {}
        DELETE_COPY_CLASS(Event)
        DELETE_MOVE_CLASS(Event)
        virtual ~Event() = default;

        virtual EventType                               getEventType() const noexcept       = 0;
        virtual std::underlying_type_t< EventCategory > getEventCategories() const noexcept = 0;
#if defined(NUTS_ENABLE_LOG)
        virtual const char* getName() const noexcept                 = 0;
        virtual std::string getStringRepresentation() const noexcept = 0;
#endif

        bool isInCategory(EventCategory category) { return static_cast< bool >(getEventCategories() & static_cast< std::underlying_type_t< EventCategory > >(category)); }

      private:
        bool mIsHandled;
    };

    struct EventHandler {
        template < typename TEventClass, typename Func >
        requires(std::is_base_of_v< Event, TEventClass >, std::is_invocable_v< Func, TEventClass& >) bool handle(Func&& func) {
            if (TEventClass::getStaticEventType() == mEvent.getEventType()) {
                mEvent.mIsHandled = func(static_cast< TEventClass& >(mEvent));
                return true;
            }
            return false;
        }

      private:
        Event& mEvent;
    };
} // namespace nuts

#if defined(NUTS_ENABLE_LOG)
inline std::ostream& operator<<(std::ostream& os, const nuts::Event& _event) {
    os << _event.getName();
    return os;
}
#endif