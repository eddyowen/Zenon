#pragma once

#include "Core/Assert.h"

#include <type_traits>

namespace zn
{
    template <typename Signature>
    class Delegate;

    template <typename R, typename...Args>
    class Delegate<R(Args...)>
    {
    public:
        // Creates an unbound delegate
        Delegate() = default;

        // We want the Delegate to be copyable, since its lightweight
        Delegate(const Delegate& other) = default;
        auto operator=(const Delegate& other)->Delegate & = default;

        // Call the underlying bound function
        template <typename... UArgs,
            typename = std::enable_if_t<std::is_invocable_v<R(Args...), UArgs...>>>
        auto operator()(UArgs&&...args) -> R
        {
            return std::invoke(m_Stub, m_Instance, args...);
        }

        template <auto MemberFunction,
            typename = std::enable_if_t<std::is_invocable_r_v<R, decltype(Function), Args...>>>
        auto Bind() -> void
        {
            m_Instance = nullptr;

            m_Stub = static_cast<StubFunction>([](const void* p, Args...args) -> R
            {
                return std::invoke(MemberFunction, std::forward<Args>(args)...);
            });
        }

        template <auto MemberFunction, typename Class,
            typename = std::enable_if_t<std::is_invocable_r_v<R, decltype(MemberFunction), const Class*, Args...>>>
        auto Bind(const Class* c) -> void
        {
            m_Instance = c;

            m_Stub = static_cast<StubFunction>([](const void* p, Args... args) -> R
            {
                const auto* cls = static_cast<const Class*>(p);

                return std::invoke(MemberFunction, cls, std::forward<Args>(args)...);
            });
        }

        template <auto MemberFunction, typename Class,
            typename = std::enable_if_t<std::is_invocable_r_v<R, decltype(MemberFunction), Class*, Args...>>>
        auto Bind(Class* c) -> void
        {
            m_Instance = c;

            m_Stub = static_cast<StubFunction>([](const void* p, Args...args) -> R
            {
                auto* cls = const_cast<Class*>(static_cast<const Class*>(p));
            
                return std::invoke(MemberFunction, cls, std::forward<Args>(args)...);
            });
        }

        [[noreturn]]
        static auto StubNull(const void* p, Args...) -> R
        {
            throw std::exception{};
        }

    private:
        using StubFunction = R(*)(const void*, Args...);

        const void* m_Instance = nullptr;
        StubFunction m_Stub = &StubNull;
    };
}