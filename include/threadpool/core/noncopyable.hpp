#pragma once

namespace thread_pool
{
	class noncopyable
	{
	protected:
		noncopyable() = default;
		~noncopyable() = default;

	private:
		noncopyable(const noncopyable&) = delete;
		noncopyable& operator=(const noncopyable&) = delete;
	};
} // namespace thread_pool