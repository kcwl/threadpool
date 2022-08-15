#pragma once

namespace thread_pool
{
	namespace detail
	{
		template<typename Func>
		class basic_task
		{
			static_assert(std::is_function_v<Func>, "Func must be std::function!");

		public:
			basic_task(std::size_t priority, Func f)
				: func_(f)
				, priority_(priority)
			{
			}

		public:
			bool operator<(const basic_task& rhs) const
			{
				return priority_ < rhs.priority_;
			}

			void operator()(void) const
			{
				if (func_)
				{
					func_();
				}
			}

		private:
			Func func_;
			std::size_t priority_;
		};
	}

}
