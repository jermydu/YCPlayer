#ifndef _YCQUEUE_H
#define _YCQUEUE_H

#include <queue>
#include <mutex>
#include <memory>

namespace YCLIB
{
	template<typename T>
	class YCQueue
	{
	public:
		YCQueue() = default;
		~YCQueue() = default;

		// 禁用拷贝构造和赋值
		YCQueue(const YCQueue&) = delete;
		YCQueue& operator=(const YCQueue&) = delete;

		// 移动构造/赋值可以按需放开，这里保持禁止
		YCQueue(YCQueue&&) = delete;
		YCQueue& operator=(YCQueue&&) = delete;

		void Push(std::shared_ptr<T> value)
		{
			std::lock_guard<std::mutex> lock(mutex_);
			queue_.push(value);
		}
		bool Pop(std::shared_ptr<T>& out)
		{
			std::lock_guard<std::mutex> lock(mutex_);
			if (queue_.empty())
				return false;
			out = queue_.front();
			queue_.pop();
			return true;
		}
		bool IsEmpty() const
		{
			std::lock_guard<std::mutex> lock(mutex_);
			return queue_.empty();
		}
		int Size() const
		{
			std::lock_guard<std::mutex> lock(mutex_);
			return static_cast<int>(queue_.size());
		}
		void Clear()
		{
			std::lock_guard<std::mutex> lock(mutex_);
			while (!queue_.empty())
			{
				queue_.pop();
			}
		}
	private:
		mutable std::mutex mutex_;
		std::queue<std::shared_ptr<T>> queue_;
	};
}

#endif // !_YCQUEUE_H
