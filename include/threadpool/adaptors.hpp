#pragma once
#include "threadpool/task.hpp"
#include <functional>
#include <memory>

namespace threadpool
{
	template <typename _Pool, typename Runable>
	bool schedule(_Pool& pool, const std::shared_ptr<Runable>& obj)
	{
		return pool.schedule(std::bind(&Runable::run, obj));
	}

	template <typename _Pool, typename Func>
	bool schedule(_Pool& pool, Func&& f)
	{
		return pool.schedule(std::forward<Func>(f));
	}

	template <typename _Pool>
	bool schedule(_Pool& pool, typename _Pool::task_t& task)
	{
		return pool.schedule(task);
	}

	template <typename _Pool, typename Func, typename Priority = std::size_t>
	bool schedule(_Pool& pool, Priority p, Func&& f)
	{
		return pool.schedule(task(p, std::forward<Func>(f)));
	}
} // namespace threadpool
