#include "./window_service.hpp"
#include <unordered_set>
#include <vector>
#include <ranges>

const int DEFAULT_NB_WINDOWS = 20;

namespace app::window_service {
    static core::event::OnEventCallback s_on_window_event_callback;

    WindowService::WindowService() {
        m_screen_width = core::winapi_utils::screen::get_width();
        m_screen_height = core::winapi_utils::screen::get_height();
        m_windows_register.reserve(DEFAULT_NB_WINDOWS);

        core::winapi_utils::events::on_window_destroyed([this] (core::winapi_utils::WINDOW_HANDLE hwnd) {
            if(!m_windows_register.contains(hwnd)) {
                return;
            }
            auto e = WindowDestroyedEvent{.hwnd = hwnd};
            s_on_window_event_callback(e);
            m_windows_register.erase(hwnd);
            core::winapi_utils::screen::disable_animations();
        });
        
        core::winapi_utils::events::on_window_created([this] (core::winapi_utils::WINDOW_HANDLE hwnd) {
            if(m_windows_register.contains(hwnd)) {
                return;
            }
            m_focused_window = hwnd;
            auto e = NewWindowEvent{.hwnd = hwnd};
            s_on_window_event_callback(e);
            m_windows_register.emplace(hwnd);
        });
    
        core::winapi_utils::events::on_window_focused([this] (core::winapi_utils::WINDOW_HANDLE hwnd) {
            if(!m_windows_register.contains(hwnd)) {
                return;
            }
            m_focused_window = hwnd;
            auto e = WindowFocusedEvent{.hwnd = hwnd};
            s_on_window_event_callback(e);
        });
    }

    WindowService::~WindowService() {}

    void WindowService::OnEvent(core::event::OnEventCallback callback) {
        s_on_window_event_callback = callback;

        for (auto hwnd : core::winapi_utils::window::get_all()) {
            m_windows_register.emplace(hwnd);
            auto e = NewWindowEvent{.hwnd = hwnd};
            s_on_window_event_callback(e);
        }
    }

    void WindowService::PollEvents() {
        core::winapi_utils::events::pool_once();
    }

    core::winapi_utils::WINDOW_HANDLE WindowService::GetFocusedWindow() {
        return m_focused_window;
    }

    int WindowService::GetScreenHeight() {
        return m_screen_height;
    }

    int WindowService::GetScreenWidth() {
        return m_screen_width;
    }
}