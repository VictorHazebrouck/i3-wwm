#pragma once

inline int generate_s_type_id() {
    static int counter = 0;
    counter += 1;
    return counter;
}

namespace core::service {
    class IService {
    public:
        virtual ~IService() = default;
        virtual void Start() {}
        virtual void PollEvents() {}
    };

    template<typename T>
    class Service : public IService {
    public:
        inline static const int s_type_id = generate_s_type_id();
    };
}

