#pragma once
#include "detail/pool_core.hpp"
#include "scheduling_policies.hpp"
#include "shutdown_policies.hpp"
#include "task.hpp"

namespace thread_pool
{
	template<
		typename Task,
		template<typename> typename Schedule,
		template<typename> typename Shutdown
	>
		class thread_pool
	{
	private:
		using core_t = detail::pool_core<Task, Schedule, Shutdown>;
		using task_t = Task;

	public:
		explicit thread_pool()
			: core_ptr_(new core_t())
		{
			start_thread();
		}

		thread_pool(std::size_t thread_size)
			: core_ptr_(new core_t(thread_size))
		{
			start_thread();
		}

		virtual ~thread_pool()
		{
			close();
		}

	public:
		std::size_t size()
		{
			return core_ptr_->size();
		}

		bool schedule(task_t&& task)
		{
			return core_ptr_->schedule(std::forward<task_t>(task));
		}

		bool empty()
		{
			return core_ptr_->empty();
		}

		void close()
		{
			return core_ptr_->close();
		}

	private:
		void start_thread()
		{
			return core_ptr_->start();
		}

	private:
		std::shared_ptr<core_t> core_ptr_;
	};

	using priority_pool = thread_pool<priority_task, priority_scheduler, wait_for_all_task>;

	using pool = thread_pool<task, fifo_scheduler, wait_for_all_task>;

	using multi_pool = thread_pool<task, multi_fifo_schedule, wait_for_all_task>;
}



