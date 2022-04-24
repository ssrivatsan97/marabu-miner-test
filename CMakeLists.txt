cmake_minimum_required(VERSION 3.10)

project(MarabuMinerTest)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()
 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

include_directories(cryptopp)
include_directories(source)

file(GLOB CryptoPPSource
     "cryptopp/*.cpp"
)

add_library(CryptoPP ${CryptoPPSource})
add_library(Hashes source/hash.cpp)

add_executable(HashRate source/hash-rate.cpp)
target_link_libraries(HashRate Hashes)
target_link_libraries(HashRate CryptoPP)

add_executable(MinerStandalone source/miner.cpp)
target_link_libraries(MinerStandalone Hashes)
target_link_libraries(MinerStandalone CryptoPP)

