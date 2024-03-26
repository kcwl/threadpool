#pragma once

namespace thread_pool
{
	namespace impl
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
	} // namespace impl
} // namespace thread_pool