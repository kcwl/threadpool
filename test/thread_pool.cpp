#include "thread_pool.hpp"
#include "gtest/gtest.h"

TEST(threadpool, normal)
{
	thread_pool::pool pl{ 10 };

	auto future = pl.schedule(
		[]()
		{
			EXPECT_TRUE(true);

			std::cout << "[" << std::this_thread::get_id() << "]"
					  << "the task on this thread is completed!" << std::endl;
		});

	future.get();
}