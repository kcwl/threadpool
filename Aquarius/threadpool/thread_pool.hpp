#pragma once
#include "detail/pool_core.hpp"
#include "scheduling_policies.hpp"
#include "shutdown_policies.hpp"

namespace Aquarius
{
	namespace thread_pool
	{
		template<
			class Task = priority_task,
			template<class> class Schedule = priority_scheduler,
			template<class> class Shutdown = wait_for_all_task
		>
		class thread_pool
		{
		private:
			using pool_core_type = detail::pool_core<Task, Schedule,Shutdown>;
			using task_type = Task;

		public:
			thread_pool(std::size_t thread_size)
				: core_ptr_(new pool_core_type(thread_size)){}

		public:
			std::size_t size()
			{
				return core_ptr_->size();
			}

			bool schedule(const task_type& task)
			{
				return core_ptr_->schedule(task);
			}

			bool empty()
			{
				return core_ptr_->empty();
			}

			auto get_result()
			{
				return core_ptr_->get_result();
			}

			void start_thread()
			{
				return core_ptr_->start_threads();
			}

			void close_thread()
			{
				return core_ptr_->close_threads();
			}
		private:
			std::shared_ptr<pool_core_type> core_ptr_;
		};


		using pri_pool = thread_pool<priority_task, priority_scheduler, wait_for_all_task>;

		using pool = thread_pool<task_func, fifo_scheduler, wait_for_all_task>;
	}
}


