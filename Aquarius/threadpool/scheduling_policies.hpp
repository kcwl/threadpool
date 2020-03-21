#pragma once
#include <queue>
#include <functional>
#include "task.hpp"


namespace Aquarius
{
	namespace thread_pool
	{
		template<class Task = priority_task>
		class priority_scheduler
		{
		private:
            using task_type = Task;
		public:
			priority_scheduler() = default;


		public:
			bool empty()
			{
				return container_.empty();
			}

			const task_type& top()
			{
				return container_.top();
			}

			void pop()
			{
				return container_.pop();
			}

			bool push(const task_type& task)
			{
				container_.push(task);

				return true;
			}

            void clear()
            {
                while(!container_.empty())
                {
                    container_.pop();
                }
            }

		private:
			std::priority_queue<task_type> container_;
		};


        template <typename Task = task_func>
        class fifo_scheduler
        {
        public:
            using task_type = Task;
        public:
            bool push(task_type const& task)
            {
                container_.push_back(task);
                return true;
            }

            void pop()
            {
                container_.pop_front();
            }

            task_type const& top() const
            {
                return container_.front();
            }

            size_t size() const
            {
                return container_.size();
            }

            bool empty() const
            {
                return container_.empty();
            }

            void clear()
            {
                container_.clear();
            }

        protected:
            std::deque<task_type> container_;
        };

	}
}