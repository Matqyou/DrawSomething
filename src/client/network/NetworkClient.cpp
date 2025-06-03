//
// Created by Matq on 23/04/2025.
//

#include "NetworkClient.h"
#include "../../shared/core/Strings.h"
#include <curl/curl.h>

#include <utility>

size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output)
{
	size_t total_size = size * nmemb;
	output->append((char *)contents, total_size);
	return total_size;
}

json ParseJson(const std::string& response)
{
	try
	{
		return json::parse(response);
	} catch (const std::exception& e)
	{
		std::cerr << "[JSON Error] " << e.what() << std::endl;
		return json::object();
	}
}

Packet::Packet(std::string route, std::string method, json *json_data)
	: route(std::move(route)), method(std::move(method))
{
	id = NetworkClient::CurrentPacketID++;
	sent = false;
	resolved = false;
	errored = false;
	error_reason = "no error";

	send_json = json_data ? *json_data : json{ };
}

void Packet::SetResolved(NetworkResponse server_response)
{
	if (resolved)
		return;

	resolved = true;
	sent = false;

	const char *ResponseCodeColor = server_response.success_evaluation ? "&a" : "&c";
	dbg_msg("&d[SERVER Response] &rResolved packet #%d : %s%d %s %s \n&8%s\n", id, ResponseCodeColor, server_response.response_code, route.c_str(), method.c_str(), server_response.response_json.dump(2).c_str());

	std::string server_error = server_response.response_json.value("error", "");
	if (!server_error.empty())
		dbg_msg("&d[SERVER Response]&c Error: %s\n", server_error.c_str());

	response = std::move(server_response);
	if (response_callback)
		response_callback(response);
}

void Packet::SetErrored(const std::string& reason)
{
	if (errored)
		return;

	errored = true;
	resolved = true;
	error_reason = reason;

	dbg_msg("&9[CLIENT] &cError packet #%d %s %s: %s\n", id, route.c_str(), method.c_str(), reason.c_str());
}

void Packet::CallErrored()
{
	if (errored_callback)
		errored_callback();
}

void Packet::Send(bool blocking)
{
	if (sent)
		return;

	sent = true;
	sent_at = std::chrono::steady_clock::now();

	NetworkClient::SendPacket(this, blocking);
}

FilePacket::FilePacket(std::string route, std::string method)
	: id(NetworkClient::CurrentPacketID++), sent(false), resolved(false), errored(false),
	  route(std::move(route)), method(std::move(method))
{

}

size_t FilePacket::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t totalSize = size * nmemb;
	auto *buffer = static_cast<std::vector<unsigned char> *>(userp);
	unsigned char *dataPtr = static_cast<unsigned char *>(contents);
	buffer->insert(buffer->end(), dataPtr, dataPtr + totalSize);
	return totalSize;
}

void FilePacket::SetResolved()
{
	resolved = true;
	sent = false;

	dbg_msg("&d[SERVER Response] &rResolved file packet #%d : %s %s\n", id, route.c_str(), method.c_str());
	if (response_callback)
		response_callback(data);
}

void FilePacket::SetErrored(const std::string& reason)
{
	errored = true;
	resolved = true;
	error_reason = reason;

	dbg_msg("&9[CLIENT] &cError file packet #%d %s %s: %s\n", id, route.c_str(), method.c_str(), reason.c_str());
}

void FilePacket::CallErrored()
{
	if (errored_callback)
		errored_callback();
}

void FilePacket::Send(bool blocking)
{
	if (sent)
		return;

	NetworkClient::SendFilePacket(this);

	sent = true;
	auto thread_func = [this]()
	{
		CURL *curl = curl_easy_init();
		if (!curl)
		{
			SetErrored("Failed to initialize curl");
			return;
		}

		std::string url = route; // full url
//		std::string url = "http://localhost:8000" + route;

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

		// Optional: you can set HTTP method explicitly, but GET is default for curl
		if (method == "POST")
			curl_easy_setopt(curl, CURLOPT_POST, 1L);
		else if (method == "GET")
			curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
		else
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());

		// Add headers if needed (Authorization, etc.)
		struct curl_slist *headers = nullptr;
		// headers = curl_slist_append(headers, "Authorization: Bearer TOKEN");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		CURLcode res = curl_easy_perform(curl);
		SetFinished();
		if (res == CURLE_OK)
		{
			long http_code = 0;
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
			if (http_code >= 200 && http_code < 300) { }
			else { SetErrored("HTTP error code: " + std::to_string(http_code)); }
		}
		else
		{
			SetErrored(curl_easy_strerror(res));
//			auto packet = NetworkClient::GetPacket(packet_id);
//			packet->SetErrored(curl_easy_strerror(res));
		}

		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
	};

	if (blocking)
	{
		thread_func();
	}
	else
	{
		std::thread(thread_func).detach();
	}
}

std::mutex NetworkClient::response_mutex;
std::deque<NetworkResponse> NetworkClient::response_queue;

std::string NetworkClient::session_key;
std::vector<Packet *> NetworkClient::sent_packets;
std::vector<FilePacket *> NetworkClient::sent_filepackets;

int NetworkClient::CurrentPacketID = 0;

