#include "./workspace.hpp"
#include <algorithm>
#include <iostream>

namespace app::workspace {
    using namespace core;

    Workspace::Workspace() {
        Workspace(0);
    }

    Workspace::Workspace(int id) {
        m_id = id;
        m_is_active = false;
        m_mode = MODE::TILED;
    }

    Workspace::~Workspace() {}

    void Workspace::OnEvent(event::IEvent& ev) {
        if(auto e = ev.As<window_service::NewWindowEvent>()) {
            return _HandleNewWindow(*e);
        }
        else if(auto e = ev.As<window_service::WindowDestroyedEvent>()) {
            return _HandleWindowDestroyed(*e);
        }
        else if(auto e = ev.As<keybinds_service::CurrWinCloseEvent>()) {
            return _HandleCurrWinClose(*e);
        }
        else if(auto e = ev.As<keybinds_service::CurrWinChangeWorkspaceEvent>()){
            return _HandleCurrWinChangeWorkspace(*e);
        }
        else if (auto e = ev.As<keybinds_service::ChangeWorkspaceEvent>()) {
            return _HandleChangeWorkspace(*e);
        }
        else if(auto e = ev.As<keybinds_service::CurrWinMoveEvent>()) {
            return _HandleCurrWinMove(*e);
        }
        else if(auto e = ev.As<keybinds_service::FocusOtherWinEvent>()) {
            return _HandleFocusOtherWin(*e);
        }
        else if(auto e = ev.As<keybinds_service::CurrWorkspaceSetMode>()) {
            return _HandleCurrWorkspaceSetMode(*e);
        }
    }

    void Workspace::_HandleWindowDestroyed(window_service::WindowDestroyedEvent ev) {
        _RemoveWindow(ev.hwnd);
    };

    void Workspace::_HandleCurrWinClose(keybinds_service::CurrWinCloseEvent ev) {
        if(!m_is_active) {
            return;
        }
        auto curr_window = application::Application::Get()
            .Service<window_service::WindowService>()
            .GetFocusedWindow();
        winapi_utils::window::destroy(curr_window);
    };

    void Workspace::_HandleCurrWinChangeWorkspace(keybinds_service::CurrWinChangeWorkspaceEvent ev) {
        auto curr_window = application::Application::Get()
            .Service<window_service::WindowService>()
            .GetFocusedWindow();

        if(ev.id == m_id) {
            _AddWindow(curr_window);
        } else {
            _RemoveWindow(curr_window);
        }
    };


    void Workspace::_HandleChangeWorkspace(keybinds_service::ChangeWorkspaceEvent ev) {
        if(ev.id == m_id) {
            _SetActive();
            _ResizeWindows();
        } else {
            _Close();
        }
    }

    void Workspace::_HandleCurrWinMove(keybinds_service::CurrWinMoveEvent ev) {
        if(!m_is_active) {
            return;
        }
        auto curr_window = application::Application::Get()
            .Service<window_service::WindowService>()
            .GetFocusedWindow();
        
        for(size_t i = 0; i < m_window_handles.size(); i++) {
            if(m_window_handles[i] != curr_window) {
                continue;
            }

            auto tmp = m_window_handles[i];
            switch (ev.dir) {
            case keybinds_service::DIRECTION::LEFT: {
                size_t prev_i = i == 0 ? 0 : i - 1;
                m_window_handles[i] = m_window_handles[prev_i];
                m_window_handles[prev_i] = tmp;
                break;
            }
            case keybinds_service::DIRECTION::RIGHT: {
                size_t next_i = i + 1 == m_window_handles.size() ? i : i + 1;
                m_window_handles[i] = m_window_handles[next_i];
                m_window_handles[next_i] = tmp;
                break;
            }
            default:
                break;
            }
        }
        _ResizeWindows();
    }

