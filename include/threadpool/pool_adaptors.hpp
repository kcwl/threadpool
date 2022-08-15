#pragma once
#include <memory>
#include <functional>


namespace thread_pool
{
	template<typename Pool, typename Runable>
	bool schedule(Pool& pool, const std::shared_ptr<Runable>& obj)
	{
		return pool.schedule(std::bind(&Runable::run, obj));
	}

	template<typename Pool, typename Func>
	bool schedule(Pool& pool, Func&& f)
	{
		return pool.schedule(std::forward<Func>(f));
	}

	template<typename Pool>
	bool schedule(Pool& pool, typename Pool::task_t& task)
	{
		return pool.schedule(task);
	}

	template<typename Pool, typename Func, typename Priority = std::size_t>
	bool schedule(Pool& pool, Priority p, Func&& f)
	{
		return pool.schedule(task(p, std::forward<Func>(f)));
	}
}
