#include "./winapi_utils.hpp"
#include <windows.h>
#include <iostream>

namespace core::winapi_utils::screen {
    int get_height() {
        return GetSystemMetrics(SM_CYSCREEN);
    }

    int get_width() {
        return GetSystemMetrics(SM_CXSCREEN);
    };

    void disable_animations() {
        ANIMATIONINFO animation_info;
        animation_info.cbSize = sizeof(ANIMATIONINFO);
        animation_info.iMinAnimate = FALSE;
        SystemParametersInfo(SPI_SETANIMATION, sizeof(ANIMATIONINFO), &animation_info, SPIF_SENDCHANGE);
    };
}