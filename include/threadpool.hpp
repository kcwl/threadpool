#pragma once
#include "threadpool/adaptors.hpp"
#include "threadpool/core/pool.hpp"
#include "threadpool/schedule/fifo.hpp"
#include "threadpool/schedule/multi_fifo.hpp"
#include "threadpool/schedule/priority.hpp"
#include "threadpool/shutdown/wait_active.hpp"
#include "threadpool/shutdown/wait_all.hpp"
#include "threadpool/shutdown/wait_for.hpp"
#include "threadpool/shutdown/wait_until.hpp"
#include "threadpool/task.hpp"

namespace threadpool
{
	using priority_pool = basic_thread_pool<default_priority_task, priority_scheduler, wait_for_all_task>;

	using pool = basic_thread_pool<default_task, fifo_scheduler, wait_for_all_task>;

	using multi_pool = basic_thread_pool<default_task, multi_fifo_schedule, wait_for_all_task>;
} // namespace threadpool