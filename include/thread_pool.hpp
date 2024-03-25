#pragma once
#include "threadpool/adaptors.hpp"
#include "threadpool/core/pool.hpp"
#include "threadpool/police/scheduling.hpp"
#include "threadpool/police/shutdown.hpp"
#include "threadpool/task.hpp"

namespace thread_pool
{
	using priority_pool = basic_thread_pool<default_priority_task, priority_scheduler, wait_for_all_task>;

	using pool = basic_thread_pool<default_task, fifo_scheduler, wait_for_all_task>;

	using multi_pool = basic_thread_pool<default_task, multi_fifo_schedule, wait_for_all_task>;
} // namespace thread_pool