//
// Created by Matq on 19/01/2025.
//

#pragma once

#define APPLICATION_VERSION "1.3.0"
#define APPLICATION_NAME "DrawSomething"

#include <cstdint>
#include <functional>

using Callback = std::function<void()>;

enum class MessageID : uint32_t {
    SERVER_Accept,
    SERVER_Deny,
    SERVER_Message,

    CLIENT_MessageAll,

    SHARED_Ping,
};
