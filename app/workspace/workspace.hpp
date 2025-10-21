#pragma once

// #include <memory>
// #include "core/winapi_utils/winapi_utils.hpp"
// #include "core/event/event.hpp"
// #include "core/application/application.hpp"
// #include "app/input_manager/input_manager.hpp"

// namespace core::application { class Application; }

#include "core/event/event.hpp"
#include "core/winapi_utils/winapi_utils.hpp"
#include "core/application/application.hpp"
#include "core/service/service.hpp"
#include "app/window_service/window_service.hpp"
#include "app/keybinds_service/keybinds_service.hpp"
#include <array>

namespace app::workspace {
    namespace MODE {
        enum Enum : unsigned int {
            TILED = 0,
            STACKED = 1,
        };
    }

    class Workspace {
    public:
        Workspace();
        Workspace(int id);
        ~Workspace();
        
        void OnEvent(core::event::IEvent& event);
    private:
        void _HandleNewWindow(window_service::NewWindowEvent ev);
        void _HandleWindowDestroyed(window_service::WindowDestroyedEvent ev);
        void _HandleCurrWinClose(keybinds_service::CurrWinCloseEvent ev);
        void _HandleCurrWinChangeWorkspace(keybinds_service::CurrWinChangeWorkspaceEvent ev);
        void _HandleChangeWorkspace(keybinds_service::ChangeWorkspaceEvent ev);
        void _HandleCurrWinMove(keybinds_service::CurrWinMoveEvent ev);
        void _HandleFocusOtherWin(keybinds_service::FocusOtherWinEvent ev);
        void _HandleCurrWorkspaceSetMode(keybinds_service::CurrWorkspaceSetMode ev);

        void _SetActive();
        void _SetInactive();
        
        void _ResizeWindows();
        void _Close();
        
        void _AddWindow(core::winapi_utils::WINDOW_HANDLE hwnd);
        void _RemoveWindow(core::winapi_utils::WINDOW_HANDLE hwnd);
    private:
        int m_id;
        bool m_is_active;
        MODE::Enum m_mode;
        std::vector<core::winapi_utils::WINDOW_HANDLE> m_window_handles;
    };


    static const size_t NB_WORKSPACES = 10;
    
    class WorkspacesService : public core::service::Service<WorkspacesService> {
    public:
        WorkspacesService();
        ~WorkspacesService();
    private:
        std::array<Workspace, NB_WORKSPACES> m_workspaces;
    };
}