#pragma once
#include <deque>
#include <random>

namespace threadpool
{
	template <typename _Task>
	class multi_fifo_schedule
	{
	public:
		multi_fifo_schedule(std::size_t queue_size = std::thread::hardware_concurrency())
			: queues_(queue_size)
		{
		}

		void push(_Task&& t)
		{
			auto index = find_less();

			queues_[index].push(std::forward<_Task>(t));
		}

		bool pop(_Task& t)
		{
			std::random_device rd;
			std::mt19937 gen(rd());

			std::uniform_int_distribution<> dis(0, static_cast<int>(queues_.size() - 1));

			int id = dis(gen);

			while (queues_.at(id).empty())
			{
				++id > queues_.size() - 1 ? id = 0 : 0;
			}

			t = queues_.at(id).front();

			queues_.at(id).pop();

			return true;
		}

		bool empty()
		{
			return std::all_of(queues_.begin(), queues_.end(), [](auto queue) { return queue.empty(); });
		}

		std::size_t size()
		{
			return queues_.size();
		}

	private:
		std::size_t find_less()
		{
			std::size_t total{};
			int index = 0;

			for (std::size_t i = 0; i < queues_.size(); ++i)
			{
				if (queues_[i].size() > total)
					continue;

				total = queues_[i].size();

				index = i;
			}

			return index;
		}

	private:
		std::vector<std::queue<_Task>> queues_;
	};
}