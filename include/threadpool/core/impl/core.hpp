#pragma once
#include "concepts.hpp"
#include "noncopyable.hpp"
#include "work.hpp"
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <vector>

namespace threadpool
{
	namespace impl
	{
		template <typename _Task, template <typename> typename _Schedule, template <typename> typename _Shutdown>
		class pool_core : public std::enable_shared_from_this<pool_core<_Task, _Schedule, _Shutdown>>,
						  public noncopyable
		{
		public:
			using task_t = _Task;
			using pool_t = pool_core<_Task, _Schedule, _Shutdown>;
			using schedule_t = _Schedule<task_t>;
			using thread_t = work_thread<pool_t>;
			using shutdown_t = _Shutdown<pool_t>;

			pool_core(std::size_t thread_size = std::thread::hardware_concurrency() * 2)
				: thread_size_(thread_size)
				, is_shutdown_(false)
			{}

		public:
			template <typename _Func, typename... _Args>
			auto schedule(_Func&& func, _Args&&... args) -> std::future<std::invoke_result_t<_Func, _Args...>>
			{
				std::unique_lock lk(mutex_);

				if (is_shutdown_.load())
					return std::future<std::invoke_result_t<_Func, _Args...>>{};

				auto future = use_task<task_t>()(schedule_, std::forward<_Func>(func), std::forward<_Args>(args)...);

				cv_.notify_all();

				return future;
			}

			bool execute()
			{
				std::unique_lock lk(mutex_);

				if (schedule_.empty())
				{
					ternimate_cv_.notify_all();

					cv_.wait(lk, [this] { return !schedule_.empty() || is_shutdown_; });

					if (is_shutdown_.load())
						return false;
				}

				active_thread_count_++;

				task_t task{};

				schedule_.pop(task);

				task();

				active_thread_count_--;

				return true;
			}

			void start()
			{
				threads_.resize(thread_size_);

				for (auto& iter : threads_)
				{
					iter.reset(new thread_t(this->shared_from_this()));
				}
			}

			void clear()
			{
				std::unique_lock lk(mutex_);

				return schedule_.clear();
			}

			void wait()
			{
				std::unique_lock lk(mutex_);

				auto self = this->shared_from_this();

				ternimate_cv_.wait(lk, [self, this]() { return schedule_.empty() && active_thread_count_ == 0; });
			}

			template <typename _Time>
			void wait_for(const _Time& tm)
			{
				std::unique_lock lk(mutex_);

				auto self = this->shared_from_this();

				ternimate_cv_.wait_for(lk, tm,
									   [self, this]() { return schedule_.empty() && active_thread_count_ == 0; });
			}

			template <typename _Time>
			void wait_until(const _Time& tm)
			{
				std::unique_lock lk(mutex_);

				auto self = this->shared_from_this();

				ternimate_cv_.wait_until(lk, tm,
										 [self, this]() { return schedule_.empty() && active_thread_count_ == 0; });
			}

			void work_complete()
			{
				active_thread_count_--;
			}

			void close()
			{
				return shutdown_t::shutdown(this->shared_from_this());
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

			std::atomic_size_t active_thread_count_;
		};
	} // namespace impl
} // namespace threadpool
