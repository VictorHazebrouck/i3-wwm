#include "./winapi_utils.hpp"
#include <windows.h>
#include <iostream>

namespace core::winapi_utils::window {
    WINDOW_HANDLE get_focused() {
        return GetActiveWindow();
    };
    
    std::vector<WINDOW_HANDLE> get_all() {
        std::vector<WINDOW_HANDLE> windows;
        
        auto callback = [](WINDOW_HANDLE hwnd, LPARAM user_data) -> BOOL {
            auto* windows = reinterpret_cast<std::vector<WINDOW_HANDLE>*>(user_data);
            if (is_real(hwnd)) {
                windows->push_back(hwnd);
            }
            return TRUE;
        };
        
        LPARAM user_data = reinterpret_cast<LPARAM>(&windows);
        EnumWindows(callback, user_data);
        
        return windows;
    }
    
    std::string get_class_name(WINDOW_HANDLE hwnd) {
        if (!hwnd || !IsWindow(hwnd)) {
            return {};
        }
        
        char class_name[256];
        int len = GetClassNameA(hwnd, class_name, sizeof(class_name));
        
        return std::string(class_name, len);
    }
    
    std::string get_title(WINDOW_HANDLE hwnd) {
        if (!hwnd || !IsWindow(hwnd)) {
            return {};
        }
        
        char title[256];
        int len = GetWindowTextA(hwnd, title, sizeof(title));
        
        return  std::string(title, len);
    }
    
    bool is_real(WINDOW_HANDLE hwnd) {
        if(!hwnd || !IsWindow(hwnd)) {
            return false;
        }
        if (GetWindow(hwnd, GW_OWNER)) {
            return false;
        }
        if (GetParent(hwnd)) {
            return false;
        }
        if (!IsWindowVisible(hwnd)) {
            return false;
        }
        
        LONG ex_style = GetWindowLong(hwnd, GWL_EXSTYLE);
        if (ex_style & WS_EX_TOOLWINDOW) {
            return false;
        }
        
        std::string class_name = get_class_name(hwnd);
        std::string title = get_title(hwnd);

        if(class_name.empty() || title.empty()) {
            return false;
        }
        if (!strcmp(class_name.data(), "ApplicationFrameWindow")) {
            return false;
        }
        if (!strcmp(class_name.data(), "Windows.UI.Core.CoreWindow")) {
            return false;
        }
        if (!strcmp(class_name.data(), "XamlExplorerHostIslandWindow")) {
            return false;
        }
        
        return true;
    };

    void destroy(WINDOW_HANDLE hwnd) {
        if (!hwnd || !IsWindow(hwnd)) {
            return;
        }
        
        SendMessage(hwnd, WM_CLOSE, 0, 0);
    }
    
    void focus(WINDOW_HANDLE hwnd) {
        if (!hwnd || !IsWindow(hwnd)) {
            return;
        }
        
        if (IsIconic(hwnd)) {
            ShowWindow(hwnd, SW_RESTORE);
        } else {
            ShowWindow(hwnd, SW_SHOW);
        }
        
        HWND foreground = GetForegroundWindow();
        DWORD current_thread = GetCurrentThreadId();
        DWORD target_thread = GetWindowThreadProcessId(hwnd, nullptr);
        DWORD foreground_thread = GetWindowThreadProcessId(foreground, nullptr);
        
        AttachThreadInput(current_thread, target_thread, TRUE);
        AttachThreadInput(current_thread, foreground_thread, TRUE);
        
        SetForegroundWindow(hwnd);
        SetFocus(hwnd);
        BringWindowToTop(hwnd);
        SetActiveWindow(hwnd);
        
        AttachThreadInput(current_thread, target_thread, FALSE);
        AttachThreadInput(current_thread, foreground_thread, FALSE);
    }
    
    void set_fullscreen(WINDOW_HANDLE hwnd) {
        if(!hwnd || !IsWindow(hwnd)) {
            return;
        }
        
        ShowWindow(hwnd, SW_MAXIMIZE);
    }
    
    void set_minimized(WINDOW_HANDLE hwnd) {
        if(!hwnd || !IsWindow(hwnd)) {
            return;
        }
        
        ShowWindow(hwnd, SW_MINIMIZE);
    }
    
    void set_windowed(WINDOW_HANDLE hwnd) {
        if(!hwnd || !IsWindow(hwnd)) {
            return;
        }
        
        // @todo need to do it twice else it won't trigger windowed mode for some reason ?
        // should the dupe be here or in places we use it ?
        ShowWindow(hwnd, SW_RESTORE);
        ShowWindow(hwnd, SW_RESTORE);
    }
    
    void set_pos(
        WINDOW_HANDLE hwnd, 
        WINDOW_HANDLE hwnd_insert_after, 
        int x, int y, 
        int width, int height,
        FLAGS::Enum flags
    ) {
        if (!hwnd || !IsWindow(hwnd)) {
            return;
        }
        
        // @todo there's apparently some weird padding/margin/spacing stuff decided by windows, need 
        // understand all this all works, for now good old GPT make somehting that seems to work.

        RECT winRect;
        RECT clientRect;
        GetWindowRect(hwnd, &winRect);
        GetClientRect(hwnd, &clientRect);

        // Convert client origin (0,0) to screen coords
        POINT clientOrigin = { 0, 0 };
        ClientToScreen(hwnd, &clientOrigin);

        // Offsets of client origin inside window (how far client area is from window's top-left)
        int offsetX = clientOrigin.x - winRect.left;
        int offsetY = clientOrigin.y - winRect.top;

        // Current sizes
        int currentWindowWidth  = winRect.right - winRect.left;
        int currentWindowHeight = winRect.bottom - winRect.top;
        int currentClientWidth  = clientRect.right - clientRect.left;
        int currentClientHeight = clientRect.bottom - clientRect.top;

        // Compute how much extra space the window has (non-client extents)
        int extraWidth  = currentWindowWidth  - currentClientWidth;
        int extraHeight = currentWindowHeight - currentClientHeight;

        // Desired window size = requested client size + non-client extents (measured above)
        int newWindowWidth  = width  + extraWidth;
        int newWindowHeight = height + extraHeight;

        // Desired window top-left so that client top-left becomes clientX,clientY
        int newWindowX = x - offsetX;
        int newWindowY = y - offsetY;

        SetWindowPos(
            hwnd, hwnd_insert_after,
            newWindowX, newWindowY,
            newWindowWidth, newWindowHeight,
            flags
        );
    }

    void debug(WINDOW_HANDLE hwnd) {
        std::cout
            << "Title: " << get_title(hwnd).data() << "\n"
            << "ClassName: " << get_class_name(hwnd).data() << "\n";
    }
}