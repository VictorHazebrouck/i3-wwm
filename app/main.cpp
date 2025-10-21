#include <iostream>
#include <vector>
#include <unordered_map>

#include "core/application/application.hpp"
#include "core/event/event.hpp"
#include "app/input_service/input_service.hpp"
#include "app/keybinds_service/keybinds_service.hpp"
#include "app/window_service/window_service.hpp"
#include "app/workspace/workspace.hpp"

int main() {
    core::application::Application app;

    app::input_service::Config input_config;
    input_config.blacklist = app::input_service::KEY_CODES::CAPS_LOCK;
    app.RegisterService<app::input_service::InputService>(input_config);

    app.RegisterService<app::keybinds_service::KeybindsService>();

    app.RegisterService<app::window_service::WindowService>();
    
    app.RegisterService<app::workspace::WorkspacesService>();
    
    app.Start();
}