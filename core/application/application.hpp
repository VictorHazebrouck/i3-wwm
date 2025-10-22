#pragma once

#include "core/service/service.hpp"
#include <unordered_map>
#include <memory>

namespace core::application {
    class Application {
    public:
        Application();
        ~Application();
        static Application& Get();

        void Start();

        template<typename T, typename... Args>
        requires std::is_base_of_v<service::IService, T>
        void RegisterService(Args&&... args) {
            auto service_instance = std::make_unique<T>(std::forward<Args>(args)...);
            m_services_registery.emplace(T::s_type_id, std::move(service_instance));
        }

        template<typename T>
        requires std::is_base_of_v<service::IService, T>
        T& Service() {
            auto it = m_services_registery.find(T::s_type_id);
            service::IService* raw_ptr = it->second.get();
            return *static_cast<T*>(raw_ptr);
        }
    private:
        bool m_should_quit;
        std::unordered_map<int, std::unique_ptr<core::service::IService>> m_services_registery;
    };
}
