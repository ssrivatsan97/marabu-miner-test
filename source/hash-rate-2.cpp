#include "hex.h"
#include "hash.h"
#include "json.hpp"

#include <iostream>
#include <chrono>

std::string TEST_TARGET = "00000002af000000000000000000000000000000000000000000000000000000";

int main() {
	auto target = hashing::decodeHex(TEST_TARGET);
	int numBlocks = 5;
	nlohmann::json msgs [numBlocks];
	std::string previd = "00000000a420b7cefa2b7730243316921ed59ffe836e111ca3801f82a4f5360e";
	for (int i = 0; i < numBlocks; i++) {
		msgs[i]["T"] = "00000002af000000000000000000000000000000000000000000000000000000";
		msgs[i]["created"] = 1650650324705;
		msgs[i]["miner"] = "grader";
		msgs[i]["nonce"] = "0000000000000000000000000000000000000000000000000000002634878840";
		msgs[i]["type"] = "block";
	}	
	msgs[0]["note"] = "This block contains an invalid transaction";
	msgs[0]["txids"] =
        {"cd60a35605170d6fbad30843416d01e3e3716b39dbc3dc47962c920ad47a094a"
        ,"4ff754865b893700e1ec65805bbb64e5c4d276bae00085e786fe50bbf3152b48"
        };
	msgs[1]["note"] = "This block has a transaction spending the coinbase";
	msgs[1]["txids"] = {"cd60a35605170d6fbad30843416d01e3e3716b39dbc3dc47962c920ad47a094a","5dbf776d4227962fef8fc877143401196a98f401827b40b3f2971c8ed3fa8d9c"};
    msgs[2]["note"] = "This block has 2 coinbase transactions";
    msgs[2]["txids"] = {"cd60a35605170d6fbad30843416d01e3e3716b39dbc3dc47962c920ad47a094a","cd60a35605170d6fbad30843416d01e3e3716b39dbc3dc47962c920ad47a094a"};
    msgs[3]["note"] = "This block has a coinbase transaction";
    msgs[3]["txids"] = {"cd60a35605170d6fbad30843416d01e3e3716b39dbc3dc47962c920ad47a094a"};
    msgs[4]["note"] = "This block violates the law of conservation";
    msgs[4]["txids"] =
        {"ac6e325209d3120a7c60a3b8fee93e64a38253ff5b5b5df1431313f0abdbecbd",
        "5dbf776d4227962fef8fc877143401196a98f401827b40b3f2971c8ed3fa8d9c"};
	
	for (int i = 0; i < numBlocks; i++) {
		nlohmann::json msg = msgs[i];
		msg["previd"] = previd;
		Hex64 nonce;
		std::string hash;
		std::string dumped = msg.dump();
		std::string prefix = dumped.substr(0, dumped.find("\"nonce\":") + 9);
		std::string postfix = dumped.substr(dumped.find("\"nonce\":") + 64 + 9, dumped.size());
		for (auto& c : prefix) {
			if (c == '\'') {
				c = '"';
			}
		}
		for (auto& c : postfix) {
			if (c == '\'') {
				c = '"';
			}
		}
		nonce = Hex64();
		if ((prefix + "0000000000000000000000000000000000000000000000000000002634878840" + postfix)
			== dumped){ 
			std::cout << "Dumped == prefix + nonce + postfix" << '\n';
		}
		else {
			std::cout << "Dumped != prefix + nonce + postfix" << '\n';
		}
		auto cur = std::chrono::high_resolution_clock::now();
		while (true) {
			hash = hashing::SHA256(prefix + nonce.value() + postfix);
			if (hashing::compareBytes(hash, target)) {
				break;
			}
			nonce++;
		}
		auto after = std::chrono::high_resolution_clock::now();
		std::cout << "Time took to find a hash for target: " << TEST_TARGET
			<< " is: " << (after - cur).count() / 1000000000 << " seconds" << '\n';
		std::cout << "hash: " << hashing::encodeHex(hash) << '\n';
		std::cout << "block: \n" << prefix + nonce.value() + postfix << "\n";
        if (i = 3){
            previd = hashing::encodeHex(hash);
        }
	}
}
