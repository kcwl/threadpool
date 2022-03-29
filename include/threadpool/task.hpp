#pragma once
#include <functional>
#include "detail/basic_task.hpp"

namespace thread_pool
{
	using task = std::function<void()>;

	using priority_task = detail::basic_task<task>;
}