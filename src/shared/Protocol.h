//
// Created by Matq on 19/01/2025.
//

#pragma once

#define APPLICATION_VERSION "1.1.4"
#define APPLICATION_NAME "DrawSomething"

#include <cstdint>

enum class MessageID : uint32_t {
    SERVER_Accept,
    SERVER_Deny,
    SERVER_Message,

    CLIENT_MessageAll,

    SHARED_Ping,
};
