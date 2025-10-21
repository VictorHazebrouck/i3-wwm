#include "./input_service.hpp"
#include <iostream>
#include <ranges>

namespace app::input_service {
    core::event::OnEventCallback s_on_event_cb;

    InputService::InputService(Config config) {
        m_config = config;

        for(size_t i = 0; i < m_keys_pressed_tracked.size(); i += 1) {
            m_keys_pressed_tracked[i] = KEY_CODES::NOOP;
        };

        core::winapi_utils::events::on_key_down([this] (int key_code) {
            return _KeyDown((KEY_CODES::Enum)key_code);
        });
        core::winapi_utils::events::on_key_up([this] (int key_code) {
            return _KeyUp((KEY_CODES::Enum)key_code);
        });
    }

    InputService::~InputService() {}

    void InputService::OnEvent(core::event::OnEventCallback callback) {
        s_on_event_cb = callback;
    }

    void InputService::PollEvents() {
        core::winapi_utils::events::pool_once();
    }

    core::winapi_utils::events::PROPAGATE_KEY_EV::Enum InputService::_KeyDown(KEY_CODES::Enum key_code) {
        bool is_old_key = _IsKeyCodeAlreadyTracked(key_code);
        if(is_old_key) {
            if(key_code == m_config.blacklist) {
                return core::winapi_utils::events::PROPAGATE_KEY_EV::STOP;
            }
            return core::winapi_utils::events::PROPAGATE_KEY_EV::CONTINUE; 
        }
        _TrackKeyCode(key_code);

        auto e = KeysPressedEvent{.keys = m_keys_pressed_tracked };
        // auto e2  = core::event::make<KeysPressedEvent>(m_keys_pressed_tracked);
        // std::cout << e2->keys[0] << std::endl;
        
        s_on_event_cb(e);

        if(_IsKeyCodeAlreadyTracked(m_config.blacklist)) {
            return core::winapi_utils::events::PROPAGATE_KEY_EV::STOP;
        }
        return core::winapi_utils::events::PROPAGATE_KEY_EV::CONTINUE;
    }
    
    core::winapi_utils::events::PROPAGATE_KEY_EV::Enum InputService::_KeyUp(KEY_CODES::Enum key_code) {
        // @todo canceling on keydown seems to "automatically" disconnect on key up system wide
        // does the winapi keep a similar "registery" or keys already pressed ?
        // i.e: stop L_WIN propagation on key down stops the winmenus from opening ?  
        _UnTrackKeyCode(key_code);
        return core::winapi_utils::events::PROPAGATE_KEY_EV::CONTINUE;
    }

    void InputService::_TrackKeyCode(KEY_CODES::Enum key_code) {
        for(size_t i = 0; i < m_keys_pressed_tracked.size(); i += 1) {
            if(m_keys_pressed_tracked[i] == KEY_CODES::NOOP) {
                m_keys_pressed_tracked[i] = key_code;
                return;
            }
        };
    }

    void InputService::_UnTrackKeyCode(KEY_CODES::Enum key_code) {
        for(size_t i = 0; i < m_keys_pressed_tracked.size(); i += 1) {
            if(m_keys_pressed_tracked[i] == key_code) {
                m_keys_pressed_tracked[i] = KEY_CODES::NOOP;
            }
        };
    }

    bool InputService::_IsKeyCodeAlreadyTracked(KEY_CODES::Enum key_code) {
        for(size_t i = 0; i < m_keys_pressed_tracked.size(); i += 1) {
            if(m_keys_pressed_tracked[i] == key_code) {
                return true;
            }
        };
        return false;
    };
}