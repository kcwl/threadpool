#pragma once
#include <memory>
#include <chrono>

namespace threadpool
{
	template <typename _Pool>
	struct wait_task_for
	{
		template <typename _Clock, typename _Dura>
		static void shutdown(std::shared_ptr<_Pool> pool, const std::chrono::time_point<_Clock,_Dura>& tm)
		{
			pool->wait_for(tm);
			pool->shutdown();
		}
	};
}