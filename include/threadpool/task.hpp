#pragma once
#include <functional>
#include "detail/basic_task.hpp"


namespace thread_pool
{
	using default_task = detail::basic_task<std::function<void()>>;

	using default_priority_task = detail::basic_priority_task<std::function<void()>>;
}