#pragma once

namespace zn
{
	template<typename Signature>
	class Delegate;

	template<typename R, typename...Args>
	class Delegate<R(Args...)>
	{
	public:
		// Creates an unbound delegate
		Delegate() = default;

		// We want the Delegate to be copyable, since its lightweight
		Delegate(const Delegate& other) = default;

		Delegate& operator=(const Delegate& other) = default;

		// Call the underlying bound function
		R operator()(Args...args) const
		{
			if (m_stub == nullptr)
			{
				return;
				//std::cout << "ERROR m_STUB = NULLPTR";
			}

			return (*m_stub)(m_instance, args...);
		}

		// Bind to a free function
		template<R(*Function)(Args...)>
		void Bind()
		{
			// We don't use this for non-member functions, so just set it to nullptr
			m_instance = nullptr;

			// Bind the function pointer
			m_stub = static_cast<StubFunction>([](const void*, Args...args) -> R
			{
				return (*Function)(args...);
			});
		}

		// Bind to a class const member function
		template<auto MemberFunction, typename Class>
		void Bind(const Class* c)
		{
			// Assign the pointer to the instance that holds the member function
			m_instance = c;

			// Bind the function pointer. Fun fact, non-capturing lambdas can be convertible to a function pointer with the same signature
			m_stub = static_cast<StubFunction>([](const void* p, Args...args) -> R
			{
				// Cast back to the specific class to access the member function
				const auto* cls = static_cast<const Class*>(p);

				// Safe, because we know the pointer was bound to a const instance
				return (cls->*MemberFunction)(args...);
			});
		}


		// Bind to a class non-const member function
		template<auto MemberFunction, typename Class>
		void Bind(Class* c)
		{
			// Assign the pointer to the instance that holds the member function
			m_instance = c;

			// Bind the function pointer. Fun fact, non-capturing lambdas can be convertible to a function pointer with the same signature
			m_stub = static_cast<StubFunction>([](const void* p, Args...args) -> R
			{
				// Cast back to the specific class to access the member function
				auto* cls = const_cast<Class*>(static_cast<const Class*>(p));

				// Safe, because we know the pointer was bound to a non-const instance
				return (cls->*MemberFunction)(args...);
			});
		}

	private:
		using StubFunction = R(*)(const void*, Args...);

		const void* m_instance = nullptr; ///< A pointer to the instance (if it exists)
		StubFunction m_stub = nullptr;   ///< A pointer to the function to invoke
	};
}