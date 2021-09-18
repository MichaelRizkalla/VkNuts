#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Utilities/TypeAlias.h>

namespace nuts {

    class TimeController {
      public:
        using clock_type = std::chrono::steady_clock;

        struct TimeStep {

            TimeStep(clock_type::duration&& step) : mStep(std::move(step)) {
            }

            std::chrono::nanoseconds getNanoSeconds() const noexcept {
                return std::chrono::duration_cast< std::chrono::nanoseconds >(mStep);
            }
            std::chrono::microseconds getMicroSeconds() const noexcept {
                return std::chrono::duration_cast< std::chrono::microseconds >(mStep);
            }
            std::chrono::milliseconds getMilliSeconds() const noexcept {
                return std::chrono::duration_cast< std::chrono::milliseconds >(mStep);
            }
            std::chrono::seconds getSeconds() const noexcept {
                return std::chrono::duration_cast< std::chrono::seconds >(mStep);
            }

          private:
            clock_type::duration mStep;
        };

        using time_step_type = TimeStep;

        void start() noexcept {
            mStartPoint = clock_type::now();
        }

        TimeStep end() const noexcept {
            return TimeStep { std::chrono::duration_cast< clock_type::duration >(clock_type::now() - mStartPoint) };
        }

      private:
        clock_type::time_point mStartPoint;
    };

} // namespace nuts
