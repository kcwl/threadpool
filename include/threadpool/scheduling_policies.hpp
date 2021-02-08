#pragma once
#include <queue>
#include <functional>
#include "detail/basic_task.hpp"

namespace thread_pool
{
	template<class Task = detail::priority_task>
	class priority_scheduler
	{
	public:
		bool empty()
		{
			return queue_.empty();
		}

		bool push_back(const Task& task)
		{
			queue_.push(task);

			return true;
		}

		auto pop_front()
		{
			auto front = queue_.top();
			queue_.pop();

			return front;
		}

		void clear()
		{
			while (!queue_.empty())
			{
				queue_.pop();
			}
		}

	private:
		std::priority_queue<Task> queue_;
	};


	template <typename Task = detail::task>
	class fifo_scheduler
	{
	public:
		bool push_back(const Task& task)
		{
			queue_.push_back(task);
			return true;
		}

		auto pop_front()
		{
			auto front = queue_.front();
			queue_.pop_front();

			return front;
		}


		size_t size() const
		{
			return queue_.size();
		}

		bool empty() const
		{
			return queue_.empty();
		}

		void clear()
		{
			queue_.clear();
		}

	protected:
		std::deque<Task> queue_;
	};

}
