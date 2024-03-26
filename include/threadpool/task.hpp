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

		template <typename Func>
		class basic_priority_task : public basic_task<Func>
		{
		public:
			basic_priority_task() = default;

			basic_priority_task(Func&& f, std::size_t&& priority)
				: basic_task<Func>(std::forward<Func>(f))
				, priority_(priority)
			{}

		public:
			bool operator<(const basic_priority_task& rhs) const
			{
				return priority_ < rhs.priority_;
			}

		private:
			std::size_t priority_;
		};
	} // namespace impl

	using default_task = impl::basic_task<void>;

	using default_priority_task = impl::basic_priority_task<void>;
} // namespace threadpool