#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/Event/EventType.hpp>
#include <VkNuts/Core/Event/EventCategory.hpp>

#define EVENT_TYPE_INTERFACE(eventType)                                             \
    static EventType GetStaticEventType() noexcept { return EventType::eventType; } \
    EventType        GetEventType() const noexcept override { return EventType::eventType; }

#define EVENT_CATEGORY_INTERFACE(eventCategories)                                                                                                                        \
    static std::underlying_type_t< EventCategory > GetStaticEventCategory() noexcept { return static_cast< std::underlying_type_t< EventCategory > >(eventCategories); } \
    std::underlying_type_t< EventCategory >        GetEventCategories() const noexcept override {                                                                        \
        return static_cast< std::underlying_type_t< EventCategory > >(eventCategories);                                                                           \
    }

#define EVENT_NAME_INTERFACE(name) NUTS_LOG(const char* GetName() const noexcept override { return #name; })

#define EVENT_TO_STRING_INTERFACE(text)                                       \
    NUTS_LOG(std::string GetStringRepresentation() const noexcept override { \
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

        virtual EventType                               GetEventType() const noexcept       = 0;
        virtual std::underlying_type_t< EventCategory > GetEventCategories() const noexcept = 0;
#if defined(NUTS_ENABLE_LOG)
        virtual const char* GetName() const noexcept                 = 0;
        virtual std::string GetStringRepresentation() const noexcept = 0;
#endif

        bool IsInCategory(EventCategory category) { return static_cast< bool >(GetEventCategories() & static_cast< std::underlying_type_t< EventCategory > >(category)); }

      private:
        bool mIsHandled;
    };

    struct EventHandler {
        template < typename TEventClass, typename Func >
        requires(std::is_base_of_v< Event, TEventClass >, std::is_invocable_v< Func, TEventClass& >) bool Handle(Func&& func) {
            if (TEventClass::GetStaticEventType() == mEvent.GetEventType()) {
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
    os << _event.GetName();
    return os;
}
#endif