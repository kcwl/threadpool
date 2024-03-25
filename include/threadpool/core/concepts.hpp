#pragma once
#include <future>
#include <memory>
#include <type_traits>

namespace thread_pool
{
	namespace impl
	{
		template <typename _Func, typename... _Args>
		struct task_result
		{
			using type = std::invoke_result_t<_Func, _Args...>;
		};
	} // namespace impl

	template <typename _Func, typename... _Args>
	using task_result_t = impl::task_result<_Func, _Args...>::type;

	template <typename _Task>
	struct use_task
	{
		template <typename _Schedule, typename _Func, typename... _Args>
		auto operator()(_Schedule& schedule, _Func&& func, _Args&&... args)
			-> std::future<std::invoke_result_t<_Func, _Args...>>
		{
			using return_type = std::invoke_result_t<_Func, _Args...>;

			auto task_ptr = std::make_shared<std::packaged_task<return_type()>>(
				std::bind(std::forward<_Func>(func), std::forward<_Args>(args)...));

			auto future = task_ptr->get_future();

			_Task task([tk = std::move(task_ptr)] { (*tk)(); });

			schedule.push(std::move(task));

			return future;
		}
	};
} // namespace thread_pool