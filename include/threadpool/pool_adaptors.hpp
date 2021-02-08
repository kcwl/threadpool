#pragma once
#include <memory>
#include <functional>


namespace thread_pool
{
	template<class Pool, class Runable>
	bool schedule(Pool& pool, const std::shared_ptr<Runable>& obj)
	{
		return pool.schedule(std::bind(&Runable::run, obj));
	}

	template<class Pool, class Func>
	bool schedule(Pool& pool, Func&& f)
	{
		return pool.schedule(std::forward<Func>(f));
	}

	template<class Pool>
	bool schedule(Pool& pool, typename Pool::task_t& task)
	{
		return pool.schedule(task);
	}

	template<class Pool, class Func, class Priority = std::size_t>
	bool schedule(Pool& pool, Priority p, Func&& f)
	{
		return pool.schedule(task(p, std::forward<Func>(f)));
	}
}
