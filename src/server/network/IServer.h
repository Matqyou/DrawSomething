//
// Created by Matq on 19/01/2025.
//

#pragma once

#include <iostream>
#include "../../shared/network/ThreadQueue.h"
#include "../../shared/network/Connection.h"
#include "../../shared/network/Message.h"

template<typename T>
class IServer {
protected:
    ThreadQueue<OwnedMessage<T>> messages_incoming;
    std::deque<std::shared_ptr<Connection<T>>> connections;
    asio::io_context asio_context;
    std::thread thread_context;
    asio::ip::tcp::acceptor asio_acceptor;
    uint32_t current_id = 0;

public:
    explicit IServer(uint16_t port)
        : asio_acceptor(
        asio_context,
        asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)
    ) { }
    virtual ~IServer() { Stop(); }

    bool Start() {
        try {
            AsyncWaitForConnection();
            thread_context = std::thread([this]() { asio_context.run(); });
        }
        catch (std::exception& e) {
            std::cerr << "[SERVER] Exception: " << e.what() << "\n";
            return false;
        }

        std::cout << "[SERVER] Started!\n";
        return true;
    }
    void Stop() {
        asio_context.stop();
        if (thread_context.joinable())
            thread_context.join();

        std::cout << "[SERVER] Stopped!\n";
    }

    void AsyncWaitForConnection() {
        asio_acceptor.async_accept(
            [this](std::error_code ec, asio::ip::tcp::socket socket) {
                if (!ec) {
                    std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";

                    std::shared_ptr<Connection<T>> newconn =
                        std::make_shared<Connection<T>>(Connection<T>::owner::server, asio_context, std::move(socket), messages_incoming);

                    if (OnClientConnect(newconn)) {
                        connections.push_back(std::move(newconn));
                        connections.back()->ConnectToClient(current_id++);

                        std::cout << "[" << connections.back()->GetID() << "] Connection Approved\n";
                    } else {
                        std::cout << "[-----] Connection Denied\n";
                    }
                } else {
                    std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";
                }

                AsyncWaitForConnection();
            });
    }

    void MessageClient(std::shared_ptr<Connection<T>> client, const Message<T>& msg) {
        if (client && client->IsConnected()) {
            client->Send(msg);
        } else {
            OnClientDisconnect(client);
            client.reset();
            connections.erase(std::remove(connections.begin(), connections.end(), client), connections.end());
        }
    }

    void MessageAllClients(const Message<T>& msg, std::shared_ptr<Connection<T>> pIgnoreClient = nullptr) {
        bool bInvalidClientExists = false;

        for (auto& client : connections) {
            if (client && client->IsConnected()) {
                if (client != pIgnoreClient)
                    client->AsyncSend(msg);
            } else {
                OnClientDisconnect(client);
                client.reset();

                bInvalidClientExists = true;
            }
        }

        if (bInvalidClientExists)
            connections.erase(std::remove(connections.begin(), connections.end(), nullptr), connections.end());
    }

    void Update(size_t nMaxMessages = -1, bool bWait = false) {
        if (bWait) messages_incoming.wait();

        size_t nMessageCount = 0;
        while (nMessageCount < nMaxMessages && !messages_incoming.empty()) {
            auto message = messages_incoming.pop_front();

            OnMessage(message.connection, message.message);
            nMessageCount++;
        }
    }

protected:
    // Called when a client connects, you can veto the connection by returning false
    virtual bool OnClientConnect(std::shared_ptr<Connection<T>> client) {
        return false;
    }

    // Called when a client appears to have disconnected
    virtual void OnClientDisconnect(std::shared_ptr<Connection<T>> client) {

    }

    // Called when a message arrives
    virtual void OnMessage(std::shared_ptr<Connection<T>> client, Message<T>& msg) {

    }

};
