#pragma once

#include <unordered_set>
#include "core/event/event.hpp"
#include "core/winapi_utils/winapi_utils.hpp"
#include "core/service/service.hpp"

namespace app::window_service {
    struct NewWindowEvent : core::event::Event<NewWindowEvent> { 
        core::winapi_utils::WINDOW_HANDLE hwnd;
    };
    struct WindowDestroyedEvent : core::event::Event<WindowDestroyedEvent> {
        core::winapi_utils::WINDOW_HANDLE hwnd;
    };
    struct WindowFocusedEvent : core::event::Event<WindowFocusedEvent> {
        core::winapi_utils::WINDOW_HANDLE hwnd;
    };

    class WindowService : public core::service::Service<WindowService> {
    public:
        WindowService();
        ~WindowService();

        void OnEvent(core::event::OnEventCallback callback);
        void PollEvents() override;

        core::winapi_utils::WINDOW_HANDLE GetFocusedWindow();
        int GetScreenWidth();
        int GetScreenHeight();
    private:
        core::winapi_utils::WINDOW_HANDLE m_focused_window;
        // @todo window.h OBJECT_CREATED event too hard to filter out real windows, so OBJECT_SHOWN is used
        // thus we have to keep track of already tracked windows to avoid falsly triggering NewWindowEvent 
        std::unordered_set<core::winapi_utils::WINDOW_HANDLE> m_windows_register;
        int m_screen_width;
        int m_screen_height;
    };
}
