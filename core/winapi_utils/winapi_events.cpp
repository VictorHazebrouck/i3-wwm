#include "./winapi_utils.hpp"
#include <windows.h>
#include <iostream>

namespace core::winapi_utils::events {
    static WindowCallback *s_on_window_created_callback = nullptr;

    void CALLBACK winevent_obj_shown_proc(HWINEVENTHOOK, DWORD, HWND hwnd, LONG, LONG, DWORD, DWORD) {
        if(!core::winapi_utils::window::is_real(hwnd)) {
            return;
        }
        if(!s_on_window_created_callback) {
            return;
        }
        (*s_on_window_created_callback)(hwnd);
    }

    CleanupFn on_window_created(WindowCallback callback) {
        if(s_on_window_created_callback) {
            std::cerr << "Error: `on_window_created` callback already registered!\n";
            std::terminate();
        }

        s_on_window_created_callback = new WindowCallback(std::move(callback));

        HWINEVENTHOOK hook = SetWinEventHook(EVENT_OBJECT_SHOW, EVENT_OBJECT_SHOW, NULL, winevent_obj_shown_proc, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);

        return [hook]() {
            UnhookWinEvent(hook);
            delete s_on_window_created_callback;
        };
    };
}

namespace core::winapi_utils::events {
    static WindowCallback *s_on_window_destroyed_callback = nullptr;

    void CALLBACK winevent_obj_destroyed_proc(
        HWINEVENTHOOK, DWORD,
        HWND hwnd,
        LONG, LONG, DWORD, DWORD
    ) {
        // if(!core::winapi_utils::window::is_real(hwnd)) {
        //     return;
        // }
        if(!s_on_window_destroyed_callback) {
            return;
        }
        (*s_on_window_destroyed_callback)(hwnd);
    }

    CleanupFn on_window_destroyed(WindowCallback callback) {
        if(s_on_window_destroyed_callback) {
            std::cerr << "Error: `on_window_destroyed` callback already registered!\n";
            std::terminate();
        }

        s_on_window_destroyed_callback = new WindowCallback(std::move(callback));

        HWINEVENTHOOK hook = SetWinEventHook(
            EVENT_OBJECT_DESTROY, EVENT_OBJECT_DESTROY,
            NULL,
            winevent_obj_destroyed_proc,
            0, 0,
            WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS
        );

        return [hook]() {
            UnhookWinEvent(hook);
            delete s_on_window_destroyed_callback;
        };
    };
}

namespace core::winapi_utils::events {
    static WindowCallback *s_on_window_focused_callback = nullptr;

    void CALLBACK winevent_obj_focused_proc(
        HWINEVENTHOOK, DWORD, HWND hwnd, LONG, LONG, DWORD, DWORD
    ) {
        if (!core::winapi_utils::window::is_real(hwnd)) {
            return;
        }
        if (!s_on_window_focused_callback) {
            return;
        }
        (*s_on_window_focused_callback)(hwnd);
    }

    CleanupFn on_window_focused(WindowCallback callback) {
        if (s_on_window_focused_callback) {
            std::cerr << "Error: `on_window_focused` callback already registered!\n";
            std::terminate();
        }

        s_on_window_focused_callback = new WindowCallback(std::move(callback));

        HWINEVENTHOOK hook = SetWinEventHook(
            EVENT_OBJECT_FOCUS, EVENT_OBJECT_FOCUS,
            NULL,
            winevent_obj_focused_proc,
            0, 0,
            WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS
        );

        return [hook]() {
            UnhookWinEvent(hook);
            delete s_on_window_focused_callback;
        };
    };
}

namespace core::winapi_utils::events {
    static KeyCallback *s_on_key_down_callback = nullptr;

    static LRESULT CALLBACK winevent_key_down_proc(int key_code, WPARAM w_param, LPARAM l_param) {
        if ((w_param == WM_SYSKEYDOWN || w_param == WM_KEYDOWN) && s_on_key_down_callback) {
            KBDLLHOOKSTRUCT* key = reinterpret_cast<KBDLLHOOKSTRUCT*>(l_param);
            return (*s_on_key_down_callback)(key->vkCode);
        }
        return CallNextHookEx(nullptr, key_code, w_param, l_param);
    }

    CleanupFn on_key_down(KeyCallback callback) {
        if(s_on_key_down_callback) {
            std::cerr << "Error: `on_key_down` callback already registered!\n";
            std::terminate();
        }

        s_on_key_down_callback = new KeyCallback(std::move(callback));

        HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, winevent_key_down_proc, nullptr, 0);

        return [hook]() {
            UnhookWindowsHookEx(hook);
            delete s_on_key_down_callback;
        };
    };
}


namespace core::winapi_utils::events {
    static KeyCallback *s_on_key_up_callback = nullptr;

    static LRESULT CALLBACK winevent_key_up_proc(int key_code, WPARAM w_param, LPARAM l_param) {
        if ((w_param == WM_SYSKEYUP || w_param == WM_KEYUP) && s_on_key_up_callback) {
            KBDLLHOOKSTRUCT* key = reinterpret_cast<KBDLLHOOKSTRUCT*>(l_param);
            return (*s_on_key_up_callback)(key->vkCode);
        }
        return CallNextHookEx(nullptr, key_code, w_param, l_param);
    }

    CleanupFn on_key_up(KeyCallback callback) {
        if(s_on_key_up_callback) {
            std::cerr << "Error: `on_key_up` callback already registered!\n";
            std::terminate();
        }

        s_on_key_up_callback = new KeyCallback(std::move(callback));

        HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, winevent_key_up_proc, nullptr, 0);

        return [hook]() {
            UnhookWindowsHookEx(hook);
            delete s_on_key_up_callback;
        };
    };
}

namespace core::winapi_utils::events {
    void start_msg_loop(bool *should_stop) {
        MSG msg;
        while (false == (*should_stop)) {
            GetMessage(&msg, NULL, 0, 0);
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    };

    void pool_once() {
        MSG msg;
        PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
