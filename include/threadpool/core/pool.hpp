#pragma once
#include "core.hpp"

namespace thread_pool
{
	template <typename _Task, template <typename> typename _Schedule, template <typename> typename _Shutdown>
	class basic_thread_pool
	{
	private:
		using core_t = impl::pool_core<_Task, _Schedule, _Shutdown>;
		using task_t = _Task;

	public:
		explicit basic_thread_pool()
			: core_ptr_(new core_t())
		{
			start_thread();
		}

		basic_thread_pool(std::size_t thread_size)
			: core_ptr_(new core_t(thread_size))
		{
			start_thread();
		}

		virtual ~basic_thread_pool()
		{
			close();
		}

	public:
		std::size_t size()
		{
			return core_ptr_->size();
		}

		template <typename _Func, typename... _Args>
		auto schedule(_Func&& f, _Args&&... args)
		{
			return core_ptr_->schedule(std::forward<_Func>(f), std::forward<_Args>(args)...);
		}

		bool empty()
		{
			return core_ptr_->empty();
		}

		void close()
		{
			return core_ptr_->close();
		}

	private:
		void start_thread()
		{
			return core_ptr_->start();
		}

	private:
		std::shared_ptr<core_t> core_ptr_;
	};
} // namespace thread_pool
