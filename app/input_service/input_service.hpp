#pragma once

#include "core/winapi_utils/winapi_utils.hpp"
#include "core/event/event.hpp"
#include "core/service/service.hpp"
#include <functional>
#include <vector>
#include <array>

namespace app::input_service {
    namespace KEY_CODES {
        // @todo incomplete list of keys
        enum Enum : unsigned int {
            NOOP=0,

            ESCAPE=27,  
            SQUARE=192, NUM_1=49, NUM_2=50, NUM_3=51, NUM_4=52, NUM_5=53, NUM_6=54, NUM_7=55, NUM_8=56, NUM_9=57, NUM_0=48, PAREN_R=189, EQUAL=187, BACKSPACE=8,
            TAB=9, KEY_Q=81, KEY_W=87, KEY_E=69, KEY_R=82, KEY_T=84, KEY_Y=89, KEY_U=85, KEY_I=73, KEY_O=79, KEY_P=90, SQR_BRACKET_L=219, SQR_BRACKET_R=221,       
            CAPS_LOCK=20, KEY_A=65, KEY_S=83, KEY_D=68, KEY_F=70, KEY_G=71, KEY_H=72, KEY_J=74, KEY_K=75, KEY_L=78, SEMI_COL=186, QUOTES=222, BACKSLASH=220, ENTER=13,
            L_SHIFT=160, KEY_Z=90, KEY_X=88, KEY_C=67, KEY_V=86, KEY_B=66, KEY_N=78, KEY_M=77, COMMA=188, DOT=190, SLASH=191, R_SHIFT=161,
            L_CRTL=162, L_WIN=91, L_ALT=164, SPACE=32, R_ALT = 165,
            ARROW_L=37, ARROW_R=39, ARROW_UP=38, ARROW_DOWN = 40,
        };
    }

    struct Config {
        KEY_CODES::Enum blacklist = KEY_CODES::NOOP; 
    };

    // array cause very small size, and easily copyable for dispatch ?
    static const size_t NB_KEYS_TRACKED = 5;
    using KeysTrackedArr = std::array<KEY_CODES::Enum, NB_KEYS_TRACKED>;

    struct KeysPressedEvent : core::event::Event<KeysPressedEvent> {
        KeysTrackedArr keys;
    };
    
    class InputService : public core::service::Service<InputService> {
    public:
        InputService(Config config);
        ~InputService();

        void PollEvents() override;
        void OnEvent(core::event::OnEventCallback callback);
    private:
        core::winapi_utils::events::PROPAGATE_KEY_EV::Enum _KeyDown(KEY_CODES::Enum key_code);
        core::winapi_utils::events::PROPAGATE_KEY_EV::Enum _KeyUp(KEY_CODES::Enum key_code);
        bool _IsKeyCodeAlreadyTracked(KEY_CODES::Enum key_code);
        void _TrackKeyCode(KEY_CODES::Enum key_code);
        void _UnTrackKeyCode(KEY_CODES::Enum key_code);
    private:
        Config m_config;
        std::array<KEY_CODES::Enum, NB_KEYS_TRACKED> m_keys_pressed_tracked;
    };
}
