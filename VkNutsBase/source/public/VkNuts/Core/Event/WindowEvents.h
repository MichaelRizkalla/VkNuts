#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/Event/Event.h>

namespace nuts {

    struct WindowClosedEvent : public Event {
      public:
        WindowClosedEvent()  = default;
        ~WindowClosedEvent() = default;

        EVENT_INTERFACE(WindowClosed, EventCategory::Window, WindowClosedEvent, "WindowClosedEvent\n")
    };

    struct WindowMinimizedEvent : public Event {
      public:
        WindowMinimizedEvent()  = default;
        ~WindowMinimizedEvent() = default;

        EVENT_INTERFACE(WindowMinimized, EventCategory::Window, WindowMinimizedEvent, "WindowMinimizedEvent\n")
    };

    struct WindowResizedEvent : public Event {
      public:
        WindowResizedEvent(int inWidth, int inHeight) : mWidth { inWidth }, mHeight { inHeight } {}
        ~WindowResizedEvent() = default;

        int GetWidth() const { return mWidth; }
        int GetHeight() const { return mHeight; }

        EVENT_INTERFACE(WindowResized, EventCategory::Window, WindowResizedEvent, "WindowResizedEvent: new width: " << mWidth << ", new height: " << mHeight << '\n')

      private:
        int mWidth;
        int mHeight;
    };

    struct WindowFocusedEvent : public Event {
      public:
        WindowFocusedEvent()  = default;
        ~WindowFocusedEvent() = default;

        EVENT_INTERFACE(WindowFocused, EventCategory::Window, WindowFocusedEvent, "WindowFocusedEvent\n")
    };

    struct WindowLostEvent : public Event {
      public:
        WindowLostEvent()  = default;
        ~WindowLostEvent() = default;

        EVENT_INTERFACE(WindowLost, EventCategory::Window, WindowLostEvent, "WindowLostEvent\n")
    };

    struct WindowMovedEvent : public Event {
      public:
        WindowMovedEvent()  = default;
        ~WindowMovedEvent() = default;

        EVENT_INTERFACE(WindowMoved, EventCategory::Window, WindowMovedEvent, "WindowMovedEvent\n")
    };

} // namespace nuts