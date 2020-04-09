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
        }
    }
    else if (argument.compare("cpu") == 0) {
        Processor cpu;
        std::cout << cpu.GetCPULoad() << std::endl;
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
    }
    else if (argument.compare("uptime") == 0) {
        System sys;
        auto uptime = sys.getUpTime();
        std::cout << std::get<0>(uptime) << std::endl;
        std::cout << std::get<1>(uptime) << std::endl;
        std::cout << std::get<2>(uptime) << std::endl;
        std::cout << std::get<3>(uptime) << std::endl;
    }
    else if (argument.compare("name") == 0) {
        System sys;
        std::cout << sys.getComputerName() << std::endl;
    }
    else if (argument.compare("disk-space") == 0) {
        Storage st;
        auto disk_space = st.getDiskSizeInfo();
        for (auto& x : disk_space) {
            std::cout << x << std::endl;
        }
    }
    else if (argument.compare("disk-type") == 0) {
        Storage st;
        std::cout << st.getDriveType() << std::endl;
    }
    else 
    {
        std::cerr << "Wrong argument provided, program will stop" << std::endl;
        exit(-1);

    }
    exit(0);
}
