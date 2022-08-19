#pragma once
#include <memory>
#include <vector>
#include <future>
#include <mutex>
#include <utility>
#include <functional>
#include <condition_variable>
#include "noncopyable.hpp"
#include "work_thread.hpp"
#include "task_traits.hpp"


namespace thread_pool
{
	namespace detail
	{
		template<typename Task,
			template<typename> typename Schedule,
			template<typename> typename Shutdown>
		class pool_core
			: public std::enable_shared_from_this<pool_core<Task, Schedule, Shutdown>>
			, public noncopyable
		{
		public:
			using task_t = Task;
			using pool_t = pool_core<Task, Schedule, Shutdown>;
			using schedule_t = Schedule<task_t>;
			using thread_t = work_thread<pool_t>;

			pool_core(std::size_t thread_size = std::thread::hardware_concurrency() * 2)
				: thread_size_(thread_size)
				, is_shutdown_(false)
			{
			}

		public:
			template<typename _Func, typename... _Args>
			auto schedule(_Func&& func, _Args&&... args) ->std::future<task_result_t<task_t, _Func, _Args...>>
			{
				std::unique_lock lk(mutex_);

				if (is_shutdown_.load())
					return std::future<task_result_t<task_t, _Func, _Args...>>{};

				auto future = use_task<task_t>()(schedule_, std::forward<_Func>(func), std::forward<_Args>(args)...);

				cv_.notify_one();

				return future;
			}

			bool execute()
			{
				std::unique_lock lk(mutex_);

				while (schedule_.empty())
				{
					active_thread_count_--;

					ternimate_cv_.notify_all();

					cv_.wait(lk, [this] 
						{
							return !schedule_.empty() || is_shutdown_;
						});

					if (is_shutdown_.load())
						return false;

					active_thread_count_++;
				}

				task_t task{};

				schedule_.pop(task);

				task();

				return true;
			}

			void start()
			{
				threads_.resize(thread_size_);

				for (auto& iter : threads_)
				{
					iter.reset(new thread_t(this->shared_from_this()));
				}

				active_thread_count_ = thread_size_;
			}

			void clear()
			{
				return schedule_.clear();
			}

			void wait()
			{
				std::unique_lock lk(mutex_);

				auto self = this->shared_from_this();

				ternimate_cv_.wait(lk, [self, this]()
					{ 
						return schedule_.empty() && active_thread_count_ == 0;
					});
			}

			void work_complete()
			{
				std::unique_lock lk(mutex_);

				active_thread_count_--;
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

			std::vector<std::shared_ptr<thread_t>> threads_;

			std::condition_variable_any cv_;

			mutable std::recursive_mutex mutex_;

			std::atomic_bool is_shutdown_;

			std::condition_variable_any ternimate_cv_;

			std::size_t active_thread_count_;
		};
	}
}
