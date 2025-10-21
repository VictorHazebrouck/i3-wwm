#pragma once

#include <functional>
#include <memory>

namespace core::event {
    struct IEvent {
        constexpr IEvent(int id)
            :type_id(id) {};
        const int type_id;

        template<typename T>
        requires std::is_base_of_v<IEvent, T>
        bool Is() {
            static_assert(std::is_base_of<IEvent, T>(), "T must derive from IEvent");
            return T::s_type_id == this->type_id;
        }
        
        template<typename T>
        requires std::is_base_of_v<IEvent, T>
        T* As() {
            bool is_requested_t = Is<T>();
            return is_requested_t ? static_cast<T*>(this) : nullptr;
        }
    };
    
    inline int generate_s_type_id() {
        static int counter = 0;
        counter += 1;
        return counter;
    }
    
    template<typename D> // forcefully recompute static stuff for each derived instance 
    struct Event : IEvent {
        Event()
            :IEvent(s_type_id) {};
        inline static const int s_type_id = generate_s_type_id();
    };

    using OnEventCallback = std::function<void(IEvent&)>;

    // template<typename T>
    // requires std::is_aggregate_v<T>
    // std::shared_ptr<T> make(T&& value) {
    //     return std::make_shared<T>(std::forward<T>(value));
    // }
}

// EXAMLPE USAGE
#if false
void example() {
    struct FooEvent : event::Event<FooEvent> { int data; };
    struct BarEvent : event::Event<BarEvent> { int data_2; };
    
    auto foo_e = FooEvent{.data = 69};
    event::EventBase& ev = foo_e;

    if(auto foo = ev.As<FooEvent>()) {
        foo->data;
    }
    else if(auto bar = ev.As<BarEvent>()) {
        bar->data_2;
    }
}
#endif
