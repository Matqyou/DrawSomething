#include "../shared/Protocol.h"
#define SERVER_NAME (APPLICATION_NAME " Server")
#define ASIO_STANDALONE

#include "../shared/core/Strings.h"
#include "network/IServer.h"
#include <iostream>

class CustomServer : public IServer<MessageID> {
public:
    explicit CustomServer(uint16_t nPort) : IServer<MessageID>(nPort) {

    }

protected:
    bool OnClientConnect(std::shared_ptr<Connection<MessageID>> client) override {
        Message<MessageID> msg;
        msg.header.message_id = MessageID::SERVER_Accept;
        client->AsyncSend(msg);
        return true;
    }

    void OnClientDisconnect(std::shared_ptr<Connection<MessageID>> client) override {
        std::cout << "Removing client [" << client->GetID() << "]\n";
    }

    void OnMessage(std::shared_ptr<Connection<MessageID>> client, Message<MessageID>& message) override {
        switch (message.header.message_id) {
            case MessageID::SHARED_Ping: {
                std::cout << "[" << client->GetID() << "]: Server Ping\n";

                client->AsyncSend(message);
                break;
            }
            case MessageID::CLIENT_MessageAll: {
                std::cout << "[" << client->GetID() << "]: Message All\n";

                Message<MessageID> message;
                message.header.message_id = MessageID::SERVER_Message;
                message << client->GetID();
                MessageAllClients(message, client);
                break;
            }
            default: {
                std::cout << Strings::FStringColors("[Server] &8Unknown message_id: %u\n", message.header.message_id);
                break;
            }
        }
    }
};

int main() {
    system("");
    Strings::PrintDivider(SERVER_NAME, false);
    std::cout << Strings::FStringColors("&3%s %s\n", SERVER_NAME, APPLICATION_VERSION);
    std::cout << Strings::FStringColors("[Server] &aHello there!\n");
    Strings::PrintDivider();

    CustomServer server(60000);
    server.Start();

    while (1) {
        server.Update(-1, true);
    }

    return 0;
}
