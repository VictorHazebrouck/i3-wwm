#pragma once

#include <vector> 
#include <string>
#include <functional>
// barrel export ish the relevant windows.h type definitions 

typedef unsigned int UINT;

struct HWND__;
typedef HWND__* HWND;

namespace core::winapi_utils{
    using WINDOW_HANDLE = HWND;

    namespace window {
        namespace Z_HELPER {
            inline const WINDOW_HANDLE TOP = (WINDOW_HANDLE)0;
            inline const WINDOW_HANDLE BOTTOM = (WINDOW_HANDLE)1;
            inline const WINDOW_HANDLE TOPMOST = (WINDOW_HANDLE)-1;
            inline const WINDOW_HANDLE NOTOPMOST = (WINDOW_HANDLE)-2;
        }
        
        namespace FLAGS {
            enum Enum : UINT {
                NOSIZE = 0x0001,
                NOMOVE = 0x0002,
                NOZORDER = 0x0004,
                NOREDRAW = 0x0008,
                NOACTIVATE = 0x0010,
                FRAMECHANGED = 0x0020,
                SHOWWINDOW = 0x0040,
                HIDEWINDOW = 0x0080,
                NOCOPYBITS = 0x0100,
                NOOWNERZORDER = 0x0200,
                NOSENDCHANGING = 0x0400,
                DRAWFRAME = FLAGS::FRAMECHANGED,
                NOREPOSITION = FLAGS::NOOWNERZORDER,
                DEFERERASE = 0x2000,
                ASYNCWINDOWPOS = 0x4000,
            };
        }
        
        WINDOW_HANDLE get_focused();
        std::vector<WINDOW_HANDLE> get_all();
        std::string get_title(WINDOW_HANDLE hwnd);
        std::string get_class_name(WINDOW_HANDLE hwnd);
        
        bool is_real(WINDOW_HANDLE hwnd);
        
        void destroy(WINDOW_HANDLE hwnd);
        void focus(WINDOW_HANDLE hwnd);
        
        void set_windowed(WINDOW_HANDLE hwnd);
        void set_fullscreen(WINDOW_HANDLE hwnd);
        void set_minimized(WINDOW_HANDLE hwnd);
        void set_pos(
            WINDOW_HANDLE hwnd,
            WINDOW_HANDLE hwnd_insert_after, 
            int x, int y, 
            int width, int height,
            FLAGS::Enum window_flags
        );

        void debug(WINDOW_HANDLE hwnd);
    }
    
    namespace screen {
        int get_width();
        int get_height();
        void disable_animations();
    }

    namespace events {
        namespace PROPAGATE_KEY_EV {
            enum Enum : bool {
                STOP = true,
                CONTINUE = false,
            };
        }

        using WindowCallback = std::function<void(WINDOW_HANDLE)>;
        // return true to not propagate the key event to the system and apps
        // i.e: if win key set as mod, return true to prevent default win key behaviour
        using KeyCallback = std::function<PROPAGATE_KEY_EV::Enum(int)>;
        using CleanupFn = std::function<void()>;

        CleanupFn on_window_created(WindowCallback callback);
        CleanupFn on_window_destroyed(WindowCallback callback);
        CleanupFn on_window_focused(WindowCallback callback);
        CleanupFn on_key_down(KeyCallback callback);
        CleanupFn on_key_up(KeyCallback callback);

        void start_msg_loop(bool *should_stop);
        void pool_once();
    }
}
