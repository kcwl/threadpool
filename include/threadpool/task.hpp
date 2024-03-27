#pragma once
#include <functional>

namespace threadpool
{
	namespace impl
	{
		template <typename _Result, typename... _Args>
		class basic_task
		{
		public:
			using function_t = std::function<_Result(_Args...)>;

		public:
			basic_task() = default;

			virtual ~basic_task() = default;

			template <typename _Func>
			basic_task(_Func&& func)
				: func_(std::move(func))
			{}

			basic_task(basic_task&& other)
				: func_(std::move(other.func_))
			{

			}

			basic_task(const basic_task&) = default;
			basic_task& operator=(const basic_task&) = default;

		public:
			void operator()()
			{
				if (func_)
				{
					func_();
				}
			}

		protected:
			std::function<_Result(_Args...)> func_;
		};

		template <typename _Result, typename... _Args>
		class basic_priority_task : public basic_task<_Result, _Args...>
		{
			using base_type = basic_task<_Result, _Args...>;

			using typename base_type::function_t;
		public:
			basic_priority_task() = default;

			basic_priority_task(function_t&& f)
				: base_type(std::forward<function_t>(f))
				, priority_()
			{}

		public:
			bool operator<(const basic_priority_task& rhs) const
			{
				return priority_ < rhs.priority_;
			}

			void set_priority(std::size_t priority)
			{
				priority_ = priority;
			}

		private:
			std::size_t priority_;
		};
	} // namespace impl

	using default_task = impl::basic_task<void>;

	using default_priority_task = impl::basic_priority_task<void>;
} // namespace threadpool