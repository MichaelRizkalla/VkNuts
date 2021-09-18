#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Utilities/TypeAlias.h>

namespace nuts {

    template < class Type >
    class Singleton {
      public:
        using class_type = Type;
        using pointer    = Type*;

      private:
        inline static std::once_flag    mOnceFlag {};
        inline static UniqueRef< Type > mInstance { nullptr };

      protected:
        Singleton()          = default;
        virtual ~Singleton() = default;
        DELETE_COPY_CLASS(Singleton)
        DELETE_MOVE_CLASS(Singleton)

      public:
        static pointer getInstance() noexcept {
            std::call_once(mOnceFlag, [&]() {
                struct TypeMaker : public class_type {};
                NutsAllocator< TypeMaker > alloc {};

                mInstance = std::move(allocate_unique< class_type, TypeMaker >(alloc));
            });

            return mInstance.get();
        }
    };

} // namespace nuts
