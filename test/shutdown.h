#pragma once
#include <chrono>

using namespace std::chrono_literals;

TEST(shut, wait_for_active)
{
	using pool = threadpool::basic_thread_pool<threadpool::default_task, threadpool::fifo_scheduler,
											   threadpool::wait_for_active_task>;

	std::atomic_size_t total{};

	pool p{ 1 };

	{
		p.schedule(
			[&]
			{
				std::this_thread::sleep_for(5s);

				std::cout << "task 1\n";

				total++;
			});

		p.schedule(
			[&]
			{
				std::this_thread::sleep_for(5s);

				std::cout << "task 2\n";

				total++;
			});

		p.schedule(
			[&]
			{
				std::this_thread::sleep_for(5s);

				std::cout << "task 3\n";

				total++;
			});
	}

	std::this_thread::sleep_for(1s);

	p.close();

	EXPECT_TRUE(total == 1);
}

TEST(shut, wait_all)
{
	using pool = threadpool::basic_thread_pool<threadpool::default_task, threadpool::fifo_scheduler,
											   threadpool::wait_for_all_task>;

	std::atomic_size_t total{};

	pool p{ 2 };

	p.schedule(
		[&]
		{
			std::this_thread::sleep_for(1s);

			std::cout << "task 1\n";

			total++;
		});

	p.schedule(
		[&]
		{
			std::this_thread::sleep_for(1s);

			std::cout << "task 2\n";

			total++;
		});

	p.schedule(
		[&]
		{
			std::this_thread::sleep_for(1s);

			std::cout << "task 3\n";

			total++;
		});

	p.schedule(
		[&]
		{
			std::this_thread::sleep_for(1s);

			std::cout << "task 4\n";

			total++;
		});

	p.schedule(
		[&]
		{
			std::this_thread::sleep_for(1s);

			std::cout << "task 5\n";

			total++;
		});

	p.close();

	EXPECT_TRUE(total == 5);
}

TEST(shut, wait_for)
{
	auto now = std::chrono::system_clock::now();

	using pool = threadpool::basic_thread_pool<threadpool::default_task, threadpool::fifo_scheduler,
											   threadpool::wait_for_active_task>;

	std::atomic_size_t total{};

	{
		pool p{ 2 };

		auto future = p.schedule(
			[&]
			{
				std::this_thread::sleep_for(5s);

				std::cout << "task 1\n";

				total++;
			});

		future.wait_for(2s);

		EXPECT_TRUE(total == 0);
	}
}