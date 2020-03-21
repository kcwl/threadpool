#pragma once
#include <memory>
#include <vector>
#include <future>
#include <mutex>
#include <deque>
#include <utility>
#include <condition_variable>
#include "noncopyable.hpp"
#include "work_thead.hpp"
#include "type_traits.hpp"

namespace Aquarius
{
	namespace thread_pool
	{
		namespace detail
		{
			template<class Task, 
				template<class> class Schedule,
				template<class> class Shutdown>
			class pool_core 
				: public std::enable_shared_from_this<pool_core<Task,Schedule,Shutdown>>
				, public noncopyable
			{
			public:
				using task_type = Task;
				using pool_type = pool_core<Task, Schedule,Shutdown>;
				using schedule_type = Schedule<task_type>;
				

			public:
				pool_core() = default;

				pool_core(std::size_t thread_size)
					: thread_size_(thread_size)
					, is_shutdown_(false) {}


			public:
				std::size_t pending()
				{
					return schedule_.pending();
				}

				bool schedule(const task_type& task)
				{
					if(is_shutdown_)
						return false;

					if(!schedule_.push(task))
						return false;

					cv_.notify_one();
					return true;
				}

				bool empty()
				{
					std::unique_lock<std::recursive_mutex> lk(re_mutex_);
					return schedule_.empty();
				}

				bool execute_task()
				{
					if(is_shutdown_)
						return false;

					std::unique_lock<std::recursive_mutex> lk(re_mutex_);

					cv_.wait(lk, [this]
						{
							return !schedule_.empty();
						});

					auto task = schedule_.top();
					schedule_.pop();

					auto future = std::async(std::launch::deferred, task);
					future.wait();

					task_result_.push_back(std::move(future));

					cv_.notify_all();

					return true;
				}

				auto get_result() -> std::deque<std::future<task_type>>
				{
					return task_result_;
				}

			public:
				void start_threads()
				{
					threads_.resize(thread_size_);

					for(auto& iter : threads_)
					{
						iter.reset(new work_thread<pool_type>(this->shared_from_this()));
					}
				}

				void clear()
				{
					return schedule_.clear();
				}

				void close_threads()
				{
					return Shutdown<pool_type>::shutdown(this->shared_from_this());
				}

				void wait() {}

				void shutdown()
				{
					std::scoped_lock lk(re_mutex_);

					is_shutdown_ = true;

					cv_.notify_all();

					for(auto iter : threads_)
					{
						iter->join();
					}
				}

			private:
				std::size_t thread_size_;

				schedule_type schedule_;

				std::vector<std::shared_ptr<work_thread<pool_type>>> threads_;
				std::deque<std::future<function_rt_t<task_type>>> task_result_;
				std::condition_variable_any cv_;

				mutable std::recursive_mutex re_mutex_;

				bool is_shutdown_;
			};
		}
	}
}