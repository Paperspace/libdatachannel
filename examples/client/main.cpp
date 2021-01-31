/*
 * libdatachannel client example
 * Copyright (c) 2019-2020 Paul-Louis Ageneau
 * Copyright (c) 2019 Murat Dogan
 * Copyright (c) 2020 Will Munn
 * Copyright (c) 2020 Nico Chatzi
 * Copyright (c) 2020 Lara Mackey
 * Copyright (c) 2020 Erik Cota-Robles
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 */

#include "rtc/rtc.hpp"

#include "parse_cl.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <future>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include <thread>
#include <unordered_map>

using namespace rtc;
using namespace std;
using namespace std::chrono_literals;

using json = nlohmann::json;

template <class T> weak_ptr<T> make_weak_ptr(shared_ptr<T> ptr) { return ptr; }

unordered_map<string, shared_ptr<PeerConnection>> peerConnectionMap;
unordered_map<string, shared_ptr<DataChannel>> dataChannelMap;

string localId;
string peerId;
string messageText;
int messageSize;
int messageCount;
int connectionCount;
bool sendBinary;
bool replyPrompt;
bool echo;
bool exitOnReply;
vector<std::byte> binaryMessage;

shared_ptr<PeerConnection> createPeerConnection(const Configuration &config,
                                                weak_ptr<WebSocket> wws, string id);
string randomId(size_t length);

void timer_start(std::function<void(void)> func, unsigned int interval) {
	std::thread([func, interval]() {
		while (true) {
			auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(interval);
			func();
			std::this_thread::sleep_until(x);
		}
	}).detach();
}

int main(int argc, char **argv) try {
	auto params = std::make_unique<Cmdline>(argc, argv);

	rtc::InitLogger(LogLevel::Info);

	Configuration config;
	string stunServer = "";
	if (params->noStun()) {
		cout << "No STUN server is configured. Only local hosts and public IP addresses supported."
		     << endl;
	} else {
		if (params->stunServers().substr(0, 5).compare("stun:") != 0) {
			stunServer = "stun:";
		}
		stunServer += params->stunServers() + ":" + to_string(params->stunPort());
		cout << "Stun server is " << stunServer << endl;
		config.iceServers.emplace_back(stunServer);
	}

	if (!params->id().empty())
		localId = params->id();
	else
		localId = randomId(4);
	cout << "The local ID is: " << localId << endl;

	peerId = params->peerId();
	messageText = params->message();
	messageSize = params->messageSize();
	sendBinary = params->sendBinary();
	if (messageText.empty()) {
		if (sendBinary) {
			cout << "sendBinary" << endl;
			if (messageSize > 0) {
				cout << "messageSize" << endl;
				binaryMessage.resize(messageSize);
				srand(unsigned int(time(NULL)));
				for (size_t i = 0; i < messageSize; i++)
					binaryMessage[i] = byte(rand() % 256);
			}
		} else {
			if (messageSize > 0) {
				cout << "text messageSize" << endl;
				messageText = randomId(messageSize);
			} else
				messageText = "Hello";
		}
	}
	messageCount = params->messageCount();
	connectionCount = params->connectionCount();
	replyPrompt = params->replyPrompt();
	echo = params->echo();
	exitOnReply = params->exitOnReply();

	auto ws = make_shared<WebSocket>();

	std::promise<void> wsPromise;
	auto wsFuture = wsPromise.get_future();

	ws->onOpen([&wsPromise, wss = make_weak_ptr(ws)]() {
		cout << "WebSocket connected, signaling ready" << endl;
		wsPromise.set_value();
		timer_start(
		    [wss]() {
			    if (auto ws = wss.lock()) {
				    ws->send("ping");
			    }
		    },
		    10000);
	});

	ws->onError([&wsPromise](string s) {
		cout << "WebSocket error" << endl;
		wsPromise.set_exception(std::make_exception_ptr(std::runtime_error(s)));
	});

	ws->onClosed([]() { cout << "WebSocket closed" << endl; });

	ws->onMessage([&](variant<binary, string> data) {
		if (!holds_alternative<string>(data))
			return;

		json message = json::parse(get<string>(data));

		auto it = message.find("id");
		if (it == message.end())
			return;
		string id = it->get<string>();

		it = message.find("type");
		if (it == message.end())
			return;
		string type = it->get<string>();

		shared_ptr<PeerConnection> pc;
		if (auto jt = peerConnectionMap.find(id); jt != peerConnectionMap.end()) {
			pc = jt->second;
		} else if (type == "offer") {
			cout << "Answering to " + id << endl;
			pc = createPeerConnection(config, ws, id);
		} else {
			return;
		}

		if (type == "offer" || type == "answer") {
			auto sdp = message["description"].get<string>();
			pc->setRemoteDescription(Description(sdp, type));
		} else if (type == "candidate") {
			auto sdp = message["candidate"].get<string>();
			auto mid = message["mid"].get<string>();
			pc->addRemoteCandidate(Candidate(sdp, mid));
		}
	});

	string wsPrefix = "";
	if (params->webSocketServer().substr(0, 5).compare("ws://") != 0) {
		wsPrefix = "ws://";
	}
	const string url = wsPrefix + params->webSocketServer() + ":" +
	                   to_string(params->webSocketPort()) + "/" + localId;
	cout << "Url is " << url << endl;
	ws->open(url);

	cout << "Waiting for signaling to be connected..." << endl;
	wsFuture.get();
	
	string id;
	if (!peerId.empty()) {
		cout << "Remote peer ID: " << peerId << endl;
		id = peerId;
	} else {
		while (true) {
			cout << "Enter a remote ID to send an offer:" << endl;
			cin >> id;
			cin.ignore();
			if (id.empty())
				return 0;
			if (id != localId)
				break;
		}
	}

	cout << "Offering to " + id << endl;
	auto pc = createPeerConnection(config, ws, id);

	// We are the offerer, so create a data channel to initiate the process
	const string label = "test";
	cout << "Creating DataChannel with label \"" << label << "\"" << endl;
	auto dc = pc->createDataChannel(label);

	dc->onOpen([id, wdc = make_weak_ptr(dc)]() {
		cout << "DataChannel from " << id << " open" << endl;
		if (messageCount) {
			if (auto dc = wdc.lock()) {
				if (sendBinary)
					dc->send(binaryMessage.data(), binaryMessage.size());
				else
					dc->send(messageText + " from " + localId);
			}
			if (messageCount > 0)
				--messageCount;
		}
	});

	dc->onClosed([id]() {
		cout << "DataChannel from " << id << " closed" << endl;
	});

	dc->onMessage([id, wdc = make_weak_ptr(dc)](variant<binary, string> data) {
		if (holds_alternative<string>(data))
			cout << "Message from " << id << " received: " << get<string>(data) << endl;
		else
			cout << "Binary message from " << id
				    << " received, size=" << get<binary>(data).size() << endl;
		if (echo) {
			if (auto dc = wdc.lock()) {
				if (holds_alternative<string>(data))
					dc->send(get<string>(data));
				else
					dc->send(get<binary>(data).data(), get<binary>(data).size());
			}
		} else if (messageCount) {
			if (auto dc = wdc.lock()) {
				if (sendBinary)
					dc->send(binaryMessage.data(), binaryMessage.size());
				else
					dc->send(messageText + " from " + localId);
			}
			if (messageCount > 0)
				--messageCount;
		}
	});

	dataChannelMap.emplace(id, dc);

	cout << "Press any key to exit... " << endl;
	//main thread is pausing here awaitng various callbacks
	cin.ignore();

	cout << "Cleaning up..." << endl;

	dataChannelMap.clear();
	peerConnectionMap.clear();
	return 0;

} catch (const std::exception &e) {
	std::cout << "Error: " << e.what() << std::endl;
	dataChannelMap.clear();
	peerConnectionMap.clear();
	return -1;
}

