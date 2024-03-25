#pragma once
#include <queue>

namespace thread_pool
{
	template <typename _Task>
	class fifo_scheduler
	{
	public:
		void push(_Task&& t)
		{
			queue_.push(std::forward<_Task>(t));
		}

		bool pop(_Task& t)
		{
			if (queue_.empty())
				return false;

			t = std::move(queue_.front());

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

		bool empty()
		{
			return queue_.empty();
		}

	private:
		std::queue<_Task> queue_;
	};
}