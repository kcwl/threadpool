#pragma once
#include <future>
#include "task.hpp"


namespace thread_pool
{
	template<typename _Task>
	struct use_task
	{
		template<typename _Schedule, typename _Func, typename... _Args>
		auto operator()(_Schedule& schedule, _Func&& func, _Args&&... args) -> std::future<std::invoke_result_t<_Func, _Args...>>
		{
			using return_type = std::invoke_result_t<_Func, _Args...>;

			auto task_ptr = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<_Func>(func), std::forward<_Args>(args)...));

			auto future = task_ptr->get_future();

			default_task task([tk = std::move(task_ptr)]{ (*tk)();});

			schedule.push(std::move(task));

			return future;
		}
	};

	template<>
	struct use_task<default_priority_task>
	{
		template<typename _Schedule, typename _Func, typename... _Args>
		auto operator()(_Schedule& schedule, _Func&& func, _Args&&... args, std::size_t&& priority) -> std::future<std::invoke_result_t<_Func, _Args...>>&&
		{
			using return_type = std::invoke_result_t<_Func, _Args...>;

			auto task_ptr = std::make_shared<std::packaged_task<return_type(_Args...)>>(std::bind(std::forward<_Func>(func), std::forward<_Args>(args)...));

			auto future = task_ptr->get_future();

			default_priority_task task([tk = std::move(task_ptr)]{ (*tk)(); }, std::forward<std::size_t>(priority));

			schedule.push(std::move(task));

			return std::move(future);
		}
	};

	template<typename _Task, typename _Func, typename... _Args>
	struct task_result
	{
		using type = std::invoke_result_t<_Func, _Args...>;
	};

	template<typename _Func, typename... _Args>
	struct task_result<default_priority_task, _Func, _Args...>
	{
		template<typename... _Args1>
		constexpr static auto task_type(_Func&&, _Args1&&..., std::size_t) ->std::invoke_result_t<_Func, _Args1...>
		{
			return {};
		}

		using type = decltype(task_type(std::declval<_Func>(), std::declval<_Args...>()));
	};

	template<typename _Task, typename _Func, typename... _Args>
	using task_result_t = task_result<_Task, _Func, _Args...>::type;
}