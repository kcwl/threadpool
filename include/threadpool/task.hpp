#pragma once
#include <functional>

namespace thread_pool
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
				: func_(std::forward<_Func>(func))
			{}

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
} // namespace thread_pool