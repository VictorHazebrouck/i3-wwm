#include "./keybinds_service.hpp"
#include <iostream>

static const size_t NB_DEFAULT_KEYS_TRACKED = 4;

namespace app::keybinds_service {
    core::event::OnEventCallback s_on_event_cb;

    KeybindsService::KeybindsService() {
        m_config.MOD[0] = input_service::KEY_CODES::CAPS_LOCK;
        m_config.MOD[1] = input_service::KEY_CODES::NOOP;
        m_config.MOD_PLUS = input_service::KEY_CODES::L_SHIFT;

        core::application::Application::Get()
            .Service<input_service::InputService>()
            .OnEvent([this] (core::event::IEvent& e) {
                if(auto ev = e.As<input_service::KeysPressedEvent>()) {
                    _KeysPressed(ev->keys);
                }
            });
    }

    KeybindsService::~KeybindsService() {}

    void KeybindsService::OnEvent(core::event::OnEventCallback callback) {
        s_on_event_cb = callback; 
    };

    void KeybindsService::_KeysPressed(input_service::KeysTrackedArr keys) {
        bool is_mod_key_pressed = false;
        bool is_mod_plus_key_pressed = false;
        input_service::KEY_CODES::Enum key_code = input_service::KEY_CODES::NOOP;

        for(auto key : keys) {
            if(key == m_config.MOD[0]) {
                is_mod_key_pressed = true;
            } 
            else if(key == m_config.MOD_PLUS) {
                is_mod_plus_key_pressed = true;
            }
            else if(key != input_service::KEY_CODES::NOOP){
                key_code = key;
            }
        }
        
        if(!is_mod_key_pressed || key_code == input_service::KEY_CODES::NOOP) {
            return;
        }
        
        // ChangeWorkspaceEvent e;
        // e.id = key_code;
        // s_on_event_cb(e);

        if(!is_mod_plus_key_pressed) {
            switch(key_code) {
            case input_service::KEY_CODES::NUM_1: {
                /// sharedpointer
                auto e = ChangeWorkspaceEvent{.id = 1};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::NUM_2: {
                auto e = ChangeWorkspaceEvent{.id = 2};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::NUM_3: {
                auto e = ChangeWorkspaceEvent{.id = 3};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::NUM_4: {
                auto e = ChangeWorkspaceEvent{.id = 4};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::NUM_5: {
                auto e = ChangeWorkspaceEvent{.id = 5};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::NUM_6: {
                auto e = ChangeWorkspaceEvent{.id = 6};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::NUM_7: {
                auto e = ChangeWorkspaceEvent{.id = 7};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::NUM_8: {
                auto e = ChangeWorkspaceEvent{.id = 8};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::NUM_9: {
                auto e = ChangeWorkspaceEvent{.id = 9};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::NUM_0: {
                auto e = ChangeWorkspaceEvent{.id = 10};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::ARROW_L: {
                auto e = FocusOtherWinEvent{.dir = DIRECTION::LEFT};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::ARROW_R: {
                auto e = FocusOtherWinEvent{.dir = DIRECTION::RIGHT};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::ARROW_UP: {
                auto e = FocusOtherWinEvent{.dir = DIRECTION::TOP};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::ARROW_DOWN: {
                auto e = FocusOtherWinEvent{.dir = DIRECTION::BOTTOM};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::KEY_W: {
                auto e = CurrWorkspaceSetMode{.mode = 0};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::KEY_E: {
                auto e = CurrWorkspaceSetMode{.mode = 1};
                s_on_event_cb(e);
                break;
            }
            default:
                break;
            }
        } else {
            switch(key_code) {
            case input_service::KEY_CODES::NUM_1: {
                auto e = CurrWinChangeWorkspaceEvent{.id = 1};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::NUM_2: {
                auto e = CurrWinChangeWorkspaceEvent{.id = 2};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::NUM_3: {
                auto e = CurrWinChangeWorkspaceEvent{.id = 3};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::NUM_4: {
                auto e = CurrWinChangeWorkspaceEvent{.id = 4};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::NUM_5: {
                auto e = CurrWinChangeWorkspaceEvent{.id = 5};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::NUM_6: {
                auto e = CurrWinChangeWorkspaceEvent{.id = 6};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::NUM_7: {
                auto e = CurrWinChangeWorkspaceEvent{.id = 7};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::NUM_8: {
                auto e = CurrWinChangeWorkspaceEvent{.id = 8};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::NUM_9: {
                auto e = CurrWinChangeWorkspaceEvent{.id = 9};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::NUM_0: {
                auto e = CurrWinChangeWorkspaceEvent{.id = 10};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::ARROW_L: {
                auto e = CurrWinMoveEvent{.dir = DIRECTION::LEFT};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::ARROW_R: {
                auto e = CurrWinMoveEvent{.dir = DIRECTION::RIGHT};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::ARROW_UP: {
                auto e = CurrWinMoveEvent{.dir = DIRECTION::TOP};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::ARROW_DOWN: {
                auto e = CurrWinMoveEvent{.dir = DIRECTION::BOTTOM};
                s_on_event_cb(e);
                break;
            }
            case input_service::KEY_CODES::KEY_Q: {
                auto e = CurrWinCloseEvent{};
                s_on_event_cb(e);
                break;
            }
            default:
                break;
            }
        }
    }
}