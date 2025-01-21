//
// Created by Matq on 19/01/2025.
//

#pragma once

#define APPLICATION_VERSION "1.2.1"
#define APPLICATION_NAME "DrawSomething"

#include <cstdint>

enum class SimpleAlign {
    DONT,
    LEFT,
    CENTER,
    RIGHT,
    TOP = LEFT,
    BOTTOM = RIGHT,
};

enum class Align {
    DONT,
    BEHIND_LEFT,
    LEFT,
    CENTER,
    RIGHT,
    BEHIND_RIGHT,
    ABOVE_TOP = BEHIND_LEFT,
    TOP = LEFT,
    BOTTOM = RIGHT,
    UNDER_BOTTOM = BEHIND_RIGHT,
};

enum class MessageID : uint32_t {
    SERVER_Accept,
    SERVER_Deny,
    SERVER_Message,

    CLIENT_MessageAll,

    SHARED_Ping,
};
