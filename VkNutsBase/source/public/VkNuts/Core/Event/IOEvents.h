#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/Generic/KeyCode.h>
#include <Platform/Generic/MouseCode.h>
#include <VkNuts/Core/Event/Event.h>

namespace nuts {

    /* Virtual helper structs */
    struct KeyEvent : public Event {
        KeyCode getKeyCode() const noexcept { return mKeyCode; }

        EVENT_CATEGORY_INTERFACE(EventCategory::Input | EventCategory::Keyboard)

      protected:
        KeyEvent(KeyCode code) : mKeyCode { code } {}
        ~KeyEvent() = default;

        KeyCode mKeyCode;
    };

    struct MouseButtonEvent : public Event {
        MouseCode getMouseCode() const noexcept { return mButtonCode; }

        EVENT_CATEGORY_INTERFACE(EventCategory::Input | EventCategory::Mouse)

      protected:
        MouseButtonEvent(MouseCode code) : mButtonCode(code) {}
        ~MouseButtonEvent() = default;

        MouseCode mButtonCode;
    };

    /* Event structs */

    struct KeyPressedEvent : public KeyEvent {
        KeyPressedEvent(KeyCode code, const uint16_t& count) : KeyEvent { code }, mCount { count } {}
        ~KeyPressedEvent() = default;

        uint16_t getCount() const noexcept { return mCount; }

        EVENT_TO_STRING_INTERFACE("KeyPressedEvent: Key pressed: " << mKeyCode << ", repeated: " << mCount << " times\n")
        EVENT_TYPE_INTERFACE(KeyPressed)
        EVENT_NAME_INTERFACE(KeyPressedEvent)

      private:
        uint16_t mCount;
    };

    struct KeyReleasedEvent : public KeyEvent {
        KeyReleasedEvent(KeyCode code) : KeyEvent { code } {}

        EVENT_TO_STRING_INTERFACE("KeyReleasedEvent: Key released:" << mKeyCode << '\n')
        EVENT_TYPE_INTERFACE(KeyReleased)
        EVENT_NAME_INTERFACE(KeyReleasedEvent)
    };

    struct KeyCharacterEvent : public KeyEvent {
        KeyCharacterEvent(KeyCode code) : KeyEvent { code } {}

        EVENT_TO_STRING_INTERFACE("KeyCharacterEvent: Key pressed" << mKeyCode << '\n')
        EVENT_TYPE_INTERFACE(KeyCharacter)
        EVENT_NAME_INTERFACE(KeyCharacterEvent)
    };

    struct MouseButtonPressedEvent : public MouseButtonEvent {
        MouseButtonPressedEvent(MouseCode code /*, uin16_t count ??*/) : MouseButtonEvent { code } {}
        ~MouseButtonPressedEvent() = default;

        EVENT_TO_STRING_INTERFACE("MouseButtonPressedEvent: Button pressed: " << mButtonCode << '\n')
        EVENT_TYPE_INTERFACE(MouseButtonPressed)
        EVENT_NAME_INTERFACE(MouseButtonPressedEvent)
    };

    struct MouseButtonReleasedEvent : public MouseButtonEvent {
        MouseButtonReleasedEvent(MouseCode code /*, uin16_t count ??*/) : MouseButtonEvent { code } {}
        ~MouseButtonReleasedEvent() = default;

        EVENT_TO_STRING_INTERFACE("MouseButtonReleasedEvent: Button released: " << mButtonCode << '\n')
        EVENT_TYPE_INTERFACE(MouseButtonReleased)
        EVENT_NAME_INTERFACE(MouseButtonReleasedEvent)
    };

    struct MouseScrollTriggeredEvent : public Event {
        MouseScrollTriggeredEvent(double xOffset, double yOffset) : mXOffset(xOffset), mYOffset(yOffset) {}
        ~MouseScrollTriggeredEvent() = default;

        double getXOffset() const noexcept { return mXOffset; }
        double getYOffset() const noexcept { return mYOffset; }

        EVENT_INTERFACE(MouseScrollTriggered, EventCategory::Input | EventCategory::Mouse, MouseScrollTriggeredEvent,
                        "MouseScrollTriggeredEvent: X-axis offset: " << mXOffset << ", Y-axis offset: " << mYOffset << '\n')
      private:
        double mXOffset;
        double mYOffset;
    };

    struct MouseMovedEvent : public Event {
        MouseMovedEvent(double xPos, double yPos) : mXPos(xPos), mYPos(yPos) {}
        ~MouseMovedEvent() = default;

        double getXPosition() const noexcept { return mXPos; }
        double getYPosiiton() const noexcept { return mYPos; }

        EVENT_INTERFACE(MouseMoved, EventCategory::Input | EventCategory::Mouse, MouseMovedEvent,
                        "MouseMovedEvent: X-axis position: " << mXPos << ", Y-axis position: " << mYPos << '\n')
      private:
        double mXPos;
        double mYPos;
    };

} // namespace nuts