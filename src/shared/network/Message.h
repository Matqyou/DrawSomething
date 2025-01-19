//
// Created by Matq on 19/01/2025.
//

#pragma once

template<typename T>
struct Header {
    T message_id{ };
    uint32_t message_size = 0;
};

template<typename T>
struct Message {
    Header<T> header { };
    std::vector<uint8_t> message_body;

    // Getting
    [[nodiscard]] size_t size() const { return message_body.size(); }

    // Print
    friend std::ostream& operator<<(std::ostream& os, const Message<T>& message) {
        os << "ID:" << int(message.header.id) << " Size:" << message.header.size;
        return os;
    }

    // Manipulation
    template<typename DataType>
    friend Message<T>& operator<<(Message<T>& message, const DataType& data) {
        static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

        size_t i = message.message_body.size();
        message.message_body.resize(message.message_body.size() + sizeof(DataType));

        std::memcpy(message.message_body.data() + i, &data, sizeof(DataType));
        message.header.message_size = message.size();

        return message;
    }
    template<typename DataType>
    friend Message<T>& operator>>(Message<T>& message, DataType& data) {
        static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

        size_t i = message.message_body.size() - sizeof(DataType);
        std::memcpy(&data, message.message_body.data() + i, sizeof(DataType));

        message.message_body.resize(i);
        message.header.message_size = message.size();

        return message;
    }
};

template<typename T>
class Connection;
template<typename T>
struct OwnedMessage {
    std::shared_ptr<Connection<T>> connection = nullptr;
    Message<T> message;

    friend std::ostream& operator<<(std::ostream& os, const OwnedMessage<T>& message) {
        os << message.message;
        return os;
    }
};
