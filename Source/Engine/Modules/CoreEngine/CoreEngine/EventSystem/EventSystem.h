#pragma once

#include <unordered_map>
#include <string>
#include <vector>

namespace Eclipse
{
	struct BaseEvent {
		virtual ~BaseEvent() = default;
	};

	template<typename... Args>
	struct Event final : public BaseEvent {
		Event() = default;

		Event(void(*_callback)(Args...))
			: callback(_callback) {
		}

		Event& operator=(const Event& other) {
			if (this == &other)
				return *this;

			callback = other.callback;
		}

		void Invoke(Args... args) {
			callback(args...);
		}

		void(*callback)(Args...);
	};

	using EventMap = std::vector<BaseEvent*>;

	class EventSystem
	{
	public:
		template<typename... Args>
		static inline Event<Args...>* Subscribe(const std::string& eventName, void(*callback)(Args...)) {
			EventMap& eMap = events[eventName];
			Event<Args...>* e = new Event<Args...>(callback);
			eMap.push_back(reinterpret_cast<BaseEvent*>(e));
			return e;
		}

		template<typename... Args>
		static inline void Unsubscribe(void(*callback)(Args...)) {
			for (auto& [eventName, eventMap] : events) {
				for (auto it = eventMap.begin(); it != eventMap.end();)
				{
					BaseEvent* base = *it;
					if (auto* e = dynamic_cast<Event<Args...>*>(base)) {
						if (e->callback == callback) {
							delete e;
							it = eventMap.erase(it);
							continue;
						}
						else it++;
					}
				}
			}
		}

		template<typename... Args>
		static inline void Unsubscribe(const std::string& eventName, void(*callback)(Args...)) {
			if (events.find(eventName) == events.end()) return;
			EventMap& eventMap = events[eventName];
			for (auto it = eventMap.begin(); it != eventMap.end();)
			{
				BaseEvent* base = *it;
				if (auto* e = dynamic_cast<Event<Args...>*>(base)) {
					if (e->callback == callback) {
						delete e;
						it = eventMap.erase(it);
						continue;
					}
					else it++;
				}
			}
		}

		template<typename... Args>
		static inline void Trigger(const std::string& eventName, Args... args) {
			EventMap& eMap = events[eventName];

			for (BaseEvent* e : eMap)
			{
				reinterpret_cast<Event<Args...>*>(e)->callback(args...);
			}
		}

		static inline std::unordered_map<std::string, EventMap> events;
	};
}