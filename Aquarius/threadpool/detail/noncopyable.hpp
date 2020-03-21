#pragma once

namespace Aquarius
{
	namespace thread_pool
	{
		namespace detail
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
		}
	}
}