    void Workspace::_HandleFocusOtherWin(keybinds_service::FocusOtherWinEvent ev) {
        if(!m_is_active) {
            return;
        }
        auto curr_window = application::Application::Get()
            .Service<window_service::WindowService>()
            .GetFocusedWindow();
        
        for(size_t i = 0; i < m_window_handles.size(); i++) {
            if(m_window_handles[i] != curr_window) {
                continue;
            }

            switch (ev.dir) {
            case keybinds_service::DIRECTION::LEFT: {
                size_t prev_i = i == 0 ? 0 : i - 1;
                winapi_utils::window::focus(m_window_handles[prev_i]);
                break;
            }
            case keybinds_service::DIRECTION::RIGHT: {
                size_t next_i = i + 1 == m_window_handles.size() ? i : i + 1;
                winapi_utils::window::focus(m_window_handles[next_i]);
                break;
            }
            default:
                break;
            }
        }
    }

    void Workspace::_HandleCurrWorkspaceSetMode(keybinds_service::CurrWorkspaceSetMode ev) {
        if(!m_is_active) {
            return;
        }
        
        switch(ev.mode) {
        case 0:
            m_mode = MODE::TILED;
            break;
        case 1:
            m_mode = MODE::STACKED;
            break;
        }
        _ResizeWindows();
    }


    void Workspace::_HandleNewWindow(window_service::NewWindowEvent ev) {
        if(m_is_active) {
            _AddWindow(ev.hwnd);
        }
    }

    void Workspace::_AddWindow(winapi_utils::WINDOW_HANDLE hwnd) {
        auto it_is_new = std::find_if(m_window_handles.begin(), m_window_handles.end(), 
            [&hwnd] (winapi_utils::WINDOW_HANDLE w) { 
                return w == hwnd; 
            });

        if(it_is_new == m_window_handles.end()) {
            m_window_handles.emplace_back(hwnd);
            if(m_is_active) {
                _ResizeWindows();
                winapi_utils::window::focus(hwnd);
            }
        }
    }
    
    void Workspace::_RemoveWindow(winapi_utils::WINDOW_HANDLE hwnd) {
        winapi_utils::window::set_minimized(hwnd);
        std::erase_if(m_window_handles, [&hwnd] (winapi_utils::WINDOW_HANDLE w) { 
            return w == hwnd; 
        });

        if(m_is_active) {
            _ResizeWindows();
        }
    }

    void Workspace::_ResizeWindows() {
        int width = application::Application::Get().Service<window_service::WindowService>().GetScreenWidth();
        int height = application::Application::Get().Service<window_service::WindowService>().GetScreenHeight();

        size_t nb_windows = m_window_handles.size();
        for(size_t i = 0; i < nb_windows; i++) {
            if(m_mode == MODE::STACKED) {
                winapi_utils::window::set_fullscreen(m_window_handles[i]);
                continue;
            }

            int width_computed = width / nb_windows;
            int height_computed = height;
 
            winapi_utils::window::set_windowed(m_window_handles[i]);
            winapi_utils::window::set_pos(
                m_window_handles[i], 0,
                width_computed * i, 0,
                width_computed, height_computed,
                winapi_utils::window::FLAGS::NOZORDER
            );
        }
    }

    void Workspace::_Close() {
        if(m_is_active) {
            for(winapi_utils::WINDOW_HANDLE& hwnd : m_window_handles) {
                winapi_utils::window::set_minimized(hwnd);
            }
        }
        _SetInactive();
    }
    
    void Workspace::_SetActive() {
        m_is_active = true;
        bool has_windows = m_window_handles.size() >= 1;
        if(has_windows) {
            winapi_utils::window::focus(m_window_handles[0]);
        }
    }
    
    void Workspace::_SetInactive() {
        m_is_active = false;
    }
}


namespace app::workspace {
    using namespace core;

    WorkspacesService::WorkspacesService() {
        for(size_t i = 1; i <= NB_WORKSPACES; i += 1) {
            m_workspaces[i - 1] = Workspace(i); 
        }
        auto e = keybinds_service::ChangeWorkspaceEvent{.id = 1};
        m_workspaces[0].OnEvent(e);
        
        application::Application::Get()
            .Service<keybinds_service::KeybindsService>()
            .OnEvent([this] (event::IEvent& e) {
                for(auto& w : m_workspaces) {
                    w.OnEvent(e);
                }
            });
        
        application::Application::Get()
            .Service<window_service::WindowService>()
            .OnEvent([this] (event::IEvent& e) {
                for(auto& w : m_workspaces) {
                    w.OnEvent(e);
                }
            });
    };

    WorkspacesService::~WorkspacesService() {};
}