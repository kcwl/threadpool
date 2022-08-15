#pragma once
#include <queue>
#include <functional>
#include <mutex>
#include <random>
#include "detail/basic_task.hpp"
#include "detail/hash_fnv.hpp"

namespace thread_pool
{
	template<class Task>
	class priority_scheduler
	{
	public:
		bool empty()
		{
			return queue_.empty();
		}

		void push(const Task& task)
		{
			queue_.push(task);
		}

		bool pop(Task& t)
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
		std::priority_queue<Task> queue_;
	};

	template<typename T>
	class fifo_scheduler
	{
	public:
		void push(T&& t)
		{
			std::scoped_lock lk(mutex_);

			queue_.push(std::forward<T>(t));

			cv_.notify_one();
		}

		bool pop(T& t)
		{
			std::unique_lock lk(mutex_);

			cv_.wait(lk, [&] {return !queue_.empty(); });

			if (queue_.empty())
				return false;

			t = std::move(queue_.front());

			queue_.pop();

			return true;
		}

		std::size_t size()
		{
			std::scoped_lock lk(mutex_);

			return queue_.size();
		}

		bool empty()
		{
			std::scoped_lock lk(mutex_);

			return queue_.empty();
		}

	private:
		std::queue<T> queue_;

		std::mutex mutex_;

		std::condition_variable cv_;
	};

	template<typename T>
	class multi_fifo_schedule
	{
	public:
		multi_fifo_schedule(std::size_t queue_size = std::thread::hardware_concurrency())
		{
			for (int i = 0; i < queue_size; i++)
			{
				queues_.push_back({});

				real_list_.push_back(std::to_string(i));
			}

			hf_.put_real(real_list_);
		}

		void push(T&& t)
		{
			auto addr = std::addressof(t);

			auto str_id = hf_.get_node(std::to_string(*reinterpret_cast<int*>(addr)));

			push_by_id(std::forward<T>(t), std::atoi(str_id.data()));
		}

		bool pop(T& t)
		{
			std::random_device rd;
			std::mt19937 gen(rd());

			std::uniform_int_distribution<> dis(0, static_cast<int>(queues_.size() - 1));

			int id = dis(gen);

			while (queues_.at(id).empty())
			{
				++id > queues_.size() - 1 ? id = 0 : 0;
			}
				
			t = queues_.at(id).front();

			queues_.at(id).pop();

			return true;
		}

		bool empty()
		{
			return std::all_of(queues_.begin(), queues_.end(), [](auto queue) {return queue.empty(); });
		}

		std::size_t size()
		{
			return queues_.size();
		}

	private:
		void push_by_id(T&& t, std::size_t id)
		{
			std::scoped_lock lk(mutex_);

			queues_.at(id).push(std::forward<T>(t));
		}

		bool pop_by_id(T& t, std::size_t id)
		{
			std::scoped_lock lk(mutex_);

			return queues_.at(id).pop(t);
		}

	private:
		std::mutex mutex_;

		std::vector<std::queue<T>> queues_;

		detail::hash_fnv hf_;

		std::vector<std::string> real_list_;
	};
}
