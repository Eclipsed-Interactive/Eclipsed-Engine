#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <utility>

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
            return *this;
        }

        void Invoke(Args... args) {
            if (callback)
                callback(std::forward<Args>(args)...);
        }

        void(*callback)(Args...) = nullptr;
    };

    using EventMap = std::vector<BaseEvent*>;

    class EventSystem
    {
    public:
        template<typename... Args>
        static Event<Args...>* Subscribe(const std::string& eventName, void(*callback)(Args...)) {
            EventMap& eMap = events[eventName];

            auto* e = new Event<Args...>(callback);
            eMap.push_back(e); // no cast needed

            return e;
        }

        template<typename... Args>
        static void Unsubscribe(void(*callback)(Args...)) {
            for (auto& [eventName, eventMap] : events) {
                for (auto it = eventMap.begin(); it != eventMap.end(); )
                {
                    if (auto* e = dynamic_cast<Event<Args...>*>(*it)) {
                        if (e->callback == callback) {
                            delete e;
                            it = eventMap.erase(it);
                            continue;
                        }
                    }
                    ++it;
                }
            }
        }

        template<typename... Args>
        static void Unsubscribe(const std::string& eventName, void(*callback)(Args...)) {
            auto found = events.find(eventName);
            if (found == events.end()) return;

            EventMap& eventMap = found->second;

            for (auto it = eventMap.begin(); it != eventMap.end(); )
            {
                if (auto* e = dynamic_cast<Event<Args...>*>(*it)) {
                    if (e->callback == callback) {
                        delete e;
                        it = eventMap.erase(it);
                        continue;
                    }
                }
                ++it;
            }
        }

        template<typename... Args>
        static void Trigger(const std::string& eventName, Args&&... args) {
            auto found = events.find(eventName);
            if (found == events.end()) return;

            EventMap& eMap = found->second;

            for (BaseEvent* base : eMap)
            {
                if (auto* e = dynamic_cast<Event<Args...>*>(base)) {
                    e->Invoke(std::forward<Args>(args)...);
                }
            }
        }

        static inline std::unordered_map<std::string, EventMap> events;
    };
}