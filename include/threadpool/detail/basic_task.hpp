#pragma once

namespace thread_pool
{
	namespace detail
	{
		template<typename _Func>
		class basic_task
		{
		public:
			basic_task() = default;

			basic_task(_Func&& func)
				: func_(std::forward<_Func>(func))
			{

			}

			virtual ~basic_task() = default;

			void operator()()
			{
				if (func_)
				{
					func_();
				}
			}

		protected:
			_Func func_;
		};

		template<typename Func>
		class basic_priority_task : public basic_task<Func>
		{
		public:
			basic_priority_task() = default;

			basic_priority_task(Func&& f, std::size_t&& priority)
				: basic_task<Func>(std::forward<Func>(f))
				, priority_(priority)
			{

			}

		public:
			bool operator<(const basic_priority_task& rhs) const
			{
				return priority_ < rhs.priority_;
			}

		private:
			std::size_t priority_;
		};
	}

}
