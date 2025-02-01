#define ASIO_STANDALONE

#include <iostream>
#include <string>
#include "../client/network/IClient.h"
#include "../shared/core/Strings.h"
#include "../shared/Protocol.h"

class CustomClient : public IClient<MessageID> {
public:
    void PingServer() {
        Message<MessageID> message;
        message.header.message_id = MessageID::SHARED_Ping;

        std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

        message << timeNow;
        Send(message);
    }

    void MessageAll() {
        Message<MessageID> message;
        message.header.message_id = MessageID::CLIENT_MessageAll;
        Send(message);
    }

};

int main() {
    system("");
    Strings::PrintDivider("TestClient", false);
    Strings::PrintDivider();

    std::cout << Strings::FStringColors("[Client] Connecting to server..\n");

    CustomClient client;
    client.Connect("127.0.0.1", 60000);

    std::chrono::steady_clock::time_point last_ping;
    while (true) {
        if (client.IsConnected()) {
            if (!client.Incoming().empty()) {
                auto message = client.Incoming().pop_front().message;

                switch (message.header.message_id) {
                    case MessageID::SERVER_Accept: {
                        std::cout << Strings::FStringColors("[Client] &aServer reached\n");
                        break;
                    }
                    case MessageID::SHARED_Ping: {
                        std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
                        std::chrono::system_clock::time_point timeThen;
                        message >> timeThen;
                        auto ping = std::chrono::duration_cast<std::chrono::nanoseconds>(timeNow - timeThen).count();
                        auto ping_color = ping < 115000000 ? "&a" : ping < 240000000 ? "&e" : "&c";
                        std::cout << Strings::FStringColors("[Client] Ping: %s%s\n", ping_color, Strings::ConvertTimeNano(ping).c_str());
                        break;
                    }
                    case MessageID::SERVER_Message: {
                        uint32_t clientID;
                        message >> clientID;
                        std::cout << Strings::FStringColors("[Client] Server: &bHello from [%u]\n", clientID);
                        break;
                    }
                    default: {
                        std::cout << Strings::FStringColors("[Client] &8Unknown message_id: %u\n", message.header.message_id);
                        break;
                    }
                }
            }

            if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - last_ping).count() > 1500) {
                last_ping = std::chrono::steady_clock::now();
                Message<MessageID> message;
                message.header.message_id = MessageID::SHARED_Ping;

                std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
                message << timeNow;

                client.Send(message);
            }
        } else {
            std::cout << Strings::FStringColors("[Client] &cServer shutdown\n");
            break;
        }
    }



    return 0;
}
