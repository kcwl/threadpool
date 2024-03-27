#pragma once

TEST(threadpool, pool)
{
	threadpool::pool pl{ 10 };

	auto future = pl.schedule(
		[]()
		{
			EXPECT_TRUE(true);

			std::cout << "(pool)[" << std::this_thread::get_id() << "]"
				<< "the task on this thread is completed!" << std::endl;
		});

	future.get();
}

TEST(threadpool, priority)
{
	threadpool::priority_pool pl{ 10 };

	auto future = pl.schedule(
		[]()
		{
			EXPECT_TRUE(true);

			std::cout << "(priority)[" << std::this_thread::get_id() << "]"
				<< "the task on this thread is completed!" << std::endl;
		});

	future.get();
}

TEST(threadpool, multi)
{
	threadpool::multi_pool pl{ 10 };

	auto future = pl.schedule(
		[]()
		{
			EXPECT_TRUE(true);

			std::cout << "(multi)[" << std::this_thread::get_id() << "]"
				<< "the task on this thread is completed!" << std::endl;
		});

	future.get();
}