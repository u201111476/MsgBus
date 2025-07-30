#include <string>
#include <functional>
#include <map>
#include "Any.h"
#include "function_traits.h"

#include "ThreadPool.h"

using namespace std;

class MsgBus
{
public:

    static MsgBus& GetInst()
    {
        static MsgBus bus;
        return bus;
    }

	/// @brief 订阅消息
	/// @tparam F 消息处理函数模板
	/// @param f 消息处理函数签名
	/// @param topic 要订阅的消息
	/// @return 订阅该消息的订阅号
	template<typename F>
	uint32_t Subscribe(uint32_t topic, F&& f)
	{
		auto func = to_function(std::forward<F>(f));
		return Add(topic, std::move(func));
	}

	/// @brief 分发消息
	/// @param topic 要分发的消息号
	void SendReq(uint32_t topic)
	{
		using function_type = std::function<void()>;
		auto range = m_map.equal_range(topic);
		for(Iterater it=range.first; it!=range.second; ++it)
		{
			auto f = it->second.first.AnyCast<function_type>();
            if (topic > 0x3fff)    //放入线程池去执行
            {
                ThreadPool::GetInst().EnQueue(f);
            }
            else
				f();
		}
	}
	/// @brief 分发消息
	/// @tparam R 该消息的消息处理函数返回值类型模板
	/// @tparam ...Args 消息处理函数的参数模板
	/// @param ...args 实参
	/// @param topic 消息号
	template<typename R, typename... Args>
	void SendReq(Args&&... args, uint32_t topic)
	{
		using function_type = std::function<R(Args...)>;
		auto range = m_map.equal_range(topic);
		for(Iterater it=range.first; it!=range.second; ++it)
		{
			auto f = it->second.first.AnyCast<function_type>();
            if (topic & 0x1000)    //放入线程池去执行
            {
                ThreadPool::GetInst().EnQueue(f, std::forward<Args>(args)...);
            }
            else
                f(std::forward<Args>(args)...);
		}
	}
    //移除整个消息订阅
	//template<typename R, typename... Args>
	void RemoveTopic(uint32_t topic)
	{
		auto range = m_map.equal_range(topic);
		m_map.erase(range.first, range.second);
	}

    /// @brief 移除某个订阅号
    /// @param topic 订阅号针对的消息
    /// @param subsNum 要移除的订阅号
    void RemoveSubscribe(uint32_t topic, uint32_t subsNum)
    {
        auto range = m_map.equal_range(topic);
        for (auto it = range.first; it != range.second; ++it)
        {
            if (it->second.second == subsNum)
            {
                m_map.erase(it);
                break;              //必须马上break，删除了元素，迭代器不能再使用了
            }    
        }
    }

private:
	template<typename F>
	uint32_t Add(uint32_t topic, F&& f)
	{
        ++m_counts;
		m_map.emplace(topic, std::make_pair(std::forward<F>(f),m_counts.load()));
        return m_counts;
	}

private: 
	std::multimap<uint32_t, std::pair<Any,uint32_t>> m_map;
	typedef std::multimap<uint32_t, std::pair<Any, uint32_t>>::iterator Iterater;
    std::atomic<uint32_t> m_counts{ 0 };

protected:
    MsgBus() = default;
    MsgBus(const MsgBus&) = delete;
    MsgBus(MsgBus&&) = delete;
    MsgBus& operator=(const MsgBus&) = delete;
    MsgBus& operator=(MsgBus&&) = delete;
};
