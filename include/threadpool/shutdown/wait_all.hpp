#pragma once
#include <memory>

namespace thread_pool
{
	template <typename _Pool>
	struct wait_for_all_task
	{
		static void shutdown(std::shared_ptr<_Pool> pool)
		{
			pool->wait();
			pool->shutdown();
		}
	};
} // namespace thread_pool
