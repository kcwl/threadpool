#pragma once
#include <memory>

namespace Aquarius
{
	namespace thread_pool
	{
		template<class Pool>
		class wait_for_all_task
		{
		public:
			static void shutdown(std::shared_ptr<Pool> pool)
			{
				pool->wait();
				pool->shutdown();
			}
		};

		template<class Pool>
		class wait_for_active_task
		{
		public:
			static void shutdown(std::shared_ptr<Pool> pool)
			{
				pool->clear();
				pool->wait();
				pool->shutdown();
			}
		};
	}
}