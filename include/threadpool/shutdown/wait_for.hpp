#pragma once
#include <memory>

namespace thread_pool
{
	template <typename Pool>
	struct wait_task_for
	{
		template <typename _Time>
		static void shutdown(std::shared_ptr<Pool> pool, const _Time& tm)
		{
			pool->wait_for(tm);
			pool->shutdown();
		}
	};
}