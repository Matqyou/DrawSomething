//
// Created by Matq on 23/04/2025.
//

#pragma once

#include <nlohmann/json.hpp>
#include "Protocol.h"
#include <fcntl.h>
#include <thread>
#include <queue>
#include <utility>

#define PACKET_TIMEOUT_MILLISECONDS 3000

using json = nlohmann::json;

struct NetworkResponse
{
	std::string raw_response_data;
	int response_code;
	bool success_evaluation;
	json response_json;

	// Sense
	[[nodiscard]] std::string GetCode() const { return response_json.value("code", "No code provided"); }
	[[nodiscard]] std::string GetMsg() const { return response_json.value("message", "No message provided"); }
};

using ResponseCallback = std::function<void(const NetworkResponse&)>;
class Packet
{
private:
	int id;
	bool sent;
	std::atomic<bool> resolved;
	bool errored;
	std::string error_reason;

	const std::string route;
	const std::string method;
	json send_json;

	NetworkResponse response;
	ResponseCallback response_callback;
	Callback errored_callback;
	std::chrono::steady_clock::time_point sent_at;

public:
	Packet(std::string route, std::string method, json *json_data = nullptr);

	// Getting
	[[nodiscard]] int GetID() const { return id; }
	[[nodiscard]] const std::string& Route() const { return route; }
	[[nodiscard]] const std::string& Method() const { return method; }
	[[nodiscard]] json& Payload() { return send_json; }
	[[nodiscard]] bool Resolved() const { return resolved; }
	[[nodiscard]] bool Errored() const { return errored; }
	[[nodiscard]] const std::string& ErrorReason() const { return error_reason; }
	[[nodiscard]] NetworkResponse& Response() { return response; }

	// Generating
//	[[nodiscard]] bool HasTimedout();

	// Options
	Packet *SetResponseCallback(ResponseCallback callback)
	{
		response_callback = std::move(callback);
		return this;
	}
	Packet *SetErroredCallback(Callback callback)
	{
		errored_callback = std::move(callback);
		return this;
	}

	// Manipulating
	void SetResolved(NetworkResponse server_response);
	void SetErrored(const std::string& reason);
	void CallErrored();
	void Send(bool blocking = false, const std::string& filepath = "");

};

using FileResponseCallback = std::function<void(const std::vector<unsigned char>&)>;
class FilePacket
{
private:
	int id;
	bool sent;
	std::atomic<bool> finished;
	std::atomic<bool> resolved;
	bool errored;
	std::string error_reason;

	std::string route;
	std::string method;

	std::vector<unsigned char> data;

	FileResponseCallback response_callback;
	Callback errored_callback;

	static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

public:
	explicit FilePacket(std::string route, std::string method = "GET");

	// Getting
	[[nodiscard]] int GetID() const { return id; }
	[[nodiscard]] const std::string& Route() const { return route; }
	[[nodiscard]] const std::string& Method() const { return method; }
	[[nodiscard]] bool Finished() const { return finished; }
	[[nodiscard]] bool Resolved() const { return resolved; }
	[[nodiscard]] bool Errored() const { return errored; }
	[[nodiscard]] const std::string& ErrorReason() const { return error_reason; }
	[[nodiscard]] const std::vector<unsigned char>& GetData() const { return data; }

	// Options
	FilePacket *SetResponseCallback(FileResponseCallback callback)
	{
		response_callback = std::move(callback);
		return this;
	}
	FilePacket *SetErroredCallback(Callback callback)
	{
		errored_callback = std::move(callback);
		return this;
	}

	// Manipulating
	void SetFinished() { finished = true; };
	void SetResolved();
	void SetErrored(const std::string& reason);
	void CallErrored();
	void Send(bool blocking = false);

};

class NetworkClient
{
private:
	static std::mutex response_mutex;
	static std::deque<NetworkResponse> response_queue;

	static std::string session_key;
	static std::vector<FilePacket *> sent_filepackets;
	static std::vector<Packet *> sent_packets;

	void StartListening();

public:
	static int CurrentPacketID;

	NetworkClient();
	~NetworkClient();

	// Generating
	static Packet *GetPacket(int packet_id);
	static FilePacket *GetFilePacket(int file_packet_id);

	// Manipulating
//	static void SendPacket(Packet *packet, bool blocking = false);
	static void SendPacket(Packet *packet, bool blocking = false, const std::string& filepath = "");
	static void SendFilePacket(FilePacket *file_packet, bool blocking = false);
//	static void SendRequest(const std::string& route, const std::string& method, json *json_data = nullptr);
	static void SetSessionKey(const std::string& key);

	// Ticking
//	static bool PollResponse(NetworkResponse& server_response);
	static void TickResolve();
	static void TickErrored();

};
