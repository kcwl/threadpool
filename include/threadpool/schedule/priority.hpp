#pragma once
#include <functional>
#include <queue>

namespace thread_pool
{
	template <typename _Task>
	class priority_scheduler
	{
	public:
		bool empty()
		{
			return queue_.empty();
		}

		void push(const _Task& task)
		{
			queue_.push(task);
		}

		bool pop(_Task& t)
		{
			if (queue_.empty())
				return false;

			t = queue_.top();

			queue_.pop();

			return true;
		}

		void clear()
		{
			while (!queue_.empty())
			{
				queue_.pop();
			}
		}

		std::size_t size()
		{
			return queue_.size();
		}

	private:
		std::priority_queue<_Task> queue_;
	};

	

	
} // namespace thread_pool
