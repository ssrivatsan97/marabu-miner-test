#include "hex.h"
#include "hash.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

int main(int args, char** argv) {
	if (args < 3) {
		std::cout << "Error: Missing arguments!" << '\n';
		std::cout << "Args: [block] [target] [threads(opts, default = 4" << '\n';
		return -1;
	}

	std::string block = argv[1];
	auto target = hashing::decodeHex(argv[2]);
	int threads = 4;
	if (args > 3) {
		threads = atoi(argv[3]);
	}

	size_t noncePos = block.find("\"nonce\":");
	if (noncePos == std::string::npos) {
		std::cout << "Error: Block has no nonce field" << '\n';
		return -2;
	}

	std::string prefix = block.substr(0, noncePos + 10);
	std::string postfix = block.substr(noncePos + 64 + 10, block.size());

	std::string finalBlock;
	std::string blockHash;
	std::mutex foundMutex;
	bool found = false;

	auto workFunc = [&](Hex64 nonce) {
		while (!found) {
			auto blk = prefix + nonce.value() + postfix;
			auto hash = hashing::SHA256(blk);
			if (hashing::compareBytes(hash, target)) {
				std::lock_guard<std::mutex> lock(foundMutex);
				if (found) {
					return;
				}

				found = true;
				finalBlock = blk;
				blockHash = hashing::encodeHex(hash);
				return;
			}
			nonce++;
		}
	};

	std::vector<std::thread> workers;
	Hex64 initialNonce;
	for (int i = 0; i < threads; ++i) {
		Hex64 nonce(initialNonce);
		nonce.reverse();
		workers.emplace_back(workFunc, nonce);
		initialNonce++;
	}

	for (auto& worker : workers) {
		worker.join();
	}

	std::cout << finalBlock << '\n';
	std::cout << blockHash << '\n';
}