void NetworkClient::StartListening()
{
	std::thread([]()
				{
					int sock = socket(AF_INET, SOCK_STREAM, 0);

					struct sockaddr_in serv_addr;
					serv_addr.sin_family = AF_INET;
					serv_addr.sin_port = htons(8000);  // You can pick another port for "push"
					inet_pton(AF_INET, "localhost", &serv_addr.sin_addr); // 127.0.0.1

					if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
					{
						dbg_msg("&c[CURL] Connection failed\n");
						return;
					}

					char buffer[4096];
					while (true)
					{
						memset(buffer, 0, sizeof(buffer));
						int valread = read(sock, buffer, sizeof(buffer));
						if (valread > 0)
						{
							std::string raw_response_data(buffer, valread);

							// parse JSON
							json response_json = ParseJson(raw_response_data);

							// Push to the same response queue
							std::lock_guard<std::mutex> lock(response_mutex);
							response_queue.push_back(NetworkResponse(
								raw_response_data,
								200, // Assuming 200 OK, you can customize
								true,
								response_json
							));
						}
						else if (valread == 0)
						{
							dbg_msg("&c[CURL] Server closed connection\n");
							break;
						}
						else
						{
							dbg_msg("&c[CURL] Read error\n");
							break;
						}
					}

					close(sock);
				}).detach();
}

NetworkClient::NetworkClient()
{
//	StartListening();
}

NetworkClient::~NetworkClient()
{

}

Packet *NetworkClient::GetPacket(int packet_id)
{
	for (Packet *packet : sent_packets)
		if (packet->GetID() == packet_id)
			return packet;

	return nullptr;
}

FilePacket *NetworkClient::GetFilePacket(int file_packet_id)
{
	for (FilePacket *packet : sent_filepackets)
		if (packet->GetID() == file_packet_id)
			return packet;

	return nullptr;
}

void NetworkClient::SendPacket(Packet *packet, bool blocking)
{
	sent_packets.push_back(packet);
	int packet_id = packet->GetID();
	std::string route = packet->Route();
	std::string method = packet->Method();
	std::string send_payload = packet->Payload().dump();

	dbg_msg("&9[CLIENT] &rPacket: #%d %s %s \n&8%s\n", packet_id, method.c_str(), route.c_str(), packet->Payload().dump(2).c_str());

	auto thread_func = [packet_id, route, method, send_payload]()
	{
		CURL *curl = curl_easy_init();
		if (!curl) return;

		std::string raw_response_data;
		struct curl_slist *headers = nullptr;

		std::string url = "http://localhost:8000" + route;
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

		if (method == "POST")
		{
			curl_easy_setopt(curl, CURLOPT_POST, 1L);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, send_payload.c_str());
			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, send_payload.size());
		}
		else if (method == "GET")
		{
			curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
		}

		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, Strings::FString("X-Packet-Id: %d", packet_id).c_str());
		headers = curl_slist_append(headers, ("Authorization: Bearer " + session_key).c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &raw_response_data);

		CURLcode res = curl_easy_perform(curl);
		if (res == CURLE_OK)
		{
			long http_code = 0;
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
			json response_json = ParseJson(raw_response_data);

			std::lock_guard<std::mutex> lock(response_mutex);
			response_queue.push_back(NetworkResponse(raw_response_data,
													 http_code,
													 http_code >= 200 && http_code < 300,
													 response_json));
		}
		else
		{
			auto packet = NetworkClient::GetPacket(packet_id);
			packet->SetErrored(curl_easy_strerror(res));
		}

		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
	};

	if (blocking)
	{
		std::thread t(thread_func);
		t.join();  // Wait for the request to complete
	}
	else
	{
		std::thread(thread_func).detach();  // Continue async
	}
}

void NetworkClient::SendFilePacket(FilePacket *file_packet, bool blocking)
{
	sent_filepackets.push_back(file_packet);
	dbg_msg("&9[CLIENT] &rFilePacket: #%d %s %s\n", file_packet->GetID(), file_packet->Method().c_str(), file_packet->Route().c_str());
}

void NetworkClient::SetSessionKey(const std::string& key)
{
	session_key = key;
}

void NetworkClient::TickResolve()
{
	std::lock_guard<std::mutex> lock(response_mutex);
	for (auto it = response_queue.begin(); it != response_queue.end();)
	{
		NetworkResponse& server_response = *it;
		int packet_id = server_response.response_json.value("packet_id", -1);
		if (packet_id == -1)
		{
			++it;
			continue;
		}

		Packet *packet = GetPacket(packet_id);
		if (!packet)
		{
			++it;
			continue;
		}

		packet->SetResolved(server_response);
		sent_packets.erase(std::find(sent_packets.begin(), sent_packets.end(), packet));
		it = response_queue.erase(it);
	}

	// 2 in 1
	for (auto it = sent_filepackets.begin(); it != sent_filepackets.end();)
	{
		FilePacket *file_packet = *it;
		if (!file_packet->Finished())
		{
			++it;
			continue;
		}

		it = sent_filepackets.erase(it);
		if (!file_packet->Errored())
			file_packet->SetResolved();
		else
			file_packet->CallErrored();
	}
}

void NetworkClient::TickErrored()
{
	for (auto it = sent_packets.begin(); it != sent_packets.end();)
	{
		Packet *packet = *it;
		if (!packet->Resolved() || !packet->Errored())
		{
			++it;
			continue;
		}

		it = sent_packets.erase(it);
		packet->CallErrored();
	}
}
