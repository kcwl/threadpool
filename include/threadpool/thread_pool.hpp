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

		template<typename _Func, typename... _Args>
		auto schedule(_Func&& f, _Args&&... args)
		{
			return core_ptr_->schedule(std::forward<_Func>(f), std::forward<_Args>(args)...);
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

	using priority_pool = thread_pool<default_priority_task, priority_scheduler, wait_for_all_task>;

	using pool = thread_pool<default_task, fifo_scheduler, wait_for_all_task>;

	using multi_pool = thread_pool<default_task, multi_fifo_schedule, wait_for_all_task>;
}