// Create and setup a PeerConnection
shared_ptr<PeerConnection> createPeerConnection(const Configuration &config,
                                                weak_ptr<WebSocket> wws, string id) {
	auto pc = make_shared<PeerConnection>(config);

	pc->onStateChange([](PeerConnection::State state) { cout << "State: " << state << endl; });

	pc->onGatheringStateChange(
	    [](PeerConnection::GatheringState state) { cout << "Gathering State: " << state << endl; });

	pc->onLocalDescription([wws, id](Description description) {
		json message = {
		    {"id", id}, {"type", description.typeString()}, {"description", string(description)}};

		if (auto ws = wws.lock())
			ws->send(message.dump());
	});

	pc->onLocalCandidate([wws, id](Candidate candidate) {
		json message = {{"id", id},
		                {"type", "candidate"},
		                {"candidate", string(candidate)},
		                {"mid", candidate.mid()}};

		if (auto ws = wws.lock())
			ws->send(message.dump());
	});

	pc->onDataChannel([id](shared_ptr<DataChannel> dc) {
		cout << "DataChannel from " << id << " received with label \"" << dc->label() << "\""
		     << endl;

		dc->onClosed([id]() { cout << "DataChannel from " << id << " closed" << endl; });

		dc->onMessage([id, wdc = make_weak_ptr(dc)](variant<binary, string> data) {
			if (holds_alternative<string>(data))
				cout << "Message from " << id << " received: " << get<string>(data) << endl;
			else
				cout << "Binary message from " << id
				     << " received, size=" << get<binary>(data).size() << endl;
			if (echo) {
				if (auto dc = wdc.lock()) {
					if (holds_alternative<string>(data))
						dc->send(get<string>(data));
					else
						dc->send(get<binary>(data).data(), get<binary>(data).size());
				}
			} else if (messageCount) {
				if (auto dc = wdc.lock()) {
					if (sendBinary)
						dc->send(binaryMessage.data(), binaryMessage.size());
					else
						dc->send(messageText + " from " + localId);
				}
				if (messageCount > 0)
					--messageCount;
			}
		});

		dataChannelMap.emplace(id, dc);
	});

	peerConnectionMap.emplace(id, pc);
	return pc;
};

// Helper function to generate a random ID
string randomId(size_t length) {
	static const string characters(
	    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	string id(length, '0');
	default_random_engine rng(random_device{}());
	uniform_int_distribution<int> dist(0, int(characters.size() - 1));
	generate(id.begin(), id.end(), [&]() { return characters.at(dist(rng)); });
	return id;
}
