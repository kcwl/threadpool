#pragma once
#include <memory>
#include <thread>

namespace threadpool
{
	namespace impl
	{
		template <typename _Pool>
		class work_thread : public std::enable_shared_from_this<work_thread<_Pool>>
		{
		public:
			work_thread(std::shared_ptr<_Pool> pool_ptr)
				: pool_ptr_(pool_ptr)
			{
				thread_ptr_.reset(new std::thread([&] { run(); }));
			}

		public:
			void run()
			{
				while (pool_ptr_->execute())
					;

				pool_ptr_->work_complete();
			}

			void join()
			{
				return thread_ptr_->join();
			}

		private:
			std::shared_ptr<_Pool> pool_ptr_;

			std::shared_ptr<std::thread> thread_ptr_;
		};
	} // namespace impl
} // namespace threadpool