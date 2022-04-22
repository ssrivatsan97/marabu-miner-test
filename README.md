### Installation

- Clone the repository and the init the submodules:  
`git clone --recurse-submodules https://github.com/ssrivatsan97/marabu-miner-test`  

- Install cmake to build, you will need a C++ compiler like gcc clang:  
`brew install cmake`  

- Create a build directory and run cmake:  
`mkdir build`  
`cd build`  
`cmake ..`  
`make`  

- Edit the number of blocks and the previd and txids of blocks in the file hash-rate.cpp

- Compile again by running `make`

- Run `./HashRate`