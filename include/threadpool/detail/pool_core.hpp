#pragma once
#include <memory>
#include <vector>
#include <mutex>
#include <utility>
#include <thread>
#include <functional>
#include <condition_variable>
#include "noncopyable.hpp"


namespace thread_pool
{
	namespace detail
	{
		template<class Task,
			template<class> class Schedule,
			template<class> class Shutdown>
		class pool_core
			: public std::enable_shared_from_this<pool_core<Task, Schedule, Shutdown>>
			, public noncopyable
		{
		public:
			using task_t = Task;
			using pool_t = pool_core<Task, Schedule, Shutdown>;
			using schedule_t = Schedule<task_t>;

			pool_core(std::size_t thread_size = std::thread::hardware_concurrency() * 2)
				: thread_size_(thread_size)
				, is_shutdown_(false)
			{
			}

		public:
			bool schedule(task_t&& task)
			{
				std::scoped_lock lk(mutex_);

				if (is_shutdown_.load())
					return false;

				schedule_.push(std::forward<task_t>(task));

				cv_.notify_one();
				return true;
			}

			bool execute()
			{
				std::unique_lock lk(mutex_);

				if (is_shutdown_.load())
					return false;

				cv_.wait(lk, [this]
					{
						return !schedule_.empty();
					});

				task_t task{};

				schedule_.pop(task);

				if (task == nullptr)
					return true;

				task();

				return true;
			}

			void start()
			{
				threads_.resize(thread_size_);

				for (auto& iter : threads_)
				{
					iter.reset(new std::thread(std::bind(&pool_core::execute,this)));
				}
			}

			void clear()
			{
				return schedule_.clear();
			}

			void wait()
			{
				cv_.notify_all();

				while (!schedule_.empty())
				{
					execute();
				}
			}

			void close()
			{
				return Shutdown<pool_t>::shutdown(this->shared_from_this());
			}

			void shutdown()
			{
				is_shutdown_.store(true);

				cv_.notify_all();

				for (auto iter : threads_)
				{
					iter->join();
				}
			}

		private:
			std::size_t thread_size_;

			schedule_t schedule_;

			std::vector<std::shared_ptr<std::thread>> threads_;

			std::condition_variable_any cv_;

			mutable std::recursive_mutex mutex_;

			std::atomic_bool is_shutdown_;
		};
	}
}
