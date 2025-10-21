#pragma once

#include "core/event/event.hpp"
#include "core/service/service.hpp"
#include "core/application/application.hpp"
#include "app/input_service/input_service.hpp"

namespace app::keybinds_service {
    namespace DIRECTION {
        enum Enum : int {
            NOOP = 0,
            LEFT = 1,
            RIGHT = 2,
            TOP = 3,
            BOTTOM = 4,
        };
    }

    struct ChangeWorkspaceEvent : core::event::Event<ChangeWorkspaceEvent> { 
        int id; 
    };
    struct CurrWinChangeWorkspaceEvent : core::event::Event<CurrWinChangeWorkspaceEvent> { 
        int id;
    };
    struct CurrWinMoveEvent : core::event::Event<CurrWinMoveEvent> {
        DIRECTION::Enum dir;
    };
    struct FocusOtherWinEvent : core::event::Event<FocusOtherWinEvent> { 
        DIRECTION::Enum dir;
    };
    struct CurrWinCloseEvent : core::event::Event<CurrWinCloseEvent> {};
    struct CurrWorkspaceSetMode : core::event::Event<CurrWorkspaceSetMode> {
        int mode;
    };

    struct Config {
        std::array<input_service::KEY_CODES::Enum, 2>  MOD; // i.e: (ctrl+shift)
        input_service::KEY_CODES::Enum  MOD_PLUS;   // i.e: MOD + shit + NUM_2 => move curr window to workspace 2
        input_service::KEY_CODES::Enum  LEFT;       // i.e: MOD + arrowleft => move window left
        input_service::KEY_CODES::Enum  RIGHT;      // i.e: MOD + arrowright => move window right
        input_service::KEY_CODES::Enum  TOP;        // i.e: MOD + MOD_PLUS + arrowup => resize window top
        input_service::KEY_CODES::Enum  BOTTOM;     // i.e: MOD + MOD_PLUS + arrowdown => resize window top
    };
    
    class KeybindsService : public core::service::Service<KeybindsService> {
    public:
        KeybindsService();
        ~KeybindsService();

        void OnEvent(core::event::OnEventCallback callback);
    private:
        void _KeysPressed(input_service::KeysTrackedArr keys);
    private:
        Config m_config;
    };
}
