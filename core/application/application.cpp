#include "./application.hpp"
#include <iostream>
#include <vector>
#include <ranges>
#include <thread>
#include <chrono>

static core::application::Application* s_application = nullptr;

namespace core::application {
    Application::Application() {
        s_application = this;
        m_should_quit = false;
    }

    Application::~Application() {}

    Application& Application::Get() {
        return *s_application;
    }

    void Application::Start() {
        while (!m_should_quit) {
            for(auto& [_, service] : m_services_registery) {
                service->PollEvents();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    }
}