#pragma once
#include <functional>

namespace Aquarius
{
	namespace thread_pool
	{
		using task_func = std::function<void()>;
		
		class priority_task
		{
		public:
			priority_task(std::size_t priority, task_func f)
				: func_(f)
				, priority_(priority) { }


			bool operator<(const priority_task& rhs) const
			{
				return priority_ < rhs.priority_;
			}

			void operator()(void) const
			{
				if(func_)
				{
					func_();
				}
			}

		private:
			task_func func_;
			std::size_t priority_;
		};
	}
}