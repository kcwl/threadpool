#pragma once
#include <memory>
#include <thread>



namespace Aquarius
{
	namespace thread_pool
	{
		namespace detail
		{
			template<class Pool>
			class work_thread
			{
			private:
				using pool_type = Pool;
			public:
				work_thread(std::shared_ptr<pool_type> pool_ptr)
					: pool_ptr_(pool_ptr)
				{
					thread_ptr_.reset(new std::thread(std::bind(&work_thread::run, this)));
				}

			public:
				void run()
				{
					while(pool_ptr_->execute_task()) {}
				}

				void join()
				{
					thread_ptr_->join();
				}

			private:
				std::shared_ptr<std::thread> thread_ptr_;

				std::shared_ptr<pool_type> pool_ptr_;
			};
		}
	}
}