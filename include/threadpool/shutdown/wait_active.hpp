#pragma once
#include <memory>

namespace thread_pool
{
	template <typename _Pool>
	struct wait_for_active_task
	{
		static void shutdown(std::shared_ptr<_Pool> pool)
		{
			pool->clear();
			pool->wait();
			pool->shutdown();
		}
	};
}