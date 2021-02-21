#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Utilities/Utilities.hpp>

namespace nuts {

    template < class Type, class Del = std::default_delete< Type > >
    class Singelton {
      private:
        static std::once_flag           mOnceFlag;
        static UniqueRef< Type, Del > mInstance;

      protected:
        Singelton()          = default;
        virtual ~Singelton() = default;
        DELETE_COPY_CLASS(Singelton)
        DELETE_MOVE_CLASS(Singelton)

      public:
        using class_type   = Type;
        using deleter_type = Del;
        using pointer      = Type*;

        static pointer GetInstance() noexcept {
            std::call_once(mOnceFlag, [&]() {
                struct TypeMaker : public class_type {};

                mInstance = std::move(UniqueRef< class_type, deleter_type >(new TypeMaker {}, deleter_type {}));
            });

            return mInstance.get();
        }
    };

    template < class Type, class Del >
    std::once_flag Singelton< Type, Del >::mOnceFlag;

    template < class Type, class Del >
    UniqueRef< Type, Del > Singelton< Type, Del >::mInstance = nullptr;

} // namespace nuts