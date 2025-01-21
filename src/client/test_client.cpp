#define ASIO_STANDALONE

#include <iostream>
#include <string>
#include "../shared/core/Strings.h"
#include "../shared/Protocol.h"
#include "../client/network/IClient.h"

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
                        std::cout << Strings::FStringColors("[Client] &ePing: %f\n", std::chrono::duration<double>(timeNow - timeThen).count());
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
        } else {
            std::cout << Strings::FStringColors("[Client] &cServer shutdown\n");
            break;
        }
    }

    return 0;
}
