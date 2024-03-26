#pragma once
#include <memory>

namespace threadpool
{
	template <typename _Pool>
	struct wait_task_until
	{
		template <typename _Clock, typename _Dura>
		static void shutdown(std::shared_ptr<_Pool> pool, const const std::chrono::time_point<_Clock, _Dura>& tm)
		{
			pool->wait_until(tm);
			pool->shutdown();
		}
	};
} // namespace threadpool