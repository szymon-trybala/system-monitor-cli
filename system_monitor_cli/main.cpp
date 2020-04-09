#include "Cli.h"

#include "features/Memory.h"
#include "features/Processor.h"
#include "features/Storage.h"
#include "features/Network.h"
#include "features/System.h"

#include <iostream>

int main(int argc, char* argv[])
{
    Cli cli;

    std::string argument = cli.parse(argc, argv);
    if (argument.compare("ram") == 0) {
        Mem mem;
        auto info = mem.getMemoryInfo();
        for (auto& el : info) {
            std::cout << el << std::endl;
            exit(0);
        }
    }
    else if (argument.compare("cpu") == 0) {
        Processor cpu;
        std::cout << cpu.GetCPULoad() << std::endl;
        exit(0);
    }
    else if (argument.compare("net-adapter") == 0) {
        Network net;
        auto info = net.getNetworkSpeed();
        std::cout << std::get<0>(info) << std::endl;
        std::cout << std::get<1>(info) << std::endl;
    }
    else if (argument.compare("win") == 0) {
        System sys;
        std::cout << sys.getSystemVersion() << std::endl;
        exit(0);
    }
    else {
        std::cerr << "Wrong argument provided, program will stop" << std::endl;
        exit(-1);
    }
}
